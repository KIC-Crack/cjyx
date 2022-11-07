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

// Cjyx includes
#include "qCjyxCamerasModuleWidget.h"
#include "ui_qCjyxCamerasModuleWidget.h"
#include "vtkCjyxCamerasModuleLogic.h"

// MRML includes
#include "vtkMRMLViewNode.h"
#include "vtkMRMLCameraNode.h"
#include "vtkMRMLScene.h"

// STD includes

//-----------------------------------------------------------------------------
class qCjyxCamerasModuleWidgetPrivate: public Ui_qCjyxCamerasModuleWidget
{
public:
};

//-----------------------------------------------------------------------------
qCjyxCamerasModuleWidget::qCjyxCamerasModuleWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxCamerasModuleWidgetPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxCamerasModuleWidget::~qCjyxCamerasModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::setup()
{
  Q_D(qCjyxCamerasModuleWidget);
  d->setupUi(this);

  connect(d->ViewNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
          this, SLOT(onCurrentViewNodeChanged(vtkMRMLNode*)));
  connect(d->CameraNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
          this, SLOT(setCameraToCurrentView(vtkMRMLNode*)));
  connect(d->CameraNodeSelector, SIGNAL(nodeAdded(vtkMRMLNode*)),
          this, SLOT(onCameraNodeAdded(vtkMRMLNode*)));
  connect(d->CameraNodeSelector, SIGNAL(nodeAboutToBeRemoved(vtkMRMLNode*)),
          this, SLOT(onCameraNodeRemoved(vtkMRMLNode*)));
}

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::onCurrentViewNodeChanged(vtkMRMLNode* mrmlNode)
{
  vtkMRMLViewNode* currentViewNode = vtkMRMLViewNode::SafeDownCast(mrmlNode);
  this->synchronizeCameraWithView(currentViewNode);
}

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::synchronizeCameraWithView()
{
  Q_D(qCjyxCamerasModuleWidget);
  vtkMRMLViewNode* currentViewNode = vtkMRMLViewNode::SafeDownCast(
    d->ViewNodeSelector->currentNode());
  this->synchronizeCameraWithView(currentViewNode);
}

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::synchronizeCameraWithView(vtkMRMLViewNode* currentViewNode)
{
  Q_D(qCjyxCamerasModuleWidget);
  if (!currentViewNode)
    {
    return;
    }
  vtkCjyxCamerasModuleLogic* camerasLogic =
    vtkCjyxCamerasModuleLogic::SafeDownCast(this->logic());
  vtkMRMLCameraNode *found_camera_node =
    camerasLogic->GetViewActiveCameraNode(currentViewNode);
  d->CameraNodeSelector->setCurrentNode(found_camera_node);
}


//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::setCameraToCurrentView(vtkMRMLNode* mrmlNode)
{
  Q_D(qCjyxCamerasModuleWidget);
  vtkMRMLCameraNode *currentCameraNode =
        vtkMRMLCameraNode::SafeDownCast(mrmlNode);
  if (!currentCameraNode)
    {// if the camera list is empty, there is no current camera
    return;
    }
  vtkMRMLViewNode *currentViewNode = vtkMRMLViewNode::SafeDownCast(
    d->ViewNodeSelector->currentNode());
  if (currentViewNode == nullptr)
    {
    return;
    }
  currentCameraNode->SetLayoutName(currentViewNode->GetLayoutName());
}

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::onCameraNodeAdded(vtkMRMLNode* mrmlNode)
{
  vtkMRMLCameraNode *cameraNode = vtkMRMLCameraNode::SafeDownCast(mrmlNode);
  if (!cameraNode)
    {
    //Q_ASSERT(cameraNode);
    return;
    }
  this->qvtkConnect(cameraNode, vtkMRMLCameraNode::LayoutNameModifiedEvent,
                    this, SLOT(synchronizeCameraWithView()));
}

//-----------------------------------------------------------------------------
void qCjyxCamerasModuleWidget::onCameraNodeRemoved(vtkMRMLNode* mrmlNode)
{
  vtkMRMLCameraNode *cameraNode = vtkMRMLCameraNode::SafeDownCast(mrmlNode);
  if (!cameraNode)
    {
    //Q_ASSERT(cameraNode);
    return;
    }
  this->qvtkDisconnect(cameraNode, vtkMRMLCameraNode::LayoutNameModifiedEvent,
                       this, SLOT(synchronizeCameraWithView()));
}

//-----------------------------------------------------------------------------
void  qCjyxCamerasModuleWidget::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);

  // When the view and camera selectors populate their items, the view might populate
  // its items before the camera, and the synchronizeCameraWithView() might do nothing.
  // Let's resync here.
  this->synchronizeCameraWithView();
}

//-----------------------------------------------------------
bool qCjyxCamerasModuleWidget::setEditedNode(vtkMRMLNode* node,
                                               QString role /* = QString()*/,
                                               QString context /* = QString()*/)
{
  Q_D(qCjyxCamerasModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);

  if (vtkMRMLViewNode::SafeDownCast(node))
    {
    d->ViewNodeSelector->setCurrentNode(node);
    return true;
    }

  if (vtkMRMLCameraNode::SafeDownCast(node))
    {
    vtkMRMLCameraNode* cameraNode = vtkMRMLCameraNode::SafeDownCast(node);
    vtkMRMLViewNode* viewNode = vtkMRMLViewNode::SafeDownCast(
      this->mrmlScene()->GetSingletonNode(cameraNode->GetLayoutName(), "vtkMRMLViewNode"));
    if (!viewNode)
      {
      return false;
      }
    d->ViewNodeSelector->setCurrentNode(viewNode);
    return true;
    }

  return false;
}
