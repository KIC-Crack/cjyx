/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

=========================================================================auto=*/

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxSettingsUserInformationPanel.h"
#include "ui_qCjyxSettingsUserInformationPanel.h"

// VTK includes
#include <vtkSmartPointer.h>

#include "vtkPersonInformation.h"

#include "qsettings.h"

// --------------------------------------------------------------------------
// qCjyxSettingsUserInformationPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsUserInformationPanelPrivate: public Ui_qCjyxSettingsUserInformationPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsUserInformationPanel);
protected:
  qCjyxSettingsUserInformationPanel* const q_ptr;

public:
  qCjyxSettingsUserInformationPanelPrivate(qCjyxSettingsUserInformationPanel& object);
  void init();

  vtkSmartPointer<vtkPersonInformation> UserInformation;
};

// --------------------------------------------------------------------------
// qCjyxSettingsUserInformationPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsUserInformationPanelPrivate
::qCjyxSettingsUserInformationPanelPrivate(qCjyxSettingsUserInformationPanel& object)
  :q_ptr(&object)
{
  this->UserInformation = nullptr;
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanelPrivate::init()
{
  Q_Q(qCjyxSettingsUserInformationPanel);

  this->setupUi(q);

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->NameLineEdit, SIGNAL(editingFinished()),
                   q, SLOT(onNameChanged()));
  QObject::connect(this->LoginLineEdit, SIGNAL(editingFinished()),
                   q, SLOT(onLoginChanged()));
  QObject::connect(this->EmailLineEdit, SIGNAL(textChanged(QString)),
                   q, SLOT(onEmailChanged(QString)));
  QObject::connect(this->OrganizationLineEdit, SIGNAL(editingFinished()),
                   q, SLOT(onOrganizationChanged()));
  QObject::connect(this->OrganizationRoleLineEdit, SIGNAL(editingFinished()),
                   q, SLOT(onOrganizationRoleChanged()));
  QObject::connect(this->ProcedureRoleLineEdit, SIGNAL(editingFinished()),
                   q, SLOT(onProcedureRoleChanged()));
}

// --------------------------------------------------------------------------
// qCjyxSettingsUserInformationPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsUserInformationPanel::qCjyxSettingsUserInformationPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsUserInformationPanelPrivate(*this))
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsUserInformationPanel::~qCjyxSettingsUserInformationPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::setUserInformation(vtkPersonInformation* userInfo)
{
  Q_D(qCjyxSettingsUserInformationPanel);
  if (d->UserInformation == userInfo)
    {
    return;
    }

  this->qvtkReconnect(d->UserInformation, userInfo,
    vtkCommand::ModifiedEvent,
    this, SLOT(updateFromUserInformation()));
  d->UserInformation = userInfo;

  // Default values
  this->updateFromUserInformation();
  this->UserInformationBackup = userInfo->GetAsString();
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::updateFromUserInformation()
{
  Q_D(qCjyxSettingsUserInformationPanel);

  if (d->UserInformation == nullptr)
    {
    return;
    }

  d->NameLineEdit->setText(d->UserInformation->GetName().c_str());
  d->LoginLineEdit->setText(d->UserInformation->GetLogin().c_str());
  d->EmailLineEdit->setText(d->UserInformation->GetEmail().c_str());
  d->OrganizationLineEdit->setText(d->UserInformation->GetOrganization().c_str());
  d->OrganizationRoleLineEdit->setText(d->UserInformation->GetOrganizationRole().c_str());
  d->ProcedureRoleLineEdit->setText(d->UserInformation->GetProcedureRole().c_str());

  this->resetWarnings();
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::applySettings()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  this->UserInformationBackup = d->UserInformation->GetAsString();
  this->updateFromUserInformation();
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::resetSettings()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetFromString(this->UserInformationBackup);
  updateFromUserInformation();
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::resetWarnings()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->EmailValidationLabel->setText("");
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onNameChanged()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetName(d->NameLineEdit->text().toStdString().c_str());
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onLoginChanged()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetLogin(d->LoginLineEdit->text().toStdString().c_str());
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onEmailChanged(const QString& value)
{
  Q_D(qCjyxSettingsUserInformationPanel);
  if(!d->UserInformation->SetEmail(value.toStdString().c_str()))
    {
    d->EmailValidationLabel->setText("Invalid format");
    }
  else
    {
    d->EmailValidationLabel->setText("");
    }
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onOrganizationChanged()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetOrganization(d->OrganizationLineEdit->text().toStdString().c_str());
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onOrganizationRoleChanged()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetOrganizationRole(d->OrganizationRoleLineEdit->text().toStdString().c_str());
}

// --------------------------------------------------------------------------
void qCjyxSettingsUserInformationPanel::onProcedureRoleChanged()
{
  Q_D(qCjyxSettingsUserInformationPanel);
  d->UserInformation->SetProcedureRole(d->ProcedureRoleLineEdit->text().toStdString().c_str());
}
