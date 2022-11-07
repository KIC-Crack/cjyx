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

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Segmentations includes
#include "Logic/vtkCjyxSegmentationsModuleLogic.h"
#include "qMRMLSortFilterSegmentsProxyModel.h"
#include "qCjyxSegmentationsModuleWidget.h"
#include "ui_qCjyxSegmentationsModule.h"

#include "vtkMRMLSegmentationNode.h"
#include "vtkMRMLSegmentationDisplayNode.h"
#include "vtkCjyxSegmentationsModuleLogic.h"

#include "qMRMLSegmentsTableView.h"
#include "qMRMLSegmentationRepresentationsListView.h"

// Terminologies includes
#include "vtkCjyxTerminologiesModuleLogic.h"

// Cjyx includes
#include <qCjyxApplication.h>
#include <qCjyxAbstractModuleWidget.h>
#include <qCjyxSubjectHierarchyAbstractPlugin.h>
#include <qCjyxSubjectHierarchyFolderPlugin.h>
#include <qCjyxSubjectHierarchyPluginHandler.h>

// MRML includes
#include "vtkMRMLScene.h"
#include "vtkMRMLLabelMapVolumeNode.h"
#include "vtkMRMLModelNode.h"
#include "vtkMRMLSubjectHierarchyNode.h"

// VTK includes
#include <vtkCollection.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

// CTK includes
#include <ctkMessageBox.h>

// Qt includes
#include <QAbstractItemView>
#include <QButtonGroup>
#include <QDebug>
#include <QItemSelection>
#include <QMessageBox>

//-----------------------------------------------------------------------------
/// \ingroup CjyxRt_QtModules_Segmentations
class qCjyxSegmentationsModuleWidgetPrivate: public Ui_qCjyxSegmentationsModule
{
  Q_DECLARE_PUBLIC(qCjyxSegmentationsModuleWidget);
protected:
  qCjyxSegmentationsModuleWidget* const q_ptr;
public:
  qCjyxSegmentationsModuleWidgetPrivate(qCjyxSegmentationsModuleWidget& object);
  ~qCjyxSegmentationsModuleWidgetPrivate();
  vtkCjyxSegmentationsModuleLogic* logic() const;
  void populateTerminologyContextComboBox();

public:
  vtkWeakPointer<vtkMRMLSegmentationNode> SegmentationNode;
  /// Using this flag prevents overriding the parameter set node contents when the
  ///   QMRMLCombobox selects the first instance of the specified node type when initializing
  bool ModuleWindowInitialized;

  /// Import/export buttons
  QButtonGroup* ImportExportOperationButtonGroup;
  /// Model/labelmap buttons
  QButtonGroup* ImportExportTypeButtonGroup;
};

//-----------------------------------------------------------------------------
// qCjyxSegmentationsModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxSegmentationsModuleWidgetPrivate::qCjyxSegmentationsModuleWidgetPrivate(qCjyxSegmentationsModuleWidget& object)
  : q_ptr(&object)
  , ModuleWindowInitialized(false)
  , ImportExportOperationButtonGroup(nullptr)
  , ImportExportTypeButtonGroup(nullptr)
{
}

//-----------------------------------------------------------------------------
qCjyxSegmentationsModuleWidgetPrivate::~qCjyxSegmentationsModuleWidgetPrivate() = default;

//-----------------------------------------------------------------------------
vtkCjyxSegmentationsModuleLogic*
qCjyxSegmentationsModuleWidgetPrivate::logic() const
{
  Q_Q(const qCjyxSegmentationsModuleWidget);
  return vtkCjyxSegmentationsModuleLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidgetPrivate::populateTerminologyContextComboBox()
{
  Q_Q(const qCjyxWidget);

  this->ComboBox_TerminologyContext->clear();

  vtkCjyxTerminologiesModuleLogic* terminologiesLogic = vtkCjyxTerminologiesModuleLogic::SafeDownCast(
    qCjyxCoreApplication::application()->moduleLogic("Terminologies"));
  if (!terminologiesLogic)
    {
    qCritical() << Q_FUNC_INFO << ": Terminologies logic is not found";
    return;
    }

  std::vector<std::string> terminologyNames;
  terminologiesLogic->GetLoadedTerminologyNames(terminologyNames);
  for (std::vector<std::string>::iterator termIt=terminologyNames.begin(); termIt!=terminologyNames.end(); ++termIt)
    {
    this->ComboBox_TerminologyContext->addItem(termIt->c_str());
    }
}

//-----------------------------------------------------------------------------
// qCjyxSegmentationsModuleWidget methods

//-----------------------------------------------------------------------------
qCjyxSegmentationsModuleWidget::qCjyxSegmentationsModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qCjyxSegmentationsModuleWidgetPrivate(*this) )
{
}

//-----------------------------------------------------------------------------
qCjyxSegmentationsModuleWidget::~qCjyxSegmentationsModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::enter()
{
  this->onEnter();
  this->Superclass::enter();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::exit()
{
  this->Superclass::exit();

  // remove mrml scene observations, don't need to update the GUI while the
  // module is not showing
  this->qvtkDisconnectAll();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onEnter()
{
  if (!this->mrmlScene())
    {
    qCritical() << Q_FUNC_INFO << ": Invalid scene";
    return;
    }

  Q_D(qCjyxSegmentationsModuleWidget);

  d->ModuleWindowInitialized = true;

  this->qvtkConnect(this->mrmlScene(), vtkMRMLScene::EndImportEvent,
                    this, SLOT(onMRMLSceneEndImportEvent()));
  this->qvtkConnect(this->mrmlScene(), vtkMRMLScene::EndBatchProcessEvent,
                    this, SLOT(onMRMLSceneEndBatchProcessEvent()));
  this->qvtkConnect(this->mrmlScene(), vtkMRMLScene::EndCloseEvent,
                    this, SLOT(onMRMLSceneEndCloseEvent()));
  this->qvtkConnect(this->mrmlScene(), vtkMRMLScene::EndRestoreEvent,
                    this, SLOT(onMRMLSceneEndRestoreEvent()));

  this->onSegmentationNodeChanged( d->MRMLNodeComboBox_Segmentation->currentNode() );

  d->populateTerminologyContextComboBox();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::setup()
{
  this->init();
}

//-----------------------------------------------------------------------------
vtkMRMLSegmentationDisplayNode* qCjyxSegmentationsModuleWidget::segmentationDisplayNode(bool create/*=false*/)
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSegmentationNode* segmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (!segmentationNode)
    {
    return nullptr;
    }

  vtkMRMLSegmentationDisplayNode* displayNode =
    vtkMRMLSegmentationDisplayNode::SafeDownCast( segmentationNode->GetDisplayNode() );
  if (!displayNode && create)
    {
    segmentationNode->CreateDefaultDisplayNodes();
    displayNode = vtkMRMLSegmentationDisplayNode::SafeDownCast( segmentationNode->GetDisplayNode() );
    }
  return displayNode;
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::updateWidgetFromMRML()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  // Don't update widget while there are pending operations.
  // (for example, we may create a new display node while a display node already exists, just the node
  // references have not been finalized yet)
  if (!this->mrmlScene() || this->mrmlScene()->IsBatchProcessing())
    {
    return;
    }

  // Hide the current node in the other segmentation combo box
  QStringList hiddenNodeIDs;
  if (d->SegmentationNode)
    {
    hiddenNodeIDs << QString(d->SegmentationNode->GetID());
    }
  d->MRMLNodeComboBox_OtherSegmentationOrRepresentationNode->sortFilterProxyModel()->setHiddenNodeIDs(hiddenNodeIDs);

  // Update display group from segmentation display node
  d->SegmentationDisplayNodeWidget->setSegmentationNode(d->SegmentationNode);
  d->SegmentationDisplayNodeWidget->updateWidgetFromMRML();

  d->show3DButton->setSegmentationNode(d->SegmentationNode);

  // Update copy/move/import/export buttons from selection
  this->updateCopyMoveButtonStates();

  // Update export color checkbox/selector
  this->updateExportColorWidgets();

  // Update layer info widgets
  this->updateLayerWidgets();

  // Update segment handler button states based on segment selection
  this->onSegmentSelectionChanged(QItemSelection(),QItemSelection());

  // Update master volume label and combobox for export
  this->onSegmentationNodeReferenceChanged();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::updateCopyMoveButtonStates()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  // Disable copy/move buttons then enable later based on selection
  d->toolButton_MoveFromCurrentSegmentation->setEnabled(false);
  d->toolButton_CopyFromCurrentSegmentation->setEnabled(false);
  d->toolButton_CopyToCurrentSegmentation->setEnabled(false);
  d->toolButton_MoveToCurrentSegmentation->setEnabled(false);

  // Set button states that copy/move to current segmentation
  vtkMRMLSegmentationNode* otherSegmentationNode = vtkMRMLSegmentationNode::SafeDownCast(
    d->SegmentsTableView_Other->segmentationNode() );
  if (otherSegmentationNode)
    {
    // All options are possible if other node is segmentation
    d->toolButton_CopyToCurrentSegmentation->setEnabled(true);
    d->toolButton_MoveToCurrentSegmentation->setEnabled(true);
    }

  // Set button states that copy/move from current segmentation
  vtkMRMLSegmentationNode* currentSegmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (currentSegmentationNode && currentSegmentationNode->GetSegmentation()->GetNumberOfSegments() > 0)
    {
    d->toolButton_MoveFromCurrentSegmentation->setEnabled(true);
    d->toolButton_CopyFromCurrentSegmentation->setEnabled(true);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::init()
{
  Q_D(qCjyxSegmentationsModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  // Ensure that four representations fit in the table by default
  d->RepresentationsListView->setMinimumHeight(108);

  d->ImportExportOperationButtonGroup = new QButtonGroup(d->CollapsibleButton_ImportExportSegment);
  d->ImportExportOperationButtonGroup->addButton(d->radioButton_Export);
  d->ImportExportOperationButtonGroup->addButton(d->radioButton_Import);

  d->ImportExportTypeButtonGroup = new QButtonGroup(d->CollapsibleButton_ImportExportSegment);
  d->ImportExportTypeButtonGroup->addButton(d->radioButton_Labelmap);
  d->ImportExportTypeButtonGroup->addButton(d->radioButton_Model);

  // Make connections
  connect(d->MRMLNodeComboBox_Segmentation, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    this, SLOT(onSegmentationNodeChanged(vtkMRMLNode*)) );
  connect(d->MRMLNodeComboBox_Segmentation, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    d->SegmentsTableView, SLOT(setSegmentationNode(vtkMRMLNode*)) );
  connect(d->MRMLNodeComboBox_Segmentation, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    d->SegmentsTableView_Current, SLOT(setSegmentationNode(vtkMRMLNode*)) );
  connect(d->MRMLNodeComboBox_Segmentation, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    d->RepresentationsListView, SLOT(setSegmentationNode(vtkMRMLNode*)) );

  connect(d->SegmentsTableView, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
    this, SLOT(onSegmentSelectionChanged(QItemSelection,QItemSelection)));
  connect(d->pushButton_AddSegment, SIGNAL(clicked()),
    this, SLOT(onAddSegment()) );
  connect(d->toolButton_Edit, SIGNAL(clicked()),
    this, SLOT(onEditSegmentation()) );
  connect(d->pushButton_RemoveSelected, SIGNAL(clicked()),
    this, SLOT(onRemoveSelectedSegments()) );

  connect(d->MRMLNodeComboBox_OtherSegmentationOrRepresentationNode, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    this, SLOT(setOtherSegmentationOrRepresentationNode(vtkMRMLNode*)) );

  connect(d->ImportExportOperationButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
    this, SLOT(updateImportExportWidgets()));

  connect(d->ImportExportTypeButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)),
    this, SLOT(updateImportExportWidgets()));

  connect(d->PushButton_ImportExport, SIGNAL(clicked()),
    this, SLOT(onImportExportApply()));
  connect(d->pushButton_ClearSelection, SIGNAL(clicked()),
    this, SLOT(onImportExportClearSelection()));

  connect(d->UseColorTableValuesCheckBox, SIGNAL(clicked()),
    this, SLOT(updateExportColorWidgets()));
  connect(d->ColorTableNodeSelector, SIGNAL(currentNodeIDChanged(const QString&)),
    this, SLOT(onExportColorTableChanged()));

  d->ExportToFilesWidget->setSettingsKey("ExportSegmentsToFiles");
  connect(d->MRMLNodeComboBox_Segmentation, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
    d->ExportToFilesWidget, SLOT(setSegmentationNode(vtkMRMLNode*)));

  connect(d->toolButton_MoveFromCurrentSegmentation, SIGNAL(clicked()),
    this, SLOT(onMoveFromCurrentSegmentation()) );
  connect(d->toolButton_CopyFromCurrentSegmentation, SIGNAL(clicked()),
    this, SLOT(onCopyFromCurrentSegmentation()) );
  connect(d->toolButton_CopyToCurrentSegmentation, SIGNAL(clicked()),
    this, SLOT(onCopyToCurrentSegmentation()) );
  connect(d->toolButton_MoveToCurrentSegmentation, SIGNAL(clicked()),
    this, SLOT(onMoveToCurrentSegmentation()) );

  connect(d->CollapsibleButton_BinaryLabelmapLayers, SIGNAL(contentsCollapsed(bool)),
    this, SLOT(updateLayerWidgets()));
  connect(d->pushButton_CollapseLayers, SIGNAL(clicked()),
    this, SLOT(collapseLabelmapLayers()));

  // Show only segment names in copy/view segment list and make it non-editable
  d->SegmentsTableView_Current->setSelectionMode(QAbstractItemView::ExtendedSelection);
  d->SegmentsTableView_Current->setHeaderVisible(false);
  d->SegmentsTableView_Current->setVisibilityColumnVisible(false);
  d->SegmentsTableView_Current->setColorColumnVisible(false);
  d->SegmentsTableView_Current->setOpacityColumnVisible(false);
  d->SegmentsTableView_Current->setStatusColumnVisible(false);

  d->SegmentsTableView_Other->setSelectionMode(QAbstractItemView::ExtendedSelection);
  d->SegmentsTableView_Other->setHeaderVisible(false);
  d->SegmentsTableView_Other->setVisibilityColumnVisible(false);
  d->SegmentsTableView_Other->setColorColumnVisible(false);
  d->SegmentsTableView_Other->setOpacityColumnVisible(false);
  d->SegmentsTableView_Other->setStatusColumnVisible(false);

  d->radioButton_Export->setChecked(true);
  d->radioButton_Labelmap->setChecked(true);
  this->updateImportExportWidgets();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onSegmentationNodeChanged(vtkMRMLNode* node)
{
  Q_D(qCjyxSegmentationsModuleWidget);
  if (!this->mrmlScene() || this->mrmlScene()->IsBatchProcessing())
    {
    return;
    }
  if (!d->ModuleWindowInitialized)
    {
    return;
    }

  vtkMRMLSegmentationNode* segmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(node);
  if (segmentationNode)
    {
    // Make sure display node exists
    segmentationNode->CreateDefaultDisplayNodes();
    }

  qvtkReconnect( d->SegmentationNode, segmentationNode, vtkCommand::ModifiedEvent, this, SLOT(updateWidgetFromMRML()) );
  qvtkReconnect( d->SegmentationNode, segmentationNode, vtkMRMLDisplayableNode::DisplayModifiedEvent, this, SLOT(updateWidgetFromMRML()) );
  qvtkReconnect( d->SegmentationNode, segmentationNode, vtkSegmentation::MasterRepresentationModified, this, SLOT(updateWidgetFromMRML()) );
  qvtkReconnect( d->SegmentationNode, segmentationNode, vtkMRMLNode::ReferenceAddedEvent, this, SLOT(onSegmentationNodeReferenceChanged()) );
  qvtkReconnect( d->SegmentationNode, segmentationNode, vtkMRMLNode::ReferenceModifiedEvent, this, SLOT(onSegmentationNodeReferenceChanged()) );
  qvtkReconnect(d->SegmentationNode, segmentationNode, vtkSegmentation::SegmentModified, this, SLOT(updateLayerWidgets()) );

  d->SegmentationNode = segmentationNode;
  d->SegmentationDisplayNodeWidget->setSegmentationNode(segmentationNode);
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::selectSegmentationNode(vtkMRMLSegmentationNode* segmentationNode)
{
  Q_D(qCjyxSegmentationsModuleWidget);

  d->MRMLNodeComboBox_Segmentation->setCurrentNode(segmentationNode);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onSegmentSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
  Q_UNUSED(selected);
  Q_UNUSED(deselected);
  Q_D(qCjyxSegmentationsModuleWidget);

  d->pushButton_AddSegment->setEnabled(d->SegmentationNode != nullptr);

  QStringList selectedSegmentIds = d->SegmentsTableView->selectedSegmentIDs();
  d->toolButton_Edit->setEnabled(d->SegmentationNode != nullptr);
  d->pushButton_RemoveSelected->setEnabled(selectedSegmentIds.count() > 0);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onAddSegment()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSegmentationNode* currentSegmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (!currentSegmentationNode)
    {
    qWarning() << Q_FUNC_INFO << ": No segmentation selected";
    return;
    }

  // Create empty segment in current segmentation
  std::string addedSegmentID = currentSegmentationNode->GetSegmentation()->AddEmptySegment(d->SegmentsTableView->textFilter().toStdString());
  int status = 0;
  for (int i = 0; i < vtkCjyxSegmentationsModuleLogic::LastStatus; ++i)
    {
    if (d->SegmentsTableView->sortFilterProxyModel()->showStatus(i))
      {
      status = i;
      break;
      }
    }
  vtkCjyxSegmentationsModuleLogic::SetSegmentStatus(currentSegmentationNode->GetSegmentation()->GetSegment(addedSegmentID), status);

  // Select the new segment
  if (!addedSegmentID.empty())
    {
    QStringList segmentIDList;
    segmentIDList << QString(addedSegmentID.c_str());
    d->SegmentsTableView->setSelectedSegmentIDs(segmentIDList);
    }

  // Assign the new segment the terminology of the (now second) last segment
  if (currentSegmentationNode->GetSegmentation()->GetNumberOfSegments() > 1)
    {
    vtkSegment* secondLastSegment = currentSegmentationNode->GetSegmentation()->GetNthSegment(
      currentSegmentationNode->GetSegmentation()->GetNumberOfSegments() - 2 );
    std::string repeatedTerminologyEntry("");
    secondLastSegment->GetTag(secondLastSegment->GetTerminologyEntryTagName(), repeatedTerminologyEntry);
    currentSegmentationNode->GetSegmentation()->GetSegment(addedSegmentID)->SetTag(
      secondLastSegment->GetTerminologyEntryTagName(), repeatedTerminologyEntry );
    }
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onEditSegmentation()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  if (!d->MRMLNodeComboBox_Segmentation->currentNode())
    {
    qCritical() << Q_FUNC_INFO << ": Invalid segmentation";
    return;
    }

  QStringList segmentID;
  if (d->SegmentsTableView->selectedSegmentIDs().count() > 0)
    {
    segmentID << d->SegmentsTableView->selectedSegmentIDs()[0];
    }

  // Switch to Segment Editor module, select segmentation node and segment ID
  qCjyxAbstractModuleWidget* moduleWidget = qCjyxSubjectHierarchyAbstractPlugin::switchToModule("SegmentEditor");
  if (!moduleWidget)
    {
    qCritical() << Q_FUNC_INFO << ": Segment Editor module is not available";
    return;
    }

  if (!segmentID.empty())
    {
    // Get segmentation selector combobox and set segmentation
    qMRMLNodeComboBox* nodeSelector = moduleWidget->findChild<qMRMLNodeComboBox*>("SegmentationNodeComboBox");
    if (!nodeSelector)
      {
      qCritical() << Q_FUNC_INFO << ": MRMLNodeComboBox_Segmentation is not found in Segment Editor module";
      return;
      }
    nodeSelector->setCurrentNode(d->MRMLNodeComboBox_Segmentation->currentNode());

    // Get segments table and select segment
    qMRMLSegmentsTableView* segmentsTable = moduleWidget->findChild<qMRMLSegmentsTableView*>("SegmentsTableView");
    if (!segmentsTable)
      {
      qCritical() << Q_FUNC_INFO << ": SegmentsTableView is not found in Segment Editor module";
      return;
      }
    segmentsTable->setSelectedSegmentIDs(segmentID);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onRemoveSelectedSegments()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSegmentationNode* currentSegmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (!currentSegmentationNode)
    {
    qCritical() << Q_FUNC_INFO << ": No segmentation selected";
    return;
    }

  QStringList selectedSegmentIds = d->SegmentsTableView->selectedSegmentIDs();
  foreach (QString segmentId, selectedSegmentIds)
    {
    currentSegmentationNode->GetSegmentation()->RemoveSegment(segmentId.toUtf8().constData());
    }
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::setOtherSegmentationOrRepresentationNode(vtkMRMLNode* node)
{
  Q_D(qCjyxSegmentationsModuleWidget);

  if (!this->mrmlScene())
    {
    qCritical() << Q_FUNC_INFO << ": Invalid scene";
    return;
    }
  if (!d->ModuleWindowInitialized)
    {
    return;
    }

  // Decide if segmentation or representation node
  vtkMRMLSegmentationNode* segmentationNode = vtkMRMLSegmentationNode::SafeDownCast(node);
  d->SegmentsTableView_Other->setSegmentationNode(segmentationNode);

  // Update widgets based on selection
  this->updateCopyMoveButtonStates();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::collapseLabelmapLayers()
{
  Q_D(qCjyxSegmentationsModuleWidget);
  if (!d->SegmentationNode)
    {
    return;
    }

  bool forceToSingleLayer = d->checkBox_OverwriteSegments->isChecked();
  vtkCjyxSegmentationsModuleLogic::CollapseBinaryLabelmaps(d->SegmentationNode, forceToSingleLayer);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::updateLayerWidgets()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  if (d->CollapsibleButton_BinaryLabelmapLayers->collapsed())
    {
    return;
    }

  std::stringstream segmentCountSS;
  if (!d->SegmentationNode)
    {
    segmentCountSS << "0";
    }
  else
    {
    segmentCountSS << d->SegmentationNode->GetSegmentation()->GetNumberOfSegments();
    }
  d->label_SegmentCountValue->setText(QString::fromStdString(segmentCountSS.str()));

  std::stringstream layerCountSS;
  if (!d->SegmentationNode)
    {
    layerCountSS << "0";
    }
  else
    {
    layerCountSS << d->SegmentationNode->GetSegmentation()->GetNumberOfLayers(vtkSegmentationConverter::GetBinaryLabelmapRepresentationName());
    }
  d->label_LayerCountValue->setText(QString::fromStdString(layerCountSS.str()));
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::updateImportExportWidgets()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  // Operation: export/import
  if (d->radioButton_Export->isChecked())
    {
    d->label_ImportExportType->setText("Output type:");
    d->label_ImportExportNode->setText("Output node:");
    d->PushButton_ImportExport->setText("Export");
    d->label_TerminologyContext->setVisible(false);
    d->ComboBox_TerminologyContext->setVisible(false);
    }
  else // Import
    {
    d->label_ImportExportType->setText("Input type:");
    d->label_ImportExportNode->setText("Input node:");
    d->PushButton_ImportExport->setText("Import");
    d->label_TerminologyContext->setVisible(d->radioButton_Labelmap->isChecked());
    d->ComboBox_TerminologyContext->setVisible(d->radioButton_Labelmap->isChecked());
    }
  d->ComboBox_ExportedSegments->setEnabled(d->radioButton_Export->isChecked());
  d->MRMLNodeComboBox_ExportLabelmapReferenceVolume->setEnabled(
    d->radioButton_Labelmap->isChecked() && d->radioButton_Export->isChecked());
  d->pushButton_ClearSelection->setVisible(d->radioButton_Export->isChecked());

  // Type: labelmap/model
  QStringList nodeTypes;
  QStringList levelFilter;
  if (d->radioButton_Labelmap->isChecked())
    {
    nodeTypes << "vtkMRMLLabelMapVolumeNode";
    if (d->radioButton_Export->isChecked())
      {
      d->SubjectHierarchyComboBox_ImportExport->setDefaultText("Export to new labelmap");
      }
    }
  else // Model
    {
    if (d->radioButton_Export->isChecked())
      {
      nodeTypes << "vtkMRMLFolderDisplayNode"; // Do not show any data nodes (folder display node belongs to folders)
      d->SubjectHierarchyComboBox_ImportExport->setDefaultText("Export models to new folder");
      // Show only hierarchy items (folder, study, patient)
      levelFilter << vtkMRMLSubjectHierarchyConstants::GetSubjectHierarchyLevelFolder()
        << vtkMRMLSubjectHierarchyConstants::GetDICOMLevelStudy().c_str()
        << vtkMRMLSubjectHierarchyConstants::GetDICOMLevelPatient().c_str();
      }
    else // Import
      {
      nodeTypes << "vtkMRMLModelNode";
      }
    }
  d->SubjectHierarchyComboBox_ImportExport->setNodeTypes(nodeTypes);
  d->SubjectHierarchyComboBox_ImportExport->setLevelFilter(levelFilter);

  d->MRMLNodeComboBox_ExportLabelmapReferenceVolume->setEnabled(
    d->radioButton_Labelmap->isChecked() && d->radioButton_Export->isChecked() );
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::updateExportColorWidgets()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  QSignalBlocker blocker1(d->ColorTableNodeSelector);
  d->ColorTableNodeSelector->setEnabled(d->UseColorTableValuesCheckBox->isChecked());

  vtkMRMLColorTableNode* exportColorTableNode = nullptr;
  if (d->SegmentationNode)
    {
    exportColorTableNode = d->SegmentationNode->GetLabelmapConversionColorTableNode();
    }

  QSignalBlocker blocker2(d->ColorTableNodeSelector);
  d->ColorTableNodeSelector->setCurrentNode(exportColorTableNode);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onExportColorTableChanged()
{
  Q_D(qCjyxSegmentationsModuleWidget);
  if (!d->SegmentationNode)
    {
    return;
    }

  std::string currentNodeID = d->ColorTableNodeSelector->currentNodeID().toStdString();
  d->SegmentationNode->SetLabelmapConversionColorTableNodeID(currentNodeID.c_str());
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onImportExportApply()
{
  Q_D(qCjyxSegmentationsModuleWidget);
  if (d->radioButton_Export->isChecked())
    {
    this->exportFromCurrentSegmentation();
    }
  else
    {
    this->importToCurrentSegmentation();
    }
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onImportExportClearSelection()
{
  Q_D(qCjyxSegmentationsModuleWidget);
  d->SubjectHierarchyComboBox_ImportExport->clearSelection();
  this->updateImportExportWidgets();
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsModuleWidget::copySegmentBetweenSegmentations(
  vtkSegmentation* fromSegmentation, vtkSegmentation* toSegmentation,
  QString segmentId, bool removeFromSource/*=false*/ )
{
  if (!fromSegmentation || !toSegmentation || segmentId.isEmpty())
    {
    return false;
    }

  std::string segmentIdStd(segmentId.toUtf8().constData());

  // Get segment
  vtkSegment* segment = fromSegmentation->GetSegment(segmentIdStd);
  if (!segment)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to get segment";
    return false;
    }

  // If target segmentation is empty, make it match the source
  if (toSegmentation->GetNumberOfSegments()==0)
    {
    toSegmentation->SetMasterRepresentationName(fromSegmentation->GetMasterRepresentationName());
    }

  // Check whether target is suitable to accept the segment.
  if (!toSegmentation->CanAcceptSegment(segment))
    {
    qCritical() << Q_FUNC_INFO << ": Segmentation cannot accept segment " << segment->GetName();

    // Pop up error message to the user explaining the problem
    vtkMRMLSegmentationNode* fromNode = vtkCjyxSegmentationsModuleLogic::GetSegmentationNodeForSegmentation(this->mrmlScene(), fromSegmentation);
    vtkMRMLSegmentationNode* toNode = vtkCjyxSegmentationsModuleLogic::GetSegmentationNodeForSegmentation(this->mrmlScene(), toSegmentation);
    if (!fromNode || !toNode) // Sanity check, should never happen
      {
      qCritical() << Q_FUNC_INFO << ": Unable to get parent nodes for segmentation objects";
      return false;
      }

    QString message = QString("Cannot convert source master representation '%1' into target master '%2', "
      "thus unable to copy segment '%3' from segmentation '%4' to '%5'.\n\nWould you like to change the master representation of '%5' to '%1'?\n\n"
      "Note: This may result in unwanted data loss in %5.")
      .arg(fromSegmentation->GetMasterRepresentationName().c_str())
      .arg(toSegmentation->GetMasterRepresentationName().c_str()).arg(segmentId).arg(fromNode->GetName()).arg(toNode->GetName());
    QMessageBox::StandardButton answer =
      QMessageBox::question(nullptr, tr("Failed to copy segment"), message,
      QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (answer == QMessageBox::Yes)
      {
      // Convert target segmentation to master representation of source segmentation
      bool successfulConversion = toSegmentation->CreateRepresentation(fromSegmentation->GetMasterRepresentationName());
      if (!successfulConversion)
        {
        QString message = QString("Failed to convert %1 to %2!").arg(toNode->GetName()).arg(fromSegmentation->GetMasterRepresentationName().c_str());
        QMessageBox::warning(nullptr, tr("Conversion failed"), message);
        return false;
        }

      // Change master representation of target to that of source
      toSegmentation->SetMasterRepresentationName(fromSegmentation->GetMasterRepresentationName());

      // Retry copy of segment
      return this->copySegmentBetweenSegmentations(fromSegmentation, toSegmentation, segmentId, removeFromSource);
      }

    return false;
    }

  // Perform the actual copy/move operation
  return toSegmentation->CopySegmentFromSegmentation(fromSegmentation, segmentIdStd, removeFromSource);
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsModuleWidget::copySegmentsBetweenSegmentations(bool copyFromCurrentSegmentation, bool removeFromSource/*=false*/)
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSegmentationNode* currentSegmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (!currentSegmentationNode)
    {
    qWarning() << Q_FUNC_INFO << ": No current segmentation is selected";
    return false;
    }

  vtkMRMLSegmentationNode* otherSegmentationNode = vtkMRMLSegmentationNode::SafeDownCast(
    d->SegmentsTableView_Other->segmentationNode() );
  if (!otherSegmentationNode)
    {
    qWarning() << Q_FUNC_INFO << ": No other segmentation is selected";
    return false;
    }

  // Get source and target segmentation
  QStringList selectedSegmentIds;
  vtkSegmentation* sourceSegmentation = nullptr;
  vtkSegmentation* targetSegmentation = nullptr;
  if (copyFromCurrentSegmentation)
    {
    sourceSegmentation = currentSegmentationNode->GetSegmentation();
    targetSegmentation = otherSegmentationNode->GetSegmentation();
    otherSegmentationNode->CreateDefaultDisplayNodes();
    selectedSegmentIds = d->SegmentsTableView_Current->selectedSegmentIDs();
    }
  else
    {
    sourceSegmentation = otherSegmentationNode->GetSegmentation();
    targetSegmentation = currentSegmentationNode->GetSegmentation();
    currentSegmentationNode->CreateDefaultDisplayNodes();
    selectedSegmentIds = d->SegmentsTableView_Other->selectedSegmentIDs();
    }

  if (selectedSegmentIds.empty())
    {
    qWarning() << Q_FUNC_INFO << ": No segments are selected";
    return false;
    }

  // Copy/move segments
  bool success = true;
  foreach(QString segmentId, selectedSegmentIds)
    {
    success = success && this->copySegmentBetweenSegmentations(sourceSegmentation,
      targetSegmentation, segmentId, removeFromSource);
    }

  return success;
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsModuleWidget::exportFromCurrentSegmentation()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(this->mrmlScene());
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }

  vtkMRMLSegmentationNode* currentSegmentationNode =  vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode() );
  if (!currentSegmentationNode || !currentSegmentationNode->GetSegmentation())
    {
    qWarning() << Q_FUNC_INFO << ": No segmentation selected";
    return false;
    }

  // Get IDs of segments to be exported
  std::vector<std::string> segmentIDs;
  if (d->ComboBox_ExportedSegments->currentIndex() == 0)
    {
    // All segments
    currentSegmentationNode->GetSegmentation()->GetSegmentIDs(segmentIDs);
    }
  else
    {
    // Visible segments
    vtkMRMLSegmentationDisplayNode* displayNode = vtkMRMLSegmentationDisplayNode::SafeDownCast(currentSegmentationNode->GetDisplayNode());
    displayNode->GetVisibleSegmentIDs(segmentIDs);
    }

  vtkMRMLVolumeNode* referenceVolumeNode = vtkMRMLVolumeNode::SafeDownCast(d->MRMLNodeComboBox_ExportLabelmapReferenceVolume->currentNode());
  if (referenceVolumeNode)
    {
    vtkNew<vtkStringArray> segmentIDsArray;
    segmentIDsArray->SetNumberOfValues(segmentIDs.size());
    for (int i = 0; i < segmentIDs.size(); ++i)
      {
      segmentIDsArray->SetValue(i, segmentIDs[i]);
      }

    if (vtkCjyxSegmentationsModuleLogic::IsEffectiveExentOutsideReferenceVolume(
      referenceVolumeNode, d->SegmentationNode, segmentIDsArray))
      {
      ctkMessageBox* exportWarningMesssgeBox = new ctkMessageBox(this);
      exportWarningMesssgeBox->setAttribute(Qt::WA_DeleteOnClose);
      exportWarningMesssgeBox->setWindowTitle("Export may erase data");
      exportWarningMesssgeBox->addButton(QMessageBox::StandardButton::Ok);
      exportWarningMesssgeBox->addButton(QMessageBox::StandardButton::Cancel);
      exportWarningMesssgeBox->setDontShowAgainVisible(true);
      exportWarningMesssgeBox->setIcon(QMessageBox::Warning);
      exportWarningMesssgeBox->setDontShowAgainSettingsKey("Segmentations/AlwaysCropDuringSegmentationNodeExport");
      exportWarningMesssgeBox->setText("The current segmentation does not completely fit into the new geometry.\n"
                                       "Do you want to crop the segmentation?\n");
      if (exportWarningMesssgeBox->exec() != QMessageBox::StandardButton::Ok)
        {
        return false;
        }
      }
    }

  // Get selected item
  vtkIdType selectedItem = d->SubjectHierarchyComboBox_ImportExport->currentItem();
  vtkIdType folderItem = vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID; // Often exporting into a folder

  // Determine if selected item is a folder
  qCjyxSubjectHierarchyFolderPlugin* folderPlugin = qobject_cast<qCjyxSubjectHierarchyFolderPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Folder") );
  if (folderPlugin->canOwnSubjectHierarchyItem(selectedItem) > 0.0)
    {
    folderItem = selectedItem;
    }

  // Create new labelmap if exporting to labelmap and selection was not an existing labelmap
  vtkMRMLLabelMapVolumeNode* labelmapNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(
    shNode->GetItemDataNode(selectedItem) );
  if (d->radioButton_Labelmap->isChecked() && !labelmapNode)
    {
    // Add segment name to node name if only one segment is exported
    std::string exportedNodeName(currentSegmentationNode->GetName());
    if (segmentIDs.size() == 1)
      {
      exportedNodeName += "-" + std::string(currentSegmentationNode->GetSegmentation()->GetSegment(segmentIDs[0])->GetName());
      }
    exportedNodeName += "-label";

    vtkSmartPointer<vtkMRMLNode> newNode = vtkSmartPointer<vtkMRMLNode>::Take(
      currentSegmentationNode->GetScene()->CreateNodeByClass("vtkMRMLLabelMapVolumeNode"));
    vtkMRMLLabelMapVolumeNode* newLabelmapNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(
      currentSegmentationNode->GetScene()->AddNode(newNode));
    newLabelmapNode->SetName(this->mrmlScene()->GetUniqueNameByString(exportedNodeName.c_str()));
    newLabelmapNode->CreateDefaultDisplayNodes();

    // Move new labelmap under folder if selected
    if (folderItem)
      {
      vtkIdType newLabelmapItem = shNode->GetItemByDataNode(newLabelmapNode);
      shNode->SetItemParent(newLabelmapItem, folderItem);
      }

    labelmapNode = newLabelmapNode;
    }

  // Create folder if exporting to model and there was no folder selection
  if (d->radioButton_Model->isChecked() && !folderItem)
    {
    std::string exportedItemName(currentSegmentationNode->GetName());
    exportedItemName += "-models";

    folderItem = shNode->CreateFolderItem(shNode->GetSceneItemID(), shNode->GenerateUniqueItemName(exportedItemName));
    }

  vtkMRMLColorTableNode* colorTableNode = nullptr;
  if (d->UseColorTableValuesCheckBox->isChecked())
    {
    colorTableNode = vtkMRMLColorTableNode::SafeDownCast(d->ColorTableNodeSelector->currentNode());
    }

  // Do the export
  if (labelmapNode)
    {
    // Export selected segments into a multi-label labelmap volume
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    bool success = vtkCjyxSegmentationsModuleLogic::ExportSegmentsToLabelmapNode(currentSegmentationNode, segmentIDs, labelmapNode,
      referenceVolumeNode, vtkSegmentation::EXTENT_UNION_OF_EFFECTIVE_SEGMENTS_AND_REFERENCE_GEOMETRY, colorTableNode);
    QApplication::restoreOverrideCursor();
    if (!success)
      {
      QString message = QString("Failed to export segments from segmentation %1 to labelmap node %2!\n\n"
        "Most probably the segment cannot be converted into binary labelmap representation.").
        arg(currentSegmentationNode->GetName()).arg(labelmapNode->GetName());
      qCritical() << Q_FUNC_INFO << ": " << message;
      QMessageBox::warning(nullptr, tr("Failed to export segments to labelmap"), message);
      return false;
      }
    }
  else if (folderItem)
    {
    // Export selected segments into a models, a model node from each segment
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    bool success = vtkCjyxSegmentationsModuleLogic::ExportSegmentsToModels(currentSegmentationNode, segmentIDs, folderItem);
    QApplication::restoreOverrideCursor();
    if (!success)
      {
      QString message = QString("Failed to export segments from segmentation %1 to models in folder %2!\n\n"
        "Most probably the segment cannot be converted into closed surface representation.").
        arg(currentSegmentationNode->GetName()).arg(shNode->GetItemName(folderItem).c_str());
      qCritical() << Q_FUNC_INFO << ": " << message;
      QMessageBox::warning(nullptr, tr("Failed to export segments to models"), message);
      return false;
      }
    }

  return true;
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsModuleWidget::importToCurrentSegmentation()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  vtkMRMLSegmentationNode* currentSegmentationNode = vtkMRMLSegmentationNode::SafeDownCast(
    d->MRMLNodeComboBox_Segmentation->currentNode());
  if (!currentSegmentationNode)
    {
    qWarning() << Q_FUNC_INFO << ": No segmentation selected";
    return false;
    }

  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(this->mrmlScene());
  if (!shNode)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access subject hierarchy node";
    return false;
    }

  currentSegmentationNode->CreateDefaultDisplayNodes();

  // Get selected item
  vtkIdType selectedItem = d->SubjectHierarchyComboBox_ImportExport->currentItem();

  // Determine if selected item is a folder, labelmap, or model
  vtkIdType folderItem = vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID;
  qCjyxSubjectHierarchyFolderPlugin* folderPlugin = qobject_cast<qCjyxSubjectHierarchyFolderPlugin*>(
  qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Folder") );
  if (folderPlugin->canOwnSubjectHierarchyItem(selectedItem) > 0.0)
    {
    folderItem = selectedItem;
    }
  vtkMRMLLabelMapVolumeNode* labelmapNode = vtkMRMLLabelMapVolumeNode::SafeDownCast(
    shNode->GetItemDataNode(selectedItem) );
  vtkMRMLModelNode* modelNode = vtkMRMLModelNode::SafeDownCast(
    shNode->GetItemDataNode(selectedItem) );

  QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
  if (labelmapNode)
    {
    std::string currentTerminologyContextName(
      d->ComboBox_TerminologyContext->currentText() == d->ComboBox_TerminologyContext->defaultText()
        ? "" : d->ComboBox_TerminologyContext->currentText().toUtf8().constData());
    bool success = d->logic()->ImportLabelmapToSegmentationNodeWithTerminology(
      labelmapNode, currentSegmentationNode, currentTerminologyContextName);
    QApplication::restoreOverrideCursor();
    if (!success)
      {
      QString message = QString("Failed to copy labels from labelmap volume node %1!").arg(labelmapNode->GetName());
      qCritical() << Q_FUNC_INFO << ": " << message;
      QMessageBox::warning(nullptr, tr("Failed to import labelmap volume"), message);
      QApplication::restoreOverrideCursor();
      return false;
      }
    }
  else if (modelNode)
    {
    if (!vtkCjyxSegmentationsModuleLogic::ImportModelToSegmentationNode(modelNode, currentSegmentationNode))
      {
      QString message = QString("Failed to copy polydata from model node %1!").arg(modelNode->GetName());
      qCritical() << Q_FUNC_INFO << ": " << message;
      QMessageBox::warning(nullptr, tr("Failed to import model node"), message);
      QApplication::restoreOverrideCursor();
      return false;
      }
    }
  else if (folderItem)
    {
    if (!vtkCjyxSegmentationsModuleLogic::ImportModelsToSegmentationNode(folderItem, currentSegmentationNode))
      {
      QString message = QString("Failed to copy polydata from models under folder %1!").arg(shNode->GetItemName(folderItem).c_str());
      qCritical() << Q_FUNC_INFO << ": " << message;
      QMessageBox::warning(nullptr, tr("Failed to import models"), message);
      QApplication::restoreOverrideCursor();
      return false;
      }
    }
  QApplication::restoreOverrideCursor();

  return true;
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMoveFromCurrentSegmentation()
{
  this->copySegmentsBetweenSegmentations(true, true);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onCopyFromCurrentSegmentation()
{
  this->copySegmentsBetweenSegmentations(true, false);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onCopyToCurrentSegmentation()
{
  this->copySegmentsBetweenSegmentations(false, false);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMoveToCurrentSegmentation()
{
  this->copySegmentsBetweenSegmentations(false, true);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMRMLSceneEndImportEvent()
{
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMRMLSceneEndRestoreEvent()
{
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMRMLSceneEndBatchProcessEvent()
{
  if (!this->mrmlScene())
    {
    return;
    }
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onMRMLSceneEndCloseEvent()
{
  if (!this->mrmlScene() || this->mrmlScene()->IsBatchProcessing())
    {
    return;
    }
  this->updateWidgetFromMRML();
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsModuleWidget::setEditedNode(
    vtkMRMLNode* node,
    QString role/*=QString()*/,
    QString context/*=QString()*/)
{
  Q_D(qCjyxSegmentationsModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);
  if (vtkMRMLSegmentationNode::SafeDownCast(node))
    {
    d->MRMLNodeComboBox_Segmentation->setCurrentNode(node);
    return true;
    }
  return false;
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModuleWidget::onSegmentationNodeReferenceChanged()
{
  Q_D(qCjyxSegmentationsModuleWidget);

  this->updateExportColorWidgets();

  vtkMRMLNode* referenceVolumeNode = nullptr;
  if (d->SegmentationNode)
    {
    referenceVolumeNode = d->SegmentationNode->GetNodeReference(vtkMRMLSegmentationNode::GetReferenceImageGeometryReferenceRole().c_str());
    }
  if (referenceVolumeNode)
    {
    // Reference volume is available
    // Get reference volume node
    vtkMRMLNode* referenceVolumeNode = d->SegmentationNode->GetNodeReference(
      vtkMRMLSegmentationNode::GetReferenceImageGeometryReferenceRole().c_str());

    // If there is a reference volume, then show labels
    d->label_ReferenceVolumeText->setVisible(true);
    d->label_ReferenceVolumeName->setVisible(true);
    d->label_ReferenceVolumeName->setText(referenceVolumeNode->GetName());

    d->MRMLNodeComboBox_ExportLabelmapReferenceVolume->setCurrentNode(referenceVolumeNode);
    }
  else
    {
    d->label_ReferenceVolumeText->setVisible(false);
    d->label_ReferenceVolumeName->setVisible(false);
    d->MRMLNodeComboBox_ExportLabelmapReferenceVolume->setCurrentNode(nullptr);
    }
}
