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

#ifndef __vtkCjyxTemplateKeyMarkupslogic_h_
#define __vtkCjyxTemplateKeyMarkupslogic_h_

#include <vtkCjyxMarkupsLogic.h>

#include "vtkCjyxTemplateKeyModuleLogicExport.h"

class VTK_CJYX_TEMPLATEKEY_MODULE_LOGIC_EXPORT vtkCjyxTemplateKeyLogic:
  public vtkCjyxMarkupsLogic
{
public:
  static vtkCjyxTemplateKeyLogic* New();
  vtkTypeMacro(vtkCjyxTemplateKeyLogic, vtkCjyxMarkupsLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkCjyxTemplateKeyLogic();
  ~vtkCjyxTemplateKeyLogic() override;

  void RegisterNodes() override;

private:
  vtkCjyxTemplateKeyLogic(const vtkCjyxTemplateKeyLogic&) = delete;
  void operator=(const vtkCjyxTemplateKeyLogic&) = delete;
};

#endif // __vtkCjyxTemplateKeyMarkupslogic_h_
