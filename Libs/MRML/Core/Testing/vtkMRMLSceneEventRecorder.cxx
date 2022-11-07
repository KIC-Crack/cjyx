/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// MRML includes
#include "vtkMRMLScene.h"
#include "vtkMRMLSceneEventRecorder.h"

//---------------------------------------------------------------------------
vtkMRMLSceneEventRecorder *vtkMRMLSceneEventRecorder ::New()
{
  return new vtkMRMLSceneEventRecorder;
}

//---------------------------------------------------------------------------
vtkMRMLSceneEventRecorder::vtkMRMLSceneEventRecorder() = default;

//---------------------------------------------------------------------------
vtkMRMLSceneEventRecorder::~vtkMRMLSceneEventRecorder() = default;

//---------------------------------------------------------------------------
void vtkMRMLSceneEventRecorder::Execute(
  vtkObject *vtkcaller, unsigned long eid, void *vtkNotUsed(calldata))
{
  if (vtkMRMLScene::SafeDownCast(vtkcaller) == nullptr)
    {
    return;
    }
  if (eid == vtkCommand::ModifiedEvent)
    {
    return;
    }
  ++this->CalledEvents[eid];
  this->LastEventMTime[eid] = vtkTimeStamp();
}