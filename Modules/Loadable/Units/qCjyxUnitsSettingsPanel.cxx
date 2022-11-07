/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Johan Andruejol, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// CTK Includes
#include "ctkCollapsibleGroupBox.h"
#include "ctkSettingsPanel.h"

// Qt includes
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QHash>
#include <QSettings>

// QtGUI includes
#include "qMRMLNodeComboBox.h"
#include "qMRMLSettingsUnitWidget.h"
#include "qMRMLUnitWidget.h"
#include "qCjyxApplication.h"
#include "qCjyxUnitsSettingsPanel.h"
#include "ui_qCjyxUnitsSettingsPanel.h"

// Units includes
#include "vtkMRMLScene.h"
#include "vtkMRMLSelectionNode.h"
#include "vtkMRMLUnitNode.h"
#include "vtkCjyxUnitsLogic.h"

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>

// --------------------------------------------------------------------------
// qCjyxUnitsSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxUnitsSettingsPanelPrivate: public Ui_qCjyxUnitsSettingsPanel
{
  Q_DECLARE_PUBLIC(qCjyxUnitsSettingsPanel);
protected:
  qCjyxUnitsSettingsPanel* const q_ptr;

public:
  qCjyxUnitsSettingsPanelPrivate(qCjyxUnitsSettingsPanel& object);
  void init();
  void registerProperties(
    QString quantity, qMRMLSettingsUnitWidget* unitWidget);
  void addQuantity(const QString& quantity);
  void clearQuantities();
  void setMRMLScene(vtkMRMLScene* scene);
  void setSelectionNode(vtkMRMLSelectionNode* selectionNode);
  void resize(bool showall);

  vtkSmartPointer<vtkCjyxUnitsLogic> Logic;
  vtkMRMLScene* MRMLScene;
  QHash<QString, qMRMLSettingsUnitWidget*> Quantities;
  vtkMRMLSelectionNode* SelectionNode;
};

// --------------------------------------------------------------------------
// qCjyxUnitsSettingsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxUnitsSettingsPanelPrivate
::qCjyxUnitsSettingsPanelPrivate(qCjyxUnitsSettingsPanel& object)
  :q_ptr(&object)
{
  this->Logic = nullptr;
  this->MRMLScene = nullptr;
  this->SelectionNode = nullptr;
}

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate::init()
{
  Q_Q(qCjyxUnitsSettingsPanel);

  this->setupUi(q);

  q->connect(this->ShowAllCheckBox, SIGNAL(toggled(bool)),
    q, SLOT(showAll(bool)));
  this->ShowAllCheckBox->setChecked(false);
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate
::registerProperties(QString quantity, qMRMLSettingsUnitWidget* unitWidget)
{
  Q_Q(qCjyxUnitsSettingsPanel);

  qCjyxCoreApplication* app = qCjyxCoreApplication::application();

  q->registerProperty(quantity + "/id", unitWidget->unitComboBox(),
    "currentNodeID", SIGNAL(currentNodeIDChanged(QString)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/prefix", unitWidget->unitWidget(),
    "prefix", SIGNAL(prefixChanged(QString)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/suffix", unitWidget->unitWidget(),
    "suffix", SIGNAL(suffixChanged(QString)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/precision", unitWidget->unitWidget(),
    "precision", SIGNAL(precisionChanged(int)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/minimum", unitWidget->unitWidget(),
    "minimum", SIGNAL(minimumChanged(double)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/maximum", unitWidget->unitWidget(),
    "maximum", SIGNAL(maximumChanged(double)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/coefficient", unitWidget->unitWidget(),
    "coefficient", SIGNAL(coefficientChanged(double)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
  q->registerProperty(quantity + "/offset", unitWidget->unitWidget(),
    "offset", SIGNAL(offsetChanged(double)),
    QString(), ctkSettingsPanel::OptionNone, app->userSettings());
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate::addQuantity(const QString& quantity)
{
  Q_Q(qCjyxUnitsSettingsPanel);
  QString lowerQuantity = quantity.toLower();

  // Add collapsible groupbox
  ctkCollapsibleGroupBox* groupbox = new ctkCollapsibleGroupBox(q);
  QString groupboxTitle = lowerQuantity;
  groupboxTitle[0] = groupboxTitle[0].toUpper();
  groupbox->setTitle(groupboxTitle);
  QVBoxLayout* layout = new QVBoxLayout;
  layout->setSizeConstraint(QLayout::SetMaximumSize);

  groupbox->setLayout(layout);

  // Add unit widget
  qMRMLSettingsUnitWidget* unitWidget = new qMRMLSettingsUnitWidget(groupbox);
  unitWidget->setUnitsLogic(this->Logic);
  unitWidget->unitComboBox()->setNodeTypes(QStringList() << "vtkMRMLUnitNode");
  unitWidget->unitComboBox()->addAttribute(
    "vtkMRMLUnitNode", "Quantity", lowerQuantity);
  unitWidget->unitComboBox()->setMRMLScene(this->MRMLScene);
  unitWidget->unitComboBox()->setEnabled(false);
  unitWidget->unitWidget()->setDisplayedProperties(
    this->ShowAllCheckBox->isChecked() ?
      qMRMLUnitWidget::All : qMRMLUnitWidget::Precision);
  layout->addWidget(unitWidget);

  this->QuantitiesLayout->addWidget(groupbox);
  this->Quantities[lowerQuantity] = unitWidget;
  this->registerProperties(lowerQuantity, unitWidget);

  emit q->quantitiesChanged(this->Quantities.keys());
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate::clearQuantities()
{
  foreach (QObject* obj, this->GridLayout->children())
    {
    delete obj;
    }
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate::setMRMLScene(vtkMRMLScene* scene)
{
  Q_Q(qCjyxUnitsSettingsPanel);

  if (scene == this->MRMLScene)
    {
    return;
    }
  this->MRMLScene = scene;

  // Quantities are hardcoded for now
  //q->registerProperty("Units", q, "quantities",
  //  SIGNAL(quantitiesChanged(QStringList)));
  QStringList quantities; // delete this when "un-hardcoding" quantities
  quantities << "length" << "time" << "frequency" << "velocity" << "intensity";
  q->setQuantities(quantities);

  foreach (qMRMLSettingsUnitWidget* widget, this->Quantities.values())
    {
    widget->unitComboBox()->setMRMLScene(this->MRMLScene);
    }

 vtkMRMLSelectionNode* newSelectionNode = nullptr;
  if (this->MRMLScene)
    {
    newSelectionNode = vtkMRMLSelectionNode::SafeDownCast(
      scene->GetNodeByID("vtkMRMLSelectionNodeSingleton"));
    }

  this->setSelectionNode(newSelectionNode);
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate
::setSelectionNode(vtkMRMLSelectionNode* newSelectionNode)
{
  Q_Q(qCjyxUnitsSettingsPanel);
  if (newSelectionNode == this->SelectionNode)
    {
    return;
    }

  q->qvtkReconnect(this->SelectionNode, newSelectionNode,
    vtkMRMLSelectionNode::UnitModifiedEvent, q,
    SLOT(updateFromSelectionNode()));
  this->SelectionNode = newSelectionNode;
  q->updateFromSelectionNode();
}

// ---------------------------------------------------------------------------
void qCjyxUnitsSettingsPanelPrivate::resize(bool showall)
{
  if(showall)
    {
    this->scrollArea->setMinimumSize(QSize(0, 700));
    }
  else
    {
    this->scrollArea->setMinimumSize(QSize(0, 350));
    }
}

// --------------------------------------------------------------------------
// qCjyxUnitsSettingsPanel methods

// --------------------------------------------------------------------------
qCjyxUnitsSettingsPanel::qCjyxUnitsSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxUnitsSettingsPanelPrivate(*this))
{
  Q_D(qCjyxUnitsSettingsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxUnitsSettingsPanel::~qCjyxUnitsSettingsPanel() = default;

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanel::setUnitsLogic(vtkCjyxUnitsLogic* logic)
{
  Q_D(qCjyxUnitsSettingsPanel);

  qvtkReconnect(d->Logic, logic, vtkCommand::ModifiedEvent,
                this, SLOT(onUnitsLogicModified()));
  d->Logic = logic;

  foreach (qMRMLSettingsUnitWidget* widget, d->Quantities.values())
    {
    widget->setUnitsLogic(d->Logic);
    }

  this->onUnitsLogicModified();
}

// --------------------------------------------------------------------------
QStringList qCjyxUnitsSettingsPanel::quantities()
{
  Q_D(qCjyxUnitsSettingsPanel);
  return d->Quantities.keys();
}

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanel::onUnitsLogicModified()
{
  Q_D(qCjyxUnitsSettingsPanel);
  if (!d->Logic)
    {
    return;
    }

  d->setMRMLScene(d->Logic->GetMRMLScene());
  this->updateFromSelectionNode();
}

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanel::setQuantities(const QStringList& newQuantities)
{
  Q_D(qCjyxUnitsSettingsPanel);

  foreach(QString newQuantity, newQuantities)
    {
    if (!d->Quantities.contains(newQuantity))
      {
      d->addQuantity(newQuantity);
      }
    }
  // \todo Add removeQuantity(oldQuantity)
}

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanel::updateFromSelectionNode()
{
  Q_D(qCjyxUnitsSettingsPanel);
  if (! d->SelectionNode)
    {
    // clear panel ?
    return;
    }

  std::vector<vtkMRMLUnitNode*> units;
  d->SelectionNode->GetUnitNodes(units);
  for (std::vector<vtkMRMLUnitNode*>::iterator it = units.begin();
    it != units.end(); ++it)
    {
    if (*it)
      {
      QString quantity = (*it)->GetQuantity();
      if (!d->Quantities.contains(quantity))
        {
        d->addQuantity(quantity);
        }

      d->Quantities[quantity]->unitComboBox()->setCurrentNodeID(
        (*it)->GetID());
      }
    }
}

// --------------------------------------------------------------------------
void qCjyxUnitsSettingsPanel::showAll(bool showAll)
{
  Q_D(qCjyxUnitsSettingsPanel);

  d->resize(showAll);

  foreach (qMRMLSettingsUnitWidget* widget, d->Quantities.values())
    {
    qMRMLUnitWidget::UnitProperties allButNameAndQuantity =
      qMRMLUnitWidget::Preset |
      qMRMLUnitWidget::Prefix | qMRMLUnitWidget::Suffix |
      qMRMLUnitWidget::Precision |
      qMRMLUnitWidget::Minimum | qMRMLUnitWidget::Maximum |
      qMRMLUnitWidget::Coefficient | qMRMLUnitWidget::Offset;

    widget->unitWidget()->setDisplayedProperties(showAll ?
      allButNameAndQuantity : qMRMLUnitWidget::Precision);
    }

}
