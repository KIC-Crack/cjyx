/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxSuperLoadableModuleTemplateModule_h
#define __qCjyxSuperLoadableModuleTemplateModule_h

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxSuperLoadableModuleTemplateModuleExport.h"

class qCjyxSuperLoadableModuleTemplateModulePrivate;

/// \ingroup Cjyx_QtModules_ExtensionTemplate
class Q_CJYX_QTMODULES_SUPERLOADABLEMODULETEMPLATE_EXPORT
qCjyxSuperLoadableModuleTemplateModule
  : public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:

  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxSuperLoadableModuleTemplateModule(QObject *parent=0);
  ~qCjyxSuperLoadableModuleTemplateModule() override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  QString helpText()const override;
  QString acknowledgementText()const override;
  QStringList contributors()const override;

  QIcon icon()const override;

  QStringList categories()const override;
  QStringList dependencies() const override;

protected:

  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

protected:
  QScopedPointer<qCjyxSuperLoadableModuleTemplateModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSuperLoadableModuleTemplateModule);
  Q_DISABLE_COPY(qCjyxSuperLoadableModuleTemplateModule);

};

#endif
