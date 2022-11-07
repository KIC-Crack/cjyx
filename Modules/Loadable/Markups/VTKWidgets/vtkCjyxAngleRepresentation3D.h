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
 * @class   vtkCjyxAngleRepresentation3D
 * @brief   Default representation for the angle widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation3D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxAngleRepresentation3D_h
#define vtkCjyxAngleRepresentation3D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation3D.h"

class vtkActor;
class vtkArcSource;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkTubeFilter;

class vtkMRMLInteractionEventData;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxAngleRepresentation3D : public vtkCjyxMarkupsWidgetRepresentation3D
{
public:
  /// Instantiate this class.
  static vtkCjyxAngleRepresentation3D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxAngleRepresentation3D,vtkCjyxMarkupsWidgetRepresentation3D);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Subclasses of vtkMRMLAbstractWidgetRepresentation must implement these methods. These
  /// are the methods that the widget and its representation use to
  /// communicate with each other.
  void UpdateFromMRML(vtkMRMLNode* caller, unsigned long event, void *callData=nullptr) override;

  /// Methods to make this class behave as a vtkProp.
  void GetActors(vtkPropCollection *) override;
  void ReleaseGraphicsResources(vtkWindow *) override;
  int RenderOverlay(vtkViewport *viewport) override;
  int RenderOpaqueGeometry(vtkViewport *viewport) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport *viewport) override;
  vtkTypeBool HasTranslucentPolygonalGeometry() override;

  /// Return the bounds of the representation
  double *GetBounds() override;

  bool GetTransformationReferencePoint(double referencePointWorld[3]) override;

  void CanInteract(vtkMRMLInteractionEventData* interactionEventData,
    int &foundComponentType, int &foundComponentIndex, double &closestDistance2) override;

protected:
  vtkCjyxAngleRepresentation3D();
  ~vtkCjyxAngleRepresentation3D() override;

  vtkSmartPointer<vtkPolyData>   Line;
  vtkSmartPointer<vtkArcSource>  Arc;
  vtkSmartPointer<vtkTubeFilter> TubeFilter;
  vtkSmartPointer<vtkTubeFilter> ArcTubeFilter;

  vtkSmartPointer<vtkPolyDataMapper> LineMapper;
  vtkSmartPointer<vtkPolyDataMapper> ArcMapper;
  vtkSmartPointer<vtkPolyDataMapper> LineOccludedMapper;
  vtkSmartPointer<vtkPolyDataMapper> ArcOccludedMapper;

  vtkSmartPointer<vtkActor> LineActor;
  vtkSmartPointer<vtkActor> ArcActor;
  vtkSmartPointer<vtkActor> LineOccludedActor;
  vtkSmartPointer<vtkActor> ArcOccludedActor;

  void BuildArc();

  // Update visibility of interaction handles for representation
  void UpdateInteractionPipeline() override;

private:
  vtkCjyxAngleRepresentation3D(const vtkCjyxAngleRepresentation3D&) = delete;
  void operator=(const vtkCjyxAngleRepresentation3D&) = delete;
};

#endif
