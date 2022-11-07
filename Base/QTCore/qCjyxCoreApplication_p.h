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

#ifndef __qCjyxCoreApplication_p_h
#define __qCjyxCoreApplication_p_h

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Cjyx API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

// Qt includes
#include <QPointer>
#include <QProcessEnvironment>
#include <QSettings>
#include <QSharedPointer>

// CTK includes
#include <ctkErrorLogAbstractModel.h>

// Cjyx includes
#include "qCjyxBaseQTCoreExport.h"
#include "qCjyxCoreApplication.h"

// VTK includes
#include <vtkSmartPointer.h>

class vtkCacheManager;
class vtkDataIOManagerLogic;
class vtkMRMLRemoteIOLogic;

//-----------------------------------------------------------------------------
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxCoreApplicationPrivate
{
  Q_DECLARE_PUBLIC(qCjyxCoreApplication);
protected:
  qCjyxCoreApplication* q_ptr;
public:
  typedef qCjyxCoreApplicationPrivate Self;

  qCjyxCoreApplicationPrivate(qCjyxCoreApplication& object,
                                qCjyxCoreCommandOptions * coreCommandOptions,
                                qCjyxCoreIOManager * coreIOManager);
  virtual ~qCjyxCoreApplicationPrivate();

  virtual void init();

  /// Terminates the calling process "immediately".
  void quickExit(int exitCode);

  /// Set up the local and remote data input/output for this application.
  /// Use this as a template for creating stand alone scenes, then call
  /// vtkCjyxApplicationLogic::SetMRMLSceneDataIO to hook it into a scene.
  virtual void initDataIO();

  /// Instantiate settings object
  virtual QSettings* newSettings();
  QSettings* instantiateSettings(bool useTmp);

  /// Return true is this instance of Cjyx is running from an installed directory
  bool isInstalled(const QString& cjyxHome)const;

  /// Given the program name, should return Cjyx Home Directory
  QString discoverCjyxHomeDirectory();

#ifdef Cjyx_USE_PYTHONQT
  void setPythonOsEnviron(const QString& key, const QString& value);
#endif

#ifdef Q_WS_WIN
  void updatePythonOsEnviron();
#endif

  /// Prepend or append value to environment variable using \a separator
  void updateEnvironmentVariable(
    const QString& key, const QString& value, QChar separator, bool prepend = false);

  /// If it successfully obtains 'applicationDirPath()', returns Cjyx binary directory
  /// \note CjyxBin doesn't contain Debug/Release/... (see IntDir)
  /// \sa QCoreApplication::applicationDirPath
  QString discoverCjyxBinDirectory();

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  QString defaultExtensionsInstallPathForMacOSX()const;
#endif

  /// \brief Return true if application was started using CTKAppLauncher
  ///
  /// Value is currently hardcoded:
  ///
  ///         | Build tree | Install tree
  /// --------| -----------|---------------
  /// Linux   |  true      |  true
  /// macOS   |  true      |  false
  /// Windows |  true      |  true
  ///
  bool isUsingLauncher()const;

  /// Convenient function used to create a \a path.
  /// If it fails, print an error message using given \a description
  bool createDirectory(const QString& path, const QString& description) const;

  /// Parse arguments
  void parseArguments();

  /// \brief Returns list of translation files contained in given \a dir for the input \a settingsLanguage
  ///
  /// If \a settingsLanguage is empty returns an empty list (application default language)
  /// If \a settingsLanguage is not empty try to find the translation files from specific extension to generic extension
  /// For example when \a settingsLanguage = "en_US", translation files ending with "en_US.qm" will be searched first
  /// if no files are found then files ending with "en.qm" will be searched.
  static QStringList findTranslationFiles(const QString& dir, const QString& settingsLanguage);

  /// \brief Returns list of translation files contained in given \a dir for the input \a languageExtension
  static QStringList findTranslationFilesWithLanguageExtension(const QString& dir, const QString& languageExtension);

public:
  /// MRMLScene and AppLogic pointers
  vtkSmartPointer<vtkMRMLScene>               MRMLScene;
  vtkSmartPointer<vtkCjyxApplicationLogic>  AppLogic;
  vtkSmartPointer<vtkMRMLRemoteIOLogic>       MRMLRemoteIOLogic;

  /// Data manager
  vtkSmartPointer<vtkDataIOManagerLogic>      DataIOManagerLogic;

  QString                                     CjyxHome;
  /// On windows platform, after the method 'discoverCjyxBinDirectory' has been called,
  /// IntDir should be set to either Debug,
  /// Release, RelWithDebInfo, MinSizeRel or any other custom build type.
  QString                                     IntDir;

  /// Current working directory at the time the application was started.
  QString StartupWorkingPath;

  QSettings*                                  DefaultSettings;
  QSettings*                                  UserSettings;
  QSettings*                                  RevisionUserSettings;

  /// ModuleManager - It should exist only one instance of the factory
  QSharedPointer<qCjyxModuleManager>        ModuleManager;

  /// CoreIOManager - It should exist only one instance of the IOManager
  QSharedPointer<qCjyxCoreIOManager>        CoreIOManager;

  /// CoreCommandOptions - It should exist only one instance of the CoreCommandOptions
  QSharedPointer<qCjyxCoreCommandOptions>   CoreCommandOptions;

  /// ErrorLogModel - It should exist only one instance of the ErrorLogModel
  QSharedPointer<ctkErrorLogAbstractModel> ErrorLogModel;

  /// ReturnCode flag
  int                                         ReturnCode;

#ifdef Cjyx_USE_PYTHONQT
  /// CorePythonManager - It should exist only one instance of the CorePythonManager
  QSharedPointer<qCjyxCorePythonManager>    CorePythonManager;
  QPointer<ctkPythonConsole> PythonConsole; // it may be owned by a widget, so we cannot refer to it by a strong pointer
#endif

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  QSharedPointer<qCjyxExtensionsManagerModel> ExtensionsManagerModel;
#endif

  QProcessEnvironment                         Environment;

#ifdef Cjyx_BUILD_DICOM_SUPPORT
  /// Application-wide database instance
  QSharedPointer<ctkDICOMDatabase>            DICOMDatabase;
#endif

  QHash<int, QByteArray>                      LoadedResources;
  int                                         NextResourceHandle;

  /// Associated modules for each node type.
  /// Key: node class name; values: module names.
  QMultiMap<QString, QString> ModulesForNodes;
};

#endif

