/*=========================================================================

 Copyright (c) ProxSim ltd., Kwun Tong, Hong Kong. All Rights Reserved.

 See COPYRIGHT.txt
 or http://www.slicer.org/copyright/copyright.txt for details.

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 This file was originally developed by Davide Punzo, punzodavide@hotmail.it,
 and development was supported by ProxSim ltd.

=========================================================================*/

#include "vtkCjyxLineWidget.h"
#include "vtkMRMLSliceNode.h"
#include "vtkCjyxLineRepresentation2D.h"
#include "vtkCjyxLineRepresentation3D.h"

// VTK includes
#include "vtkCommand.h"
#include "vtkEvent.h"

vtkStandardNewMacro(vtkCjyxLineWidget);

//----------------------------------------------------------------------
vtkCjyxLineWidget::vtkCjyxLineWidget()
{
  this->SetEventTranslationClickAndDrag(WidgetStateOnWidget, vtkCommand::LeftButtonPressEvent, vtkEvent::AltModifier,
    WidgetStateRotate, WidgetEventRotateStart, WidgetEventRotateEnd);
  this->SetEventTranslationClickAndDrag(WidgetStateOnWidget, vtkCommand::RightButtonPressEvent, vtkEvent::AltModifier,
    WidgetStateScale, WidgetEventScaleStart, WidgetEventScaleEnd);
}

//----------------------------------------------------------------------
vtkCjyxLineWidget::~vtkCjyxLineWidget() = default;

//----------------------------------------------------------------------
void vtkCjyxLineWidget::CreateDefaultRepresentation(
  vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer)
{
  vtkSmartPointer<vtkCjyxMarkupsWidgetRepresentation> rep = nullptr;
  if (vtkMRMLSliceNode::SafeDownCast(viewNode))
    {
    rep = vtkSmartPointer<vtkCjyxLineRepresentation2D>::New();
    }
  else
    {
    rep = vtkSmartPointer<vtkCjyxLineRepresentation3D>::New();
    }
  this->SetRenderer(renderer);
  this->SetRepresentation(rep);
  rep->SetViewNode(viewNode);
  rep->SetMarkupsDisplayNode(markupsDisplayNode);
  rep->UpdateFromMRML(nullptr, 0); // full update
}
