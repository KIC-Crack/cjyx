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
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyOpacityPlugin.h"

// SubjectHierarchy logic includes
#include "vtkCjyxSubjectHierarchyModuleLogic.h"

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "vtkCjyxApplicationLogic.h"

// MRML includes
#include <vtkMRMLDisplayableNode.h>
#include <vtkMRMLDisplayNode.h>
#include "vtkMRMLScalarVolumeNode.h"

// CTK includes
#include "ctkDoubleSlider.h"

// Qt includes
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QWidgetAction>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyOpacityPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyOpacityPlugin);
protected:
  qCjyxSubjectHierarchyOpacityPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyOpacityPluginPrivate(qCjyxSubjectHierarchyOpacityPlugin& object);
  ~qCjyxSubjectHierarchyOpacityPluginPrivate() override;
  void init();
public:
  QAction* OpacityAction;
  QMenu* OpacityMenu;
  ctkDoubleSlider* OpacitySlider;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyOpacityPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyOpacityPluginPrivate::qCjyxSubjectHierarchyOpacityPluginPrivate(qCjyxSubjectHierarchyOpacityPlugin& object)
: q_ptr(&object)
{
  this->OpacityAction = nullptr;
  this->OpacityMenu = nullptr;
  this->OpacitySlider = nullptr;
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyOpacityPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyOpacityPlugin);

  this->OpacityMenu = new QMenu(tr("Opacity"));
  this->OpacitySlider = new ctkDoubleSlider(this->OpacityMenu);
  this->OpacitySlider->setOrientation(Qt::Horizontal);
  this->OpacitySlider->setRange(0.0, 1.0);
  this->OpacitySlider->setSingleStep(0.1);
  QObject::connect(this->OpacitySlider, SIGNAL(valueChanged(double)), q, SLOT(setOpacityForCurrentItem(double)));
  QWidgetAction* opacityAction = new QWidgetAction(this->OpacityMenu);
  opacityAction->setDefaultWidget(this->OpacitySlider);
  this->OpacityMenu->addAction(opacityAction);

  this->OpacityAction = new QAction("Opacity",q);
  this->OpacityAction->setToolTip("Set item opacity in the sub-menu");
  this->OpacityAction->setMenu(this->OpacityMenu);
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyOpacityPluginPrivate::~qCjyxSubjectHierarchyOpacityPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyOpacityPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyOpacityPlugin::qCjyxSubjectHierarchyOpacityPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyOpacityPluginPrivate(*this) )
{
  this->m_Name = QString("Opacity");

  Q_D(qCjyxSubjectHierarchyOpacityPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyOpacityPlugin::~qCjyxSubjectHierarchyOpacityPlugin() = default;

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyOpacityPlugin::visibilityContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyOpacityPlugin);

  QList<QAction*> actions;
  actions << d->OpacityAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyOpacityPlugin::showVisibilityContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyOpacityPlugin);

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

  // Show opacity for every non-scene items with display node
  vtkMRMLDisplayNode* displayNode = nullptr;
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(itemID));
  vtkMRMLScalarVolumeNode* volumeNode = vtkMRMLScalarVolumeNode::SafeDownCast(displayableNode);
  if (displayableNode)
    {
    displayNode = displayableNode->GetDisplayNode();
    }
  else
    {
    // Folder nodes may have display nodes directly associated
    displayNode = vtkMRMLDisplayNode::SafeDownCast(shNode->GetItemDataNode(itemID));
    }

  if (displayNode)
    {
    d->OpacitySlider->setValue(displayNode->GetOpacity());
    }

  // Show opacity action if there is a valid display node and if the node is not a volume
  d->OpacityAction->setVisible(displayNode && !volumeNode);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyOpacityPlugin::setOpacityForCurrentItem(double opacity)
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

  // Get display node
  vtkMRMLDisplayNode* displayNode = nullptr;
  vtkMRMLDisplayableNode* displayableNode = vtkMRMLDisplayableNode::SafeDownCast(shNode->GetItemDataNode(currentItemID));
  if (displayableNode)
    {
    displayNode = displayableNode->GetDisplayNode();
    }
  else
    {
    // Folder nodes may have display nodes directly associated
    displayNode = vtkMRMLDisplayNode::SafeDownCast(shNode->GetItemDataNode(currentItemID));
    }
  if (!displayNode)
    {
    qCritical() << Q_FUNC_INFO << ": Unable to find display node for subject hierarchy item " << shNode->GetItemName(currentItemID).c_str();
    return;
    }

  displayNode->SetOpacity(opacity);
}
