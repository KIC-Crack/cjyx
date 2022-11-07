/*==============================================================================

  Copyright (c) The Intervention Centre
  Oslo University Hospital, Oslo, Norway. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Rafael Palomar (The Intervention Centre,
  Oslo University Hospital) and was supported by The Research Council of Norway
  through the ALive project (grant nr. 311393).

==============================================================================*/

#include "vtkCjyxTestLineWidget.h"

// Liver Markups VTKWidgets include
#include "vtkCjyxTestLineRepresentation3D.h"
#include "vtkCjyxTestLineRepresentation2D.h"

// VTK includes
#include <vtkObjectFactory.h>

// MRML includes
#include <vtkMRMLSliceNode.h>

//------------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxTestLineWidget);

//------------------------------------------------------------------------------
vtkCjyxTestLineWidget::vtkCjyxTestLineWidget()
{

}

//------------------------------------------------------------------------------
vtkCjyxTestLineWidget::~vtkCjyxTestLineWidget() = default;

//------------------------------------------------------------------------------
void vtkCjyxTestLineWidget::CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode,
                                                          vtkMRMLAbstractViewNode* viewNode,
                                                          vtkRenderer* renderer)
{
  vtkSmartPointer<vtkCjyxMarkupsWidgetRepresentation> rep = nullptr;
  if (vtkMRMLSliceNode::SafeDownCast(viewNode))
    {
    rep = vtkSmartPointer<vtkCjyxTestLineRepresentation2D>::New();
    }
  else
    {
    rep = vtkSmartPointer<vtkCjyxTestLineRepresentation3D>::New();
    }
  this->SetRenderer(renderer);
  this->SetRepresentation(rep);
  rep->SetViewNode(viewNode);
  rep->SetMarkupsDisplayNode(markupsDisplayNode);
  rep->UpdateFromMRML(nullptr, 0); // full update
}

//------------------------------------------------------------------------------
vtkCjyxMarkupsWidget* vtkCjyxTestLineWidget::CreateInstance() const
{
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkCjyxTestLineWidget");
  if(ret)
    {
    return static_cast<vtkCjyxTestLineWidget*>(ret);
    }

  vtkCjyxTestLineWidget* result = new vtkCjyxTestLineWidget;
#ifdef VTK_HAS_INITIALIZE_OBJECT_BASE
  result->InitializeObjectBase();
#endif
  return result;
}
