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

  This file was originally developed by Benjamin Long, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QSettings>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxSettingsInternationalizationPanel.h"
#include "ui_qCjyxSettingsInternationalizationPanel.h"

// --------------------------------------------------------------------------
// qCjyxSettingsInternationalizationPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsInternationalizationPanelPrivate: public Ui_qCjyxSettingsInternationalizationPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsInternationalizationPanel);
protected:
  qCjyxSettingsInternationalizationPanel* const q_ptr;

public:
  qCjyxSettingsInternationalizationPanelPrivate(qCjyxSettingsInternationalizationPanel& object);
  void init();
};

// --------------------------------------------------------------------------
// qCjyxSettingsInternationalizationPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsInternationalizationPanelPrivate
::qCjyxSettingsInternationalizationPanelPrivate(qCjyxSettingsInternationalizationPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsInternationalizationPanelPrivate::init()
{
  Q_Q(qCjyxSettingsInternationalizationPanel);

  this->setupUi(q);

  // Default values
  this->InternationalizationEnabledCheckBox->setChecked(false);

  // Register settings
  q->registerProperty("Internationalization/Enabled",
                      this->InternationalizationEnabledCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Enable/Disable Internationalization",
                      ctkSettingsPanel::OptionRequireRestart);

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->InternationalizationEnabledCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(enableInternationalization(bool)));
}

// --------------------------------------------------------------------------
// qCjyxSettingsInternationalizationPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsInternationalizationPanel::qCjyxSettingsInternationalizationPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsInternationalizationPanelPrivate(*this))
{
  Q_D(qCjyxSettingsInternationalizationPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsInternationalizationPanel::~qCjyxSettingsInternationalizationPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsInternationalizationPanel::enableInternationalization(bool value)
{
  Q_UNUSED(value);
}
