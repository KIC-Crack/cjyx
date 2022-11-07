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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxVolumesModuleWidget_h
#define __qCjyxVolumesModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxVolumesModuleExport.h"

class qCjyxVolumesModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Cjyx_QtModules_Volumes
class Q_CJYX_QTMODULES_VOLUMES_EXPORT qCjyxVolumesModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxVolumesModuleWidget(QWidget *parent=nullptr);
  ~qCjyxVolumesModuleWidget() override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

protected:
  void setup() override;

protected slots:
  void nodeSelectionChanged(vtkMRMLNode*);
  void updateWidgetFromMRML();
  void convertVolume();
  void colorLegendCollapsibleButtonCollapsed(bool state);

protected:
  QScopedPointer<qCjyxVolumesModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxVolumesModuleWidget);
  Q_DISABLE_COPY(qCjyxVolumesModuleWidget);
};

#endif
