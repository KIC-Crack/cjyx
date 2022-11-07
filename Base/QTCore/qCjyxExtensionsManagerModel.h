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

#ifndef __qCjyxExtensionsManagerModel_h
#define __qCjyxExtensionsManagerModel_h

// Qt includes
#include <QHash>
#include <QStringList>
#include <QUrl>
#include <QSettings>
#include <QUuid>
#include <QVariantMap>

// CTK includes
#include <ctkErrorLogModel.h>

// QtGUI includes
#include "qCjyxBaseQTCoreExport.h"
#include "qCjyxExtensionDownloadTask.h"

class QNetworkReply;
class qCjyxExtensionsManagerModelPrivate;
class QStandardItemModel;

/// \brief Class querying and storing extensions data
///
/// The model maintains a list of "managed" extensions, i.e., extensions that
/// are currently installed or that the user bookmarked.
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxExtensionsManagerModel : public QObject
{
  Q_OBJECT
  Q_PROPERTY(int numberOfInstalledExtensions READ numberOfInstalledExtensions NOTIFY modelUpdated)
  Q_PROPERTY(int installedExtensionsCount READ installedExtensionsCount NOTIFY modelUpdated)
  /// Number of extensions that are installed or bookmarked
  Q_PROPERTY(int managedExtensionsCount READ managedExtensionsCount NOTIFY modelUpdated)
  /// Names of extensions that are installed or bookmarked
  Q_PROPERTY(QStringList managedExtensions READ managedExtensions NOTIFY modelUpdated)
  /// Names of bookmarked extensions
  Q_PROPERTY(QStringList bookmarkedExtensions READ bookmarkedExtensions NOTIFY modelUpdated)
  Q_PROPERTY(QStringList installedExtensions READ installedExtensions NOTIFY modelUpdated)
  Q_PROPERTY(QStringList enabledExtensions READ enabledExtensions NOTIFY modelUpdated)
  /// Names of extensions that have updates available on the server
  Q_PROPERTY(QStringList availableUpdateExtensions READ availableUpdateExtensions NOTIFY modelUpdated)
  Q_PROPERTY(bool newExtensionEnabledByDefault READ newExtensionEnabledByDefault WRITE setNewExtensionEnabledByDefault NOTIFY newExtensionEnabledByDefaultChanged)
  Q_PROPERTY(bool interactive READ interactive WRITE setInteractive NOTIFY interactiveChanged)
  Q_PROPERTY(QString extensionsSettingsFilePath READ extensionsSettingsFilePath WRITE setExtensionsSettingsFilePath NOTIFY extensionsSettingsFilePathChanged)
  Q_PROPERTY(QString cjyxRevision READ cjyxRevision WRITE setCjyxRevision NOTIFY cjyxRevisionChanged)
  Q_PROPERTY(QString cjyxOs READ cjyxOs WRITE setCjyxOs NOTIFY cjyxOsChanged)
  Q_PROPERTY(QString cjyxArch READ cjyxArch WRITE setCjyxArch NOTIFY cjyxArchChanged)
  /// Download extensions metadata from the server at application startup to allow the application to display update available indicator.
  Q_PROPERTY(bool autoUpdateCheck READ autoUpdateCheck WRITE setAutoUpdateCheck NOTIFY autoUpdateSettingsChanged)
  /// Automatically install any extension updates.
  Q_PROPERTY(bool autoUpdateInstall READ autoUpdateInstall WRITE setAutoUpdateInstall NOTIFY autoUpdateSettingsChanged)
  /// Automatically install all dependencies (other extensions that the installed extension requires) when installing an extension.
  Q_PROPERTY(bool autoInstallDependencies READ autoInstallDependencies WRITE setAutoInstallDependencies NOTIFY autoUpdateSettingsChanged)

public:
  /// Superclass typedef
  typedef QObject Superclass;

  /// Pimpl typedef
  typedef qCjyxExtensionsManagerModelPrivate Pimpl;

  /// Self typedef
  typedef qCjyxExtensionsManagerModel Self;

  /// Constructor
  explicit qCjyxExtensionsManagerModel(QObject* parent = nullptr);

  /// Destructor
  ~qCjyxExtensionsManagerModel() override;

  /// \brief Extension metadata typedef
  typedef QVariantMap ExtensionMetadataType;

  enum ServerAPI
    {
    Girder_v1 = 1,
    ServerAPI_Last
    };

  /// Return current serverAPI.
  ///
  /// \sa serverAPIFromString()
  int serverAPI() const;

  /// Return a string representation of the \a serverAPI.
  ///
  /// \sa serverAPIFromString()
  static QString serverAPIToString(int serverAPI);

  /// Return ServerAPI from the string \a str.
  ///
  /// \sa serverAPIToString()
  static int serverAPIFromString(const QString& str);

  /// \brief Backend server URL used to perform API calls.
  ///
  /// If set, it returns the value associated with `CJYX_EXTENSIONS_MANAGER_SERVER_URL`
  /// environment variable. Otherwise, it returns the extensions settings value `Extensions/ServerUrl`.
  ///
  /// \sa extensionsSettingsFilePath()
  Q_INVOKABLE QUrl serverUrl()const;

  /// \brief Frontend server URL displaying extension manager web page.
  ///
  /// If set, it returns the value associated with `CJYX_EXTENSIONS_MANAGER_FRONTEND_SERVER_URL`
  /// environment variable. Otherwise, it returns the extensions settings value `Extensions/FrontendServerUrl`.
  ///
  /// \sa extensionsSettingsFilePath()
  Q_INVOKABLE QUrl frontendServerUrl()const;

  /// Returns the URL of the extensions manager frontend for the current application version and operating system.
  Q_INVOKABLE QUrl extensionsListUrl()const;

  Q_INVOKABLE QString extensionsInstallPath()const;

  Q_INVOKABLE QString extensionInstallPath(const QString& extensionName) const;

  Q_INVOKABLE QStringList extensionModulePaths(const QString& extensionName)const;

  Q_INVOKABLE QString extensionDescriptionFile(const QString& extensionName) const;

  void setNewExtensionEnabledByDefault(bool value);
  bool newExtensionEnabledByDefault()const;

  bool autoUpdateCheck()const;
  bool autoUpdateInstall()const;
  bool autoInstallDependencies()const;

  /// If set to true (by default) then the user may be asked to confirm installation of additional dependencies.
  /// If set to false then no blocking popups are displayed and dependencies are installed automatically.
  bool interactive()const;

  enum MatadataSource
    {
    MetadataAll = 0, ///< return local metadata, and if any fields are not set locally then set it from the server
    MetadataLocal,   ///< return local metadata (stored in s4ext files in the extensions folder)
    MetadataServer   ///< return metadata downloaded from the server
    };
  Q_INVOKABLE ExtensionMetadataType extensionMetadata(const QString& extensionName, int source = MetadataAll)const;

  Q_INVOKABLE QString extensionDescription(const QString& extensionName)const;

  /// \brief Return the number of managed extensions
  /// \sa managedExtensionsCount, installedExtensionsCount, bookmarkedExtensionsCount
  int managedExtensionsCount()const;

  /// \brief Return names of all managed extensions, i.e., installed or bookmarked extensions
  QStringList managedExtensions()const;

  /// \brief Return True if the \a extensionName is loaded
  bool isExtensionLoaded(const QString& extensionName) const;

  /// \brief Return True if the \a extensionName is installed
  /// \sa installExtension, installedExtensionsCount, installedExtensions, extensionInstalled
  Q_INVOKABLE bool isExtensionInstalled(const QString& extensionName)const;

  /// \brief Return the number of installed extensions
  /// \sa installExtension, isExtensionInstalled, installedExtensions, extensionInstalled
  int installedExtensionsCount()const;

  /// \brief Return the number of installed extensions. Deprecated, use installedExtensionsCount instead.
  /// \sa installExtension, isExtensionInstalled, installedExtensions, extensionInstalled
  int numberOfInstalledExtensions()const;

  /// \brief Return names of all installed extensions sorted in alphabetical order.
  /// \sa installExtension, installedExtensionsCount, isExtensionInstalled, extensionInstalled
  QStringList installedExtensions()const;

  /// \brief Return True if the \a extensionName is bookmarked.
  /// Bookmarked extensions are included in the list of managed extensions list, even if not installed.
  /// \sa setExtensionBookmarked, extensionBookmarkedChanged, bookmarkedExtensions
  Q_INVOKABLE bool isExtensionBookmarked(const QString& extensionName)const;

  /// \brief Return True if the \a extensionName is enabled
  /// \sa setExtensionEnabled, extensionEnabledChanged, enabledExtensions
  Q_INVOKABLE bool isExtensionEnabled(const QString& extensionName)const;

  /// Get the names of all extensions scheduled for update.
  ///
  /// \sa scheduleExtensionForUpdate, isExtensionScheduledForUpdate,
  ///     extensionScheduledForUpdate
  QStringList scheduledForUpdateExtensions() const;

  /// Check if an update is known to be available for the specified extension.
  ///
  /// \return \c true if a previous check for updates has determined that an
  ///         update is available for the specified extension.
  ///
  /// \sa checkForUpdates
  Q_INVOKABLE bool isExtensionUpdateAvailable(const QString& extensionName)const;

  /// Get list of extension names that has available updates.
  /// \sa checkForUpdates
  QStringList availableUpdateExtensions()const;

  /// Test if extension is scheduled to be updated.
  ///
  /// \return \c true if \p extensionName is scheduled to be updated.
  ///
  /// \sa updateScheduledExtensions();
  Q_INVOKABLE bool isExtensionScheduledForUpdate(const QString& extensionName)const;

  /// \brief Return names of all extensions scheduled for uninstall
  /// \sa scheduleExtensionForUninstall, isExtensionScheduledForUninstall, extensionScheduledForUninstall
  QStringList scheduledForUninstallExtensions() const;

  /// \brief Return True if the \a extensionName is scheduled to be uninstalled
  /// \sa uninstallScheduledExtensions();
  Q_INVOKABLE bool isExtensionScheduledForUninstall(const QString& extensionName)const;

  /// \brief Return names of all bookmarked extensions
  /// \sa setExtensionBookmarked, extensionBookmarkedChanged, bookmarkedExtensions
  QStringList bookmarkedExtensions()const;

  /// \brief Return names of all enabled extensions sorted in alphabetical order.
  /// \sa setExtensionEnabled, extensionEnabledChanged, isExtensionEnabled
  QStringList enabledExtensions()const;

  /// \brief Set/Get extension settings file path.
  ///
  /// Signal extensionsSettingsFilePathChanged() is emitted when a new path is set.
  QString extensionsSettingsFilePath()const;
  void setExtensionsSettingsFilePath(const QString& extensionsSettingsFilePath);

  /// \brief Set/Get Cjyx revision.
  ///
  /// Signal cjyxRevisionChanged() is emitted when a revision is set.
  QString cjyxRevision()const;
  void setCjyxRevision(const QString& revision);

  /// \brief Set/Get Cjyx operating system.
  ///
  /// Signal cjyxOsChanged() is emitted when a new operating system is set.
  QString cjyxOs()const;
  void setCjyxOs(const QString& os);

  /// \brief Set/Get Cjyx architecture.
  ///
  /// Signal cjyxArchChanged() is emitted when a new architecture is set.
  QString cjyxArch()const;
  void setCjyxArch(const QString& arch);

  /// \brief Convenience function setting Cjyx revision, operating system and architecture.
  ///
  /// Signal cjyxRevisionChanged(), cjyxArchChanged() and cjyxArchChanged() are emitted
  /// only if the corresponding value is updated.
  ///
  /// The, signal cjyxRequirementsChanged() is emitted only once it at least one of the
  /// three properties has been updated.
  void setCjyxRequirements(const QString& revision, const QString& os, const QString& arch);

  QString cjyxVersion()const;
  void setCjyxVersion(const QString& version);

  /// \brief Check if \a extensionName is compatible with the system identified
  /// by \a cjyxRevision, \a cjyxOs and \a cjyxArch.
  /// @return Return the reasons justifying the incompatibility or an empty list if the extension
  /// is compatible.
  Q_INVOKABLE QStringList isExtensionCompatible(const QString& extensionName, const QString& cjyxRevision,
                                                const QString& cjyxOs, const QString& cjyxArch) const;

  /// \brief Check if \a extensionName is compatible.
  /// An extension is considered incompatible when the version of Cjyx used
  /// to build the extension is different from the version of Cjyx attempting
  /// to load the extension.
  /// \sa isExtensionCompatible(const QString&, const QString&, const QString&)
  /// \sa setCjyxRevision, setCjyxOs, setCjyxArch, setCjyxRequirements
  Q_INVOKABLE QStringList isExtensionCompatible(const QString& extensionName) const;

  /// Install extension from the specified archive file.
  ///
  /// This attempts to install an extension given only the archive file
  /// containing the extension. The archive file is inspected in order to
  /// determine the extension name.
  ///
  /// \sa installExtension(const QString&,ExtensionMetadataType,const QString&)
  Q_INVOKABLE bool installExtension(const QString &archiveFile, bool installDependencies = true);

  /// Install extension.
  ///
  /// This attempts to install an extension with the specified name and
  /// metadata from the specified archive file. If the metadata is empty, the
  /// metadata from the extension description contained in the archive is used.
  ///
  /// \sa isExtensionScheduledForUninstall, extensionScheduledForUninstall
  Q_INVOKABLE bool installExtension(const QString& extensionName,
                                    ExtensionMetadataType extensionMetadata,
                                    const QString &archiveFile, bool installDependencies = true);

  /// \brief Uninstall \a extensionName
  /// It is only allowed if the extension is not loaded already.
  /// If the extension is already loaded then use scheduleExtensionForUninstall instead.
  /// \note The directory containing the extension will be deleted.
  /// \sa installExtension
  bool uninstallExtension(const QString& extensionName);

  /// \brief Extract \a archiveFile into \a destinationPath/extensionName directory
  Q_INVOKABLE bool extractExtensionArchive(const QString& extensionName,
                                           const QString& archiveFile,
                                           const QString &destinationPath);

  /// Return the item model used internally
  Q_INVOKABLE const QStandardItemModel * model()const;

  /// Return time of last successful update of extensions metadata from the server.
  /// If there has not been any updates then it the object is set to null.
  QDateTime lastUpdateTimeExtensionsMetadataFromServer();

  /// Number of operations in progress
  QStringList activeTasks() const;

  /// Conversion map to get extensions manager model keys from metadata returned by the extensions server
  /// \sa convertExtensionMetadata()
  static QHash<QString, QString> serverToExtensionDescriptionKey(int serverAPI);

  /// Convert server keys to extensions manager model keys.
  /// \sa serverToExtensionDescriptionKey()
  static ExtensionMetadataType convertExtensionMetadata(const ExtensionMetadataType &extensionMetadata, int serverAPI);

  /// Server metadata fields that should not be copied to the extensions manager model.
  /// \sa filterExtensionMetadata()
  static QStringList serverKeysToIgnore(int serverAPI);

  /// Remove server metadata fields that should be ignored.
  /// \sa serverKeysToIgnore()
  static ExtensionMetadataType filterExtensionMetadata(const ExtensionMetadataType &extensionMetadata, int serverAPI);

  static QStringList readArrayValues(QSettings& settings,
                                     const QString& arrayName, const QString fieldName);

  static void writeArrayValues(QSettings& settings, const QStringList& values,
                               const QString& arrayName, const QString fieldName);

  static bool writeExtensionDescriptionFile(const QString& file,
                                            const ExtensionMetadataType& metadata);

  static ExtensionMetadataType parseExtensionDescriptionFile(const QString& file);

public slots:

  /// \brief Add/remove bookmark for an extension.
  /// Add/remove this extension from the list of bookmarked extensions in the application settings.
  void setExtensionBookmarked(const QString& extensionName, bool value);

  /// \brief Enable or disable an extension.
  /// Tell the application to load (or skip the loading) of \a extensionName
  /// by adding (or removing) all associated module paths to the application settings.
  void setExtensionEnabled(const QString& extensionName, bool value);

  /// \brief Download and install \a extensionId
  /// The \a extensionId corresponds to the identifier used on the extension server itself.
  /// \sa installExtension, scheduleExtensionForUninstall, uninstallScheduledExtensions
  bool downloadAndInstallExtension(const QString& extensionId, bool installDependencies=true);

  /// \brief Download and install \a extensionId
  /// The \a extensionId corresponds to the identifier used on the extension server itself.
  /// This method is used by the extensions.cjyx.org extension installer.
  /// \sa installExtension, scheduleExtensionForUninstall, uninstallScheduledExtensions
  bool downloadAndInstallExtensionByName(const QString& extensionId, bool installDependencies=true);

  /// \brief Schedule \a extensionName of uninstall
  /// Tell the application to uninstall \a extensionName when it will restart
  /// An extension scheduled for uninstall can be effectively uninstalled by calling
  /// uninstallScheduledExtensions()
  /// \sa isExtensionScheduledForUninstall, uninstallScheduledExtensions
  bool scheduleExtensionForUninstall(const QString& extensionName);

  /// \brief Cancel the uninstallation of \a extensionName
  /// Tell the application to keep \a extensionName installed
  /// \sa scheduleExtensionForUninstall
  bool cancelExtensionScheduledForUninstall(const QString& extensionName);

  /// Request updating the extension metadata that is stored on the extensions server.
  /// Only queries the extensions server if update is due (sufficient time
  /// is elapsed since the last update), or force is set to true.
  /// The last query result is saved and if the server is not contacted
  /// then this cached information is used.
  /// Set waitForCompletion to true to make sure all metadata is up-to-date when the method returns.
  /// Returns false if waitForCompletion is set to true and metadata cannot be retrieved.
  bool updateExtensionsMetadataFromServer(bool force=false, bool waitForCompletion=false);

  /// Compares current extensions versions with versions available on the server.
  /// Emits extensionMetadataUpdated(QString extensionName) and emit extensionUpdatesAvailable(bool found) signals.
  /// If Extensions/AutoUpdateInstall is enabled in application settings then this will also install the updated extensions.
  void checkForExtensionsUpdates();

  /// Schedule \p extensionName to be updated (reinstalled).
  ///
  /// This records \p extensionName in the list of extensions scheduled to be
  /// updated (which is done by reinstalling the extension at next startup).
  ///
  /// \sa isExtensionScheduledForUpdate, updateScheduledExtensions
  bool scheduleExtensionForUpdate(const QString& extensionName);

  /// \brief Cancel the update of \a extensionName
  /// Tell the application to keep \a extensionName installed
  /// \sa scheduleExtensionForUninstall
  bool cancelExtensionScheduledForUpdate(const QString& extensionName);

  /// Update extensions scheduled for update.
  ///
  /// \param updatedExtensions
  ///   QStringList which received the list of extensions which are
  ///   successfully updated.
  /// \return \c true if all scheduled extensions are successfully updated.
  ///
  /// \sa scheduleExtensionForUpdate, isExtensionScheduledForUpdate
  bool updateScheduledExtensions(QStringList &updatedExtensions);

  /// Update extensions scheduled for update.
  ///
  /// \return \c true if all scheduled extensions are successfully updated.
  ///
  /// \sa scheduleExtensionForUpdate, isExtensionScheduledForUpdate
  bool updateScheduledExtensions();

  /// Uninstall extensions scheduled for uninstall.
  ///
  /// \param uninstalledExtensions
  ///   QStringList which received the list of extensions which are
  ///   successfully uninstalled.
  /// \return \c true if all scheduled extensions are successfully uninstalled.
  ///
  /// \sa scheduleExtensionForUninstall, isExtensionScheduledForUninstall,
  bool uninstallScheduledExtensions(QStringList &uninstalledExtensions);

  /// Uninstall extensions scheduled for uninstall.
  ///
  /// \return \c true if all scheduled extensions are successfully uninstalled.
  ///
  /// \sa scheduleExtensionForUninstall, isExtensionScheduledForUninstall,
  bool uninstallScheduledExtensions();

  void identifyIncompatibleExtensions();

  bool exportExtensionList(QString& exportFilePath);

  QStringList checkInstallPrerequisites() const;

  /// Call this method when extensions are started to be loaded.
  /// It allows to the model to know which extensions are loaded,
  /// which is important because after application startup extensions
  /// cannot be immediately loaded or unloaded.
  void aboutToLoadExtensions();

  /// Full update of the extensions from extension description files and settings.
  void updateModel();

  void setAutoUpdateCheck(bool enable);
  void setAutoUpdateInstall(bool enable);
  void setAutoInstallDependencies(bool enable);
  void setInteractive(bool value);

signals:

  void downloadStarted(QNetworkReply * reply);

  void downloadFinished(QNetworkReply * reply);

  void updateDownloadProgress(const QString& extensionName,
                              qint64 received, qint64 total);

  void modelUpdated();

  /// Emitted when metadata download from the extensions server is completed.
  void updateExtensionsMetadataFromServerCompleted(bool success);

  void extensionUpdateAvailable(const QString& extensionName);

  /// Emitted after updates are checked on the extensions server.
  void extensionUpdatesAvailable(bool available);

  void extensionInstalled(const QString& extensionName);

  void extensionUpdated(const QString& extensionName);

  /// Emitted when extension metadata (description, icon URL, etc. is updated)
  void extensionMetadataUpdated(const QString& extensionName);

  void extensionScheduledForUninstall(const QString& extensionName);

  void extensionCancelledScheduleForUninstall(const QString& extensionName);

  void extensionScheduledForUpdate(const QString& extensionName);

  void extensionCancelledScheduleForUpdate(const QString& extensionName);

  void extensionUninstalled(const QString& extensionName);

  /// Emitted when bookmark is added to or removed from an extension
  void extensionBookmarkedChanged(const QString& extensionName, bool value);

  void extensionEnabledChanged(const QString& extensionName, bool value);

  void extensionIdentifedAsIncompatible(const QString& extensionName);

  void newExtensionEnabledByDefaultChanged(bool value);

  void cjyxRequirementsChanged(const QString& revision, const QString& os, const QString& arch);
  void cjyxArchChanged(const QString& cjyxArch);
  void cjyxOsChanged(const QString& cjyxOs);
  void cjyxRevisionChanged(const QString& cjyxRevision);

  void interactiveChanged(bool interactive);

  void cjyxVersionChanged(const QString& cjyxVersion);

  void messageLogged(const QString& text, ctkErrorLogLevel::LogLevels level) const;

  void installDownloadProgress(const QString& extensionName, qint64 received, qint64 total);

  void extensionsSettingsFilePathChanged(const QString& extensionsSettingsFilePath);

  /// Emitted when autoUpdateCheck, autoUpdateInstall, or autoInstallDependencies properties are changed
  void autoUpdateSettingsChanged();

protected slots:

  void onInstallDownloadProgress(qCjyxExtensionDownloadTask* task, qint64 received, qint64 total);

  /// \sa downloadAndInstallExtension
  void onInstallDownloadFinished(qCjyxExtensionDownloadTask* task);

  /// \sa scheduleExtensionForUpdate
  void onUpdateDownloadFinished(qCjyxExtensionDownloadTask* task);

  void onUpdateDownloadProgress(qCjyxExtensionDownloadTask* task,
                                qint64 received, qint64 total);

  bool onExtensionsMetadataFromServerQueryFinished(const QUuid& requestId);

protected:
  QScopedPointer<qCjyxExtensionsManagerModelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxExtensionsManagerModel);
  Q_DISABLE_COPY(qCjyxExtensionsManagerModel);
};

Q_DECLARE_METATYPE(qCjyxExtensionsManagerModel::ServerAPI);

// Metatype already declared in qCjyxIO.h
//Q_DECLARE_METATYPE(qCjyxExtensionsManagerModel::ExtensionMetadataType)

#endif
