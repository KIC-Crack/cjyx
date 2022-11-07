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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxVolumeRenderingSettingsPanel_h
#define __qCjyxVolumeRenderingSettingsPanel_h

// CTK includes
#include <ctkVTKObject.h>
#include <ctkSettingsPanel.h>

// VolumeRendering includes
#include "qCjyxVolumeRenderingModuleExport.h"
class qCjyxVolumeRenderingSettingsPanelPrivate;
class vtkCjyxVolumeRenderingLogic;

class Q_CJYX_QTMODULES_VOLUMERENDERING_EXPORT qCjyxVolumeRenderingSettingsPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  QVTK_OBJECT
  Q_PROPERTY(QString defaultRenderingMethod READ defaultRenderingMethod WRITE setDefaultRenderingMethod NOTIFY defaultRenderingMethodChanged)
  Q_PROPERTY(QString defaultQuality READ defaultQuality WRITE setDefaultQuality NOTIFY defaultQualityChanged)
  Q_PROPERTY(int defaultInteractiveSpeed READ defaultInteractiveSpeed WRITE setDefaultInteractiveSpeed NOTIFY defaultInteractiveSpeedChanged)
  Q_PROPERTY(bool defaultSurfaceSmoothing READ defaultSurfaceSmoothing WRITE setDefaultSurfaceSmoothing NOTIFY defaultSurfaceSmoothingChanged)
  Q_PROPERTY(bool defaultAutoReleaseGraphicsResources READ defaultAutoReleaseGraphicsResources \
    WRITE setDefaultAutoReleaseGraphicsResources NOTIFY defaultAutoReleaseGraphicsResourcesChanged)
  Q_PROPERTY(QString gpuMemory READ gpuMemory WRITE setGPUMemory NOTIFY gpuMemoryChanged)

public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxVolumeRenderingSettingsPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxVolumeRenderingSettingsPanel() override;

  /// Volume rendering logic is synchronized with the settings.
  /// \sa vtkCjyxVolumeRenderingLogic::SetDefaultRenderingMethod
  void setVolumeRenderingLogic(vtkCjyxVolumeRenderingLogic* logic);
  vtkCjyxVolumeRenderingLogic* volumeRenderingLogic()const;

  QString defaultRenderingMethod()const;
  QString defaultQuality()const;
  int defaultInteractiveSpeed()const;
  bool defaultSurfaceSmoothing()const;
  bool defaultAutoReleaseGraphicsResources()const;
  QString gpuMemory()const;

public slots:
  void setDefaultRenderingMethod(const QString& method);
  void setDefaultQuality(const QString& quality);
  void setDefaultInteractiveSpeed(int interactiveSpeed);
  void setDefaultSurfaceSmoothing(bool surfaceSmoothing);
  void setDefaultAutoReleaseGraphicsResources(bool autoRelease);
  void setGPUMemory(const QString& gpuMemory);

signals:
  void defaultRenderingMethodChanged(const QString&);
  void defaultQualityChanged(const QString&);
  void defaultInteractiveSpeedChanged(int);
  void defaultSurfaceSmoothingChanged(bool);
  void defaultAutoReleaseGraphicsResourcesChanged(bool);
  void gpuMemoryChanged(QString);

protected slots:
  void onVolumeRenderingLogicModified();
  void onDefaultRenderingMethodChanged(int);
  void updateVolumeRenderingLogicDefaultRenderingMethod();
  void onDefaultQualityChanged(int);
  void onDefaultInteractiveSpeedChanged(double);
  void onDefaultSurfaceSmoothingChanged(bool);
  void onDefaultAutoReleaseGraphicsResourcesChanged(bool);
  void onGPUMemoryChanged();
  void updateDefaultViewNodeFromWidget();

protected:
  QScopedPointer<qCjyxVolumeRenderingSettingsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxVolumeRenderingSettingsPanel);
  Q_DISABLE_COPY(qCjyxVolumeRenderingSettingsPanel);
};

#endif
