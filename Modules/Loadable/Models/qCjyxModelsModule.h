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

#ifndef __qCjyxModelsModule_h
#define __qCjyxModelsModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxModelsModuleExport.h"

class qCjyxModelsModulePrivate;

/// \ingroup Cjyx_QtModules_Models
class Q_CJYX_QTMODULES_MODELS_EXPORT qCjyxModelsModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:

  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxModelsModule(QObject *parent=nullptr);
  ~qCjyxModelsModule() override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  /// Return a custom icon for the module
  QIcon icon()const override;

  QStringList categories()const override;
  QStringList dependencies() const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

protected:
  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxModelsModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModelsModule);
  Q_DISABLE_COPY(qCjyxModelsModule);

};

#endif
