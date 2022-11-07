/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __vtkCjyxTerminologyCategory_h
#define __vtkCjyxTerminologyCategory_h

// MRML includes
#include "vtkCodedEntry.h"

#include "vtkCjyxTerminologiesModuleLogicExport.h"

/// \brief Terminology property category object
///
/// Encapsulates the mandatory and optional fields for a terminology category.
/// The following fields inherited from \sa vtkCodedEntry:
///   CodingSchemeDesignator: 'codingScheme' member of the category object. Value example "SCT"
///   CodeValue: 'codeValue' member of the category object. Value example "85756007"
///   CodeMeaning: 'codeMeaning' member of the category object. Value example "Tissue"
///
class VTK_CJYX_TERMINOLOGIES_LOGIC_EXPORT vtkCjyxTerminologyCategory : public vtkCodedEntry
{
public:
  static vtkCjyxTerminologyCategory *New();
  vtkTypeMacro(vtkCjyxTerminologyCategory, vtkCodedEntry);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Reset state of object
  void Initialize() override;

  /// Copy one category into another
  void Copy(vtkCodedEntry* aCategory) override;

public:
  vtkGetStringMacro(SNOMEDCTConceptID);
  vtkSetStringMacro(SNOMEDCTConceptID);
  vtkGetStringMacro(UMLSConceptUID);
  vtkSetStringMacro(UMLSConceptUID);
  vtkGetStringMacro(Cid);
  vtkSetStringMacro(Cid);
  vtkGetStringMacro(ContextGroupName);
  vtkSetStringMacro(ContextGroupName);

  vtkGetMacro(ShowAnatomy, bool);
  vtkSetMacro(ShowAnatomy, bool);
  vtkBooleanMacro(ShowAnatomy, bool);

protected:
  vtkCjyxTerminologyCategory();
  ~vtkCjyxTerminologyCategory() override;
  vtkCjyxTerminologyCategory(const vtkCjyxTerminologyCategory&);
  void operator=(const vtkCjyxTerminologyCategory&);

protected:
  /// 'SNOMEDCTConceptID' member of the category object. Value example "85756007"
  char* SNOMEDCTConceptID;
  /// 'UMLSConceptUID' member of the category object. Value example "C0040300"
  char* UMLSConceptUID;
  /// 'cid' member of the category object. Value example "7051"
  char* Cid;
  /// 'contextGroupName' member of the category object. Value example "Segmentation Property Categories"
  char* ContextGroupName;

  /// 'showAnatomy' member of the category object
  bool ShowAnatomy;
};

#endif
