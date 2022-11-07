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
 * @class   vtkCjyxCurveWidget
 * @brief   create a curve with a set of N points
 *
 * The vtkCjyxCurveWidget is used to create a curve widget with a set of N points.
 *
*/

#ifndef vtkCjyxCurveWidget_h
#define vtkCjyxCurveWidget_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidget.h"

class vtkPolyData;
class vtkIdList;
class vtkMRMLMarkupsCurveNode;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxCurveWidget : public vtkCjyxMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkCjyxCurveWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkCjyxCurveWidget, vtkCjyxMarkupsWidget);

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

  /// Create instance of the markups widget
  vtkCjyxMarkupsWidgetCreateInstanceMacro(vtkCjyxCurveWidget);

protected:
  vtkCjyxCurveWidget();
  ~vtkCjyxCurveWidget() override;

  bool ProcessControlPointInsert(vtkMRMLInteractionEventData* eventData) override;

  vtkMRMLMarkupsCurveNode* GetMarkupsCurveNode();

private:
  vtkCjyxCurveWidget(const vtkCjyxCurveWidget&) = delete;
  void operator=(const vtkCjyxCurveWidget&) = delete;
};

#endif
