#ifndef __qCjyxVolumeDisplayWidget_h
#define __qCjyxVolumeDisplayWidget_h

// Cjyx  includes
#include <qMRMLWidget.h>

// CTK includes
#include <ctkVTKObject.h>

// Volumes includes
#include "qCjyxVolumesModuleWidgetsExport.h"

class vtkMRMLNode;
class qCjyxVolumeDisplayWidgetPrivate;

/// \ingroup Cjyx_QtModules_Volumes
class Q_CJYX_QTMODULES_VOLUMES_WIDGETS_EXPORT qCjyxVolumeDisplayWidget : public qMRMLWidget
{
  Q_OBJECT
  QVTK_OBJECT

public:
  /// Constructors
  typedef qMRMLWidget Superclass;
  explicit qCjyxVolumeDisplayWidget(QWidget* parent=nullptr);
  ~qCjyxVolumeDisplayWidget() override;

public slots:
  /// Set the MRML node of interest
  void setMRMLVolumeNode(vtkMRMLNode* node);

protected slots:
  /// Internally use in case the current display widget should change when the
  /// volume node changes
  void updateFromMRML(vtkObject* volume);
protected:
  QScopedPointer<qCjyxVolumeDisplayWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxVolumeDisplayWidget);
  Q_DISABLE_COPY(qCjyxVolumeDisplayWidget);
};

#endif
