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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __vtkCjyxScriptedLoadableModuleLogic_h
#define __vtkCjyxScriptedLoadableModuleLogic_h


// Cjyx includes
#include "vtkCjyxModuleLogic.h"

// VTK includes
#include "vtkObject.h"
#include "vtkObjectFactory.h"

class VTK_CJYX_BASE_LOGIC_EXPORT vtkCjyxScriptedLoadableModuleLogic :
  public vtkCjyxModuleLogic
{
public:

  static vtkCjyxScriptedLoadableModuleLogic *New();
  vtkTypeMacro(vtkCjyxScriptedLoadableModuleLogic, vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  bool SetPythonSource(const std::string& pythonSource);

protected:

  vtkCjyxScriptedLoadableModuleLogic();
  ~vtkCjyxScriptedLoadableModuleLogic() override;

//  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);

private:

  vtkCjyxScriptedLoadableModuleLogic(const vtkCjyxScriptedLoadableModuleLogic&) = delete;
  void operator=(const vtkCjyxScriptedLoadableModuleLogic&) = delete;

  class vtkInternal;
  vtkInternal * Internal;
};

#endif

