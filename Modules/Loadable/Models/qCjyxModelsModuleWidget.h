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

#ifndef __qCjyxModelsModuleWidget_h
#define __qCjyxModelsModuleWidget_h

// CTK includes
#include "ctkVTKObject.h"

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxModelsModuleExport.h"

class qCjyxModelsModuleWidgetPrivate;
class vtkMRMLNode;
class vtkMRMLSelectionNode;

/// \ingroup Cjyx_QtModules_Models
class Q_CJYX_QTMODULES_MODELS_EXPORT qCjyxModelsModuleWidget : public qCjyxAbstractModuleWidget
{
  Q_OBJECT
  QVTK_OBJECT

public:
  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxModelsModuleWidget(QWidget *parent=nullptr);
  ~qCjyxModelsModuleWidget() override;

  void enter() override;
  void exit() override;
  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:
  void setMRMLScene(vtkMRMLScene* scene) override;

  /// Set current subject hierarchy item IDs.
  /// The current node (the properties of which the widget displays) will be the one associated
  /// to the first selected subject hierarchy item.
  /// When a property changes, then it is applied to all the models in the selection
  void setDisplaySelectionFromSubjectHierarchyItems(QList<vtkIdType> itemIDs);

  void onClippingConfigurationButtonClicked();
  void onDisplayNodeChanged();
  void onClipSelectedModelToggled(bool);
  /// Create or get first color legend if group box is expanded
  void onColorLegendCollapsibleGroupBoxToggled(bool);

  static void onMRMLSceneEvent(vtkObject* vtk_obj, unsigned long event,
                               void* client_data, void* call_data);

  /// hide/show all the models in the scene
  void hideAllModels();
  void showAllModels();

protected slots:
  /// Called when a subject hierarchy item is modified.
  /// Updates current item selection to reflect changes in item (such as display node creation)
  void onSubjectHierarchyItemModified(vtkObject* caller, void* callData);

  /// Called when the information collapsible button collapsed state is changed.
  void onInformationSectionCollapsed(bool);

protected:
  void setup() override;

protected:
  QScopedPointer<qCjyxModelsModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModelsModuleWidget);
  Q_DISABLE_COPY(qCjyxModelsModuleWidget);
};

#endif
