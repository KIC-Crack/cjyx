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

#ifndef __qCjyxViewControllersModuleWidget_h
#define __qCjyxViewControllersModuleWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

// View Controllers includes
#include "qCjyxViewControllersModuleExport.h"

class qCjyxViewControllersModuleWidgetPrivate;
class vtkMRMLNode;

class Q_CJYX_QTMODULES_VIEWCONTROLLERS_EXPORT qCjyxViewControllersModuleWidget
  : public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxViewControllersModuleWidget(QWidget *parent=nullptr);
  ~qCjyxViewControllersModuleWidget() override;

public slots:
  void setMRMLScene(vtkMRMLScene *newScene) override;
  void onNodeAddedEvent(vtkObject* scene, vtkObject* node);
  void onNodeRemovedEvent(vtkObject* scene, vtkObject* node);
  void onLayoutChanged(int);

protected slots:
  void onAdvancedViewNodeChanged(vtkMRMLNode*);

protected:
  void setup() override;

protected:
  QScopedPointer<qCjyxViewControllersModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxViewControllersModuleWidget);
  Q_DISABLE_COPY(qCjyxViewControllersModuleWidget);
};

#endif
