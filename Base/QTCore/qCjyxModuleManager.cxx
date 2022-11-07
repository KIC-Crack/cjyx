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

// Cjyx includes
#include "qCjyxModuleManager.h"
#include "qCjyxCoreApplication.h"
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxModuleFactoryManager.h"

// MRML includes

//-----------------------------------------------------------------------------
class qCjyxModuleManagerPrivate
{
public:
  qCjyxModuleFactoryManager* ModuleFactoryManager;
};

//-----------------------------------------------------------------------------
qCjyxModuleManager::qCjyxModuleManager(QObject* newParent)
  : Superclass(newParent), d_ptr(new qCjyxModuleManagerPrivate)
{
  Q_D(qCjyxModuleManager);
  d->ModuleFactoryManager = new qCjyxModuleFactoryManager(this);
  connect(d->ModuleFactoryManager, SIGNAL(moduleLoaded(QString)),
          this, SIGNAL(moduleLoaded(QString)));
  connect(d->ModuleFactoryManager, SIGNAL(moduleAboutToBeUnloaded(QString)),
          this, SIGNAL(moduleAboutToBeUnloaded(QString)));
}

//-----------------------------------------------------------------------------
qCjyxModuleManager::~qCjyxModuleManager() = default;

//-----------------------------------------------------------------------------
void qCjyxModuleManager::printAdditionalInfo()
{
  Q_D(qCjyxModuleManager);
  qDebug() << "qCjyxModuleManager (" << this << ")";
  d->ModuleFactoryManager->printAdditionalInfo();
}

//---------------------------------------------------------------------------
qCjyxModuleFactoryManager* qCjyxModuleManager::factoryManager()const
{
  Q_D(const qCjyxModuleManager);
  return const_cast<qCjyxModuleFactoryManager*>(d->ModuleFactoryManager);
}

//---------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxModuleManager::module(const QString& name)const
{
  Q_D(const qCjyxModuleManager);
  return d->ModuleFactoryManager->loadedModule(name);
}

//---------------------------------------------------------------------------
QStringList qCjyxModuleManager::modulesNames()const
{
  Q_D(const qCjyxModuleManager);
  return d->ModuleFactoryManager->loadedModuleNames();
}
