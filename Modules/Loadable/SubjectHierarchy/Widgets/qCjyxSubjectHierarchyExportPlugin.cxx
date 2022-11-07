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

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyExportPlugin.h"

// SubjectHierarchy logic includes
#include "vtkCjyxSubjectHierarchyModuleLogic.h"

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxIOManager.h"
#include "vtkCjyxApplicationLogic.h"

// VTK includes
#include <vtkObjectFactory.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

// MRML includes
#include <vtkMRMLStorableNode.h>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyExportPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyExportPlugin);
protected:
  qCjyxSubjectHierarchyExportPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyExportPluginPrivate(qCjyxSubjectHierarchyExportPlugin& object);
  ~qCjyxSubjectHierarchyExportPluginPrivate() override;
  void init();

  // Utility function for getting the subject-hierarchy-based relative path of an item.
  // Starting from fromItemID, this function climbs up the subject hierarchy tree until it reaches toItemID,
  // and it constructs a list of item names that it encounters along the way. The list goes into the return parameter `path`.
  // The list can be thought of as the interval "(fromItem, toItem]"; i.e. it excludes fromItem.
  // The returned bool indicates success.
  static bool getSubjectHierarchyPath(vtkMRMLSubjectHierarchyNode* shNode, vtkIdType fromItemID, vtkIdType toItemID, QStringList& path);

public:
  QAction* ExportItemAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyExportPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyExportPluginPrivate::qCjyxSubjectHierarchyExportPluginPrivate(qCjyxSubjectHierarchyExportPlugin& object)
: q_ptr(&object)
{
  this->ExportItemAction = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyExportPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyExportPlugin);

  this->ExportItemAction = new QAction("Export to file...",q);
  this->ExportItemAction->setToolTip("Export this node to a file");
  // Put towards end of the list, where export features typically would go
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->ExportItemAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 3);
  QObject::connect(this->ExportItemAction, SIGNAL(triggered()), q, SLOT(exportItems()));
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyExportPluginPrivate::getSubjectHierarchyPath(
  vtkMRMLSubjectHierarchyNode* shNode, vtkIdType fromItemID, vtkIdType toItemID, QStringList& path)
{
  path.clear();
  vtkIdType endItemID = shNode->GetItemParent(toItemID); // One item above the highest we want to go
  std::vector<vtkIdType> encounteredItemIds;
  for (vtkIdType itemID = shNode->GetItemParent(fromItemID); itemID != endItemID; itemID = shNode->GetItemParent(itemID))
    {
    if (itemID==0)
      {
      qCritical() << Q_FUNC_INFO << "failed while getting chain of parents from subject hierarchy item"
        << fromItemID << "to subject hierarchy item" << toItemID
        << ": encountered item ID 0 before reaching" << toItemID;
      return false;
      }
    if  (std::find(encounteredItemIds.begin(), encounteredItemIds.end(), itemID) != encounteredItemIds.end())
      {
      qCritical() << Q_FUNC_INFO << "failed while getting chain of parents from subject hierarchy item"
        << fromItemID << "to subject hierarchy item" << toItemID
        << ": encountered item ID" << itemID << "twice.";
      return false;
      }
    encounteredItemIds.push_back(itemID);
    path.push_back(QString::fromStdString(shNode->GetItemName(itemID)));
    }
  return true;
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyExportPluginPrivate::~qCjyxSubjectHierarchyExportPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyExportPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyExportPlugin::qCjyxSubjectHierarchyExportPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyExportPluginPrivate(*this) )
{
  this->m_Name = QString("Export");

  Q_D(qCjyxSubjectHierarchyExportPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyExportPlugin::~qCjyxSubjectHierarchyExportPlugin() = default;

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyExportPlugin::itemContextMenuActions() const
{
  Q_D(const qCjyxSubjectHierarchyExportPlugin);

  QList<QAction*> actions;
  actions << d->ExportItemAction;
  return actions;
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyExportPlugin::sceneContextMenuActions() const
{
  return this->itemContextMenuActions();
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyExportPlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyExportPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  if (!itemID)
    {
    return; // skip the case of invalid item ID
    }

  vtkIdType parentItemID = shNode->GetItemParent(itemID);
  if (parentItemID && shNode->IsItemVirtualBranchParent(parentItemID))
    {
    return; // no export for virtual branch items
    }

  vtkMRMLNode* node = shNode->GetItemDataNode(itemID);
  vtkMRMLStorableNode* storableNode = vtkMRMLStorableNode::SafeDownCast(node);

  std::vector<vtkIdType> children;
  shNode->GetItemChildren(itemID, children);

  // Show export action if the selection is a storable node or if it has children
  if (storableNode || children.size() > 0)
    {
    d->ExportItemAction->setVisible(true);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyExportPlugin::exportItems()
{
  // Get currently selected item in the subject hierarchy
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType selectedItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!selectedItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current subject hierarchy item!";
    return;
    }

  QList<QString> childIdsToExportNonrecursive;
  QList<QString> childIdsToExportRecursive;

  // This will map each node ID that the export dialog could need to a list of subject hierarchy item names
  // See the comment above the definition of getSubjectHierarchyPath for a description of the values in this map.
  QHash<QString, QVariant> nodeIdToSubjectHierarchyPath;

  std::vector<vtkIdType> childrenNonrecursive;
  std::vector<vtkIdType> childrenRecursive;
  shNode->GetItemChildren(selectedItemID, childrenNonrecursive, false);
  shNode->GetItemChildren(selectedItemID, childrenRecursive, true);
  for (vtkIdType childItemID : childrenNonrecursive)
    {
    vtkMRMLStorableNode* childStorableNode = vtkMRMLStorableNode::SafeDownCast(shNode->GetItemDataNode(childItemID));
    if (childStorableNode)
      {
      childIdsToExportNonrecursive.push_back(childStorableNode->GetID());
      }
    }
  for (vtkIdType childItemID : childrenRecursive)
    {
    vtkMRMLStorableNode* childStorableNode = vtkMRMLStorableNode::SafeDownCast(shNode->GetItemDataNode(childItemID));
    if (childStorableNode)
      {
      childIdsToExportRecursive.push_back(childStorableNode->GetID());

      QStringList path;
      if (!qCjyxSubjectHierarchyExportPluginPrivate::getSubjectHierarchyPath(shNode, childItemID, selectedItemID, path))
        {
        qCritical() << Q_FUNC_INFO << "failed: unable to ascend from subject hierarchy item" << childItemID << "to item" << selectedItemID;
        return;
        }
      nodeIdToSubjectHierarchyPath[childStorableNode->GetID()] = QVariant(path);
      }
    }

  vtkMRMLStorableNode* selectedStorableNode = vtkMRMLStorableNode::SafeDownCast(shNode->GetItemDataNode(selectedItemID));

  qCjyxIO::IOProperties properties{};
  if (selectedStorableNode)
    {
    properties["selectedNodeID"] = QString(selectedStorableNode->GetID());
    nodeIdToSubjectHierarchyPath[selectedStorableNode->GetID()] = QVariant(QStringList()); // The path from an item to itself is empty
    }
  if (!childIdsToExportNonrecursive.isEmpty())
    {
    properties["childIdsNonrecursive"] = QVariant(childIdsToExportNonrecursive);
    }
  if (!childIdsToExportRecursive.isEmpty())
    {
    properties["childIdsRecursive"] = QVariant(childIdsToExportRecursive);
    }
  properties["nodeIdToSubjectHierarchyPath"] = QVariant(nodeIdToSubjectHierarchyPath);

  qCjyxApplication::application()->ioManager()->openDialog(
    QString("GenericNodeExport"),
    qCjyxFileDialog::Write,
    properties
  );

}
