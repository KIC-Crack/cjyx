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
#include "qCjyxSubjectHierarchyTransformsPlugin.h"
#include "qCjyxSubjectHierarchyDefaultPlugin.h"

// Subject Hierarchy includes
#include <vtkCjyxSubjectHierarchyModuleLogic.h>

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLTransformNode.h>
#include <vtkMRMLTransformableNode.h>
#include <vtkMRMLTransformDisplayNode.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkNew.h>
#include <vtkMatrix4x4.h>

// Qt includes
#include <QDebug>
#include <QStandardItem>
#include <QAction>
#include <QMessageBox>

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

// MRML widgets includes
#include "qMRMLNodeComboBox.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Plugins
class qCjyxSubjectHierarchyTransformsPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyTransformsPlugin);
protected:
  qCjyxSubjectHierarchyTransformsPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyTransformsPluginPrivate(qCjyxSubjectHierarchyTransformsPlugin& object);
  ~qCjyxSubjectHierarchyTransformsPluginPrivate() override;
  void init();
public:
  QIcon TransformIcon;

  QAction* InvertAction;
  QAction* IdentityAction;

  QAction* ToggleInteractionBoxAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyTransformsPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyTransformsPluginPrivate::qCjyxSubjectHierarchyTransformsPluginPrivate(qCjyxSubjectHierarchyTransformsPlugin& object)
: q_ptr(&object)
{
  this->TransformIcon = QIcon(":Icons/Transform.png");
  this->ToggleInteractionBoxAction = nullptr;
  this->InvertAction = nullptr;
  this->IdentityAction = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyTransformsPlugin);

  this->InvertAction = new QAction("Invert transform",q);
  QObject::connect(this->InvertAction, SIGNAL(triggered()), q, SLOT(invert()));

  this->IdentityAction = new QAction("Reset transform to identity",q);
  QObject::connect(this->IdentityAction, SIGNAL(triggered()), q, SLOT(identity()));

  this->ToggleInteractionBoxAction = new QAction("Interaction in 3D view",q);
  QObject::connect(this->ToggleInteractionBoxAction, SIGNAL(toggled(bool)), q, SLOT(toggleInteractionBox(bool)));
  this->ToggleInteractionBoxAction->setCheckable(true);
  this->ToggleInteractionBoxAction->setChecked(false);
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyTransformsPluginPrivate::~qCjyxSubjectHierarchyTransformsPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyTransformsPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyTransformsPlugin::qCjyxSubjectHierarchyTransformsPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyTransformsPluginPrivate(*this) )
{
  this->m_Name = QString("Transforms");

  Q_D(qCjyxSubjectHierarchyTransformsPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyTransformsPlugin::~qCjyxSubjectHierarchyTransformsPlugin() = default;

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyTransformsPlugin::canReparentItemInsideSubjectHierarchy(vtkIdType itemID, vtkIdType parentItemID)const
{
  if (!itemID || !parentItemID)
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

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(shNode->GetItemDataNode(parentItemID));
  if (transformNode)
    {
    // If parent item is transform then can reparent
    return 1.0;
    }

  return 0.0;
}

//---------------------------------------------------------------------------
bool qCjyxSubjectHierarchyTransformsPlugin::reparentItemInsideSubjectHierarchy(vtkIdType itemID, vtkIdType parentItemID)
{
  if (!itemID || !parentItemID)
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

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(shNode->GetItemDataNode(parentItemID));
  if (!transformNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access transform node from parent item " << parentItemID;
    return false;
    }

  // Ask the user if any child node in the branch is transformed with a transform different from the chosen one
  bool hardenExistingTransforms = true;
  if (shNode->IsAnyNodeInBranchTransformed(itemID))
    {
    QMessageBox::StandardButton answer =
      QMessageBox::question(nullptr, tr("Some nodes in the branch are already transformed"),
      tr("Do you want to harden all already applied transforms before setting the new one?\n\n"
      "  Note: If you choose no, then the applied transform will simply be replaced."),
      QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
      QMessageBox::Yes);
    if (answer == QMessageBox::No)
      {
      hardenExistingTransforms = false;
      }
    else if (answer == QMessageBox::Cancel)
      {
      return false;
      }
    }

  // Transform all items in branch
  vtkCjyxSubjectHierarchyModuleLogic::TransformBranch(shNode, itemID, transformNode, hardenExistingTransforms);

  // Actual reparenting will never happen, only setting of the transform
  return false;
}

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyTransformsPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node, vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_UNUSED(parentItemID);
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << ": Input node is nullptr";
    return 0.0;
    }
  else if (node->IsA("vtkMRMLTransformNode"))
    {
    // Node is a transform
    return 0.5;
    }
  return 0.0;
}

//---------------------------------------------------------------------------
double qCjyxSubjectHierarchyTransformsPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
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

  // Transform
  vtkMRMLNode* associatedNode = shNode->GetItemDataNode(itemID);
  if (associatedNode && associatedNode->IsA("vtkMRMLTransformNode"))
    {
    return 0.5; // There are other plugins that can handle special transform nodes better, thus the relatively low value
    }

  return 0.0;
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyTransformsPlugin::roleForPlugin()const
{
  return "Transform";
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyTransformsPlugin::tooltip(vtkIdType itemID)const
{
  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QString("Invalid");
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return QString("Error");
    }

  // Get basic tooltip from abstract plugin
  QString tooltipString = Superclass::tooltip(itemID);

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (transformNode)
    {
    QString transformInfo = QString("\nTransform to parent:\n%1\nTransform from parent:\n%2").arg(
      transformNode->GetTransformToParentInfo()).arg(transformNode->GetTransformFromParentInfo());
    tooltipString.append(transformInfo);
    }

  return tooltipString;
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyTransformsPlugin::icon(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyTransformsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    qCritical() << Q_FUNC_INFO << ": Invalid input item";
    return QIcon();
    }

  // Transform
  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    return d->TransformIcon;
    }

  // Item unknown by plugin
  return QIcon();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyTransformsPlugin::visibilityIcon(int visible)
{
  // Have the default plugin (which is not registered) take care of this
  return qCjyxSubjectHierarchyPluginHandler::instance()->defaultPlugin()->visibilityIcon(visible);
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyTransformsPlugin::itemContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyTransformsPlugin);

  QList<QAction*> actions;
  actions << d->InvertAction << d->IdentityAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyTransformsPlugin);

  if (itemID == vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID)
    {
    // There are no scene actions in this plugin
    return;
    }
  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }

  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    d->InvertAction->setVisible(true);
    vtkMRMLTransformNode* tnode = vtkMRMLTransformNode::SafeDownCast(shNode->GetItemDataNode(itemID));
    if (tnode && tnode->IsLinear())
      {
      d->IdentityAction->setVisible(true);
      }
    }
}

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyTransformsPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyTransformsPlugin);

  QList<QAction*> actions;
  actions << d->ToggleInteractionBoxAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyTransformsPlugin);

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

  if (this->canOwnSubjectHierarchyItem(itemID))
    {
    vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(shNode->GetItemDataNode(itemID));
    if (transformNode)
      {
      vtkMRMLTransformDisplayNode* displayNode = vtkMRMLTransformDisplayNode::SafeDownCast(transformNode->GetDisplayNode());
      if (!displayNode)
        {
        transformNode->CreateDefaultDisplayNodes();
        displayNode = vtkMRMLTransformDisplayNode::SafeDownCast(transformNode->GetDisplayNode());
        }
      if (displayNode)
        {
        d->ToggleInteractionBoxAction->setVisible(true);
        bool wasBlocked = d->ToggleInteractionBoxAction->blockSignals(true);
        d->ToggleInteractionBoxAction->setChecked(displayNode->GetEditorVisibility());
        d->ToggleInteractionBoxAction->blockSignals(wasBlocked);
        }
      }
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPlugin::invert()
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

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(
    shNode->GetItemDataNode(currentItemID) );
  if (transformNode)
    {
    MRMLNodeModifyBlocker blocker(transformNode);
    transformNode->Inverse();
    transformNode->InverseName();
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPlugin::identity()
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

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(
    shNode->GetItemDataNode(currentItemID) );
  if (transformNode && transformNode->IsLinear())
    {
    vtkNew<vtkMatrix4x4> matrix; // initialized to identity by default
    transformNode->SetMatrixTransformToParent(matrix.GetPointer());
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyTransformsPlugin::toggleInteractionBox(bool visible)
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

  vtkMRMLTransformNode* transformNode = vtkMRMLTransformNode::SafeDownCast(
    shNode->GetItemDataNode(currentItemID) );
  if (!transformNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get transform node";
    return;
    }
  vtkMRMLTransformDisplayNode* displayNode = vtkMRMLTransformDisplayNode::SafeDownCast(
    transformNode->GetDisplayNode() );
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get transform display node";
    return;
    }

  displayNode->SetEditorVisibility(visible);
}
