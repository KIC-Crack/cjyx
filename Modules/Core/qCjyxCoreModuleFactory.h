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

#ifndef __qCjyxCoreModuleFactory_h
#define __qCjyxCoreModuleFactory_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkAbstractQObjectFactory.h>

// Cjyx includes
#include "qCjyxAbstractModule.h"
#include "qCjyxModulesCoreExport.h"

class qCjyxCoreModuleFactoryPrivate;

class Q_CJYX_MODULES_CORE_EXPORT qCjyxCoreModuleFactory :
  public ctkAbstractQObjectFactory<qCjyxAbstractCoreModule>
{
public:

  typedef ctkAbstractQObjectFactory<qCjyxAbstractCoreModule> Superclass;
  qCjyxCoreModuleFactory();
  ~qCjyxCoreModuleFactory() override;

  ///
  void registerItems() override;

  ///
  QString objectNameToKey(const QString& objectName) override;

  /// Extract module name given a core module \a className
  /// For example:
  ///  qCjyxCamerasModule -> cameras
  ///  qCjyxTransformsModule -> transforms
  static QString extractModuleName(const QString& className);

protected:
  QScopedPointer<qCjyxCoreModuleFactoryPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCoreModuleFactory);
  Q_DISABLE_COPY(qCjyxCoreModuleFactory);
};

#endif
