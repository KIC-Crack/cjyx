/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

=========================================================================auto=*/
///  vtkCjyxModuleLogic - superclass for cjyx module logic classes
///
/// Superclass for all cjyx module logic classes
/// \note No GUI code goes in the logic class.

#ifndef __vtkCjyxModuleLogic_h
#define __vtkCjyxModuleLogic_h

// CjyxLogic includes
#include "vtkCjyxApplicationLogic.h"

// MRMLLogic includes
#include <vtkMRMLAbstractLogic.h>

class VTK_CJYX_BASE_LOGIC_EXPORT vtkCjyxModuleLogic
  : public vtkMRMLAbstractLogic
{
public:
  /// The Usual vtk class functions
  static vtkCjyxModuleLogic *New();
  vtkTypeMacro(vtkCjyxModuleLogic, vtkMRMLAbstractLogic);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /// Get access to overall application state
  virtual vtkCjyxApplicationLogic* GetApplicationLogic();
  //TODO virtual void SetApplicationLogic(vtkCjyxApplicationLogic* logic);

  /// Convenience method for getting another module's logic from the application logic.
  virtual vtkMRMLAbstractLogic* GetModuleLogic(const char* moduleName);

  std::string GetModuleShareDirectory()const;
  void SetModuleShareDirectory(const std::string& shareDirectory);

protected:

  vtkCjyxModuleLogic();
  ~vtkCjyxModuleLogic() override;

private:

  vtkCjyxModuleLogic(const vtkCjyxModuleLogic&) = delete;
  void operator=(const vtkCjyxModuleLogic&) = delete;

  std::string ModuleShareDirectory;
};

#endif

