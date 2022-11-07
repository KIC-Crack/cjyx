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

// Qt includes
#include <QSettings>

// For:
//  - Cjyx_USE_PYTHONQT
#include "vtkCjyxConfigure.h"

// Cjyx includes
#include "qCjyxScriptedLoadableModuleFactory.h"
#include "qCjyxCoreApplication.h"
#include "qCjyxScriptedLoadableModule.h"
#include "qCjyxUtils.h"
#ifdef Cjyx_USE_PYTHONQT
# include "qCjyxCorePythonManager.h"
#endif

// For:
//  - Cjyx_QTSCRIPTEDMODULES_LIB_DIR

//----------------------------------------------------------------------------
// ctkFactoryScriptedItem methods

//----------------------------------------------------------------------------
bool ctkFactoryScriptedItem::load()
{
#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    // By convention, if the module is not embedded, "<MODULEPATH>" will be appended to PYTHONPATH
    if (!qCjyxCoreApplication::application()->isEmbeddedModule(this->path()))
      {
      QDir modulePathWithoutIntDir = QFileInfo(this->path()).dir();
      QString intDir = qCjyxCoreApplication::application()->intDir();
      if (intDir ==  modulePathWithoutIntDir.dirName())
        {
        modulePathWithoutIntDir.cdUp();
        }
      qCjyxCorePythonManager * pythonManager = qCjyxCoreApplication::application()->corePythonManager();
      pythonManager->appendPythonPaths(QStringList() << modulePathWithoutIntDir.absolutePath());
      }
    }
#endif

  return true;
}

//----------------------------------------------------------------------------
qCjyxAbstractCoreModule* ctkFactoryScriptedItem::instanciator()
{
  // Using a scoped pointer ensures the memory will be cleaned if instantiator
  // fails before returning the module. See QScopedPointer::take()
  QScopedPointer<qCjyxScriptedLoadableModule> module(new qCjyxScriptedLoadableModule());

  module->setPath(this->path());

  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  module->setInstalled(qCjyxUtils::isPluginInstalled(this->path(), app->cjyxHome()));
  module->setBuiltIn(qCjyxUtils::isPluginBuiltIn(this->path(), app->cjyxHome(), app->revision()));

  bool ret = module->setPythonSource(this->path());
  if (!ret)
    {
    return nullptr;
    }

  return module.take();
}

//-----------------------------------------------------------------------------
// qCjyxScriptedLoadableModuleFactoryPrivate

//-----------------------------------------------------------------------------
class qCjyxScriptedLoadableModuleFactoryPrivate
{
public:
  /// Return a list of module paths
  QStringList modulePaths() const;
};

//-----------------------------------------------------------------------------
// qCjyxScriptedLoadableModuleFactoryPrivate Methods

//-----------------------------------------------------------------------------
QStringList qCjyxScriptedLoadableModuleFactoryPrivate::modulePaths() const
{
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  Q_ASSERT(app);

  // cjyxHome shouldn't be empty
  Q_ASSERT(!app->cjyxHome().isEmpty());

  QStringList defaultQTModulePaths;

#ifdef Cjyx_BUILD_QTLOADABLEMODULES
  bool appendDefaultQTModulePaths = true;
#else
  bool appendDefaultQTModulePaths = app->isInstalled();
#endif
  if (appendDefaultQTModulePaths)
    {
    defaultQTModulePaths << app->cjyxHome() + "/" + Cjyx_QTSCRIPTEDMODULES_LIB_DIR;
    if (!app->intDir().isEmpty())
      {
      // On Win32, *both* paths have to be there, since scripts are installed
      // in the install location, and exec/libs are *automatically* installed
      // in intDir.
      defaultQTModulePaths << app->cjyxHome() + "/" + Cjyx_QTSCRIPTEDMODULES_LIB_DIR + "/" + app->intDir();
      }
    }

  // Add the default modules directory (based on the cjyx
  // installation or build tree) to the user paths
  QSettings * settings = app->revisionUserSettings();
  QStringList additionalModulePaths = app->toCjyxHomeAbsolutePaths(settings->value("Modules/AdditionalPaths").toStringList());
  QStringList qtModulePaths = additionalModulePaths + defaultQTModulePaths;

//  qDebug() << "scriptedModulePaths:" << qtModulePaths;

  return qtModulePaths;
}

//-----------------------------------------------------------------------------
// qCjyxScriptedLoadableModuleFactory Methods

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModuleFactory::qCjyxScriptedLoadableModuleFactory()
  : d_ptr(new qCjyxScriptedLoadableModuleFactoryPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModuleFactory::~qCjyxScriptedLoadableModuleFactory() = default;

//-----------------------------------------------------------------------------
bool qCjyxScriptedLoadableModuleFactory::isValidFile(const QFileInfo& file)const
{
  // Skip if current file isn't a python file
  if(!ctkAbstractFileBasedFactory<qCjyxAbstractCoreModule>::isValidFile(file))
    {
    return false;
    }

  if (qCjyxUtils::isCLIScriptedExecutable(file.filePath()))
    {
    return false;
    }

  // Otherwise, accept if current file is a python script
  if (file.suffix().compare("py", Qt::CaseInsensitive) == 0)
    {
    return true;
    }
  // Accept if current file is a pyc file and there is no associated py file
  if (file.suffix().compare("pyc", Qt::CaseInsensitive) == 0)
    {
    int length = file.filePath().size();
    QString pyFilePath = file.filePath().remove(length - 1, 1);
    if (!QFile::exists(pyFilePath))
      {
      return true;
      }
    }
  return false;
}

//----------------------------------------------------------------------------
ctkAbstractFactoryItem<qCjyxAbstractCoreModule>* qCjyxScriptedLoadableModuleFactory
::createFactoryFileBasedItem()
{
  return new ctkFactoryScriptedItem();
}

//-----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleFactory::registerItems()
{
  Q_D(qCjyxScriptedLoadableModuleFactory);
  this->registerAllFileItems(d->modulePaths());
}
