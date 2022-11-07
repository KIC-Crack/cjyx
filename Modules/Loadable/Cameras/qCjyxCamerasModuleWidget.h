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

#ifndef __qCjyxCamerasModuleWidget_h
#define __qCjyxCamerasModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxCamerasModuleExport.h"

class vtkMRMLNode;
class vtkMRMLViewNode;
class qCjyxCamerasModuleWidgetPrivate;

class Q_CJYX_QTMODULES_CAMERAS_EXPORT qCjyxCamerasModuleWidget
  : public qCjyxAbstractModuleWidget
{
  Q_OBJECT
public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxCamerasModuleWidget(QWidget *parent=nullptr);
  ~qCjyxCamerasModuleWidget() override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:
  ///
  /// Inherited from qCjyxWidget. Reimplemented for refresh issues.
  void setMRMLScene(vtkMRMLScene*) override;

protected:
  void setup() override;
  void synchronizeCameraWithView(vtkMRMLViewNode* currentViewNode);

protected slots:
  void onCurrentViewNodeChanged(vtkMRMLNode*);
  void setCameraToCurrentView(vtkMRMLNode*);
  void onCameraNodeAdded(vtkMRMLNode*);
  void onCameraNodeRemoved(vtkMRMLNode*);
  void synchronizeCameraWithView();

protected:
  QScopedPointer<qCjyxCamerasModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCamerasModuleWidget);
  Q_DISABLE_COPY(qCjyxCamerasModuleWidget);
};

#endif
