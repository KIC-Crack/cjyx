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

#ifndef __qCjyxEventBrokerModule_h
#define __qCjyxEventBrokerModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxCoreModule.h"

#include "qCjyxModulesCoreExport.h"

class qCjyxEventBrokerModulePrivate;

class Q_CJYX_MODULES_CORE_EXPORT qCjyxEventBrokerModule :
  public qCjyxCoreModule
{
  Q_OBJECT
public:

  typedef qCjyxCoreModule Superclass;
  qCjyxEventBrokerModule(QObject *parent=nullptr);
  ~qCjyxEventBrokerModule() override;

  QStringList categories()const override;

  /// Display name for the module
  qCjyxGetTitleMacro("Event Broker");

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

protected:
  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

  QScopedPointer<qCjyxEventBrokerModulePrivate> d_ptr;
private:
  Q_DECLARE_PRIVATE(qCjyxEventBrokerModule);
  Q_DISABLE_COPY(qCjyxEventBrokerModule);
};

#endif
