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

// qCjyxGPURayCastVolumeRendering includes
#include "qCjyxGPURayCastVolumeRenderingPropertiesWidget.h"
#include "vtkMRMLGPURayCastVolumeRenderingDisplayNode.h"
#include "ui_qCjyxGPURayCastVolumeRenderingPropertiesWidget.h"

// MRML includes
#include "vtkMRMLScene.h"
#include "vtkMRMLViewNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_VolumeRendering
class qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate
  : public Ui_qCjyxGPURayCastVolumeRenderingPropertiesWidget
{
  Q_DECLARE_PUBLIC(qCjyxGPURayCastVolumeRenderingPropertiesWidget);
protected:
  qCjyxGPURayCastVolumeRenderingPropertiesWidget* const q_ptr;

public:
  qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate(
    qCjyxGPURayCastVolumeRenderingPropertiesWidget& object);
  virtual ~qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate();

  virtual void setupUi(qCjyxGPURayCastVolumeRenderingPropertiesWidget*);
  void populateRenderingTechniqueComboBox();
};

// --------------------------------------------------------------------------
qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate
::qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate(
  qCjyxGPURayCastVolumeRenderingPropertiesWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate::
~qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate() = default;

// --------------------------------------------------------------------------
void qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate
::setupUi(qCjyxGPURayCastVolumeRenderingPropertiesWidget* widget)
{
  this->Ui_qCjyxGPURayCastVolumeRenderingPropertiesWidget::setupUi(widget);
  this->populateRenderingTechniqueComboBox();
  QObject::connect(this->RenderingTechniqueComboBox, SIGNAL(currentIndexChanged(int)),
                   widget, SLOT(setRenderingTechnique(int)));
  QObject::connect(this->SurfaceSmoothingCheckBox, SIGNAL(toggled(bool)),
                   widget, SLOT(setSurfaceSmoothing(bool)));
}

// --------------------------------------------------------------------------
void qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate::populateRenderingTechniqueComboBox()
{
  this->RenderingTechniqueComboBox->clear();
  this->RenderingTechniqueComboBox->addItem(
    "Composite With Shading", vtkMRMLViewNode::Composite);
  this->RenderingTechniqueComboBox->addItem(
    "Maximum Intensity Projection", vtkMRMLViewNode::MaximumIntensityProjection);
  this->RenderingTechniqueComboBox->addItem(
    "Minimum Intensity Projection", vtkMRMLViewNode::MinimumIntensityProjection);
}

//-----------------------------------------------------------------------------
// qCjyxGPURayCastVolumeRenderingPropertiesWidget methods

//-----------------------------------------------------------------------------
qCjyxGPURayCastVolumeRenderingPropertiesWidget
::qCjyxGPURayCastVolumeRenderingPropertiesWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qCjyxGPURayCastVolumeRenderingPropertiesWidgetPrivate(*this) )
{
  Q_D(qCjyxGPURayCastVolumeRenderingPropertiesWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qCjyxGPURayCastVolumeRenderingPropertiesWidget::~qCjyxGPURayCastVolumeRenderingPropertiesWidget() = default;

//-----------------------------------------------------------------------------
vtkMRMLGPURayCastVolumeRenderingDisplayNode* qCjyxGPURayCastVolumeRenderingPropertiesWidget
::mrmlGPURayCastDisplayNode()
{
  return vtkMRMLGPURayCastVolumeRenderingDisplayNode::SafeDownCast(
    this->mrmlVolumeRenderingDisplayNode());
}

//-----------------------------------------------------------------------------
void qCjyxGPURayCastVolumeRenderingPropertiesWidget::updateWidgetFromMRML()
{
  Q_D(qCjyxGPURayCastVolumeRenderingPropertiesWidget);

  vtkMRMLGPURayCastVolumeRenderingDisplayNode* displayNode = this->mrmlGPURayCastDisplayNode();
  if (!displayNode)
    {
    return;
    }
  vtkMRMLViewNode* firstViewNode = displayNode->GetFirstViewNode();
  if (!firstViewNode)
    {
    return;
    }

  int technique = firstViewNode->GetRaycastTechnique();
  int index = d->RenderingTechniqueComboBox->findData(QVariant(technique));
  if (index == -1)
    {
    index = 0;
    }
  bool wasBlocked = d->RenderingTechniqueComboBox->blockSignals(true);
  d->RenderingTechniqueComboBox->setCurrentIndex(index);
  d->RenderingTechniqueComboBox->blockSignals(wasBlocked);

  wasBlocked = d->SurfaceSmoothingCheckBox->blockSignals(true);
  d->SurfaceSmoothingCheckBox->setChecked(firstViewNode->GetVolumeRenderingSurfaceSmoothing());
  d->SurfaceSmoothingCheckBox->blockSignals(wasBlocked);
}

//-----------------------------------------------------------------------------
void qCjyxGPURayCastVolumeRenderingPropertiesWidget::setRenderingTechnique(int index)
{
  Q_D(qCjyxGPURayCastVolumeRenderingPropertiesWidget);
  vtkMRMLGPURayCastVolumeRenderingDisplayNode* displayNode = this->mrmlGPURayCastDisplayNode();
  if (!displayNode)
    {
    return;
    }
  int technique = d->RenderingTechniqueComboBox->itemData(index).toInt();

  std::vector<vtkMRMLNode*> viewNodes;
  displayNode->GetScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    if (displayNode->IsDisplayableInView(viewNode->GetID()))
      {
      viewNode->SetRaycastTechnique(technique);
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxGPURayCastVolumeRenderingPropertiesWidget::setSurfaceSmoothing(bool on)
{
  Q_D(qCjyxGPURayCastVolumeRenderingPropertiesWidget);
  vtkMRMLGPURayCastVolumeRenderingDisplayNode* displayNode = this->mrmlGPURayCastDisplayNode();
  if (!displayNode)
    {
    return;
    }

  std::vector<vtkMRMLNode*> viewNodes;
  displayNode->GetScene()->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector<vtkMRMLNode*>::iterator it=viewNodes.begin(); it!=viewNodes.end(); ++it)
    {
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(*it);
    if (displayNode->IsDisplayableInView(viewNode->GetID()))
      {
      viewNode->SetVolumeRenderingSurfaceSmoothing(on);
      }
    }
}
