#ifndef __qCjyxCropVolumeModuleWidget_h
#define __qCjyxCropVolumeModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxCropVolumeModuleExport.h"

class qCjyxCropVolumeModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLCropVolumeParametersNode;

/// \ingroup Cjyx_QtModules_CropVolume
class Q_CJYX_QTMODULES_CROPVOLUME_EXPORT qCjyxCropVolumeModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxCropVolumeModuleWidget(QWidget *parent=nullptr);
  ~qCjyxCropVolumeModuleWidget() override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:
  void setParametersNode(vtkMRMLNode* node);

protected:
  QScopedPointer<qCjyxCropVolumeModuleWidgetPrivate> d_ptr;

  void setup() override;
  void enter() override;
  void setMRMLScene(vtkMRMLScene*) override;

protected slots:
  void setInputVolume(vtkMRMLNode*);
  void setOutputVolume(vtkMRMLNode* node);
  void setInputROI(vtkMRMLNode*);
  void initializeInputROI(vtkMRMLNode*);
  /// when ROIs get added to the node selector, if the selector doesn't
  /// have a current node, select it
  void onInputROIAdded(vtkMRMLNode* node);

  void onROIVisibilityChanged(bool);
  void onROIFit();
  void onInterpolationModeChanged();
  void onApply();
  void onFixAlignment();
  void updateWidgetFromMRML();
  void onSpacingScalingValueChanged(double);
  void onIsotropicModeChanged(bool);
  void onMRMLSceneEndBatchProcessEvent();
  void onInterpolationEnabled(bool interpolationEnabled);
  void onVolumeInformationSectionClicked(bool isOpen);
  void onFillValueChanged(double);

  void updateVolumeInfo();

private:
  Q_DECLARE_PRIVATE(qCjyxCropVolumeModuleWidget);
  Q_DISABLE_COPY(qCjyxCropVolumeModuleWidget);
};

#endif
