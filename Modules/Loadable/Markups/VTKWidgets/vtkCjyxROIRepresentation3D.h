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
 * @class   vtkCjyxROIRepresentation3D
 * @brief   Default representation for the plane widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation3D vtkMRMLAbstractWidget
*/

#ifndef vtkCjyxROIRepresentation3D_h
#define vtkCjyxROIRepresentation3D_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidgetRepresentation3D.h"

class vtkActor;
class vtkArrayCalculator;
class vtkAppendPolyData;
class vtkArrowSource;
class vtkGlyph3DMapper;
class vtkLookupTable;
class vtkMRMLInteractionEventData;
class vtkMRMLMarkupsROINode;
class vtkOutlineFilter;
class vtkPassThroughFilter;
class vtkPlaneSource;
class vtkPolyDataAlgorithm;
class vtkPolyDataMapper;
class vtkPolyData;
class vtkTransformPolyDataFilter;
class vtkTubeFilter;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxROIRepresentation3D : public vtkCjyxMarkupsWidgetRepresentation3D
{
public:
  /// Instantiate this class.
  static vtkCjyxROIRepresentation3D *New();

  /// Standard methods for instances of this class.
  vtkTypeMacro(vtkCjyxROIRepresentation3D,vtkCjyxMarkupsWidgetRepresentation3D);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Subclasses of vtkMRMLAbstractWidgetRepresentation must implement these methods. These
  /// are the methods that the widget and its representation use to
  /// communicate with each other.
  void UpdateFromMRML(vtkMRMLNode* caller, unsigned long event, void *callData=nullptr) override;

  /// Updates the dimensions of the cube source filter
  virtual void UpdateCubeSourceFromMRML(vtkMRMLMarkupsROINode* roiNode);

  /// Methods to make this class behave as a vtkProp.
  void GetActors(vtkPropCollection *) override;
  void ReleaseGraphicsResources(vtkWindow *) override;
  int RenderOverlay(vtkViewport *viewport) override;
  int RenderOpaqueGeometry(vtkViewport *viewport) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport *viewport) override;
  vtkTypeBool HasTranslucentPolygonalGeometry() override;

  /// Return the bounds of the representation
  double *GetBounds() override;

  void CanInteract(vtkMRMLInteractionEventData* interactionEventData,
    int& foundComponentType, int& foundComponentIndex, double& closestDistance2) override;

  void CanInteractWithROI(vtkMRMLInteractionEventData* interactionEventData,
    int& foundComponentType, int& foundComponentIndex, double& closestDistance2);

protected:
  vtkCjyxROIRepresentation3D();
  ~vtkCjyxROIRepresentation3D() override;

  // Initialize interaction handle pipeline
  void SetupInteractionPipeline() override;

  // Update visibility of interaction handles for representation
  void UpdateInteractionPipeline() override;

  void SetROISource(vtkPolyDataAlgorithm* roiSource);

  vtkSmartPointer<vtkPolyDataAlgorithm> ROISource;

  vtkSmartPointer<vtkPassThroughFilter> ROIPipelineInputFilter;

  vtkSmartPointer<vtkTransformPolyDataFilter>    ROITransformFilter;
  vtkSmartPointer<vtkTransform>                  ROIToWorldTransform;

  vtkSmartPointer<vtkPolyDataMapper>             ROIMapper;
  vtkSmartPointer<vtkProperty>                   ROIProperty;
  vtkSmartPointer<vtkActor>                      ROIActor;

  vtkSmartPointer<vtkPolyDataMapper>             ROIOccludedMapper;
  vtkSmartPointer<vtkProperty>                   ROIOccludedProperty;
  vtkSmartPointer<vtkActor>                      ROIOccludedActor;

  vtkSmartPointer<vtkOutlineFilter>              ROIOutlineFilter;

  vtkSmartPointer<vtkTransformPolyDataFilter>    ROIOutlineTransformFilter;

  vtkSmartPointer<vtkPolyDataMapper>             ROIOutlineMapper;
  vtkSmartPointer<vtkProperty>                   ROIOutlineProperty;
  vtkSmartPointer<vtkActor>                      ROIOutlineActor;

  vtkSmartPointer<vtkPolyDataMapper>             ROIOutlineOccludedMapper;
  vtkSmartPointer<vtkProperty>                   ROIOutlineOccludedProperty;
  vtkSmartPointer<vtkActor>                      ROIOutlineOccludedActor;

  class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT MarkupsInteractionPipelineROI : public MarkupsInteractionPipeline
  {
  public:
    MarkupsInteractionPipelineROI(vtkCjyxMarkupsWidgetRepresentation* representation);
    ~MarkupsInteractionPipelineROI() override = default;

    // Initialize scale handles
    void CreateScaleHandles() override;

    // Update scale handle positions
    virtual void UpdateScaleHandles();

    // Update scale handle visibilities
    void UpdateHandleVisibility() override;

    // Get handle opacity
    double GetHandleOpacity(int type, int index) override;


    void GetHandleColor(int type, int index, double color[4]) override;
    HandleInfoList GetHandleInfoList() override;
    void GetInteractionHandleAxisWorld(int type, int index, double axis[3]) override;
  };
  friend class vtkCjyxROIRepresentation2D;

private:
  vtkCjyxROIRepresentation3D(const vtkCjyxROIRepresentation3D&) = delete;
  void operator=(const vtkCjyxROIRepresentation3D&) = delete;
};

#endif
