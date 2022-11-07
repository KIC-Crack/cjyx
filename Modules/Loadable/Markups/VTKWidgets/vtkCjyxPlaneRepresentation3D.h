/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, Cancer
  Care Ontario, OpenAnatomy, and Brigham and Women's Hospital through NIH grant R01MH112748.

==============================================================================*/

/**
 * @class   vtkCjyxPlaneRepresentation3D
 * @brief   Default representation for the plane widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation3D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxPlaneRepresentation3D_h
#define vtkCjyxPlaneRepresentation3D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation3D.h"

class vtkActor;
class vtkArrayCalculator;
class vtkAppendPolyData;
class vtkArrowSource;
class vtkGlyph3DMapper;
class vtkLookupTable;
class vtkMRMLInteractionEventData;
class vtkPlaneSource;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkTransformPolyDataFilter;
class vtkTubeFilter;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxPlaneRepresentation3D : public vtkCjyxMarkupsWidgetRepresentation3D
{
public:
  /// Instantiate this class.
  static vtkCjyxPlaneRepresentation3D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxPlaneRepresentation3D,vtkCjyxMarkupsWidgetRepresentation3D);
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

  void CanInteractWithPlane(vtkMRMLInteractionEventData* interactionEventData,
    int& foundComponentType, int& foundComponentIndex, double& closestDistance2);

protected:
  vtkCjyxPlaneRepresentation3D();
  ~vtkCjyxPlaneRepresentation3D() override;

  vtkNew<vtkPlaneSource>     PlaneFillFilter;
  vtkNew<vtkArrowSource>     ArrowFilter;
  vtkNew<vtkGlyph3D>         ArrowGlypher;
  vtkNew<vtkTubeFilter>      PlaneOutlineFilter;
  vtkNew<vtkArrayCalculator> ArrowColorFilter;
  vtkNew<vtkArrayCalculator> PlaneOutlineColorFilter;
  vtkNew<vtkArrayCalculator> PlaneFillColorFilter;
  vtkNew<vtkAppendPolyData>  Append;

  vtkNew<vtkActor>           PlaneActor;
  vtkNew<vtkActor>           PlaneOccludedActor;

  vtkNew<vtkPolyDataMapper>  PlaneMapper;
  vtkNew<vtkPolyDataMapper>  PlaneOccludedMapper;

  vtkNew<vtkLookupTable>    PlaneColorLUT;

  std::string LabelFormat;

  // Setup the pipeline for plane display
  void BuildPlane();


  // Initialize interaction handle pipeline
  void SetupInteractionPipeline() override;

  // Update visibility of interaction handles for representation
  void UpdateInteractionPipeline() override;

  class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT MarkupsInteractionPipelinePlane : public MarkupsInteractionPipeline
  {
  public:
    MarkupsInteractionPipelinePlane(vtkCjyxMarkupsWidgetRepresentation* representation);
    ~MarkupsInteractionPipelinePlane() override = default;

    // Initialize scale handles
    void CreateScaleHandles() override;

    HandleInfoList GetHandleInfoList() override;

    // Update scale handle positions
    virtual void UpdateScaleHandles();

    // Update scale handle visibilities
    void UpdateHandleVisibility() override;

    void GetHandleColor(int type, int index, double color[4]) override;
    double GetHandleOpacity(int type, int index) override;

    void GetInteractionHandleAxisWorld(int type, int index, double axis[3]) override;
  };
  friend class vtkCjyxPlaneRepresentation2D;

private:
  vtkCjyxPlaneRepresentation3D(const vtkCjyxPlaneRepresentation3D&) = delete;
  void operator=(const vtkCjyxPlaneRepresentation3D&) = delete;
};

#endif
