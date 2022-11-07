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

// Tables Widgets includes
#include "qCjyxTableColumnPropertiesWidget.h"

// Markups includes
//#include <vtkCjyxTablesLogic.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxModuleManager.h"
#include "qCjyxAbstractCoreModule.h"
#include "qMRMLTableView.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkMRMLInteractionNode.h>
#include <vtkMRMLTableNode.h>

// Qt includes
#include <QDebug>
#include <QPointer>

static const char SCHEMA_PROPERTY_NAME[] = "SchemaPropertyName";

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CreateModels
class qCjyxTableColumnPropertiesWidgetPrivate
  : public Ui_qCjyxTableColumnPropertiesWidget
{
  Q_DECLARE_PUBLIC(qCjyxTableColumnPropertiesWidget);
protected:
  qCjyxTableColumnPropertiesWidget* const q_ptr;

public:
  qCjyxTableColumnPropertiesWidgetPrivate( qCjyxTableColumnPropertiesWidget& object);
  ~qCjyxTableColumnPropertiesWidgetPrivate();
  virtual void setupUi(qCjyxTableColumnPropertiesWidget*);

public:
  QStringList ColumnNames;

  bool ColumnNameVisible;
  bool ConfirmTypeChange;

  QList<QLineEdit*> PropertyEditWidgets;
  vtkWeakPointer<vtkMRMLTableNode> CurrentTableNode;
  QPointer<qMRMLTableView> TableViewForSelection;
};

// --------------------------------------------------------------------------
qCjyxTableColumnPropertiesWidgetPrivate::qCjyxTableColumnPropertiesWidgetPrivate( qCjyxTableColumnPropertiesWidget& object)
  : q_ptr(&object)
  , ColumnNameVisible(true)
  , ConfirmTypeChange(true)
{
}

//-----------------------------------------------------------------------------
qCjyxTableColumnPropertiesWidgetPrivate::~qCjyxTableColumnPropertiesWidgetPrivate() = default;

// --------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidgetPrivate::setupUi(qCjyxTableColumnPropertiesWidget* widget)
{
  this->Ui_qCjyxTableColumnPropertiesWidget::setupUi(widget);
}


//-----------------------------------------------------------------------------
// qCjyxTableColumnPropertiesWidget methods

//-----------------------------------------------------------------------------
qCjyxTableColumnPropertiesWidget::qCjyxTableColumnPropertiesWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr(new qCjyxTableColumnPropertiesWidgetPrivate(*this))
{
  this->setup();
}

//-----------------------------------------------------------------------------
qCjyxTableColumnPropertiesWidget::~qCjyxTableColumnPropertiesWidget()
{
  this->setMRMLTableNode((vtkMRMLTableNode*)nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setup()
{
  Q_D(qCjyxTableColumnPropertiesWidget);

  d->setupUi(this);

  d->ApplyTypeChangeButton->setVisible(false);
  d->CancelTypeChangeButton->setVisible(false);

  d->NullValueLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("nullValue"));
  d->LongNameLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("longName"));
  d->DescriptionLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("description"));
  d->UnitLabelLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("unitLabel"));
  d->ComponentCountLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("componentCount"));
  d->ComponentNamesLineEdit->setProperty(SCHEMA_PROPERTY_NAME, QString("componentNames"));

  d->PropertyEditWidgets << d->NullValueLineEdit;
  d->PropertyEditWidgets << d->LongNameLineEdit;
  d->PropertyEditWidgets << d->DescriptionLineEdit;
  d->PropertyEditWidgets << d->UnitLabelLineEdit;
  d->PropertyEditWidgets << d->ComponentCountLineEdit;
  d->PropertyEditWidgets << d->ComponentNamesLineEdit;

  connect(d->DataTypeComboBox, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(onDataTypeChanged(const QString&)));
  foreach(QLineEdit* widget, d->PropertyEditWidgets)
    {
    connect(widget, SIGNAL(textEdited(const QString&)), this, SLOT(onPropertyChanged(const QString&)));
    }
  connect(d->ApplyTypeChangeButton, SIGNAL(clicked()), this, SLOT(onApplyTypeChange()));
  connect(d->CancelTypeChangeButton, SIGNAL(clicked()), this, SLOT(onCancelTypeChange()));

}


//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setMRMLTableNode(vtkMRMLNode* node)
{
  this->setMRMLTableNode(vtkMRMLTableNode::SafeDownCast(node));
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setMRMLTableNode(vtkMRMLTableNode* node)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (node == d->CurrentTableNode)
    {
    // not changed
    return;
    }

  // Reconnect the appropriate nodes
  this->qvtkReconnect(d->CurrentTableNode, node, vtkCommand::ModifiedEvent, this, SLOT(updateWidget()));
  d->CurrentTableNode = node;

  this->updateWidget();
}

//------------------------------------------------------------------------------
vtkMRMLTableNode* qCjyxTableColumnPropertiesWidget::mrmlTableNode()const
{
  Q_D(const qCjyxTableColumnPropertiesWidget);
  return d->CurrentTableNode;
}

//-----------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setColumnProperty(QString propertyName, QString propertyValue)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (d->CurrentTableNode == nullptr)
    {
    qWarning() << Q_FUNC_INFO << " failed: table node is not selected";
    return;
    }
  if (d->ColumnNames.empty())
    {
    qWarning() << Q_FUNC_INFO << " failed: table column names are not specified";
    return;
    }
  foreach(const QString& columnName, d->ColumnNames)
    {
    d->CurrentTableNode->SetColumnProperty(columnName.toUtf8().constData(), propertyName.toUtf8().constData(), propertyValue.toUtf8().constData());
    }
}

//-----------------------------------------------------------------------------
QString qCjyxTableColumnPropertiesWidget::columnProperty(QString propertyName) const
{
  Q_D(const qCjyxTableColumnPropertiesWidget);
  if (d->CurrentTableNode == nullptr)
    {
    qWarning() << Q_FUNC_INFO << " failed: table node is not selected";
    return "";
    }
  if (d->ColumnNames.empty())
    {
    return "";
    }
  std::string commonPropertyValue = d->CurrentTableNode->GetColumnProperty(d->ColumnNames[0].toUtf8().constData(), propertyName.toUtf8().constData());
  foreach(const QString& columnName, d->ColumnNames)
    {
    std::string currentPropertyValue = d->CurrentTableNode->GetColumnProperty(columnName.toUtf8().constData(), propertyName.toUtf8().constData());
    if (currentPropertyValue != commonPropertyValue)
      {
      // not all column types are the same
      return QString();
      }
    }
  return commonPropertyValue.c_str();
}

//-----------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::updateWidget()
{
  Q_D(qCjyxTableColumnPropertiesWidget);

  d->NameLabel->setVisible(d->ColumnNameVisible);
  d->NameLineEdit->setVisible(d->ColumnNameVisible);

  if (d->CurrentTableNode == nullptr)
    {
    this->setEnabled(false);
    return;
    }

  bool componentRowsVisible = vtkVariant(this->columnProperty("componentCount").toStdString()) > 1;
  d->ComponentCountLabel->setVisible(componentRowsVisible);
  d->ComponentCountLineEdit->setVisible(componentRowsVisible);
  d->ComponentNamesLabel->setVisible(componentRowsVisible);
  d->ComponentNamesLineEdit->setVisible(componentRowsVisible);

  d->NameLineEdit->setText(d->ColumnNames.join(", "));

  QString columnType = this->columnProperty("type");
  int columnTypeIndex = d->DataTypeComboBox->findText(columnType);
  d->DataTypeComboBox->setCurrentIndex(columnTypeIndex);

  foreach(QLineEdit* widget, d->PropertyEditWidgets)
    {
    widget->setText(this->columnProperty(widget->property(SCHEMA_PROPERTY_NAME).toString()));
    }

  this->setEnabled(!d->ColumnNames.empty());
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setMRMLScene(vtkMRMLScene* scene)
{
  this->Superclass::setMRMLScene(scene);
  this->updateWidget();
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setSelectionFromMRMLTableView(qMRMLTableView* tableView)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (tableView == d->TableViewForSelection)
    {
    // no change
    return;
    }
  vtkMRMLScene* newScene = nullptr;
  vtkMRMLTableNode* newTableNode = nullptr;

  if (tableView != nullptr)
    {
    newScene = tableView->mrmlScene();
    newTableNode = tableView->mrmlTableNode();
    }

  this->Superclass::setMRMLScene(newScene);
  this->setMRMLTableNode(newTableNode);

  if (d->TableViewForSelection)
    {
    disconnect(d->TableViewForSelection, SIGNAL(selectionChanged()), this, SLOT(tableViewSelectionChanged()));
    }
  d->TableViewForSelection = tableView;
  if (d->TableViewForSelection)
    {
    connect(d->TableViewForSelection, SIGNAL(selectionChanged()), this, SLOT(tableViewSelectionChanged()));
    }

  this->updateWidget();
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::tableViewSelectionChanged()
{
  Q_D(const qCjyxTableColumnPropertiesWidget);

  if (d->TableViewForSelection == nullptr)
    {
    this->setMRMLTableNode((vtkMRMLTableNode*)nullptr);
    return;
    }

  this->setMRMLTableNode(d->TableViewForSelection->mrmlTableNode());

  QStringList selectedColumnNames;
  if (d->TableViewForSelection && d->TableViewForSelection->mrmlTableNode())
    {
    vtkMRMLTableNode* tableNode = d->TableViewForSelection->mrmlTableNode();
    QList<int> selectedColumns = d->TableViewForSelection->selectedMRMLTableColumnIndices();
    foreach(int columnIndex, selectedColumns)
      {
      selectedColumnNames << tableNode->GetColumnName(columnIndex).c_str();
      }
    }
  this->setMRMLTableColumnNames(selectedColumnNames);
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setMRMLTableColumnNames(const QStringList& selectedColumns)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  d->ColumnNames = selectedColumns;
  this->updateWidget();
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setMRMLTableColumnName(const QString& selectedColumn)
{
  QStringList selectedColumns;
  selectedColumns << selectedColumn;
  this->setMRMLTableColumnNames(selectedColumns);
}

//------------------------------------------------------------------------------
QStringList qCjyxTableColumnPropertiesWidget::mrmlTableColumnNames()
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  return d->ColumnNames;
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::onDataTypeChanged(const QString& newDataType)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (newDataType == this->columnProperty("type"))
    {
    // no change
    return;
    }
  if (d->ConfirmTypeChange)
    {
    d->ApplyTypeChangeButton->setVisible(true);
    d->CancelTypeChangeButton->setVisible(true);
    }
  else
    {
    this->setColumnProperty("type", d->DataTypeComboBox->currentText());
    }
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::onPropertyChanged(const QString& newPropertyValue)
{
  QLineEdit* propertyWidget = qobject_cast<QLineEdit*>(sender());
  if (propertyWidget != nullptr)
    {
    QString propertyName = propertyWidget->property(SCHEMA_PROPERTY_NAME).toString();
    this->setColumnProperty(propertyName, newPropertyValue);
    }
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::onApplyTypeChange()
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  this->setColumnProperty("type", d->DataTypeComboBox->currentText());
  d->ApplyTypeChangeButton->setVisible(false);
  d->CancelTypeChangeButton->setVisible(false);
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::onCancelTypeChange()
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  this->updateWidget();
  d->ApplyTypeChangeButton->setVisible(false);
  d->CancelTypeChangeButton->setVisible(false);
}

//------------------------------------------------------------------------------
bool qCjyxTableColumnPropertiesWidget::columnNameVisible()const
{
  Q_D(const qCjyxTableColumnPropertiesWidget);
  return d->ColumnNameVisible;
}

//------------------------------------------------------------------------------
bool qCjyxTableColumnPropertiesWidget::confirmTypeChange()const
{
  Q_D(const qCjyxTableColumnPropertiesWidget);
  return d->ConfirmTypeChange;
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setColumnNameVisible(bool visible)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (d->ColumnNameVisible == visible)
    {
    // no change
    return;
    }
  d->ColumnNameVisible = visible;
  this->updateWidget();
}

//------------------------------------------------------------------------------
void qCjyxTableColumnPropertiesWidget::setConfirmTypeChange(bool confirm)
{
  Q_D(qCjyxTableColumnPropertiesWidget);
  if (d->ConfirmTypeChange == confirm)
    {
    // no change
    return;
    }
  d->ConfirmTypeChange = confirm;
  if (!d->ConfirmTypeChange)
    {
    this->onCancelTypeChange();
    }
}
