/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QSettings>
#include <QMessageBox>

// QtGUI includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxApplication.h"
#include "qCjyxModuleManager.h"
#include "qCjyxSegmentationsSettingsPanel.h"
#include "qCjyxTerminologySelectorDialog.h"
#include "ui_qCjyxSegmentationsSettingsPanel.h"

// Logic includes
#include <vtkCjyxSegmentationsModuleLogic.h>
#include <vtkCjyxTerminologiesModuleLogic.h>

// --------------------------------------------------------------------------
// qCjyxSegmentationsSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSegmentationsSettingsPanelPrivate: public Ui_qCjyxSegmentationsSettingsPanel
{
  Q_DECLARE_PUBLIC(qCjyxSegmentationsSettingsPanel);
protected:
  qCjyxSegmentationsSettingsPanel* const q_ptr;

public:
  qCjyxSegmentationsSettingsPanelPrivate(qCjyxSegmentationsSettingsPanel& object);
  void init();

  QString DefaultTerminologyString;

  vtkWeakPointer<vtkCjyxSegmentationsModuleLogic> SegmentationsLogic;
  vtkWeakPointer<vtkCjyxTerminologiesModuleLogic> TerminologiesLogic;
};

// --------------------------------------------------------------------------
// qCjyxSegmentationsSettingsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSegmentationsSettingsPanelPrivate
::qCjyxSegmentationsSettingsPanelPrivate(qCjyxSegmentationsSettingsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanelPrivate::init()
{
  Q_Q(qCjyxSegmentationsSettingsPanel);

  this->setupUi(q);

  this->TerminologiesLogic = vtkCjyxTerminologiesModuleLogic::SafeDownCast(
    qCjyxCoreApplication::application()->moduleLogic("Terminologies"));
  if (!this->TerminologiesLogic)
    {
    qCritical() << Q_FUNC_INFO << ": Terminologies logic is not found";
    }

  // Default values
  this->AutoOpacitiesCheckBox->setChecked(true);
  this->SurfaceSmoothingCheckBox->setChecked(true);

  // Register settings
  q->registerProperty("Segmentations/AutoOpacities", this->AutoOpacitiesCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Automatically set opacities of the segments based on which contains which, so that no segment obscures another", ctkSettingsPanel::OptionNone);
  q->registerProperty("Segmentations/DefaultSurfaceSmoothing", this->SurfaceSmoothingCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Enable closed surface representation smoothing by default", ctkSettingsPanel::OptionNone);
  q->registerProperty("Segmentations/DefaultTerminologyEntry", q,
                      "defaultTerminologyEntry", SIGNAL(defaultTerminologyEntryChanged(QString)),
                      "Default terminology entry", ctkSettingsPanel::OptionNone);

  this->AllowEditingHiddenSegmentComboBox->addItem("Ask user", QMessageBox::InvalidRole);
  this->AllowEditingHiddenSegmentComboBox->addItem("Always make visible", QMessageBox::Yes);
  this->AllowEditingHiddenSegmentComboBox->addItem("Always allow", QMessageBox::No);
  q->registerProperty("Segmentations/ConfirmEditHiddenSegment", this->AllowEditingHiddenSegmentComboBox,
    "currentUserDataAsString", SIGNAL(currentIndexChanged(int)));

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->AutoOpacitiesCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setAutoOpacities(bool)));
  QObject::connect(this->SurfaceSmoothingCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setDefaultSurfaceSmoothing(bool)));
  QObject::connect(this->EditDefaultTerminologyEntryPushButton, SIGNAL(clicked()),
                   q, SLOT(onEditDefaultTerminologyEntry()));

  // Update default segmentation node from settings when startup completed.
  QObject::connect(qCjyxApplication::application(), SIGNAL(startupCompleted()),
    q, SLOT(updateDefaultSegmentationNodeFromWidget()));
}

// --------------------------------------------------------------------------
// qCjyxSegmentationsSettingsPanel methods

// --------------------------------------------------------------------------
qCjyxSegmentationsSettingsPanel::qCjyxSegmentationsSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSegmentationsSettingsPanelPrivate(*this))
{
  Q_D(qCjyxSegmentationsSettingsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSegmentationsSettingsPanel::~qCjyxSegmentationsSettingsPanel() = default;

// --------------------------------------------------------------------------
vtkCjyxSegmentationsModuleLogic* qCjyxSegmentationsSettingsPanel::segmentationsLogic()const
{
  Q_D(const qCjyxSegmentationsSettingsPanel);
  return d->SegmentationsLogic;
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::setSegmentationsLogic(vtkCjyxSegmentationsModuleLogic* logic)
{
  Q_D(qCjyxSegmentationsSettingsPanel);
  d->SegmentationsLogic = logic;
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::setAutoOpacities(bool on)
{
  Q_UNUSED(on);
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::setDefaultSurfaceSmoothing(bool on)
{
  Q_UNUSED(on);
  if (this->segmentationsLogic())
    {
    this->segmentationsLogic()->SetDefaultSurfaceSmoothingEnabled(on);
    }
}

// --------------------------------------------------------------------------
QString qCjyxSegmentationsSettingsPanel::defaultTerminologyEntry()
{
  Q_D(qCjyxSegmentationsSettingsPanel);
  return d->DefaultTerminologyString;
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::setDefaultTerminologyEntry(QString terminologyStr)
{
  Q_D(qCjyxSegmentationsSettingsPanel);
  d->DefaultTerminologyString = terminologyStr;
  QString buttonText=tr("(set)");
  if (d->TerminologiesLogic && !terminologyStr.isEmpty())
    {
    vtkNew<vtkCjyxTerminologyEntry> entry;
    std::string terminologyStdStr = d->DefaultTerminologyString.toUtf8().constData();
    if (d->TerminologiesLogic->DeserializeTerminologyEntry(terminologyStdStr, entry))
      {
      buttonText.clear();
      buttonText += (entry->GetCategoryObject() && entry->GetCategoryObject()->GetCodeMeaning()
          ? entry->GetCategoryObject()->GetCodeMeaning() : "?");
      buttonText += "/";
      buttonText += (entry->GetTypeObject() && entry->GetTypeObject()->GetCodeMeaning()
          ? entry->GetTypeObject()->GetCodeMeaning() : "?");
      }
    }
  d->EditDefaultTerminologyEntryPushButton->setText(buttonText);
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::onEditDefaultTerminologyEntry()
{
  Q_D(qCjyxSegmentationsSettingsPanel);

  if (!d->TerminologiesLogic)
  {
    return;
  }
  vtkNew<vtkCjyxTerminologyEntry> entry;
  std::string terminologyStdStr = d->DefaultTerminologyString.toUtf8().constData();
  d->TerminologiesLogic->DeserializeTerminologyEntry(terminologyStdStr, entry);
  if (!qCjyxTerminologySelectorDialog::getTerminology(entry, this))
    {
    // user cancelled
    return;
    }
  this->setDefaultTerminologyEntry(vtkCjyxTerminologiesModuleLogic::SerializeTerminologyEntry(entry).c_str());
  emit defaultTerminologyEntryChanged(d->DefaultTerminologyString);
}

// --------------------------------------------------------------------------
void qCjyxSegmentationsSettingsPanel::updateDefaultSegmentationNodeFromWidget()
{
  Q_D(qCjyxSegmentationsSettingsPanel);
  this->setDefaultSurfaceSmoothing(d->SurfaceSmoothingCheckBox->isChecked());
}
