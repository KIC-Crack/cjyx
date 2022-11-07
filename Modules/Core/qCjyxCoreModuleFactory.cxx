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

// Cjyx/CoreModules includes
#include "qCjyxCoreModuleFactory.h"
#include "qCjyxEventBrokerModule.h"
#include "qCjyxUtils.h"

//-----------------------------------------------------------------------------
class qCjyxCoreModuleFactoryPrivate
{
  Q_DECLARE_PUBLIC(qCjyxCoreModuleFactory);
protected:
  qCjyxCoreModuleFactory* const q_ptr;

public:
  qCjyxCoreModuleFactoryPrivate(qCjyxCoreModuleFactory& object);

  /// Add a module class to the core module factory
  template<typename ClassType>
  void registerCoreModule();
};

//-----------------------------------------------------------------------------
// qCjyxModuleFactoryPrivate methods

//-----------------------------------------------------------------------------
qCjyxCoreModuleFactoryPrivate::qCjyxCoreModuleFactoryPrivate(qCjyxCoreModuleFactory& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
template<typename ClassType>
void qCjyxCoreModuleFactoryPrivate::registerCoreModule()
{
  Q_Q(qCjyxCoreModuleFactory);

  QString _moduleName;
  if (!q->registerQObject<ClassType>(_moduleName))
    {
    if (q->verbose())
      {
      qDebug() << "Failed to register module: " << _moduleName;
      }
    return;
    }
}

//-----------------------------------------------------------------------------
// qCjyxCoreModuleFactory methods

//-----------------------------------------------------------------------------
qCjyxCoreModuleFactory::qCjyxCoreModuleFactory()
  : d_ptr(new qCjyxCoreModuleFactoryPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxCoreModuleFactory::~qCjyxCoreModuleFactory() = default;

//-----------------------------------------------------------------------------
void qCjyxCoreModuleFactory::registerItems()
{
  Q_D(qCjyxCoreModuleFactory);
  d->registerCoreModule<qCjyxEventBrokerModule>();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreModuleFactory::objectNameToKey(const QString& objectName)
{
  return qCjyxCoreModuleFactory::extractModuleName(objectName);
}

//-----------------------------------------------------------------------------
QString qCjyxCoreModuleFactory::extractModuleName(const QString& className)
{
  return qCjyxUtils::extractModuleNameFromClassName(className);
}
