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

/// Qt includes
#include <QFileInfo>

// CTK includes
#include <ctkFlowLayout.h>
#include <ctkUtils.h>

/// Scene includes
#include "qCjyxIOOptions_p.h"
#include "qCjyxSceneIOOptionsWidget.h"
#include "ui_qCjyxSceneIOOptionsWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Scene
class qCjyxSceneIOOptionsWidgetPrivate
  : public qCjyxIOOptionsPrivate
  , public Ui_qCjyxSceneIOOptionsWidget
{
public:
};

//-----------------------------------------------------------------------------
qCjyxSceneIOOptionsWidget::qCjyxSceneIOOptionsWidget(QWidget* parentWidget)
  : qCjyxIOOptionsWidget(new qCjyxSceneIOOptionsWidgetPrivate, parentWidget)
{
  Q_D(qCjyxSceneIOOptionsWidget);
  d->setupUi(this);

  ctkFlowLayout::replaceLayout(this);

  connect(d->ClearSceneCheckBox, SIGNAL(toggled(bool)),
          this, SLOT(updateProperties()));

  this->updateProperties();
}

//-----------------------------------------------------------------------------
qCjyxSceneIOOptionsWidget::~qCjyxSceneIOOptionsWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxSceneIOOptionsWidget::updateProperties()
{
  Q_D(qCjyxSceneIOOptionsWidget);

  d->Properties["clear"] = d->ClearSceneCheckBox->isChecked();
}

//------------------------------------------------------------------------------
void qCjyxSceneIOOptionsWidget::updateGUI(const qCjyxIO::IOProperties& ioProperties)
{
  Q_D(qCjyxSceneIOOptionsWidget);
  qCjyxIOOptionsWidget::updateGUI(ioProperties);
  if (ioProperties.contains("clear"))
    {
    d->ClearSceneCheckBox->setChecked(ioProperties["clear"].toBool());
    }
}
