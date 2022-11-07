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
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Terminologies includes
#include "qCjyxTerminologySelectorDialog.h"

#include "vtkCjyxTerminologyEntry.h"

// Qt includes
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Terminologies_Widgets
class qCjyxTerminologySelectorDialogPrivate : public QDialog
{
  Q_DECLARE_PUBLIC(qCjyxTerminologySelectorDialog);
protected:
  qCjyxTerminologySelectorDialog* const q_ptr;
public:
  qCjyxTerminologySelectorDialogPrivate(qCjyxTerminologySelectorDialog& object);
  ~qCjyxTerminologySelectorDialogPrivate() override;
public:
  void init();
private:
  qCjyxTerminologyNavigatorWidget* NavigatorWidget{nullptr};
  QPushButton* SelectButton{nullptr};
  QPushButton* CancelButton{nullptr};

  /// Terminology and other metadata (name, color, auto-generated flags) into which the selection is set
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle TerminologyInfo;
};

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorDialogPrivate::qCjyxTerminologySelectorDialogPrivate(qCjyxTerminologySelectorDialog& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorDialogPrivate::~qCjyxTerminologySelectorDialogPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorDialogPrivate::init()
{
  Q_Q(qCjyxTerminologySelectorDialog);

  // Set up UI
  this->setWindowTitle("Terminology");

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setSpacing(4);
  layout->setContentsMargins(0, 0, 0, 0);

  this->NavigatorWidget = new qCjyxTerminologyNavigatorWidget();
  layout->addWidget(this->NavigatorWidget);

  QHBoxLayout* buttonsLayout = new QHBoxLayout();
  buttonsLayout->setSpacing(4);
  buttonsLayout->setContentsMargins(4, 4, 4, 4);

  this->SelectButton = new QPushButton("Select");
  this->SelectButton->setDefault(true);
  this->SelectButton->setEnabled(false); // Disabled until terminology selection becomes valid
  buttonsLayout->addWidget(this->SelectButton, 2);

  this->CancelButton = new QPushButton("Cancel");
  buttonsLayout->addWidget(this->CancelButton, 1);

  layout->addLayout(buttonsLayout);

  // Make connections
  connect(this->NavigatorWidget, SIGNAL(selectionValidityChanged(bool)), q, SLOT(setSelectButtonEnabled(bool)));
  connect(this->NavigatorWidget, SIGNAL(typeDoubleClicked()), this, SLOT(accept()));
  connect(this->SelectButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(this->CancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

//-----------------------------------------------------------------------------
// qCjyxTerminologySelectorDialog methods

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorDialog::qCjyxTerminologySelectorDialog(QObject* parent)
  : QObject(parent)
  , d_ptr(new qCjyxTerminologySelectorDialogPrivate(*this))
{
  Q_D(qCjyxTerminologySelectorDialog);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorDialog::qCjyxTerminologySelectorDialog(
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &initialTerminologyInfo, QObject* parent)
  : QObject(parent)
  , d_ptr(new qCjyxTerminologySelectorDialogPrivate(*this))
{
  Q_D(qCjyxTerminologySelectorDialog);
  d->TerminologyInfo = initialTerminologyInfo;

  d->init();
}

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorDialog::~qCjyxTerminologySelectorDialog() = default;

//-----------------------------------------------------------------------------
bool qCjyxTerminologySelectorDialog::exec()
{
  Q_D(qCjyxTerminologySelectorDialog);

  // Initialize dialog
  d->NavigatorWidget->setTerminologyInfo(d->TerminologyInfo);

  // Show dialog
  bool result = false;
  if (d->exec() != QDialog::Accepted)
    {
    return result;
    }

  // Save selection after clean exit
  d->NavigatorWidget->terminologyInfo(d->TerminologyInfo);
  return true;
}

//-----------------------------------------------------------------------------
bool qCjyxTerminologySelectorDialog::getTerminology(
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo, QObject* parent)
{
  // Open terminology dialog and store result
  qCjyxTerminologySelectorDialog dialog(terminologyInfo, parent);
  bool result = dialog.exec();
  dialog.terminologyInfo(terminologyInfo);
  return result;
}

//-----------------------------------------------------------------------------
bool qCjyxTerminologySelectorDialog::getTerminology(vtkCjyxTerminologyEntry* terminologyEntry, QObject* parent)
{
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle terminologyInfo;
  terminologyInfo.GetTerminologyEntry()->Copy(terminologyEntry);
  // Open terminology dialog and store result
  qCjyxTerminologySelectorDialog dialog(terminologyInfo, parent);
  dialog.setOverrideSectionVisible(false);
  if (!dialog.exec())
    {
    return false;
    }
  dialog.terminologyInfo(terminologyInfo);
  terminologyEntry->Copy(terminologyInfo.GetTerminologyEntry());
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorDialog::terminologyInfo(
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo )
{
  Q_D(qCjyxTerminologySelectorDialog);
  terminologyInfo = d->TerminologyInfo;
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorDialog::setSelectButtonEnabled(bool enabled)
{
  Q_D(qCjyxTerminologySelectorDialog);
  d->SelectButton->setEnabled(enabled);
}

//-----------------------------------------------------------------------------
bool qCjyxTerminologySelectorDialog::overrideSectionVisible() const
{
  Q_D(const qCjyxTerminologySelectorDialog);
  return d->NavigatorWidget->overrideSectionVisible();
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorDialog::setOverrideSectionVisible(bool visible)
{
  Q_D(qCjyxTerminologySelectorDialog);
  d->NavigatorWidget->setOverrideSectionVisible(visible);
}
