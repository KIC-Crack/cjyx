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
 * @class   vtkCjyxPlaneWidget
 * @brief   create a plane with a set of 3 points
 *
 * The vtkCjyxPlaneWidget is used to create a plane widget with a set of 3 points.
 *
*/

#ifndef vtkCjyxPlaneWidget_h
#define vtkCjyxPlaneWidget_h

#include "vtkCjyxMarkupsModuleVTKWidgetsExport.h"
#include "vtkCjyxMarkupsWidget.h"

class vtkCjyxMarkupsWidgetRepresentation;
class vtkPolyData;
class vtkIdList;

class VTK_CJYX_MARKUPS_MODULE_VTKWIDGETS_EXPORT vtkCjyxPlaneWidget : public vtkCjyxMarkupsWidget
{
public:
  /// Instantiate this class.
  static vtkCjyxPlaneWidget *New();

  /// Standard methods for a VTK class.
  vtkTypeMacro(vtkCjyxPlaneWidget,vtkCjyxMarkupsWidget);

  /// Create instance of the markups widget
  vtkCjyxMarkupsWidgetCreateInstanceMacro(vtkCjyxPlaneWidget);

  /// Widget states
  enum
  {
    WidgetStateTranslatePlane = WidgetStateMarkups_Last, // translating the plane
    WidgetStateSymmetricScale, // Scaling the plane without moving the center
    WidgetStateMarkupsPlane_Last
  };

  /// Widget events
  enum
  {
    WidgetEventControlPointPlace = WidgetEventMarkups_Last,
    WidgetEventControlPointPlacePlaneNormal,
    WidgetEventPlaneMoveStart,
    WidgetEventPlaneMoveEnd,
    WidgetEventSymmetricScaleStart,
    WidgetEventSymmetricScaleEnd,
    WidgetEventMarkupsPlane_Last
  };

  /// Create the default widget representation and initializes the widget and representation.
  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode, vtkMRMLAbstractViewNode* viewNode, vtkRenderer* renderer) override;

  bool PlacePoint(vtkMRMLInteractionEventData* eventData) override;
  virtual bool PlacePlaneNormal(vtkMRMLInteractionEventData* eventData);

  bool CanProcessInteractionEvent(vtkMRMLInteractionEventData* eventData, double& distance2) override;
  bool ProcessInteractionEvent(vtkMRMLInteractionEventData* eventData) override;
  bool ProcessUpdatePlaneFromViewNormal(vtkMRMLInteractionEventData* event);
  bool ProcessPlaneMoveStart(vtkMRMLInteractionEventData* event);
  bool ProcessPlaneMoveEnd(vtkMRMLInteractionEventData* event);
  bool ProcessMouseMove(vtkMRMLInteractionEventData* eventData) override;
  bool ProcessPlaneTranslate(vtkMRMLInteractionEventData* event);
  bool ProcessWidgetSymmetricScaleStart(vtkMRMLInteractionEventData* eventData);
  bool ProcessPlaneSymmetricScale(vtkMRMLInteractionEventData* event);
  bool ProcessEndMouseDrag(vtkMRMLInteractionEventData* eventData) override;
  bool ProcessWidgetStopPlace(vtkMRMLInteractionEventData* eventData) override;

protected:
  vtkCjyxPlaneWidget();
  ~vtkCjyxPlaneWidget() override;

  void ScaleWidget(double eventPos[2]) override;
  virtual void ScaleWidget(double eventPos[2], bool symmetricScale);
  void RotateWidget(double eventPos[2]) override;

  /// Flip the selected index across the specified axis.
  /// Ex. Switch between L--R face.
  /// Used when the user drags an ROI handle across the ROI origin.
  void FlipPlaneHandles(bool flipLRHandle, bool flipAPHandle);

private:
  vtkCjyxPlaneWidget(const vtkCjyxPlaneWidget&) = delete;
  void operator=(const vtkCjyxPlaneWidget&) = delete;
};

#endif
