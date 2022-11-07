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
#include <QDir>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxAbstractModuleFactoryManager.h"
#include "qCjyxAbstractCoreModule.h"

// STD includes
#include <csignal>
#include <typeinfo>

//-----------------------------------------------------------------------------
class qCjyxAbstractModuleFactoryManagerPrivate
{
  Q_DECLARE_PUBLIC(qCjyxAbstractModuleFactoryManager);
protected:
  qCjyxAbstractModuleFactoryManager* const q_ptr;
public:
  qCjyxAbstractModuleFactoryManagerPrivate(qCjyxAbstractModuleFactoryManager& object);

  void printAdditionalInfo();

  typedef qCjyxAbstractModuleFactoryManager::qCjyxModuleFactory
    qCjyxModuleFactory;
  typedef qCjyxAbstractModuleFactoryManager::qCjyxFileBasedModuleFactory
    qCjyxFileBasedModuleFactory;
  QVector<qCjyxFileBasedModuleFactory*> fileBasedFactories()const;
  QVector<qCjyxModuleFactory*> notFileBasedFactories()const;

  // Helper function that returns module factory for a module name, without
  // the risk of creating a nullptr entry if the module is not registered.
  qCjyxModuleFactory* registeredModuleFactory(const QString& moduleName)const;

  QStringList SearchPaths;
  QStringList ExplicitModules;
  QStringList ModulesToIgnore;
  QMap<QString, QFileInfo> IgnoredModules;
  QMap<qCjyxModuleFactory*, int> Factories;
  QMap<QString, qCjyxModuleFactory*> RegisteredModules;
  QMap<QString, QStringList> ModuleDependees;

  bool Verbose;
};

//-----------------------------------------------------------------------------
// qCjyxAbstractModuleFactoryManagerPrivate methods
qCjyxAbstractModuleFactoryManagerPrivate::qCjyxAbstractModuleFactoryManagerPrivate(qCjyxAbstractModuleFactoryManager& object)
  : q_ptr(&object)
{
  this->Verbose = false;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManagerPrivate::printAdditionalInfo()
{
  Q_Q(qCjyxAbstractModuleFactoryManager);
  qDebug() << "Factories:";
  foreach(qCjyxAbstractModuleFactoryManager::qCjyxModuleFactory* factory,
          this->Factories.keys())
    {
    // todo: qCjyxModuleFactory should derive from QObject.
    qDebug() << "\t" << typeid(factory).name() << ": ";
    factory->printAdditionalInfo();
    }
  qDebug() << "Modules to ignore:" << q->modulesToIgnore();
  qDebug() << "Registered modules:" << q->registeredModuleNames();
  qDebug() << "Ignored modules:" << q->ignoredModuleNames();
  qDebug() << "Instantiated modules:" << q->instantiatedModuleNames();
}

//-----------------------------------------------------------------------------
QVector<qCjyxAbstractModuleFactoryManagerPrivate::qCjyxFileBasedModuleFactory*>
qCjyxAbstractModuleFactoryManagerPrivate::fileBasedFactories()const
{
  QVector<qCjyxFileBasedModuleFactory*> factories;
  foreach(qCjyxModuleFactory* factory, this->Factories.keys())
    {
    if (dynamic_cast<qCjyxFileBasedModuleFactory*>(factory) != nullptr)
      {
      factories << dynamic_cast<qCjyxFileBasedModuleFactory*>(factory);
      }
    }
  return factories;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleFactoryManagerPrivate::qCjyxModuleFactory*
qCjyxAbstractModuleFactoryManagerPrivate
::registeredModuleFactory(const QString& moduleName)const
{
  if (!this->RegisteredModules.contains(moduleName))
    {
    return nullptr;
    }
  return this->RegisteredModules[moduleName];
}

//-----------------------------------------------------------------------------
QVector<qCjyxAbstractModuleFactoryManagerPrivate::qCjyxModuleFactory*>
qCjyxAbstractModuleFactoryManagerPrivate
::notFileBasedFactories()const
{
  QVector<qCjyxModuleFactory*> factories;
  foreach(qCjyxModuleFactory* factory, this->Factories.keys())
    {
    if (dynamic_cast<qCjyxFileBasedModuleFactory*>(factory) == nullptr)
      {
      factories << factory;
      }
    }
  return factories;
}

//-----------------------------------------------------------------------------
// qCjyxAbstractModuleFactoryManager methods

//-----------------------------------------------------------------------------
qCjyxAbstractModuleFactoryManager::qCjyxAbstractModuleFactoryManager(QObject * newParent)
  : Superclass(newParent), d_ptr(new qCjyxAbstractModuleFactoryManagerPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleFactoryManager::~qCjyxAbstractModuleFactoryManager()
{
  this->uninstantiateModules();
  this->unregisterFactories();
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::printAdditionalInfo()
{
  Q_D(qCjyxAbstractModuleFactoryManager);

  qDebug() << "qCjyxAbstractModuleFactoryManager (" << this << ")";
  d->printAdditionalInfo();
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager
::registerFactory(qCjyxModuleFactory* factory, int priority)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  Q_ASSERT(!d->Factories.contains(factory));
  d->Factories[factory] = priority;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::unregisterFactory(qCjyxModuleFactory* factory)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  Q_ASSERT(d->Factories.contains(factory));
  d->Factories.remove(factory);
  delete factory;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::unregisterFactories()
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  while (!d->Factories.isEmpty())
    {
    this->unregisterFactory(d->Factories.begin().key());
    }
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::setSearchPaths(const QStringList& paths)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  d->SearchPaths = paths;
  d->SearchPaths.removeDuplicates();
  d->SearchPaths.removeAll(QString());
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::searchPaths()const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->SearchPaths;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::setExplicitModules(const QStringList& moduleNames)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  if (d->ModulesToIgnore == moduleNames)
    {
    return;
    }
  d->ExplicitModules = moduleNames;
  emit explicitModulesChanged(moduleNames);
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::explicitModules()const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->ExplicitModules;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::setModulesToIgnore(const QStringList& moduleNames)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  if (d->ModulesToIgnore == moduleNames)
    {
    return;
    }
  d->ModulesToIgnore = moduleNames;
  emit modulesToIgnoreChanged(moduleNames);
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::modulesToIgnore()const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->ModulesToIgnore;
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::ignoredModuleNames()const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->IgnoredModules.keys();
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::registerModules()
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  // Register "regular" factories first
  // \todo: don't support factories other than filebased factories
  foreach(qCjyxModuleFactory* factory, d->notFileBasedFactories())
    {
    factory->registerItems();
    foreach(const QString& moduleName, factory->itemKeys())
      {
      if (d->Verbose)
        {
        qDebug() << "Registering: " << moduleName;
        }
      d->RegisteredModules[moduleName] = factory;
      emit moduleRegistered(moduleName);
      }
    }
  // then register file based factories
  foreach(const QString& path, d->SearchPaths)
    {
    if (d->Verbose)
      {
      qDebug() << "Searching path: " << path;
      }
    this->registerModules(path);
    }
  emit this->modulesRegistered(d->RegisteredModules.keys());
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::registerModules(const QString& path)
{
  QDir directory(path);
  /// \tbd recursive search ?
  foreach (const QFileInfo& file,
           directory.entryInfoList(QDir::Files))
    {
    this->registerModule(file);
    }
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::registerModule(const QFileInfo& file)
{
  Q_D(qCjyxAbstractModuleFactoryManager);

  qCjyxFileBasedModuleFactory* moduleFactory = nullptr;
  foreach(qCjyxFileBasedModuleFactory* factory, d->fileBasedFactories())
    {
    if (d->Verbose)
      {
      qDebug() << " checking file: " << file.absoluteFilePath() << " as a " << typeid(*factory).name();
      }
    if (!factory->isValidFile(file))
      {
      continue;
      }
    if (d->Verbose)
      {
      qDebug() << " recognized file: " << file.absoluteFilePath() << " as a " << typeid(*factory).name();
      }
    moduleFactory = factory;
    break;
    }
  // File not supported by any factory
  if (moduleFactory == nullptr)
    {
    return;
    }
  QString moduleName = moduleFactory->itemKey(file);
  bool dontEmitSignal = false;
  // Has the module been already registered
  qCjyxModuleFactory* existingModuleFactory = d->registeredModuleFactory(moduleName);
  if (existingModuleFactory)
    {
    if (d->Factories[existingModuleFactory] >=
        d->Factories[moduleFactory])
      {
      if (d->Verbose)
        {
        qDebug() << " file: " << file.absoluteFilePath() << " already registered";
        }
      return;
      }
    // Replace the factory of the registered module with this higher priority
    // factory.
    //existingModuleFactory->unregisterItem(file);
    dontEmitSignal = true;
    }
  if (d->ModulesToIgnore.contains(moduleName))
    {
    //qDebug() << "Ignore module" << moduleName;
    if (d->Verbose)
      {
      qDebug() << " file: " << file.absoluteFilePath() << " is in ignore list";
      }
    d->IgnoredModules[moduleName] = file;
    emit moduleIgnored(moduleName);
    return;
    }
  QString registeredModuleName = moduleFactory->registerFileItem(file);
  if (registeredModuleName != moduleName)
    {
    //qDebug() << "Ignore module" << moduleName;
    if (d->Verbose)
      {
      qDebug() << " file: " << file.absoluteFilePath() << " ignored because moduleName does not match registeredModuleName";
      }
    d->IgnoredModules[moduleName] = file;
    emit moduleIgnored(moduleName);
    return;
    }
  d->RegisteredModules[moduleName] = moduleFactory;
  if (!dontEmitSignal)
    {
    emit moduleRegistered(moduleName);
    }
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::instantiateModules()
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  foreach (const QString& moduleName, d->RegisteredModules.keys())
    {
    this->instantiateModule(moduleName);
    }

  // XXX See issue #3804
  // Python maps SIGINT (control-c) to its own handler.  We will remap it
  // to the default so that control-c works. Note that this is already done in
  // "ctkAbstractPythonManager::initPythonQt" but the import of 'async'
  // module by 'gitdb' module (itself imported by the CjyxExtensionWizard)
  // resets the handler.
  #ifdef SIGINT
  signal(SIGINT, SIG_DFL);
  #endif

  emit this->modulesInstantiated(this->instantiatedModuleNames());
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxAbstractModuleFactoryManager
::instantiateModule(const QString& moduleName)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  qCjyxModuleFactory* factory = d->registeredModuleFactory(moduleName);
  if (!factory)
    {
    qCritical() << "Fail to instantiate module " << moduleName << " (not registered)";
    return nullptr;
    }
  qCjyxAbstractCoreModule* module = factory->instantiate(moduleName);
  if (!module)
    {
    qCritical() << "Fail to instantiate module " << moduleName;
    return nullptr;
    }
  module->setName(moduleName);
  module->setObjectName(QString("%1Module").arg(moduleName));
  foreach(const QString& associatedNodeType, module->associatedNodeTypes())
    {
    qCjyxCoreApplication::application()->addModuleAssociatedNodeType(associatedNodeType, moduleName);
    }
  foreach(const QString& dependency, module->dependencies())
    {
    QStringList dependees = d->ModuleDependees.value(dependency);
    if (!dependees.contains(moduleName))
      {
      d->ModuleDependees.insert(dependency, dependees << moduleName);
      }
    }
  emit moduleInstantiated(moduleName);
  return module;
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::registeredModuleNames() const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->RegisteredModules.keys();
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::instantiatedModuleNames() const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  QStringList instantiatedModules;
  foreach(const QString& moduleName, d->RegisteredModules.keys())
    {
    qCjyxModuleFactory* factory = d->registeredModuleFactory(moduleName);
    if (!factory)
      {
      continue;
      }
    if (factory->instance(moduleName))
      {
      instantiatedModules << moduleName;
      }
    }
  return instantiatedModules;
}


//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::uninstantiateModules()
{
  QStringList modulesToUninstantiate = this->instantiatedModuleNames();
  emit modulesAboutToBeUninstantiated(modulesToUninstantiate);
  foreach(const QString& name, modulesToUninstantiate)
    {
    this->uninstantiateModule(name);
    }
  emit modulesUninstantiated(modulesToUninstantiate);
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::uninstantiateModule(const QString& moduleName)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  if (d->Verbose)
    {
    qDebug() << "Uninstantiating:" << moduleName;
    }
  qCjyxModuleFactory* factory = d->registeredModuleFactory(moduleName);
  if (!factory)
    {
    qWarning() << "uninstantiateModule failed: module " << moduleName << " is not registered";
    return;
    }
  emit moduleAboutToBeUninstantiated(moduleName);
  factory->uninstantiate(moduleName);
  emit moduleUninstantiated(moduleName);
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxAbstractModuleFactoryManager::moduleInstance(const QString& moduleName)const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  qCjyxModuleFactory* factory = d->registeredModuleFactory(moduleName);
  return factory ? factory->instance(moduleName) : nullptr;
}

//-----------------------------------------------------------------------------
bool qCjyxAbstractModuleFactoryManager::isRegistered(const QString& moduleName)const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return (d->registeredModuleFactory(moduleName) != nullptr);
}

//-----------------------------------------------------------------------------
bool qCjyxAbstractModuleFactoryManager::isInstantiated(const QString& moduleName)const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  bool instantiated = this->isRegistered(moduleName) &&
    d->RegisteredModules[moduleName]->instance(moduleName) != nullptr;
  return instantiated;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::setVerboseModuleDiscovery(bool verbose)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  foreach (qCjyxModuleFactory* factory, d->Factories.keys())
    {
    factory->setVerbose(verbose);
    }
  this->setIsVerbose(verbose);
}

//---------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::dependentModules(const QString& dependency)const
{
  QStringList dependents;
  foreach(const QString& moduleName, this->instantiatedModuleNames())
    {
    qCjyxAbstractCoreModule* coreModule = this->moduleInstance(moduleName);
    if (coreModule && coreModule->dependencies().contains(dependency))
      {
      dependents << moduleName;
      }
    }
  return dependents;
}

//---------------------------------------------------------------------------
QStringList qCjyxAbstractModuleFactoryManager::moduleDependees(const QString& module)const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->ModuleDependees.value(module);
}

//---------------------------------------------------------------------------
bool  qCjyxAbstractModuleFactoryManager::isVerbose()const
{
  Q_D(const qCjyxAbstractModuleFactoryManager);
  return d->Verbose;
}

//---------------------------------------------------------------------------
void qCjyxAbstractModuleFactoryManager::setIsVerbose(bool flag)
{
  Q_D(qCjyxAbstractModuleFactoryManager);
  d->Verbose = flag;
}

