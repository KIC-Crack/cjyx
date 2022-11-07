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

// .NAME vtkCjyxLoadableModuleTemplateLogic - cjyx logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkCjyxLoadableModuleTemplateLogic_h
#define __vtkCjyxLoadableModuleTemplateLogic_h

// Cjyx includes
#include "vtkCjyxModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkCjyxLoadableModuleTemplateModuleLogicExport.h"


/// \ingroup Cjyx_QtModules_ExtensionTemplate
class VTK_CJYX_LOADABLEMODULETEMPLATE_MODULE_LOGIC_EXPORT vtkCjyxLoadableModuleTemplateLogic :
  public vtkCjyxModuleLogic
{
public:

  static vtkCjyxLoadableModuleTemplateLogic *New();
  vtkTypeMacro(vtkCjyxLoadableModuleTemplateLogic, vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkCjyxLoadableModuleTemplateLogic();
  ~vtkCjyxLoadableModuleTemplateLogic() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;
  void UpdateFromMRMLScene() override;
  void OnMRMLSceneNodeAdded(vtkMRMLNode* node) override;
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* node) override;
private:

  vtkCjyxLoadableModuleTemplateLogic(const vtkCjyxLoadableModuleTemplateLogic&); // Not implemented
  void operator=(const vtkCjyxLoadableModuleTemplateLogic&); // Not implemented
};

#endif
