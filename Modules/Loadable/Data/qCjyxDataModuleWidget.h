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

#ifndef __qCjyxDataModuleWidget_h
#define __qCjyxDataModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxDataModuleExport.h"

class vtkMRMLNode;
class qCjyxDataModuleWidgetPrivate;
class qMRMLSubjectHierarchyModel;

class Q_CJYX_QTMODULES_DATA_EXPORT qCjyxDataModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT
public:
  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxDataModuleWidget(QWidget *parentWidget = nullptr);
  ~qCjyxDataModuleWidget() override;

  void enter() override;

  enum
    {
    TabIndexSubjectHierarchy = 0,
    TabIndexTransformHierarchy,
    TabIndexAllNodes
    };

public slots:
  /// Reimplemented for internal reasons
  void setMRMLScene(vtkMRMLScene* scene) override;

  /// Change visibility of the MRML node ID columns
  void setMRMLIDsVisible(bool visible);

  /// Show or hide transforms
  void setTransformsVisible(bool visible);

  /// Set data node associated to the selected subject hierarchy item to the data node inspector
  void setDataNodeFromSubjectHierarchyItem(vtkIdType itemID);
  /// Set subject hierarchy item information to the label
  void setInfoLabelFromSubjectHierarchyItem(vtkIdType itemID);
  /// Handle subject hierarchy item modified event (update item info label if needed)
  void onSubjectHierarchyItemModified(vtkIdType itemID);

  /// Insert new transform node
  void insertTransformNode();
  /// Harden transform on current node
  void hardenTransformOnCurrentNode();

public:
  /// Assessor function for subject hierarchy model (for python)
  Q_INVOKABLE qMRMLSubjectHierarchyModel* subjectHierarchySceneModel()const;

protected:
  static void onSubjectHierarchyItemEvent(vtkObject* caller, unsigned long event, void* clientData, void* callData);

protected slots:
  void onCurrentNodeChanged(vtkMRMLNode* newCurrentNode);
  void onCurrentTabChanged(int tabIndex);
  void onHelpButtonClicked();

protected:
  void setup() override;

protected:
  QScopedPointer<qCjyxDataModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDataModuleWidget);
  Q_DISABLE_COPY(qCjyxDataModuleWidget);
};

#endif
