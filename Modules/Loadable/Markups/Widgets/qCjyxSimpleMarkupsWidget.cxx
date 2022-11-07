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
#include "qCjyxSimpleMarkupsWidget.h"

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
#include <vtkMRMLMarkupsNode.h>

// Qt includes
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QTableWidgetItem>

int CONTROL_POINT_LABEL_COLUMN = 0;
int CONTROL_POINT_X_COLUMN = 1;
int CONTROL_POINT_Y_COLUMN = 2;
int CONTROL_POINT_Z_COLUMN = 3;
int CONTROL_POINT_COLUMNS = 4;


//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CreateModels
class qCjyxSimpleMarkupsWidgetPrivate
  : public Ui_qCjyxSimpleMarkupsWidget
{
  Q_DECLARE_PUBLIC(qCjyxSimpleMarkupsWidget);
protected:
  qCjyxSimpleMarkupsWidget* const q_ptr;

public:
  qCjyxSimpleMarkupsWidgetPrivate( qCjyxSimpleMarkupsWidget& object);
  ~qCjyxSimpleMarkupsWidgetPrivate();
  virtual void setupUi(qCjyxSimpleMarkupsWidget*);

public:
  vtkWeakPointer<vtkCjyxMarkupsLogic> MarkupsLogic;
  bool EnterPlaceModeOnNodeChange;
  bool JumpToSliceEnabled;
  int ViewGroup;

  vtkWeakPointer<vtkMRMLMarkupsNode> CurrentMarkupsNode;
};

// --------------------------------------------------------------------------
qCjyxSimpleMarkupsWidgetPrivate::qCjyxSimpleMarkupsWidgetPrivate( qCjyxSimpleMarkupsWidget& object)
  : q_ptr(&object)
  , EnterPlaceModeOnNodeChange(true)
  , JumpToSliceEnabled(false)
  , ViewGroup(-1)
{
}

//-----------------------------------------------------------------------------
qCjyxSimpleMarkupsWidgetPrivate::~qCjyxSimpleMarkupsWidgetPrivate() = default;

// --------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidgetPrivate::setupUi(qCjyxSimpleMarkupsWidget* widget)
{
  this->Ui_qCjyxSimpleMarkupsWidget::setupUi(widget);
}


//-----------------------------------------------------------------------------
// qCjyxSimpleMarkupsWidget methods

//-----------------------------------------------------------------------------
qCjyxSimpleMarkupsWidget::qCjyxSimpleMarkupsWidget(QWidget* parentWidget) : Superclass( parentWidget ) , d_ptr( new qCjyxSimpleMarkupsWidgetPrivate(*this) )
{
  this->setup();
}

//-----------------------------------------------------------------------------
qCjyxSimpleMarkupsWidget::~qCjyxSimpleMarkupsWidget()
{
  this->setCurrentNode(nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setup()
{
  Q_D(qCjyxSimpleMarkupsWidget);

  d->MarkupsLogic = vtkCjyxMarkupsLogic::SafeDownCast(this->moduleLogic("Markups"));
  if (!d->MarkupsLogic)
    {
    qCritical() << Q_FUNC_INFO << ": Markups module is not found, some markup manipulation features will not be available";
    }

  d->setupUi(this);

  connect( d->MarkupsNodeComboBox, SIGNAL( currentNodeChanged( vtkMRMLNode* ) ), this, SLOT( onMarkupsNodeChanged() ) );
  connect( d->MarkupsNodeComboBox, SIGNAL( nodeAddedByUser( vtkMRMLNode* ) ), this, SLOT( onMarkupsNodeAdded( vtkMRMLNode* ) ) );
  connect( d->MarkupsPlaceWidget, SIGNAL( activeMarkupsPlaceModeChanged(bool) ), this, SIGNAL( activeMarkupsPlaceModeChanged(bool) ) );

  d->MarkupsControlPointsTableWidget->setColumnCount( CONTROL_POINT_COLUMNS );
  d->MarkupsControlPointsTableWidget->setHorizontalHeaderLabels( QStringList() << "Label" << "R" << "A" << "S" );
  d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

  // Reduce row height to minimum necessary
  d->MarkupsControlPointsTableWidget->setWordWrap(true);
  d->MarkupsControlPointsTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  d->MarkupsControlPointsTableWidget->setContextMenuPolicy( Qt::CustomContextMenu );
  d->MarkupsControlPointsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // only select rows rather than cells

  connect(d->MarkupsControlPointsTableWidget, SIGNAL( customContextMenuRequested(const QPoint&) ),
    this, SLOT( onMarkupsControlPointsTableContextMenu(const QPoint&) ) );
  connect(d->MarkupsControlPointsTableWidget, SIGNAL( cellChanged( int, int ) ), this, SLOT( onMarkupsControlPointEdited( int, int ) ) );
  // listen for click on a markup
  connect(d->MarkupsControlPointsTableWidget, SIGNAL(cellClicked(int,int)), this, SLOT(onMarkupsControlPointSelected(int,int)));
  // listen for the current cell selection change (happens when arrows are used to navigate)
  connect(d->MarkupsControlPointsTableWidget, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(onMarkupsControlPointSelected(int, int)));
}

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxSimpleMarkupsWidget::currentNode() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsNodeComboBox->currentNode();
}

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxSimpleMarkupsWidget::getCurrentNode()
{
  qWarning("qCjyxSimpleMarkupsWidget::getCurrentNode() method is deprecated. Use qCjyxSimpleMarkupsWidget::currentNode() method instead");
  return this->currentNode();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setCurrentNode(vtkMRMLNode* currentNode)
{
  Q_D(qCjyxSimpleMarkupsWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast( currentNode );
  if (currentMarkupsNode==d->CurrentMarkupsNode)
    {
    // not changed
    return;
    }

  // Don't change the active markups if the current node is changed programmatically
  bool wasBlocked = d->MarkupsNodeComboBox->blockSignals(true);
  d->MarkupsNodeComboBox->setCurrentNode( currentMarkupsNode );
  d->MarkupsNodeComboBox->blockSignals(wasBlocked);

  d->MarkupsPlaceWidget->setCurrentNode( currentMarkupsNode );

  // Reconnect the appropriate nodes
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointAddedEvent, this, SLOT(onPointAdded()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointRemovedEvent, this, SLOT(updateWidget()));
  this->qvtkReconnect(d->CurrentMarkupsNode, currentMarkupsNode, vtkMRMLMarkupsNode::PointModifiedEvent, this, SLOT(updateWidget()));

  d->CurrentMarkupsNode = currentMarkupsNode;

  this->updateWidget();

  emit markupsNodeChanged();
  emit markupsFiducialNodeChanged();
}

//-----------------------------------------------------------------------------
vtkMRMLInteractionNode* qCjyxSimpleMarkupsWidget::interactionNode()const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsPlaceWidget->interactionNode();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setInteractionNode(vtkMRMLInteractionNode* interactionNode)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setInteractionNode(interactionNode);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setNodeBaseName(const QString& newNodeBaseName)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsNodeComboBox->setBaseName(newNodeBaseName);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setDefaultNodeColor(QColor color)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setDefaultNodeColor(color);
}

//-----------------------------------------------------------------------------
QColor qCjyxSimpleMarkupsWidget::defaultNodeColor() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsPlaceWidget->defaultNodeColor();
}

//-----------------------------------------------------------------------------
bool qCjyxSimpleMarkupsWidget::enterPlaceModeOnNodeChange() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->EnterPlaceModeOnNodeChange;
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setEnterPlaceModeOnNodeChange(bool enterPlaceMode)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->EnterPlaceModeOnNodeChange = enterPlaceMode;
}

//-----------------------------------------------------------------------------
bool qCjyxSimpleMarkupsWidget::jumpToSliceEnabled() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->JumpToSliceEnabled;
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setJumpToSliceEnabled(bool enable)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->JumpToSliceEnabled = enable;
}

//-----------------------------------------------------------------------------
bool qCjyxSimpleMarkupsWidget::nodeSelectorVisible() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsNodeComboBox->isVisible();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setNodeSelectorVisible(bool visible)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsNodeComboBox->setVisible(visible);
}

//-----------------------------------------------------------------------------
bool qCjyxSimpleMarkupsWidget::optionsVisible() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsPlaceWidget->isVisible();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setOptionsVisible(bool visible)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setVisible(visible);
}

//-----------------------------------------------------------------------------
QTableWidget* qCjyxSimpleMarkupsWidget::tableWidget() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsControlPointsTableWidget;
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setNodeColor(QColor color)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setNodeColor(color);
}

//-----------------------------------------------------------------------------
QColor qCjyxSimpleMarkupsWidget::nodeColor() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsPlaceWidget->nodeColor();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setViewGroup(int newViewGroup)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->ViewGroup = newViewGroup;
}

//-----------------------------------------------------------------------------
int qCjyxSimpleMarkupsWidget::viewGroup() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->ViewGroup;
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::highlightNthControlPoint(int n)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  if ( n >= 0 && n < d->MarkupsControlPointsTableWidget->rowCount() )
    {
    d->MarkupsControlPointsTableWidget->selectRow(n);
    d->MarkupsControlPointsTableWidget->setCurrentCell(n,0);
    d->MarkupsControlPointsTableWidget->scrollTo(d->MarkupsControlPointsTableWidget->currentIndex());
    }
  else
    {
    d->MarkupsControlPointsTableWidget->clearSelection();
    }
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::highlightNthFiducial(int n)
{
  this->highlightNthControlPoint(n);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::activate()
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setCurrentNodeActive(true);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::placeActive(bool place)
{
  Q_D(qCjyxSimpleMarkupsWidget);
  d->MarkupsPlaceWidget->setPlaceModeEnabled(place);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onMarkupsNodeChanged()
{
  Q_D(qCjyxSimpleMarkupsWidget);
  vtkMRMLMarkupsNode* currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast( d->MarkupsNodeComboBox->currentNode() );
  this->setCurrentNode(currentMarkupsNode);

  if (d->EnterPlaceModeOnNodeChange)
    {
    d->MarkupsPlaceWidget->setPlaceModeEnabled(currentMarkupsNode!=nullptr);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onMarkupsNodeAdded( vtkMRMLNode* newNode )
{
  Q_D(qCjyxSimpleMarkupsWidget);
  if (d->MarkupsLogic == nullptr)
    {
    qCritical("qCjyxSimpleMarkupsWidget::onMarkupsNodeAdded failed: Markups module logic is invalid");
    return;
    }

  vtkMRMLMarkupsNode* newMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast( newNode );
  if (newMarkupsNode->GetDisplayNode()==nullptr)
    {
    // Make sure there is an associated display node
    d->MarkupsLogic->AddNewDisplayNodeForMarkupsNode( newMarkupsNode );
    }
  d->MarkupsNodeComboBox->setCurrentNode( newMarkupsNode );
  this->setNodeColor( defaultNodeColor() );
  this->onMarkupsNodeChanged();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onMarkupsControlPointsTableContextMenu(const QPoint& position)
{
  Q_D(qCjyxSimpleMarkupsWidget);

  if (d->MarkupsLogic == nullptr)
    {
    qCritical("qCjyxSimpleMarkupsWidget::onMarkupsControlPointsTableContextMenu failed: Markups module logic is invalid");
    return;
    }

  QPoint globalPosition = d->MarkupsControlPointsTableWidget->viewport()->mapToGlobal( position );

  QMenu* controlPointsMenu = new QMenu( d->MarkupsControlPointsTableWidget );
  QAction* deleteAction = new QAction( "Delete highlighted control points", controlPointsMenu );
  QAction* upAction = new QAction( "Move current control point up", controlPointsMenu );
  QAction* downAction = new QAction( "Move current control point down", controlPointsMenu );
  QAction* jumpAction = new QAction( "Jump slices to control point", controlPointsMenu );

  controlPointsMenu->addAction( deleteAction );
  controlPointsMenu->addAction( upAction );
  controlPointsMenu->addAction( downAction );
  controlPointsMenu->addAction( jumpAction );

  QAction* selectedAction = controlPointsMenu->exec( globalPosition );

  int currentControlPoint = d->MarkupsControlPointsTableWidget->currentRow();
  vtkMRMLMarkupsNode* currentNode = vtkMRMLMarkupsNode::SafeDownCast( d->MarkupsNodeComboBox->currentNode() );

  if ( currentNode == nullptr )
    {
    return;
    }

  // Only do this for non-null node
  if ( selectedAction == deleteAction )
    {
    QItemSelectionModel* selectionModel = d->MarkupsControlPointsTableWidget->selectionModel();
    std::vector< int > deleteControlPoints;
    // Need to find selected before removing because removing automatically refreshes the table
    for ( int i = 0; i < d->MarkupsControlPointsTableWidget->rowCount(); i++ )
      {
      if ( selectionModel->rowIntersectsSelection( i, d->MarkupsControlPointsTableWidget->rootIndex() ) )
        {
        deleteControlPoints.push_back( i );
        }
      }
    // Do this in batch mode
    int wasModifying = currentNode->StartModify();
    //Traversing this way should be more efficient and correct
    for ( int i = static_cast<int>(deleteControlPoints.size()) - 1; i >= 0; i-- )
      {
      // remove the point at that row
      currentNode->RemoveNthControlPoint(deleteControlPoints.at( static_cast<size_t>(i) ));
      }
    currentNode->EndModify(wasModifying);
    }


  if ( selectedAction == upAction )
    {
    if ( currentControlPoint > 0 )
      {
      currentNode->SwapControlPoints(currentControlPoint, currentControlPoint - 1 );
      }
    }

  if ( selectedAction == downAction )
    {
    if ( currentControlPoint < currentNode->GetNumberOfControlPoints() - 1 )
      {
      currentNode->SwapControlPoints( currentControlPoint, currentControlPoint + 1 );
      }
    }

  if ( selectedAction == jumpAction )
    {
    d->MarkupsLogic->JumpSlicesToNthPointInMarkup(this->currentNode()->GetID(), currentControlPoint, true /* centered */, d->ViewGroup);
    }

  this->updateWidget();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onMarkupsControlPointSelected(int row, int column)
{
  Q_UNUSED(column)
  Q_D(qCjyxSimpleMarkupsWidget);

  if (d->JumpToSliceEnabled)
    {
    vtkMRMLMarkupsNode *currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast(this->currentNode());
    if (currentMarkupsNode == nullptr)
      {
      return;
      }

    if (d->MarkupsLogic == nullptr)
      {
      qCritical("qCjyxSimpleMarkupsWidget::onMarkupsControlPointSelected "
                "failed: Cannot jump, markups module logic is invalid");
      return;
      }
    d->MarkupsLogic->JumpSlicesToNthPointInMarkup(currentMarkupsNode->GetID(), row, true /* centered */, d->ViewGroup);
  }

  emit currentMarkupsControlPointSelectionChanged(row);
  emit currentMarkupsFiducialSelectionChanged(row);
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onMarkupsControlPointEdited(int row, int column)
{
  Q_D(qCjyxSimpleMarkupsWidget);

  vtkMRMLMarkupsNode* currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast( this->currentNode() );

  if ( currentMarkupsNode == nullptr )
    {
    return;
    }

  // Find the control point's current properties
  double currentControlPointPosition[3] = { 0, 0, 0 };
  currentMarkupsNode->GetNthControlPointPosition( row, currentControlPointPosition );
  std::string currentControlPointLabel = currentMarkupsNode->GetNthControlPointLabel( row );

  // Find the entry that we changed
  QTableWidgetItem* qItem = d->MarkupsControlPointsTableWidget->item( row, column );
  QString qText = qItem->text();

  if ( column == CONTROL_POINT_LABEL_COLUMN )
    {
    currentMarkupsNode->SetNthControlPointLabel( row, qText.toStdString() );
    }

  // Check if the value can be converted to double is already performed implicitly
  double newControlPointPosition = qText.toDouble();

  // Change the position values
  if ( column == CONTROL_POINT_X_COLUMN )
    {
    currentControlPointPosition[ 0 ] = newControlPointPosition;
    }
  if ( column == CONTROL_POINT_Y_COLUMN )
    {
    currentControlPointPosition[ 1 ] = newControlPointPosition;
    }
  if ( column == CONTROL_POINT_Z_COLUMN )
    {
    currentControlPointPosition[ 2 ] = newControlPointPosition;
    }

  currentMarkupsNode->SetNthControlPointPosition( row, currentControlPointPosition );

  this->updateWidget(); // This may not be necessary the widget is updated whenever a control point is changed
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::updateWidget()
{
  Q_D(qCjyxSimpleMarkupsWidget);

  if (d->MarkupsLogic == nullptr || this->mrmlScene() == nullptr)
    {
    qCritical("qCjyxSimpleMarkupsWidget::updateWidget failed: Markups module logic or scene is invalid");
    }

  vtkMRMLMarkupsNode* currentMarkupsNode = vtkMRMLMarkupsNode::SafeDownCast( d->MarkupsNodeComboBox->currentNode() );
  if ( currentMarkupsNode == nullptr || d->MarkupsLogic == nullptr)
    {
    d->MarkupsControlPointsTableWidget->clear();
    d->MarkupsControlPointsTableWidget->setRowCount( 0 );
    d->MarkupsControlPointsTableWidget->setColumnCount( 0 );
    d->MarkupsPlaceWidget->setEnabled(false);
    emit updateFinished();
    return;
    }

  d->MarkupsPlaceWidget->setEnabled(true);

  // Update the control points table
  bool wasBlockedTableWidget = d->MarkupsControlPointsTableWidget->blockSignals( true );

  if (d->MarkupsControlPointsTableWidget->rowCount()==currentMarkupsNode->GetNumberOfControlPoints())
    {
    // don't recreate the table if the number of items is not changed to preserve selection state
    double controlPointPosition[ 3 ] = { 0, 0, 0 };
    std::string controlPointLabel;
    for ( int i = 0; i < currentMarkupsNode->GetNumberOfControlPoints(); i++ )
      {
      controlPointLabel = currentMarkupsNode->GetNthControlPointLabel(i);
      currentMarkupsNode->GetNthControlPointPosition(i, controlPointPosition);
      d->MarkupsControlPointsTableWidget->item(i, CONTROL_POINT_LABEL_COLUMN)->setText(QString::fromStdString(controlPointLabel));
      d->MarkupsControlPointsTableWidget->item(i, CONTROL_POINT_X_COLUMN)->setText(QString::number( controlPointPosition[0], 'f', 3 ));
      d->MarkupsControlPointsTableWidget->item(i, CONTROL_POINT_Y_COLUMN)->setText(QString::number( controlPointPosition[1], 'f', 3 ));
      d->MarkupsControlPointsTableWidget->item(i, CONTROL_POINT_Z_COLUMN)->setText(QString::number( controlPointPosition[2], 'f', 3 ));
      }
    }
  else
    {
    d->MarkupsControlPointsTableWidget->clear();
    d->MarkupsControlPointsTableWidget->setRowCount( currentMarkupsNode->GetNumberOfControlPoints() );
    d->MarkupsControlPointsTableWidget->setColumnCount( CONTROL_POINT_COLUMNS );
    d->MarkupsControlPointsTableWidget->setHorizontalHeaderLabels( QStringList() << "Label" << "R" << "A" << "S" );
    d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    d->MarkupsControlPointsTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    double controlPointPosition[ 3 ] = { 0, 0, 0 };
    std::string controlPointLabel;
    for ( int i = 0; i < currentMarkupsNode->GetNumberOfControlPoints(); i++ )
      {
      controlPointLabel = currentMarkupsNode->GetNthControlPointLabel( i );
      currentMarkupsNode->GetNthControlPointPosition( i, controlPointPosition );

      QTableWidgetItem* labelItem = new QTableWidgetItem( QString::fromStdString(controlPointLabel) );
      QTableWidgetItem* xItem = new QTableWidgetItem( QString::number( controlPointPosition[0], 'f', 3 ) );
      QTableWidgetItem* yItem = new QTableWidgetItem( QString::number( controlPointPosition[1], 'f', 3 ) );
      QTableWidgetItem* zItem = new QTableWidgetItem( QString::number( controlPointPosition[2], 'f', 3 ) );

      d->MarkupsControlPointsTableWidget->setItem( i, CONTROL_POINT_LABEL_COLUMN, labelItem );
      d->MarkupsControlPointsTableWidget->setItem( i, CONTROL_POINT_X_COLUMN, xItem );
      d->MarkupsControlPointsTableWidget->setItem( i, CONTROL_POINT_Y_COLUMN, yItem );
      d->MarkupsControlPointsTableWidget->setItem( i, CONTROL_POINT_Z_COLUMN, zItem );
      }
    }

  d->MarkupsControlPointsTableWidget->blockSignals( wasBlockedTableWidget );

  emit updateFinished();
}

//-----------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::onPointAdded()
{
  Q_D(qCjyxSimpleMarkupsWidget);
  this->updateWidget();
  d->MarkupsControlPointsTableWidget->scrollToBottom();
}

//------------------------------------------------------------------------------
void qCjyxSimpleMarkupsWidget::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);
  this->updateWidget();
}

//-----------------------------------------------------------------------------
qCjyxMarkupsPlaceWidget* qCjyxSimpleMarkupsWidget::markupsPlaceWidget() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsPlaceWidget;
}

//-----------------------------------------------------------------------------
qMRMLNodeComboBox* qCjyxSimpleMarkupsWidget::markupsSelectorComboBox() const
{
  Q_D(const qCjyxSimpleMarkupsWidget);
  return d->MarkupsNodeComboBox;
}
