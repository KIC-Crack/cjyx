/*==============================================================================

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

/// Qt includes
#include <QFileInfo>
#include <QSettings>

// Cjyx includes
#include "qCjyxApplication.h"

// CTK includes
#include <ctkFlowLayout.h>
#include <ctkUtils.h>

// VTK includes
#include <vtkNew.h>

/// Segmentations includes
#include "qCjyxIOOptions_p.h"
#include "qCjyxSegmentationsIOOptionsWidget.h"
#include "ui_qCjyxSegmentationsIOOptionsWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Volumes
class qCjyxSegmentationsIOOptionsWidgetPrivate
  : public qCjyxIOOptionsPrivate
  , public Ui_qCjyxSegmentationsIOOptionsWidget
{
public:
};

//-----------------------------------------------------------------------------
qCjyxSegmentationsIOOptionsWidget::qCjyxSegmentationsIOOptionsWidget(QWidget* parentWidget)
  : qCjyxIOOptionsWidget(new qCjyxSegmentationsIOOptionsWidgetPrivate, parentWidget)
{
  Q_D(qCjyxSegmentationsIOOptionsWidget);
  d->setupUi(this);

  ctkFlowLayout::replaceLayout(this);

  QSettings* settings = qCjyxApplication::application()->settingsDialog()->settings();
  if (settings->contains("Segmentations/AutoOpacities"))
    {
    bool autoOpacities = settings->value("Segmentations/AutoOpacities").toBool();
    d->AutoOpacitiesCheckBox->setChecked(autoOpacities);
    d->Properties["autoOpacities"] = autoOpacities;
    }

  connect(d->AutoOpacitiesCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(updateProperties()));
  connect(d->ColorNodeSelector, SIGNAL(currentNodeIDChanged(QString)),
          this, SLOT(updateProperties()));
}

//-----------------------------------------------------------------------------
qCjyxSegmentationsIOOptionsWidget::~qCjyxSegmentationsIOOptionsWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxSegmentationsIOOptionsWidget::updateProperties()
{
  Q_D(qCjyxSegmentationsIOOptionsWidget);

  d->Properties["autoOpacities"] = d->AutoOpacitiesCheckBox->isChecked();
  d->Properties["colorNodeID"] = d->ColorNodeSelector->currentNodeID();
}
