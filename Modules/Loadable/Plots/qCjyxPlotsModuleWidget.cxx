/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright 2015 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Andras Lasso (PerkLab, Queen's
  University) and Kevin Wang (Princess Margaret Hospital, Toronto) and was
  supported through OCAIRO and the Applied Cancer Research Unit program of
  Cancer Care Ontario.

==============================================================================*/

// Qt includes
#include <QDebug>

// C++ includes
#include <cmath>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxLayoutManager.h"
#include "qCjyxPlotsModuleWidget.h"
#include "ui_qCjyxPlotsModuleWidget.h"

// vtkCjyxLogic includes
#include "vtkCjyxPlotsLogic.h"

// MRMLWidgets includes
#include <qMRMLUtils.h>
//#include <qMRMLPlotModel.h>

// MRML includes
#include <vtkMRMLLayoutNode.h>
#include "vtkMRMLPlotChartNode.h"
#include "vtkMRMLPlotSeriesNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLSelectionNode.h"

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>
#include <vtkPlot.h>

//-----------------------------------------------------------------------------
class qCjyxPlotsModuleWidgetPrivate: public Ui_qCjyxPlotsModuleWidget
{
  Q_DECLARE_PUBLIC(qCjyxPlotsModuleWidget);
protected:
  qCjyxPlotsModuleWidget* const q_ptr;
public:
  qCjyxPlotsModuleWidgetPrivate(qCjyxPlotsModuleWidget& object);

  vtkCjyxPlotsLogic*      logic()const;

  vtkWeakPointer<vtkMRMLPlotChartNode> MRMLPlotChartNode;
  vtkWeakPointer<vtkMRMLPlotSeriesNode> MRMLPlotSeriesNode;
};

//-----------------------------------------------------------------------------
qCjyxPlotsModuleWidgetPrivate::qCjyxPlotsModuleWidgetPrivate(qCjyxPlotsModuleWidget& object)
  : q_ptr(&object)
{
  this->MRMLPlotChartNode = nullptr;
  this->MRMLPlotSeriesNode = nullptr;
}
//-----------------------------------------------------------------------------
vtkCjyxPlotsLogic* qCjyxPlotsModuleWidgetPrivate::logic()const
{
  Q_Q(const qCjyxPlotsModuleWidget);
  return vtkCjyxPlotsLogic::SafeDownCast(q->logic());
}

/*
//-----------------------------------------------------------------------------
vtkPlot* qCjyxPlotsModuleWidgetPrivate::table()const
{
  if (this->MRMLPlotChartNode.GetPointer()==nullptr)
    {
    return nullptr;
    }
  return this->MRMLPlotChartNode->GetPlot();
}
*/

//-----------------------------------------------------------------------------
qCjyxPlotsModuleWidget::qCjyxPlotsModuleWidget(QWidget* _parentWidget)
  : Superclass(_parentWidget)
  , d_ptr(new qCjyxPlotsModuleWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxPlotsModuleWidget::~qCjyxPlotsModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::setup()
{
  Q_D(qCjyxPlotsModuleWidget);
  d->setupUi(this);

  this->connect(d->PlotChartNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)), SLOT(onNodeSelected(vtkMRMLNode*)));

  this->connect(d->PlotSeriesNodeSelector, SIGNAL(nodeAddedByUser(vtkMRMLNode*)), SLOT(onSeriesNodeAddedByUser(vtkMRMLNode*)));
  this->connect(d->PlotChartPropertiesWidget, SIGNAL(seriesNodeAddedByUser(vtkMRMLNode*)), SLOT(onSeriesNodeAddedByUser(vtkMRMLNode*)));

  this->connect(d->ShowChartButton, SIGNAL(clicked()), SLOT(onShowChartButtonClicked()));
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onNodeSelected(vtkMRMLNode* node)
{
  Q_D(qCjyxPlotsModuleWidget);
  vtkMRMLPlotChartNode* chartNode = vtkMRMLPlotChartNode::SafeDownCast(node);

  this->qvtkReconnect(d->MRMLPlotChartNode, chartNode, vtkCommand::ModifiedEvent, this, SLOT(onMRMLPlotChartNodeModified(vtkObject*)));
  d->MRMLPlotChartNode = chartNode;

  // Update GUI from the newly selected node
  //this->onMRMLPlotChartNodeModified(d->MRMLPlotChartNode);
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onMRMLPlotChartNodeModified(vtkObject* caller)
{
  Q_UNUSED(caller);
  /*
  Q_D(qCjyxPlotsModuleWidget);
#ifndef QT_NO_DEBUG
  vtkMRMLPlotChartNode* tableNode = vtkMRMLPlotChartNode::SafeDownCast(caller);
  Q_ASSERT(d->MRMLPlotChartNode == tableNode);
#else
  Q_UNUSED(caller);
#endif

  bool validNode = d->MRMLPlotChartNode != 0;
  bool editableNode = d->MRMLPlotChartNode != 0 && !d->MRMLPlotChartNode->GetLocked();

  d->DisplayEditCollapsibleWidget->setEnabled(validNode);
  d->LockPlotButton->setEnabled(validNode);
  d->CopyButton->setEnabled(validNode);
  d->PasteButton->setEnabled(editableNode);
  d->EditControlsFrame->setEnabled(editableNode);

  if (!d->MRMLPlotChartNode)
    {
    return;
    }

  if (d->MRMLPlotChartNode->GetLocked())
    {
    d->LockPlotButton->setIcon(QIcon(":Icons/Medium/CjyxLock.png"));
    d->LockPlotButton->setToolTip(QString("Click to unlock this table so that values can be modified"));
    }
  else
    {
    d->LockPlotButton->setIcon(QIcon(":Icons/Medium/CjyxUnlock.png"));
    d->LockPlotButton->setToolTip(QString("Click to lock this table to prevent modification of the values in the user interface"));
    }

  if (d->MRMLPlotChartNode->GetUseColumnNameAsColumnHeader() != d->LockFirstRowButton->isChecked())
    {
    bool wasBlocked = d->LockFirstRowButton->blockSignals(true);
    d->LockFirstRowButton->setChecked(d->MRMLPlotChartNode->GetUseColumnNameAsColumnHeader());
    d->LockFirstRowButton->blockSignals(wasBlocked);
    }

  if (d->MRMLPlotChartNode->GetUseFirstColumnAsRowHeader() != d->LockFirstColumnButton->isChecked())
    {
    bool wasBlocked = d->LockFirstColumnButton->blockSignals(true);
    d->LockFirstColumnButton->setChecked(d->MRMLPlotChartNode->GetUseFirstColumnAsRowHeader());
    d->LockFirstColumnButton->blockSignals(wasBlocked);
    }
    */
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onLockPlotButtonClicked()
{
  Q_D(qCjyxPlotsModuleWidget);

  if (!d->MRMLPlotChartNode)
    {
    return;
    }

  // toggle the lock
  //int locked = d->MRMLPlotChartNode->GetLocked();
  //d->MRMLPlotChartNode->SetLocked(!locked);
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::setCurrentPlotNode(vtkMRMLNode* tableNode)
{
  Q_D(qCjyxPlotsModuleWidget);
  d->PlotChartNodeSelector->setCurrentNode(tableNode);
  d->PlotsTabWidget->setCurrentIndex(0);
}

//-----------------------------------------------------------
bool qCjyxPlotsModuleWidget::setEditedNode(vtkMRMLNode* node,
                                              QString role /* = QString()*/,
                                              QString context /* = QString()*/)
{
  Q_D(qCjyxPlotsModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);

  if (vtkMRMLPlotChartNode::SafeDownCast(node))
    {
    d->PlotChartNodeSelector->setCurrentNode(node);
    d->PlotsTabWidget->setCurrentIndex(0);
    return true;
    }
  else if (vtkMRMLPlotSeriesNode::SafeDownCast(node))
    {
    d->PlotSeriesNodeSelector->setCurrentNode(node);
    d->PlotsTabWidget->setCurrentIndex(1);
    return true;
    }

  return false;
}

// --------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onCopyPlotSeriesNodeClicked()
{
  Q_D(const qCjyxPlotsModuleWidget);

  if (!d->MRMLPlotSeriesNode)
    {
    return;
    }

  vtkCjyxPlotsLogic* logic = d->logic();
  if (!logic)
    {
    qCritical() << Q_FUNC_INFO << "failed: plot logic is not set";
    return;
    }


  vtkMRMLPlotSeriesNode *clonedSeriesNode = logic->CloneSeries(d->MRMLPlotSeriesNode, nullptr);

  // Add the cloned node to the selected chart node
  if (d->MRMLPlotChartNode!= nullptr)
    {
    d->MRMLPlotChartNode->AddAndObservePlotSeriesNodeID(clonedSeriesNode->GetID());
    }
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onShowChartButtonClicked()
{
  Q_D(qCjyxPlotsModuleWidget);

  if (!d->MRMLPlotChartNode)
    {
    return;
    }

  vtkCjyxPlotsLogic* logic = d->logic();
  if (!logic)
    {
    qCritical() << Q_FUNC_INFO << "failed: plot logic is not set";
    return;
    }

  logic->ShowChartInLayout(d->MRMLPlotChartNode);
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModuleWidget::onSeriesNodeAddedByUser(vtkMRMLNode* addedNode)
{
  Q_D(qCjyxPlotsModuleWidget);

  if (!d->MRMLPlotChartNode)
    {
    return;
    }
  vtkMRMLPlotSeriesNode* addedSeriesNode = vtkMRMLPlotSeriesNode::SafeDownCast(addedNode);
  if (!addedSeriesNode)
    {
    return;
    }
  addedSeriesNode->SetUniqueColor();

  d->PlotSeriesNodeSelector->setCurrentNode(addedNode);
}
