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
#include "vtkMRMLSubjectHierarchyConstants.h"
#include "vtkMRMLSubjectHierarchyNode.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyFolderPlugin.h"
#include "qCjyxSubjectHierarchyDefaultPlugin.h"

// Terminologies includes
#include "qCjyxTerminologyItemDelegate.h"

// Qt includes
#include <QAction>
#include <QDebug>
#include <QVariant>

// MRML includes
#include <vtkMRMLFolderDisplayNode.h>
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyFolderPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyFolderPlugin);
protected:
  qCjyxSubjectHierarchyFolderPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyFolderPluginPrivate(qCjyxSubjectHierarchyFolderPlugin& object);
  ~qCjyxSubjectHierarchyFolderPluginPrivate() override;
  void init();
public:
  QIcon FolderIcon;

  QAction* CreateFolderUnderSceneAction;
  QAction* CreateFolderUnderNodeAction;
  QAction* ApplyColorToBranchAction;

  QString AddedByFolderPluginAttributeName;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyFolderPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyFolderPluginPrivate::qCjyxSubjectHierarchyFolderPluginPrivate(qCjyxSubjectHierarchyFolderPlugin& object)
: q_ptr(&object)
{
  this->FolderIcon = QIcon(":Icons/Folder.png");

  this->CreateFolderUnderSceneAction = nullptr;
  this->CreateFolderUnderNodeAction = nullptr;
  this->ApplyColorToBranchAction = nullptr;

  std::string addedByFolderPluginAttributeNameStr =
      vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyAttributePrefix()
    + std::string(vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder());
  this->AddedByFolderPluginAttributeName = QString(addedByFolderPluginAttributeNameStr.c_str());
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyFolderPlugin);

  this->CreateFolderUnderSceneAction = new QAction("Create new folder",q);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->CreateFolderUnderSceneAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionFolder, 5);
  QObject::connect(this->CreateFolderUnderSceneAction, SIGNAL(triggered()), q, SLOT(createFolderUnderScene()));

  this->CreateFolderUnderNodeAction = new QAction("Create child folder",q);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->CreateFolderUnderNodeAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionFolder, 6);
  QObject::connect(this->CreateFolderUnderNodeAction, SIGNAL(triggered()), q, SLOT(createFolderUnderCurrentNode()));

  this->ApplyColorToBranchAction = new QAction("Apply color to all children",q);
  this->ApplyColorToBranchAction->setToolTip("If on, then children items will inherit the display properties (e.g. color or opacity) set to the folder");
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->ApplyColorToBranchAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionFolder, 7);
  QObject::connect(this->ApplyColorToBranchAction, SIGNAL(toggled(bool)), q, SLOT(onApplyColorToBranchToggled(bool)));
  this->ApplyColorToBranchAction->setCheckable(true);
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyFolderPluginPrivate::~qCjyxSubjectHierarchyFolderPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyFolderPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyFolderPlugin::qCjyxSubjectHierarchyFolderPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyFolderPluginPrivate(*this) )
{
  this->m_Name = QString("Folder");

  Q_D(qCjyxSubjectHierarchyFolderPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyFolderPlugin::~qCjyxSubjectHierarchyFolderPlugin() = default;

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyFolderPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is NULL";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLFolderDisplayNode"))
    {
    // Node is a folder display node (handle cases when the display node is added instead of an item created)
    return 1.0;
    }
  return 0.0;
}

//----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyFolderPlugin::addNodeToSubjectHierarchy(vtkMRMLNode* nodeToAdd, vtkIdType parentItemID)
{
  Q_D(qCjyxSubjectHierarchyFolderPlugin);
  if (nodeToAdd->GetAttribute(d->AddedByFolderPluginAttributeName.toUtf8().constData()))
    {
    // Prevent creation of new folder item if the folder display node was not added by the folder plugin
    return true;
    }

  return qCjyxSubjectHierarchyAbstractPlugin::addNodeToSubjectHierarchy(nodeToAdd, parentItemID);
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyFolderPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Input item is invalid";
    return 0.0;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return 0.0;
    }

  if (itemID == shNode->GetSceneItemID())
    {
    // Do not allow to assign display properties to the scene item,
    // because the scene item is not always visible and overall it is not prepared
    // to be used as a regular folder.
    return 0.0;
    }

  if (shNode->IsItemLevel(itemID, vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder()))
    {
    // Folder with no hierarchy node
    return 1.0;
    }
  else if (!shNode->GetItemLevel(itemID).empty())
    {
    // There is any level information (for example for DICOM levels which are also folders)
    return 0.5;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyFolderPlugin::roleForPlugin()const
{
  // Get current node to determine role
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!currentItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current node";
    return "Error!";
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return "Error!";
    }

  // Folder level
  if (shNode->IsItemLevel(currentItemID, vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder()))
    {
    return "Folder";
    }

  return QString("Error!");
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyFolderPlugin::icon(vtkIdType itemID)
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Input item is invalid";
    return QIcon();
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QIcon();
    }

  Q_D(qCjyxSubjectHierarchyFolderPlugin);

  // Subject and Folder icon
  if (shNode->IsItemLevel(itemID, vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder()))
    {
    return d->FolderIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyFolderPlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qCjyxSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  // Use the folder display node to set folder visibility.
  // This is necessary because the displayable manager considers this display node.
  vtkMRMLDisplayNode* displayNode = this->displayNodeForItem(itemID);
  if (!displayNode)
    {
    displayNode = this->createDisplayNodeForItem(itemID);
    }
  if (!displayNode)
    {
    // No display node can be associated with this item
    // (for example, it is a scripted module node)
    return;
    }
  displayNode->SetVisibility(visible);
}

//-----------------------------------------------------------------------------
int qCjyxSubjectHierarchyFolderPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  // Use the folder display node to get folder visibility.
  // This is necessary because the displayable manager considers this display node.
  vtkMRMLDisplayNode* displayNode = this->displayNodeForItem(itemID);
  if (displayNode)
    {
    return displayNode->GetVisibility();
    }

  // Visible by default, until visibility is changed, or apply display properties to branch is turned on.
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::setDisplayColor(vtkIdType itemID, QColor color, QMap<int, QVariant> terminologyMetaData)
{
  Q_UNUSED(terminologyMetaData);
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

  QMap<int, QVariant> dummyTerminology;
  QColor oldColor = this->getDisplayColor(itemID, dummyTerminology);
  if (oldColor != color)
    {
    // Get associated display node, create one if absent
    vtkMRMLDisplayNode* displayNode = this->displayNodeForItem(itemID);
    if (!displayNode)
      {
      displayNode = this->createDisplayNodeForItem(itemID);
      }

    displayNode->SetColor(color.redF(), color.greenF(), color.blueF());

    // Call modified on the folder item
    shNode->ItemModified(itemID);

    // If apply color to branch is not active then enable that option so that the new color shows
    if (!this->isApplyColorToBranchEnabledForItem(itemID))
      {
      this->setApplyColorToBranchEnabledForItem(itemID, true);
      }
   } // If color changed
}

//-----------------------------------------------------------------------------
QColor qCjyxSubjectHierarchyFolderPlugin::getDisplayColor(vtkIdType itemID, QMap<int, QVariant> &terminologyMetaData)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QColor(0,0,0,0);
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QColor(0,0,0,0);
    }
  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene";
    return QColor(0,0,0,0);
    }

  if (scene->IsImporting())
    {
    // During import SH node may be created before the segmentation is read into the scene,
    // so don't attempt to access the segment yet
    return QColor(0,0,0,0);
    }

  // Set dummy terminology information
  terminologyMetaData.clear();
  terminologyMetaData[qCjyxTerminologyItemDelegate::NameRole] = shNode->GetItemName(itemID).c_str();
  terminologyMetaData[qCjyxTerminologyItemDelegate::NameAutoGeneratedRole] = false;
  terminologyMetaData[qCjyxTerminologyItemDelegate::ColorAutoGeneratedRole] = true;

  // Get and return color
  vtkMRMLDisplayNode* displayNode = this->displayNodeForItem(itemID);
  if (!displayNode)
    {
    return QColor(0,0,0,0);
    }

  double colorArray[3] = {0.0,0.0,0.0};
  displayNode->GetColor(colorArray);
  return QColor::fromRgbF(colorArray[0], colorArray[1], colorArray[2]);
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyFolderPlugin::itemContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyFolderPlugin);

  QList<QAction*> actions;
  actions << d->CreateFolderUnderNodeAction;
  return actions;
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyFolderPlugin::sceneContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyFolderPlugin);

  QList<QAction*> actions;
  actions << d->CreateFolderUnderSceneAction << d->ApplyColorToBranchAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyFolderPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Scene
  if (itemID == shNode->GetSceneItemID())
    {
    d->CreateFolderUnderSceneAction->setVisible(true);
    return;
    }

  // Folder can be created under any node
  if (itemID)
    {
    d->CreateFolderUnderNodeAction->setVisible(true);
    }

  // Folder
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    bool applyColorToBranch = this->isApplyColorToBranchEnabledForItem(itemID);

    d->ApplyColorToBranchAction->blockSignals(true);
    d->ApplyColorToBranchAction->setChecked(applyColorToBranch);
    d->ApplyColorToBranchAction->blockSignals(false);
    d->ApplyColorToBranchAction->setVisible(true);
    }
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyFolderPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyFolderPlugin);

  QList<QAction*> actions;
  actions << d->ApplyColorToBranchAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyFolderPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  // Folder
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    bool applyColorToBranch = this->isApplyColorToBranchEnabledForItem(itemID);

    d->ApplyColorToBranchAction->blockSignals(true);
    d->ApplyColorToBranchAction->setChecked(applyColorToBranch);
    d->ApplyColorToBranchAction->blockSignals(false);
    d->ApplyColorToBranchAction->setVisible(true);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::editProperties(vtkIdType itemID)
{
  qCjyxSubjectHierarchyPluginHandler::instance()->defaultPlugin()->editProperties(itemID);
}

//---------------------------------------------------------------------------
vtkIdType qCjyxSubjectHierarchyFolderPlugin::createFolderUnderItem(vtkIdType parentItemID)
{
  vtkMRMLScene* scene = qCjyxSubjectHierarchyPluginHandler::instance()->mrmlScene();
  if (!scene)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid MRML scene";
    return vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID;
    }

  // Create folder subject hierarchy node
  std::string name = vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyNewItemNamePrefix()
    + vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder();
  name = shNode->GenerateUniqueItemName(name);
  vtkIdType childItemID = shNode->CreateFolderItem(parentItemID, name);
  emit requestExpandItem(childItemID);

  return childItemID;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::createFolderUnderScene()
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  this->createFolderUnderItem(shNode->GetSceneItemID());
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::createFolderUnderCurrentNode()
{
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!currentItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current node";
    return;
    }

  this->createFolderUnderItem(currentItemID);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::onApplyColorToBranchToggled(bool on)
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!currentItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current item";
    return;
    }

  this->setApplyColorToBranchEnabledForItem(currentItemID, on);
}

//-----------------------------------------------------------------------------
vtkMRMLDisplayNode* qCjyxSubjectHierarchyFolderPlugin::displayNodeForItem(vtkIdType itemID)const
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return nullptr;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return nullptr;
    }

  vtkMRMLNode* dataNode = shNode->GetItemDataNode(itemID);
  vtkMRMLDisplayNode* displayNode = vtkMRMLDisplayNode::SafeDownCast(dataNode);
  if (displayNode)
    {
    return displayNode;
    }
  return nullptr;
}

//-----------------------------------------------------------------------------
vtkMRMLDisplayNode* qCjyxSubjectHierarchyFolderPlugin::createDisplayNodeForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyFolderPlugin);
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return nullptr;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return nullptr;
    }

  vtkMRMLDisplayNode* existingDisplayNode = vtkMRMLDisplayNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  vtkMRMLNode* existingDataNode = shNode->GetItemDataNode(itemID);
  if (existingDisplayNode)
    {
    return existingDisplayNode;
    }
  if (existingDataNode)
    {
    qCritical() << Q_FUNC_INFO << ": Item " << itemID << " is already associated to a data node, but it is not a display node";
    return nullptr;
    }

  vtkNew<vtkMRMLFolderDisplayNode> displayNode;
  displayNode->SetName(shNode->GetItemName(itemID).c_str());
  displayNode->SetHideFromEditors(0); // Need to set this so that the folder shows up in SH
  displayNode->SetAttribute(d->AddedByFolderPluginAttributeName.toUtf8().constData(), "1");
  shNode->GetScene()->AddNode(displayNode);

  shNode->SetItemDataNode(itemID, displayNode);

  shNode->ItemModified(itemID);
  return displayNode;
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyFolderPlugin::isApplyColorToBranchEnabledForItem(vtkIdType itemID)const
{
  if (!itemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return false;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }

  vtkMRMLFolderDisplayNode* folderDisplayNode = vtkMRMLFolderDisplayNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!folderDisplayNode)
    {
    return false;
    }

  return folderDisplayNode->GetApplyDisplayPropertiesOnBranch();
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyFolderPlugin::setApplyColorToBranchEnabledForItem(vtkIdType itemID, bool enabled)
{
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

  vtkMRMLFolderDisplayNode* folderDisplayNode = vtkMRMLFolderDisplayNode::SafeDownCast(
    this->createDisplayNodeForItem(itemID) );
  if (!folderDisplayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get folder display node for item " << itemID;
    return;
    }

  folderDisplayNode->SetApplyDisplayPropertiesOnBranch(enabled);
}
