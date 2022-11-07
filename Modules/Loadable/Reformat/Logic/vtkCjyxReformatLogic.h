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

  This file was originally developed by Michael Jeulin-Lagarrigue, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// .NAME vtkCjyxReformatLogic - cjyx logic class for volumes manipulation
// .SECTION Description
// This class manages the logic associated with reading, saving,
// and changing propertied of the volumes


#ifndef __vtkCjyxReformatLogic_h
#define __vtkCjyxReformatLogic_h

// Cjyx includes
#include "vtkCjyxModuleLogic.h"

// MRML includes

// STD includes
#include <cstdlib>

#include "vtkCjyxReformatModuleLogicExport.h"


/// \ingroup Cjyx_QtModules_TransformsReformatWidget
class VTK_CJYX_REFORMAT_MODULE_LOGIC_EXPORT
vtkCjyxReformatLogic : public vtkCjyxModuleLogic
{
public:
  static vtkCjyxReformatLogic *New();
  typedef vtkCjyxReformatLogic Self;
  vtkTypeMacro(vtkCjyxReformatLogic,vtkCjyxModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Set the world coordinate origin position
  static void SetSliceOrigin(vtkMRMLSliceNode* node, double x, double y, double z);
  static void SetSliceOrigin(vtkMRMLSliceNode* node, double position[3]);

  /// Set the normal to the plane of the slice node.
  static void SetSliceNormal(vtkMRMLSliceNode* node, double x, double y, double z);
  static void SetSliceNormal(vtkMRMLSliceNode* node, double normal[3]);

  /// Compute and return the volume bounding box
  static void GetVolumeBounds(vtkMRMLSliceNode* node, double bounds[6]);

  /// Compute the center from a bounds
  static void GetCenterFromBounds(double bounds[6], double center[3]);

protected:
  vtkCjyxReformatLogic();
  ~vtkCjyxReformatLogic() override;

private:

  vtkCjyxReformatLogic(const vtkCjyxReformatLogic&) = delete;
  void operator=(const vtkCjyxReformatLogic&) = delete;
};

#endif

