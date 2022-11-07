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

// qCjyxVolumeRendering includes
#include "qCjyxVolumeRenderingPropertiesWidget.h"
#include "vtkMRMLVolumeRenderingDisplayNode.h"

// MRML includes
#include "vtkMRMLVolumeNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_VolumeRendering
class qCjyxVolumeRenderingPropertiesWidgetPrivate
{
  Q_DECLARE_PUBLIC(qCjyxVolumeRenderingPropertiesWidget);
protected:
  qCjyxVolumeRenderingPropertiesWidget* const q_ptr;

public:
  qCjyxVolumeRenderingPropertiesWidgetPrivate(qCjyxVolumeRenderingPropertiesWidget& object);

  vtkMRMLVolumeRenderingDisplayNode* VolumeRenderingDisplayNode;
  vtkMRMLVolumeNode* VolumeNode;
};

//-----------------------------------------------------------------------------
// qCjyxVolumeRenderingPropertiesWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingPropertiesWidgetPrivate
::qCjyxVolumeRenderingPropertiesWidgetPrivate(
  qCjyxVolumeRenderingPropertiesWidget& object)
  : q_ptr(&object)
{
  this->VolumeRenderingDisplayNode = nullptr;
  this->VolumeNode = nullptr;
}

//-----------------------------------------------------------------------------
// qCjyxVolumeRenderingPropertiesWidget methods

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingPropertiesWidget
::qCjyxVolumeRenderingPropertiesWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qCjyxVolumeRenderingPropertiesWidgetPrivate(*this) )
{
}

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingPropertiesWidget::~qCjyxVolumeRenderingPropertiesWidget() = default;

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxVolumeRenderingPropertiesWidget::mrmlNode()const
{
  return vtkMRMLVolumeRenderingDisplayNode::SafeDownCast(
    this->mrmlVolumeRenderingDisplayNode());
}

//-----------------------------------------------------------------------------
vtkMRMLVolumeRenderingDisplayNode* qCjyxVolumeRenderingPropertiesWidget
::mrmlVolumeRenderingDisplayNode()const
{
  Q_D(const qCjyxVolumeRenderingPropertiesWidget);
  return d->VolumeRenderingDisplayNode;
}

//-----------------------------------------------------------------------------
vtkMRMLVolumeNode* qCjyxVolumeRenderingPropertiesWidget
::mrmlVolumeNode()const
{
  Q_D(const qCjyxVolumeRenderingPropertiesWidget);
  return d->VolumeNode;
}

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingPropertiesWidget
::setMRMLNode(vtkMRMLNode* node)
{
  this->setMRMLVolumeRenderingDisplayNode(
    vtkMRMLVolumeRenderingDisplayNode::SafeDownCast(node));
}

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingPropertiesWidget
::setMRMLVolumeRenderingDisplayNode(vtkMRMLVolumeRenderingDisplayNode* displayNode)
{
  Q_D(qCjyxVolumeRenderingPropertiesWidget);
  qvtkReconnect(d->VolumeRenderingDisplayNode, displayNode, vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromMRML()));

  d->VolumeRenderingDisplayNode = displayNode;
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingPropertiesWidget::updateWidgetFromMRML()
{
  Q_D(qCjyxVolumeRenderingPropertiesWidget);
  vtkMRMLVolumeNode* newVolumeNode =
    d->VolumeRenderingDisplayNode ? d->VolumeRenderingDisplayNode->GetVolumeNode() : nullptr;
  qvtkReconnect(d->VolumeNode, newVolumeNode, vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromMRMLVolumeNode()));
  d->VolumeNode = newVolumeNode;
  this->updateWidgetFromMRMLVolumeNode();
}

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingPropertiesWidget::updateWidgetFromMRMLVolumeNode()
{
}
