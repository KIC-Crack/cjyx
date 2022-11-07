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
#include <QCloseEvent>
#include <QPushButton>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxExtensionsManagerDialog.h"
#include "qCjyxExtensionsManagerModel.h"
#include "qCjyxSettingsExtensionsPanel.h"
#include "ui_qCjyxExtensionsManagerDialog.h"

//-----------------------------------------------------------------------------
class qCjyxExtensionsManagerDialogPrivate: public Ui_qCjyxExtensionsManagerDialog
{
  Q_DECLARE_PUBLIC(qCjyxExtensionsManagerDialog);
protected:
  qCjyxExtensionsManagerDialog* const q_ptr;

public:
  qCjyxExtensionsManagerDialogPrivate(qCjyxExtensionsManagerDialog& object);
  void init();
  void updateButtons();

  bool RestartRequested;

  QStringList PreviousModulesAdditionalPaths;
  QStringList PreviousExtensionsScheduledForUninstall;
  QVariantMap PreviousExtensionsScheduledForUpdate;
};

// --------------------------------------------------------------------------
qCjyxExtensionsManagerDialogPrivate::qCjyxExtensionsManagerDialogPrivate(qCjyxExtensionsManagerDialog& object)
  : q_ptr(&object)
  , RestartRequested(false)
{
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialogPrivate::init()
{
  Q_Q(qCjyxExtensionsManagerDialog);

  this->setupUi(q);

  QObject::connect(this->ExtensionsManagerWidget, SIGNAL(inBatchProcessing(bool)),
    q, SLOT(onBatchProcessingChanged()));

  QPushButton * restartButton = this->ButtonBox->button(QDialogButtonBox::Ok);
  restartButton->setText("Restart");

  q->setRestartRequested(false);

  // Assuming the dialog is instantiated prior any update of the settings,
  // keeping track of settings will allow us to display the "RestartRequestedLabel"
  // only if it applies. Note also that keep track of "EnvironmentVariables/PYTHONPATH'
  // isn't required, "Modules/AdditionalPaths" is enough to know if we should restart.
  QSettings * settings = qCjyxCoreApplication::application()->revisionUserSettings();
    // this->PreviousModulesAdditionalPaths contain the raw (relative or absolute) paths, not converted to absolute
  this->PreviousModulesAdditionalPaths = settings->value("Modules/AdditionalPaths").toStringList();
  this->PreviousExtensionsScheduledForUninstall = settings->value("Extensions/ScheduledForUninstall").toStringList();
  this->PreviousExtensionsScheduledForUpdate = settings->value("Extensions/ScheduledForUpdate").toMap();

  qCjyxSettingsExtensionsPanel * extensionsPanel =
      qobject_cast<qCjyxSettingsExtensionsPanel*>(
        qCjyxApplication::application()->settingsDialog()->panel("Extensions"));
  Q_ASSERT(extensionsPanel);
  if (extensionsPanel)
    {
    QObject::connect(extensionsPanel, SIGNAL(extensionsServerUrlChanged(QString)),
                     this->ExtensionsManagerWidget, SLOT(refreshInstallWidget()));
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialogPrivate::updateButtons()
{
  Q_Q(qCjyxExtensionsManagerDialog);
  Q_ASSERT(q->extensionsManagerModel());
  bool shouldRestart = false;
  qCjyxCoreApplication * coreApp = qCjyxCoreApplication::application();
  // this->PreviousModulesAdditionalPaths contain the raw (relative or absolute) paths, not converted to absolute
  if (this->PreviousModulesAdditionalPaths
      != coreApp->revisionUserSettings()->value("Modules/AdditionalPaths").toStringList() ||
    this->PreviousExtensionsScheduledForUninstall
      != coreApp->revisionUserSettings()->value("Extensions/ScheduledForUninstall").toStringList() ||
    this->PreviousExtensionsScheduledForUpdate
      != coreApp->revisionUserSettings()->value("Extensions/ScheduledForUpdate").toMap())
    {
    shouldRestart = true;
    }
  bool isInBatchMode = this->ExtensionsManagerWidget->isInBatchProcessing();

  this->ButtonBox->setEnabled(!isInBatchMode);
  q->setRestartRequested(shouldRestart);
}

// --------------------------------------------------------------------------
// qCjyxExtensionsManagerDialog methods

// --------------------------------------------------------------------------
qCjyxExtensionsManagerDialog::qCjyxExtensionsManagerDialog(QWidget *_parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxExtensionsManagerDialogPrivate(*this))
{
  Q_D(qCjyxExtensionsManagerDialog);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxExtensionsManagerDialog::~qCjyxExtensionsManagerDialog() = default;

// --------------------------------------------------------------------------
qCjyxExtensionsManagerModel* qCjyxExtensionsManagerDialog::extensionsManagerModel()const
{
  Q_D(const qCjyxExtensionsManagerDialog);
  return d->ExtensionsManagerWidget->extensionsManagerModel();
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::setExtensionsManagerModel(qCjyxExtensionsManagerModel* model)
{
  Q_D(qCjyxExtensionsManagerDialog);

  if (this->extensionsManagerModel() == model)
    {
    return;
    }

  disconnect(this, SLOT(onModelUpdated()));

  d->ExtensionsManagerWidget->setExtensionsManagerModel(model);

  if (model)
    {
    this->onModelUpdated();
    connect(model, SIGNAL(modelUpdated()),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionInstalled(QString)),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionScheduledForUninstall(QString)),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionCancelledScheduleForUninstall(QString)),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionScheduledForUpdate(QString)),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionCancelledScheduleForUpdate(QString)),
            this, SLOT(onModelUpdated()));
    connect(model, SIGNAL(extensionEnabledChanged(QString,bool)),
            this, SLOT(onModelUpdated()));
    }
}

// --------------------------------------------------------------------------
bool qCjyxExtensionsManagerDialog::restartRequested()const
{
  Q_D(const qCjyxExtensionsManagerDialog);
  return d->RestartRequested;
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::setRestartRequested(bool value)
{
  Q_D(qCjyxExtensionsManagerDialog);
  d->RestartRequested = value;
  d->RestartRequestedLabel->setVisible(value);
  d->ButtonBox->button(QDialogButtonBox::Ok)->setEnabled(value);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::onModelUpdated()
{
  Q_D(qCjyxExtensionsManagerDialog);
  d->updateButtons();
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::onBatchProcessingChanged()
{
  Q_D(qCjyxExtensionsManagerDialog);
  d->updateButtons();
}

// --------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::closeEvent(QCloseEvent* event)
{
  Q_D(qCjyxExtensionsManagerDialog);
  if (d->ExtensionsManagerWidget->confirmClose())
    {
    event->accept(); // close window
    }
  else
    {
    event->ignore(); // ignore close event
    }
}

//-----------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::accept()
{
  Q_D(qCjyxExtensionsManagerDialog);
  if (d->ExtensionsManagerWidget->confirmClose())
    {
    Superclass::accept(); // close window
    }
}

//-----------------------------------------------------------------------------
void qCjyxExtensionsManagerDialog::reject()
{
  Q_D(qCjyxExtensionsManagerDialog);
  if (d->ExtensionsManagerWidget->confirmClose())
    {
    Superclass::reject(); // close window
    }
}
