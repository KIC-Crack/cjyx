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
 * @class   vtkCjyxPointsWidget
 * @brief   Widget to display a set of interactive points.
 *
 *
 *
*/

#ifndef vtkCjyxPointsWidget_h
#define vtkCjyxPointsWidget_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidget.h"

class vtkPolyData;
class vtkIdList;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxPointsWidget : public vtkCjyxMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkCjyxPointsWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkCjyxPointsWidget, vtkCjyxMarkupsWidget);

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

  /// Create instance of the markups widget
  vtkCjyxMarkupsWidgetCreateInstanceMacro(vtkCjyxPointsWidget);

protected:
  vtkCjyxPointsWidget();
  ~vtkCjyxPointsWidget() override;

private:
  vtkCjyxPointsWidget(const vtkCjyxPointsWidget&) = delete;
  void operator=(const vtkCjyxPointsWidget&) = delete;
};

#endif
