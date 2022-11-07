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

#include "qCjyxDiffusionWeightedVolumeDisplayWidget.h"
#include "ui_qCjyxDiffusionWeightedVolumeDisplayWidget.h"

// Qt includes

// MRML includes
#include "vtkMRMLDiffusionWeightedVolumeNode.h"
#include "vtkMRMLDiffusionWeightedVolumeDisplayNode.h"

// VTK includes
#include <vtkImageData.h>

// STD includes

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Volumes
class qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate
  : public Ui_qCjyxDiffusionWeightedVolumeDisplayWidget
{
  Q_DECLARE_PUBLIC(qCjyxDiffusionWeightedVolumeDisplayWidget);
protected:
  qCjyxDiffusionWeightedVolumeDisplayWidget* const q_ptr;
public:
  qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate(qCjyxDiffusionWeightedVolumeDisplayWidget& object);
  ~qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate();
  void init();
  vtkWeakPointer<vtkMRMLDiffusionWeightedVolumeNode> VolumeNode;
};

//-----------------------------------------------------------------------------
qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate
::qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate(
  qCjyxDiffusionWeightedVolumeDisplayWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate
::~qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate::init()
{
  Q_Q(qCjyxDiffusionWeightedVolumeDisplayWidget);

  this->setupUi(q);

  QObject::connect(this->DWIComponentSlider, SIGNAL(valueChanged(int)),
                   q, SLOT(setDWIComponent(int)));
}

// --------------------------------------------------------------------------
qCjyxDiffusionWeightedVolumeDisplayWidget
::qCjyxDiffusionWeightedVolumeDisplayWidget(QWidget* parentWidget)
  : Superclass(parentWidget)
  , d_ptr(new qCjyxDiffusionWeightedVolumeDisplayWidgetPrivate(*this))
{
  Q_D(qCjyxDiffusionWeightedVolumeDisplayWidget);
  d->init();

  // disable as there is not MRML Node associated with the widget
  this->setEnabled(false);
}

// --------------------------------------------------------------------------
qCjyxDiffusionWeightedVolumeDisplayWidget
::~qCjyxDiffusionWeightedVolumeDisplayWidget() = default;

// --------------------------------------------------------------------------
vtkMRMLDiffusionWeightedVolumeNode* qCjyxDiffusionWeightedVolumeDisplayWidget
::volumeNode()const
{
  Q_D(const qCjyxDiffusionWeightedVolumeDisplayWidget);
  return d->VolumeNode;
}

// --------------------------------------------------------------------------
vtkMRMLDiffusionWeightedVolumeDisplayNode* qCjyxDiffusionWeightedVolumeDisplayWidget
::volumeDisplayNode()const
{
  vtkMRMLDiffusionWeightedVolumeNode* volumeNode = this->volumeNode();
  return volumeNode ? vtkMRMLDiffusionWeightedVolumeDisplayNode::SafeDownCast(
    volumeNode->GetVolumeDisplayNode()) : nullptr;
}

// --------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidget::setMRMLVolumeNode(vtkMRMLNode* node)
{
  this->setMRMLVolumeNode(vtkMRMLDiffusionWeightedVolumeNode::SafeDownCast(node));
}

// --------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidget
::setMRMLVolumeNode(vtkMRMLDiffusionWeightedVolumeNode* volumeNode)
{
  Q_D(qCjyxDiffusionWeightedVolumeDisplayWidget);

  vtkMRMLDiffusionWeightedVolumeDisplayNode* oldVolumeDisplayNode = this->volumeDisplayNode();

  qvtkReconnect(oldVolumeDisplayNode, volumeNode ? volumeNode->GetVolumeDisplayNode() :nullptr,
                vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromDisplayNode()));
  qvtkReconnect(d->VolumeNode, volumeNode,
                vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromVolumeNode()));

  d->VolumeNode = volumeNode;
  d->ScalarVolumeDisplayWidget->setMRMLVolumeNode(volumeNode);

  this->updateWidgetFromVolumeNode();
  this->updateWidgetFromDisplayNode();
}

// --------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidget::updateWidgetFromVolumeNode()
{
  Q_D(qCjyxDiffusionWeightedVolumeDisplayWidget);
  this->setEnabled(d->VolumeNode != nullptr);
  if (!d->VolumeNode)
    {
    return;
    }
  int maxRange = d->VolumeNode->GetImageData() ?
    d->VolumeNode->GetImageData()->GetNumberOfScalarComponents() - 1 : 0;
  // we save the component here, as changing the range of the slider/spinbox
  // can change the component value. We want to set it back.
  vtkMRMLDiffusionWeightedVolumeDisplayNode* displayNode =
    this->volumeDisplayNode();

  int component = displayNode ? displayNode->GetDiffusionComponent() : d->DWIComponentSlider->value();
  bool sliderWasBlocking = d->DWIComponentSlider->blockSignals(true);
  bool spinBoxWasBlocking = d->DWIComponentSpinBox->blockSignals(true);
  d->DWIComponentSlider->setRange(0, maxRange);
  d->DWIComponentSpinBox->setRange(0, maxRange);
  d->DWIComponentSlider->blockSignals(sliderWasBlocking);
  d->DWIComponentSpinBox->blockSignals(spinBoxWasBlocking);
  d->DWIComponentSlider->setValue(component);
}

// --------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidget::updateWidgetFromDisplayNode()
{
  Q_D(qCjyxDiffusionWeightedVolumeDisplayWidget);
  vtkMRMLDiffusionWeightedVolumeDisplayNode* displayNode =
    this->volumeDisplayNode();
  if (!displayNode)
    {
    return;
    }
  d->DWIComponentSlider->setValue(displayNode->GetDiffusionComponent());
}


//----------------------------------------------------------------------------
void qCjyxDiffusionWeightedVolumeDisplayWidget::setDWIComponent(int component)
{
  vtkMRMLDiffusionWeightedVolumeDisplayNode* volumeDisplayNode = this->volumeDisplayNode();
  if (!volumeDisplayNode)
    {
    return;
    }
  volumeDisplayNode->SetDiffusionComponent(component);
}
