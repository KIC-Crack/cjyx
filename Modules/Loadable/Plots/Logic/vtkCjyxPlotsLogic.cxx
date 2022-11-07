/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright 2015 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Andras Lasso (PerkLab, Queen's
  University) and Kevin Wang (Princess Margaret Hospital, Toronto) and was
  supported through OCAIRO and the Applied Cancer Research Unit program of
  Cancer Care Ontario.

==============================================================================*/

// Plots Logic includes
#include "vtkCjyxPlotsLogic.h"

// MRML includes
#include <vtkMRMLPlotChartNode.h>
#include <vtkMRMLLayoutNode.h>
#include <vtkMRMLPlotSeriesNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>

// VTK includes
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

// STD includes

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkCjyxPlotsLogic);

//----------------------------------------------------------------------------
vtkCjyxPlotsLogic::vtkCjyxPlotsLogic() = default;

//----------------------------------------------------------------------------
vtkCjyxPlotsLogic::~vtkCjyxPlotsLogic() = default;

//----------------------------------------------------------------------------
void vtkCjyxPlotsLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
int vtkCjyxPlotsLogic::GetLayoutWithPlot(int currentLayout)
{
  switch (currentLayout)
    {
    case vtkMRMLLayoutNode::CjyxLayoutFourUpPlotView:
    case vtkMRMLLayoutNode::CjyxLayoutFourUpPlotTableView:
    case vtkMRMLLayoutNode::CjyxLayoutOneUpPlotView:
    case vtkMRMLLayoutNode::CjyxLayoutConventionalPlotView:
    case vtkMRMLLayoutNode::CjyxLayoutThreeOverThreePlotView:
      // plot already shown, no need to change
      return currentLayout;
    case vtkMRMLLayoutNode::CjyxLayoutFourUpTableView:
      return vtkMRMLLayoutNode::CjyxLayoutFourUpPlotTableView;
    case vtkMRMLLayoutNode::CjyxLayoutConventionalView:
      return vtkMRMLLayoutNode::CjyxLayoutConventionalPlotView;
    default:
      return vtkMRMLLayoutNode::CjyxLayoutFourUpPlotView;
    }
}

// --------------------------------------------------------------------------
vtkMRMLPlotSeriesNode* vtkCjyxPlotsLogic::CloneSeries(vtkMRMLPlotSeriesNode* source, const char * vtkNotUsed(name))
{
  if (!source || source->GetScene() == nullptr)
    {
    vtkErrorMacro("vtkCjyxPlotsLogic::CloneSeries failed: source is nullptr or not added to a a scene");
    return nullptr;
    }

  vtkSmartPointer<vtkMRMLNode> clonedNode = vtkSmartPointer<vtkMRMLNode>::Take(
    source->GetScene()->CreateNodeByClass("vtkMRMLPlotSeriesNode"));
  vtkMRMLPlotSeriesNode *clonedSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(clonedNode);
  clonedSeriesNode->CopyWithScene(source);
  std::string nodeName(source->GetName() ? source->GetName() : "");
  nodeName += "_Copy";

  clonedSeriesNode->SetName(source->GetScene()->GetUniqueNameByString(nodeName.c_str()));
  source->GetScene()->AddNode(clonedSeriesNode);
  return clonedSeriesNode;
}

// --------------------------------------------------------------------------
void vtkCjyxPlotsLogic::ShowChartInLayout(vtkMRMLPlotChartNode* chartNode)
{
  // Switch to a layout that contains plot
  vtkMRMLLayoutNode* layoutNode = vtkMRMLLayoutNode::SafeDownCast(this->GetMRMLScene()->GetFirstNodeByClass("vtkMRMLLayoutNode"));
  if (layoutNode)
    {
    int currentLayout = layoutNode->GetViewArrangement();
    int layoutWithPlot = vtkCjyxPlotsLogic::GetLayoutWithPlot(currentLayout);
    if (currentLayout != layoutWithPlot)
      {
      layoutNode->SetViewArrangement(layoutWithPlot);
      }
    }

  // Show plot in viewers
  vtkCjyxApplicationLogic* appLogic = this->GetApplicationLogic();
  if (appLogic)
    {
    vtkMRMLSelectionNode* selectionNode = appLogic->GetSelectionNode();
    if (selectionNode)
      {
      const char* chartNodeID = (chartNode ? chartNode->GetID() : nullptr);
      selectionNode->SetActivePlotChartID(chartNodeID);
      }
    appLogic->PropagatePlotChartSelection();
    }
}

// --------------------------------------------------------------------------
vtkMRMLPlotChartNode* vtkCjyxPlotsLogic::GetFirstPlotChartForSeries(vtkMRMLPlotSeriesNode* seriesNode)
{
  if (seriesNode == nullptr)
    {
    return nullptr;
    }
  std::vector<vtkMRMLNode*> chartNodes;
  unsigned int numberOfNodes = this->GetMRMLScene()->GetNodesByClass("vtkMRMLPlotChartNode", chartNodes);
  for (unsigned int chartNodeIndex=0; chartNodeIndex<numberOfNodes; chartNodeIndex++)
    {
    vtkMRMLPlotChartNode* chartNode = vtkMRMLPlotChartNode::SafeDownCast(chartNodes[chartNodeIndex]);
    if (!chartNode)
      {
      continue;
      }
    if (chartNode->HasPlotSeriesNodeID(seriesNode->GetID()))
      {
      return chartNode;
      }
    }
  // not found
  return nullptr;
}
