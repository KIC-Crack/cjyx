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

==============================================================================*/

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyColorLegendPlugin.h"

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxApplication.h"
#include "qCjyxLayoutManager.h"

// Colors includes
#include "vtkCjyxColorLogic.h"

// MRML includes
#include <vtkMRMLCameraNode.h>
#include <vtkMRMLColorLegendDisplayNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLViewLogic.h>
#include <vtkMRMLViewNode.h>
#include <vtkMRMLVolumeDisplayNode.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>

// Qt includes
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QSettings>

// MRML widgets includes
#include "qMRMLNodeComboBox.h"
#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyColorLegendPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyColorLegendPlugin);
protected:
  qCjyxSubjectHierarchyColorLegendPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyColorLegendPluginPrivate(qCjyxSubjectHierarchyColorLegendPlugin& object);
  ~qCjyxSubjectHierarchyColorLegendPluginPrivate() override;
  void init();
public:

  QAction* ShowColorLegendAction;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyColorLegendPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyColorLegendPluginPrivate::qCjyxSubjectHierarchyColorLegendPluginPrivate(qCjyxSubjectHierarchyColorLegendPlugin& object)
  : q_ptr(&object)
  , ShowColorLegendAction(nullptr)
{
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyColorLegendPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyColorLegendPlugin);

  this->ShowColorLegendAction = new QAction("Show color legend",q);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->ShowColorLegendAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 10);
  QObject::connect(this->ShowColorLegendAction, SIGNAL(toggled(bool)), q, SLOT(toggleVisibilityForCurrentItem(bool)));
  this->ShowColorLegendAction->setCheckable(true);
  this->ShowColorLegendAction->setChecked(false);
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyColorLegendPluginPrivate::~qCjyxSubjectHierarchyColorLegendPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyColorLegendPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyColorLegendPlugin::qCjyxSubjectHierarchyColorLegendPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyColorLegendPluginPrivate(*this) )
{
  this->m_Name = QString("ColorLegend");

  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyColorLegendPlugin::~qCjyxSubjectHierarchyColorLegendPlugin() = default;

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyColorLegendPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyColorLegendPlugin);

  QList<QAction*> actions;
  actions << d->ShowColorLegendAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyColorLegendPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);

  if (!itemID)
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
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    // Not a displayable node color legend is not applicable
    return;
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode || !displayNode->GetColorNode())
    {
    // No color node for this node, color legend is not applicable
    return;
    }

  vtkMRMLColorLegendDisplayNode* colorLegendDisplayNode = vtkCjyxColorLogic::GetColorLegendDisplayNode(displayNode);
  QSignalBlocker blocker(d->ShowColorLegendAction);
  d->ShowColorLegendAction->setChecked(colorLegendDisplayNode && colorLegendDisplayNode->GetVisibility());
  d->ShowColorLegendAction->setVisible(true);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyColorLegendPlugin::toggleVisibilityForCurrentItem(bool on)
{
  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);
  vtkIdType itemID = qCjyxSubjectHierarchyPluginHandler::instance()->currentItem();
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
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!displayableNode)
    {
    // Not a displayable node color legend is not applicable
    return;
    }
  vtkMRMLDisplayNode* displayNode = displayableNode->GetDisplayNode();
  if (!displayNode || !displayNode->GetColorNode())
    {
    // No color node for this node, color legend is not applicable
    return;
    }

  vtkMRMLColorLegendDisplayNode* colorLegendDisplayNode = nullptr;
  if (on)
    {
    colorLegendDisplayNode = vtkCjyxColorLogic::AddDefaultColorLegendDisplayNode(displayNode);
    }
  else
    {
    colorLegendDisplayNode = vtkCjyxColorLogic::GetColorLegendDisplayNode(displayNode);
    }
  if (colorLegendDisplayNode)
    {
    colorLegendDisplayNode->SetVisibility(on);
    // If visibility is set to false then prevent making the node visible again on show.
    colorLegendDisplayNode->SetShowMode(on ? vtkMRMLDisplayNode::ShowDefault : vtkMRMLDisplayNode::ShowIgnore);
    }
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyColorLegendPlugin::showItemInView(vtkIdType itemID, vtkMRMLAbstractViewNode* viewNode, vtkIdList* allItemsToShow)
{
  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);

  vtkMRMLViewNode* threeDViewNode = vtkMRMLViewNode::SafeDownCast(viewNode);
  vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(viewNode);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }
  vtkMRMLScalarVolumeNode* volumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!volumeNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to find scalar volume node associated to subject hierarchy item " << itemID;
    return false;
    }

  bool wasVisible = false;
  vtkMRMLColorLegendDisplayNode* displayNode = vtkCjyxColorLogic::GetColorLegendDisplayNode(volumeNode);
  if (displayNode)
    {
    wasVisible = displayNode->GetVisibility();
    }
  else
    {
    // if there is no color legend node => create it, get first color legend node otherwise
    displayNode = vtkCjyxColorLogic::AddDefaultColorLegendDisplayNode(volumeNode);
    }
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to create color display node for scalar volume node " << volumeNode->GetName();
    return false;
    }

  if (viewNode)
    {
    // Show in specific view
    MRMLNodeModifyBlocker blocker(displayNode);
    // show
    if (!wasVisible)
      {
      displayNode->SetVisibility(true);
      }
    displayNode->AddViewNodeID(viewNode->GetID());
    }
  else if (sliceNode)
    {
    // Show in specific view
    MRMLNodeModifyBlocker blocker(displayNode);
    // show
    if (!wasVisible)
      {
      displayNode->SetVisibility(true);
      }
    displayNode->AddViewNodeID(sliceNode->GetID());
    }
  else
    {
    // Show in all views
    MRMLNodeModifyBlocker blocker(displayNode);
    displayNode->RemoveAllViewNodeIDs();
    displayNode->SetVisibility(true);
    }

  return true;
}

//---------------------------------------------------------------------------
bool qCjyxSubjectHierarchyColorLegendPlugin::showColorLegendInView( bool show, vtkIdType itemID, vtkMRMLViewNode* viewNode/*=nullptr*/)
{
  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }
  vtkMRMLScalarVolumeNode* volumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!volumeNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to find scalar volume node associated to subject hierarchy item " << itemID;
    return false;
    }

  bool wasVisible = false;
  vtkMRMLColorLegendDisplayNode* displayNode = vtkCjyxColorLogic::GetColorLegendDisplayNode(volumeNode);
  if (displayNode)
    {
    wasVisible = displayNode->GetVisibility();
    }
  else
    {
    // there is no color legend display node
    if (!show)
      {
      // not visible and should not be visible, so we are done
      return true;
      }
    // if there is no color legend node => create it, get first color legend node otherwise
    displayNode = vtkCjyxColorLogic::AddDefaultColorLegendDisplayNode(volumeNode);
    }
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to create color display node for scalar volume node " << volumeNode->GetName();
    return false;
    }

  if (viewNode)
    {
    // Show/hide in specific view
    MRMLNodeModifyBlocker blocker(displayNode);
    if (show)
      {
      // show
      if (!wasVisible)
        {
        displayNode->SetVisibility(true);
        // This was hidden in all views, show it only in the currently selected view
        displayNode->RemoveAllViewNodeIDs();
        }
      displayNode->AddViewNodeID(viewNode->GetID());
      }
    else
      {
      // This hides the volume rendering in all views, which is a bit more than asked for,
      // but since drag-and-drop to view only requires selective showing (and not selective hiding),
      // this should be good enough. The behavior can be refined later if needed.
      displayNode->SetVisibility(false);
      }
    }
  else
    {
    // Show in all views
    MRMLNodeModifyBlocker blocker(displayNode);
    displayNode->RemoveAllViewNodeIDs();
    displayNode->SetVisibility(show);
    }

  return true;
}

//---------------------------------------------------------------------------
bool qCjyxSubjectHierarchyColorLegendPlugin::showColorLegendInSlice( bool show, vtkIdType itemID, vtkMRMLSliceNode* sliceNode/*=nullptr*/)
{
  Q_D(qCjyxSubjectHierarchyColorLegendPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }
  vtkMRMLScalarVolumeNode* volumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  if (!volumeNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to find scalar volume node associated to subject hierarchy item " << itemID;
    return false;
    }

  bool wasVisible = false;
  vtkMRMLColorLegendDisplayNode* displayNode = vtkCjyxColorLogic::GetColorLegendDisplayNode(volumeNode);
  if (displayNode)
    {
    wasVisible = displayNode->GetVisibility();
    }
  else
    {
    // there is no color legend display node
    if (!show)
      {
      // not visible and should not be visible, so we are done
      return true;
      }
    // if there is no color legend node => create it, get first color legend node otherwise
    displayNode = vtkCjyxColorLogic::AddDefaultColorLegendDisplayNode(volumeNode);
    }
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to create color display node for scalar volume node " << volumeNode->GetName();
    return false;
    }

  if (sliceNode)
    {
    // Show/hide in specific view
    MRMLNodeModifyBlocker blocker(displayNode);
    if (show)
      {
      // show
      if (!wasVisible)
        {
        displayNode->SetVisibility(true);
        // This was hidden in all views, show it only in the currently selected view
        displayNode->RemoveAllViewNodeIDs();
        }
      displayNode->AddViewNodeID(sliceNode->GetID());
      }
    else
      {
      // This hides the volume rendering in all views, which is a bit more than asked for,
      // but since drag-and-drop to view only requires selective showing (and not selective hiding),
      // this should be good enough. The behavior can be refined later if needed.
      displayNode->SetVisibility(false);
      }
    }
  else
    {
    // Show in all views
    MRMLNodeModifyBlocker blocker(displayNode);
    displayNode->RemoveAllViewNodeIDs();
    displayNode->SetVisibility(show);
    }

  return true;
}
