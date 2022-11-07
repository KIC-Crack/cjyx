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

#include "vtkCjyxConfigure.h" // For Cjyx_USE_PYTHONQT

// Qt includes
#include <QDebug>
#ifdef Cjyx_USE_PYTHONQT
# include <QDir>
# include <QVariant>
#endif

// CTK includes
#ifdef Cjyx_USE_PYTHONQT
# include <ctkScopedCurrentDir.h>
#endif

// Cjyx includes
#include "qCjyxLoadableModule.h"
#ifdef Cjyx_USE_PYTHONQT
# include "qCjyxCoreApplication.h"
# include "qCjyxCorePythonManager.h"
#endif

//-----------------------------------------------------------------------------
class qCjyxLoadableModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxLoadableModule::qCjyxLoadableModule(QObject* _parentObject)
  : Superclass(_parentObject)
  , d_ptr(new qCjyxLoadableModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxLoadableModule::~qCjyxLoadableModule() = default;

//-----------------------------------------------------------------------------
bool qCjyxLoadableModule::importModulePythonExtensions(
    qCjyxCorePythonManager * pythonManager,
    const QString& intDir,const QString& modulePath,
    bool isEmbedded)
{
  Q_UNUSED(intDir);
#ifdef Cjyx_USE_PYTHONQT
  if(!pythonManager)
    {
    return false;
    }

  QString pythonModuleDir = QFileInfo(modulePath).absoluteFilePath();
  if (!QFileInfo(pythonModuleDir).isDir())
    {
    pythonModuleDir = QFileInfo(pythonModuleDir).absolutePath();
    }

  // Update current application directory, so that *PythonD modules can be loaded
  ctkScopedCurrentDir scopedCurrentDir(pythonModuleDir);

  if (!isEmbedded)
    {
    QStringList paths; paths << scopedCurrentDir.currentPath();
    pythonManager->appendPythonPaths(paths);
    }

  pythonManager->executeString(QString(
        "from cjyx.util import importVTKClassesFromDirectory;"
        "importVTKClassesFromDirectory(%1, 'cjyx', filematch='vtkCjyx*ModuleLogicPython.*');"
        "importVTKClassesFromDirectory(%1, 'cjyx', filematch='vtkCjyx*ModuleMRMLPython.*');"
        "importVTKClassesFromDirectory(%1, 'cjyx', filematch='vtkCjyx*ModuleMRMLDisplayableManagerPython.*');"
        "importVTKClassesFromDirectory(%1, 'cjyx', filematch='vtkCjyx*ModuleVTKWidgetsPython.*');"
        ).arg(qCjyxCorePythonManager::toPythonStringLiteral(scopedCurrentDir.currentPath())));
  pythonManager->executeString(QString(
        "from cjyx.util import importQtClassesFromDirectory;"
        "importQtClassesFromDirectory(%1, 'cjyx', filematch='qCjyx*PythonQt.*');"
        ).arg(qCjyxCorePythonManager::toPythonStringLiteral(scopedCurrentDir.currentPath())));
  return !pythonManager->pythonErrorOccured();
#else
  Q_UNUSED(isEmbedded);
  Q_UNUSED(modulePath);
  Q_UNUSED(pythonManager);
  return false;
#endif
}

//-----------------------------------------------------------------------------
bool qCjyxLoadableModule::addModuleToCjyxModules(
    qCjyxCorePythonManager * pythonManager,
    qCjyxAbstractModule * module,
    const QString& moduleName)
{
#ifdef Cjyx_USE_PYTHONQT
  if(!pythonManager || !module || moduleName.isEmpty())
    {
    return false;
    }
  pythonManager->addObjectToPythonMain("_tmp_module_variable", module);
  pythonManager->executeString(
        QString("import __main__;"
                "setattr( cjyx.modules, %1, __main__._tmp_module_variable);"
                "del __main__._tmp_module_variable").arg(
          qCjyxCorePythonManager::toPythonStringLiteral(moduleName.toLower())));
  return !pythonManager->pythonErrorOccured();
#else
  Q_UNUSED(pythonManager);
  Q_UNUSED(module);
  Q_UNUSED(moduleName);
  return false;
#endif
}

//-----------------------------------------------------------------------------
bool qCjyxLoadableModule::addModuleNameToCjyxModuleNames(
    qCjyxCorePythonManager * pythonManager,
    const QString& moduleName)
{
#ifdef Cjyx_USE_PYTHONQT
  if(!pythonManager || moduleName.isEmpty())
    {
    return false;
    }
  pythonManager->executeString(
        QString("import __main__;"
                "setattr( cjyx.moduleNames, %1, %2)")
                .arg(qCjyxCorePythonManager::toPythonStringLiteral(moduleName.toLower()))
                .arg(qCjyxCorePythonManager::toPythonStringLiteral(moduleName)));
  return !pythonManager->pythonErrorOccured();
#else
  Q_UNUSED(pythonManager);
  Q_UNUSED(moduleName);
  return false;
#endif
}

//-----------------------------------------------------------------------------
void qCjyxLoadableModule::setup()
{
#ifndef QT_NO_DEBUG
  Q_D(qCjyxLoadableModule);
  // Q_ASSERT(d != 0);
#endif

#ifdef Cjyx_USE_PYTHONQT
  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  if (app && !qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    // By convention, if the module is not embedded,
    // "<MODULEPATH>/Python" will be appended to PYTHONPATH
    if (!Self::importModulePythonExtensions(
          app->corePythonManager(), app->intDir(), this->path(),
          app->isEmbeddedModule(this->path())))
      {
      qWarning() << "qCjyxLoadableModule::setup - Failed to import module" << this->name() << "python extensions";
      }
    }
#endif
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModule::helpText()const
{
  qDebug() << "WARNING: " << this->metaObject()->className()
           << "::helpText() is not implemented";
  return QString();
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModule::acknowledgementText()const
{
  qDebug() << "WARNING: " << this->metaObject()->className()
           << "::acknowledgementText - Not implemented";
  return QString();
}
