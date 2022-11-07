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


#ifndef __qCjyxCLILoadableModuleFactory_h
#define __qCjyxCLILoadableModuleFactory_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkAbstractLibraryFactory.h>

// Cjyx includes
#include "qCjyxAbstractModule.h"
#include "qCjyxBaseQTCLIExport.h"

class ModuleDescription;
class ModuleLogo;
class qCjyxCLIModule;

//-----------------------------------------------------------------------------
class qCjyxCLILoadableModuleFactoryItem
  : public ctkFactoryLibraryItem<qCjyxAbstractCoreModule>
{
public:
  typedef ctkFactoryLibraryItem<qCjyxAbstractCoreModule> Superclass;
  qCjyxCLILoadableModuleFactoryItem(const QString& newTempDirectory);
  bool load() override;

  static void loadLibraryAndResolveSymbols(
      void* libraryLoader,  ModuleDescription& desc);

protected:
  /// Return path of the expected XML file.
  QString xmlModuleDescriptionFilePath()const;

  qCjyxAbstractCoreModule* instanciator() override;
  QString resolveXMLModuleDescriptionSymbol();
  bool resolveSymbols(ModuleDescription& desc);
  static bool updateLogo(qCjyxCLILoadableModuleFactoryItem* item, ModuleLogo& logo);
private:
  QString TempDirectory;
};

class qCjyxCLILoadableModuleFactoryPrivate;

//-----------------------------------------------------------------------------
class Q_CJYX_BASE_QTCLI_EXPORT qCjyxCLILoadableModuleFactory :
  public ctkAbstractLibraryFactory<qCjyxAbstractCoreModule>
{
public:
  typedef ctkAbstractLibraryFactory<qCjyxAbstractCoreModule> Superclass;
  qCjyxCLILoadableModuleFactory();
  ~qCjyxCLILoadableModuleFactory() override;

  /// Reimplemented to scan the directory of the command line modules
  void registerItems() override;

  /// Extract module name given \a libraryName
  /// For example:
  ///  libThresholdLib.so -> threshold
  ///  libThresholdLib.{dylib, bundle, so} -> threshold
  ///  ThresholdLib.dll -> threshold
  /// \sa qCjyxUtils::extractModuleNameFromLibraryName
  QString fileNameToKey(const QString& fileName)const override;

  void setTempDirectory(const QString& newTempDirectory);

protected:
  ctkAbstractFactoryItem<qCjyxAbstractCoreModule>*
    createFactoryFileBasedItem() override;

  bool isValidFile(const QFileInfo& file)const override;

protected:

  QScopedPointer<qCjyxCLILoadableModuleFactoryPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCLILoadableModuleFactory);
  Q_DISABLE_COPY(qCjyxCLILoadableModuleFactory);
};

#endif
