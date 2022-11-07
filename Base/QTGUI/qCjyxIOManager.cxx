/// Qt includes
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMetaProperty>
#include <QProgressDialog>
#include <QSettings>
#include <QTimer>

// CTK includes
#include <ctkMessageBox.h>
#include <ctkScreenshotDialog.h>

/// Cjyx includes
#include "qCjyxIOManager.h"
#include "qCjyxDataDialog.h"
#include "qCjyxModelsDialog.h"
#include "qCjyxSaveDataDialog.h"
#include "qCjyxApplication.h"
#include "qCjyxCoreApplication.h"
#include "qCjyxLayoutManager.h"
#include "qCjyxModuleManager.h"
#include "qCjyxAbstractCoreModule.h"

/// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLMessageCollection.h>

/// VTK includes
#include <vtkCollection.h>

//-----------------------------------------------------------------------------
class qCjyxIOManagerPrivate
{
  Q_DECLARE_PUBLIC(qCjyxIOManager);

protected:
  qCjyxIOManager* const q_ptr;

public:
  qCjyxIOManagerPrivate(qCjyxIOManager& object);

  vtkMRMLScene* currentScene()const;

  /// Return true if a dialog is created, false if a dialog already existed
  bool startProgressDialog(int steps = 1);
  void stopProgressDialog();
  void readSettings();
  void writeSettings();
  QString createUniqueDialogName(qCjyxIO::IOFileType,
                                 qCjyxFileDialog::IOAction,
                                 const qCjyxIO::IOProperties&);

  qCjyxFileDialog* findDialog(qCjyxIO::IOFileType fileType,
                                qCjyxFileDialog::IOAction)const;

  QStringList History;
  QList<QUrl> Favorites;
  QList<qCjyxFileDialog*> ReadDialogs;
  QList<qCjyxFileDialog*> WriteDialogs;

  QSharedPointer<ctkScreenshotDialog> ScreenshotDialog;
  QProgressDialog* ProgressDialog{nullptr};

  /// File dialog that next call of execDelayedFileDialog signal will execute
  qCjyxFileDialog* DelayedFileDialog{nullptr};
};

//-----------------------------------------------------------------------------
// qCjyxIOManagerPrivate methods

//-----------------------------------------------------------------------------
qCjyxIOManagerPrivate::qCjyxIOManagerPrivate(qCjyxIOManager& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
vtkMRMLScene* qCjyxIOManagerPrivate::currentScene()const
{
  return qCjyxCoreApplication::application()->mrmlScene();
}

//-----------------------------------------------------------------------------
bool qCjyxIOManagerPrivate::startProgressDialog(int steps)
{
  Q_Q(qCjyxIOManager);
  if (this->ProgressDialog)
    {
    return false;
    }
  int max = (steps != 1 ? steps : 100);
  this->ProgressDialog = new QProgressDialog("Loading file... ", "Cancel", 0, max);
  this->ProgressDialog->setWindowTitle(QString("Loading ..."));
  if (steps == 1)
    {
    // We only support cancelling a load action if we can have control over it
    this->ProgressDialog->setCancelButton(nullptr);
    }
  this->ProgressDialog->setWindowModality(Qt::WindowModal);
  this->ProgressDialog->setMinimumDuration(1000);
  this->ProgressDialog->setValue(0);

  if (steps == 1)
    {
    q->qvtkConnect(qCjyxCoreApplication::application()->mrmlScene(),
                    vtkMRMLScene::NodeAddedEvent,
                    q, SLOT(updateProgressDialog()));
    }
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxIOManagerPrivate::stopProgressDialog()
{
  Q_Q(qCjyxIOManager);
  if (!this->ProgressDialog)
    {
    return;
    }
  this->ProgressDialog->setValue(this->ProgressDialog->maximum());

  q->qvtkDisconnect(qCjyxCoreApplication::application()->mrmlScene(),
                    vtkMRMLScene::NodeAddedEvent,
                    q, SLOT(updateProgressDialog()));
  delete this->ProgressDialog;
  this->ProgressDialog = nullptr;
}

//-----------------------------------------------------------------------------
void qCjyxIOManagerPrivate::readSettings()
{
  QSettings settings;
  settings.beginGroup("ioManager");

  if (!settings.value("favoritesPaths").toList().isEmpty())
    {
    QStringList paths = qCjyxCoreApplication::application()->toCjyxHomeAbsolutePaths(settings.value("favoritesPaths").toStringList());
    foreach (const QString& varUrl, paths)
      {
      this->Favorites << QUrl(varUrl);
      }
    }
  else
    {
    this->Favorites << QUrl::fromLocalFile(QDir::homePath());
    }

  settings.endGroup();
}

//-----------------------------------------------------------------------------
void qCjyxIOManagerPrivate::writeSettings()
{
  Q_Q(qCjyxIOManager);
  QSettings settings;
  settings.beginGroup("ioManager");
  QStringList list;
  foreach (const QUrl& url, q->favorites())
    {
    list << url.toString();
    }
  QStringList paths = qCjyxCoreApplication::application()->toCjyxHomeRelativePaths(list);
  settings.setValue("favoritesPaths", QVariant(paths));
  settings.endGroup();
}

//-----------------------------------------------------------------------------
QString qCjyxIOManagerPrivate::
createUniqueDialogName(qCjyxIO::IOFileType fileType,
                       qCjyxFileDialog::IOAction action,
                       const qCjyxIO::IOProperties& ioProperties)
{
  QString objectName;

  objectName += action == qCjyxFileDialog::Read ? "Add" : "Save";
  objectName += fileType;
  objectName += ioProperties["multipleFiles"].toBool() ? "s" : "";
  objectName += "Dialog";

  return objectName;
}

//-----------------------------------------------------------------------------
qCjyxFileDialog* qCjyxIOManagerPrivate
::findDialog(qCjyxIO::IOFileType fileType,
             qCjyxFileDialog::IOAction action)const
{
  const QList<qCjyxFileDialog*>& dialogs =
    (action == qCjyxFileDialog::Read)? this->ReadDialogs : this->WriteDialogs;
  foreach(qCjyxFileDialog* dialog, dialogs)
    {
    if (dialog->fileType() == fileType)
      {
      return dialog;
      }
    }
  return nullptr;
}

//-----------------------------------------------------------------------------
// qCjyxIOManager methods

//-----------------------------------------------------------------------------
qCjyxIOManager::qCjyxIOManager(QObject* _parent):Superclass(_parent)
  , d_ptr(new qCjyxIOManagerPrivate(*this))
{
  Q_D(qCjyxIOManager);
  d->readSettings();
}

//-----------------------------------------------------------------------------
qCjyxIOManager::~qCjyxIOManager()
{
  Q_D(qCjyxIOManager);
  d->writeSettings();
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openLoadSceneDialog()
{
  qCjyxIO::IOProperties properties;
  properties["clear"] = true;
  return this->openDialog(QString("SceneFile"), qCjyxFileDialog::Read, properties);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddSceneDialog()
{
  qCjyxIO::IOProperties properties;
  properties["clear"] = false;
  return this->openDialog(QString("SceneFile"), qCjyxFileDialog::Read, properties);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openDialog(qCjyxIO::IOFileType fileType,
                                  qCjyxFileDialog::IOAction action,
                                  qCjyxIO::IOProperties properties,
                                  vtkCollection* loadedNodes)
{
  Q_D(qCjyxIOManager);
  bool deleteDialog = false;
  if (properties["objectName"].toString().isEmpty())
    {
    QString name = d->createUniqueDialogName(fileType, action, properties);
    properties["objectName"] = name;
    }
  qCjyxFileDialog* dialog = d->findDialog(fileType, action);
  if (dialog == nullptr)
    {
    deleteDialog = true;
    qCjyxStandardFileDialog* standardDialog =
      new qCjyxStandardFileDialog(this);
    standardDialog->setFileType(fileType);
    standardDialog->setAction(action);
    dialog = standardDialog;
    }
  bool res = dialog->exec(properties);
  if (loadedNodes)
    {
    foreach(const QString& nodeID, dialog->loadedNodes())
      {
      vtkMRMLNode* node = d->currentScene()->GetNodeByID(nodeID.toUtf8());
      if (node)
        {
        loadedNodes->AddItem(node);
        }
      }
    }
  if (deleteDialog)
   {
    delete dialog;
    }
  return res;
}

//---------------------------------------------------------------------------
void qCjyxIOManager::dragEnterEvent(QDragEnterEvent *event)
{
  Q_D(qCjyxIOManager);
  foreach(qCjyxFileDialog* dialog, d->ReadDialogs)
    {
    if (dialog->isMimeDataAccepted(event->mimeData()))
      {
      event->accept();
      break;
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::dropEvent(QDropEvent *event)
{
  Q_D(qCjyxIOManager);
  QStringList supportedReaders;
  QStringList genericReaders; // those must be last in the choice menu
  foreach(qCjyxFileDialog* dialog, d->ReadDialogs)
    {
    if (dialog->isMimeDataAccepted(event->mimeData()))
      {
      QString supportedReader = dialog->description();
      if (dialog->fileType() == "NoFile")
        {
        genericReaders << supportedReader;
        }
      else
        {
        supportedReaders << supportedReader;
        }
      }
    }
  supportedReaders << genericReaders;
  QString selectedReader;
  if (supportedReaders.size() > 1)
    {
    QString title = tr("Select a reader");
    QString label = tr("Select a reader to use for your data?");
    int current = 0;
    bool editable = false;
    bool ok = false;
    selectedReader = QInputDialog::getItem(nullptr, title, label, supportedReaders, current, editable, &ok);
    if (!ok)
      {
      selectedReader = QString();
      }
    }
  else if (supportedReaders.size() ==1)
    {
    selectedReader = supportedReaders[0];
    }
  if (selectedReader.isEmpty())
    {
    return;
    }
  foreach(qCjyxFileDialog* dialog, d->ReadDialogs)
    {
    if (dialog->description() == selectedReader)
      {
      dialog->dropEvent(event);
      if (event->isAccepted())
        {
        // If we immediately executed the dialog here then we would block the application
        // that initiated the drag-and-drop operation. Therefore we return and open the dialog
        // with a timer.
        d->DelayedFileDialog = dialog;
        QTimer::singleShot(0, this, SLOT(execDelayedFileDialog()));
        break;
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::execDelayedFileDialog()
{
  Q_D(qCjyxIOManager);
  if (d->DelayedFileDialog)
    {
    d->DelayedFileDialog->exec();
    }
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::addHistory(const QString& path)
{
  Q_D(qCjyxIOManager);
  d->History << path;
}

//-----------------------------------------------------------------------------
const QStringList& qCjyxIOManager::history()const
{
  Q_D(const qCjyxIOManager);
  return d->History;
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::setFavorites(const QList<QUrl>& urls)
{
  Q_D(qCjyxIOManager);
  QList<QUrl> newFavorites;
  foreach(const QUrl& url, urls)
    {
    newFavorites << url;
    }
  d->Favorites = newFavorites;
}

//-----------------------------------------------------------------------------
const QList<QUrl>& qCjyxIOManager::favorites()const
{
  Q_D(const qCjyxIOManager);
  return d->Favorites;
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::registerDialog(qCjyxFileDialog* dialog)
{
  Q_D(qCjyxIOManager);
  Q_ASSERT(dialog);
  qCjyxFileDialog* existingDialog =
    d->findDialog(dialog->fileType(), dialog->action());
  if (existingDialog)
    {
    d->ReadDialogs.removeAll(existingDialog);
    d->WriteDialogs.removeAll(existingDialog);
    existingDialog->deleteLater();
    }
  if (dialog->action() == qCjyxFileDialog::Read)
    {
    d->ReadDialogs.prepend(dialog);
    }
  else if (dialog->action() == qCjyxFileDialog::Write)
    {
    d->WriteDialogs.prepend(dialog);
    }
  else
    {
    Q_ASSERT(dialog->action() == qCjyxFileDialog::Read ||
             dialog->action() == qCjyxFileDialog::Write);
    }
  dialog->setParent(this);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::loadNodes(const qCjyxIO::IOFileType& fileType,
  const qCjyxIO::IOProperties& parameters, vtkCollection* loadedNodes,
  vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  Q_D(qCjyxIOManager);
  // speed up data loading by disabling re-rendering
  // (it can make a big difference if hundreds of nodes are loaded)
  CjyxRenderBlocker renderBlocker;
  bool needStop = d->startProgressDialog(1);
  d->ProgressDialog->setLabelText(
    "Loading file " + parameters.value("fileName").toString() + " ...");
  if (needStop)
    {
    d->ProgressDialog->setValue(25);
    }

  bool res = this->qCjyxCoreIOManager::loadNodes(fileType, parameters, loadedNodes, userMessages);
  if (needStop)
    {
    d->stopProgressDialog();
    }
  return res;
}


//-----------------------------------------------------------------------------
bool qCjyxIOManager::loadNodes(const QList<qCjyxIO::IOProperties>& files,
  vtkCollection* loadedNodes, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  Q_D(qCjyxIOManager);
  // Speed up data loading by disabling re-rendering
  // (it can make a big difference if hundreds of nodes are loaded)
  CjyxRenderBlocker renderBlocker;
  bool needStop = d->startProgressDialog(files.count());
  bool success = true;
  foreach(qCjyxIO::IOProperties fileProperties, files)
    {
    int numberOfUserMessagesBefore = userMessages ? userMessages->GetNumberOfMessages() : 0;
    success = this->loadNodes(
      static_cast<qCjyxIO::IOFileType>(fileProperties["fileType"].toString()),
      fileProperties,
      loadedNodes, userMessages) && success;
    if (userMessages && userMessages->GetNumberOfMessages() > numberOfUserMessagesBefore)
      {
      userMessages->AddSeparator();
      }

    this->updateProgressDialog();
    if (d->ProgressDialog->wasCanceled())
      {
      success = false;
      break;
      }
    }

  if (needStop)
    {
    d->stopProgressDialog();
    }
  return success;
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::updateProgressDialog()
{
  Q_D(qCjyxIOManager);
  if (!d->ProgressDialog)
    {
    return;
    }
  int progress = d->ProgressDialog->value();
  d->ProgressDialog->setValue(qMin(progress + 1, d->ProgressDialog->maximum() - 1) );
  // Give time to process graphic events including the progress dialog if needed
  // TBD: Not needed ?
  //qApp->processEvents();
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::openScreenshotDialog()
{
  Q_D(qCjyxIOManager);
  // try opening the Annotation module's screen shot
  qCjyxModuleManager *moduleManager = qCjyxApplication::application()->moduleManager();

  qCjyxAbstractCoreModule *modulePointer = nullptr;
  if (moduleManager)
    {
    modulePointer = moduleManager->module("Annotations");
    }
  if (modulePointer)
    {
    QMetaObject::invokeMethod(modulePointer, "showScreenshotDialog");
    }
  else
    {
    qWarning() << "qCjyxIOManager::openScreenshotDialog: Unable to get Annotations module (annotations), using the CTK screen shot dialog.";
    // use the ctk one
    if (!d->ScreenshotDialog)
      {
      d->ScreenshotDialog = QSharedPointer<ctkScreenshotDialog>(
        new ctkScreenshotDialog());
      d->ScreenshotDialog->setWidgetToGrab(
        qCjyxApplication::application()->layoutManager()->viewport());
      }
    d->ScreenshotDialog->show();
    }
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::openSceneViewsDialog()
{
//  Q_D(qCjyxIOManager);
  qCjyxModuleManager *moduleManager = qCjyxApplication::application()->moduleManager();
  if (!moduleManager)
    {
    qWarning() << "qCjyxIOManager::openSceneViewsDialog: unable to get module manager, can't get at the Scene Views module";
    return;
    }

  qCjyxAbstractCoreModule *modulePointer = moduleManager->module("SceneViews");
  if (modulePointer == nullptr)
    {
    qWarning() << "qCjyxIOManager::openSceneViewsDialog: Unable to get at the SceneViews module (sceneviews).";
    return;
    }
  QMetaObject::invokeMethod(modulePointer, "showSceneViewDialog");
}

//-----------------------------------------------------------------------------
void qCjyxIOManager::showLoadNodesResultDialog(bool overallSuccess, vtkMRMLMessageCollection* userMessages)
{
  bool isTestingEnabled = qCjyxApplication::testAttribute(qCjyxCoreApplication::AA_EnableTesting);
  if (isTestingEnabled)
    {
    // Do not block the execution with popup windows if testing mode is enabled.
    return;
    }
  if (overallSuccess
    && userMessages->GetNumberOfMessagesOfType(vtkCommand::WarningEvent) == 0
    && userMessages->GetNumberOfMessagesOfType(vtkCommand::ErrorEvent) == 0)
    {
    // Everything is OK, no need to show error popup.
    return;
    }
  qCjyxApplication* app = qCjyxApplication::application();
  QWidget* mainWindow = app ? app->mainWindow() : nullptr;
  ctkMessageBox* messageBox = new ctkMessageBox(mainWindow);
  QString text;
  if (overallSuccess)
    {
    messageBox->setWindowTitle(tr("Adding data succeeded"));
    messageBox->setIcon(QMessageBox::Information);
    text = tr("The selected files were loaded successfully but errors or warnings were reported.");
    }
  else
    {
    messageBox->setWindowTitle(tr("Adding data failed"));
    messageBox->setIcon(QMessageBox::Critical);
    text = tr("Error occurred while loading the selected files.");
    }
  text += "\n";
  if (userMessages)
    {
    text += tr("Click 'Show details' button and check the application log for more information.");
    QString messagesStr = QString::fromStdString(userMessages->GetAllMessagesAsString());
    messageBox->setDetailedText(messagesStr);
    qWarning() << Q_FUNC_INFO << "Errors occurred while loading nodes:" << messagesStr;
    }
  else
    {
    text += tr("Check the application log for more information.");
    }
  messageBox->setText(text);
  messageBox->exec();
  messageBox->deleteLater();
}
