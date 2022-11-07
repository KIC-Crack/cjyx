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

#ifndef __qCjyxEventBrokerModuleWidget_h
#define __qCjyxEventBrokerModuleWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxModulesCoreExport.h"

class qCjyxEventBrokerModuleWidgetPrivate;

class Q_CJYX_MODULES_CORE_EXPORT qCjyxEventBrokerModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxEventBrokerModuleWidget(QWidget *parent=nullptr);
  ~qCjyxEventBrokerModuleWidget() override;

protected slots:
  void onCurrentObjectChanged(vtkObject* );

protected:
  QScopedPointer<qCjyxEventBrokerModuleWidgetPrivate> d_ptr;
  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxEventBrokerModuleWidget);
  Q_DISABLE_COPY(qCjyxEventBrokerModuleWidget);
};

#endif
