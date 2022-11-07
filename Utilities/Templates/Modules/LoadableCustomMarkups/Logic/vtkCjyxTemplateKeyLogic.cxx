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

#include "vtkCjyxTemplateKeyLogic.h"

// TemplateKey MRML includes
#include "vtkMRMLMarkupsTestLineNode.h"

// TemplateKey VTKWidgets includes
#include "vtkCjyxTestLineWidget.h"

// MRML includes
#include <vtkMRMLScene.h>

// Markups logic includes
#include <vtkCjyxMarkupsLogic.h>

// Markups MRML includes
#include <vtkMRMLMarkupsDisplayNode.h>

// VTK includes
#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxTemplateKeyLogic);

//---------------------------------------------------------------------------
vtkCjyxTemplateKeyLogic::vtkCjyxTemplateKeyLogic()
{
}

//---------------------------------------------------------------------------
vtkCjyxTemplateKeyLogic::~vtkCjyxTemplateKeyLogic() = default;

//---------------------------------------------------------------------------
void vtkCjyxTemplateKeyLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//-----------------------------------------------------------------------------
void vtkCjyxTemplateKeyLogic::RegisterNodes()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("RegisterNodes failed: invalid scene");
    return;
    }

  vtkCjyxMarkupsLogic* markupsLogic = vtkCjyxMarkupsLogic::SafeDownCast(this->GetModuleLogic("Markups"));
  if (!markupsLogic)
    {
    vtkErrorMacro("RegisterNodes failed: invalid markups module logic");
    return;
    }

  vtkNew<vtkMRMLMarkupsTestLineNode> markupsTestLineNode;
  vtkNew<vtkCjyxTestLineWidget> testLineWidget;
  markupsLogic->RegisterMarkupsNode(markupsTestLineNode, testLineWidget);
}
