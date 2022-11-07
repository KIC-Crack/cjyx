#ifndef __qCjyxAnnotationModuleReportDialog_h
#define __qCjyxAnnotationModuleReportDialog_h

#include "ui_qCjyxAnnotationModuleReportDialog.h"
#include "Logic/vtkCjyxAnnotationModuleLogic.h"
#include "vtkCollection.h"

/// \ingroup Cjyx_QtModules_Annotation
class qCjyxAnnotationModuleReportDialog : public QDialog
{
  Q_OBJECT
public:
  qCjyxAnnotationModuleReportDialog();
  ~qCjyxAnnotationModuleReportDialog() override;

  Ui::qCjyxAnnotationModuleReportDialog getReportDialogUi();

  /// Set the Annotation module logic.
  void setLogic(vtkCjyxAnnotationModuleLogic* logic);

  /// Set a collection of Annotations to be included in the report
  void setAnnotations(vtkCollection* collection);

  /// Update the report
  void updateReport();

protected:


protected slots:
  void onDialogRejected();
  void onDialogAccepted();
  void onTextEdited();
  void onPrintButtonClicked();

signals:
  void dialogRejected();
  void dialogAccepted();

private:

    Ui::qCjyxAnnotationModuleReportDialog ui;
    void createConnection();

    QString generateReport();
    void generateReportRecursive(int level, vtkMRMLAnnotationHierarchyNode* currentHierarchy);

    // check if a mrmlId is part of the selected annotations
    bool isSelected(const char* mrmlId);

    bool saveReport();

    vtkCjyxAnnotationModuleLogic* m_Logic;

    // the selected annotations
    vtkCollection* m_Annotations;
    // check if a mrmlId is part of the selected annotations
    bool isAnnotationSelected(const char* mrmlId);

    QString m_Html;

};

#endif
