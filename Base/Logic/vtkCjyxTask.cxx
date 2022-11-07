#include "vtkCjyxTask.h"

// VTK includes
#include <vtkObjectFactory.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxTask);

//----------------------------------------------------------------------------
vtkCjyxTask::vtkCjyxTask()
{
  this->TaskObject = nullptr;
  this->TaskFunction = nullptr;
  this->TaskClientData = nullptr;
  this->Type = vtkCjyxTask::Undefined;
}
//----------------------------------------------------------------------------
vtkCjyxTask::~vtkCjyxTask() = default;

//----------------------------------------------------------------------------
void vtkCjyxTask::SetTaskFunction(vtkMRMLAbstractLogic *object,
                                    vtkMRMLAbstractLogic::TaskFunctionPointer function,
                                    void *clientdata)
{
  this->TaskObject = object;
  this->TaskFunction = function;
  this->TaskClientData = clientdata;
}

//----------------------------------------------------------------------------
void vtkCjyxTask::Execute()
{
  if (this->TaskObject)
    {
    ((*this->TaskObject).*(this->TaskFunction))(this->TaskClientData);
    }
}

//----------------------------------------------------------------------------
void vtkCjyxTask::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os, indent);
}
