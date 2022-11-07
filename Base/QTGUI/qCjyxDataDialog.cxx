/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

/// Qt includes
#include <QDebug>
#include <QComboBox>
#include <QDropEvent>
#include <QFileDialog>
#include <QMainWindow>
#include <QMimeData>
#include <QMessageBox>
#include <QScrollBar>
#include <QTemporaryDir>

/// CTK includes
#include <ctkCheckableHeaderView.h>
#include <ctkCheckableModelHelper.h>

/// Cjyx includes
#include "vtkArchive.h"
#include "vtkMRMLApplicationLogic.h"

/// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxDataDialog_p.h"
#include "qCjyxIOManager.h"
#include "qCjyxIOOptionsWidget.h"
#include "vtkMRMLMessageCollection.h"

/// VTK includes
#include <vtkNew.h>

//-----------------------------------------------------------------------------
qCjyxDataDialogPrivate::qCjyxDataDialogPrivate(qCjyxDataDialog* object, QWidget* _parent/*=nullptr*/)
  :QDialog(_parent)
  ,q_ptr(object)
{
  this->setupUi(this);

  // Checkable headers.
  // We replace the current FileWidget header view with a checkable header view.
  // Checked files (rows) will be loaded into the scene, unchecked files will be
  // discarded.
  // In order to have a column checkable, we need to manually set a checkstate
  // to a column. No checkstate (null QVariant) means uncheckable.
  this->FileWidget->model()->setHeaderData(
    FileColumn, Qt::Horizontal, Qt::Unchecked, Qt::CheckStateRole);
  QHeaderView* previousHeaderView = this->FileWidget->horizontalHeader();
  ctkCheckableHeaderView* headerView = new ctkCheckableHeaderView(
    Qt::Horizontal, this->FileWidget);
  // Copy the previous behavior of the header into the new checkable header view
  headerView->setSectionsClickable(previousHeaderView->sectionsClickable());
  headerView->setSectionsMovable(previousHeaderView->sectionsMovable());
  headerView->setHighlightSections(previousHeaderView->highlightSections());
  headerView->setStretchLastSection(previousHeaderView->stretchLastSection());
  // Propagate to top-level items only (depth = 1),no need to go deeper
  // (depth = -1 or 2, 3...) as it is a flat list.
  headerView->checkableModelHelper()->setPropagateDepth(1);
  // Finally assign the new header to the view
  this->FileWidget->setHorizontalHeader(headerView);

  headerView->setStretchLastSection(false);
  headerView->setSectionResizeMode(FileColumn, QHeaderView::Stretch);
  headerView->setSectionResizeMode(TypeColumn, QHeaderView::ResizeToContents);
  headerView->setSectionResizeMode(OptionsColumn, QHeaderView::ResizeToContents);

  this->FileWidget->sortItems(-1, Qt::AscendingOrder);

  // Connect the "Options" button with the visibility of the "Options" column.
  connect(this->ShowOptionsCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(showOptions(bool)));
  // Hide the options by default;
  this->showOptions(false);

  connect(this->AddDirectoryButton, SIGNAL(clicked()), this, SLOT(addDirectory()));
  connect(this->AddFilesButton, SIGNAL(clicked()), this, SLOT(addFiles()));

  // Reset clears the FileWidget of all previously added files.
  QPushButton* resetButton = this->ButtonBox->button(QDialogButtonBox::Reset);
  connect(resetButton, SIGNAL(clicked()), this, SLOT(reset()));

  // Authorize Drops action from outside
  this->setAcceptDrops(true);

  // Set up button focus default action:
  // * Space bar: clicks the button where the focus is on (e.g., if a user added data from
  //   file and wants to add another data from file then he should press space bar)
  // * Enter key: loads the selected data (unless cancel button has the focus, that case
  //   enter key cancels loading). It is important that after add data button was clicked
  //   and add data button has the focus enter key still loads selected data instead of
  //   opening the add data window. This allows the user to load data then just hit Enter
  //   key to load data.

  // All buttons have strong focus, so after clicking/tabbing on them hitting space bar
  // clicks them. However, we need to prevent all push-buttons (other than OK and Cancel)
  // to become default buttons.
  // Default button is the one that is clicked when user hits Enter key.
  resetButton->setAutoDefault(false);
  resetButton->setDefault(false);
  this->AddDirectoryButton->setDefault(false);
  this->AddDirectoryButton->setAutoDefault(false);
  this->AddFilesButton->setDefault(false);
  this->AddFilesButton->setAutoDefault(false);
}

//-----------------------------------------------------------------------------
qCjyxDataDialogPrivate::~qCjyxDataDialogPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::addDirectory()
{
  qCjyxStandardFileDialog fileDialog;
  QString directory = fileDialog.getExistingDirectory();
  if (directory.isNull())
    {
    return;
    }
  bool sortingEnabled = this->FileWidget->isSortingEnabled();
  this->FileWidget->setSortingEnabled(false);
  this->addDirectory(directory);
  this->FileWidget->setSortingEnabled(sortingEnabled);
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::addFiles()
{
  qCjyxStandardFileDialog fileDialog;
  QStringList files = fileDialog.getOpenFileName();

  foreach(QString file, files)
    {
    this->addFile(file);
    }
  this->resetColumnWidths();
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::addDirectory(const QDir& directory)
{
  bool recursive = true;
  QDir::Filters filters =
    QDir::AllDirs | QDir::Files | QDir::Readable | QDir::NoDotAndDotDot;
  QFileInfoList fileInfoList = directory.entryInfoList(filters);

  //
  // check to see if any readers recognize the directory contents
  // and provide an archetype.
  //
  qCjyxCoreIOManager* coreIOManager =
    qCjyxCoreApplication::application()->coreIOManager();
  QString readerDescription;
  qCjyxIO::IOProperties ioProperties;
  QFileInfo archetypeEntry;
  if (coreIOManager->examineFileInfoList(fileInfoList, archetypeEntry, readerDescription, ioProperties))
    {
    this->addFile(archetypeEntry, readerDescription, &ioProperties);
    }

  //
  // now add any files and directories that weren't filtered
  // out by the ioManager
  //
  foreach(QFileInfo entry, fileInfoList)
    {
    if (entry.isFile())
      {
      this->addFile(entry);
      }
    else if (entry.isDir() && recursive)
      {
      this->addDirectory(entry.absoluteFilePath());
      }
    }
  this->resetColumnWidths();
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::addFile(const QFileInfo& file, const QString& readerDescription, qCjyxIO::IOProperties* ioProperties)
{
  if (!file.isFile() || !file.exists() || !file.isReadable())
    {
    return;
    }
  if (!this->FileWidget->findItems(file.absoluteFilePath(),
                                   Qt::MatchExactly).isEmpty())
    {
    return; // file already exists
    }


  //
  // check for archive, and optionally open it
  //
  if (this->checkAndHandleArchive(file))
    {
    return; // file was an archive
    }

  //
  // use the IOManager to check for ways to load the data
  //
  qCjyxCoreIOManager* coreIOManager =
    qCjyxCoreApplication::application()->coreIOManager();
  QStringList fileDescriptions;
  if (readerDescription.isEmpty())
    {
    fileDescriptions = coreIOManager->fileDescriptions(file.absoluteFilePath());
    }
  else
    {
    fileDescriptions << readerDescription;
    }
  if (fileDescriptions.isEmpty())
    {
    return;
    }

  //
  // add the file to the dialog
  //
  bool sortingEnabled = this->FileWidget->isSortingEnabled();
  this->FileWidget->setSortingEnabled(false);
  int row = this->FileWidget->rowCount();
  this->FileWidget->insertRow(row);
  // File name
  QTableWidgetItem *fileItem = new QTableWidgetItem(file.absoluteFilePath());
  fileItem->setFlags( (fileItem->flags() | Qt::ItemIsUserCheckable) & ~Qt::ItemIsEditable);
  fileItem->setCheckState(Qt::Checked);
  this->FileWidget->setItem(row, FileColumn, fileItem);
  // Description
  QComboBox* descriptionComboBox = new QComboBox(this->FileWidget);
  foreach(const QString& fileDescription, fileDescriptions)
    {
    descriptionComboBox->addItem(fileDescription,
                                 QVariant(coreIOManager->fileTypeFromDescription(fileDescription)));
    }
  // adding items to the combobox automatically selects the first item
  // let's select none, connect the signal and then selecting the first will
  // automatically create the option widget
  descriptionComboBox->setCurrentIndex(-1);
  QObject::connect(descriptionComboBox, SIGNAL(currentIndexChanged(QString)),
                   this, SLOT(onFileTypeChanged(QString)));
  QObject::connect(descriptionComboBox, SIGNAL(activated(QString)),
                   this, SLOT(onFileTypeActivated(QString)));
  this->FileWidget->setCellWidget(row, TypeColumn, descriptionComboBox);
  descriptionComboBox->setCurrentIndex(0);
  if (!readerDescription.isEmpty() && ioProperties != nullptr)
    {
    qCjyxIOOptionsWidget* ioOptionsWidget = dynamic_cast<qCjyxIOOptionsWidget*> (this->FileWidget->cellWidget(row, OptionsColumn));
    if (ioOptionsWidget)
      {
      ioOptionsWidget->updateGUI(*ioProperties);
      }
    }

  this->FileWidget->setSortingEnabled(sortingEnabled);
}

//---------------------------------------------------------------------------
void qCjyxDataDialogPrivate::dragEnterEvent(QDragEnterEvent* event)
{
  Q_Q(qCjyxDataDialog);
  if (event && q->isMimeDataAccepted(event->mimeData()))
    {
    event->accept();
    }
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::dropEvent(QDropEvent* event)
{
  Q_Q(qCjyxDataDialog);
  q->dropEvent(event);
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::reset()
{
  this->FileWidget->setRowCount(0);
  // forceFileColumnStretch=true to make the widgets nicely fill the available space
  this->resetColumnWidths(true);
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::resetColumnWidths(bool forceFileColumnStretch/*=false*/)
{
  bool optionsVisible = !this->FileWidget->isColumnHidden(OptionsColumn);

  QHeaderView* headerView = this->FileWidget->horizontalHeader();
  if (optionsVisible && !forceFileColumnStretch)
    {
    // Use as much space for the filename as needed (if it becomes too long then we reduce the size later)
    this->FileWidget->resizeColumnToContents(FileColumn);
    // Ensure there is enough space for options
    this->FileWidget->resizeColumnToContents(OptionsColumn);
    // Allow the user to squeeze the filename column (to make more space for options)
    headerView->setSectionResizeMode(FileColumn, QHeaderView::Interactive);

    // Ensure that the OptionsColumn is visible (at least up to a size of 60 pixels)
    int minimumOptionsColumnWidth = std::min(60, this->FileWidget->columnWidth(OptionsColumn));
    int maximumFileColumnWidth = this->FileWidget->width() - this->FileWidget->columnWidth(TypeColumn) - minimumOptionsColumnWidth;
    //int maximumFileColumnWidth = this->FileWidget->width() * 0.8;
    if (this->FileWidget->columnWidth(FileColumn) > maximumFileColumnWidth)
      {
      this->FileWidget->setColumnWidth(FileColumn, maximumFileColumnWidth);
      }
    }
  else
    {
    // Options section is hidden, use all the space for the filename
    headerView->setSectionResizeMode(FileColumn, QHeaderView::Stretch);
    }
}


//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::showOptions(bool show)
{
  this->ShowOptionsCheckBox->setChecked(show);
  this->FileWidget->setColumnHidden(OptionsColumn, !show);
  this->resetColumnWidths();
  if (show)
    {
    if (QApplication::instance())
      {
      QApplication::instance()->processEvents(QEventLoop::ExcludeUserInputEvents);
      }
    this->FileWidget->horizontalScrollBar()->setSliderPosition(
      this->FileWidget->horizontalScrollBar()->maximum());
    }
}

//-----------------------------------------------------------------------------
QList<qCjyxIO::IOProperties> qCjyxDataDialogPrivate::selectedFiles()const
{
  QList<qCjyxIO::IOProperties> files;
  for (int row = 0; row < this->FileWidget->rowCount(); ++row)
    {
    qCjyxIO::IOProperties properties;
    QTableWidgetItem* fileItem = this->FileWidget->item(row, FileColumn);
    QComboBox* descriptionComboBox =
      qobject_cast<QComboBox*>(this->FileWidget->cellWidget(row, TypeColumn));
    Q_ASSERT(fileItem);
    Q_ASSERT(descriptionComboBox);
    if (fileItem->checkState() != Qt::Checked)
      {
      continue;
      }
    // TBD: fileType is not good enough to describe what reader to use
    properties["fileType"] = descriptionComboBox->itemData(
      descriptionComboBox->currentIndex()).toString();
    qCjyxIOOptionsWidget* optionsItem = dynamic_cast<qCjyxIOOptionsWidget*>(
      this->FileWidget->cellWidget(row, OptionsColumn));
    if (optionsItem)
      {
      // The optionsItem contains all the file properties including "fileName"
      properties.unite(optionsItem->properties());
      }
    else
      {
      properties["fileName"] = fileItem->text();
      }
    files << properties;
    }
  return files;
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::onFileTypeChanged(const QString& description)
{
  int row = this->senderRow();
  QString fileName = this->FileWidget->item(row, FileColumn)->text();
  this->setFileOptions(row, fileName, description);
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::onFileTypeActivated(const QString& description)
{
  int activatedRow = this->senderRow();
  if (this->propagateChange(activatedRow))
    {
    for(int row = 0; row < this->FileWidget->rowCount(); ++row)
      {
      if (!this->haveSameTypeOption(activatedRow, row))
        {
        continue;
        }
      QComboBox* selectedComboBox = qobject_cast<QComboBox*>(
        this->FileWidget->cellWidget(row, TypeColumn));
      int descriptionIndex =
        selectedComboBox ? selectedComboBox->findText(description) : -1;
      qDebug() << "id" << descriptionIndex;
      if (descriptionIndex != -1)
        {
        selectedComboBox->setCurrentIndex(descriptionIndex);
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::setFileOptions(
  int row, const QString& fileName, const QString& fileDescription)
{
  qCjyxCoreIOManager* coreIOManager =
    qCjyxCoreApplication::application()->coreIOManager();
  // Options
  qCjyxIOOptions* options = coreIOManager->fileOptions(fileDescription);
  qCjyxIOOptionsWidget* optionsWidget =
    dynamic_cast<qCjyxIOOptionsWidget*>(options);
  if (optionsWidget)
    {
    // TODO: support uneven rows. Until that day, we want to make sure the whole
    // widget is visible
    optionsWidget->setMinimumWidth(optionsWidget->sizeHint().width());
    // The optionsWidget can use the filename to initialize some options.
    optionsWidget->setFileName(fileName);
    // TODO: connect signal validChanged(bool) with the accept button
    }
  else
    {
    delete options;
    }
  this->FileWidget->setCellWidget(row, OptionsColumn, optionsWidget);
  this->FileWidget->resizeColumnToContents(OptionsColumn);
}

//-----------------------------------------------------------------------------
int qCjyxDataDialogPrivate::senderRow()const
{
  QComboBox* comboBox = qobject_cast<QComboBox*>(this->sender());
  if (!comboBox)
    {
    qCritical() << "qCjyxDataDialogPrivate::onFileTypeChanged must be called"
                << "by a QComboBox signal";
    return -1;
    }
  int row = -1;
  for (int i = 0; i < this->FileWidget->rowCount(); ++i)
    {
    if (this->FileWidget->cellWidget(i, TypeColumn) == comboBox)
      {
      row = i;
      break;
      }
    }
  if (row < 0)
    {
    qCritical() << "Can't find the item to update";
    }
  return row;
}

//-----------------------------------------------------------------------------
bool qCjyxDataDialogPrivate::haveSameTypeOption(int row1, int row2)const
{
  QComboBox* comboBox1 = qobject_cast<QComboBox*>(
    this->FileWidget->cellWidget(row1, TypeColumn));
  QComboBox* comboBox2 = qobject_cast<QComboBox*>(
    this->FileWidget->cellWidget(row2, TypeColumn));
  if (!comboBox1 || !comboBox2)
    {
    return false;
    }
  if (comboBox1->count() != comboBox2->count())
    {
    return false;
    }
  for (int i=0; i < comboBox1->count(); ++i)
    {
    if (comboBox1->itemText(i) != comboBox2->itemText(i))
      {
      return false;
      }
    }
  return true;
}
//-----------------------------------------------------------------------------
bool qCjyxDataDialogPrivate::propagateChange(int changedRow)const
{
  QTableWidgetItem* item = this->FileWidget->item(changedRow, FileColumn);
  bool fileSelected = item ? item->checkState() != Qt::Unchecked : false;
  return fileSelected
    && (QApplication::keyboardModifiers() & Qt::ShiftModifier);
}

//-----------------------------------------------------------------------------
bool qCjyxDataDialogPrivate::checkAndHandleArchive(const QFileInfo& file)
{
  if (file.suffix().toLower() == "zip")
    {
    if (QMessageBox::question(this, tr("Open archive?"),
      tr("The selected file is a .zip archive, open it and load contents?")) == QMessageBox::Yes)
      {
      this->temporaryArchiveDirectory.reset(new QTemporaryDir());
      if (this->temporaryArchiveDirectory->isValid())
        {
        if (vtkArchive::UnZip(file.absoluteFilePath().toStdString().c_str(), this->temporaryArchiveDirectory->path().toStdString().c_str()))
          {
          this->addDirectory(QDir(this->temporaryArchiveDirectory->path()));
          return true;
          }
        }
      }
    }
  return false;
}

/*
//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::updateCheckBoxes(Qt::Orientation orientation, int first, int last)
{
  if (orientation != Qt::Horizontal ||
      first > FileColumn || last < FileColumn)
    {
    return;
    }
  Qt::CheckState headerState = this->FileWidget->horizontalHeaderItem(FileColumn)->checkState();
  if (headerState == Qt::PartiallyChecked)
    {
    return;
    }
  for (int row = 0; row < this->FileWidget->rowCount(); ++row)
    {
    this->FileWidget->item(row, FileColumn)->setCheckState(headerState);
    }
}

//-----------------------------------------------------------------------------
void qCjyxDataDialogPrivate::updateCheckBoxHeader(int itemRow, int itemColumn)
{
  if (itemColumn != FileColumn)
    {
    return;
    }
  Qt::CheckState headerCheckState = this->FileWidget->item(itemRow,itemColumn)->checkState();
  for (int row = 0; row < this->FileWidget->rowCount(); ++row)
    {
    if (this->FileWidget->item(row, FileColumn)->checkState() !=
        headerCheckState)
      {
      this->FileWidget->model()->setHeaderData(FileColumn, Qt::Horizontal, Qt::PartiallyChecked, Qt::CheckStateRole);
      return;
      }
    }
  this->FileWidget->model()->setHeaderData(FileColumn, Qt::Horizontal, headerCheckState, Qt::CheckStateRole);
}*/

//-----------------------------------------------------------------------------
qCjyxDataDialog::qCjyxDataDialog(QObject* _parent)
  : qCjyxFileDialog(_parent)
  , d_ptr(new qCjyxDataDialogPrivate(this, nullptr))
{
}

//-----------------------------------------------------------------------------
qCjyxDataDialog::~qCjyxDataDialog() = default;

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxDataDialog::fileType()const
{
  // FIXME: not really a scene file, but more a collection of files
  return QString("NoFile");
}

//-----------------------------------------------------------------------------
QString qCjyxDataDialog::description()const
{
  return tr("Any Data");
}

//-----------------------------------------------------------------------------
qCjyxFileDialog::IOAction qCjyxDataDialog::action()const
{
  return qCjyxFileDialog::Read;
}

//---------------------------------------------------------------------------
bool qCjyxDataDialog::isMimeDataAccepted(const QMimeData* mimeData)const
{
  return mimeData->hasFormat("text/uri-list");
}

//-----------------------------------------------------------------------------
void qCjyxDataDialog::dropEvent(QDropEvent *event)
{
  Q_D(qCjyxDataDialog);
  bool pathAdded = false;
  foreach(QUrl url, event->mimeData()->urls())
    {
    if (!url.isValid() || url.isEmpty())
      {
      continue;
      }

    QString localPath = url.toLocalFile(); // convert QUrl to local path
    QFileInfo pathInfo;
    pathInfo.setFile(localPath); // information about the path

    if (pathInfo.isDir()) // if it is a directory we add the files to the dialog
      {
      d->addDirectory(QDir(localPath));
      pathAdded = true;
      }
    else if (pathInfo.isFile()) // if it is a file we simply add the file
      {
      d->addFile(pathInfo);
      pathAdded = true;
      }
    }
  if (pathAdded)
    {
    event->acceptProposedAction();
    }
}

//-----------------------------------------------------------------------------
bool qCjyxDataDialog::exec(const qCjyxIO::IOProperties& readerProperties)
{
  Q_D(qCjyxDataDialog);
  Q_ASSERT(!readerProperties.contains("fileName"));
  if (readerProperties.contains("fileNames"))
    {
    QStringList fileNames = readerProperties["fileNames"].toStringList();
    foreach(QString fileName, fileNames)
      {
      d->addFile(QFileInfo(fileName));
      }
    }
  d->resetColumnWidths();

  bool success = false;

  Qt::WindowFlags windowFlags = d->windowFlags();
  qCjyxApplication* app = qCjyxApplication::application();
  QWidget* mainWindow = app ? app->mainWindow() : nullptr;
  if (mainWindow)
    {
    // setParent resets window flags, so save them and then restore
    Qt::WindowFlags windowFlags = d->windowFlags();
    d->setParent(mainWindow);
    d->setWindowFlags(windowFlags);
    }
  int result = d->exec();
  if (mainWindow)
    {
    d->setParent(nullptr);
    d->setWindowFlags(windowFlags);
    }

  if (result != QDialog::Accepted)
    {
    d->reset();
    return success;
    }
  QList<qCjyxIO::IOProperties> files = d->selectedFiles();
  for (int i = 0; i < files.count(); ++i)
    {
    files[i].unite(readerProperties);
    }
  vtkNew<vtkMRMLMessageCollection> userMessages;
  success = qCjyxCoreApplication::application()->coreIOManager()->loadNodes(files, nullptr, userMessages);
  qCjyxIOManager::showLoadNodesResultDialog(success, userMessages);
  d->reset();
  return success;
}

//-----------------------------------------------------------------------------
void qCjyxDataDialog::addFile(const QString filePath)
{
  Q_D(qCjyxDataDialog);
  d->addFile(QFileInfo(filePath));
}

//-----------------------------------------------------------------------------
void qCjyxDataDialog::addDirectory(const QString directoryPath)
{
  Q_D(qCjyxDataDialog);
  d->addDirectory(QDir(directoryPath));
}
