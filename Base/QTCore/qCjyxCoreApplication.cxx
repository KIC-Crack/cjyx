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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// standard library includes
#include  <clocale>

// Qt includes
#include <QDebug>
#include <QDir>
#include <QLocale>
#include <QMessageBox>
#include <QTimer>
#include <QNetworkProxyFactory>
#include <QResource>
#include <QSettings>
#include <QTranslator>
#include <QStandardPaths>
#include <QTemporaryFile>

// For:
//  - Cjyx_QTLOADABLEMODULES_LIB_DIR
//  - Cjyx_CLIMODULES_BIN_DIR
//  - Cjyx_LIB_DIR
//  - Cjyx_SHARE_DIR
//  - Cjyx_USE_PYTHONQT
//  - Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
//  - Cjyx_BUILD_WIN32_CONSOLE
//  - Cjyx_BUILD_CLI_SUPPORT
//  - Cjyx_BUILD_I18N_SUPPORT
//  - Cjyx_ORGANIZATION_DOMAIN
//  - Cjyx_ORGANIZATION_NAME
//  - CJYX_REVISION_SPECIFIC_USER_SETTINGS_FILEBASENAME
//  - Cjyx_STORE_SETTINGS_IN_APPLICATION_HOME_DIR
#include "vtkCjyxConfigure.h"

#ifdef Cjyx_USE_PYTHONQT
// PythonQt includes
#include <PythonQt.h>
#endif

#ifdef Cjyx_USE_PYTHONQT_WITH_OPENSSL
#include <QSslCertificate>
#include <QSslSocket>
#endif

// Cjyx includes
#include "qCjyxCoreApplication_p.h"
#include "qCjyxCoreCommandOptions.h"
#include "qCjyxCoreIOManager.h"
#ifdef Cjyx_USE_PYTHONQT
# include "qCjyxCorePythonManager.h"
# include "ctkPythonConsole.h"
#endif
#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
# include "qCjyxExtensionsManagerModel.h"
#endif
#include "qCjyxLoadableModuleFactory.h"
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxModuleManager.h"
#include "qCjyxUtils.h"

// CjyxLogic includes
#include "vtkDataIOManagerLogic.h"

// MRMLDisplayable includes
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>

// MRMLLogic includes
#include <vtkMRMLRemoteIOLogic.h>

// MRML includes
#include <vtkCacheManager.h>
#include <vtkEventBroker.h>
#include <vtkMRMLCrosshairNode.h>
#ifdef Cjyx_BUILD_CLI_SUPPORT
# include <vtkMRMLCommandLineModuleNode.h>
#endif
#include <vtkMRMLScene.h>

// CTK includes
#include <ctkUtils.h>

// CTKLauncherLib includes
#include <ctkAppLauncherEnvironment.h>
#include <ctkAppLauncherSettings.h>

// VTK includes
#include <vtkCallbackCommand.h>
#include <vtkNew.h>
#include <vtksys/SystemTools.hxx>

// VTKAddon includes
#include <vtkPersonInformation.h>

// Cjyx includes
#include "vtkCjyxVersionConfigure.h" // For Cjyx_VERSION_{MINOR, MAJOR}, Cjyx_VERSION_FULL

#ifdef Cjyx_BUILD_DICOM_SUPPORT
// XXX Avoid  warning: "HAVE_XXXX" redefined
#undef HAVE_STAT
#undef HAVE_FTIME
#undef HAVE_GETPID
#undef HAVE_IO_H
#undef HAVE_STRERROR
#undef HAVE_SYS_UTIME_H
#undef HAVE_TEMPNAM
#undef HAVE_TMPNAM
#undef HAVE_LONG_LONG
// XXX Fix windows build error
#undef HAVE_INT64_T
#include <ctkDICOMDatabase.h>
#endif

//-----------------------------------------------------------------------------
// Helper function

#ifdef Cjyx_USE_PYTHONQT
namespace
{
wchar_t* QStringToPythonWCharPointer(QString str)
  {
  wchar_t* res = (wchar_t*)PyMem_RawMalloc((str.size() + 1) * sizeof(wchar_t));
  int len = str.toWCharArray(res);
  res[len] = 0; // ensure zero termination
  return res;
  }
}
#endif

//-----------------------------------------------------------------------------
// qCjyxCoreApplicationPrivate methods

//-----------------------------------------------------------------------------
qCjyxCoreApplicationPrivate::qCjyxCoreApplicationPrivate(
  qCjyxCoreApplication& object,
  qCjyxCoreCommandOptions * coreCommandOptions,
  qCjyxCoreIOManager * coreIOManager) : q_ptr(&object)
{
  qRegisterMetaType<qCjyxCoreApplication::ReturnCode>("qCjyxCoreApplication::ReturnCode");
  this->DefaultSettings = nullptr;
  this->UserSettings = nullptr;
  this->RevisionUserSettings = nullptr;
  this->ReturnCode = qCjyxCoreApplication::ExitNotRequested;
  this->CoreCommandOptions = QSharedPointer<qCjyxCoreCommandOptions>(coreCommandOptions);
  this->CoreIOManager = QSharedPointer<qCjyxCoreIOManager>(coreIOManager);
#ifdef Cjyx_BUILD_DICOM_SUPPORT
  this->DICOMDatabase = QSharedPointer<ctkDICOMDatabase>(new ctkDICOMDatabase);
#endif
  this->NextResourceHandle = 0;
  this->StartupWorkingPath = QDir::currentPath();
}

//-----------------------------------------------------------------------------
qCjyxCoreApplicationPrivate::~qCjyxCoreApplicationPrivate()
{
  // - The ModuleManager deals with scripted module which internally work with
  // python references. (I.e calling Py_DECREF, ...)
  // - The PythonManager takes care of initializing and terminating the
  // python embedded interpreter
  // => De facto, it's important to make sure PythonManager is destructed
  // after the ModuleManager.
  // To do so, the associated SharedPointer are cleared in the appropriate order
  this->ModuleManager.clear();
  this->CoreIOManager.clear();
#ifdef Cjyx_USE_PYTHONQT
  this->CorePythonManager.clear();
#endif

  this->AppLogic->TerminateProcessingThread();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplicationPrivate::init()
{
  Q_Q(qCjyxCoreApplication);

  // Minimize the number of call to 'systemEnvironment()' by keeping
  // a reference to 'Environment'. Indeed, re-creating QProcessEnvironment is a non-trivial
  // operation. See http://doc.qt.nokia.com/4.7/qprocessenvironment.html#systemEnvironment
  // Note also that since environment variables are set using 'setEnvironmentVariable()',
  // 'Environment' is maintained 'up-to-date'. Nevertheless, if the environment
  // is updated solely using 'putenv(...)' function, 'Environment' won't be updated.
  this->Environment = QProcessEnvironment::systemEnvironment();

  // Set the locale to be "C" to avoid issues related to reading and writing
  // of floating point numbers.  For example, when the decimal point is set to be
  // a comma instead of a period, there can be truncation of data.
  // See these previous commits and the bug report.
  // http://viewvc.slicer.org/viewvc.cgi/Slicer4?view=revision&revision=21856
  // http://viewvc.slicer.org/viewvc.cgi/Slicer4?view=revision&revision=21865
  // http://slicer-devel.65872.n3.nabble.com/Re-Rounding-to-integer-tt4027985.html
  // http://slicer-devel.65872.n3.nabble.com/Re-slicer-users-Slicer4-can-t-really-use-it-yet-td4028040.html
  // http://slicer-users.65878.n3.nabble.com/Slicer4-DICOM-many-problems-td4025919.html
  // and issue #3029
  // Set both the Qt locale, and the standard library locale to cover
  // all supported read and write methods.
  QLocale::setDefault(QLocale::C);
  setlocale(LC_ALL, "C");

  // allow a debugger to be attached during startup
  if(qApp->arguments().contains("--attach-process"))
    {
    QString msg("This message box is here to give you time to attach "
                "your debugger to process [PID %1]");
    QMessageBox::information(nullptr, "Attach process", msg.arg(QCoreApplication::applicationPid()));
    }

  QCoreApplication::setOrganizationDomain(Cjyx_ORGANIZATION_DOMAIN);
  QCoreApplication::setOrganizationName(Cjyx_ORGANIZATION_NAME);

  QSettings::setDefaultFormat(QSettings::IniFormat);

  if (q->arguments().isEmpty())
    {
    qDebug() << "qCjyxCoreApplication must be given the True argc/argv";
    }

  this->parseArguments();

  this->CjyxHome = this->discoverCjyxHomeDirectory();

  // Save the environment if no launcher is used (this is for example the case
  // on macOS when cjyx is started from an install tree)
  if (ctkAppLauncherEnvironment::currentLevel() == 0)
    {
    QProcessEnvironment updatedEnv;
    ctkAppLauncherEnvironment::saveEnvironment(
          this->Environment, this->Environment.keys(), updatedEnv);
    foreach(const QString& varname, updatedEnv.keys())
      {
      q->setEnvironmentVariable(varname, updatedEnv.value(varname));
      }
    }

  q->setEnvironmentVariable("CJYX_HOME", this->CjyxHome);

  ctkAppLauncherSettings appLauncherSettings;
  appLauncherSettings.setLauncherName(q->applicationName());
  appLauncherSettings.setLauncherDir(this->CjyxHome);
  if (!appLauncherSettings.readSettings(q->launcherSettingsFilePath()))
    {
    q->showConsoleMessage(QString("Failed to read launcher settings %1").arg(q->launcherSettingsFilePath()));
    }

  // Regular environment variables
  QHash<QString, QString> envVars = appLauncherSettings.envVars();
  foreach(const QString& key, envVars.keys())
    {
    q->setEnvironmentVariable(key, envVars.value(key));
    }
  // Path environment variables (includes PATH, (DY)LD_LIBRARY_PATH and variables like PYTHONPATH)
  QHash<QString, QStringList> pathsEnvVars = appLauncherSettings.pathsEnvVars();
  foreach(const QString& key, pathsEnvVars.keys())
    {
    QString value;
    if (this->Environment.contains(key))
      {
      QStringList consolidatedPaths;
      consolidatedPaths << pathsEnvVars.value(key) << this->Environment.value(key).split(appLauncherSettings.pathSep());
      consolidatedPaths.removeDuplicates();
      value = consolidatedPaths.join(appLauncherSettings.pathSep());
      }
    else
      {
      value = pathsEnvVars.value(key).join(appLauncherSettings.pathSep());
      }
    q->setEnvironmentVariable(key, value);
    }

#ifdef Cjyx_USE_PYTHONQT_WITH_OPENSSL
  if (!QSslSocket::supportsSsl())
    {
    qWarning() << "[SSL] SSL support disabled - Failed to load SSL library !";
    }
  if (!qCjyxCoreApplication::loadCaCertificates(this->CjyxHome))
    {
    qWarning() << "[SSL] Failed to load Cjyx.crt";
    }
#endif

  // Add 'CJYX_SHARE_DIR' to the environment so that Tcl scripts can reference
  // their dependencies.
  q->setEnvironmentVariable("CJYX_SHARE_DIR", Cjyx_SHARE_DIR);

  // Load default settings if any.
  if (q->defaultSettings())
    {
    foreach(const QString& key, q->defaultSettings()->allKeys())
      {
      if (!q->userSettings()->contains(key))
        {
        q->userSettings()->setValue(key, q->defaultSettings()->value(key));
        }
      if (!q->revisionUserSettings()->contains(key))
        {
        q->revisionUserSettings()->setValue(key, q->defaultSettings()->value(key));
        }
      }
    }

  // Create the application Logic object,
  this->AppLogic = vtkSmartPointer<vtkCjyxApplicationLogic>::New();

  // Create callback function that allows invoking VTK object modified requests from any thread.
  // This is used for example in MRMLIDImageIO to indicate that image update is completed.
  { // placed in a block to avoid memory leaks on quick exit at handlePreApplicationCommandLineArguments
    vtkNew<vtkCallbackCommand> modifiedRequestCallback;
    modifiedRequestCallback->SetClientData(this->AppLogic);
    modifiedRequestCallback->SetCallback(vtkCjyxApplicationLogic::RequestModifiedCallback);
    vtkEventBroker::GetInstance()->SetRequestModifiedCallback(modifiedRequestCallback);
  }

  // Ensure that temporary folder is writable
  {
    // QTemporaryFile is deleted automatically when leaving this scope
    QTemporaryFile fileInTemporaryPathFolder(
      QFileInfo(q->temporaryPath(), "_write_test_XXXXXX.tmp").absoluteFilePath());
    if (!fileInTemporaryPathFolder.open())
      {
      QString newTempFolder = q->defaultTemporaryPath();
      qWarning() << Q_FUNC_INFO << "Setting temporary folder to " << newTempFolder
        << " because previously set " << q->temporaryPath() << " folder is not writable";
      q->setTemporaryPath(newTempFolder);
      }
  }
  this->AppLogic->SetTemporaryPath(q->temporaryPath().toUtf8());

  vtkPersonInformation* userInfo = this->AppLogic->GetUserInformation();
  if (userInfo)
    {
    QString userInfoString = q->userSettings()->value("UserInformation").toString();
    userInfo->SetFromString(userInfoString.toUtf8().constData());
    }
  q->qvtkConnect(this->AppLogic, vtkCommand::ModifiedEvent,
              q, SLOT(onCjyxApplicationLogicModified()));
  q->qvtkConnect(this->AppLogic, vtkCjyxApplicationLogic::RequestInvokeEvent,
                 q, SLOT(requestInvokeEvent(vtkObject*,void*)), 0.0, Qt::DirectConnection);
  q->connect(q, SIGNAL(invokeEventRequested(unsigned int,void*,unsigned long,void*)),
             q, SLOT(scheduleInvokeEvent(unsigned int,void*,unsigned long,void*)), Qt::AutoConnection);
  q->qvtkConnect(this->AppLogic, vtkCjyxApplicationLogic::RequestModifiedEvent,
              q, SLOT(onCjyxApplicationLogicRequest(vtkObject*,void*,ulong)));
  q->qvtkConnect(this->AppLogic, vtkCjyxApplicationLogic::RequestReadDataEvent,
              q, SLOT(onCjyxApplicationLogicRequest(vtkObject*,void*,ulong)));
  q->qvtkConnect(this->AppLogic, vtkCjyxApplicationLogic::RequestWriteDataEvent,
              q, SLOT(onCjyxApplicationLogicRequest(vtkObject*,void*,ulong)));
  q->qvtkConnect(this->AppLogic, vtkMRMLApplicationLogic::PauseRenderEvent,
              q, SLOT(pauseRender()));
  q->qvtkConnect(this->AppLogic, vtkMRMLApplicationLogic::ResumeRenderEvent,
              q, SLOT(resumeRender()));
  q->qvtkConnect(this->AppLogic, vtkCjyxApplicationLogic::EditNodeEvent,
              q, SLOT(editNode(vtkObject*, void*, ulong)));
  q->qvtkConnect(this->AppLogic->GetUserInformation(), vtkCommand::ModifiedEvent,
    q, SLOT(onUserInformationModified()));

  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->SetMRMLApplicationLogic(
    this->AppLogic.GetPointer());
  vtkMRMLSliceViewDisplayableManagerFactory::GetInstance()->SetMRMLApplicationLogic(
    this->AppLogic.GetPointer());

  // pass through event handling once without observing the scene
  // -- allows any dependent nodes to be created
  // Note that Interaction and Selection Node are now created
  // in MRMLApplicationLogic.
  //this->AppLogic->ProcessMRMLEvents(scene, vtkCommand::ModifiedEvent, nullptr);
  //this->AppLogic->SetAndObserveMRMLScene(scene);
  this->AppLogic->CreateProcessingThread();

  // Set up Cjyx to use the system proxy
  QNetworkProxyFactory::setUseSystemConfiguration(true);

  // Set up Data IO
  this->initDataIO();

  // Create MRML scene
  vtkMRMLScene* scene = vtkMRMLScene::New();
  q->setMRMLScene(scene);
  // Scene is not owned by this class. Remove the local variable because
  // handlePreApplicationCommandLineArguments() may cause quick exit from the application
  // and we would have memory leaks if we still hold a reference to the scene in a
  // local variable.
  scene->UnRegister(nullptr);

  // Instantiate moduleManager
  this->ModuleManager = QSharedPointer<qCjyxModuleManager>(new qCjyxModuleManager);
  this->ModuleManager->factoryManager()->setAppLogic(this->AppLogic.GetPointer());
  this->ModuleManager->factoryManager()->setMRMLScene(scene);
  q->connect(q, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
                 this->ModuleManager->factoryManager(), SLOT(setMRMLScene(vtkMRMLScene*)));

  // The application may exit here immediately if a simple command is specified on the
  // command-line (for example `--version` prints the version information and quits).
  q->handlePreApplicationCommandLineArguments();

#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    if (q->corePythonManager())
      {
      q->corePythonManager()->mainContext(); // Initialize python
      q->corePythonManager()->setSystemExitExceptionHandlerEnabled(true);
      q->connect(q->corePythonManager(), SIGNAL(systemExitExceptionRaised(int)),
                 q, SLOT(terminate(int)));
      }
    }
#endif

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT

  qCjyxExtensionsManagerModel * model = new qCjyxExtensionsManagerModel(q);
  model->setExtensionsSettingsFilePath(q->cjyxRevisionUserSettingsFilePath());
  model->setCjyxRequirements(q->revision(), q->os(), q->arch());
  q->setExtensionsManagerModel(model);

  // Clear extension server settings with requested serverAPI changed
  QSettings extensionsSettings(model->extensionsSettingsFilePath(), QSettings::IniFormat);
  int lastServerAPI = qCjyxExtensionsManagerModel::serverAPIFromString(extensionsSettings.value("Extensions/LastServerAPI").toString());
  if (lastServerAPI != model->serverAPI())
    {
    extensionsSettings.remove("Extensions/ServerUrl");
    extensionsSettings.remove("Extensions/FrontendServerUrl");
    }
  extensionsSettings.setValue("Extensions/LastServerAPI", qCjyxExtensionsManagerModel::serverAPIToString(model->serverAPI()));

# ifdef Q_OS_MAC
  this->createDirectory(this->defaultExtensionsInstallPathForMacOSX(), "extensions"); // Make sure the path exists
  q->addLibraryPath(this->defaultExtensionsInstallPathForMacOSX());
  q->setExtensionsInstallPath(this->defaultExtensionsInstallPathForMacOSX());
# endif

  this->createDirectory(q->extensionsInstallPath(), "extensions"); // Make sure the path exists

  // Prevent extensions manager model from displaying popups during startup (don't ask for confirmation)
  bool wasInteractive = model->interactive();
  model->setInteractive(false);

  // If auto-update is disabled then this will not contact the server
  model->updateExtensionsMetadataFromServer();

  model->updateModel();

  QStringList updatedExtensions;
  model->updateScheduledExtensions(updatedExtensions);
  foreach(const QString& extensionName, updatedExtensions)
    {
    qDebug() << "Successfully updated extension" << extensionName;
    }

  QStringList uninstalledExtensions;
  model->uninstallScheduledExtensions(uninstalledExtensions);
  foreach(const QString& extensionName, uninstalledExtensions)
    {
    qDebug() << "Successfully uninstalled extension" << extensionName;
    }

  if (model->autoUpdateCheck())
    {
    // The latest metadata updates will not be available yet, but next time the application
    // is started.
    model->checkForExtensionsUpdates();
    }

  model->setInteractive(wasInteractive);

  // Indicate that there are no more changes to the extensions
  // and they will be loaded using current settings.
  model->aboutToLoadExtensions();

  // Set the list of installed extensions in the scene so that we can warn the user
  // if the scene is loaded with some of the extensions not present.
  QString extensionList = model->installedExtensions().join(";");
  scene->SetExtensions(extensionList.toStdString().c_str());

#endif

  if (q->userSettings()->value("Internationalization/Enabled").toBool())
    {
    // We load the language selected for the application
    qCjyxCoreApplication::loadLanguage();
    }

  q->connect(q, SIGNAL(aboutToQuit()), q, SLOT(onAboutToQuit()));
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplicationPrivate::quickExit(int exitCode)
{
  Q_Q(qCjyxCoreApplication);

  // Delete VTK objects to exit cleanly, without memory leaks
  this->AppLogic = nullptr;
  this->MRMLRemoteIOLogic = nullptr;
  this->DataIOManagerLogic = nullptr;
#ifdef Cjyx_BUILD_DICOM_SUPPORT
  // Make sure the DICOM database is closed properly
  this->DICOMDatabase.clear();
#endif
  q->setMRMLScene(nullptr);
  q->qvtkDisconnectAll();

  // XXX When supporting exclusively C++11, replace with std::quick_exit
#ifdef Q_OS_WIN32
  ExitProcess(exitCode);
#else
  _exit(exitCode);
#endif
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplicationPrivate::initDataIO()
{
  Q_Q(qCjyxCoreApplication);

  // Create MRMLRemoteIOLogic
  this->MRMLRemoteIOLogic = vtkSmartPointer<vtkMRMLRemoteIOLogic>::New();

  // Ensure cache folder is writable
  {
    // QTemporaryFile is deleted automatically when leaving this scope
    QTemporaryFile fileInCacheFolder(
      QFileInfo(q->cachePath(), "_write_test_XXXXXX.tmp").absoluteFilePath());
    if (!fileInCacheFolder.open())
    {
      QString newCacheFolder = q->defaultCachePath();
      qWarning() << Q_FUNC_INFO << "Setting cache folder to " << newCacheFolder
        << " because previously set " << q->cachePath() << " folder is not writable";
      q->setCachePath(newCacheFolder);
    }
  }
  this->MRMLRemoteIOLogic->GetCacheManager()->SetRemoteCacheDirectory(q->cachePath().toUtf8());

  this->DataIOManagerLogic = vtkSmartPointer<vtkDataIOManagerLogic>::New();
  this->DataIOManagerLogic->SetMRMLApplicationLogic(this->AppLogic);
  this->DataIOManagerLogic->SetAndObserveDataIOManager(
    this->MRMLRemoteIOLogic->GetDataIOManager());
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplicationPrivate::newSettings()
{
  Q_Q(qCjyxCoreApplication);
  return new QSettings(q);
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplicationPrivate::instantiateSettings(bool useTmp)
{
  Q_Q(qCjyxCoreApplication);
  if (useTmp)
    {
    q->setApplicationName(q->applicationName() + "-tmp");
    }
#ifdef Cjyx_STORE_SETTINGS_IN_APPLICATION_HOME_DIR
  // If a Cjyx.ini file is available in the home directory then use that,
  // otherwise use the default one in the user profile folder.
  // Qt appends organizationName/organizationDomain to the directory set in QSettings::setPath, therefore we must include it in the folder name
  // (otherwise QSettings() would return a different setting than app->userSettings()).
  QString iniFileName = QDir(this->CjyxHome).filePath(QString("%1/%2.ini").arg(ctkAppLauncherSettings().organizationDir()).arg(q->applicationName()));
  if (QFile(iniFileName).exists())
    {
    // Use settings file in the home folder
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, this->CjyxHome);
    }
#endif
  QSettings* settings = this->newSettings();
  if (useTmp && !q->coreCommandOptions()->keepTemporarySettings())
    {
    settings->clear();
    }
  return settings;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplicationPrivate::isInstalled(const QString& cjyxHome)const
{
  return !QFile::exists(cjyxHome + "/CMakeCache.txt");
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplicationPrivate::discoverCjyxHomeDirectory()
{
  // Since some standalone executables (i.e EMSegmentCommandLine) can create
  // an instance of qCjyx(Core)Application so that the environment and the
  // python manager are properly initialized. These executables will have
  // to set CJYX_HOME. If not, the current directory associated with that
  // executable will be considered and initialization code expecting CJYX_HOME
  // to be properly set will fail.
  QString cjyxHome = this->Environment.value("CJYX_HOME");
  if (cjyxHome.isEmpty())
    {
    QString cjyxBin = this->discoverCjyxBinDirectory();
    QDir cjyxBinDir(cjyxBin);
    bool cdUpRes = cjyxBinDir.cdUp();
    Q_ASSERT(cdUpRes);
    (void)cdUpRes;
    cjyxHome = cjyxBinDir.canonicalPath();
    }

#ifdef Q_OS_WIN32
  Q_Q(qCjyxCoreApplication);
  if (!this->isInstalled(cjyxHome))
    {
    foreach(const QString& subDir,
            QStringList() << Cjyx_BIN_DIR << Cjyx_CLIMODULES_BIN_DIR << "Cxx")
      {
      qCjyxUtils::pathWithoutIntDir(q->applicationDirPath(), subDir, this->IntDir);
      if (!this->IntDir.isEmpty())
        {
        break;
        }
      }
    }
  Q_ASSERT(this->isInstalled(cjyxHome) ? this->IntDir.isEmpty() : !this->IntDir.isEmpty());
#endif

  return cjyxHome;
}

//-----------------------------------------------------------------------------
#ifdef Cjyx_USE_PYTHONQT
void qCjyxCoreApplicationPrivate::setPythonOsEnviron(const QString& key, const QString& value)
{
  if(!this->CorePythonManager->isPythonInitialized())
    {
    return;
    }
  this->CorePythonManager->executeString(
        QString("import os; os.environ[%1]=%2; del os")
          .arg(qCjyxCorePythonManager::toPythonStringLiteral(key))
          .arg(qCjyxCorePythonManager::toPythonStringLiteral(value)));
}
#endif

//-----------------------------------------------------------------------------
void qCjyxCoreApplicationPrivate::updateEnvironmentVariable(const QString& key, const QString& value,
                                                              QChar separator, bool prepend)
{
  Q_Q(qCjyxCoreApplication);
  if(q->isEnvironmentVariableValueSet(key, value))
    {
    return;
    }
  std::string currentValue;
  vtksys::SystemTools::GetEnv(key.toUtf8(), currentValue);
  if(currentValue.size() > 0)
    {
    QString updatedValue(value);
    if(prepend)
      {
      q->setEnvironmentVariable(key, updatedValue.prepend(separator).prepend(QString::fromStdString(currentValue)));
      }
    else
      {
      q->setEnvironmentVariable(key, updatedValue.append(separator).append(QString::fromStdString(currentValue)));
      }
    }
  else
    {
    q->setEnvironmentVariable(key, value);
    }
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplicationPrivate::discoverCjyxBinDirectory()
{
  Q_Q(qCjyxCoreApplication);
  QString cjyxBin;
  // Note: On Linux, QCoreApplication::applicationDirPath() will attempt
  //       to get the path using the "/proc" filesystem.
  if (!QFile::exists(q->applicationDirPath()))
    {
    q->showConsoleMessage(QString("Cannot find Cjyx executable %1").arg(q->applicationDirPath()));
    return cjyxBin;
    }
#ifndef Q_OS_MAC
  cjyxBin =
      qCjyxUtils::pathWithoutIntDir(q->applicationDirPath(), Cjyx_BIN_DIR, this->IntDir);
#else
  // There are two cases to consider, the application could be started from:
  //   1) Install tree
  //        Application location: /path/to/Foo.app/Contents/MacOSX/myapp
  //        Binary directory:     /path/to/Foo.app/Contents/bin
  //   2) Build tree
  //        Application location: /path/to/build-dir/bin/Foo.app/Contents/MacOSX/myapp
  //        Binary directory:     /path/to/build-dir/bin
  //
  QDir cjyxBinAsDir(q->applicationDirPath());
  cjyxBinAsDir.cdUp(); // Move from /path/to/Foo.app/Contents/MacOSX to /path/to/Foo.app/Contents
  if(!cjyxBinAsDir.cd(Cjyx_BIN_DIR))
    {
    cjyxBinAsDir.cdUp(); // Move from /path/to/build-dir/bin/Foo.app/Contents to /path/to/build-dir/bin/Foo.app
    cjyxBinAsDir.cdUp(); // Move from /path/to/build-dir/bin/Foo.app          to /path/to/build-dir/bin
    cjyxBinAsDir.cd(Cjyx_BIN_DIR);
    }
  cjyxBin = cjyxBinAsDir.path();
#endif
  Q_ASSERT(qCjyxUtils::pathEndsWith(cjyxBin, Cjyx_BIN_DIR));
  return cjyxBin;
}

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
//-----------------------------------------------------------------------------
QString qCjyxCoreApplicationPrivate::defaultExtensionsInstallPathForMacOSX()const
{
  Q_Q(const qCjyxCoreApplication);
  if (q->isInstalled())
    {
    QDir cjyxHomeDir(q->cjyxHome());
    cjyxHomeDir.cdUp();
    return cjyxHomeDir.absolutePath() + "/Contents/" Cjyx_EXTENSIONS_DIRNAME;
    }
  else
    {
    return q->cjyxHome() + "/bin/" Cjyx_BUNDLE_LOCATION "/" Cjyx_EXTENSIONS_DIRNAME;
    }
}
#endif

//-----------------------------------------------------------------------------
bool qCjyxCoreApplicationPrivate::isUsingLauncher()const
{
  Q_Q(const qCjyxCoreApplication);
  if (!q->isInstalled())
    {
    return true;
    }
  else
    {
#ifdef Q_OS_MAC
    return false;
#else
    return true;
#endif
    }
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplicationPrivate::createDirectory(const QString& path, const QString& description) const
{
  if (path.isEmpty())
    {
    return false;
    }
  if (QDir(path).exists())
    {
    return true;
    }
  if (!QDir::root().mkpath(path))
    {
    qCritical() << qCjyxCoreApplication::tr("Failed to create %1 directory").arg(description) << path;
    return false;
    }
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplicationPrivate::parseArguments()
{
  Q_Q(qCjyxCoreApplication);

  qCjyxCoreCommandOptions* options = this->CoreCommandOptions.data();
  if (!options)
    {
    q->showConsoleMessage("Failed to parse arguments - "
                  "it seems you forgot to call setCoreCommandOptions()");
    this->quickExit(EXIT_FAILURE);
    }
  if (!options->parse(q->arguments()))
    {
    q->showConsoleMessage("Problem parsing command line arguments.  Try with --help.");
    this->quickExit(EXIT_FAILURE);
    }
}

//----------------------------------------------------------------------------
QStringList qCjyxCoreApplicationPrivate::findTranslationFilesWithLanguageExtension(const QString& dir, const QString& languageExtension)
{
  // Search with both underscore and hyphen as language/region separator (such as pt_BR),
  // because languageExtension is expected to use underscore, but the translation .qm file may use either.
  QStringList foundFiles;

  // Underscore (Cjyx_pt_BR.qm)
  QString languageExtensionUnderscore(languageExtension);
  // languageExtension is expected to use underscore, but just in case hyphen was used replace them
  languageExtensionUnderscore.replace('-', '_');
  const QString localeFilterUnderscore = QString("*%1.qm").arg(languageExtensionUnderscore);
  foundFiles << QDir(dir).entryList(QStringList(localeFilterUnderscore));

  // Hyphen (Cjyx_pt-BR.qm)
  QString languageExtensionHyphen(languageExtension);
  languageExtensionHyphen.replace('_', '-');
  const QString localeFilterHyphen = QString("*%1.qm").arg(languageExtensionHyphen);
  foundFiles << QDir(dir).entryList(QStringList(localeFilterHyphen));

  foundFiles.removeDuplicates();
  return foundFiles;
}

//----------------------------------------------------------------------------
QStringList qCjyxCoreApplicationPrivate::findTranslationFiles(const QString& dir, const QString& settingsLanguage)
{
  // If settings language is empty don't search translation files (application default)
  if (settingsLanguage.isEmpty())
    {
    return QStringList{};
    }

  // Try to find the translation files using specific language extension
  // (In case of fr_FR -> look for *fr_FR.qm files).
  QStringList files = findTranslationFilesWithLanguageExtension(dir, settingsLanguage);
  if (files.isEmpty())
    {
    // If no specific translations have been found, look for a generic language extension
    // (In case of fr_FR -> look for *fr.qm files).
    const QString genericExtension = settingsLanguage.split("_")[0];
    if (genericExtension != settingsLanguage)
      {
      files = findTranslationFilesWithLanguageExtension(dir, genericExtension);
      }
    }

  return files;
}

//-----------------------------------------------------------------------------
// qCjyxCoreApplication methods

//-----------------------------------------------------------------------------
qCjyxCoreApplication::qCjyxCoreApplication(int &_argc, char **_argv):Superclass(_argc, _argv)
  , d_ptr(new qCjyxCoreApplicationPrivate(*this, new qCjyxCoreCommandOptions, new qCjyxCoreIOManager))
{
  Q_D(qCjyxCoreApplication);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxCoreApplication::qCjyxCoreApplication(
  qCjyxCoreApplicationPrivate* pimpl, int &argc, char **argv)
  : Superclass(argc, argv), d_ptr(pimpl)
{
  // Note: You are responsible to call init() in the constructor of derived class.
}

//-----------------------------------------------------------------------------
qCjyxCoreApplication::~qCjyxCoreApplication() = default;

//-----------------------------------------------------------------------------
qCjyxCoreApplication* qCjyxCoreApplication::application()
{
  qCjyxCoreApplication* app = qobject_cast<qCjyxCoreApplication*>(QApplication::instance());
  return app;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setAttribute(qCjyxCoreApplication::ApplicationAttribute attribute, bool on)
{
  QCoreApplication::setAttribute(static_cast<Qt::ApplicationAttribute>(attribute), on);
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::ApplicationAttribute attribute)
{
  return QCoreApplication::testAttribute(static_cast<Qt::ApplicationAttribute>(attribute));
}

//-----------------------------------------------------------------------------
QProcessEnvironment qCjyxCoreApplication::startupEnvironment() const
{
  return ctkAppLauncherEnvironment::environment(0);
}

//-----------------------------------------------------------------------------
QProcessEnvironment qCjyxCoreApplication::environment() const
{
  Q_D(const qCjyxCoreApplication);
  return d->Environment;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setEnvironmentVariable(const QString& key, const QString& value)
{
  Q_D(qCjyxCoreApplication);

  d->Environment.insert(key, value);
  // Since QProcessEnvironment can't be used to update the environment of the
  // current process, let's use 'putenv()'.
  // See http://doc.qt.nokia.com/4.6/qprocessenvironment.html#details
  vtksys::SystemTools::PutEnv(QString("%1=%2").arg(key).arg(value).toUtf8().constData());

#ifdef Cjyx_USE_PYTHONQT
  d->setPythonOsEnviron(key, value);
#endif
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplication::isEnvironmentVariableValueSet(const QString& key, const QString& value)
{
  std::string currentValue;
  vtksys::SystemTools::GetEnv(key.toUtf8(), currentValue);
  return QString::fromStdString(currentValue).contains(value);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::prependEnvironmentVariable(const QString& key, const QString& value, QChar separator)
{
  Q_D(qCjyxCoreApplication);
  d->updateEnvironmentVariable(key, value, separator, true);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::appendEnvironmentVariable(const QString& key, const QString& value, QChar separator)
{
  Q_D(qCjyxCoreApplication);
  d->updateEnvironmentVariable(key, value, separator, false);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::parseArguments(bool& exitWhenDone)
{
  Q_D(qCjyxCoreApplication);
  d->parseArguments();
  exitWhenDone = (d->ReturnCode != ExitNotRequested);
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::returnCode()const
{
  Q_D(const qCjyxCoreApplication);
  return d->ReturnCode;
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::exec()
{
  int exit_code = QApplication::exec();
  if (exit_code == qCjyxCoreApplication::ExitSuccess)
    {
    int return_code = qCjyxCoreApplication::application()->returnCode();
    if (return_code != qCjyxCoreApplication::ExitNotRequested)
      {
      exit_code = return_code;
      }
    }
  return exit_code;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::handlePreApplicationCommandLineArguments()
{
  Q_D(qCjyxCoreApplication);

  qCjyxCoreCommandOptions* options = this->coreCommandOptions();
  Q_ASSERT(options);

  if (options->displayHelpAndExit())
    {
    if(!d->isUsingLauncher())
      {
      std::cout << "Usage\n"
                << "  " << qPrintable(this->applicationName()) << " [options]\n\n"
                << "Options\n";
      }
    std::cout << qPrintable(options->helpText()) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->displayVersionAndExit())
    {
    std::cout << qPrintable(this->applicationName() + " " +
                            this->applicationVersion()) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->displayProgramPathAndExit())
    {
    std::cout << qPrintable(this->arguments().at(0)) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->displayHomePathAndExit())
    {
    std::cout << qPrintable(this->cjyxHome()) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->displaySettingsPathAndExit())
    {
    std::cout << qPrintable(this->userSettings()->fileName()) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->displayTemporaryPathAndExit())
    {
    std::cout << qPrintable(this->temporaryPath()) << std::endl;
    d->quickExit(EXIT_SUCCESS);
    }

  if (options->ignoreRest())
    {
    qDebug() << "Ignored arguments:" << options->unparsedArguments();
    return;
    }

  if (!options->settingsDisabled() && options->keepTemporarySettings())
    {
    this->showConsoleMessage("Argument '--keep-temporary-settings' requires "
                  "'--settings-disabled' to be specified.");
    }

  if (options->isTestingEnabled())
    {
    this->setAttribute(AA_EnableTesting);
    }

#ifdef Cjyx_USE_PYTHONQT
  if (options->isPythonDisabled())
    {
    this->setAttribute(AA_DisablePython);
    }
#endif
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::handleCommandLineArguments()
{
  qCjyxCoreCommandOptions* options = this->coreCommandOptions();

  QStringList filesToLoad;
  QStringList unparsedArguments = options->unparsedArguments();
  if (unparsedArguments.length() > 0 &&
      options->pythonScript().isEmpty() &&
      options->extraPythonScript().isEmpty())
    {
    foreach(QString fileName, unparsedArguments)
      {
      QUrl url = QUrl(fileName);
      if (url.scheme().toLower() == this->applicationName().toLower()) // Scheme is case insensitive
        {
        qDebug() << "URL received via command-line: " << fileName;
        emit urlReceived(fileName);
        continue;
        }

      QFileInfo file(fileName);
      if (file.exists())
        {
        qDebug() << "Local filepath received via command-line: " << fileName;
        // Do not load immediately but just collect the files into a list and load at once
        // so that all potential loading errors can be also reported at once.
        filesToLoad << fileName;
        continue;
        }

      qDebug() << "Ignore argument received via command-line (not a valid URL or existing local file): " << fileName;
      }
    }

  if (!filesToLoad.isEmpty())
    {
    this->loadFiles(filesToLoad);
    }

#ifndef Cjyx_USE_PYTHONQT
  Q_UNUSED(options);
#else
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    // Note that 'pythonScript' is ignored if 'extraPythonScript' is specified
    QString pythonScript = options->pythonScript();
    QString extraPythonScript = options->extraPythonScript();
    QStringList scriptArgs = options->unparsedArguments();
    // Do not pass "--attach-process", it will avoid some python script to complain about
    // unknown argument.
    scriptArgs.removeAll("--attach-process");
    if(!extraPythonScript.isEmpty())
      {
      scriptArgs.removeFirst();
      pythonScript = extraPythonScript;
      }

    // Set 'argv' so that python script can retrieve its associated arguments

    // TODO do we need validation here?

    int pythonArgc = 1 /*scriptname*/ + scriptArgs.count();
    wchar_t** pythonArgv = new wchar_t*[pythonArgc];
    pythonArgv[0] = QStringToPythonWCharPointer(pythonScript);
    for(int i = 0; i < scriptArgs.count(); ++i)
      {
      pythonArgv[i + 1] = QStringToPythonWCharPointer(scriptArgs.at(i));
      }

    // See https://docs.python.org/c-api/init.html
    PySys_SetArgvEx(pythonArgc, pythonArgv, /*updatepath=*/false);

    // Set 'sys.executable' so that Cjyx can be used as a "regular" python interpreter
    this->corePythonManager()->executeString(
          QString("import sys; sys.executable = %1; del sys").arg(
            qCjyxCorePythonManager::toPythonStringLiteral(QStandardPaths::findExecutable("PythonCjyx")))
          );

    // Clean memory
    for (int i = 0; i < pythonArgc; i++)
      {
      PyMem_RawFree(pythonArgv[i]);
      }
    delete[] pythonArgv;
    pythonArgv = nullptr;
    pythonArgc = 0;

    // Attempt to load Cjyx RC file only if 'display...AndExit' options are not True
    if (!(options->displayMessageAndExit() ||
        options->ignoreCjyxRC()))
      {
      this->corePythonManager()->executeString("loadCjyxRCFile()");
      }

    if (this->testAttribute(AA_EnableTesting))
      {
      options->setRunPythonAndExit(true);
      }

    // Execute python script
    if(!pythonScript.isEmpty())
      {
      if (QFile::exists(pythonScript))
        {
        qApp->processEvents();
        this->corePythonManager()->executeFile(pythonScript);
        }
      else
        {
        this->showConsoleMessage(QString("Specified python script doesn't exist: %1").arg(pythonScript));
        }
      }
    QString pythonCode = options->pythonCode();
    if(!pythonCode.isEmpty())
      {
      qApp->processEvents();
      this->corePythonManager()->executeString(pythonCode);
      }
    if (options->runPythonAndExit())
      {
      qCjyxCoreApplication::exit(
            this->corePythonManager()->pythonErrorOccured() ? EXIT_FAILURE : EXIT_SUCCESS);
      }
    }
#endif
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplication::defaultSettings()const
{
  Q_D(const qCjyxCoreApplication);
  if (!QFile(this->cjyxDefaultSettingsFilePath()).exists())
    {
    return nullptr;
    }
  qCjyxCoreApplication* mutable_self =
    const_cast<qCjyxCoreApplication*>(this);
  qCjyxCoreApplicationPrivate* mutable_d =
    const_cast<qCjyxCoreApplicationPrivate*>(d);
  // If required, instantiate Settings
  if(!mutable_d->DefaultSettings)
    {
    mutable_d->DefaultSettings =
        new QSettings(this->cjyxDefaultSettingsFilePath(), QSettings::IniFormat, mutable_self);
    }
  return mutable_d->DefaultSettings;
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplication::userSettings()const
{
  Q_D(const qCjyxCoreApplication);
  qCjyxCoreApplicationPrivate* mutable_d =
    const_cast<qCjyxCoreApplicationPrivate*>(d);
  // If required, instantiate Settings
  if(!mutable_d->UserSettings)
    {
    mutable_d->UserSettings = mutable_d->instantiateSettings(
          this->coreCommandOptions()->settingsDisabled());
    }
  return mutable_d->UserSettings;
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplication::settings()const
{
  return this->userSettings();
}

//-----------------------------------------------------------------------------
QSettings* qCjyxCoreApplication::revisionUserSettings()const
{
  Q_D(const qCjyxCoreApplication);
  qCjyxCoreApplicationPrivate* mutable_d =
    const_cast<qCjyxCoreApplicationPrivate*>(d);
  // If required, instantiate Settings
  if(!mutable_d->RevisionUserSettings)
    {
    mutable_d->RevisionUserSettings =
        new QSettings(this->cjyxRevisionUserSettingsFilePath(),
                      QSettings::IniFormat, const_cast<qCjyxCoreApplication*>(this));
    }
  return mutable_d->RevisionUserSettings;
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxCoreApplication, QString, intDir, IntDir);
CTK_GET_CPP(qCjyxCoreApplication, QString, startupWorkingPath, StartupWorkingPath);

//-----------------------------------------------------------------------------
bool qCjyxCoreApplication::isInstalled()const
{
  Q_D(const qCjyxCoreApplication);
  return d->isInstalled(d->CjyxHome);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::releaseType()const
{
  return QString(Cjyx_RELEASE_TYPE);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setMRMLScene(vtkMRMLScene* newMRMLScene)
{
  Q_D(qCjyxCoreApplication);
  if (d->MRMLScene == newMRMLScene)
  {
    return;
  }

  // Set the default scene save directory
  if (newMRMLScene)
    {
    newMRMLScene->SetRootDirectory(this->defaultScenePath().toUtf8());

#ifdef Cjyx_BUILD_CLI_SUPPORT
    // Register the node type for the command line modules
    // TODO: should probably done in the command line logic
    vtkNew<vtkMRMLCommandLineModuleNode> clmNode;
    newMRMLScene->RegisterNodeClass(clmNode.GetPointer());
#endif

    // First scene needs a crosshair to be added manually
    vtkNew<vtkMRMLCrosshairNode> crosshair;
    crosshair->SetCrosshairName("default");
    newMRMLScene->AddNode(crosshair.GetPointer());
    }

  if (d->AppLogic.GetPointer())
    {
    d->AppLogic->SetMRMLScene(newMRMLScene);
    d->AppLogic->SetMRMLSceneDataIO(newMRMLScene, d->MRMLRemoteIOLogic.GetPointer(), d->DataIOManagerLogic.GetPointer());
    }

  d->MRMLScene = newMRMLScene;

  emit this->mrmlSceneChanged(newMRMLScene);
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxCoreApplication, vtkMRMLScene*, mrmlScene, MRMLScene);

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxCoreApplication, vtkCjyxApplicationLogic*, applicationLogic, AppLogic);

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cjyxHome() const
{
  Q_D(const qCjyxCoreApplication);
  return d->CjyxHome;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::defaultScenePath() const
{
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  QString defaultScenePath = this->toCjyxHomeAbsolutePath(appSettings->value(
        "DefaultScenePath", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString());

  return defaultScenePath;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setDefaultScenePath(const QString& path)
{
  if (this->defaultScenePath() == path)
    {
    return;
    }
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  appSettings->setValue("DefaultScenePath", this->toCjyxHomeRelativePath(path));
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cjyxSharePath() const
{
  return Cjyx_SHARE_DIR;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplication::isEmbeddedModule(const QString& moduleFileName)const
{
  QString cjyxRevision = this->revision();
#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  cjyxRevision = this->extensionsManagerModel()->cjyxRevision();
#endif
  return vtkCjyxApplicationLogic::IsEmbeddedModule(moduleFileName.toStdString(),
                                                     this->cjyxHome().toStdString(),
                                                     cjyxRevision.toStdString());
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::defaultTemporaryPath() const
{
#ifdef Q_OS_UNIX
  // In multi-user Linux environment, a single temporary directory is shared
  // by all users. We need to create a separate directory for each user,
  // as users do not have access to another user's directory.
  QString userName = qgetenv("USER");
  return QFileInfo(QDir::tempPath(), this->applicationName()+"-"+userName).absoluteFilePath();
#else
  return QFileInfo(QDir::tempPath(), this->applicationName()).absoluteFilePath();
#endif
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::temporaryPath() const
{
  Q_D(const qCjyxCoreApplication);
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  QString temporaryPath = qCjyxCoreApplication::application()->toCjyxHomeAbsolutePath(
    appSettings->value("TemporaryPath", this->defaultTemporaryPath()).toString());
  d->createDirectory(temporaryPath, "temporary"); // Make sure the path exists
  return temporaryPath;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::defaultCachePath() const
{
  QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  // According to Qt documentation: Returns a directory location where user-specific
  // non-essential (cached) data should be written. This is an application-specific directory.
  // The returned path is never empty.
  //
  // Examples:
  // - Windows: C:/Users/username/AppData/Local/NA-MIC/Cjyx/cache
  // - Linux: /home/username/.cache/NA-MIC/Cjyx
  // - macOS: /Users/username/Library/Caches/NA-MIC/Cjyx
  //
  // This is already a user and application specific folder, but various software components
  // may place files there (e.g., QWebEngine), therefore we create a subfolder (CjyxIO)
  // that Cjyx manager (cleans up, etc).
  return QFileInfo(cachePath, this->applicationName() + "IO").absoluteFilePath();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cachePath() const
{
  Q_D(const qCjyxCoreApplication);
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  QString cachePath = qCjyxCoreApplication::application()->toCjyxHomeAbsolutePath(
    appSettings->value("Cache/Path", this->defaultCachePath()).toString());
  d->createDirectory(cachePath, "cache"); // Make sure the path exists
  return cachePath;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setCachePath(const QString& path)
{
  Q_D(qCjyxCoreApplication);
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  appSettings->setValue("Cache/Path", this->toCjyxHomeRelativePath(path));
  if (!d->MRMLRemoteIOLogic)
    {
    qCritical() << Q_FUNC_INFO << " failed: invalid MRMLRemoteIOLogic";
    return;
    }
  d->MRMLRemoteIOLogic->GetCacheManager()->SetRemoteCacheDirectory(this->cachePath().toUtf8());
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::launcherExecutableFilePath()const
{
  Q_D(const qCjyxCoreApplication);
  if (!d->isUsingLauncher())
    {
    return QString();
    }
  QString appName = this->applicationName().replace("-tmp", "");
  return this->cjyxHome() + "/" + appName + qCjyxUtils::executableExtension();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::launcherSettingsFilePath()const
{
  QString appName = this->applicationName().replace("-tmp", "");
  if (this->isInstalled())
    {
    return this->cjyxHome() + "/" Cjyx_BIN_DIR "/" + appName + "LauncherSettings.ini";
    }
  else
    {
    return this->cjyxHome() + "/" + appName + "LauncherSettings.ini";
    }
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::launcherRevisionSpecificUserSettingsFilePath()const
{
  if (this->isInstalled())
    {
#ifdef Q_OS_MAC
    return QString();
#else
    return this->cjyxRevisionUserSettingsFilePath();
#endif
    }
  else
    {
    return this->cjyxRevisionUserSettingsFilePath();
    }
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cjyxDefaultSettingsFilePath()const
{
  return this->cjyxHome() + "/" Cjyx_SHARE_DIR "/" + this->applicationName() + "DefaultSettings.ini";
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cjyxUserSettingsFilePath()const
{
  return this->userSettings()->fileName();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::cjyxRevisionUserSettingsFilePath()const
{
#ifdef Cjyx_STORE_SETTINGS_IN_APPLICATION_HOME_DIR
  this->userSettings(); // ensure applicationName is initialized
  QString filePath = QString("%1/%2").arg(this->cjyxHome()).arg(ctkAppLauncherSettings().organizationDir());
  QString prefix = this->applicationName();
#else
  QFileInfo fileInfo = QFileInfo(this->userSettings()->fileName());
  QString filePath = fileInfo.path();
  QString prefix = fileInfo.completeBaseName();
#endif

  QString suffix = "-" + this->revision();
  bool useTmp = this->coreCommandOptions()->settingsDisabled();
  if (useTmp)
    {
    suffix += "-tmp";
    useTmp = true;
    }
  QString fileName =
      QDir(filePath).filePath(QString("%1%2%3.ini")
                                     .arg(prefix)
                                     .arg(CJYX_REVISION_SPECIFIC_USER_SETTINGS_FILEBASENAME)
                                     .arg(suffix));
  if (useTmp && !this->coreCommandOptions()->keepTemporarySettings())
    {
    QSettings(fileName, QSettings::IniFormat).clear();
    }
  return fileName;
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setTemporaryPath(const QString& path)
{
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  appSettings->setValue("TemporaryPath", this->toCjyxHomeRelativePath(path));
  this->applicationLogic()->SetTemporaryPath(path.toUtf8());
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::defaultExtensionsInstallPath() const
{
#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  return QFileInfo(this->cjyxRevisionUserSettingsFilePath()).dir().filePath(Cjyx_EXTENSIONS_DIRNAME);
#else
  return QString();
#endif
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::extensionsInstallPath() const
{
  QSettings settings(this->cjyxRevisionUserSettingsFilePath(), QSettings::IniFormat);
  return qCjyxCoreApplication::application()->toCjyxHomeAbsolutePath(
    settings.value("Extensions/InstallPath", this->defaultExtensionsInstallPath()).toString());
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setExtensionsInstallPath(const QString& path)
{
  if (this->extensionsInstallPath() == path)
    {
    return;
    }
  this->revisionUserSettings()->setValue("Extensions/InstallPath",
    qCjyxCoreApplication::application()->toCjyxHomeRelativePath(path));
#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  Q_ASSERT(this->extensionsManagerModel());
  this->extensionsManagerModel()->updateModel();
#endif
}

//-----------------------------------------------------------------------------
#ifdef Cjyx_USE_PYTHONQT

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setCorePythonManager(qCjyxCorePythonManager* manager)
{
  Q_D(qCjyxCoreApplication);
  d->CorePythonManager = QSharedPointer<qCjyxCorePythonManager>(manager);
}

//-----------------------------------------------------------------------------
qCjyxCorePythonManager* qCjyxCoreApplication::corePythonManager()const
{
  Q_D(const qCjyxCoreApplication);
  return d->CorePythonManager.data();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setPythonConsole(ctkPythonConsole* console)
{
  Q_D(qCjyxCoreApplication);
  d->PythonConsole = console;
}

//-----------------------------------------------------------------------------
ctkPythonConsole* qCjyxCoreApplication::pythonConsole()const
{
  Q_D(const qCjyxCoreApplication);
  return d->PythonConsole.data();
}

#endif

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setExtensionsManagerModel(qCjyxExtensionsManagerModel* model)
{
  Q_D(qCjyxCoreApplication);
  d->ExtensionsManagerModel = QSharedPointer<qCjyxExtensionsManagerModel>(model);
}

//-----------------------------------------------------------------------------
qCjyxExtensionsManagerModel* qCjyxCoreApplication::extensionsManagerModel()const
{
  Q_D(const qCjyxCoreApplication);
  return d->ExtensionsManagerModel.data();
}

#endif

//-----------------------------------------------------------------------------
ctkErrorLogAbstractModel* qCjyxCoreApplication::errorLogModel()const
{
  Q_D(const qCjyxCoreApplication);
  return d->ErrorLogModel.data();
}

//-----------------------------------------------------------------------------
qCjyxModuleManager* qCjyxCoreApplication::moduleManager()const
{
  Q_D(const qCjyxCoreApplication);
  return d->ModuleManager.data();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setCoreIOManager(qCjyxCoreIOManager* manager)
{
  Q_D(qCjyxCoreApplication);
  d->CoreIOManager = QSharedPointer<qCjyxCoreIOManager>(manager);
}

//-----------------------------------------------------------------------------
qCjyxCoreIOManager* qCjyxCoreApplication::coreIOManager()const
{
  Q_D(const qCjyxCoreApplication);
  return d->CoreIOManager.data();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::setCoreCommandOptions(qCjyxCoreCommandOptions* options)
{
  Q_D(qCjyxCoreApplication);
  d->CoreCommandOptions = QSharedPointer<qCjyxCoreCommandOptions>(options);
}

//-----------------------------------------------------------------------------
qCjyxCoreCommandOptions* qCjyxCoreApplication::coreCommandOptions()const
{
  Q_D(const qCjyxCoreApplication);
  return d->CoreCommandOptions.data();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreApplication::isCustomMainApplication()const
{
  return (this->mainApplicationName() != QString("Cjyx"));
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::mainApplicationName()const
{
  return QString(Cjyx_MAIN_PROJECT_APPLICATION_NAME);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::mainApplicationRepositoryUrl()const
{
  return QString(Cjyx_MAIN_PROJECT_WC_URL);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::mainApplicationRepositoryRevision()const
{
  return QString(Cjyx_MAIN_PROJECT_WC_REVISION);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::mainApplicationRevision()const
{
  return QString(Cjyx_MAIN_PROJECT_REVISION);
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::mainApplicationMajorVersion()const
{
  return Cjyx_MAIN_PROJECT_VERSION_MAJOR;
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::mainApplicationMinorVersion()const
{
  return Cjyx_MAIN_PROJECT_VERSION_MINOR;
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::mainApplicationPatchVersion()const
{
  return Cjyx_MAIN_PROJECT_VERSION_PATCH;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::libraries()const
{
  QString librariesText(
    "Built on top of: "
    "<a href=\"https://www.vtk.org/\">VTK</a>, "
    "<a href=\"https://www.itk.org/\">ITK</a>, "
    "<a href=\"https://www.commontk.org/index.php/Main_Page\">CTK</a>, "
    "<a href=\"https://www.qt.io/\">Qt</a>, "
    "<a href=\"http://teem.sf.net\">Teem</a>, "
    "<a href=\"https://www.python.org/\">Python</a>, "
    "<a href=\"https://dicom.offis.de/dcmtk\">DCMTK</a><br />");
  return librariesText;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::copyrights()const
{
  QString copyrightsText(
    "<table align=\"center\" border=\"0\" width=\"80%\"><tr>"
    "<td align=\"center\"><a href=\"https://slicer.readthedocs.io/en/latest/user_guide/about.html#license\">Licensing Information</a></td>"
    "<td align=\"center\"><a href=\"https://slicer.org/\">Website</a></td>"
    "<td align=\"center\"><a href=\"https://slicer.readthedocs.io/en/latest/user_guide/about.html#acknowledgments\">Acknowledgments</a></td>"
    "</tr></table>");
  return copyrightsText;
}
//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::acknowledgment()const
{
  QString acknowledgmentText(
    "Cjyx is NOT an FDA approved medical device.<br /><br />"
    "Supported by: NA-MIC, NAC, BIRN, NCIGT and the Cjyx Community.<br /><br />"
    "Special thanks to the NIH and our other supporters.<br /><br />"
    "This work is part of the National Alliance for Medical Image Computing "
    "(NA-MIC), funded by the National Institutes of Health through the NIH "
    "Roadmap for Medical Research, Grant U54 EB005149. Information on the "
    "National Centers for Biomedical Computing can be obtained from "
    "<a href=\"https://commonfund.nih.gov/bioinformatics\">https://commonfund.nih.gov/bioinformatics</a>.<br /><br />");
  return acknowledgmentText;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::repositoryUrl()const
{
  return Cjyx_WC_URL;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::repositoryBranch()const
{
  return QFileInfo(this->repositoryUrl()).fileName();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::repositoryRevision()const
{
  return Cjyx_WC_REVISION;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::revision()const
{
  return Cjyx_REVISION;
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::majorVersion() const
{
  return Cjyx_VERSION_MAJOR;
}

//-----------------------------------------------------------------------------
int qCjyxCoreApplication::minorVersion() const
{
  return Cjyx_VERSION_MINOR;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::platform()const
{
  return QString("%1-%2").arg(Cjyx_OS).arg(Cjyx_ARCHITECTURE);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::arch()const
{
  return Cjyx_ARCHITECTURE;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreApplication::os()const
{
  return Cjyx_OS;
}

#ifdef Cjyx_BUILD_DICOM_SUPPORT
//-----------------------------------------------------------------------------
ctkDICOMDatabase* qCjyxCoreApplication::dicomDatabase()const
{
  Q_D(const qCjyxCoreApplication);
  return d->DICOMDatabase.data();
}

//-----------------------------------------------------------------------------
QSharedPointer<ctkDICOMDatabase> qCjyxCoreApplication::dicomDatabaseShared()const
{
  Q_D(const qCjyxCoreApplication);
  return d->DICOMDatabase;
}
#endif


//-----------------------------------------------------------------------------
void qCjyxCoreApplication::restart()
{
  qCjyxCoreApplication * coreApp = qCjyxCoreApplication::application();
  bool launcherAvailable = QFile::exists(coreApp->launcherExecutableFilePath());
  QStringList arguments = coreApp->arguments();
  arguments.removeFirst(); // Remove program name
#if defined (Q_OS_WIN32) && !defined (Cjyx_BUILD_WIN32_CONSOLE)
#else
  arguments.prepend("--disable-terminal-outputs");
#endif
  if (launcherAvailable)
    {
    QProcess::startDetached(coreApp->launcherExecutableFilePath(), arguments);
    }
  else
    {
    QProcess::startDetached(coreApp->applicationFilePath(), arguments);
    }
  QCoreApplication::quit();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::onCjyxApplicationLogicModified()
{
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::onUserInformationModified()
{
  vtkPersonInformation* userInfo = this->applicationLogic()->GetUserInformation();
  if (!userInfo)
    {
    return;
    }
  this->userSettings()->setValue("UserInformation", userInfo->GetAsString().c_str());
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication
::requestInvokeEvent(vtkObject* caller, void* callData)
{
  // This method can be called by any thread.
  Q_UNUSED(caller);
  vtkMRMLApplicationLogic::InvokeRequest* request =
    reinterpret_cast<vtkMRMLApplicationLogic::InvokeRequest *>(callData);
  // If the thread is the same as the main thread then it is executed directly,
  // otherwise it is queued to be executed by the main thread.
  emit invokeEventRequested(request->Delay, request->Caller,
                            request->EventID, request->CallData);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication
::scheduleInvokeEvent(unsigned int delay, void* caller,
                      unsigned long eventID, void* callData)
{
  QTimer* timer = new QTimer(this);
  timer->setSingleShot(true);
  timer->setProperty("caller", QVariant::fromValue(caller));
  timer->setProperty("eventID", QVariant::fromValue(eventID));
  timer->setProperty("callData", QVariant::fromValue(callData));
  timer->connect(timer, SIGNAL(timeout()),this, SLOT(invokeEvent()));
  timer->start(delay);
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication
::invokeEvent()
{
  QTimer* timer = qobject_cast<QTimer*>(this->sender());
  Q_ASSERT(timer);
  if (!timer)
    {
    return;
    }
  QVariant callerVariant = timer->property("caller");
  QVariant eventIDVariant = timer->property("eventID");
  QVariant callDataVariant = timer->property("callData");
  vtkObject* caller =
    reinterpret_cast<vtkObject*>(callerVariant.value<void*>());
  unsigned long eventID = eventIDVariant.toULongLong();
  void* callData = callDataVariant.value<void*>();
  if (caller)
    {
    caller->InvokeEvent(eventID, callData);
    }
  timer->deleteLater();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication
::onCjyxApplicationLogicRequest(vtkObject* appLogic, void* delay, unsigned long event)
{
  Q_D(qCjyxCoreApplication);
  Q_ASSERT(d->AppLogic.GetPointer() == vtkCjyxApplicationLogic::SafeDownCast(appLogic));
  Q_UNUSED(appLogic);
  Q_UNUSED(d);
  int delayInMs = *reinterpret_cast<int *>(delay);
  switch(event)
    {
    case vtkCjyxApplicationLogic::RequestModifiedEvent:
      QTimer::singleShot(delayInMs,
                         this, SLOT(processAppLogicModified()));
      break;
    case vtkCjyxApplicationLogic::RequestReadDataEvent:
      QTimer::singleShot(delayInMs,
                         this, SLOT(processAppLogicReadData()));
      break;
    case vtkCjyxApplicationLogic::RequestWriteDataEvent:
      QTimer::singleShot(delayInMs,
                         this, SLOT(processAppLogicWriteData()));
      break;
    default:
      break;
    }
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::processAppLogicModified()
{
  Q_D(qCjyxCoreApplication);
  d->AppLogic->ProcessModified();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::processAppLogicReadData()
{
  Q_D(qCjyxCoreApplication);
  d->AppLogic->ProcessReadData();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::processAppLogicWriteData()
{
  Q_D(qCjyxCoreApplication);
  d->AppLogic->ProcessWriteData();
}

//-----------------------------------------------------------------------------
void qCjyxCoreApplication::terminate(int returnCode)
{
  Q_D(qCjyxCoreApplication);
  d->ReturnCode = returnCode;
  // Does nothing if the event loop is not running
  this->exit(returnCode);
}

//----------------------------------------------------------------------------
void qCjyxCoreApplication::onAboutToQuit()
{
  Q_D(qCjyxCoreApplication);

  d->ModuleManager->factoryManager()->unloadModules();

#ifdef Cjyx_USE_PYTHONQT
  // Override return code only if testing mode is enabled
  if (this->corePythonManager()->pythonErrorOccured() && this->testAttribute(AA_EnableTesting))
    {
    d->ReturnCode = qCjyxCoreApplication::ExitFailure;
    }
#endif
}

//----------------------------------------------------------------------------
void qCjyxCoreApplication::loadTranslations(const QString& dir)
{
#ifdef Cjyx_BUILD_I18N_SUPPORT
  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  Q_ASSERT(app);

  QStringList qmFiles = qCjyxCoreApplicationPrivate::findTranslationFiles(dir, app->settings()->value("language").toString());

  foreach(QString qmFile, qmFiles)
    {
    QTranslator* translator = new QTranslator();
    QString qmFilePath = QString(dir + QString("/") + qmFile);

    if(!translator->load(qmFilePath))
      {
      qDebug() << "The File " << qmFile << " hasn't been loaded in the translator";
      return;
      }
    app->installTranslator(translator);
    }
#else
  Q_UNUSED(dir)
#endif
}

//----------------------------------------------------------------------------
QStringList qCjyxCoreApplication::translationFolders()
{
#ifdef Cjyx_BUILD_I18N_SUPPORT
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  if (!app)
    {
    return QStringList();
    }

  QStringList qmDirs;
  qmDirs << qCjyxCoreApplication::application()->toCjyxHomeAbsolutePath(QString(Cjyx_QM_DIR));

  // we check if the application is installed or not.
  if (!app->isInstalled())
    {
    qmDirs << QString(Cjyx_QM_OUTPUT_DIRS).split(";");
    }

  return qmDirs;
#else
  return QStringList();
#endif
}

//----------------------------------------------------------------------------
void qCjyxCoreApplication::loadLanguage()
{
#ifdef Cjyx_BUILD_I18N_SUPPORT
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  if (!app)
    {
    return;
    }
  QStringList qmDirs = qCjyxCoreApplication::translationFolders();
  foreach(QString qmDir, qmDirs)
    {
    app->loadTranslations(qmDir);
    }
#endif
}

//----------------------------------------------------------------------------
bool qCjyxCoreApplication::loadCaCertificates(const QString& cjyxHome)
{
#ifdef Cjyx_USE_PYTHONQT_WITH_OPENSSL
  if (QSslSocket::supportsSsl())
    {
    QSslSocket::setDefaultCaCertificates(
          QSslCertificate::fromPath(
            cjyxHome + "/" Cjyx_SHARE_DIR "/Cjyx.crt"));
    }
  return !QSslSocket::defaultCaCertificates().empty();
#else
  Q_UNUSED(cjyxHome);
  return false;
#endif
}

//----------------------------------------------------------------------------
int qCjyxCoreApplication::registerResource(const QByteArray& data)
{
  Q_D(qCjyxCoreApplication);

  const int handle = d->NextResourceHandle++;
  d->LoadedResources.insert(handle, data);

  const uchar* pdata =
    reinterpret_cast<const uchar*>(d->LoadedResources[handle].constData());

  if (!QResource::registerResource(pdata))
    {
    d->LoadedResources.remove(handle);
    return -1;
    }

  return handle;
}

//----------------------------------------------------------------------------
bool qCjyxCoreApplication::unregisterResource(int handle)
{
  Q_D(qCjyxCoreApplication);

  if (d->LoadedResources.contains(handle))
    {
    const uchar* pdata =
      reinterpret_cast<const uchar*>(d->LoadedResources[handle].constData());
    const bool result = QResource::unregisterResource(pdata);
    d->LoadedResources.remove(handle);
    return result;
    }

  return false;
}

// --------------------------------------------------------------------------
void qCjyxCoreApplication::addModuleAssociatedNodeType(const QString& nodeClassName, const QString& moduleName)
{
  Q_D(qCjyxCoreApplication);
  d->ModulesForNodes.insert(nodeClassName, moduleName);
}

// --------------------------------------------------------------------------
void qCjyxCoreApplication::removeModuleAssociatedNodeType(const QString& nodeClassName, const QString& moduleName)
{
  Q_D(qCjyxCoreApplication);
  d->ModulesForNodes.remove(nodeClassName, moduleName);
}

// --------------------------------------------------------------------------
QStringList qCjyxCoreApplication::modulesAssociatedWithNodeType(const QString& nodeClassName) const
{
  Q_D(const qCjyxCoreApplication);
  QList<QString> moduleNames = d->ModulesForNodes.values(nodeClassName);
  return moduleNames;
}

// --------------------------------------------------------------------------
QStringList qCjyxCoreApplication::allModuleAssociatedNodeTypes() const
{
  Q_D(const qCjyxCoreApplication);
  QList<QString> nodeClassNames = d->ModulesForNodes.uniqueKeys();
  return nodeClassNames;
}

// --------------------------------------------------------------------------
void qCjyxCoreApplication::showConsoleMessage(QString message, bool error/*=true*/) const
{
  Q_D(const qCjyxCoreApplication);
  if (error)
  {
    std::cerr << message.toLocal8Bit().constData() << std::endl;
  }
  else
  {
    std::cout << message.toLocal8Bit().constData() << std::endl;
  }
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::toCjyxHomeAbsolutePath(const QString& path) const
{
  Q_D(const qCjyxCoreApplication);
  return ctk::absolutePathFromInternal(path, d->CjyxHome);
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::toCjyxHomeRelativePath(const QString& path) const
{
  Q_D(const qCjyxCoreApplication);
  return ctk::internalPathFromAbsolute(path, d->CjyxHome);
}

// --------------------------------------------------------------------------
QStringList qCjyxCoreApplication::toCjyxHomeAbsolutePaths(const QStringList& paths) const
{
  Q_D(const qCjyxCoreApplication);
  QStringList absolutePaths;
  foreach(QString path, paths)
    {
    absolutePaths << this->toCjyxHomeAbsolutePath(path);
    }
  return absolutePaths;
}


// --------------------------------------------------------------------------
QStringList qCjyxCoreApplication::toCjyxHomeRelativePaths(const QStringList& paths) const
{
  Q_D(const qCjyxCoreApplication);
  QStringList relativePaths;
  foreach(QString path, paths)
    {
    relativePaths << this->toCjyxHomeRelativePath(path);
    }
  return relativePaths;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxCoreApplication::moduleLogic(const QString& moduleName)const
{
  Q_D(const qCjyxCoreApplication);
  vtkCjyxApplicationLogic* applicationLogic = this->applicationLogic();
  if (!applicationLogic)
    {
    return nullptr;
    }
  return applicationLogic->GetModuleLogic(moduleName.toUtf8());
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::documentationBaseUrl() const
{
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  // Since currently there is only English language documentation on readthedocs, the default URL uses "en" language.
  QString url = appSettings->value("DocumentationBaseURL", "https://slicer.readthedocs.io/en/{version}").toString();
  if (url.contains("{version}"))
    {
    url.replace("{version}", this->documentationVersion());
    }
  if (url.contains("{language}"))
    {
    url.replace("{language}", this->documentationLanguage());
    }
  return url;
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::documentationVersion() const
{
  QString version = "latest";
  if (this->releaseType() == "Stable")
    {
    version = QString("v%1.%2").arg(this->mainApplicationMajorVersion()).arg(this->mainApplicationMinorVersion());
    }
  return version;
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::documentationLanguage() const
{
  QString language = "en";
  if (this->userSettings()->value("Internationalization/Enabled", false).toBool())
    {
    language = this->userSettings()->value("language", language).toString();
    }
  return language;
}

// --------------------------------------------------------------------------
QString qCjyxCoreApplication::moduleDocumentationUrl(const QString& moduleName) const
{
  QSettings* appSettings = this->userSettings();
  Q_ASSERT(appSettings);
  QString url = appSettings->value("ModuleDocumentationURL",
    "{documentationbaseurl}/user_guide/modules/{lowercasemodulename}.html").toString();

  if (url.contains("{documentationbaseurl}"))
    {
    url.replace("{documentationbaseurl}", this->documentationBaseUrl());
    }

  if (url.contains("{lowercasemodulename}"))
    {
    url.replace("{lowercasemodulename}", moduleName.toLower());
    }

  return url;
}

//------------------------------------------------------------------------------
bool qCjyxCoreApplication::loadFiles(const QStringList& filePaths, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  bool success = true;
  foreach(QString filePath, filePaths)
    {
    QFileInfo file(filePath);
    qCjyxCoreIOManager* ioManager = this->coreIOManager();
    qCjyxIO::IOFileType fileType = ioManager->fileType(filePath);
    qCjyxIO::IOProperties fileProperties;
    // It is important to use absolute file path, as in the scene relative path
    // always relative to the .mrml scene file (while the user specified the path
    // relative to the current working directory)
    fileProperties.insert("fileName", file.absoluteFilePath());
    if (!ioManager->loadNodes(fileType, fileProperties, nullptr, userMessages))
      {
      success = false;
      }
    }
  return success;
}

//------------------------------------------------------------------------------
void qCjyxCoreApplication::openUrl(const QString& url)
{
  emit urlReceived(url);
}
