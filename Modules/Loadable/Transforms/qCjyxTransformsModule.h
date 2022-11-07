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

#ifndef __qCjyxTransformsModule_h
#define __qCjyxTransformsModule_h

// Cjyx includes
#include "qCjyxLoadableModule.h"

// Transforms includes
#include "qCjyxTransformsModuleExport.h"

class vtkMatrix4x4;
class vtkMRMLNode;
class qCjyxTransformsModulePrivate;

class Q_CJYX_QTMODULES_TRANSFORMS_EXPORT qCjyxTransformsModule
  : public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);
public:

  typedef qCjyxLoadableModule Superclass;
  qCjyxTransformsModule(QObject *parent=nullptr);
  ~qCjyxTransformsModule() override;

  /// Icon of the transform module
  QIcon icon()const override;

  QStringList categories()const override;

  /// Dependencies of the module
  QStringList dependencies()const override;

  /// Display name for the module
  qCjyxGetTitleMacro("Transforms");

  /// Help text of the module
  QString helpText()const override;

  /// Acknowledgement of the module
  QString acknowledgementText()const override;

  /// Contributors of the module
  QStringList contributors()const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

protected:
  /// Reimplemented to initialize the transforms IO
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

  QScopedPointer<qCjyxTransformsModulePrivate> d_ptr;
private:
  Q_DECLARE_PRIVATE(qCjyxTransformsModule);
  Q_DISABLE_COPY(qCjyxTransformsModule);
};

#endif
