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

#ifndef __vtkcjyxslicingcontourwidget_h_
#define __vtkcjyxslicingcontourwidget_h_

#include "vtkCjyxTemplateKeyModuleVTKWidgetsExport.h"

#include <vtkCjyxLineWidget.h>

class VTK_CJYX_TEMPLATEKEY_MODULE_VTKWIDGETS_EXPORT vtkCjyxTestLineWidget
: public vtkCjyxLineWidget
{
public:
  static vtkCjyxTestLineWidget *New();
  vtkTypeMacro(vtkCjyxTestLineWidget, vtkCjyxLineWidget);

  void CreateDefaultRepresentation(vtkMRMLMarkupsDisplayNode* markupsDisplayNode,
                                  vtkMRMLAbstractViewNode* viewNode,
                                  vtkRenderer* renderer) override;

  /// Create instance of the markups widget
  virtual vtkCjyxMarkupsWidget* CreateInstance() const override;

protected:
  vtkCjyxTestLineWidget();
  ~vtkCjyxTestLineWidget() override;

private:
  vtkCjyxTestLineWidget(const vtkCjyxTestLineWidget&) = delete;
  void operator=(const vtkCjyxTestLineWidget) = delete;
};

#endif // __vtkcjyxslicingcontourwidget_h_
