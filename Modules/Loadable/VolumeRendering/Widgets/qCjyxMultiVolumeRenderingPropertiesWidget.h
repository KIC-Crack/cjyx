/*==============================================================================

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
  and CANARIE.

==============================================================================*/

#ifndef __qCjyxMultiVolumeRenderingPropertiesWidget_h
#define __qCjyxMultiVolumeRenderingPropertiesWidget_h

// Cjyx includes
#include "qCjyxVolumeRenderingPropertiesWidget.h"
class qCjyxMultiVolumeRenderingPropertiesWidgetPrivate;
class vtkMRMLMultiVolumeRenderingDisplayNode;

/// \ingroup Cjyx_QtModules_VolumeRendering
class Q_CJYX_MODULE_VOLUMERENDERING_WIDGETS_EXPORT qCjyxMultiVolumeRenderingPropertiesWidget
  : public qCjyxVolumeRenderingPropertiesWidget
{
  Q_OBJECT
public:
  typedef qCjyxVolumeRenderingPropertiesWidget Superclass;
  qCjyxMultiVolumeRenderingPropertiesWidget(QWidget *parent=nullptr);
  ~qCjyxMultiVolumeRenderingPropertiesWidget() override;

  vtkMRMLMultiVolumeRenderingDisplayNode* mrmlDisplayNode();

public slots:
  void setRenderingTechnique(int index);
  void setSurfaceSmoothing(bool on);

protected slots:
  void updateWidgetFromMRML() override;

protected:
  QScopedPointer<qCjyxMultiVolumeRenderingPropertiesWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxMultiVolumeRenderingPropertiesWidget);
  Q_DISABLE_COPY(qCjyxMultiVolumeRenderingPropertiesWidget);
};

#endif
