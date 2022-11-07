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
#include <QBitArray>
#include <QSettings>

// CTK includes
#include <ctkVTKPythonQtWrapperFactory.h>

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxUtils.h"
#include "qCjyxCorePythonManager.h"
#include "qCjyxScriptedUtils_p.h"
#include "vtkCjyxConfigure.h"

// VTK includes
#include <vtkPythonUtil.h>
#include <vtkVersion.h>

//-----------------------------------------------------------------------------
qCjyxCorePythonManager::qCjyxCorePythonManager(QObject* _parent)
  : Superclass(_parent)
{
  this->Factory = nullptr;

  // If it applies, disable import of user site packages
  QString noUserSite = qgetenv("PYTHONNOUSERSITE");
  Py_NoUserSiteDirectory = noUserSite.toInt();

  // Import site module to ensure the 'site-packages' directory
  // is added to the python path. (see site.addsitepackages function).
  int flags = this->initializationFlags();
  flags &= ~(PythonQt::IgnoreSiteModule); // Clear bit
  this->setInitializationFlags(flags);
}

//-----------------------------------------------------------------------------
qCjyxCorePythonManager::~qCjyxCorePythonManager()
{
  if (this->Factory)
    {
    delete this->Factory;
    this->Factory = nullptr;
    }
}

//-----------------------------------------------------------------------------
QStringList qCjyxCorePythonManager::pythonPaths()
{
  return Superclass::pythonPaths();
}

//-----------------------------------------------------------------------------
void qCjyxCorePythonManager::preInitialization()
{
  Superclass::preInitialization();
  this->Factory = new ctkVTKPythonQtWrapperFactory;
  this->addWrapperFactory(this->Factory);
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  if (app)
    {
    // Add object to python interpreter context
    this->addObjectToPythonMain("_qCjyxCoreApplicationInstance", app);
    }
}

//-----------------------------------------------------------------------------
void qCjyxCorePythonManager::addVTKObjectToPythonMain(const QString& name, vtkObject * object)
{
  // Split name using '.'
#if (QT_VERSION >= QT_VERSION_CHECK(5, 14, 0))
  QStringList moduleNameList = name.split('.', Qt::SkipEmptyParts);
#else
  QStringList moduleNameList = name.split('.', QString::SkipEmptyParts);
#endif

  // Remove the last part
  QString attributeName = moduleNameList.takeLast();

  bool success = qCjyxScriptedUtils::setModuleAttribute(
        moduleNameList.join("."),
        attributeName,
        vtkPythonUtil::GetObjectFromPointer(object));
  if (!success)
    {
    qCritical() << "qCjyxCorePythonManager::addVTKObjectToPythonMain - "
                   "Failed to add VTK object:" << name;
    }
}

//-----------------------------------------------------------------------------
void qCjyxCorePythonManager::appendPythonPath(const QString& path)
{
  // TODO Make sure PYTHONPATH is updated
  this->executeString(QString(
    "import sys, os\n"
    "___path = os.path.abspath(%1)\n"
    "if ___path not in sys.path:\n"
    "  sys.path.append(___path)\n"
    "del sys, os"
    ).arg(qCjyxCorePythonManager::toPythonStringLiteral(path)));
}

//-----------------------------------------------------------------------------
void qCjyxCorePythonManager::appendPythonPaths(const QStringList& paths)
{
  foreach(const QString& path, paths)
    {
    this->appendPythonPath(path);
    }
}

//-----------------------------------------------------------------------------
QString qCjyxCorePythonManager::toPythonStringLiteral(QString path)
{
  return ctkAbstractPythonManager::toPythonStringLiteral(path);
}
