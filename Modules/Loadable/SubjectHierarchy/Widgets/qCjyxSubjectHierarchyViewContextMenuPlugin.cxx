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

// SubjectHierarchy MRML includes
#include "vtkMRMLSubjectHierarchyConstants.h"
#include "vtkMRMLSubjectHierarchyNode.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyViewContextMenuPlugin.h"
#include "qCjyxSubjectHierarchyDefaultPlugin.h"

// Qt includes
#include <QAction>
#include <QClipboard>
#include <QDebug>
#include <QVariant>

// MRML includes
#include <qMRMLSliceView.h>
#include <qMRMLSliceWidget.h>
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include <vtkMRMLAbstractViewNode.h>
#include <vtkMRMLInteractionNode.h>
#include <vtkMRMLLayoutNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLSliceNode.h>
#include <qMRMLThreeDViewControllerWidget.h>

// Cjyx includes
#include <qCjyxApplication.h>
#include <vtkCjyxApplicationLogic.h>
#include <qCjyxLayoutManager.h>

// VTK includes
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkMRMLCameraDisplayableManager.h>
#include <vtkMRMLCameraWidget.h>

// CTK includes
#include "ctkSignalMapper.h"
#include "ctkVTKWidgetsUtils.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class qCjyxSubjectHierarchyViewContextMenuPluginPrivate: public QObject
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyViewContextMenuPlugin);
protected:
  qCjyxSubjectHierarchyViewContextMenuPlugin* const q_ptr;
public:
  qCjyxSubjectHierarchyViewContextMenuPluginPrivate(qCjyxSubjectHierarchyViewContextMenuPlugin& object);
  ~qCjyxSubjectHierarchyViewContextMenuPluginPrivate() override;
  void init();
public:

  ctkSignalMapper* InteractionModeMapper;
  QAction* InteractionModeViewTransformAction = nullptr;
  QAction* InteractionModeAdjustWindowLevelAction = nullptr;
  QAction* InteractionModePlaceAction = nullptr;

  QAction* MaximizeViewAction = nullptr;
  QAction* FitSliceViewAction = nullptr;
  QAction* CenterThreeDViewAction = nullptr;
  QAction* CopyImageAction = nullptr;
  QAction* ConfigureSliceViewAnnotationsAction = nullptr;
  QAction* ToggleTiltLockAction = nullptr;

  QAction* IntersectingSlicesVisibilityAction = nullptr;
  QAction* IntersectingSlicesInteractiveAction = nullptr;

  vtkWeakPointer<vtkMRMLInteractionNode> InteractionNode;
  vtkWeakPointer<vtkMRMLAbstractViewNode> ViewNode;
  vtkWeakPointer<vtkMRMLLayoutNode> LayoutNode;
  vtkWeakPointer<vtkMRMLCameraWidget> CameraWidget;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyViewContextMenuPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyViewContextMenuPluginPrivate::qCjyxSubjectHierarchyViewContextMenuPluginPrivate(qCjyxSubjectHierarchyViewContextMenuPlugin& object)
: q_ptr(&object)
{
}

//------------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPluginPrivate::init()
{
  Q_Q(qCjyxSubjectHierarchyViewContextMenuPlugin);

  // Interaction mode

  this->InteractionModeViewTransformAction = new QAction("View transform",q);
  this->InteractionModeViewTransformAction->setObjectName("MouseModeViewTransformAction");
  this->InteractionModeViewTransformAction->setCheckable(true);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->InteractionModeViewTransformAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionInteraction, 0);

  this->InteractionModeAdjustWindowLevelAction = new QAction("Adjust window/level",q);
  this->InteractionModeAdjustWindowLevelAction->setObjectName("MouseModeAdjustWindowLevelAction");
  this->InteractionModeAdjustWindowLevelAction->setCheckable(true);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->InteractionModeAdjustWindowLevelAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionInteraction, 1);

  this->InteractionModePlaceAction = new QAction("Place", q);
  this->InteractionModePlaceAction->setObjectName("MouseModePlaceAction");
  this->InteractionModePlaceAction->setCheckable(true);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->InteractionModePlaceAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionInteraction, 2);

  QActionGroup* interactionModeActions = new QActionGroup(q);
  interactionModeActions->setExclusive(true);

  interactionModeActions->addAction(this->InteractionModeViewTransformAction);
  interactionModeActions->addAction(this->InteractionModeAdjustWindowLevelAction);
  interactionModeActions->addAction(this->InteractionModePlaceAction);

  this->InteractionModeMapper = new ctkSignalMapper(q);
  this->InteractionModeMapper->setMapping(this->InteractionModeViewTransformAction, vtkMRMLInteractionNode::ViewTransform);
  this->InteractionModeMapper->setMapping(this->InteractionModeAdjustWindowLevelAction, vtkMRMLInteractionNode::AdjustWindowLevel);
  this->InteractionModeMapper->setMapping(this->InteractionModePlaceAction, vtkMRMLInteractionNode::Place);
  QObject::connect(interactionModeActions, SIGNAL(triggered(QAction*)), this->InteractionModeMapper, SLOT(map(QAction*)));
  QObject::connect(this->InteractionModeMapper, SIGNAL(mapped(int)), q, SLOT(setInteractionMode(int)));

  // Other

  this->CenterThreeDViewAction = new QAction(tr("Center view"), q);
  this->CenterThreeDViewAction->setObjectName("CenterViewAction");
  this->CenterThreeDViewAction->setToolTip(tr("Center the slice on the currently visible 3D view content and all loaded volumes."));
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->CenterThreeDViewAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 0);
  QObject::connect(this->CenterThreeDViewAction, SIGNAL(triggered()), q, SLOT(centerThreeDView()));

  this->FitSliceViewAction = new QAction(tr("Reset field of view"), q);
  this->FitSliceViewAction->setObjectName("FitViewAction");
  this->FitSliceViewAction->setToolTip(tr("Center the slice view on the currently displayed volume."));
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->FitSliceViewAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 1);
  QObject::connect(this->FitSliceViewAction, SIGNAL(triggered()), q, SLOT(fitSliceView()));

  this->MaximizeViewAction = new QAction(tr("Maximize view"), q);
  this->MaximizeViewAction->setObjectName("MaximizeViewAction");
  this->MaximizeViewAction->setToolTip(tr("Show this view maximized in the view layout"));
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->MaximizeViewAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 2);
  QObject::connect(this->MaximizeViewAction, SIGNAL(triggered()), q, SLOT(maximizeView()));

  this->ToggleTiltLockAction = new QAction(tr("Tilt lock"), q);
  this->ToggleTiltLockAction->setObjectName("TiltLockAction");
  this->ToggleTiltLockAction->setToolTip(tr("Prevent rotation around the horizontal axis when rotating this view."));
  this->ToggleTiltLockAction->setShortcut(QKeySequence(tr("Ctrl+b")));
  this->ToggleTiltLockAction->setCheckable(true);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->ToggleTiltLockAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 3);
  QObject::connect(this->ToggleTiltLockAction, SIGNAL(triggered()), q, SLOT(toggleTiltLock()));

  this->ConfigureSliceViewAnnotationsAction = new QAction(tr("Configure slice view annotations..."), q);
  this->ConfigureSliceViewAnnotationsAction->setObjectName("ConfigureSliceViewAnnotationsAction");
  this->ConfigureSliceViewAnnotationsAction->setToolTip(tr("Configures display of corner annotations and color legend."));
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->ConfigureSliceViewAnnotationsAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 4);
  QObject::connect(this->ConfigureSliceViewAnnotationsAction, SIGNAL(triggered()), q, SLOT(configureSliceViewAnnotationsAction()));

  this->CopyImageAction = new QAction(tr("Copy image"), q);
  this->CopyImageAction->setObjectName("CopyImageAction");
  this->CopyImageAction->setToolTip(tr("Copy a screenshot of this view to the clipboard"));
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->CopyImageAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault, 20); // set to 20 to make it the last item in the action group
  QObject::connect(this->CopyImageAction, SIGNAL(triggered()), q, SLOT(saveScreenshot()));

  // Slice intersections
  this->IntersectingSlicesVisibilityAction = new QAction(tr("Slice intersections"), q);
  this->IntersectingSlicesVisibilityAction->setObjectName("IntersectingSlicesAction");
  this->IntersectingSlicesVisibilityAction->setToolTip(tr("Show how the other slice planes intersect each slice plane."));
  this->IntersectingSlicesVisibilityAction->setCheckable(true);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->IntersectingSlicesVisibilityAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault + 5); // set section to +5 to allow placing other sections above
  QObject::connect(this->IntersectingSlicesVisibilityAction, SIGNAL(triggered(bool)),
    q, SLOT(setIntersectingSlicesVisible(bool)));

  // Interactive slice intersections
  this->IntersectingSlicesInteractiveAction = new QAction(tr("Interaction"), q);
  this->IntersectingSlicesInteractiveAction->setObjectName("IntersectingSlicesHandlesAction");
  this->IntersectingSlicesInteractiveAction->setToolTip(tr("Show handles for slice interaction."));
  this->IntersectingSlicesInteractiveAction->setCheckable(true);
  this->IntersectingSlicesInteractiveAction->setEnabled(false);
  qCjyxSubjectHierarchyAbstractPlugin::setActionPosition(this->IntersectingSlicesInteractiveAction,
    qCjyxSubjectHierarchyAbstractPlugin::SectionDefault+5); // set section to +5 to allow placing other sections above
  QObject::connect(this->IntersectingSlicesInteractiveAction, SIGNAL(triggered(bool)),
    q, SLOT(setIntersectingSlicesHandlesVisible(bool)));
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyViewContextMenuPluginPrivate::~qCjyxSubjectHierarchyViewContextMenuPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyViewContextMenuPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyViewContextMenuPlugin::qCjyxSubjectHierarchyViewContextMenuPlugin(QObject* parent)
 : Superclass(parent)
 , d_ptr( new qCjyxSubjectHierarchyViewContextMenuPluginPrivate(*this) )
{
  this->m_Name = QString("ViewContextMenu");

  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyViewContextMenuPlugin::~qCjyxSubjectHierarchyViewContextMenuPlugin() = default;

//---------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyViewContextMenuPlugin::viewContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyViewContextMenuPlugin);
  QList<QAction*> actions;
  actions << d->InteractionModeViewTransformAction
    << d->InteractionModeAdjustWindowLevelAction
    << d->InteractionModePlaceAction
    << d->MaximizeViewAction
    << d->FitSliceViewAction
    << d->CenterThreeDViewAction
    << d->CopyImageAction
    << d->ToggleTiltLockAction
    << d->ConfigureSliceViewAnnotationsAction
    << d->IntersectingSlicesVisibilityAction
    << d->IntersectingSlicesInteractiveAction;
  return actions;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::showViewContextMenuActionsForItem(vtkIdType itemID, QVariantMap eventData)
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);

  vtkMRMLSubjectHierarchyNode* shNode = qCjyxSubjectHierarchyPluginHandler::instance()->subjectHierarchyNode();
  if (!shNode || !shNode->GetScene())
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return;
    }
  if (itemID != shNode->GetSceneItemID())
    {
    return;
    }
  if (!eventData.contains("ViewNodeID"))
    {
    return;
    }
  vtkMRMLAbstractViewNode* viewNode = vtkMRMLAbstractViewNode::SafeDownCast(
    shNode->GetScene()->GetNodeByID(eventData["ViewNodeID"].toString().toStdString()));
  if (!viewNode)
    {
    return;
    }
  vtkMRMLInteractionNode* interactionNode = viewNode->GetInteractionNode();
  if (!interactionNode)
    {
    return;
    }

  d->InteractionModeViewTransformAction->setVisible(true);
  d->InteractionModeAdjustWindowLevelAction->setVisible(true);
  d->InteractionModePlaceAction->setVisible(true);

  int interactionMode = interactionNode->GetCurrentInteractionMode();

  bool wasBlocked = d->InteractionModeViewTransformAction->blockSignals(true);
  d->InteractionModeViewTransformAction->setChecked(interactionMode == vtkMRMLInteractionNode::ViewTransform);
  d->InteractionModeViewTransformAction->blockSignals(wasBlocked);

  wasBlocked = d->InteractionModeAdjustWindowLevelAction->blockSignals(true);
  d->InteractionModeAdjustWindowLevelAction->setChecked(interactionMode == vtkMRMLInteractionNode::AdjustWindowLevel);
  d->InteractionModeAdjustWindowLevelAction->blockSignals(wasBlocked);

  wasBlocked = d->InteractionModePlaceAction->blockSignals(true);
  d->InteractionModePlaceAction->setChecked(interactionMode == vtkMRMLInteractionNode::Place);
  d->InteractionModePlaceAction->blockSignals(wasBlocked);

  // Update view/restore view action
  bool isMaximized = false;
  bool canBeMaximized = false;
  d->LayoutNode = viewNode->GetMaximizedState(isMaximized, canBeMaximized);
  d->MaximizeViewAction->setVisible(canBeMaximized);
  if (canBeMaximized)
    {
    d->MaximizeViewAction->setProperty("maximize", QVariant(!isMaximized));
    if (isMaximized)
      {
      d->MaximizeViewAction->setText(tr("Restore view layout"));
      }
    else
      {
      d->MaximizeViewAction->setText(tr("Maximize view"));
      }
    }

  d->CopyImageAction->setVisible(true);

  // Cache nodes to have them available for the menu action execution.
  d->InteractionNode = interactionNode;
  d->ViewNode = viewNode;

  // Check tilt lock in camera widget and set menu item accordingly
  bool isSliceViewNode = (vtkMRMLSliceNode::SafeDownCast(viewNode) != nullptr);
  d->ConfigureSliceViewAnnotationsAction->setVisible(isSliceViewNode);
  d->FitSliceViewAction->setVisible(isSliceViewNode);
  d->CenterThreeDViewAction->setVisible(!isSliceViewNode);

  vtkCjyxApplicationLogic* appLogic = qCjyxApplication::application()->applicationLogic();
  if (isSliceViewNode && appLogic)
    {
    d->IntersectingSlicesVisibilityAction->setVisible(true);
    d->IntersectingSlicesVisibilityAction->setEnabled(true);
    d->IntersectingSlicesVisibilityAction->setChecked(appLogic->GetIntersectingSlicesEnabled(vtkMRMLApplicationLogic::IntersectingSlicesVisibility));

    d->IntersectingSlicesInteractiveAction->setVisible(true);
    d->IntersectingSlicesInteractiveAction->setEnabled(d->IntersectingSlicesVisibilityAction->isChecked());
    d->IntersectingSlicesInteractiveAction->setChecked(appLogic->GetIntersectingSlicesEnabled(vtkMRMLApplicationLogic::IntersectingSlicesInteractive));
    }
  else
    {
    d->IntersectingSlicesVisibilityAction->setVisible(false);
    d->IntersectingSlicesInteractiveAction->setVisible(false);
    }

  d->ToggleTiltLockAction->setVisible(!isSliceViewNode);
  if (!qCjyxApplication::application()
    || !qCjyxApplication::application()->layoutManager())
    {
    qWarning() << Q_FUNC_INFO << " failed: cannot get layout manager";
    return;
    }
  QWidget* widget = qCjyxApplication::application()->layoutManager()->viewWidget(d->ViewNode);

  qMRMLThreeDWidget* threeDWidget = qobject_cast<qMRMLThreeDWidget*>(widget);
  vtkMRMLCameraWidget* cameraWidget = nullptr;
  if (threeDWidget)
    {
    vtkMRMLCameraDisplayableManager* cameraDisplayableManager = vtkMRMLCameraDisplayableManager::SafeDownCast(threeDWidget->
      threeDView()->displayableManagerByClassName("vtkMRMLCameraDisplayableManager"));
    if (!cameraDisplayableManager)
      {
      qWarning() << Q_FUNC_INFO << " failed: cannot get cameraDisplayableManager";
      return;
      }
    else
      {
      cameraWidget = cameraDisplayableManager->GetCameraWidget();
      d->ToggleTiltLockAction->setChecked(cameraWidget->GetTiltLocked());
      // Cache camera widget pointer to have it available for the menu action execution.
      d->CameraWidget = cameraWidget;
      }
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::setInteractionMode(int mode)
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  if (!d->InteractionNode)
    {
    qWarning() << Q_FUNC_INFO << " failed: invalid interaction node";
    return;
    }
  d->InteractionNode->SetCurrentInteractionMode(mode);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::saveScreenshot()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  if (!qCjyxApplication::application()
    || !qCjyxApplication::application()->layoutManager())
  {
    qWarning() << Q_FUNC_INFO << " failed: cannot get layout manager";
    return;
  }
  QWidget* widget = qCjyxApplication::application()->layoutManager()->viewWidget(d->ViewNode);

  // Get the inside of the widget (without the view controller bar)
  qMRMLSliceWidget* sliceWidget = qobject_cast<qMRMLSliceWidget*>(widget);
  qMRMLThreeDWidget* threeDWidget = qobject_cast<qMRMLThreeDWidget*>(widget);
  if (sliceWidget)
    {
    widget = sliceWidget->sliceView();
    }
  else if (threeDWidget)
    {
    widget = threeDWidget->threeDView();
    }
  if (!widget)
    {
    qWarning() << Q_FUNC_INFO << " failed: cannot get view widget from layout manager";
    return;
    }

  // Grab image
  QImage screenshot = ctk::grabVTKWidget(widget);

  // Copy to clipboard
  QClipboard* clipboard = QApplication::clipboard();
  if (!clipboard)
    {
    qWarning() << Q_FUNC_INFO << " failed: cannot access the clipboard";
    return;
    }
  clipboard->setImage(screenshot);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::configureSliceViewAnnotationsAction()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  qCjyxLayoutManager* layoutManager = qCjyxApplication::application()->layoutManager();
  if (!layoutManager)
    {
    return;
    }
  layoutManager->setCurrentModule("DataProbe");
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::maximizeView()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  if (!d->LayoutNode)
    {
    return;
    }
  bool maximizeView = d->MaximizeViewAction->property("maximize").toBool();
  if (maximizeView)
    {
    d->LayoutNode->SetMaximizedViewNode(d->ViewNode);
    }
  else
    {
    d->LayoutNode->SetMaximizedViewNode(nullptr);
    }
}
//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::fitSliceView()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);

  if (!qCjyxApplication::application()
    || !qCjyxApplication::application()->layoutManager())
    {
    qWarning() << Q_FUNC_INFO << " failed: cannot get layout manager";
    return;
    }
  QWidget* widget = qCjyxApplication::application()->layoutManager()->viewWidget(d->ViewNode);

  qMRMLSliceWidget* sliceWidget = qobject_cast<qMRMLSliceWidget*>(widget);
  if (sliceWidget)
    {
    sliceWidget->fitSliceToBackground();
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: sliceWidget not found";
    return;
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::centerThreeDView()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);

  if (!qCjyxApplication::application()
    || !qCjyxApplication::application()->layoutManager())
    {
    qWarning() << Q_FUNC_INFO << " failed: cannot get layout manager";
    return;
    }
  QWidget* widget = qCjyxApplication::application()->layoutManager()->viewWidget(d->ViewNode);

  qMRMLThreeDWidget* threeDWidget = qobject_cast<qMRMLThreeDWidget*>(widget);
  if (threeDWidget)
    {
    qMRMLThreeDViewControllerWidget* threeDWidgetController = threeDWidget->threeDController();
    threeDWidgetController->resetFocalPoint();
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: threeDWidget not found";
    return;
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::toggleTiltLock()
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  if (!d->CameraWidget)
    {
    qWarning() << Q_FUNC_INFO << " failed: camera widget not found.";
    return;
    }
  d->CameraWidget->SetTiltLocked(!d->CameraWidget->GetTiltLocked());
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::setIntersectingSlicesVisible(bool visible)
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  vtkCjyxApplicationLogic* appLogic = qCjyxApplication::application()->applicationLogic();
  if (!appLogic)
    {
    qCritical() << Q_FUNC_INFO << " failed: cannot get application logic";
    return;
    }
  appLogic->SetIntersectingSlicesEnabled(vtkMRMLApplicationLogic::IntersectingSlicesVisibility, visible);
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyViewContextMenuPlugin::setIntersectingSlicesHandlesVisible(bool interaction)
{
  Q_D(qCjyxSubjectHierarchyViewContextMenuPlugin);
  vtkCjyxApplicationLogic* appLogic = qCjyxApplication::application()->applicationLogic();
  if (!appLogic)
    {
    qCritical() << Q_FUNC_INFO << " failed: cannot get application logic";
    return;
    }
  appLogic->SetIntersectingSlicesEnabled(vtkMRMLApplicationLogic::IntersectingSlicesInteractive, interaction);
}
