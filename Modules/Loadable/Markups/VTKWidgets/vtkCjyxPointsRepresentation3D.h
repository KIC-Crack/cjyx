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

/**
 * @class   vtkCjyxPointsRepresentation3D
 * @brief   Default representation for the points widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation3D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxPointsRepresentation3D_h
#define vtkCjyxPointsRepresentation3D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation3D.h"

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxPointsRepresentation3D : public vtkCjyxMarkupsWidgetRepresentation3D
{
public:
  /// Instantiate this class.
  static vtkCjyxPointsRepresentation3D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxPointsRepresentation3D,vtkCjyxMarkupsWidgetRepresentation3D);

protected:
  vtkCjyxPointsRepresentation3D();
  ~vtkCjyxPointsRepresentation3D() override;

private:
  vtkCjyxPointsRepresentation3D(const vtkCjyxPointsRepresentation3D&) = delete;
  void operator=(const vtkCjyxPointsRepresentation3D&) = delete;
};

#endif
