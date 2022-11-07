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

#ifndef __vtkCjyxTerminologyEntry_h
#define __vtkCjyxTerminologyEntry_h

// VTK includes
#include <vtkObject.h>

// Terminology includes
#include "vtkCjyxTerminologiesModuleLogicExport.h"

#include "vtkCjyxTerminologyCategory.h"
#include "vtkCjyxTerminologyType.h"

/// VTK implementation of \sa qCjyxDICOMLoadable
class VTK_CJYX_TERMINOLOGIES_LOGIC_EXPORT vtkCjyxTerminologyEntry : public vtkObject
{
public:
  static vtkCjyxTerminologyEntry *New();
  vtkTypeMacro(vtkCjyxTerminologyEntry, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Copy one entry into another
  virtual void Copy(vtkCjyxTerminologyEntry* aEntry);

public:
  vtkGetStringMacro(TerminologyContextName);
  vtkSetStringMacro(TerminologyContextName);

  vtkGetObjectMacro(CategoryObject, vtkCjyxTerminologyCategory);
  vtkGetObjectMacro(TypeObject, vtkCjyxTerminologyType);
  vtkGetObjectMacro(TypeModifierObject, vtkCjyxTerminologyType);

  vtkGetStringMacro(AnatomicContextName);
  vtkSetStringMacro(AnatomicContextName);

  vtkGetObjectMacro(AnatomicRegionObject, vtkCjyxTerminologyType);
  vtkGetObjectMacro(AnatomicRegionModifierObject, vtkCjyxTerminologyType);

protected:
  vtkSetObjectMacro(CategoryObject, vtkCjyxTerminologyCategory);
  vtkSetObjectMacro(TypeObject, vtkCjyxTerminologyType);
  vtkSetObjectMacro(TypeModifierObject, vtkCjyxTerminologyType);

  vtkSetObjectMacro(AnatomicRegionObject, vtkCjyxTerminologyType);
  vtkSetObjectMacro(AnatomicRegionModifierObject, vtkCjyxTerminologyType);

protected:
  vtkCjyxTerminologyEntry();
  ~vtkCjyxTerminologyEntry() override;
  vtkCjyxTerminologyEntry(const vtkCjyxTerminologyEntry&);
  void operator=(const vtkCjyxTerminologyEntry&);

protected:
  /// Terminology context name (SegmentationCategoryTypeContextName in terminology Json)
  char* TerminologyContextName;
  /// Category properties
  vtkCjyxTerminologyCategory* CategoryObject;
  /// Type properties
  vtkCjyxTerminologyType* TypeObject;
  /// Type modifier properties
  vtkCjyxTerminologyType* TypeModifierObject;

  /// Anatomic context name (AnatomicContextName in anatomy Json) - optional
  char* AnatomicContextName;
  /// Anatomical region properties
  vtkCjyxTerminologyType* AnatomicRegionObject;
  /// Anatomical region modifier properties
  vtkCjyxTerminologyType* AnatomicRegionModifierObject;
};

#endif
