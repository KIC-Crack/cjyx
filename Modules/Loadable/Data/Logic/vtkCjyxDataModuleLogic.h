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

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// .NAME vtkCjyxDataModuleLogic - cjyx logic class for data module

#ifndef __vtkCjyxDataModuleLogic_h
#define __vtkCjyxDataModuleLogic_h

// Cjyx includes
#include "vtkCjyxModuleLogic.h"

// STD includes
#include <cstdlib>

#include "vtkCjyxDataModuleLogicExport.h"

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class VTK_CJYX_DATA_LOGIC_EXPORT vtkCjyxDataModuleLogic :
  public vtkCjyxModuleLogic
{
public:
  static vtkCjyxDataModuleLogic *New();
  vtkTypeMacro(vtkCjyxDataModuleLogic, vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

public:
  vtkSetMacro( SceneChanged, bool );
  vtkGetMacro( SceneChanged, bool );
  vtkBooleanMacro( SceneChanged, bool );

protected:
  vtkCjyxDataModuleLogic();
  ~vtkCjyxDataModuleLogic() override;

  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  void RegisterNodes() override;

  void SetMRMLSceneInternal(vtkMRMLScene* newScene) override;
  void UpdateFromMRMLScene() override;

  /// Reimplemented to delete the storage/display nodes when a displayable
  /// node is being removed.
  void OnMRMLSceneNodeRemoved(vtkMRMLNode* removedNode) override;

  ///
  /// This property controls whether the removal from the scene of a model node
  /// automatically removes its display and storage nodes or not.
  /// This automatic behavior (true by default) is used to prevent the user
  /// from seeing a model (display node) in a 3D view after a model node
  /// has been deleted/removed (delete from a tree view qt widget).
  /// If the nodes were not removed/deleted, the display and storage nodes
  /// would be zombie nodes in the scene with no one pointing on them.
  bool AutoRemoveDisplayAndStorageNodes;

private:
  vtkCjyxDataModuleLogic(const vtkCjyxDataModuleLogic&) = delete;
  void operator=(const vtkCjyxDataModuleLogic&) = delete;

protected:
  /// Flag indicating if the scene has recently changed (update of the module GUI if needed)
  bool SceneChanged;

};

#endif
