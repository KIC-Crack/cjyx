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
//  - Cjyx_QTLOADABLEMODULES_LIB_DIR
//  - Cjyx_USE_PYTHONQT
#include "vtkCjyxConfigure.h"

// Cjyx includes
#include "qCjyxLoadableModuleFactory.h"
#include "qCjyxCoreApplication.h"
#include "qCjyxUtils.h"

// VTKSYS includes
#include <vtksys/SystemTools.hxx>

//-----------------------------------------------------------------------------
qCjyxLoadableModuleFactoryItem::qCjyxLoadableModuleFactoryItem() = default;

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxLoadableModuleFactoryItem::instanciator()
{
  qCjyxAbstractCoreModule * module =
      ctkFactoryPluginItem<qCjyxAbstractCoreModule>::instanciator();
  module->setPath(this->path());

  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  module->setInstalled(qCjyxUtils::isPluginInstalled(this->path(), app->cjyxHome()));
  module->setBuiltIn(qCjyxUtils::isPluginBuiltIn(this->path(), app->cjyxHome(), app->revision()));

  return module;
}

//-----------------------------------------------------------------------------
class qCjyxLoadableModuleFactoryPrivate
{
public:
  /// Return a list of module paths
  QStringList modulePaths() const;
};

//-----------------------------------------------------------------------------
// qCjyxLoadableModuleFactoryPrivate Methods

//-----------------------------------------------------------------------------
QStringList qCjyxLoadableModuleFactoryPrivate::modulePaths() const
{
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  Q_ASSERT(app);
  Q_ASSERT(!app->cjyxHome().isEmpty());

  QStringList defaultQTModulePaths;

#ifdef Cjyx_BUILD_QTLOADABLEMODULES
  bool appendDefaultQTModulePaths = true;
#else
  bool appendDefaultQTModulePaths = app->isInstalled();
#endif
  if (appendDefaultQTModulePaths)
    {
    defaultQTModulePaths << app->cjyxHome() + "/" + Cjyx_QTLOADABLEMODULES_LIB_DIR;
    if (!app->intDir().isEmpty())
      {
      // On Win32, *both* paths have to be there, since scripts are installed
      // in the install location, and exec/libs are *automatically* installed
      // in intDir.
      defaultQTModulePaths << app->cjyxHome() + "/" + Cjyx_QTLOADABLEMODULES_LIB_DIR + "/" + app->intDir();
      }
    }

  QSettings * settings = app->revisionUserSettings();
  QStringList additionalModulePaths = app->toCjyxHomeAbsolutePaths(settings->value("Modules/AdditionalPaths").toStringList());
  QStringList qtModulePaths =  additionalModulePaths + defaultQTModulePaths;

  //qDebug() << "qtModulePaths:" << qtModulePaths;

  return qtModulePaths;
}

//-----------------------------------------------------------------------------
// qCjyxLoadableModuleFactory Methods

//-----------------------------------------------------------------------------
qCjyxLoadableModuleFactory::qCjyxLoadableModuleFactory()
  : d_ptr(new qCjyxLoadableModuleFactoryPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxLoadableModuleFactory::~qCjyxLoadableModuleFactory() = default;

//-----------------------------------------------------------------------------
void qCjyxLoadableModuleFactory::registerItems()
{
  Q_D(qCjyxLoadableModuleFactory);

  this->registerAllFileItems(d->modulePaths());
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModuleFactory::fileNameToKey(const QString& fileName)const
{
  return qCjyxLoadableModuleFactory::extractModuleName(fileName);
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModuleFactory::extractModuleName(const QString& libraryName)
{
  return qCjyxUtils::extractModuleNameFromLibraryName(libraryName);
}

//-----------------------------------------------------------------------------
qCjyxLoadableModuleFactoryItem* qCjyxLoadableModuleFactory::createFactoryFileBasedItem()
{
  return new qCjyxLoadableModuleFactoryItem();
}

//-----------------------------------------------------------------------------
bool qCjyxLoadableModuleFactory::isValidFile(const QFileInfo& file)const
{
  if (!Superclass::isValidFile(file))
    {
    return false;
    }
  return qCjyxUtils::isLoadableModule(file.absoluteFilePath());
}
