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
 * @class   vtkCjyxCurveRepresentation2D
 * @brief   Default representation for the line widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation2D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxCurveRepresentation2D_h
#define vtkCjyxCurveRepresentation2D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation2D.h"

class vtkCellLocator;
class vtkDiscretizableColorTransferFunction;
class vtkSampleImplicitFunctionFilter;
class vtkTubeFilter;

class vtkMRMLInteractionEventData;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxCurveRepresentation2D : public vtkCjyxMarkupsWidgetRepresentation2D
{
public:
  /// Instantiate this class.
  static vtkCjyxCurveRepresentation2D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxCurveRepresentation2D,vtkCjyxMarkupsWidgetRepresentation2D);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Subclasses of vtkContourCurveRepresentation must implement these methods. These
  /// are the methods that the widget and its representation use to
  /// communicate with each other.
  void UpdateFromMRML(vtkMRMLNode* caller, unsigned long event, void *callData = nullptr) override;
  void CanInteract(vtkMRMLInteractionEventData* interactionEventData,
    int &foundComponentType, int &foundComponentIndex, double &closestDistance2) override;

  /// Methods to make this class behave as a vtkProp.
  void GetActors(vtkPropCollection *) override;
  void ReleaseGraphicsResources(vtkWindow *) override;
  int RenderOverlay(vtkViewport *viewport) override;
  int RenderOpaqueGeometry(vtkViewport *viewport) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport *viewport) override;
  vtkTypeBool HasTranslucentPolygonalGeometry() override;

  /// Return the bounds of the representation
  double *GetBounds() override;

  void CanInteractWithCurve(vtkMRMLInteractionEventData* interactionEventData,
    int &foundComponentType, int &componentIndex, double &closestDistance2);

protected:
  vtkCjyxCurveRepresentation2D();
  ~vtkCjyxCurveRepresentation2D() override;

  void SetMarkupsNode(vtkMRMLMarkupsNode *markupsNode) override;

  void UpdateLineColorMap();

  vtkSmartPointer<vtkPolyData> Line;
  vtkSmartPointer<vtkPolyDataMapper2D> LineMapper;
  vtkSmartPointer<vtkActor2D> LineActor;
  vtkSmartPointer<vtkDiscretizableColorTransferFunction> LineColorMap;

  vtkSmartPointer<vtkTubeFilter> TubeFilter;

  vtkSmartPointer<vtkTransformPolyDataFilter> WorldToSliceTransformer;
  vtkSmartPointer<vtkCellLocator> SliceCurvePointLocator;

  vtkSmartPointer<vtkSampleImplicitFunctionFilter> SliceDistance;

private:
  vtkCjyxCurveRepresentation2D(const vtkCjyxCurveRepresentation2D&) = delete;
  void operator=(const vtkCjyxCurveRepresentation2D&) = delete;
};

#endif
