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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// .NAME vtkCjyxCamerasModuleLogic - Logic class for Cameras module

#ifndef __vtkCjyxCamerasModuleLogic_h
#define __vtkCjyxCamerasModuleLogic_h

// Cjyx includes
#include "vtkCjyxCamerasModuleLogicExport.h"
#include "vtkCjyxModuleLogic.h"

// MRML includes
class vtkMRMLCameraNode;
class vtkMRMLViewNode;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
///
/// Properties of the scene-to-import camera nodes are always
/// copied into the existing nodes having the same name. This is done
/// when a camera node is about to be added to the scene.
///
class VTK_CJYX_CAMERAS_LOGIC_EXPORT vtkCjyxCamerasModuleLogic
  : public vtkCjyxModuleLogic
{
public:
  static vtkCjyxCamerasModuleLogic *New();
  vtkTypeMacro(vtkCjyxCamerasModuleLogic, vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Scan the scene and search for the active camera that is used
  /// in the view.
  vtkMRMLCameraNode* GetViewActiveCameraNode(vtkMRMLViewNode* view);

protected:
  vtkCjyxCamerasModuleLogic();
  ~vtkCjyxCamerasModuleLogic() override;

private:
  vtkCjyxCamerasModuleLogic(const vtkCjyxCamerasModuleLogic&) = delete;
  void operator=(const vtkCjyxCamerasModuleLogic&) = delete;
};

#endif
