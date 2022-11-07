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

#ifndef __qCjyxLoadableModule_h
#define __qCjyxLoadableModule_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModule.h"

#include "qCjyxBaseQTGUIExport.h"

class qCjyxLoadableModulePrivate;
class qCjyxCorePythonManager;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxLoadableModule : public qCjyxAbstractModule
{
  Q_OBJECT

public:

  typedef qCjyxLoadableModule Self;
  typedef qCjyxAbstractModule Superclass;
  qCjyxLoadableModule(QObject *parent=nullptr);
  ~qCjyxLoadableModule() override;

  /// Return help/acknowledgement text
  QString helpText()const override;
  QString acknowledgementText()const override;

  /// \brief Import python extensions associated with \a modulePath.
  ///
  /// \a modulePath can either be the path to the module library or the
  /// directory containing the module library.
  ///
  /// Python extensions corresponds to files matching the following wildcard expression:
  /// <ul>
  ///   <li>vtkCjyx*ModuleLogic.py</li>
  ///   <li>vtkCjyx*ModuleMRML.py</li>
  ///   <li>vtkCjyx*ModuleMRMLDisplayableManager.py</li>
  ///   <li>qCjyx*PythonQt.* python</li>
  /// </ul>
  ///
  /// These files are searched within the \a modulePath directory minus the \a IntDir
  /// if it applies.
  ///
  /// \sa qCjyxCoreApplication::intDir(), qCjyxCoreApplication::corePythonManager()
  static bool importModulePythonExtensions(qCjyxCorePythonManager * pythonManager,
                                           const QString& intDir,
                                           const QString& modulePath,
                                           bool isEmbedded=false);

  /// Set \a module identified by \a moduleName has an attribute of "cjyx.modules" module dictionary.
  /// qCjyxCoreApplication::corePythonManager()
  static bool addModuleToCjyxModules(qCjyxCorePythonManager * pythonManager,
                                       qCjyxAbstractModule *module,
                                       const QString& moduleName);

  /// Set \a moduleName has an attribute of "cjyx.moduleNames" module dictionary.
  /// qCjyxCoreApplication::corePythonManager()
  static bool addModuleNameToCjyxModuleNames(qCjyxCorePythonManager * pythonManager,
                                               const QString& moduleName);

protected:
  void setup() override;

protected:
  QScopedPointer<qCjyxLoadableModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxLoadableModule);
  Q_DISABLE_COPY(qCjyxLoadableModule);
};

Q_DECLARE_INTERFACE(qCjyxLoadableModule,
                     "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");

#endif
