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

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyNode.h"
#include "vtkMRMLSubjectHierarchyConstants.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyLabelMapsPlugin.h"
#include "qCjyxSubjectHierarchyVolumesPlugin.h"
#include "qCjyxSubjectHierarchyDefaultPlugin.h"

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "vtkCjyxApplicationLogic.h"

// MRML includes
#include <vtkMRMLLabelMapVolumeNode.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkMRMLSliceCompositeNode.h>
#include <vtkMRMLSliceNode.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkCollection.h>
#include <vtkImageData.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Plugins
class qCjyxSubjectHierarchyLabelMapsPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyLabelMapsPlugin);
protected:
  qCjyxSubjectHierarchyLabelMapsPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyLabelMapsPluginPrivate(qCjyxSubjectHierarchyLabelMapsPlugin& object);
  ~qCjyxSubjectHierarchyLabelMapsPluginPrivate() override;
  void init();
public:
  QIcon LabelmapIcon;

  QAction* ToggleOutlineVisibilityAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyLabelMapsPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyLabelMapsPluginPrivate::qCjyxSubjectHierarchyLabelMapsPluginPrivate(qCjyxSubjectHierarchyLabelMapsPlugin& object)
: q_ptr(&object)
{
  this->LabelmapIcon = QIcon(":Icons/Labelmap.png");

  this->ToggleOutlineVisibilityAction = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyLabelMapsPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyLabelMapsPlugin);

  this->ToggleOutlineVisibilityAction = new QAction("2D outline visibility",q);
  QObject::connect(this->ToggleOutlineVisibilityAction, SIGNAL(toggled(bool)), q, SLOT(toggle2DOutlineVisibility(bool)));
  this->ToggleOutlineVisibilityAction->setCheckable(true);
  this->ToggleOutlineVisibilityAction->setChecked(false);
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyLabelMapsPluginPrivate::~qCjyxSubjectHierarchyLabelMapsPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyLabelMapsPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyLabelMapsPlugin::qCjyxSubjectHierarchyLabelMapsPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyLabelMapsPluginPrivate(*this) )
{
  this->m_Name = QString("LabelMaps");

  Q_D(qCjyxSubjectHierarchyLabelMapsPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyLabelMapsPlugin::~qCjyxSubjectHierarchyLabelMapsPlugin() = default;

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyLabelMapsPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is nullptr!";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLLabelMapVolumeNode"))
    {
    // Node is a labelmap
    return 0.7;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyLabelMapsPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
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

  // Labelmap volume
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLLabelMapVolumeNode"))
    {
    return 0.7;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyLabelMapsPlugin::roleForPlugin()const
{
  return "Label map volume";
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyLabelMapsPlugin::tooltip(vtkIdType itemID)const
{
  return qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes")->tooltip(itemID);
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyLabelMapsPlugin::icon(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyLabelMapsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  // Volume
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    return d->LabelmapIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyLabelMapsPlugin::visibilityIcon(int visible)
{
  return qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes")->visibilityIcon(visible);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyLabelMapsPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
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

  vtkMRMLLabelMapVolumeNode* associatedLabelMapNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  // Labelmap volume
  if (associatedLabelMapNode)
    {
    if (visible)
      {
      // If visibility is on, then show the labelmap in the label layer of all slice views
      this->showLabelMapInAllViews(associatedLabelMapNode);
      }
    else
      {
      // If visibility is off, then hide the labelmap from all slice views
      qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
        qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes") );
      if (!volumesPlugin)
        {
        qCritical() << Q_FUNC_INFO << ": Failed to access Volumes subject hierarchy plugin";
        return;
        }
      volumesPlugin->hideVolumeFromAllViews(associatedLabelMapNode);
      }
    }
  // Default
  else
    {
    qCjyxSubjectHierarchyPluginHandler::instance()->defaultPlugin()->setDisplayVisibility(itemID, visible);
    }
}

//-----------------------------------------------------------------------------
int qCjyxSubjectHierarchyLabelMapsPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
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
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes") );
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access Volumes subject hierarchy plugin";
    return -1;
    }

  // Sanity checks for labelmap
  vtkMRMLLabelMapVolumeNode* labelMapNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!labelMapNode)
    {
    return -1;
    }

  // Collect all volumes that are shown in any slice views in any layers
  QSet<vtkIdType> shownVolumeItemIDs;

  volumesPlugin->collectShownVolumes(shownVolumeItemIDs);
  if (shownVolumeItemIDs.contains(itemID))
    {
    return 1;
    }
  else
    {
    return 0;
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyLabelMapsPlugin::showLabelMapInAllViews(vtkMRMLLabelMapVolumeNode* node)
{
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes") );
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access Volumes subject hierarchy plugin";
    return;
    }

  volumesPlugin->showVolumeInAllViews(node, vtkMRMLApplicationLogic::LabelLayer);
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyLabelMapsPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyLabelMapsPlugin);

  QList<QAction*> actions;
  actions << d->ToggleOutlineVisibilityAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyLabelMapsPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyLabelMapsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    return;
    }

  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();

  // LabelMap
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    // Determine current state of the toggle labelmap outline checkbox (from the first slice view)
    vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast ( scene->GetNthNodeByClass( 0, "vtkMRMLSliceNode" ) );
    int useLabelOutline = sliceNode->GetUseLabelOutline();
    d->ToggleOutlineVisibilityAction->blockSignals(true);
    d->ToggleOutlineVisibilityAction->setChecked(useLabelOutline);
    d->ToggleOutlineVisibilityAction->blockSignals(false);

    d->ToggleOutlineVisibilityAction->setVisible(true);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyLabelMapsPlugin::toggle2DOutlineVisibility(bool checked)
{
  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();
  vtkMRMLSliceNode* sliceNode = nullptr;
  const int numberOfSliceNodes = scene->GetNumberOfNodesByClass("vtkMRMLSliceNode");

  for (int i=0; i<numberOfSliceNodes; i++)
    {
    sliceNode = vtkMRMLSliceNode::SafeDownCast ( scene->GetNthNodeByClass( i, "vtkMRMLSliceNode" ) );
    sliceNode->SetUseLabelOutline(checked);
    }
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyLabelMapsPlugin::showItemInView(vtkIdType itemID, vtkMRMLAbstractViewNode* viewNode, vtkIdList* allItemsToShow)
{
  qCjyxSubjectHierarchyVolumesPlugin* volumesPlugin = qobject_cast<qCjyxSubjectHierarchyVolumesPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Volumes"));
  if (!volumesPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access Volumes subject hierarchy plugin";
    return false;
    }
  return volumesPlugin->showItemInView(itemID, viewNode, allItemsToShow);
}
