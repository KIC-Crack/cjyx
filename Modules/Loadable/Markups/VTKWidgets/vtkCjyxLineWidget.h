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
 * @class   vtkCjyxLineWidget
 * @brief   create a line with a set of 2 points
 *
 * The vtkCjyxLineWidget is used to create a line widget with a set of 2 points.
 *
*/

#ifndef vtkCjyxLineWidget_h
#define vtkCjyxLineWidget_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidget.h"

class vtkCjyxMarkupsWidgetRepresentation;
class vtkPolyData;
class vtkIdList;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxLineWidget : public vtkCjyxMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkCjyxLineWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkCjyxLineWidget,vtkCjyxMarkupsWidget);

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

  /// Create instance of the markups widget
  vtkCjyxMarkupsWidgetCreateInstanceMacro(vtkCjyxLineWidget);

protected:
  vtkCjyxLineWidget();
  ~vtkCjyxLineWidget() override;

private:
  vtkCjyxLineWidget(const vtkCjyxLineWidget&) = delete;
  void operator=(const vtkCjyxLineWidget&) = delete;
};

#endif
