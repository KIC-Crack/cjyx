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

// Cjyx includes
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxAbstractCoreModule.h"

#include "vtkCjyxConfigure.h" // XXX For modulePaths() function.

#include <vtkCjyxApplicationLogic.h>

// STD includes
#include <algorithm>

//-----------------------------------------------------------------------------
class qCjyxModuleFactoryManagerPrivate
{
  Q_DECLARE_PUBLIC(qCjyxModuleFactoryManager);
protected:
  qCjyxModuleFactoryManager* const q_ptr;
public:
  qCjyxModuleFactoryManagerPrivate(qCjyxModuleFactoryManager& object);

  QStringList LoadedModules;
  vtkCjyxApplicationLogic* AppLogic;
  vtkMRMLScene* MRMLScene;
};

//-----------------------------------------------------------------------------
// qCjyxModuleFactoryManagerPrivate methods
qCjyxModuleFactoryManagerPrivate
::qCjyxModuleFactoryManagerPrivate(qCjyxModuleFactoryManager& object)
  : q_ptr(&object)
{
  this->AppLogic = nullptr;
  this->MRMLScene = nullptr;
}

//-----------------------------------------------------------------------------
// qCjyxModuleFactoryManager methods

//-----------------------------------------------------------------------------
qCjyxModuleFactoryManager::qCjyxModuleFactoryManager(QObject * newParent)
  : Superclass(newParent), d_ptr(new qCjyxModuleFactoryManagerPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxModuleFactoryManager::~qCjyxModuleFactoryManager()
{
  this->unloadModules();
}

//-----------------------------------------------------------------------------
void qCjyxModuleFactoryManager::printAdditionalInfo()
{
  Q_D(qCjyxModuleFactoryManager);
  this->Superclass::printAdditionalInfo();
  qDebug() << "LoadedModules: " << d->LoadedModules;
}

//-----------------------------------------------------------------------------
int qCjyxModuleFactoryManager::loadModules()
{
  foreach(const QString& name, this->instantiatedModuleNames())
    {
    this->loadModule(name);
    }
  emit this->modulesLoaded(this->loadedModuleNames());
  return this->loadedModuleNames().count();
}

//---------------------------------------------------------------------------
bool qCjyxModuleFactoryManager::loadModules(const QStringList& modules)
{
  // Ensure requested modules are instantiated
  foreach(const QString& moduleKey, modules)
    {
    this->instantiateModule(moduleKey);
    }

  // Load requested modules
  foreach(const QString& moduleKey, modules)
    {
    if (!this->loadModule(moduleKey))
      {
      return false;
      }
    }

  return true;
}

//---------------------------------------------------------------------------
bool qCjyxModuleFactoryManager::loadModule(const QString& name)
{
  return this->loadModule(name, QString());
}

//---------------------------------------------------------------------------
bool qCjyxModuleFactoryManager::loadModule(const QString& name, const QString& dependee)
{
  Q_D(qCjyxModuleFactoryManager);

  if (dependee.isEmpty()
      && !this->explicitModules().isEmpty()
      && !this->explicitModules().contains(name))
    {
    return false;
    }

  if (!d->AppLogic)
    {
    qCritical() << Q_FUNC_INFO << " failed: application logic must be set before loading modules";
    return false;
    }

  // A module should be registered when attempting to load it
  if (!this->isRegistered(name) ||
      !this->isInstantiated(name))
    {
    //Q_ASSERT(d->ModuleFactoryManager.isRegistered(name));
    return false;
    }

  // Check if module has been loaded already
  if (this->isLoaded(name))
    {
    return true;
    }

  if (this->Superclass::isVerbose())
    {
    qDebug() << "Loading module" << name;
    }

  // Instantiate the module if needed
  qCjyxAbstractCoreModule* instance = this->moduleInstance(name);
  if (!instance)
    {
    qDebug() << "Failed to instantiate module" << name;
    return false;
    }

  // Load the modules the module depends on.
  // There is no cycle check, so be careful
  foreach(const QString& dependency, instance->dependencies())
    {
    // no-op if the module is already loaded
    bool dependencyLoaded = this->loadModule(dependency, name);
    if (!dependencyLoaded)
      {
      qWarning() << "When loading module " << name << ", the dependency"
                 << dependency << "failed to be loaded.";
      return false;
      }
    }

  // Update internal Map
  d->LoadedModules << name;

  // Sets the logic for the module in the application logic
  d->AppLogic->SetModuleLogic(name.toStdString().c_str(), instance->logic());

  // Initialize module
  instance->initialize(d->AppLogic);

  // Check the module has a title (required)
  if (instance->title().isEmpty())
    {
    d->AppLogic->SetModuleLogic(name.toStdString().c_str(), nullptr);
    qWarning() << "Failed to retrieve module title corresponding to module name: " << name;
    Q_ASSERT(!instance->title().isEmpty());
    return false;
    }

  // Set the MRML scene
  instance->setMRMLScene(d->MRMLScene);

  // Module should also be aware if current MRML scene has changed
  this->connect(this,SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
                instance, SLOT(setMRMLScene(vtkMRMLScene*)));

  // Handle post-load initialization
  emit this->moduleLoaded(name);

  return true;
}

//---------------------------------------------------------------------------
bool qCjyxModuleFactoryManager::isLoaded(const QString& name)const
{
  Q_D(const qCjyxModuleFactoryManager);
  return d->LoadedModules.contains(name);
}

//-----------------------------------------------------------------------------
QStringList qCjyxModuleFactoryManager::loadedModuleNames()const
{
  Q_D(const qCjyxModuleFactoryManager);
  return d->LoadedModules;
}

//-----------------------------------------------------------------------------
void qCjyxModuleFactoryManager::unloadModules()
{
  QStringList modulesToUnload = this->loadedModuleNames();
  // unload in the reverse the order of load to respect dependencies
  std::reverse(modulesToUnload.begin(), modulesToUnload.end());
  emit this->modulesAboutToBeUnloaded(modulesToUnload);
  emit this->modulesAboutToBeUninstantiated(modulesToUnload);
  foreach(const QString& name, modulesToUnload)
    {
    this->unloadModule(name);
    }
  emit this->modulesUninstantiated(modulesToUnload);
  emit this->modulesUnloaded(modulesToUnload);
}

//-----------------------------------------------------------------------------
void qCjyxModuleFactoryManager::uninstantiateModules()
{
  Q_D(qCjyxModuleFactoryManager);
  if (!d->LoadedModules.isEmpty())
    {
    // unload first then uninstantiate the remaining modules
    this->unloadModules();
    }
  this->Superclass::uninstantiateModules();
}

//---------------------------------------------------------------------------
void qCjyxModuleFactoryManager::unloadModule(const QString& name)
{
  Q_D(qCjyxModuleFactoryManager);
  if (!this->isLoaded(name))
    {
    return;
    }
  emit this->moduleAboutToBeUnloaded(name);
  d->LoadedModules.removeOne(name);
  this->uninstantiateModule(name);

  // Remove the registration of module logic in application logic.
  d->AppLogic->SetModuleLogic(name.toStdString().c_str(), nullptr);

  emit this->moduleUnloaded(name);
}

//---------------------------------------------------------------------------
void qCjyxModuleFactoryManager::uninstantiateModule(const QString& name)
{
  if (this->isLoaded(name))
    {
    this->unloadModule(name);
    return;
    }

  this->Superclass::uninstantiateModule(name);
}

//---------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxModuleFactoryManager::loadedModule(const QString& name)const
{
  if (!this->isRegistered(name))
    {
    qDebug() << "The module" << name << "has not been registered.";
    qDebug() << "The following modules have been registered:"
             << this->registeredModuleNames();
    return nullptr;
    }
  if (!this->isInstantiated(name))
    {
    qDebug() << "The module" << name << "has been registered but not instantiated.";
    qDebug() << "The following modules have been instantiated:"
             << this->instantiatedModuleNames();
    return nullptr;
    }

  if (!this->isLoaded(name))
    {
    qDebug()<< "The module" << name << "has not been loaded.";
    qDebug() << "The following modules have been loaded:"
             << this->loadedModuleNames();
    return nullptr;
    }
  return this->moduleInstance(name);
}

//-----------------------------------------------------------------------------
void qCjyxModuleFactoryManager::setAppLogic(vtkCjyxApplicationLogic* logic)
{
  Q_D(qCjyxModuleFactoryManager);
  d->AppLogic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxApplicationLogic* qCjyxModuleFactoryManager::appLogic()const
{
  Q_D(const qCjyxModuleFactoryManager);
  return d->AppLogic;
}

//-----------------------------------------------------------------------------
QStringList qCjyxModuleFactoryManager::modulePaths(const QString& basePath)
{
  // XXX Each factory should be updated with virtual methods like "hasModulePath(basePath)"
  //     and "modulePath(basePath)".

  QStringList paths;

  QStringList subPaths;
#ifdef Cjyx_USE_PYTHONQT
  subPaths << Cjyx_QTSCRIPTEDMODULES_LIB_DIR;
#endif
#ifdef Cjyx_BUILD_CLI_SUPPORT
  subPaths << Cjyx_CLIMODULES_BIN_DIR;
#endif
  subPaths << Cjyx_QTLOADABLEMODULES_LIB_DIR;

  foreach(const QString& subPath, subPaths)
    {
    QString candidatePath = QDir(basePath).filePath(subPath);
    if (QDir(candidatePath).exists())
      {
      paths << candidatePath;
      }
    }
  return paths;
}

//-----------------------------------------------------------------------------
void qCjyxModuleFactoryManager::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qCjyxModuleFactoryManager);
  d->MRMLScene = scene;
  emit mrmlSceneChanged(d->MRMLScene);
}

//-----------------------------------------------------------------------------
vtkMRMLScene* qCjyxModuleFactoryManager::mrmlScene()const
{
  Q_D(const qCjyxModuleFactoryManager);
  return d->MRMLScene;
}
