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

#ifndef __qCjyxCLIExecutableModuleFactory_h
#define __qCjyxCLIExecutableModuleFactory_h

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxBaseQTCLIExport.h"
class qCjyxCLIModule;

// CTK includes
#include <ctkPimpl.h>
#include <ctkAbstractPluginFactory.h>

//-----------------------------------------------------------------------------
class qCjyxCLIExecutableModuleFactoryItem
  : public ctkAbstractFactoryFileBasedItem<qCjyxAbstractCoreModule>
{
public:
  qCjyxCLIExecutableModuleFactoryItem(const QString& newTempDirectory);
  bool load() override;
  void uninstantiate() override;
protected:
  /// Return path of the expected XML file.
  QString xmlModuleDescriptionFilePath();

  qCjyxAbstractCoreModule* instanciator() override;
  QString runCLIWithXmlArgument();
private:
  QString TempDirectory;
  qCjyxCLIModule* CLIModule;
};

class qCjyxCLIExecutableModuleFactoryPrivate;

//-----------------------------------------------------------------------------
class Q_CJYX_BASE_QTCLI_EXPORT qCjyxCLIExecutableModuleFactory :
  public ctkAbstractFileBasedFactory<qCjyxAbstractCoreModule>
{
public:
  typedef ctkAbstractFileBasedFactory<qCjyxAbstractCoreModule> Superclass;
  qCjyxCLIExecutableModuleFactory();
  qCjyxCLIExecutableModuleFactory(const QString& tempDir);
  ~qCjyxCLIExecutableModuleFactory() override;

  void registerItems() override;

  /// Extract module name given \a executableName
  /// For example:
  ///  Threshold.exe -> threshold
  ///  Threshold -> threshold
  QString fileNameToKey(const QString& fileName)const override;

  void setTempDirectory(const QString& newTempDirectory);

protected:
  bool isValidFile(const QFileInfo& file)const override;

  ctkAbstractFactoryItem<qCjyxAbstractCoreModule>*
    createFactoryFileBasedItem() override;

protected:

  QScopedPointer<qCjyxCLIExecutableModuleFactoryPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCLIExecutableModuleFactory);
  Q_DISABLE_COPY(qCjyxCLIExecutableModuleFactory);
};

#endif
