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

#include "vtkCjyxPointsWidget.h"

#include "vtkMRMLInteractionEventData.h"
#include "vtkMRMLSliceNode.h"
#include "vtkCjyxPointsRepresentation2D.h"
#include "vtkCjyxPointsRepresentation3D.h"

vtkStandardNewMacro(vtkCjyxPointsWidget);

//----------------------------------------------------------------------
vtkCjyxPointsWidget::vtkCjyxPointsWidget() = default;

//----------------------------------------------------------------------
vtkCjyxPointsWidget::~vtkCjyxPointsWidget() = default;

//----------------------------------------------------------------------
void vtkCjyxPointsWidget::CreateDefaultRepresentation(
  vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer)
{
  vtkSmartPointer<vtkCjyxMarkupsWidgetRepresentation> rep = nullptr;
  if (vtkMRMLSliceNode::SafeDownCast(viewNode))
    {
    rep = vtkSmartPointer<vtkCjyxPointsRepresentation2D>::New();
    }
  else
    {
    rep = vtkSmartPointer<vtkCjyxPointsRepresentation3D>::New();
    }
  this->SetRenderer(renderer);
  this->SetRepresentation(rep);
  rep->SetViewNode(viewNode);
  rep->SetMarkupsDisplayNode(markupsDisplayNode);
  rep->UpdateFromMRML(nullptr, 0); // full update
}
