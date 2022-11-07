/*==============================================================================

  Copyright (c) Kapteyn Astronomical Institute
  University of Groningen, Groningen, Netherlands. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Davide Punzo, Kapteyn Astronomical Institute,
  and was supported through the European Research Council grant nr. 291531.

==============================================================================*/

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyPlotsPlugin.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLPlotChartNode.h>
#include <vtkMRMLLayoutNode.h>
#include <vtkMRMLPlotSeriesNode.h>
#include <vtkMRMLPlotViewNode.h>

#include <vtkCjyxPlotsLogic.h>
#include <qMRMLPlotWidget.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxLayoutManager.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyPlotsPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyPlotsPlugin);
protected:
  qCjyxSubjectHierarchyPlotsPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyPlotsPluginPrivate(qCjyxSubjectHierarchyPlotsPlugin& object);
  ~qCjyxSubjectHierarchyPlotsPluginPrivate() override;
  void init();
public:
  QIcon PlotChartIcon;
  QIcon PlotSeriesIcon;

  vtkWeakPointer<vtkCjyxPlotsLogic> PlotsLogic;

  QIcon VisibleIcon;
  QIcon HiddenIcon;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyPlotsPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPlotsPluginPrivate::qCjyxSubjectHierarchyPlotsPluginPrivate(qCjyxSubjectHierarchyPlotsPlugin& object)
: q_ptr(&object)
{
  this->PlotChartIcon = QIcon(":Icons/Medium/CjyxInteractivePlotting.png");
  this->PlotSeriesIcon = QIcon(":Icons/Medium/CjyxPlotSeries.png");

  this->VisibleIcon = QIcon(":Icons/VisibleOn.png");
  this->HiddenIcon = QIcon(":Icons/VisibleOff.png");
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyPlotsPluginPrivate::init()
{
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPlotsPluginPrivate::~qCjyxSubjectHierarchyPlotsPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyPlotsPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPlotsPlugin::qCjyxSubjectHierarchyPlotsPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyPlotsPluginPrivate(*this) )
{
  Q_D(qCjyxSubjectHierarchyPlotsPlugin);
  this->m_Name = QString("Plots");
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPlotsPlugin::~qCjyxSubjectHierarchyPlotsPlugin() = default;

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPlotsPlugin::setPlotsLogic(vtkCjyxPlotsLogic* plotsLogic)
{
  Q_D(qCjyxSubjectHierarchyPlotsPlugin);
  d->PlotsLogic = plotsLogic;
}

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyPlotsPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is nullptr!";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLPlotChartNode"))
    {
    return 0.5;
    }
  else if (node->IsA("vtkMRMLPlotSeriesNode"))
    {
    return 0.5;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyPlotsPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLPlotChartNode"))
    {
    return 0.5; // There may be other plugins that can handle special PlotChart better
    }
  else if (associatedNode && associatedNode->IsA("vtkMRMLPlotSeriesNode"))
    {
    return 0.5; // There may be other plugins that can handle special PlotSeries better
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyPlotsPlugin::roleForPlugin()const
{
  return "Plot";
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyPlotsPlugin::icon(vtkIdType itemID)
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  Q_D(qCjyxSubjectHierarchyPlotsPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QIcon();
    }

  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLPlotChartNode"))
    {
    return d->PlotChartIcon;
    }
  else if (associatedNode && associatedNode->IsA("vtkMRMLPlotSeriesNode"))
    {
    return d->PlotSeriesIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyPlotsPlugin::visibilityIcon(int visible)
{
  Q_D(qCjyxSubjectHierarchyPlotsPlugin);

  if (visible)
    {
    return d->VisibleIcon;
    }
  else
    {
    return d->HiddenIcon;
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyPlotsPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  Q_D(qCjyxSubjectHierarchyPlotsPlugin);

  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene!";
    return;
    }

  if (this->getDisplayVisibility(itemID) == visible)
    {
    return;
    }

  vtkMRMLPlotChartNode* plotChartNode = vtkMRMLPlotChartNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  vtkMRMLPlotSeriesNode* plotSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(shNode->GetItemDataNode(itemID));

  if (!d->PlotsLogic)
    {
    qWarning() << Q_FUNC_INFO << ": plotsLogic is not set, cannot show plot in layout";
    return;
    }

  if (plotSeriesNode)
    {
    // A plot series node is selected.
    if (visible)
      {
      // Show series in current chart (if there is one), otherwise create a new chart
      vtkMRMLPlotChartNode* plotChartNode = nullptr;
      vtkMRMLPlotViewNode* plotViewNode = this->getPlotViewNode();
      if (plotViewNode)
        {
        plotChartNode = plotViewNode->GetPlotChartNode();
        }
      if (!plotChartNode)
        {
        plotChartNode = vtkMRMLPlotChartNode::SafeDownCast(scene->AddNewNodeByClass("vtkMRMLPlotChartNode"));
        }
      d->PlotsLogic->ShowChartInLayout(plotChartNode);
      if (!plotChartNode->HasPlotSeriesNodeID(plotSeriesNode->GetID()))
        {
        plotChartNode->AddAndObservePlotSeriesNodeID(plotSeriesNode->GetID());
        }
      }
    else
      {
      // Show series in current chart (if there is one), otherwise create a new chart
      vtkMRMLPlotViewNode* plotViewNode = this->getPlotViewNode();
      if (!plotViewNode)
        {
        // already hidden
        return;
        }
      vtkMRMLPlotChartNode* plotChartNode = plotViewNode->GetPlotChartNode();;
      if (!plotChartNode)
        {
        // already hidden
        return;
        }
      if (plotChartNode->HasPlotSeriesNodeID(plotSeriesNode->GetID()))
        {
        plotChartNode->RemovePlotSeriesNodeID(plotSeriesNode->GetID());
        }
      }
    }
  else if (plotChartNode)
    {
    vtkMRMLPlotChartNode* associatedPlotChartNode = vtkMRMLPlotChartNode::SafeDownCast(shNode->GetItemDataNode(itemID));
    d->PlotsLogic->ShowChartInLayout(visible ? associatedPlotChartNode : nullptr);
    }

  // Update icons of all charts (if we show this chart then we may have hidden other charts)
  if (scene->IsBatchProcessing())
    {
    return;
    }
  std::vector< vtkMRMLNode* > chartNodes;
  scene->GetNodesByClass("vtkMRMLPlotChartNode", chartNodes);
  for (std::vector< vtkMRMLNode* >::iterator chartIt = chartNodes.begin(); chartIt != chartNodes.end(); ++chartIt)
    {
    vtkMRMLPlotChartNode* chartNode = vtkMRMLPlotChartNode::SafeDownCast(*chartIt);
    vtkIdType chartNodeId = shNode->GetItemByDataNode(chartNode);
    shNode->ItemModified(chartNodeId);
    }
  std::vector< vtkMRMLNode* > seriesNodes;
  scene->GetNodesByClass("vtkMRMLPlotSeriesNode", seriesNodes);
  for (std::vector< vtkMRMLNode* >::iterator seriesIt = seriesNodes.begin(); seriesIt != seriesNodes.end(); ++seriesIt)
    {
    vtkMRMLPlotSeriesNode* seriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(*seriesIt);
    vtkIdType seriesNodeId = shNode->GetItemByDataNode(seriesNode);
    shNode->ItemModified(seriesNodeId);
    }
}

//-----------------------------------------------------------------------------
int qCjyxSubjectHierarchyPlotsPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return -1;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return -1;
    }

  vtkMRMLPlotViewNode* plotViewNode = this->getPlotViewNode();
  if (!plotViewNode || !plotViewNode->GetPlotChartNode())
    {
    // No quantitative view has been set yet
    return 0;
    }

  vtkMRMLPlotSeriesNode* plotSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (plotSeriesNode)
    {
    return (plotViewNode->GetPlotChartNode()->HasPlotSeriesNodeID(plotSeriesNode->GetID()) ? 1 : 0);
    }

  vtkMRMLPlotChartNode* plotChartNode = vtkMRMLPlotChartNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (plotChartNode)
    {
    // Return shown if plotLayout in plot view is the examined item's associated data node
    return (plotChartNode == plotViewNode->GetPlotChartNode()) ? 1 : 0;
    }

  return 0;
}

//---------------------------------------------------------------------------
vtkMRMLPlotViewNode* qCjyxSubjectHierarchyPlotsPlugin::getPlotViewNode()const
{
  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene!";
    return nullptr;
    }

  qMRMLLayoutManager* layoutManager = qCjyxApplication::application()->layoutManager();
  if (!layoutManager)
    {
    return nullptr;
    }

  for (int i=0; i<layoutManager->plotViewCount(); i++)
    {
    qMRMLPlotWidget* plotWidget = layoutManager->plotWidget(i);
    if (!plotWidget)
      {
      // invalid plot widget
      continue;
      }
    vtkMRMLPlotViewNode* plotView = plotWidget->mrmlPlotViewNode();
    if (plotView)
      {
      return plotView;
      }
    }

  // no valid plot view in current layout
  return nullptr;
}
