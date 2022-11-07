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
#include <QDebug>

/// Models includes
#include "qCjyxIOOptions_p.h"
#include "qCjyxModelsIOOptionsWidget.h"
#include "ui_qCjyxModelsIOOptionsWidget.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLStorageNode.h>

//-----------------------------------------------------------------------------
class qCjyxModelsIOOptionsWidgetPrivate
  : public qCjyxIOOptionsPrivate
  , public Ui_qCjyxModelsIOOptionsWidget
{
public:
};

//-----------------------------------------------------------------------------
qCjyxModelsIOOptionsWidget::qCjyxModelsIOOptionsWidget(QWidget* parentWidget)
  : Superclass(new qCjyxModelsIOOptionsWidgetPrivate, parentWidget)
{
  Q_D(qCjyxModelsIOOptionsWidget);
  d->setupUi(this);

  connect(d->coordinateSystemComboBox, SIGNAL(currentIndexChanged(int)),
          this, SLOT(updateProperties()));
}

//-----------------------------------------------------------------------------
qCjyxModelsIOOptionsWidget::~qCjyxModelsIOOptionsWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxModelsIOOptionsWidget::updateProperties()
{
  Q_D(qCjyxModelsIOOptionsWidget);
  d->Properties["coordinateSystem"] = vtkMRMLStorageNode::GetCoordinateSystemTypeFromString(
    d->coordinateSystemComboBox->currentText().toLatin1().constData());
}
