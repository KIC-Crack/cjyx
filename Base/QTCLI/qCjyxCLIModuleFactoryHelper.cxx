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

// QtCLI includes
#include "qCjyxCLIModuleFactoryHelper.h"

// Cjyx includes
#include "qCjyxCoreApplication.h" // For: Cjyx_CLIMODULES_LIB_DIR
#include "qCjyxUtils.h"

//-----------------------------------------------------------------------------
const QStringList qCjyxCLIModuleFactoryHelper::modulePaths()
{
  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  if (!app)
    {
    qCritical("qCjyxCLIModuleFactoryHelper::modulePaths failed: qCjyxCoreApplication is not instantiated");
    return QStringList();
    }

  // cjyxHome shouldn't be empty
  if (!app->cjyxHome().isEmpty())
    {
    qCritical("qCjyxCLIModuleFactoryHelper::modulePaths failed: app->cjyxHome() is empty");
    return QStringList();
    }

  QStringList defaultCmdLineModulePaths;
  if (QFile::exists(app->cjyxHome() + "/" + Cjyx_CLIMODULES_LIB_DIR))
    {
    defaultCmdLineModulePaths << app->cjyxHome() + "/" + Cjyx_CLIMODULES_LIB_DIR;
    if (!app->intDir().isEmpty())
       {
       // On Win32, *both* paths have to be there, since scripts are installed
       // in the install location, and exec/libs are *automatically* installed
       // in intDir.
       defaultCmdLineModulePaths << app->cjyxHome() + "/" + Cjyx_CLIMODULES_LIB_DIR + "/" + app->intDir();
       }
    }

  QSettings * settings = app->revisionUserSettings();
  QStringList additionalModulePaths = app->toCjyxHomeAbsolutePaths(settings->value("Modules/AdditionalPaths").toStringList());
  QStringList cmdLineModulePaths = additionalModulePaths + defaultCmdLineModulePaths;
  foreach(const QString& path, cmdLineModulePaths)
    {
    app->addLibraryPath(path);
    }
  return cmdLineModulePaths;
}

//-----------------------------------------------------------------------------
bool qCjyxCLIModuleFactoryHelper::isInstalled(const QString& path)
{
  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  return app ? qCjyxUtils::isPluginInstalled(path, app->cjyxHome()) : false;
}

//-----------------------------------------------------------------------------
bool qCjyxCLIModuleFactoryHelper::isBuiltIn(const QString& path)
{
  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  return app ? qCjyxUtils::isPluginBuiltIn(path, app->cjyxHome(), app->revision()) : true;
}
