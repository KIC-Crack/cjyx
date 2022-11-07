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

#ifndef __qCjyxColorsModule_h
#define __qCjyxColorsModule_h

/// Cjyx includes
#include "qCjyxLoadableModule.h"

/// Colors includes
#include "qCjyxColorsModuleExport.h"

class qCjyxAbstractModuleWidget;
class qCjyxColorsModulePrivate;

class Q_CJYX_QTMODULES_COLORS_EXPORT qCjyxColorsModule
  : public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:
  typedef qCjyxLoadableModule Superclass;
  qCjyxColorsModule(QObject *parent=nullptr);
  ~qCjyxColorsModule() override;

  QStringList categories()const override;
  QIcon icon()const override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  /// Return help/acknowledgement text
  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;
  bool isHidden()const override;

  /// Specify editable node types
  QStringList associatedNodeTypes()const override;

  void setMRMLScene(vtkMRMLScene* newMRMLScene) override;

protected:
  /// Reimplemented to initialize the color logic
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxColorsModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxColorsModule);
  Q_DISABLE_COPY(qCjyxColorsModule);
};

#endif
