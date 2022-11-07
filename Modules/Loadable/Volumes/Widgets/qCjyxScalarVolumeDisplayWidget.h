#ifndef __qCjyxScalarVolumeDisplayWidget_h
#define __qCjyxScalarVolumeDisplayWidget_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkVTKObject.h>

// Cjyx includes
#include <qCjyxWidget.h>

#include "qCjyxVolumesModuleWidgetsExport.h"

class vtkMRMLNode;
class vtkMRMLScalarVolumeDisplayNode;
class vtkMRMLScalarVolumeNode;
class qCjyxScalarVolumeDisplayWidgetPrivate;

/// \ingroup Cjyx_QtModules_Volumes
class Q_CJYX_QTMODULES_VOLUMES_WIDGETS_EXPORT qCjyxScalarVolumeDisplayWidget
  : public qCjyxWidget
{
  Q_OBJECT
  QVTK_OBJECT
  Q_PROPERTY(bool enableColorTableComboBox READ isColorTableComboBoxEnabled WRITE setColorTableComboBoxEnabled )
  Q_PROPERTY(bool enableMRMLWindowLevelWidget READ isMRMLWindowLevelWidgetEnabled WRITE setMRMLWindowLevelWidgetEnabled )
public:
  /// Constructors
  typedef qCjyxWidget Superclass;
  explicit qCjyxScalarVolumeDisplayWidget(QWidget* parent);
  ~qCjyxScalarVolumeDisplayWidget() override;

  Q_INVOKABLE vtkMRMLScalarVolumeNode* volumeNode()const;
  Q_INVOKABLE vtkMRMLScalarVolumeDisplayNode* volumeDisplayNode()const;

  bool isColorTableComboBoxEnabled()const;
  void setColorTableComboBoxEnabled(bool);

  bool isMRMLWindowLevelWidgetEnabled()const;
  void setMRMLWindowLevelWidgetEnabled(bool);

public slots:

  ///
  /// Set the MRML node of interest
  void setMRMLVolumeNode(vtkMRMLScalarVolumeNode* volumeNode);
  void setMRMLVolumeNode(vtkMRMLNode* node);

  void setInterpolate(bool interpolate);
  void setColorNode(vtkMRMLNode* colorNode);
  void setPreset(const QString& presetName);

protected slots:
  void updateWidgetFromMRML();
  void updateHistogram();
  void onPresetButtonClicked();
  void onLockWindowLevelButtonClicked();
  void onHistogramSectionExpanded(bool);

protected:
  void showEvent(QShowEvent * event) override;
protected:
  QScopedPointer<qCjyxScalarVolumeDisplayWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxScalarVolumeDisplayWidget);
  Q_DISABLE_COPY(qCjyxScalarVolumeDisplayWidget);
};

#endif
