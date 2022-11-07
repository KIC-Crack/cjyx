/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Alex Yarmakovich, Isomics Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxVolumeRenderingModule_h
#define __qCjyxVolumeRenderingModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxVolumeRenderingModuleExport.h"

class qCjyxVolumeRenderingModulePrivate;

/// \ingroup Cjyx_QtModules_VolumeRendering
class Q_CJYX_QTMODULES_VOLUMERENDERING_EXPORT qCjyxVolumeRenderingModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);

public:

  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxVolumeRenderingModule(QObject *parent=nullptr);
  ~qCjyxVolumeRenderingModule() override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  /// Help of the module
  QString helpText()const override;
  /// Acknowledgement for the module
  QString acknowledgementText()const override;
  /// Contributors of the module.
  QStringList contributors()const override;

  /// Return a custom icon for the module
  QIcon icon()const override;

  QStringList categories()const override;

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
  QScopedPointer<qCjyxVolumeRenderingModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxVolumeRenderingModule);
  Q_DISABLE_COPY(qCjyxVolumeRenderingModule);

};

#endif
