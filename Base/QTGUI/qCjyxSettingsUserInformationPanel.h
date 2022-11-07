/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

=========================================================================auto=*/

#ifndef __qqCjyxSettingsUserInformationPanel_h
#define __qqCjyxSettingsUserInformationPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>
#include <ctkVTKObject.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class qCjyxSettingsUserInformationPanelPrivate;
class vtkPersonInformation;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsUserInformationPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  QVTK_OBJECT
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsUserInformationPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsUserInformationPanel() override;

  Q_INVOKABLE virtual void setUserInformation(vtkPersonInformation* userInfo);

public Q_SLOTS:
  void resetSettings() override;
  void applySettings() override;

  void updateFromUserInformation();
  void onNameChanged();
  void onLoginChanged();
  void onEmailChanged(const QString& value);
  void onOrganizationChanged();
  void onOrganizationRoleChanged();
  void onProcedureRoleChanged();

protected:

  void resetWarnings();

  std::string UserInformationBackup;
  QScopedPointer<qCjyxSettingsUserInformationPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsUserInformationPanel);
  Q_DISABLE_COPY(qCjyxSettingsUserInformationPanel);
};

#endif
