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

  This file was originally developed by Johan Andruejol, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxUnitsModule_h
#define __qCjyxUnitsModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxUnitsModuleExport.h"

class qCjyxUnitsModulePrivate;

class Q_CJYX_QTMODULES_UNITS_EXPORT qCjyxUnitsModule
  : public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:

  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxUnitsModule(QObject *parent=nullptr);
  ~qCjyxUnitsModule() override;

  qCjyxGetTitleMacro("Units");

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  QIcon icon()const override;

  QStringList categories()const override;
  QStringList dependencies() const override;

  /// Hide unit module by default
  bool isHidden() const override;

protected:
  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxUnitsModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxUnitsModule);
  Q_DISABLE_COPY(qCjyxUnitsModule);

};

#endif
