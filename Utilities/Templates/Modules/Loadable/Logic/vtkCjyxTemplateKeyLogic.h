/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// .NAME vtkCjyxTemplateKeyLogic - cjyx logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkCjyxTemplateKeyLogic_h
#define __vtkCjyxTemplateKeyLogic_h

// Cjyx includes
#include "vtkCjyxModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkCjyxTemplateKeyModuleLogicExport.h"


/// \ingroup Cjyx_QtModules_ExtensionTemplate
class VTK_CJYX_TEMPLATEKEY_MODULE_LOGIC_EXPORT vtkCjyxTemplateKeyLogic :
  public vtkCjyxModuleLogic
{
public:

  static vtkCjyxTemplateKeyLogic *New();
  vtkTypeMacro(vtkCjyxTemplateKeyLogic, vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkCjyxTemplateKeyLogic();
  ~vtkCjyxTemplateKeyLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

  vtkCjyxTemplateKeyLogic(const vtkCjyxTemplateKeyLogic&); // Not implemented
  void operator=(const vtkCjyxTemplateKeyLogic&); // Not implemented
};

#endif
