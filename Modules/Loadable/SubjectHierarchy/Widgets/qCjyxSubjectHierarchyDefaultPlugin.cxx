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
#include "qCjyxSubjectHierarchyDefaultPlugin.h"
#include "qCjyxSubjectHierarchyPluginHandler.h"

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyConstants.h"
#include "vtkMRMLSubjectHierarchyNode.h"

// Qt includes
#include <QDebug>
#include <QIcon>
#include <QAction>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

//----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyDefaultPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyDefaultPlugin);
protected:
  qCjyxSubjectHierarchyDefaultPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyDefaultPluginPrivate(qCjyxSubjectHierarchyDefaultPlugin& object);
  ~qCjyxSubjectHierarchyDefaultPluginPrivate() override;
  void init();
public:
  QIcon UnknownIcon;

  QIcon VisibleIcon;
  QIcon HiddenIcon;
  QIcon PartiallyVisibleIcon;

  QAction* ShowAllChildrenAction;
  QAction* HideAllChildrenAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyDefaultPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDefaultPluginPrivate::qCjyxSubjectHierarchyDefaultPluginPrivate(qCjyxSubjectHierarchyDefaultPlugin& object)
: q_ptr(&object)
, ShowAllChildrenAction(nullptr)
, HideAllChildrenAction(nullptr)
{
  this->UnknownIcon = QIcon(":Icons/Unknown.png");
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDefaultPluginPrivate::~qCjyxSubjectHierarchyDefaultPluginPrivate() = default;

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyAbstractPlugin);

  this->ShowAllChildrenAction = new QAction("Show all children",q);
  QObject::connect(this->ShowAllChildrenAction, SIGNAL(triggered()), q, SLOT(showAllChildren()));

  this->HideAllChildrenAction = new QAction("Hide all children",q);
  QObject::connect(this->HideAllChildrenAction, SIGNAL(triggered()), q, SLOT(hideAllChildren()));
  }

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyDefaultPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDefaultPlugin::qCjyxSubjectHierarchyDefaultPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyDefaultPluginPrivate(*this) )
{
  this->m_Name = QString("Default");

  Q_D(qCjyxSubjectHierarchyDefaultPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyDefaultPlugin::~qCjyxSubjectHierarchyDefaultPlugin() = default;

//----------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPlugin::setDefaultVisibilityIcons(QIcon visibleIcon, QIcon hiddenIcon, QIcon partiallyVisibleIcon)
{
  Q_D(qCjyxSubjectHierarchyDefaultPlugin);

  d->VisibleIcon = visibleIcon;
  d->HiddenIcon = hiddenIcon;
  d->PartiallyVisibleIcon = partiallyVisibleIcon;
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyDefaultPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  Q_UNUSED(itemID);

  // The default Subject Hierarchy plugin is never selected by confidence number it returns
  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyDefaultPlugin::roleForPlugin()const
{
  return "Unknown";
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyDefaultPlugin::helpText()const
{
  return QString(
    "<p style=\" margin-top:4px; margin-bottom:1px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
    "<span style=\" font-family:'sans-serif'; font-size:9pt; font-weight:600; color:#000000;\">"
    "Rename item"
    "</span>"
    "</p>"
    "<p style=\" margin-top:0px; margin-bottom:11px; margin-left:26px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
    "<span style=\" font-family:'sans-serif'; font-size:9pt; color:#000000;\">"
    "Double-click the item name, or right-click the item and select 'Rename'"
    "</span>"
    "</p>"
    "<p style=\" margin-top:4px; margin-bottom:1px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
    "<span style=\" font-family:'sans-serif'; font-size:9pt; font-weight:600; color:#000000;\">"
    "Deform any branch using a transform (registration result)"
    "</span>"
    "</p>"
    "<p style=\" margin-top:0px; margin-bottom:11px; margin-left:26px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">"
    "<span style=\" font-family:'sans-serif'; font-size:9pt; color:#000000;\">"
    "Make sure the transform column is shown using the 'Show transforms' checkbox. "
    "To transform a branch, right-click on the cell in the transform column of the row in question, and choose a transform."
    "</span>"
    "</p>");
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyDefaultPlugin::icon(vtkIdType itemID)
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Input item is invalid";
    return QIcon();
    }

  Q_D(qCjyxSubjectHierarchyDefaultPlugin);

  // Unknown icon
  // This role is only used when there is no plugin to claim a node, which is an erroneous
  // scenario, as only those nodes can be added to subject hierarchy for which there is at
  // least one plugin that can claim them.
  return d->UnknownIcon;
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyDefaultPlugin::visibilityIcon(int visible)
{
  Q_D(qCjyxSubjectHierarchyDefaultPlugin);

  // Default icon is the eye icon that shows the visibility of the whole branch
  switch (visible)
    {
  case 0:
    return d->HiddenIcon;
  case 1:
    return d->VisibleIcon;
  case 2:
    return d->PartiallyVisibleIcon;
  default:
    return QIcon();
    }
}

//-----------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyDefaultPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyDefaultPlugin);

  QList<QAction*> actions;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyDefaultPlugin);

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

  // Show child-related actions only if there are children to the item
  std::vector<vtkIdType> childItems;
  shNode->GetItemChildren(itemID, childItems);
  d->ShowAllChildrenAction->setVisible(childItems.size());
  d->HideAllChildrenAction->setVisible(childItems.size());
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPlugin::toggleVisibility()
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (currentItemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current item";
    return;
    }
  qCjyxSubjectHierarchyAbstractPlugin* ownerPlugin =
    qCjyxSubjectHierarchyPluginHandler::instance()->getOwnerPluginForSubjectHierarchyItem(currentItemID);
  if (!ownerPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Subject hierarchy item " << currentItemID << " (named " << shNode->GetItemName(currentItemID).c_str() << ") is not owned by any plugin";
    return;
    }

  // Toggle current item visibility
  int visible = (ownerPlugin->getDisplayVisibility(currentItemID) > 0 ? 0 : 1);
  ownerPlugin->setDisplayVisibility(currentItemID, visible);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPlugin::showAllChildren()
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (currentItemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current item";
    return;
    }

  std::vector<vtkIdType> childItemIDs;
  shNode->GetItemChildren(currentItemID, childItemIDs, true);
  std::vector<vtkIdType>::iterator childIt;
  for (childIt=childItemIDs.begin(); childIt!=childItemIDs.end(); ++childIt)
    {
    vtkIdType childItemID = (*childIt);
    qCjyxSubjectHierarchyAbstractPlugin* ownerPlugin =
      qCjyxSubjectHierarchyPluginHandler::instance()->getOwnerPluginForSubjectHierarchyItem(childItemID);
    if (!ownerPlugin)
      {
      qCritical() << Q_FUNC_INFO << ": Subject hierarchy item " << childItemID << " (named "
        << shNode->GetItemName(childItemID).c_str() << ") is not owned by any plugin";
      return;
      }

    ownerPlugin->setDisplayVisibility(childItemID, 1);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyDefaultPlugin::hideAllChildren()
{
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  vtkIdType currentItemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
  if (currentItemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid current item";
    return;
    }

  std::vector<vtkIdType> childItemIDs;
  shNode->GetItemChildren(currentItemID, childItemIDs, true);
  std::vector<vtkIdType>::iterator childIt;
  for (childIt=childItemIDs.begin(); childIt!=childItemIDs.end(); ++childIt)
    {
    vtkIdType childItemID = (*childIt);
    qCjyxSubjectHierarchyAbstractPlugin* ownerPlugin =
      qCjyxSubjectHierarchyPluginHandler::instance()->getOwnerPluginForSubjectHierarchyItem(childItemID);
    if (!ownerPlugin)
      {
      qCritical() << Q_FUNC_INFO << ": Subject hierarchy item " << childItemID << " (named "
        << shNode->GetItemName(childItemID).c_str() << ") is not owned by any plugin";
      return;
      }

    ownerPlugin->setDisplayVisibility(childItemID, 0);
    }
}
