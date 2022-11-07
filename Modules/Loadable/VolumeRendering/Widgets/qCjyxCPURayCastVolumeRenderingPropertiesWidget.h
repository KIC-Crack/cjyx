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

#ifndef __qCjyxCPURayCastVolumeRenderingPropertiesWidget_h
#define __qCjyxCPURayCastVolumeRenderingPropertiesWidget_h

// Cjyx includes
#include "qCjyxVolumeRenderingPropertiesWidget.h"
class qCjyxCPURayCastVolumeRenderingPropertiesWidgetPrivate;
class vtkMRMLCPURayCastVolumeRenderingDisplayNode;

/// \ingroup Cjyx_QtModules_VolumeRendering
class Q_CJYX_MODULE_VOLUMERENDERING_WIDGETS_EXPORT qCjyxCPURayCastVolumeRenderingPropertiesWidget
  : public qCjyxVolumeRenderingPropertiesWidget
{
  Q_OBJECT
public:
  typedef qCjyxVolumeRenderingPropertiesWidget Superclass;
  qCjyxCPURayCastVolumeRenderingPropertiesWidget(QWidget *parent=nullptr);
  ~qCjyxCPURayCastVolumeRenderingPropertiesWidget() override;

  vtkMRMLCPURayCastVolumeRenderingDisplayNode* mrmlCPURayCastDisplayNode();

public slots:
  void setRenderingTechnique(int index);

protected slots:
  void updateWidgetFromMRML() override;

protected:
  QScopedPointer<qCjyxCPURayCastVolumeRenderingPropertiesWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCPURayCastVolumeRenderingPropertiesWidget);
  Q_DISABLE_COPY(qCjyxCPURayCastVolumeRenderingPropertiesWidget);
};

#endif
