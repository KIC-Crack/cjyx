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
#include <QSettings>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxModuleManager.h"
#include "qCjyxSettingsDeveloperPanel.h"
#include "ui_qCjyxSettingsDeveloperPanel.h"

// --------------------------------------------------------------------------
// qCjyxSettingsDeveloperPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsDeveloperPanelPrivate: public Ui_qCjyxSettingsDeveloperPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsDeveloperPanel);
protected:
  qCjyxSettingsDeveloperPanel* const q_ptr;

public:
  qCjyxSettingsDeveloperPanelPrivate(qCjyxSettingsDeveloperPanel& object);
  void init();
};

// --------------------------------------------------------------------------
// qCjyxSettingsDeveloperPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsDeveloperPanelPrivate
::qCjyxSettingsDeveloperPanelPrivate(qCjyxSettingsDeveloperPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsDeveloperPanelPrivate::init()
{
  Q_Q(qCjyxSettingsDeveloperPanel);

  this->setupUi(q);

  // Default values
  this->DeveloperModeEnabledCheckBox->setChecked(false);
  this->SelfTestMessageDelaySlider->setValue(750);
  this->QtTestingEnabledCheckBox->setChecked(false);
#ifndef Cjyx_USE_QtTesting
  this->QtTestingEnabledCheckBox->hide();
  this->QtTestingEnabledLabel->hide();
#endif

  // Register settings
  q->registerProperty("Developer/DeveloperMode", this->DeveloperModeEnabledCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Enable/Disable developer mode", ctkSettingsPanel::OptionRequireRestart);

  q->registerProperty("Developer/PreserveCLIModuleDataFiles", this->PreserveCLIModuleDataFilesCheckBox,
    "checked", SIGNAL(toggled(bool)),
    "Preserve CLI module input/output files", ctkSettingsPanel::OptionRequireRestart);

  q->registerProperty("Developer/SelfTestDisplayMessageDelay", this->SelfTestMessageDelaySlider,
                      "value", SIGNAL(valueChanged(double)),
                      "Time to wait before resuming self-test execution and hiding messages displayed to the user");

  q->registerProperty("QtTesting/Enabled", this->QtTestingEnabledCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Enable/Disable QtTesting", ctkSettingsPanel::OptionRequireRestart);

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->DeveloperModeEnabledCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(enableDeveloperMode(bool)));
  QObject::connect(this->PreserveCLIModuleDataFilesCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(preserveCLIModuleDataFiles(bool)));
  QObject::connect(this->QtTestingEnabledCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(enableQtTesting(bool)));

  QObject::connect(this->QtDesignerButton, SIGNAL(clicked()),
    qCjyxApplication::application(), SLOT(launchDesigner()));
}

// --------------------------------------------------------------------------
// qCjyxSettingsDeveloperPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsDeveloperPanel::qCjyxSettingsDeveloperPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsDeveloperPanelPrivate(*this))
{
  Q_D(qCjyxSettingsDeveloperPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsDeveloperPanel::~qCjyxSettingsDeveloperPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsDeveloperPanel::enableDeveloperMode(bool value)
{
  Q_UNUSED(value);
}

// --------------------------------------------------------------------------
void qCjyxSettingsDeveloperPanel::preserveCLIModuleDataFiles(bool value)
{
  Q_UNUSED(value);
}

// --------------------------------------------------------------------------
void qCjyxSettingsDeveloperPanel::enableQtTesting(bool value)
{
  Q_UNUSED(value);
}
