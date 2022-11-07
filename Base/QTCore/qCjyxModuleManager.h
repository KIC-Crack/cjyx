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

#ifndef __qCjyxModuleManager_h
#define __qCjyxModuleManager_h

// Qt includes
#include <QStringList>
#include <QObject>

// CTK includes
#include <ctkPimpl.h>

#include "qCjyxBaseQTCoreExport.h"

class qCjyxAbstractCoreModule;
class qCjyxModuleFactoryManager;

class qCjyxModuleManagerPrivate;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxModuleManager : public QObject
{
  Q_OBJECT
public:
  typedef QObject Superclass;
  qCjyxModuleManager(QObject* newParent = nullptr);
  ~qCjyxModuleManager() override;

  /// Print internal state using qDebug()
  virtual void printAdditionalInfo();

  /// Return a pointer to the current module factory manager
  Q_INVOKABLE qCjyxModuleFactoryManager * factoryManager()const;

  /// Return the list of all the loaded modules
  Q_INVOKABLE QStringList modulesNames()const;

  /// Return the loaded module identified by \a name
  Q_INVOKABLE qCjyxAbstractCoreModule* module(const QString& name)const;

signals:
  void moduleLoaded(const QString& module);
  void moduleAboutToBeUnloaded(const QString& module);

protected:
  QScopedPointer<qCjyxModuleManagerPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModuleManager);
  Q_DISABLE_COPY(qCjyxModuleManager);
};

#endif
