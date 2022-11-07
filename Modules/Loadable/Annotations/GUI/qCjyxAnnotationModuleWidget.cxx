#include "GUI/qCjyxAnnotationModuleWidget.h"
#include "ui_qCjyxAnnotationModuleWidget.h"
#include "Logic/vtkCjyxAnnotationModuleLogic.h"

// CTK includes
#include "ctkCollapsibleButton.h"
// Qt includes
#include <QButtonGroup>
#include <QList>
#include <QFontMetrics>
#include <QMessageBox>
#include <QTextBrowser>
#include <QFile>
#include <QLineEdit>
#include <QFileDialog>
#include <QPrintDialog>
#include <QPrinter>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBuffer>
#include <QImageWriter>

#include "qCjyxMouseModeToolBar.h"
#include "qMRMLSceneDisplayableModel.h"

// GUI includes
#include "GUI/qCjyxAnnotationModuleReportDialog.h"
#include "GUI/qCjyxAnnotationModulePropertyDialog.h"
#include "GUI/qCjyxAnnotationModuleSnapShotDialog.h"

// MRML includes
#include "vtkMRMLAnnotationDisplayNode.h"
#include "vtkMRMLAnnotationHierarchyNode.h"
#include "vtkMRMLAnnotationNode.h"
#include "vtkMRMLDisplayableHierarchyNode.h"
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLSelectionNode.h"

// VTK includes
#include <vtkCommand.h>
#include <vtkCollection.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

// CjyxLogic includes
#include "qCjyxApplication.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Annotation
class qCjyxAnnotationModuleWidgetPrivate: public Ui_qCjyxAnnotationModuleWidget
{
  Q_DECLARE_PUBLIC(qCjyxAnnotationModuleWidget);
protected:
  qCjyxAnnotationModuleWidget* const q_ptr;
public:

  qCjyxAnnotationModuleWidgetPrivate(qCjyxAnnotationModuleWidget& object);
  ~qCjyxAnnotationModuleWidgetPrivate();

  void setupUi(qCjyxWidget* widget);

  vtkCjyxAnnotationModuleLogic* logic() const;

  qCjyxAnnotationModuleSnapShotDialog* m_SnapShotDialog;
};

//-----------------------------------------------------------------------------
vtkCjyxAnnotationModuleLogic*
qCjyxAnnotationModuleWidgetPrivate::logic() const
{
  Q_Q(const qCjyxAnnotationModuleWidget);
  return vtkCjyxAnnotationModuleLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
qCjyxAnnotationModuleWidgetPrivate::qCjyxAnnotationModuleWidgetPrivate(qCjyxAnnotationModuleWidget& object)
  : q_ptr(&object)
{
  this->m_SnapShotDialog = nullptr;
}

//-----------------------------------------------------------------------------
qCjyxAnnotationModuleWidgetPrivate::~qCjyxAnnotationModuleWidgetPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxAnnotationModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidgetPrivate::setupUi(qCjyxWidget* widget)
{
  Q_Q(qCjyxAnnotationModuleWidget);

  this->Ui_qCjyxAnnotationModuleWidget::setupUi(widget);

  // Get application logic node
  q->MRMLAppLogic = qCjyxApplication::application()->applicationLogic();

  // setup the hierarchy treeWidget
  this->hierarchyTreeView->setLogic(this->logic());
  this->hierarchyTreeView->setMRMLScene(this->logic()->GetMRMLScene());

  this->hierarchyTreeView->hideScene();
  // enable scrolling when drag past end of window
  this->hierarchyTreeView->setFitSizeToVisibleIndexes(false);

  // connect the tree's edit property button to the widget's slot
  QObject::connect(this->hierarchyTreeView, SIGNAL(onPropertyEditButtonClicked(QString)),
                   q, SLOT(propertyEditButtonClicked(QString)));

  // create node panel
  q->connect(this->createLineButton, SIGNAL(clicked()),
    SLOT(onCreateLineButtonClicked()));
  q->connect(this->createROIButton, SIGNAL(clicked()),
    SLOT(onCreateROIButtonClicked()));

  // edit panel
  q->connect(this->selectAllButton, SIGNAL(clicked()),
        SLOT(selectAllButtonClicked()));
  q->connect(this->unselectAllButton, SIGNAL(clicked()),
        SLOT(unselectAllButtonClicked()));

  q->connect(this->visibleSelectedButton, SIGNAL(clicked()),
      SLOT(visibleSelectedButtonClicked()));
  q->connect(this->lockSelectedButton, SIGNAL(clicked()), q,
      SLOT(lockSelectedButtonClicked()));

  q->connect(this->jumpSlicesButton, SIGNAL(clicked()), q,
      SLOT(onJumpSlicesButtonClicked()));

  /*
  q->connect(this->moveDownSelectedButton, SIGNAL(clicked()),
      SLOT(moveDownSelected()));
  q->connect(this->moveUpSelectedButton, SIGNAL(clicked()),
      SLOT(moveUpSelected()));
  */
  q->connect(this->addHierarchyButton, SIGNAL(clicked()),
      SLOT(onAddHierarchyButtonClicked()));
  q->connect(this->deleteSelectedButton, SIGNAL(clicked()),
      SLOT(deleteSelectedButtonClicked()));

  // active list
  q->connect(this->visibleHierarchyButton, SIGNAL(clicked()), q,
      SLOT(visibleHierarchyButtonClicked()));
  q->connect(this->invisibleHierarchyButton, SIGNAL(clicked()), q,
      SLOT(invisibleHierarchyButtonClicked()));
  q->connect(this->lockHierarchyButton, SIGNAL(clicked()), q,
      SLOT(lockHierarchyButtonClicked()));
  q->connect(this->unlockHierarchyButton, SIGNAL(clicked()), q,
      SLOT(unlockHierarchyButtonClicked()));

  // Save Panel
  q->connect(this->generateReport, SIGNAL(clicked()), q,
      SLOT(onReportButtonClicked()));


  q->connect(q, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)), q, SLOT(refreshTree()));

  // listen for updates to the logic to update the active hierarchy label
  q->qvtkConnect(this->logic(), vtkCommand::ModifiedEvent,
                 q, SLOT(updateActiveHierarchyLabel()));
  q->qvtkConnect(this->logic(), vtkCjyxAnnotationModuleLogic::RefreshRequestEvent,
                 q, SLOT(refreshTree()));
  // listen to the logic for when it adds a new hierarchy node that has to be
  // expanded
  q->qvtkConnect(this->logic(), vtkCjyxAnnotationModuleLogic::HierarchyNodeAddedEvent,
                 q, SLOT(onHierarchyNodeAddedEvent(vtkObject*,vtkObject*)));

  // update from the mrml scene
  q->refreshTree();
}

//-----------------------------------------------------------------------------
qCjyxAnnotationModuleWidget::qCjyxAnnotationModuleWidget(QWidget* parent) :
  qCjyxAbstractModuleWidget(parent)
  , d_ptr(new qCjyxAnnotationModuleWidgetPrivate(*this))
{
  Q_D(qCjyxAnnotationModuleWidget);
  this->m_ReportDialog = nullptr;
  this->m_PropertyDialog = nullptr;
  this->m_CurrentAnnotationType = 0;

  d->m_SnapShotDialog = new qCjyxAnnotationModuleSnapShotDialog(this);
}

//-----------------------------------------------------------------------------
qCjyxAnnotationModuleWidget::~qCjyxAnnotationModuleWidget()
{
  if (this->m_ReportDialog)
    {
    this->m_ReportDialog->close();
    delete this->m_ReportDialog;
    this->m_ReportDialog = nullptr;
    }

  if (this->m_PropertyDialog)
    {
    this->m_PropertyDialog->close();
    delete this->m_PropertyDialog;
    this->m_PropertyDialog = nullptr;
    }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::setup()
{
  Q_D(qCjyxAnnotationModuleWidget);
  this->Superclass::setup();
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::moveDownSelected()
{
  Q_D(qCjyxAnnotationModuleWidget);

  const char* mrmlId =
    d->logic()->MoveAnnotationDown(d->hierarchyTreeView->firstSelectedNode());
  vtkMRMLNode* mrmlNode = this->mrmlScene()->GetNodeByID(mrmlId);

  d->hierarchyTreeView->clearSelection();
  d->hierarchyTreeView->setCurrentNode(mrmlNode);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::moveUpSelected()
{
  Q_D(qCjyxAnnotationModuleWidget);

  const char* mrmlId = d->logic()->MoveAnnotationUp(d->hierarchyTreeView->firstSelectedNode());
  vtkMRMLNode* mrmlNode = this->mrmlScene()->GetNodeByID(mrmlId);

  d->hierarchyTreeView->clearSelection();
  d->hierarchyTreeView->setCurrentNode(mrmlNode);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::selectAllButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->hierarchyTreeView->selectAll();
  d->logic()->SetAllAnnotationsSelected(true);
  d->logic()->SetActiveHierarchyNodeID(nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::unselectAllButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->hierarchyTreeView->clearSelection();
  d->logic()->SetAllAnnotationsSelected(false);
  d->logic()->SetActiveHierarchyNodeID(nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::onJumpSlicesButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->JumpSlicesToAnnotationCoordinate(d->hierarchyTreeView->firstSelectedNode());
}

//-----------------------------------------------------------------------------
//
// Property dialog
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::propertyEditButtonClicked(QString mrmlId)
{
  Q_D(qCjyxAnnotationModuleWidget);

  QByteArray mrmlIdArray = mrmlId.toUtf8();

  // special case for snapshots
  if (d->logic()->IsSnapshotNode(mrmlIdArray.data()))
    {

    // the selected entry is a snapshot node,

    // pass a pointer to the logic class
    d->m_SnapShotDialog->setLogic(d->logic());

    // reset all fields of the dialog
    d->m_SnapShotDialog->reset();

    // now we initialize it with existing values
    d->m_SnapShotDialog->loadNode(mrmlIdArray.data());

    // in any case, show the dialog
    d->m_SnapShotDialog->open();

    // bail out, everything below is not for snapshots
    return;
    }
  // end of special case for snapshots

  // check if there exists an annotationNode with the given ID
  // only then display the property dialog
  if (d->logic()->IsAnnotationNode(mrmlIdArray.data()) || d->logic()->IsAnnotationHierarchyNode(mrmlIdArray.data()))
    {

    if (this->m_PropertyDialog)
      {
      //QMessageBox::warning(d->hierarchyTreeView,
      //    QString("Modify Annotation Properties"), QString(
      //        "The property dialog is already open."));

      this->m_PropertyDialog->activateWindow();

      return;
      }

    // TODO
    //d->setItemEditable(d->tableWidget->selectedItems(), false);

    qCjyxAnnotationModulePropertyDialog* propertyDialog =
        new qCjyxAnnotationModulePropertyDialog(mrmlIdArray.data(), d->logic());

    this->m_PropertyDialog = propertyDialog;

    // TODO: update the property dialog elements when the mrml node changes,
    // for now just make the dialog modal so can't change the annotations
    // while have the dialog open
    this->m_PropertyDialog->setModal(true);

    this->m_PropertyDialog->setVisible(true);

    this->connect(this->m_PropertyDialog, SIGNAL(rejected()), this,
        SLOT(propertyRestored()));
    this->connect(this->m_PropertyDialog, SIGNAL(accepted()), this,
        SLOT(propertyAccepted()));

    }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::propertyRestored()
{

  const char * mrmlID = this->m_PropertyDialog->GetID();
  Q_D(qCjyxAnnotationModuleWidget);

  // TODO?

  d->logic()->SetAnnotationSelected(mrmlID, false);

  //delete this->m_PropertyDialog;
  this->m_PropertyDialog = nullptr;

}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::propertyAccepted()
{

  const char * mrmlID = this->m_PropertyDialog->GetID();
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->SetAnnotationSelected(mrmlID,false);

  // TODO?


  //delete this->m_PropertyDialog;
  this->m_PropertyDialog = nullptr;

}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::onCreateLineButtonClicked()
{

  vtkWeakPointer <vtkMRMLSelectionNode> selectionNode = (this->MRMLAppLogic && this->mrmlScene())
    ? this->MRMLAppLogic->GetSelectionNode() : nullptr;
  vtkWeakPointer <vtkMRMLInteractionNode> interactionNode = (this->MRMLAppLogic && this->mrmlScene())
    ? this->MRMLAppLogic->GetInteractionNode() : nullptr;

  if (this->mrmlScene() && selectionNode && interactionNode)
    {
    selectionNode->SetReferenceActivePlaceNodeClassName("vtkMRMLAnnotationRulerNode");
    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
    }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::onCreateROIButtonClicked()
{
  vtkWeakPointer <vtkMRMLSelectionNode> selectionNode = (this->MRMLAppLogic && this->mrmlScene())
    ? this->MRMLAppLogic->GetSelectionNode() : nullptr;
  vtkWeakPointer <vtkMRMLInteractionNode> interactionNode = (this->MRMLAppLogic && this->mrmlScene())
    ? this->MRMLAppLogic->GetInteractionNode() : nullptr;

  if (this->mrmlScene() && selectionNode && interactionNode)
    {
    selectionNode->SetReferenceActivePlaceNodeClassName("vtkMRMLAnnotationROINode");
    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
    }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::visibleSelectedButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->hierarchyTreeView->toggleVisibilityForSelected();

}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::lockSelectedButtonClicked()
{

  Q_D(qCjyxAnnotationModuleWidget);

  d->hierarchyTreeView->toggleLockForSelected();
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::deleteSelectedButtonClicked()
{

  Q_D(qCjyxAnnotationModuleWidget);

  d->hierarchyTreeView->deleteSelected();

}

// Active list buttons
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::invisibleHierarchyButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->SetHierarchyAnnotationsVisibleFlag(nullptr, false);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::visibleHierarchyButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->SetHierarchyAnnotationsVisibleFlag(nullptr, true);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::lockHierarchyButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->SetHierarchyAnnotationsLockFlag(nullptr, true);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::unlockHierarchyButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  d->logic()->SetHierarchyAnnotationsLockFlag(nullptr, false);
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::onAddHierarchyButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);
  d->logic()->SetActiveHierarchyNodeID(d->hierarchyTreeView->firstSelectedNode());
  this->refreshTree();
  if (d->logic()->AddHierarchy())
    {
    vtkMRMLNode* node = d->logic()->GetMRMLScene()->GetNodeByID(
      d->logic()->GetActiveHierarchyNodeID());
    d->hierarchyTreeView->setCurrentNode(node);
    }
  // set expanded state to match hierarchy node
  vtkMRMLNode *mrmlNode = this->mrmlScene()->GetNodeByID(d->logic()->GetActiveHierarchyNodeID());
  if (mrmlNode)
    {
    QModelIndex hierarchyIndex = d->hierarchyTreeView->sortFilterProxyModel()->indexFromMRMLNode(mrmlNode);
    vtkMRMLDisplayableHierarchyNode *hierarchyNode = vtkMRMLDisplayableHierarchyNode::SafeDownCast(mrmlNode);
    if (hierarchyNode)
      {
      d->hierarchyTreeView->setExpanded(hierarchyIndex, hierarchyNode->GetExpanded());
      }
    else
      {
      // otherwise just expand by default
      d->hierarchyTreeView->expand(hierarchyIndex);
      }
    }
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::updateActiveHierarchyLabel()
{
  Q_D(qCjyxAnnotationModuleWidget);

  const char *id = d->logic()->GetActiveHierarchyNodeID();
  //QString idString = QString(" (none)");
  QString name = QString("(none)");
  if (id)
    {
    //idString = QString(" (") + QString(id) + QString(")");
    vtkMRMLNode* node = d->logic()->GetMRMLScene() ?
      d->logic()->GetMRMLScene()->GetNodeByID(id) : nullptr;
    if (node)
      {
      name = QString(node->GetName());
      }
    }
  d->activeHierarchyLabel->setText(QString("Active list: ") + name);
}

//-----------------------------------------------------------------------------
// Refresh the hierarchy tree after an annotation was added or modified.
// Just do some layout changes - nothing special!
//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::refreshTree()
{
  Q_D(qCjyxAnnotationModuleWidget);

  if (d->logic() && d->logic()->GetMRMLScene() &&
      d->logic()->GetMRMLScene()->IsBatchProcessing())
    {
    // scene is updating, return
    return;
    }
  // don't reset unless scene is different as it also resets the expanded
  // level on the tree
  if (d->hierarchyTreeView->mrmlScene() != d->logic()->GetMRMLScene())
    {
    // this gets called on scene closed, can we make sure that the widget is
    // visible?
    d->hierarchyTreeView->setMRMLScene(d->logic()->GetMRMLScene());
    }
  d->hierarchyTreeView->hideScene();
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::onHierarchyNodeAddedEvent(vtkObject *vtkNotUsed(caller), vtkObject *callData)
{
  Q_D(qCjyxAnnotationModuleWidget);

  // the call data should be the annotation hierarchy node that was just
  // added, passed along from the logic
  if (callData == nullptr)
    {
    return;
    }
  vtkMRMLNode *node = nullptr;
  node = reinterpret_cast<vtkMRMLNode*>(callData);
  if (node == nullptr)
    {
    return;
    }

  // get the model index of the hierarchy node in the tree
  QModelIndex hierarchyIndex = d->hierarchyTreeView->sortFilterProxyModel()->indexFromMRMLNode(node);
  vtkMRMLDisplayableHierarchyNode *hierarchyNode = vtkMRMLDisplayableHierarchyNode::SafeDownCast(node);
  if (hierarchyNode)
    {
    // set the expanded state to match the node
    d->hierarchyTreeView->setExpanded(hierarchyIndex, hierarchyNode->GetExpanded());
    }
}

//-----------------------------------------------------------------------------
// Annotation SnapShot functionality

//-----------------------------------------------------------------------------
void qCjyxAnnotationModuleWidget::grabSnapShot()
{
  Q_D(qCjyxAnnotationModuleWidget);

  // show the dialog
  d->m_SnapShotDialog->setLogic(d->logic());
  d->m_SnapShotDialog->reset();
  d->m_SnapShotDialog->open();
}


//-----------------------------------------------------------------------------
// Annotation Report functionality
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Show the report dialog
void qCjyxAnnotationModuleWidget::onReportButtonClicked()
{
  Q_D(qCjyxAnnotationModuleWidget);

  if (!this->m_ReportDialog)
    {

    this->m_ReportDialog = new qCjyxAnnotationModuleReportDialog();

    // pass a pointer to the logic class
    this->m_ReportDialog->setLogic(d->logic());

    // create slots which listen to events fired by the OK and CANCEL button on the dialog
    this->connect(this->m_ReportDialog, SIGNAL(dialogRejected()), this,
        SLOT(reportDialogRejected()));
    this->connect(this->m_ReportDialog, SIGNAL(dialogAccepted()), this,
        SLOT(reportDialogAccepted()));
    }

  vtkNew<vtkCollection> collection;

  d->hierarchyTreeView->selectedAsCollection(collection.GetPointer());

  // if nothing was selected, select all
  if(collection->GetNumberOfItems() == 0)
    {
    d->hierarchyTreeView->selectAll();
    d->hierarchyTreeView->selectedAsCollection(collection.GetPointer());
    }

  this->m_ReportDialog->setAnnotations(collection.GetPointer());

  this->m_ReportDialog->updateReport();

  this->m_ReportDialog->setVisible(true);

  this->m_ReportDialog->raise();
  this->m_ReportDialog->activateWindow();

}

//-----------------------------------------------------------------------------
// Report dialog closed after saving
void qCjyxAnnotationModuleWidget::reportDialogAccepted()
{

  this->m_ReportDialog->setVisible(false);

}

//-----------------------------------------------------------------------------
// Report dialog closed without saving
void qCjyxAnnotationModuleWidget::reportDialogRejected()
{

  this->m_ReportDialog->setVisible(false);

}

//-----------------------------------------------------------
bool qCjyxAnnotationModuleWidget::setEditedNode(vtkMRMLNode* node,
                                                  QString role /* = QString()*/,
                                                  QString context /* = QString()*/)
{
  Q_D(qCjyxAnnotationModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);

  if (vtkMRMLAnnotationNode::SafeDownCast(node) || vtkMRMLAnnotationHierarchyNode::SafeDownCast(node))
    {
    d->hierarchyTreeView->setCurrentNode(node);
    return true;
    }

  if (vtkMRMLAnnotationDisplayNode::SafeDownCast(node))
    {
    vtkMRMLAnnotationDisplayNode* displayNode = vtkMRMLAnnotationDisplayNode::SafeDownCast(node);
    vtkMRMLAnnotationNode* displayableNode = vtkMRMLAnnotationNode::SafeDownCast(displayNode->GetDisplayableNode());
    if (!displayableNode)
      {
      return false;
      }
    d->hierarchyTreeView->setCurrentNode(displayableNode);
    return true;
    }

  return false;
}
