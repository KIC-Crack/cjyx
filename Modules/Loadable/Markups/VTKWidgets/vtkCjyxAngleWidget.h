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
 * @class   vtkCjyxAngleWidget
 * @brief   create an angle with a set of 3 points
 *
 * The vtkCjyxAngleWidget is used to create an angle widget with a set of 3 points.
 *
*/

#ifndef vtkCjyxAngleWidget_h
#define vtkCjyxAngleWidget_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidget.h"

class vtkCjyxMarkupsWidgetRepresentation;
class vtkPolyData;
class vtkIdList;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxAngleWidget : public vtkCjyxMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkCjyxAngleWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkCjyxAngleWidget,vtkCjyxMarkupsWidget);

  /// Create instance of the markups widget
  vtkCjyxMarkupsWidgetCreateInstanceMacro(vtkCjyxAngleWidget);

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

protected:
  vtkCjyxAngleWidget();
  ~vtkCjyxAngleWidget() override;

private:
  vtkCjyxAngleWidget(const vtkCjyxAngleWidget&) = delete;
  void operator=(const vtkCjyxAngleWidget&) = delete;
};

#endif
