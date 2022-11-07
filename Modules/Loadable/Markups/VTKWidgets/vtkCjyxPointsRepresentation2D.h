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
 * @class   vtkCjyxPointsRepresentation2D
 * @brief   Default representation for the points widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkMRMLAbstractWidgetRepresentation3D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxPointsRepresentation2D_h
#define vtkCjyxPointsRepresentation2D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation2D.h"

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxPointsRepresentation2D : public vtkCjyxMarkupsWidgetRepresentation2D
{
public:
  /// Instantiate this class.
  static vtkCjyxPointsRepresentation2D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxPointsRepresentation2D,vtkCjyxMarkupsWidgetRepresentation2D);

protected:
  vtkCjyxPointsRepresentation2D();
  ~vtkCjyxPointsRepresentation2D() override;

private:
  vtkCjyxPointsRepresentation2D(const vtkCjyxPointsRepresentation2D&) = delete;
  void operator=(const vtkCjyxPointsRepresentation2D&) = delete;
};

#endif
