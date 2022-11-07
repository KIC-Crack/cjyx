/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

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

// SliceViewControllers Logic includes
#include "vtkCjyxViewControllersLogic.h"

// MRML includes
#include <vtkMRMLPlotViewNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLViewNode.h>

// VTK includes
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxViewControllersLogic);

//----------------------------------------------------------------------------
vtkCjyxViewControllersLogic::vtkCjyxViewControllersLogic() = default;

//----------------------------------------------------------------------------
vtkCjyxViewControllersLogic::~vtkCjyxViewControllersLogic() = default;

//----------------------------------------------------------------------------
void vtkCjyxViewControllersLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
 }

//-----------------------------------------------------------------------------
void vtkCjyxViewControllersLogic::RegisterNodes()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("vtkCjyxViewControllersLogic::RegisterNodes failed: invalid scene");
    return;
    }
}

//-----------------------------------------------------------------------------
vtkMRMLSliceNode* vtkCjyxViewControllersLogic::GetDefaultSliceViewNode()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("vtkCjyxViewControllersLogic::GetDefaultSliceViewNode failed: invalid scene");
    return nullptr;
    }
  vtkMRMLNode* defaultNode = scene->GetDefaultNodeByClass("vtkMRMLSliceNode");
  if (!defaultNode)
    {
    defaultNode = scene->CreateNodeByClass("vtkMRMLSliceNode");
    scene->AddDefaultNode(defaultNode);
    defaultNode->Delete(); // scene owns it now
    }
  return vtkMRMLSliceNode::SafeDownCast(defaultNode);
}

//-----------------------------------------------------------------------------
vtkMRMLViewNode* vtkCjyxViewControllersLogic::GetDefaultThreeDViewNode()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("vtkCjyxViewControllersLogic::GetDefaultThreeDViewNode failed: invalid scene");
    return nullptr;
    }
  vtkMRMLNode* defaultNode = scene->GetDefaultNodeByClass("vtkMRMLViewNode");
  if (!defaultNode)
    {
    defaultNode = scene->CreateNodeByClass("vtkMRMLViewNode");
    scene->AddDefaultNode(defaultNode);
    defaultNode->Delete(); // scene owns it now
    }
  return vtkMRMLViewNode::SafeDownCast(defaultNode);
}

//-----------------------------------------------------------------------------
vtkMRMLPlotViewNode *vtkCjyxViewControllersLogic::GetDefaultPlotViewNode()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("vtkCjyxViewControllersLogic::GetDefaultPlotViewNode failed: invalid scene");
    return nullptr;
    }
  vtkMRMLNode* defaultNode = scene->GetDefaultNodeByClass("vtkMRMLPlotViewNode");
  if (!defaultNode)
    {
    defaultNode = scene->CreateNodeByClass("vtkMRMLPlotViewNode");
    scene->AddDefaultNode(defaultNode);
    defaultNode->Delete(); // scene owns it now
    }
  return vtkMRMLPlotViewNode::SafeDownCast(defaultNode);
}

//-----------------------------------------------------------------------------
void vtkCjyxViewControllersLogic::ResetAllViewNodesToDefault()
{
  vtkMRMLScene *scene = this->GetMRMLScene();
  if (!scene)
    {
    vtkErrorMacro("vtkCjyxViewControllersLogic::ResetAllViewNodesToDefault failed: invalid scene");
    return;
    }
  scene->StartState(vtkMRMLScene::BatchProcessState);
  vtkMRMLSliceNode* defaultSliceViewNode = GetDefaultSliceViewNode();
  std::vector< vtkMRMLNode* > viewNodes;
  scene->GetNodesByClass("vtkMRMLSliceNode", viewNodes);
  for (std::vector< vtkMRMLNode* >::iterator it = viewNodes.begin(); it != viewNodes.end(); ++it)
    {
    vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(*it);
    if (!sliceNode)
      {
      continue;
      }
    sliceNode->Reset(defaultSliceViewNode);
    sliceNode->SetOrientationToDefault();
    }
  viewNodes.clear();
  vtkMRMLViewNode* defaultThreeDViewNode = GetDefaultThreeDViewNode();
  scene->GetNodesByClass("vtkMRMLViewNode", viewNodes);
  for (std::vector< vtkMRMLNode* >::iterator it = viewNodes.begin(); it != viewNodes.end(); ++it)
    {
    (*it)->Reset(defaultThreeDViewNode);
    }
  viewNodes.clear();
  vtkMRMLPlotViewNode* defaultPlotViewNode = GetDefaultPlotViewNode();
  scene->GetNodesByClass("vtkMRMLPlotViewNode", viewNodes);
  for (std::vector< vtkMRMLNode* >::iterator it = viewNodes.begin(); it != viewNodes.end(); ++it)
    {
    (*it)->Reset(defaultPlotViewNode);
    }
  scene->EndState(vtkMRMLScene::BatchProcessState);
}
