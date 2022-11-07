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

// Subject Hierarchy includes
#include "qCjyxSubjectHierarchyPluginLogic.h"

#include "vtkCjyxSubjectHierarchyModuleLogic.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyCloneNodePlugin.h"
#include "qCjyxSubjectHierarchyParseLocalDataPlugin.h"
#include "qCjyxSubjectHierarchyRegisterPlugin.h"
#include "qCjyxSubjectHierarchyFolderPlugin.h"
#include "qCjyxSubjectHierarchyOpacityPlugin.h"
#include "qCjyxSubjectHierarchyViewContextMenuPlugin.h"
#include "qCjyxSubjectHierarchyVisibilityPlugin.h"
#include "qCjyxSubjectHierarchyExportPlugin.h"

// MRML includes
#include "vtkMRMLAbstractViewNode.h"
#include "vtkMRMLDisplayableNode.h"
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLInteractionEventData.h"

// Cjyx includes
#include "qCjyxApplication.h"
#include "vtkCjyxApplicationLogic.h"

// Qt includes
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QString>
#include <QVariantMap>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy
class qCjyxSubjectHierarchyPluginLogicPrivate
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyPluginLogic);
protected:
  qCjyxSubjectHierarchyPluginLogic* const q_ptr;
public:
  qCjyxSubjectHierarchyPluginLogicPrivate(qCjyxSubjectHierarchyPluginLogic& object);
  ~qCjyxSubjectHierarchyPluginLogicPrivate();
  void loadApplicationSettings();

  /// Menu shown when right-clicking in a slice or 3D view
  QMenu* ViewContextMenu;
  /// Edit properties action
  QAction* EditPropertiesAction;
  /// Actions from the registered plugins
  QList<QAction*> ViewContextMenuActions;
  /// Item ID for the currently displayed View menu
  vtkIdType CurrentItemID;
  /// If this list is non-empty then only those actions
  /// will be displayable in the view context menu that are in this list.
  QStringList AllowedViewContextMenuActionNames;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyPluginLogicPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPluginLogicPrivate::qCjyxSubjectHierarchyPluginLogicPrivate(qCjyxSubjectHierarchyPluginLogic& object)
  : q_ptr(&object)
  , CurrentItemID(0)
{
  // Register vtkIdType for use in python for subject hierarchy item IDs
  qRegisterMetaType<vtkIdType>("vtkIdType");
  //qRegisterMetaType<QList<vtkIdType> >("QList<vtkIdType>"); //TODO: Allows returning it but cannot be used (e.g. pluginHandler->currentItems())

  this->ViewContextMenu = new QMenu();

  this->EditPropertiesAction = new QAction("Edit properties...");
  this->EditPropertiesAction->setObjectName("EditPropertiesAction");
  // weight=30 will place it towards the end of node actions section
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->EditPropertiesAction, qCjyxSubjectHierarchyAbstractPlugin::SectionNode, 30);
  QObject::connect(this->EditPropertiesAction, SIGNAL(triggered()), q_ptr, SLOT(editProperties()));
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPluginLogicPrivate::~qCjyxSubjectHierarchyPluginLogicPrivate()
{
  this->ViewContextMenu->deleteLater();
  this->ViewContextMenu = nullptr;

  this->EditPropertiesAction->deleteLater();
  this->EditPropertiesAction = nullptr;
}

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyPluginLogic methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPluginLogic::qCjyxSubjectHierarchyPluginLogic(QObject* _parent)
  : Superclass( _parent )
  , d_ptr( new qCjyxSubjectHierarchyPluginLogicPrivate(*this) )
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);

  this->registerViewContextMenuAction(d->EditPropertiesAction);

  // Register Subject Hierarchy core plugins
  this->registerCorePlugins();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPluginLogic::~qCjyxSubjectHierarchyPluginLogic() = default;

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::registerCorePlugins()
{
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyViewContextMenuPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyFolderPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyParseLocalDataPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyCloneNodePlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyRegisterPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyOpacityPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyVisibilityPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchyExportPlugin());
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyAbstractPlugin* qCjyxSubjectHierarchyPluginLogic::subjectHierarchyPluginByName(QString name)const
{
  return qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName(name);
}

//-----------------------------------------------------------------------------
vtkIdType qCjyxSubjectHierarchyPluginLogic::currentSubjectHierarchyItem()const
{
  return qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::setCurrentSubjectHierarchyItem(vtkIdType itemID)
{
  qCjyxSubjectHierarchyPluginHandler::instance()->setCurrentItem(itemID);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::setMRMLScene(vtkMRMLScene* scene)
{
  this->qCjyxObject::setMRMLScene(scene);

  // Set the new scene to the plugin handler
  qCjyxSubjectHierarchyPluginHandler::instance()->setMRMLScene(scene);

  // Connect scene node added event so that the new subject hierarchy items can be claimed by a plugin
  qvtkReconnect( scene, vtkMRMLScene::NodeAddedEvent, this, SLOT( onNodeAdded(vtkObject*,vtkObject*) ) );
  // Connect scene node about to be removed event so that the associated subject hierarchy node can be deleted too
  qvtkReconnect( scene, vtkMRMLScene::NodeAboutToBeRemovedEvent, this, SLOT( onNodeAboutToBeRemoved(vtkObject*,vtkObject*) ) );
  // Connect scene node removed event so if the subject hierarchy node is removed, it is re-created and the hierarchy rebuilt
  qvtkReconnect( scene, vtkMRMLScene::NodeRemovedEvent, this, SLOT( onNodeRemoved(vtkObject*,vtkObject*) ) );
  // Connect scene import ended event so that subject hierarchy items can be created for supported data nodes if missing (backwards compatibility)
  // Called with high priority so that it is processed here before the model is updated
  qvtkReconnect( scene, vtkMRMLScene::EndImportEvent, this, SLOT( onSceneImportEnded(vtkObject*) ), 10.0 );
  // Connect scene close ended event so that subject hierarchy can be cleared
  qvtkReconnect( scene, vtkMRMLScene::EndCloseEvent, this, SLOT( onSceneCloseEnded(vtkObject*) ) );
  // Connect scene restore ended event so that restored subject hierarchy node containing only unresolved items can be resolved
  qvtkReconnect( scene, vtkMRMLScene::EndRestoreEvent, this, SLOT( onSceneRestoreEnded(vtkObject*) ) );
  // Connect scene batch process ended event so that subject hierarchy is updated after batch processing, when nodes
  // may be added/removed without individual events.
  // Called with high priority so that it is processed here before the model is updated
  qvtkReconnect( scene, vtkMRMLScene::EndBatchProcessEvent, this, SLOT( onSceneBatchProcessEnded(vtkObject*) ), 10.0 );

  vtkCjyxApplicationLogic* appLogic = qCjyxApplication::application()->applicationLogic();
  if (appLogic)
    {
    qvtkReconnect(appLogic, vtkMRMLApplicationLogic::ShowViewContextMenuEvent, this, SLOT(onDisplayMenuEvent(vtkObject*, vtkObject*)));
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " error: application logic is not found. Cannot observe show view context menu event";
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::observeNode(vtkMRMLNode* node)
{
  // Make observations between the added node and certain plugins

  // Observe display modified event so that display node menu events can be managed by subject hierarchy
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(node);

  // qvtkReconnect would delete connections to all other displayable nodes that have been observed, so we need to
  // add connection using qvtkIsConnected and qvtkConnect.
  if (displayableNode && !qvtkIsConnected(displayableNode, vtkMRMLDisplayableNode::DisplayModifiedEvent, this, SLOT(onDisplayNodeModified(vtkObject*, vtkObject*))))
    {
    qvtkConnect(displayableNode, vtkMRMLDisplayableNode::DisplayModifiedEvent, this, SLOT(onDisplayNodeModified(vtkObject*, vtkObject*)));
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onNodeAdded(vtkObject* sceneObject, vtkObject* nodeObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }

  // If subject hierarchy node, then merge it with the already used subject hierarchy node (and remove the new one)
  vtkMRMLSubjectHierarchyNode* subjectHierarchyNode = vtkMRMLSubjectHierarchyNode::SafeDownCast(nodeObject);
  if (subjectHierarchyNode)
    {
    // Calling this function makes sure that there is exactly one subject hierarchy node in the scene (performs the merge if more found)
    vtkMRMLSubjectHierarchyNode::ResolveSubjectHierarchy(scene);
    }
  // If data node, then add it to subject hierarchy
  else
    {
    vtkMRMLNode* node = vtkMRMLNode::SafeDownCast(nodeObject);

    // Add subject hierarchy node for the added data node
    // Don't add to subject hierarchy automatically one-by-one if importing scene, because the SH nodes may be stored in the scene and loaded
    // Also abort if invalid or hidden node. The HideFromEditors flag is not considered to work dynamically, meaning that
    // we don't expect changes on the UI when we set it after adding it to the scene, so not adding it to SH should not cause issues.
    // The GetSubjectHierarchyExcludeFromTreeAttributeName attribute on the other hand is dynamic, so we add the node to SH despite that.
    if (scene->IsImporting() || !node || node->GetHideFromEditors())
      {
      return;
      }

    // If there is a plugin that can add the data node to subject hierarchy, then add
    QList<qCjyxSubjectHierarchyAbstractPlugin*> foundPlugins =
      qCjyxSubjectHierarchyPluginHandler::instance()->pluginsForAddingNodeToSubjectHierarchy(node);
    qCjyxSubjectHierarchyAbstractPlugin* selectedPlugin = nullptr;
    if (foundPlugins.size() > 1)
      {
      // Let the user choose a plugin if more than one returned the same non-zero confidence value
      QString textToDisplay = QString("Equal confidence number found for more than one subject hierarchy plugin for adding new node to subject hierarchy.\n\nSelect plugin to add node named\n'%1'\n(type %2)").arg(node->GetName()).arg(node->GetNodeTagName());
      selectedPlugin = qCjyxSubjectHierarchyPluginHandler::instance()->selectPluginFromDialog(textToDisplay, foundPlugins);
      }
     else if (foundPlugins.size() == 1)
      {
      selectedPlugin = foundPlugins[0];
      }
    // Have the selected plugin add the new node to subject hierarchy
    if (selectedPlugin)
      {
      // Get subject hierarchy node
      vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(node->GetScene());
      if (!shNode)
        {
        qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
        return;
        }
      // Add under subject hierarchy scene item
      bool successfullyAddedByPlugin = selectedPlugin->addNodeToSubjectHierarchy(node, shNode->GetSceneItemID());
      if (!successfullyAddedByPlugin)
        {
        // Should never happen! If a plugin answers positively to the canOwn question (condition of
        // reaching this point), then it has to be able to add it.
        qCritical() << Q_FUNC_INFO << ": Failed to add node " << node->GetName() <<
          " through plugin '" << selectedPlugin->name().toUtf8().constData() << "'";
        }
      // Make observations if adding was successful
      else
        {
        this->observeNode(node);
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onNodeAboutToBeRemoved(vtkObject* sceneObject, vtkObject* nodeObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene || scene->IsClosing())
    {
    // Do nothing if scene is closing
    return;
    }

  vtkMRMLNode* dataNode = vtkMRMLNode::SafeDownCast(nodeObject);
  if (!dataNode || dataNode->IsA("vtkMRMLSubjectHierarchyNode"))
    {
    return;
    }

  // Get subject hierarchy node
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(dataNode->GetScene());
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Remove associated subject hierarchy item if any
  vtkIdType itemID = shNode->GetItemByDataNode(dataNode);
  if (itemID)
    {
    shNode->RemoveItem(itemID, false, false);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onNodeRemoved(vtkObject* sceneObject, vtkObject* nodeObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene || scene->IsClosing())
    {
    // Do nothing if scene is closing
    return;
    }

  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::SafeDownCast(nodeObject);
  if (shNode)
    {
    // Make sure a new quasi-singleton subject hierarchy node is created
    vtkMRMLSubjectHierarchyNode::ResolveSubjectHierarchy(scene);

    // Add data nodes that are supported (i.e. there is a plugin that can claim it) and were not
    // in the imported subject hierarchy node to subject hierarchy
    this->addSupportedDataNodesToSubjectHierarchy();
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onSceneImportEnded(vtkObject* sceneObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }

  // This call is needed to trigger merging the imported subject hierarchy node containing the
  // unresolved items into the singleton subject hierarchy node in the current scene. This would
  // be done when first accessing the subject hierarchy node, but it needs to be done so that
  // the addSupportedDataNodesToSubjectHierarchy call below only adds the nodes that were not
  // in the hierarchy stored by the imported scene
  vtkMRMLSubjectHierarchyNode::ResolveSubjectHierarchy(scene);

  // Add data nodes that are supported (i.e. there is a plugin that can claim it) and were not
  // in the imported subject hierarchy node to subject hierarchy
  this->addSupportedDataNodesToSubjectHierarchy();
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onSceneCloseEnded(vtkObject* sceneObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }

  // Trigger creating new subject hierarchy node
  // (scene close removed the pseudo-singleton subject hierarchy node)
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::ResolveSubjectHierarchy(scene);
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": There must be a subject hierarchy node in the scene";
    return;
    }

  // Set subject hierarchy node to plugin handler
  qCjyxSubjectHierarchyPluginHandler::instance()->observeSubjectHierarchyNode(shNode);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onSceneRestoreEnded(vtkObject* sceneObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }

  // This call is needed to resolve unresolved items that were copied into the hierarchy
  // when restoring the scene view
  vtkMRMLSubjectHierarchyNode::ResolveSubjectHierarchy(scene);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onSceneBatchProcessEnded(vtkObject* sceneObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = scene->GetSubjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": There must be a subject hierarchy node in the scene";
    return;
    }

  // Go through items, delete the ones that are not folders or virtual items and don't have data nodes
  std::vector<vtkIdType> allItemIDs;
  shNode->GetItemChildren(shNode->GetSceneItemID(), allItemIDs, true);
  for (std::vector<vtkIdType>::iterator itemIt=allItemIDs.begin(); itemIt!=allItemIDs.end(); ++itemIt)
    {
    vtkIdType itemID = (*itemIt);
    if (!shNode->GetItemLevel(itemID).empty())
      {
      continue; // Folder type item
      }
    if (shNode->GetItemOwnerPluginName(itemID) == "Segments")
      {
      // In legacy scenes, for segment virtual items, Level is set to empty
      // (instead of VirtualBranch, as in current scenes), therefore they are not
      // found as folder type items and the segments get removed from the scene.
      // To fix this, we treat "Segment" type items as folder items and skip them.
      continue;
      }
    if (shNode->HasItemAttribute(itemID,
      vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyVirtualBranchAttributeName()))
      {
      // In virtual branch
      continue;
      }
    if (shNode->GetItemDataNode(itemID) == nullptr)
      {
      shNode->RemoveItem(itemID, false, false);
      }
    }

  // Add data nodes that are supported (i.e. there is a plugin that can claim it) and were not
  // in the imported subject hierarchy node to subject hierarchy
  this->addSupportedDataNodesToSubjectHierarchy();
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onDisplayNodeModified(vtkObject* displayableNodeObject, vtkObject* displayNodeObject)
{
  vtkMRMLDisplayNode* displayNode = vtkMRMLDisplayNode::SafeDownCast(displayNodeObject);
  if (displayNode)
    {
    if (!qvtkIsConnected(displayNode, vtkMRMLDisplayNode::MenuEvent, this, SLOT(onDisplayMenuEvent(vtkObject*, vtkObject*))))
      {
      qvtkConnect(displayNode, vtkMRMLDisplayNode::MenuEvent, this, SLOT(onDisplayMenuEvent(vtkObject*, vtkObject*)));
      }
    }
  else
    {
    // end of batch processing, no display node object is provided (as multiple display nodes may have been changed)
    // update the observer on all of them
    vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(displayableNodeObject);
    if (displayableNode)
      {
      int numberOfDisplayNodes = displayableNode->GetNumberOfDisplayNodes();
      for (int displayNodeIndex = 0; displayNodeIndex < numberOfDisplayNodes; displayNodeIndex++)
        {
        vtkMRMLDisplayNode* displayNode = displayableNode->GetNthDisplayNode(displayNodeIndex);
        if (!displayNode)
          {
          continue;
          }
        if (!qvtkIsConnected(displayNode, vtkMRMLDisplayNode::MenuEvent, this, SLOT(onDisplayMenuEvent(vtkObject*, vtkObject*))))
          {
          qvtkConnect(displayNode, vtkMRMLDisplayNode::MenuEvent, this, SLOT(onDisplayMenuEvent(vtkObject*, vtkObject*)));
          }
        }
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::onDisplayMenuEvent(vtkObject* displayNodeObject, vtkObject* eventDataObject)
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);

  vtkMRMLInteractionEventData* eventData = vtkMRMLInteractionEventData::SafeDownCast(eventDataObject);
  if (!eventData)
    {
    qCritical() << Q_FUNC_INFO<< ": Menu event called with invalid event data";
    return;
    }

  vtkMRMLSubjectHierarchyNode* shNode = nullptr;
  vtkIdType itemID = 0;

  vtkMRMLDisplayNode* displayNode = vtkMRMLDisplayNode::SafeDownCast(displayNodeObject);
  if (displayNode)
  {
    if (!displayNode->GetScene())
      {
      qCritical() << Q_FUNC_INFO<< ": Invalid object type calling display menu event";
      return;
      }
    vtkMRMLDisplayableNode* displayableNode = displayNode->GetDisplayableNode();
    if (!displayableNode)
      {
      qCritical() << Q_FUNC_INFO<< ": Unable to get displayable node from display node " << displayNode->GetID();
      return;
      }

    shNode = displayNode->GetScene()->GetSubjectHierarchyNode();
    if (!shNode)
      {
      qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
      return;
      }

    // Get subject hierarchy item ID
    itemID = shNode->GetItemByDataNode(displayableNode);
    if (!itemID)
      {
      qCritical() << Q_FUNC_INFO << ": Failed to find displayable node " << (displayableNode->GetName() ? displayableNode->GetName() : "Unnamed")
        << " in subject hierarchy";
      return;
      }
    }
  else
    {
    vtkCjyxApplicationLogic* appLogic = qCjyxApplication::application()->applicationLogic();
    if (!appLogic || !appLogic->GetMRMLScene())
      {
      qCritical() << Q_FUNC_INFO << ": Failed to access application logic or scene";
      return;
      }
    vtkMRMLScene* scene = appLogic->GetMRMLScene();
    shNode = scene->GetSubjectHierarchyNode();
    if (!shNode)
      {
      qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
      return;
      }
    itemID = shNode->GetSceneItemID();
    }

  // Package event data
  QVariantMap eventDataMap;
  eventDataMap["ComponentType"] = QVariant(eventData->GetComponentType());
  eventDataMap["ComponentIndex"] = QVariant(eventData->GetComponentIndex());
  if (eventData->GetViewNode())
    {
    eventDataMap["ViewNodeID"] = QVariant(eventData->GetViewNode()->GetID());
    }
  if (eventData->IsWorldPositionValid())
    {
    double worldPos[3] = { 0.0 };
    eventData->GetWorldPosition(worldPos);
    QVariantList worldPosVector;
    worldPosVector.push_back(worldPos[0]);
    worldPosVector.push_back(worldPos[1]);
    worldPosVector.push_back(worldPos[2]);
    eventDataMap["WorldPosition"] = QVariant(worldPosVector);
    }

  // Have all plugins show context view menu actions for current item
  foreach (qCjyxSubjectHierarchyAbstractPlugin* plugin, qCjyxSubjectHierarchyPluginHandler::instance()->allPlugins())
    {
    plugin->hideAllContextMenuActions();
    plugin->showViewContextMenuActionsForItem(itemID, eventDataMap);
    }

  // Only display "Edit properties..." if properties can be actually edited
  bool editActionVisible = false;
  if (itemID)
    {
    qCjyxSubjectHierarchyAbstractPlugin* ownerPlugin =
      qCjyxSubjectHierarchyPluginHandler::instance()->getOwnerPluginForSubjectHierarchyItem(itemID);
    if (ownerPlugin)
      {
      editActionVisible = ownerPlugin->canEditProperties(itemID);
      }
    }
  d->EditPropertiesAction->setVisible(editActionVisible);

  // View context menu actions are filtered by enabledViewContextMenuActions for this item.
  QStringList allowedViewContextMenuActionListForItem = this->allowedViewContextMenuActionNamesForItem(itemID);
  if (!allowedViewContextMenuActionListForItem.empty())
    {
    for (QAction* action : d->ViewContextMenuActions)
      {
      if (!allowedViewContextMenuActionListForItem.contains(action->objectName()))
        {
        action->setVisible(false);
        }
      }
    }

  // Set current item ID for Edit properties action
  d->CurrentItemID = itemID;

  // Show menu (only if there are visible actions)
  if (!d->ViewContextMenu->isEmpty())
    {
    d->ViewContextMenu->move(QCursor::pos());
    d->ViewContextMenu->exec();
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::addSupportedDataNodesToSubjectHierarchy()
{
  // Get subject hierarchy node
  vtkMRMLScene* scene = this->mrmlScene();
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(scene);
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Traverse all nodes in the scene (those contain data that can be saved with the scene)
  // and all hierarchy nodes (that specify hierarchy for certain types of data nodes and may be mirrored by the plugins of those data node types)
  std::vector<vtkMRMLNode*> supportedNodes;
  scene->GetNodesByClass("vtkMRMLNode", supportedNodes);
  for (std::vector<vtkMRMLNode*>::iterator nodeIt = supportedNodes.begin(); nodeIt != supportedNodes.end(); ++nodeIt)
    {
    vtkMRMLNode* node = (*nodeIt);
    // Do not add into subject hierarchy if hidden. The HideFromEditors flag is not considered to work dynamically, meaning that
    // we don't expect changes on the UI when we set it after adding it to the scene, so not adding it to SH should not cause issues.
    // The GetSubjectHierarchyExcludeFromTreeAttributeName attribute on the other hand is dynamic, so we add the node to SH despite that.
    if (node->GetHideFromEditors())
      {
      continue;
      }

    // Add node to subject hierarchy if not added yet
    if (!shNode->GetItemByDataNode(node))
      {
      // If there is a plugin that can add the data node to subject hierarchy, then add
      QList<qCjyxSubjectHierarchyAbstractPlugin*> foundPlugins =
        qCjyxSubjectHierarchyPluginHandler::instance()->pluginsForAddingNodeToSubjectHierarchy(
            node, vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID);
      qCjyxSubjectHierarchyAbstractPlugin* selectedPlugin = nullptr;
      if (foundPlugins.size() > 0)
        {
        // Choose first plugin in case of confidence equality not to annoy user (it can be changed later in subject hierarchy module)
        selectedPlugin = foundPlugins[0];
        }
      // Have the selected plugin add the new node to subject hierarchy
      if (!selectedPlugin)
        {
        // no plugin found for this node
        continue;
        }
      if (!selectedPlugin->addNodeToSubjectHierarchy(node, shNode->GetSceneItemID()))
        {
        // Should never happen! If a plugin answers positively to the canOwn question (condition of
        // reaching this point), then it has to be able to add it.
        qCritical() << Q_FUNC_INFO << ": Failed to add node " << node->GetName()
          << " through plugin '" << selectedPlugin->name().toUtf8().constData() << "'";
        continue;
        }
      }

    // Make sure the node is observed
    this->observeNode(node);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::registerViewContextMenuAction(QAction* action)
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);
  if (action)
    {
    d->ViewContextMenuActions << action;
    }
  qCjyxSubjectHierarchyPluginLogic::buildMenuFromActions(d->ViewContextMenu, d->ViewContextMenuActions, d->AllowedViewContextMenuActionNames);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::editProperties()
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << " failed: Invalid subject hierarchy node";
    return;
    }
 if (!d->CurrentItemID)
    {
    qCritical() << Q_FUNC_INFO << " failed: Invalid current item";
    return;
    }

  qCjyxSubjectHierarchyAbstractPlugin* ownerPlugin =
    qCjyxSubjectHierarchyPluginHandler::instance()->getOwnerPluginForSubjectHierarchyItem(d->CurrentItemID);
  if (!ownerPlugin)
    {
    qCritical() << Q_FUNC_INFO << " failed: Invalid owner plugin";
    return;
    }
  ownerPlugin->editProperties(d->CurrentItemID);
}

//-----------------------------------------------------------------------------
QStringList qCjyxSubjectHierarchyPluginLogic::registeredViewContextMenuActionNames()
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);

  QStringList registeredActionNames;
  foreach (QAction* action, d->ViewContextMenuActions)
    {
    registeredActionNames << action->objectName();
    }

  return registeredActionNames;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::setAllowedViewContextMenuActionNames(QStringList actionObjectNames)
{
  Q_D(qCjyxSubjectHierarchyPluginLogic);
  d->AllowedViewContextMenuActionNames = actionObjectNames;
  qCjyxSubjectHierarchyPluginLogic::buildMenuFromActions(d->ViewContextMenu, d->ViewContextMenuActions, d->AllowedViewContextMenuActionNames);
}

//-----------------------------------------------------------------------------
QStringList qCjyxSubjectHierarchyPluginLogic::allowedViewContextMenuActionNames() const
{
  Q_D(const qCjyxSubjectHierarchyPluginLogic);
  return d->AllowedViewContextMenuActionNames;
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyPluginLogic::buildMenuFromActions(QMenu* menu, QList< QAction* > actions, const QStringList& allowedActions)
{
  QString menuInfo;
  if (menu)
    {
    menu->clear();
    }
  if (actions.empty())
    {
    return menuInfo;
    }

  std::sort(actions.begin(), actions.end(),
    [](const QAction* a, const QAction* b) -> bool { return a->property("section").toDouble() < b->property("section").toDouble(); });

  int lastSection = static_cast<int>(actions.front()->property("section").toDouble() + 0.5);
  foreach (QAction* action, actions)
    {
    if (!allowedActions.isEmpty() && !allowedActions.contains(action->objectName()))
      {
      // the action is not on the allow-list, skip it
      continue;
      }
    double sectionValue = action->property("section").toDouble();
    int currentSection = static_cast<int>(sectionValue + 0.5);
    if (currentSection > lastSection)
      {
      if (menu)
        {
        menu->addSeparator();
        }
      else
        {
        menuInfo.append(QString("------ (%1)\n").arg(currentSection));
        }
      lastSection = currentSection;
      }
    if (menu)
      {
      menu->addAction(action);
      }
    else
      {
      menuInfo.append(QString("%1 (%2)\n").arg(action->text()).arg(sectionValue));
      }
    }

  return menuInfo;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyPluginLogic::setAllowedViewContextMenuActionNamesForItem(vtkIdType itemID, const QStringList& actionObjectNames)
{
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(this->mrmlScene());
  if (!shNode)
    {
    qWarning() << Q_FUNC_INFO << " failed: invalid subject hierarchy node";
    return;
    }
  std::string allowedViewContextMenuActions = actionObjectNames.join(";").toStdString();
  shNode->SetItemAttribute(itemID, "allowedViewContextMenuActions", allowedViewContextMenuActions);
}

//-----------------------------------------------------------------------------
QStringList qCjyxSubjectHierarchyPluginLogic::allowedViewContextMenuActionNamesForItem(vtkIdType itemID)
{
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(this->mrmlScene());
  if (!shNode)
    {
    qWarning() << Q_FUNC_INFO << " failed: invalid subject hierarchy node";
    return QStringList();
    }
  std::string shNodeEnabledViewContextMenuActions = shNode->GetItemAttribute(itemID, "allowedViewContextMenuActions");
  if (shNodeEnabledViewContextMenuActions.empty())
    {
    return QStringList();
    }
  QStringList allowedViewContextMenuActionListForItem = QString::fromStdString(shNodeEnabledViewContextMenuActions).split(";");
  return allowedViewContextMenuActionListForItem;
}
