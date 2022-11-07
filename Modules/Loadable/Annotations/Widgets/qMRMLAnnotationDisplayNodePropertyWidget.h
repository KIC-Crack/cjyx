
#ifndef __qMRMLAnnotationDisplayNodePropertyWidget_h
#define __qMRMLAnnotationDisplayNodePropertyWidget_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>

// Cjyx includes
#include "qMRMLWidget.h"

#include "qCjyxAnnotationsModuleWidgetsExport.h"

class qMRMLAnnotationDisplayNodePropertyWidgetPrivate;

/// \ingroup Cjyx_QtModules_Annotations
class Q_CJYX_MODULE_ANNOTATIONS_WIDGETS_EXPORT qMRMLAnnotationDisplayNodePropertyWidget : public qMRMLWidget
{
  Q_OBJECT
  QVTK_OBJECT

public:
  typedef qMRMLWidget Superclass;
  qMRMLAnnotationDisplayNodePropertyWidget(QWidget *newParent = 0);
  virtual ~qMRMLAnnotationDisplayNodePropertyWidget();

protected:
  QScopedPointer<qMRMLAnnotationDisplayNodePropertyWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qMRMLAnnotationDisplayNodePropertyWidget);
  Q_DISABLE_COPY(qMRMLAnnotationDisplayNodePropertyWidget);

};

#endif
