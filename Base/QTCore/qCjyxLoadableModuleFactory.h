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

#ifndef __qCjyxLoadableModuleFactory_h
#define __qCjyxLoadableModuleFactory_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkAbstractPluginFactory.h>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"

#include "qCjyxBaseQTCoreExport.h"

class qCjyxLoadableModuleFactoryPrivate;

//-----------------------------------------------------------------------------
class qCjyxLoadableModuleFactoryItem
  : public ctkFactoryPluginItem<qCjyxAbstractCoreModule>
{
public:
  qCjyxLoadableModuleFactoryItem();
protected:
  qCjyxAbstractCoreModule* instanciator() override;
};

//-----------------------------------------------------------------------------
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxLoadableModuleFactory :
  public ctkAbstractPluginFactory<qCjyxAbstractCoreModule>
{
public:

  typedef ctkAbstractPluginFactory<qCjyxAbstractCoreModule> Superclass;
  qCjyxLoadableModuleFactory();
  ~qCjyxLoadableModuleFactory() override;

  ///
  void registerItems() override;

  ///
  QString fileNameToKey(const QString& fileName)const override;

  /// Extract module name given \a libraryName
  /// \sa qCjyxUtils::extractModuleNameFromLibraryName
  static QString extractModuleName(const QString& libraryName);

protected:
  qCjyxLoadableModuleFactoryItem* createFactoryFileBasedItem() override;

  bool isValidFile(const QFileInfo& file)const override;

protected:
  QScopedPointer<qCjyxLoadableModuleFactoryPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxLoadableModuleFactory);
  Q_DISABLE_COPY(qCjyxLoadableModuleFactory);
};

#endif
