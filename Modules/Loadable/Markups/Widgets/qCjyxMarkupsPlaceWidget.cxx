/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Matthew Holden, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Markups Widgets includes
#include "qCjyxMarkupsPlaceWidget.h"

// Markups includes
#include <vtkCjyxMarkupsLogic.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxModuleManager.h"
#include "qCjyxAbstractCoreModule.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkMRMLInteractionNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>

// Qt includes
#include <QColor>
#include <QDebug>
#include <QList>
#include <QMenu>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CreateModels
class qCjyxMarkupsPlaceWidgetPrivate
  : public Ui_qCjyxMarkupsPlaceWidget
{
  Q_DECLARE_PUBLIC(qCjyxMarkupsPlaceWidget);
protected:
  qCjyxMarkupsPlaceWidget* const q_ptr;

public:
  qCjyxMarkupsPlaceWidgetPrivate( qCjyxMarkupsPlaceWidget& object);
  ~qCjyxMarkupsPlaceWidgetPrivate();
  virtual void setupUi(qCjyxMarkupsPlaceWidget*);

public:
  vtkWeakPointer<vtkCjyxMarkupsLogic> MarkupsLogic;
  vtkWeakPointer<vtkMRMLMarkupsNode> CurrentMarkupsNode;
  vtkWeakPointer<vtkMRMLSelectionNode> SelectionNode;
  vtkWeakPointer<vtkMRMLInteractionNode> InteractionNode;
  QMenu* PlaceMenu;
  QMenu* DeleteMenu;
  qCjyxMarkupsPlaceWidget::PlaceMultipleMarkupsType PlaceMultipleMarkups;
  QList < QWidget* > OptionsWidgets;
  QColor DefaultNodeColor;
  bool DeleteMarkupsButtonVisible;
  bool DeleteAllControlPointsOptionVisible;
  bool UnsetLastControlPointOptionVisible;
  bool UnsetAllControlPointsOptionVisible;
  bool LastSignaledPlaceModeEnabled; // if placeModeEnabled changes compared to this value then a activeMarkupsPlaceModeChanged signal will be emitted
  bool IsUpdatingWidgetFromMRML;

};

// --------------------------------------------------------------------------
qCjyxMarkupsPlaceWidgetPrivate::qCjyxMarkupsPlaceWidgetPrivate( qCjyxMarkupsPlaceWidget& object)
  : q_ptr(&object)
{
  this->DeleteMarkupsButtonVisible = true;
  this->DeleteAllControlPointsOptionVisible = true;
  this->UnsetLastControlPointOptionVisible = false;
  this->UnsetAllControlPointsOptionVisible = false;
  this->PlaceMultipleMarkups = qCjyxMarkupsPlaceWidget::ShowPlaceMultipleMarkupsOption;
  this->PlaceMenu = nullptr;
  this->DeleteMenu = nullptr;
  this->LastSignaledPlaceModeEnabled = false;
  this->IsUpdatingWidgetFromMRML = false;
}

//-----------------------------------------------------------------------------
qCjyxMarkupsPlaceWidgetPrivate::~qCjyxMarkupsPlaceWidgetPrivate() = default;

// --------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidgetPrivate::setupUi(qCjyxMarkupsPlaceWidget* widget)
{
  this->Ui_qCjyxMarkupsPlaceWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qCjyxMarkupsPlaceWidget methods

//-----------------------------------------------------------------------------
qCjyxMarkupsPlaceWidget::qCjyxMarkupsPlaceWidget(QWidget* parentWidget) : Superclass( parentWidget ) , d_ptr( new qCjyxMarkupsPlaceWidgetPrivate(*this) )
{
  this->setup();
}

//-----------------------------------------------------------------------------
qCjyxMarkupsPlaceWidget::~qCjyxMarkupsPlaceWidget()
{
  this->setCurrentNode(nullptr);
  this->setInteractionNode(nullptr);
  this->setSelectionNode(nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setup()
{
  Q_D(qCjyxMarkupsPlaceWidget);

  d->MarkupsLogic = vtkCjyxMarkupsLogic::SafeDownCast(this->moduleLogic("Markups"));
  if (!d->MarkupsLogic)
    {
    qCritical() << Q_FUNC_INFO << ": Markups module is not found, some markup manipulation features will not be available";
    }

  d->setupUi(this);

  d->OptionsWidgets << d->ColorButton << d->PlaceButton << d->DeleteButton << d->MoreButton;

  d->DefaultNodeColor.setRgb(0.0,1.0,0.0); // displayed when no node is selected
  // Use the pressed signal (otherwise we can unpress buttons without clicking them)
  connect( d->ColorButton, SIGNAL( colorChanged( QColor ) ), this, SLOT( onColorButtonChanged( QColor ) ) );

  d->PlaceMenu = new QMenu(tr("Place options"), this);
  d->PlaceMenu->setObjectName("PlaceMenu");
  d->PlaceMenu->addAction(d->ActionPlacePersistentPoint);
  QObject::connect(d->ActionPlacePersistentPoint, SIGNAL(toggled(bool)), this, SLOT(onPlacePersistentPoint(bool)));
  QObject::connect(d->PlaceButton, SIGNAL(toggled(bool)), this, SLOT(setPlaceModeEnabled(bool)));

  if (d->PlaceMultipleMarkups == ShowPlaceMultipleMarkupsOption)
    {
    d->PlaceButton->setMenu(d->PlaceMenu);
    }

  d->DeleteMenu = new QMenu(tr("Delete options"), d->DeleteButton);
  d->DeleteMenu->setObjectName("DeleteMenu");
  d->DeleteMenu->addAction(d->ActionUnsetLast);
  QObject::connect(d->ActionUnsetLast, SIGNAL(triggered()), this, SLOT(unsetLastDefinedPoint()));
  d->DeleteMenu->addAction(d->ActionUnsetAll);
  QObject::connect(d->ActionUnsetAll, SIGNAL(triggered()), this, SLOT(unsetAllPoints()));
  d->DeleteMenu->addAction(d->ActionDeleteAll);
  QObject::connect(d->ActionDeleteAll, SIGNAL(triggered()), this, SLOT(deleteAllPoints()));

  d->ActionDeleteAll->setVisible(d->DeleteAllControlPointsOptionVisible);
  d->ActionUnsetLast->setVisible(d->UnsetLastControlPointOptionVisible);
  d->ActionUnsetAll->setVisible(d->UnsetAllControlPointsOptionVisible);
  updateDeleteButton();

  d->DeleteButton->setVisible(d->DeleteMarkupsButtonVisible);
  connect( d->DeleteButton, SIGNAL(clicked()), this, SLOT(modifyLastPoint()) );

  QMenu* moreMenu = new QMenu(tr("More options"), d->MoreButton);
  moreMenu->setObjectName("moreMenu");
  moreMenu->addAction(d->ActionVisibility);
  moreMenu->addAction(d->ActionLocked);
  moreMenu->addAction(d->ActionFixedNumberOfControlPoints);
  QObject::connect(d->ActionVisibility, SIGNAL(triggered()), this, SLOT(onVisibilityButtonClicked()));
  QObject::connect(d->ActionLocked, SIGNAL(triggered()), this, SLOT(onLockedButtonClicked()));
  QObject::connect(d->ActionFixedNumberOfControlPoints, SIGNAL(triggered()), this, SLOT(onFixedNumberOfControlPointsButtonClicked()));
  d->MoreButton->setMenu(moreMenu);

  updateWidget();
}

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxMarkupsPlaceWidget::currentNode() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->CurrentMarkupsNode;
}

//-----------------------------------------------------------------------------
vtkMRMLMarkupsFiducialNode* qCjyxMarkupsPlaceWidget::currentMarkupsFiducialNode() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return vtkMRMLMarkupsFiducialNode::SafeDownCast(d->CurrentMarkupsNode);
}

//-----------------------------------------------------------------------------
vtkMRMLMarkupsNode* qCjyxMarkupsPlaceWidget::currentMarkupsNode() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->CurrentMarkupsNode;
}

//-----------------------------------------------------------------------------
vtkMRMLSelectionNode* qCjyxMarkupsPlaceWidget::selectionNode()const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->SelectionNode;
}

//-----------------------------------------------------------------------------
vtkMRMLInteractionNode* qCjyxMarkupsPlaceWidget::interactionNode()const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->InteractionNode;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setInteractionNode(vtkMRMLInteractionNode* interactionNode)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  if (d->InteractionNode == interactionNode)
    {
    return;
    }
  this->qvtkReconnect(d->InteractionNode, interactionNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidget()));
  d->InteractionNode = interactionNode;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setSelectionNode(vtkMRMLSelectionNode* selectionNode)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  if (d->SelectionNode == selectionNode)
    {
    return;
    }

  this->qvtkReconnect(d->SelectionNode, selectionNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->SelectionNode, selectionNode, vtkMRMLSelectionNode::ActivePlaceNodePlacementValidEvent, this, SLOT(updateWidget()));
  d->SelectionNode = selectionNode;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setCurrentNode(vtkMRMLNode* currentNode)
{
  Q_D(qCjyxMarkupsPlaceWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast(currentNode);
  if (currentMarkupsNode==d->CurrentMarkupsNode)
    {
    // not changed
    return;
    }

  // Reconnect the appropriate nodes
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointAddedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointRemovedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::FixedNumberOfControlPointsModifiedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLDisplayableNode::DisplayModifiedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointPositionDefinedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointPositionUndefinedEvent, this, SLOT(updateWidget()));
  d->CurrentMarkupsNode = currentMarkupsNode;

  this->updateWidget();
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::modifyLastPoint()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr)
    {
    return;
    }
  if (currentMarkupsNode->GetNumberOfControlPoints() < 1)
    {
    return;
    }
  if (currentMarkupsNode->GetFixedNumberOfControlPoints())
    {
    this->unsetLastDefinedPoint();
    }
  else
    {
    this->deleteLastPoint();
    }
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::deleteLastPoint()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if ( currentMarkupsNode == nullptr )
    {
    return;
    }
  if (currentMarkupsNode->GetNumberOfControlPoints() < 1)
    {
    return;
    }
  currentMarkupsNode->RemoveNthControlPoint(currentMarkupsNode->GetNumberOfControlPoints() - 1);
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::unsetLastDefinedPoint()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr)
    {
    return;
    }
  if (currentMarkupsNode->GetNumberOfControlPoints() < 1)
    {
    return;
    }

  int pointNumber = currentMarkupsNode->GetNumberOfControlPoints();
  for (int index = pointNumber-1; index >= 0 ; index--)
    {
    if (currentMarkupsNode->GetNthControlPointPositionStatus(index) != vtkMRMLMarkupsNode::PositionUndefined)
      {
      currentMarkupsNode->UnsetNthControlPointPosition(index);
      return;
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::deleteAllPoints()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if ( currentMarkupsNode == nullptr )
    {
    return;
    }

  currentMarkupsNode->RemoveAllControlPoints();
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::unsetAllPoints()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr)
    {
    return;
    }
  currentMarkupsNode->UnsetAllControlPoints();
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::currentNodeActive() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (d->MarkupsLogic == nullptr || this->mrmlScene() == nullptr ||
      currentMarkupsNode == nullptr || d->InteractionNode == nullptr ||
      d->SelectionNode == nullptr)
    {
    return false;
    }
  bool currentNodeActive = (d->MarkupsLogic->GetActiveListID().compare( currentMarkupsNode->GetID() ) == 0);
  const char* activePlaceNodeClassName = d->SelectionNode->GetActivePlaceNodeClassName();
  bool placeNodeClassNameMatches = activePlaceNodeClassName && std::string(activePlaceNodeClassName).compare(currentMarkupsNode->GetClassName())==0;
  return placeNodeClassNameMatches && currentNodeActive;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setCurrentNodeActive(bool active)
{
  Q_D(qCjyxMarkupsPlaceWidget);

  if (d->MarkupsLogic == nullptr || this->mrmlScene() == nullptr || d->InteractionNode == nullptr)
    {
    if (active)
      {
      qCritical() << Q_FUNC_INFO << " failed: Markups module logic, scene, or interaction node is invalid";
      }
    return;
    }
  bool wasActive = this->currentNodeActive();
  if (wasActive!=active)
    {
    if (active)
      {
      d->MarkupsLogic->SetActiveList(this->currentMarkupsNode());
      }
    else
      {
      d->MarkupsLogic->SetActiveList(nullptr);
      d->InteractionNode->SetCurrentInteractionMode( vtkMRMLInteractionNode::ViewTransform );
      }
    }
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::placeModeEnabled() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  if (!this->currentNodeActive())
    {
    return false;
    }
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (d->SelectionNode == nullptr || d->InteractionNode == nullptr || this->mrmlScene() == nullptr || currentMarkupsNode == nullptr)
    {
    return false;
    }
  bool placeMode = d->InteractionNode->GetCurrentInteractionMode() == vtkMRMLInteractionNode::Place;
  const char* activePlaceNodeClassName = d->SelectionNode->GetActivePlaceNodeClassName();
  bool placeNodeClassNameMatches = activePlaceNodeClassName && std::string(activePlaceNodeClassName).compare(currentMarkupsNode->GetClassName()) == 0;
  return placeMode && placeNodeClassNameMatches;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setPlaceModeEnabled(bool placeEnable)
{
  Q_D(qCjyxMarkupsPlaceWidget);

  if (d->MarkupsLogic == nullptr || this->mrmlScene() == nullptr ||
      d->InteractionNode == nullptr || this->currentMarkupsNode() == nullptr)
    {
    if (placeEnable)
      {
      qCritical() << Q_FUNC_INFO << " activate failed: Markups module logic, scene, or interaction node is invalid";
      }
    return;
    }
  bool wasActive = this->currentNodeActive();
  if ( placeEnable )
    {
    // activate and set place mode
    if (!wasActive)
      {
      d->MarkupsLogic->SetActiveList(this->currentMarkupsNode());
      }
    if (d->PlaceMultipleMarkups == ForcePlaceSingleMarkup)
      {
      setPlaceModePersistency(false);
      }
    else if (d->PlaceMultipleMarkups == ForcePlaceMultipleMarkups)
      {
      setPlaceModePersistency(true);
      }
    d->InteractionNode->SetCurrentInteractionMode( vtkMRMLInteractionNode::Place );
    }
  else
    {
    // disable place mode
    d->InteractionNode->SetCurrentInteractionMode( vtkMRMLInteractionNode::ViewTransform );
    }
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::placeModePersistency() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  if (d->InteractionNode == nullptr)
    {
    qCritical() << Q_FUNC_INFO << " failed: interactionNode is invalid";
    return false;
    }
  return d->InteractionNode->GetPlaceModePersistence();
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setPlaceModePersistency(bool persistent)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  if (d->InteractionNode == nullptr)
    {
    qCritical() << Q_FUNC_INFO << " failed: interactionNode is invalid";
    return;
    }
  d->InteractionNode->SetPlaceModePersistence(persistent);
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::updateWidget()
  {
  Q_D(qCjyxMarkupsPlaceWidget);

  if (d->IsUpdatingWidgetFromMRML)
    {
    // Updating widget from MRML is already in progress
    return;
    }
  d->IsUpdatingWidgetFromMRML = true;
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();

  if (d->MarkupsLogic == nullptr || this->mrmlScene() == nullptr ||
    d->InteractionNode == nullptr || currentMarkupsNode == nullptr)
    {
    d->ColorButton->setEnabled(false);
    d->PlaceButton->setEnabled(false);
    d->DeleteButton->setEnabled(false);
    d->MoreButton->setEnabled(false);
    bool wasBlockedColorButton = d->ColorButton->blockSignals(true);
    d->ColorButton->setColor(d->DefaultNodeColor);
    d->ColorButton->blockSignals(wasBlockedColorButton);
    if (d->LastSignaledPlaceModeEnabled)
      {
      emit activeMarkupsFiducialPlaceModeChanged(false);
      emit activeMarkupsPlaceModeChanged(false);
      d->LastSignaledPlaceModeEnabled = false;
      }
    d->IsUpdatingWidgetFromMRML = false;
    return;
    }

  bool activePlaceNodePlacementValid = !currentMarkupsNode->GetControlPointPlacementComplete();
  d->PlaceButton->setEnabled(activePlaceNodePlacementValid);

  d->ColorButton->setEnabled(true);
  d->DeleteButton->setEnabled(currentMarkupsNode->GetNumberOfControlPoints() > 0);
  d->MoreButton->setEnabled(true);

  // Set the button indicating if this list is active
  bool wasBlockedColorButton = d->ColorButton->blockSignals( true );
  bool wasBlockedVisibilityButton = d->ActionVisibility->blockSignals( true );
  bool wasBlockedLockButton = d->ActionLocked->blockSignals( true );

  if ( currentMarkupsNode->GetDisplayNode() != nullptr  )
    {
    double* color = currentMarkupsNode->GetDisplayNode()->GetSelectedColor();
    QColor qColor;
    qMRMLUtils::colorToQColor( color, qColor );
    d->ColorButton->setColor( qColor );
    }

  if ( currentMarkupsNode->GetLocked() )
    {
    d->ActionLocked->setIcon( QIcon( ":/Icons/Small/CjyxLock.png" ) );
    }
  else
    {
    d->ActionLocked->setIcon( QIcon( ":/Icons/Small/CjyxUnlock.png" ) );
    }

  bool fixedNumberControlPoints = currentMarkupsNode->GetFixedNumberOfControlPoints();
  if (fixedNumberControlPoints)
    {
    d->ActionFixedNumberOfControlPoints->setIcon(QIcon(":/Icons/Small/CjyxPointNumberLock.png"));
    d->DeleteButton->setIcon(QIcon(":/Icons/MarkupsUnset.png"));
    d->DeleteButton->setToolTip("Unset position of the last control point placed (the control point will not be deleted).");
    }
  else
    {
    d->ActionFixedNumberOfControlPoints->setIcon(QIcon(":/Icons/Small/CjyxPointNumberUnlock.png"));
    d->DeleteButton->setIcon(QIcon(":/Icons/MarkupsDelete.png"));
    d->DeleteButton->setToolTip("Delete last added control point");
    }
  d->ActionUnsetLast->setVisible(!fixedNumberControlPoints && d->UnsetLastControlPointOptionVisible); // QToolButton button action does this so don't also have in menu
  d->ActionDeleteAll->setVisible(!fixedNumberControlPoints && d->DeleteAllControlPointsOptionVisible);
  this->updateDeleteButton();

  d->ActionVisibility->setEnabled(currentMarkupsNode->GetDisplayNode() != nullptr);
  if (currentMarkupsNode->GetDisplayNode() != nullptr)
    {
    if (currentMarkupsNode->GetDisplayNode()->GetVisibility() )
      {
      d->ActionVisibility->setIcon( QIcon( ":/Icons/Small/CjyxVisible.png" ) );
      }
    else
      {
      d->ActionVisibility->setIcon( QIcon( ":/Icons/Small/CjyxInvisible.png" ) );
      }
    }

  d->ColorButton->blockSignals( wasBlockedColorButton );
  d->ActionVisibility->blockSignals( wasBlockedVisibilityButton);
  d->ActionLocked->blockSignals( wasBlockedLockButton );

  bool wasBlockedPlaceButton = d->PlaceButton->blockSignals( true );
  d->PlaceButton->setChecked(placeModeEnabled());
  d->PlaceButton->blockSignals( wasBlockedPlaceButton );
  d->PlaceButton->setIcon(QIcon(currentMarkupsNode->GetAddIcon()));

  bool wasBlockedPersistencyAction = d->ActionPlacePersistentPoint->blockSignals( true );
  d->ActionPlacePersistentPoint->setChecked(placeModePersistency());
  d->ActionPlacePersistentPoint->blockSignals( wasBlockedPersistencyAction );

  bool currentPlaceModeEnabled = placeModeEnabled();
  if (d->LastSignaledPlaceModeEnabled != currentPlaceModeEnabled)
      {
      emit activeMarkupsFiducialPlaceModeChanged(currentPlaceModeEnabled);
      emit activeMarkupsPlaceModeChanged(currentPlaceModeEnabled);
      d->LastSignaledPlaceModeEnabled = currentPlaceModeEnabled;
      }

  d->IsUpdatingWidgetFromMRML = false;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::updateDeleteButton()
  {
  Q_D(qCjyxMarkupsPlaceWidget);
  if (d->DeleteButton)
    {
    bool showMenu = (d->ActionDeleteAll->isVisible() || d->ActionUnsetLast->isVisible() || d->ActionUnsetAll->isVisible());
    d->DeleteButton->setMenu(showMenu ? d->DeleteMenu : nullptr);
    d->DeleteButton->setPopupMode(showMenu ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);

    vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
    if (currentMarkupsNode == nullptr)
      {
      // if there is no node selected then just leave the current button visibility as is,
      // to avoid showing/hiding a button when the current node is temporarily set to nullptr
      return;
      }
    d->DeleteButton->setVisible(showMenu || !currentMarkupsNode->GetFixedNumberOfControlPoints()); // hide when no options to show
    }
  }

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qCjyxMarkupsPlaceWidget);

  this->Superclass::setMRMLScene(scene);

  vtkMRMLSelectionNode* selectionNode = nullptr;
  vtkMRMLInteractionNode *interactionNode = nullptr;

  if (d->MarkupsLogic != nullptr && d->MarkupsLogic->GetMRMLScene() != nullptr)
    {
    selectionNode = vtkMRMLSelectionNode::SafeDownCast(d->MarkupsLogic->GetMRMLScene()->GetNodeByID("vtkMRMLSelectionNodeSingleton" ) );
    interactionNode = vtkMRMLInteractionNode::SafeDownCast(d->MarkupsLogic->GetMRMLScene()->GetNodeByID( "vtkMRMLInteractionNodeSingleton" ) );
    }

  this->setInteractionNode(interactionNode);
  this->setSelectionNode(selectionNode);

  this->updateWidget();
}

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::onPlacePersistentPoint(bool enable)
{
  this->setPlaceModePersistency(enable);
}

//-----------------------------------------------------------------------------
qCjyxMarkupsPlaceWidget::PlaceMultipleMarkupsType qCjyxMarkupsPlaceWidget::placeMultipleMarkups() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->PlaceMultipleMarkups;
}

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setPlaceMultipleMarkups(PlaceMultipleMarkupsType option)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  d->PlaceMultipleMarkups = option;
  if (d->PlaceButton)
    {
    d->PlaceButton->setMenu(d->PlaceMultipleMarkups == ShowPlaceMultipleMarkupsOption ? d->PlaceMenu : nullptr);
    // Changing to DelayedPopup mode will hide menu button to avoid confusion of empty menu
    d->PlaceButton->setPopupMode(d->PlaceMultipleMarkups == ShowPlaceMultipleMarkupsOption ? QToolButton::MenuButtonPopup : QToolButton::DelayedPopup);
    }
  if (this->placeModeEnabled())
    {
    if (d->PlaceMultipleMarkups == ForcePlaceSingleMarkup)
      {
      this->setPlaceModePersistency(false);
      }
    else if (d->PlaceMultipleMarkups == ForcePlaceMultipleMarkups)
      {
      this->setPlaceModePersistency(true);
      }
    }
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::deleteAllControlPointsOptionVisible() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->DeleteAllControlPointsOptionVisible;
}

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setDeleteAllControlPointsOptionVisible(bool visible)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  d->DeleteAllControlPointsOptionVisible = visible;
  if (d->DeleteButton)
    {
    d->ActionDeleteAll->setVisible(visible);
    this->updateDeleteButton();
    }
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::unsetLastControlPointOptionVisible() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->UnsetLastControlPointOptionVisible;
}

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setUnsetLastControlPointOptionVisible(bool visible)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  d->UnsetLastControlPointOptionVisible = visible;
  if (d->DeleteButton)
    {
    d->ActionUnsetLast->setVisible(visible);
    this->updateDeleteButton();
    }
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::unsetAllControlPointsOptionVisible() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->UnsetAllControlPointsOptionVisible;
}

//------------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setUnsetAllControlPointsOptionVisible(bool visible)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  d->UnsetAllControlPointsOptionVisible = visible;
  if (d->DeleteButton)
    {
    d->ActionUnsetAll->setVisible(visible);
    this->updateDeleteButton();
    }
}

//-----------------------------------------------------------------------------
QToolButton* qCjyxMarkupsPlaceWidget::placeButton() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->PlaceButton;
}

//-----------------------------------------------------------------------------
QToolButton* qCjyxMarkupsPlaceWidget::deleteButton() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->DeleteButton;
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsPlaceWidget::buttonsVisible() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  foreach( QWidget *w, d->OptionsWidgets )
    {
    if (!w->isVisible())
      {
      return false;
      }
    }
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setButtonsVisible(bool visible)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  foreach( QWidget *w, d->OptionsWidgets )
    {
    w->setVisible(visible);
    }
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setNodeColor(QColor color)
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if ( currentMarkupsNode == nullptr )
    {
    return;
    }

  vtkMRMLDisplayNode* currentMarkupsDisplayNode = currentMarkupsNode->GetDisplayNode();
  if ( currentMarkupsDisplayNode == nullptr )
    {
    return;
    }

  double rgbDoubleVector[3] = {color.redF(),color.greenF(),color.blueF()};
  currentMarkupsDisplayNode->SetColor( rgbDoubleVector );
  currentMarkupsDisplayNode->SetSelectedColor( rgbDoubleVector );
}

//-----------------------------------------------------------------------------
QColor qCjyxMarkupsPlaceWidget::nodeColor() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if ( currentMarkupsNode == nullptr )
    {
    return d->DefaultNodeColor;
    }

  vtkMRMLDisplayNode* currentMarkupsDisplayNode = currentMarkupsNode->GetDisplayNode();
  if ( currentMarkupsDisplayNode == nullptr )
    {
    return d->DefaultNodeColor;
    }

  QColor color;
  double rgbDoubleVector[3] = {0.0,0.0,0.0};
  currentMarkupsDisplayNode->GetSelectedColor(rgbDoubleVector);
  color.setRgb(static_cast<int>(rgbDoubleVector[0]),
               static_cast<int>(rgbDoubleVector[1]),
               static_cast<int>(rgbDoubleVector[2]));
  return color;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::setDefaultNodeColor(QColor color)
{
  Q_D(qCjyxMarkupsPlaceWidget);
  d->DefaultNodeColor = color;
}

//-----------------------------------------------------------------------------
QColor qCjyxMarkupsPlaceWidget::defaultNodeColor() const
{
  Q_D(const qCjyxMarkupsPlaceWidget);
  return d->DefaultNodeColor;
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::onColorButtonChanged(QColor color)
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr || currentMarkupsNode->GetDisplayNode() == nullptr)
    {
    return;
    }
  double colorDoubleVector[3] = {0.0,0.0,0.0};
  qMRMLUtils::qColorToColor( color, colorDoubleVector );
  currentMarkupsNode->GetDisplayNode()->SetSelectedColor( colorDoubleVector );
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::onVisibilityButtonClicked()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if ( currentMarkupsNode == nullptr || currentMarkupsNode->GetDisplayNode() == nullptr )
    {
    return;
    }
  currentMarkupsNode->GetDisplayNode()->SetVisibility( ! currentMarkupsNode->GetDisplayNode()->GetVisibility() );
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::onLockedButtonClicked()
{
  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr)
    {
    return;
    }
  currentMarkupsNode->SetLocked(!currentMarkupsNode->GetLocked());
}

//-----------------------------------------------------------------------------
void qCjyxMarkupsPlaceWidget::onFixedNumberOfControlPointsButtonClicked()
{
  Q_D(const qCjyxMarkupsPlaceWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = this->currentMarkupsNode();
  if (currentMarkupsNode == nullptr)
    {
    return;
    }
  currentMarkupsNode->SetFixedNumberOfControlPoints(!currentMarkupsNode->GetFixedNumberOfControlPoints());

  // end point placement for locked node
  d->InteractionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::ViewTransform);
  this->updateWidget();
}
