/*==============================================================================

  Copyright (c) The Intervention Centre
  Oslo University Hospital, Oslo, Norway. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Rafael Palomar (The Intervention Centre,
  Oslo University Hospital) and was supported by The Research Council of Norway
  through the ALive project (grant nr. 311393).

==============================================================================*/

#ifndef __vtkcjyxtestlinerepresentation3d_h_
#define __vtkcjyxtestlinerepresentation3d_h_

#include "vtkCjyxTemplateKeyModuleVTKWidgetsExport.h"

// Markups VTKWidgets includes
#include "vtkCjyxLineRepresentation3D.h"

// VTK includes
#include <vtkWeakPointer.h>

//------------------------------------------------------------------------------
class vtkCutter;
class vtkPlane;

/**
 * @class   vtkCjyxTestLineRepresentation3D
 * @brief   Default representation for the line widget
 *
 * This class provides the default concrete representation for the
 * vtkMRMLAbstractWidget. See vtkMRMLAbstractWidget
 * for details.
 * @sa
 * vtkCjyxMarkupsWidgetRepresentation2D vtkMRMLAbstractWidget
*/

class VTK_CJYX_TEMPLATEKEY_MODULE_VTKWIDGETS_EXPORT vtkCjyxTestLineRepresentation3D
: public vtkCjyxLineRepresentation3D
{
public:
  static vtkCjyxTestLineRepresentation3D* New();
  vtkTypeMacro(vtkCjyxTestLineRepresentation3D, vtkCjyxLineRepresentation3D);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void UpdateFromMRML(vtkMRMLNode* caller, unsigned long event, void* callData=nullptr) override;

  /// Methods to make this class behave as a vtkProp.
  void GetActors(vtkPropCollection*) override;
  void ReleaseGraphicsResources(vtkWindow*) override;
  int RenderOverlay(vtkViewport* viewport) override;
  int RenderOpaqueGeometry(vtkViewport* viewport) override;
  int RenderTranslucentPolygonalGeometry(vtkViewport* viewport) override;
  vtkTypeBool HasTranslucentPolygonalGeometry() override;

protected:
  vtkCjyxTestLineRepresentation3D();
  ~vtkCjyxTestLineRepresentation3D() override;

  vtkSmartPointer<vtkCutter> Cutter;
  vtkSmartPointer<vtkPolyDataMapper> ContourMapper;
  vtkSmartPointer<vtkActor> ContourActor;
  vtkSmartPointer<vtkPolyData> MiddlePoint;
  vtkSmartPointer<vtkPolyDataMapper> MiddlePointMapper;
  vtkSmartPointer<vtkActor> MiddlePointActor;
  vtkWeakPointer<vtkPolyData> TargetOrgan;
  vtkSmartPointer<vtkSphereSource> MiddlePointSource;
  vtkSmartPointer<vtkPlane> SlicingPlane;

  void BuildMiddlePoint();
  void BuildSlicingPlane();

private:
  vtkCjyxTestLineRepresentation3D(const vtkCjyxTestLineRepresentation3D&) = delete;
  void operator=(const vtkCjyxTestLineRepresentation3D&) = delete;
};

#endif // __vtkcjyxtestlinerepresentation3d_h_
