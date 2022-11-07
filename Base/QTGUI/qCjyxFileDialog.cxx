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

// Qt includes
#include <QDebug>
#include <QMainWindow>

// CTK includes
#include <ctkFileDialog.h>
#include <ctkUtils.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxFileReader.h"
#include "qCjyxIOManager.h"
#include "qCjyxIOOptionsWidget.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkCollection.h>
#include <vtkNew.h>

/*
//-----------------------------------------------------------------------------
class qCjyxFileDialogPrivate
{
public:
};
*/

//-----------------------------------------------------------------------------
qCjyxFileDialog::qCjyxFileDialog(QObject* _parent)
  :QObject(_parent)
{
  qRegisterMetaType<qCjyxFileDialog::IOAction>("qCjyxFileDialog::IOAction");
}

//-----------------------------------------------------------------------------
qCjyxFileDialog::~qCjyxFileDialog() = default;

//-----------------------------------------------------------------------------
QStringList qCjyxFileDialog::nameFilters(qCjyxIO::IOFileType fileType)
{
  QStringList filters;
  QStringList extensions;
  QList<qCjyxFileReader*> readers =
    qCjyxApplication::application()->ioManager()->readers(fileType);
  foreach(const qCjyxFileReader* reader, readers)
    {
    foreach(const QString& filter, reader->extensions())
      {
      QString nameFilter = filter.contains('(') ? filter :
        reader->description() + " (" + filter + ")";
      filters << nameFilter;
      extensions << ctk::nameFilterToExtensions(nameFilter);
      }
    }
  filters.insert(0, QString("All (") + extensions.join(" ") + QString(")"));
  return filters;
}

//-----------------------------------------------------------------------------
bool qCjyxFileDialog::isMimeDataAccepted(const QMimeData *mimeData)const
{
  Q_UNUSED(mimeData);
  return false;
}

//-----------------------------------------------------------------------------
void qCjyxFileDialog::dropEvent(QDropEvent *event)
{
  Q_UNUSED(event);
}

//-----------------------------------------------------------------------------
QStringList qCjyxFileDialog::loadedNodes()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
class qCjyxStandardFileDialogPrivate
{
public:
  qCjyxStandardFileDialogPrivate();
  qCjyxIO::IOFileType       FileType;
  QString                     Description;
  qCjyxFileDialog::IOAction Action;
  QStringList                 LoadedNodes;
};

//-----------------------------------------------------------------------------
qCjyxStandardFileDialogPrivate::qCjyxStandardFileDialogPrivate()
{
  this->FileType = QString("NoFile");
  this->Action = qCjyxFileDialog::Read;
}

//-----------------------------------------------------------------------------
qCjyxStandardFileDialog::qCjyxStandardFileDialog(QObject* _parent)
  :qCjyxFileDialog(_parent)
  , d_ptr(new qCjyxStandardFileDialogPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxStandardFileDialog::~qCjyxStandardFileDialog() = default;

//-----------------------------------------------------------------------------
void qCjyxStandardFileDialog::setFileType(qCjyxIO::IOFileType _fileType)
{
  Q_D(qCjyxStandardFileDialog);
  d->FileType = _fileType;
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxStandardFileDialog::fileType()const
{
  Q_D(const qCjyxStandardFileDialog);
  return d->FileType;
}

//-----------------------------------------------------------------------------
void qCjyxStandardFileDialog::setDescription(const QString& description)
{
  Q_D(qCjyxStandardFileDialog);
  d->Description = description;
}

//-----------------------------------------------------------------------------
QString qCjyxStandardFileDialog::description()const
{
  Q_D(const qCjyxStandardFileDialog);
  return d->Description;
}

//-----------------------------------------------------------------------------
void qCjyxStandardFileDialog::setAction(qCjyxFileDialog::IOAction dialogAction)
{
  Q_D(qCjyxStandardFileDialog);
  d->Action = dialogAction;
}

//-----------------------------------------------------------------------------
qCjyxFileDialog::IOAction qCjyxStandardFileDialog::action()const
{
  Q_D(const qCjyxStandardFileDialog);
  return d->Action;
}

//-----------------------------------------------------------------------------
QStringList qCjyxStandardFileDialog::loadedNodes()const
{
  Q_D(const qCjyxStandardFileDialog);
  return d->LoadedNodes;
}

//-----------------------------------------------------------------------------
ctkFileDialog* qCjyxStandardFileDialog::createFileDialog(
    const qCjyxIO::IOProperties& ioProperties, QWidget* parent/*=nullptr*/)
{
  if(ioProperties["objectName"].toString().isEmpty())
    {
    qWarning() << "Impossible to create the ctkFileDialog - No object name has been set";
    return nullptr;
    }

  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  ctkFileDialog* fileDialog = new ctkFileDialog(parent);

  if(ioProperties["fileType"].toBool())
    {
    fileDialog->setNameFilters(
      qCjyxFileDialog::nameFilters(
        (qCjyxIO::IOFileType)ioProperties["fileType"].toString()));
    }
  fileDialog->setHistory(ioManager->history());
  if (ioManager->favorites().count())
    {
    fileDialog->setSidebarUrls(ioManager->favorites());
    }
#ifdef Q_OS_MAC
  // Workaround for Mac to show mounted volumes.
  // See issue #2240
  QList<QUrl> sidebarUrls = fileDialog->sidebarUrls();
  sidebarUrls.append(QUrl::fromLocalFile("/Volumes"));
  fileDialog->setSidebarUrls(sidebarUrls);
#endif
  if (ioProperties["multipleFiles"].toBool())
    {
    fileDialog->setFileMode(QFileDialog::ExistingFiles);
    }
  if (ioProperties["fileMode"].toBool())
    {
    fileDialog->setOption(QFileDialog::ShowDirsOnly);
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);
    }

  fileDialog->setObjectName(ioProperties["objectName"].toString());

  return fileDialog;
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxStandardFileDialog
::options(const qCjyxIO::IOProperties& ioProperties)const
{
  Q_D(const qCjyxStandardFileDialog);
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  // warning: we are responsible for the memory of options
  qCjyxIOOptions* options = nullptr;
  if (d->Action == qCjyxFileDialog::Read)
    {
    QStringList fileDescriptions = ioManager->fileDescriptionsByType(this->fileType());
    options = fileDescriptions.count() ?
      ioManager->fileOptions(fileDescriptions[0]) : nullptr;
    }
  else if (d->Action == qCjyxFileDialog::Write)
    {
    vtkMRMLScene* scene = qCjyxCoreApplication::application()->mrmlScene();
    vtkMRMLNode* nodeToSave = nullptr;
    if (!ioProperties["nodeID"].toString().isEmpty())
      {
      nodeToSave = scene->GetNodeByID(ioProperties["nodeID"].toString().toUtf8());
      }
    QStringList fileDescriptions =
      ioManager->fileWriterDescriptions(this->fileType());
    options = fileDescriptions.count() ?
      ioManager->fileWriterOptions(nodeToSave, fileDescriptions[0]) : nullptr;
    }
  // Update options widget based on properties.
  // This allows customizing default settings in the widget. For example,
  // in scene open dialog, Clear scene option can be set to enabled or disabled by default.
  qCjyxIOOptionsWidget* optionsWidget = dynamic_cast<qCjyxIOOptionsWidget*>(options);
  if (optionsWidget)
    {
    optionsWidget->updateGUI(ioProperties);
    }
  return options;
}

//-----------------------------------------------------------------------------
bool qCjyxStandardFileDialog::exec(const qCjyxIO::IOProperties& ioProperties)
{
  Q_D(qCjyxStandardFileDialog);
  Q_ASSERT(!ioProperties.contains("fileName"));

  d->LoadedNodes.clear();

  qCjyxIO::IOProperties properties = ioProperties;
  properties["fileType"] = d->FileType;
  qCjyxApplication* app = qCjyxApplication::application();
  QWidget* mainWindow = app ? app->mainWindow() : nullptr;
  ctkFileDialog* fileDialog = qCjyxStandardFileDialog::createFileDialog(
                                properties, mainWindow);
  QFileDialog::AcceptMode  acceptMode = (d->Action == qCjyxFileDialog::Read) ?
    QFileDialog::AcceptOpen : QFileDialog::AcceptSave;
  fileDialog->setAcceptMode(acceptMode);

  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();

  qCjyxIOOptions* options = this->options(properties);
  // warning: we are responsible for the memory of options
  qCjyxIOOptionsWidget* optionsWidget =
    dynamic_cast<qCjyxIOOptionsWidget*>(options);
  // options is not necessary a qCjyxIOOptionsWidget (for the case of
  // readers/modules with no UI. If there is a UI then add it inside the file
  // dialog.
  if (optionsWidget)
    {
    // fileDialog will reparent optionsWidget and take care of deleting
    // optionsWidget for us.
    fileDialog->setBottomWidget(optionsWidget, tr("Options:"));
    connect(fileDialog, SIGNAL(fileSelectionChanged(QStringList)),
            optionsWidget, SLOT(setFileNames(QStringList)));
    connect(optionsWidget, SIGNAL(validChanged(bool)),
            fileDialog, SLOT(setAcceptButtonEnable(bool)));
    fileDialog->setAcceptButtonEnable(optionsWidget->isValid());
    }

  if (ioProperties.contains("defaultFileName"))
    {
    fileDialog->selectFile(ioProperties["defaultFileName"].toString());
    }

  // we do not delete options now as it is still useful later (even if there is
  // no UI.) they are the options of the reader, UI or not.
  bool res = fileDialog->exec();
  if (res)
    {
    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
    properties = ioProperties;
    if (options)
      {
      properties.unite(options->properties());
      }
    properties["fileName"] = fileDialog->selectedFiles();
    if (d->Action == qCjyxFileDialog::Read)
      {
      vtkNew<vtkCollection> loadedNodes;
      ioManager->loadNodes(this->fileType(), properties, loadedNodes.GetPointer());
      for (int i = 0; i < loadedNodes->GetNumberOfItems();++i)
        {
        vtkMRMLNode* node = vtkMRMLNode::SafeDownCast(loadedNodes->GetItemAsObject(i));
        if (node)
          {
          d->LoadedNodes << node->GetID();
          }
        }
      res = !d->LoadedNodes.isEmpty();
      }
    else if(d->Action == qCjyxFileDialog::Write)
      {
      res = ioManager->saveNodes(this->fileType(), properties);
      }
    else
      {
      res = false;
      Q_ASSERT(d->Action == qCjyxFileDialog::Read ||
               d->Action == qCjyxFileDialog::Write);
      }
    QApplication::restoreOverrideCursor();
    }

  ioManager->setFavorites(fileDialog->sidebarUrls());

  // If options is not a qCjyxIOOptionsWidget, we are responsible for
  // deleting options. If it is, then fileDialog would have reparent
  // the options and take care of its destruction
  if (!optionsWidget)
    {
    delete options;
    options = nullptr;
    }

  fileDialog->deleteLater();
  return res;
}

//-----------------------------------------------------------------------------
QStringList qCjyxStandardFileDialog::getOpenFileName(
    qCjyxIO::IOProperties ioProperties)
{
  QStringList files;
  ioProperties["multipleFiles"] = QFileDialog::ExistingFiles;
  ioProperties["objectName"] = "getOpenFileName";
  qCjyxApplication* app = qCjyxApplication::application();
  QWidget* mainWindow = app ? app->mainWindow() : nullptr;
  ctkFileDialog* fileDialog = qCjyxStandardFileDialog::createFileDialog(
                                ioProperties, mainWindow);
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();

  if(fileDialog->exec() == QDialog::Accepted)
    {
    files = fileDialog->selectedFiles();
    }
  ioManager->setFavorites(fileDialog->sidebarUrls());
  fileDialog->deleteLater();
  return files;
}

//-----------------------------------------------------------------------------
QString qCjyxStandardFileDialog::getExistingDirectory(
    qCjyxIO::IOProperties ioProperties)
{
  QString directory;
  ioProperties["fileMode"] = QFileDialog::Directory;
  ioProperties["objectName"] = "getExistingDirectory";
  qCjyxApplication* app = qCjyxApplication::application();
  QWidget* mainWindow = app ? app->mainWindow() : nullptr;
  ctkFileDialog* fileDialog = qCjyxStandardFileDialog::createFileDialog(
                                ioProperties, mainWindow);
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();

  if (fileDialog->exec() == QDialog::Accepted)
    {
    directory = fileDialog->selectedFiles().value(0);
    }
  ioManager->setFavorites(fileDialog->sidebarUrls());
  fileDialog->deleteLater();
  return directory;
}
