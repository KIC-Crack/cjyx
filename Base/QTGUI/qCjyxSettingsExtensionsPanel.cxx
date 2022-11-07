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

// Qt includes
#include <QMainWindow>
#include <QSettings>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxExtensionsManagerModel.h"
#include "qCjyxModuleSelectorToolBar.h"
#include "qCjyxRelativePathMapper.h"
#include "qCjyxSettingsExtensionsPanel.h"
#include "ui_qCjyxSettingsExtensionsPanel.h"

// --------------------------------------------------------------------------
// qCjyxSettingsExtensionsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsExtensionsPanelPrivate: public Ui_qCjyxSettingsExtensionsPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsExtensionsPanel);
protected:
  qCjyxSettingsExtensionsPanel* const q_ptr;

public:
  qCjyxSettingsExtensionsPanelPrivate(qCjyxSettingsExtensionsPanel& object);
  void init();
};

// --------------------------------------------------------------------------
// qCjyxSettingsExtensionsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsExtensionsPanelPrivate::qCjyxSettingsExtensionsPanelPrivate(qCjyxSettingsExtensionsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsExtensionsPanelPrivate::init()
{
  Q_Q(qCjyxSettingsExtensionsPanel);

  this->setupUi(q);

  qCjyxApplication * app = qCjyxApplication::application();

  // Default values
  this->ExtensionsManagerEnabledCheckBox->setChecked(true);
  if (app->extensionsManagerModel()->serverAPI() == qCjyxExtensionsManagerModel::Girder_v1)
    {
    this->ExtensionsServerUrlLineEdit->setText("https://slicer-packages.kitware.com");
    this->ExtensionsFrontendServerUrlLineEdit->setText("https://extensions.slicer.org");
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << app->extensionsManagerModel()->serverAPI();
    }
  this->ExtensionsInstallPathButton->setDirectory(app->defaultExtensionsInstallPath());
#ifdef Q_OS_MAC
  this->ExtensionsInstallPathButton->setDisabled(true);
#endif
  this->AutoUpdateCheckCheckBox->setChecked(false);
  this->AutoUpdateInstallCheckBox->setChecked(false);
  this->AutoInstallDependenciesCheckBox->setChecked(true);

  bool extensionsManagerEnabled = app && app->revisionUserSettings()->value("Extensions/ManagerEnabled").toBool();
  this->OpenExtensionsManagerPushButton->setVisible(extensionsManagerEnabled);

  // Register settings
  q->registerProperty("Extensions/ManagerEnabled", this->ExtensionsManagerEnabledCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Enable/Disable extensions manager", ctkSettingsPanel::OptionRequireRestart,
                      app->revisionUserSettings());
  q->registerProperty("Extensions/ServerUrl", this->ExtensionsServerUrlLineEdit,
                      "text", SIGNAL(textChanged(QString)),
                      QString(), ctkSettingsPanel::OptionNone,
                      app->revisionUserSettings());
  q->registerProperty("Extensions/FrontendServerUrl", this->ExtensionsFrontendServerUrlLineEdit,
                      "text", SIGNAL(textChanged(QString)),
                      QString(), ctkSettingsPanel::OptionNone,
                      app->revisionUserSettings());

  q->registerProperty("Extensions/AutoUpdateCheck", this->AutoUpdateCheckCheckBox, "checked",
    SIGNAL(toggled(bool)), "Automatic update check");
  q->registerProperty("Extensions/AutoUpdateInstall", this->AutoUpdateInstallCheckBox, "checked",
    SIGNAL(toggled(bool)), "Automatic update install");
  q->registerProperty("Extensions/AutoInstallDependencies", this->AutoInstallDependenciesCheckBox, "checked",
    SIGNAL(toggled(bool)), "Automatic install of dependencies");

  qCjyxRelativePathMapper* relativePathMapper = new qCjyxRelativePathMapper(
    this->ExtensionsInstallPathButton, "directory", SIGNAL(directoryChanged(QString)));
  q->registerProperty("Extensions/InstallPath", relativePathMapper,
                      "relativePath", SIGNAL(relativePathChanged(QString)),
                      QString(), ctkSettingsPanel::OptionNone,
                      app->revisionUserSettings());

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->ExtensionsManagerEnabledCheckBox, SIGNAL(toggled(bool)),
    q, SLOT(onExtensionsManagerEnabled(bool)));
  QObject::connect(this->ExtensionsServerUrlLineEdit, SIGNAL(textChanged(QString)),
    q, SIGNAL(extensionsServerUrlChanged(QString)));
  QObject::connect(this->ExtensionsFrontendServerUrlLineEdit, SIGNAL(textChanged(QString)),
    q, SIGNAL(extensionsFrontendServerUrlChanged(QString)));
  QObject::connect(this->ExtensionsInstallPathButton, SIGNAL(directoryChanged(QString)),
    q, SLOT(onExtensionsPathChanged(QString)));
  QObject::connect(this->OpenExtensionsManagerPushButton, SIGNAL(clicked()),
    app, SLOT(openExtensionsManagerDialog()));
  QObject::connect(this->OpenExtensionsCatalogWebsitePushButton, SIGNAL(clicked()),
    app, SLOT(openExtensionsCatalogWebsite()));
  QObject::connect(app->extensionsManagerModel(), SIGNAL(autoUpdateSettingsChanged()),
    q, SLOT(updateAutoUpdateWidgetsFromModel()));
  QObject::connect(this->AutoUpdateCheckCheckBox, SIGNAL(toggled(bool)),
    app->extensionsManagerModel(), SLOT(setAutoUpdateCheck(bool)));
  QObject::connect(this->AutoUpdateInstallCheckBox, SIGNAL(toggled(bool)),
    app->extensionsManagerModel(), SLOT(setAutoUpdateInstall(bool)));
  QObject::connect(this->AutoInstallDependenciesCheckBox, SIGNAL(toggled(bool)),
    app->extensionsManagerModel(), SLOT(setAutoInstallDependencies(bool)));
}

// --------------------------------------------------------------------------
// qCjyxSettingsExtensionsPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsExtensionsPanel::qCjyxSettingsExtensionsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsExtensionsPanelPrivate(*this))
{
  Q_D(qCjyxSettingsExtensionsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsExtensionsPanel::~qCjyxSettingsExtensionsPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsExtensionsPanel::onExtensionsManagerEnabled(bool value)
{
  Q_UNUSED(value);
}

// --------------------------------------------------------------------------
void qCjyxSettingsExtensionsPanel::onExtensionsPathChanged(const QString& path)
{
  qCjyxCoreApplication::application()->setExtensionsInstallPath(path);
}

// --------------------------------------------------------------------------
void qCjyxSettingsExtensionsPanel::updateAutoUpdateWidgetsFromModel()
{
  Q_D(qCjyxSettingsExtensionsPanel);
  qCjyxApplication* app = qCjyxApplication::application();
  if (!app->extensionsManagerModel())
    {
    return;
    }
  QSignalBlocker blocker1(d->AutoUpdateCheckCheckBox);
  QSignalBlocker blocker2(d->AutoUpdateInstallCheckBox);
  QSignalBlocker blocker3(d->AutoInstallDependenciesCheckBox);
  d->AutoUpdateCheckCheckBox->setChecked(app->extensionsManagerModel()->autoUpdateCheck());
  d->AutoUpdateInstallCheckBox->setChecked(app->extensionsManagerModel()->autoUpdateInstall());
  d->AutoInstallDependenciesCheckBox->setChecked(app->extensionsManagerModel()->autoInstallDependencies());
}
