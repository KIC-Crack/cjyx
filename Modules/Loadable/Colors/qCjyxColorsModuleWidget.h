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

#ifndef __qCjyxColorsModuleWidget_h
#define __qCjyxColorsModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

// Colors includes
#include "qCjyxColorsModuleExport.h"

class qCjyxColorsModuleWidgetPrivate;
class vtkMRMLNode;
class vtkScalarBarWidget;
class QAbstractButton;

class Q_CJYX_QTMODULES_COLORS_EXPORT qCjyxColorsModuleWidget
  : public qCjyxAbstractModuleWidget
{
  Q_OBJECT
public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxColorsModuleWidget(QWidget *parent=nullptr);
  ~qCjyxColorsModuleWidget() override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:
  void setCurrentColorNode(vtkMRMLNode*);
  void updateNumberOfColors();
  void setLookupTableRange(double min, double max);
  void copyCurrentColorNode();
  void onDisplayableNodeChanged(vtkMRMLNode*);
  void createColorLegend();
  void deleteColorLegend();
  void useCurrentColorsForColorLegend();

protected slots:
  void onMRMLColorNodeChanged(vtkMRMLNode* newColorNode);
  void updateColorLegendFromMRML();

protected:
  void setup() override;
  void setMRMLScene(vtkMRMLScene *scene) override;

protected:
  QScopedPointer<qCjyxColorsModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxColorsModuleWidget);
  Q_DISABLE_COPY(qCjyxColorsModuleWidget);
};

#endif
