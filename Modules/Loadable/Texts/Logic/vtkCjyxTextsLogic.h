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
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

///  vtkCjyxTextsLogic - cjyx logic class for volumes manipulation
///
/// This class manages the logic associated with reading, saving,
/// and changing propertied of the volumes

#ifndef __vtkCjyxTextsLogic_h
#define __vtkCjyxTextsLogic_h

// CjyxLogic includes
#include "vtkCjyxBaseLogic.h"
#include "vtkCjyxTextsModuleLogicExport.h"

// MRMLLogic includes
#include <vtkMRMLAbstractLogic.h>

class VTK_CJYX_TEXTS_MODULE_LOGIC_EXPORT vtkCjyxTextsLogic : public vtkMRMLAbstractLogic
{
  public:

  /// The Usual vtk class functions
  static vtkCjyxTextsLogic *New();
  vtkTypeMacro(vtkCjyxTextsLogic,vtkMRMLAbstractLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override { Superclass::PrintSelf(os, indent); }

protected:
  vtkCjyxTextsLogic();
  ~vtkCjyxTextsLogic() override;
  vtkCjyxTextsLogic(const vtkCjyxTextsLogic&);
  void operator=(const vtkCjyxTextsLogic&);
};

#endif
