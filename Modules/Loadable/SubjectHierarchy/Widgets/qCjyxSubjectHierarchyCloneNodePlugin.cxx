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
#include "qCjyxSubjectHierarchyCloneNodePlugin.h"

// SubjectHierarchy logic includes
#include "vtkCjyxSubjectHierarchyModuleLogic.h"

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "vtkCjyxApplicationLogic.h"

// VTK includes
#include <vtkObjectFactory.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyCloneNodePluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyCloneNodePlugin);
protected:
  qCjyxSubjectHierarchyCloneNodePlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyCloneNodePluginPrivate(qCjyxSubjectHierarchyCloneNodePlugin& object);
  ~qCjyxSubjectHierarchyCloneNodePluginPrivate() override;
  void init();
public:
  QAction* CloneItemAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyCloneNodePluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyCloneNodePluginPrivate::qCjyxSubjectHierarchyCloneNodePluginPrivate(qCjyxSubjectHierarchyCloneNodePlugin& object)
: q_ptr(&object)
{
  this->CloneItemAction = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyCloneNodePluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyCloneNodePlugin);

  this->CloneItemAction = new QAction("Clone",q);
  this->CloneItemAction->setToolTip("Clone this item and its data node if any along with display and storage options");
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->CloneItemAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionNode, 0.5); // put it right after "Rename" action
  QObject::connect(this->CloneItemAction, SIGNAL(triggered()), q, SLOT(cloneCurrentItem()));
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyCloneNodePluginPrivate::~qCjyxSubjectHierarchyCloneNodePluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyCloneNodePlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyCloneNodePlugin::qCjyxSubjectHierarchyCloneNodePlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyCloneNodePluginPrivate(*this) )
{
  this->m_Name = QString("CloneNode");

  Q_D(qCjyxSubjectHierarchyCloneNodePlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyCloneNodePlugin::~qCjyxSubjectHierarchyCloneNodePlugin() = default;

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyCloneNodePlugin::itemContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyCloneNodePlugin);

  QList<QAction*> actions;
  actions << d->CloneItemAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyCloneNodePlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyCloneNodePlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  if (!itemID || itemID == shNode->GetSceneItemID())
    {
    // There are no scene actions in this plugin
    return;
    }

  vtkIdType parentItemID = shNode->GetItemParent(itemID);
  if (parentItemID && shNode->IsItemVirtualBranchParent(parentItemID))
    {
    // This generic plugin does not know how to clone virtual branch items
    return;
    }

  // Show clone node for every non-scene items
  d->CloneItemAction->setVisible(true);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyCloneNodePlugin::cloneCurrentItem()
{
  // Get currently selected node and scene
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (!currentItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current subject hierarchy item!";
    return;
    }

  vtkIdType clonedItemID = vtkCjyxSubjectHierarchyModuleLogic::CloneSubjectHierarchyItem(shNode, currentItemID);
  if (!clonedItemID)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to clone subject hierarchy item (ID:"
        << currentItemID << ", name:" << shNode->GetItemName(currentItemID).c_str() << ")";
    }

  // Trigger update
  emit requestInvalidateFilter();
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyCloneNodePlugin::getCloneNodeNamePostfix()
{
  return QString(vtkCjyxSubjectHierarchyModuleLogic::CLONED_NODE_NAME_POSTFIX);
}
