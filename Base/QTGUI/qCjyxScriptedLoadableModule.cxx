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
#include <QFileInfo>

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxIOManager.h"
#include "qCjyxScriptedLoadableModule.h"
#include "qCjyxScriptedLoadableModuleWidget.h"
#include "qCjyxScriptedFileDialog.h"
#include "qCjyxScriptedFileReader.h"
#include "qCjyxScriptedFileWriter.h"
#include "qCjyxScriptedUtils_p.h"
#include "vtkCjyxScriptedLoadableModuleLogic.h"

//-----------------------------------------------------------------------------
class qCjyxScriptedLoadableModulePrivate
{
public:
  typedef qCjyxScriptedLoadableModulePrivate Self;
  qCjyxScriptedLoadableModulePrivate();
  virtual ~qCjyxScriptedLoadableModulePrivate();

  QString Title;
  QStringList Categories;
  QStringList Contributors;
  QStringList AssociatedNodeTypes;
  QStringList Dependencies;
  QString HelpText;
  QString AcknowledgementText;
  QIcon   Icon;
  bool   Hidden;
  QVariantMap   Extensions;
  int Index;

  enum {
    SetupMethod = 0
    };

  mutable qCjyxPythonCppAPI PythonCppAPI;

  QString    PythonSource;
};

//-----------------------------------------------------------------------------
// qCjyxScriptedLoadableModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModulePrivate::qCjyxScriptedLoadableModulePrivate()
{
  this->Hidden = false;
  this->Index = -1;

  this->PythonCppAPI.declareMethod(Self::SetupMethod, "setup");
}

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModulePrivate::~qCjyxScriptedLoadableModulePrivate() = default;


//-----------------------------------------------------------------------------
// qCjyxScriptedLoadableModule methods

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModule::qCjyxScriptedLoadableModule(QObject* _parentObject)
  : Superclass(_parentObject)
  , d_ptr(new qCjyxScriptedLoadableModulePrivate)
{
  Q_D(qCjyxScriptedLoadableModule);
  d->Icon = this->Superclass::icon();
}

//-----------------------------------------------------------------------------
qCjyxScriptedLoadableModule::~qCjyxScriptedLoadableModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxScriptedLoadableModule::pythonSource()const
{
  Q_D(const qCjyxScriptedLoadableModule);
  return d->PythonSource;
}

//-----------------------------------------------------------------------------
bool qCjyxScriptedLoadableModule::setPythonSource(const QString& newPythonSource)
{
  Q_D(qCjyxScriptedLoadableModule);

  if (!Py_IsInitialized())
    {
    return false;
    }

  if (!newPythonSource.endsWith(".py") && !newPythonSource.endsWith(".pyc"))
    {
    return false;
    }

  // Extract moduleName from the provided filename
  QString moduleName = QFileInfo(newPythonSource).baseName();
  this->setName(moduleName);
  QString className = moduleName;

  // Get a reference to the main module and global dictionary
  PyObject * main_module = PyImport_AddModule("__main__");
  PyObject * global_dict = PyModule_GetDict(main_module);

  // Get a reference (or create if needed) the <moduleName> python module
  PyObject * module = PyImport_AddModule(moduleName.toUtf8());

  // Get a reference to the python module class to instantiate
  PythonQtObjectPtr classToInstantiate;
  if (module && PyObject_HasAttrString(module, className.toUtf8()))
    {
    classToInstantiate.setNewRef(PyObject_GetAttrString(module, className.toUtf8()));
    }
  if (!classToInstantiate)
    {
    PythonQtObjectPtr local_dict;
    local_dict.setNewRef(PyDict_New());
    if (!qCjyxScriptedUtils::loadSourceAsModule(moduleName, newPythonSource, global_dict, local_dict))
      {
      return false;
      }
    if (PyObject_HasAttrString(module, className.toUtf8()))
      {
      classToInstantiate.setNewRef(PyObject_GetAttrString(module, className.toUtf8()));
      }
    }

  if (!classToInstantiate)
    {
    PythonQt::self()->handleError();
    PyErr_SetString(PyExc_RuntimeError,
                    QString("qCjyxScriptedLoadableModule::setPythonSource - "
                            "Failed to load scripted loadable module: "
                            "class %1 was not found in file %2").arg(className).arg(newPythonSource).toLatin1());
    PythonQt::self()->handleError();
    return false;
    }

  d->PythonCppAPI.setObjectName(className);

  PyObject* self = d->PythonCppAPI.instantiateClass(this, className, classToInstantiate);
  if (!self)
    {
    return false;
    }

  d->PythonSource = newPythonSource;

  if (!qCjyxScriptedUtils::setModuleAttribute(
        "cjyx.modules", moduleName + "Instance", self))
    {
    qCritical() << "Failed to set" << ("cjyx.modules." + moduleName + "Instance");
    }

  // Check if there is module widget class
  QString widgetClassName = className + "Widget";
  if (!PyObject_HasAttrString(module, widgetClassName.toLatin1()))
    {
    this->setWidgetRepresentationCreationEnabled(false);
    }

  return true;
}

//-----------------------------------------------------------------------------
void qCjyxScriptedLoadableModule::setup()
{
  Q_D(qCjyxScriptedLoadableModule);

  qCjyxCoreApplication * app = qCjyxCoreApplication::application();
  if (app)
    {
    // Set to /path/to/lib/Cjyx-X.Y/qt-scripted-modules
    QString modulePath = QFileInfo(this->path()).absolutePath();
    // Set to /path/to/lib/Cjyx-X.Y
    modulePath = QFileInfo(modulePath).absolutePath();
    // Set to /path/to/lib/Cjyx-X.Y/qt-loadable-modules
    modulePath = modulePath + "/" Cjyx_QTLOADABLEMODULES_SUBDIR;

    bool isEmbedded = app->isEmbeddedModule(this->path());
    if (!isEmbedded)
      {
      if (!qCjyxLoadableModule::importModulePythonExtensions(
            app->corePythonManager(), app->intDir(), modulePath, isEmbedded))
        {
        qWarning() << "qCjyxLoadableModule::setup - Failed to import module" << this->name() << "python extensions";
        }
      }
    }

  this->registerFileDialog();
  this->registerIO();
  d->PythonCppAPI.callMethod(Pimpl::SetupMethod);
}

//-----------------------------------------------------------------------------
void qCjyxScriptedLoadableModule::registerFileDialog()
{
  Q_D(qCjyxScriptedLoadableModule);
  QScopedPointer<qCjyxScriptedFileDialog> fileDialog(new qCjyxScriptedFileDialog(this));
  bool ret = fileDialog->setPythonSource(d->PythonSource);
  if (!ret)
    {
    return;
    }
  qCjyxApplication::application()->ioManager()
    ->registerDialog(fileDialog.take());
}

//-----------------------------------------------------------------------------
void qCjyxScriptedLoadableModule::registerIO()
{
  Q_D(qCjyxScriptedLoadableModule);
  QScopedPointer<qCjyxScriptedFileWriter> fileWriter(new qCjyxScriptedFileWriter(this));
  bool ret = fileWriter->setPythonSource(d->PythonSource);
  if (ret)
    {
    qCjyxApplication::application()->ioManager()->registerIO(fileWriter.take());
    }
  QScopedPointer<qCjyxScriptedFileReader> fileReader(new qCjyxScriptedFileReader(this));
  ret = fileReader->setPythonSource(d->PythonSource);
  if (ret)
    {
    qCjyxApplication::application()->ioManager()->registerIO(fileReader.take());
    }
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxScriptedLoadableModule::createWidgetRepresentation()
{
  Q_D(qCjyxScriptedLoadableModule);

  if (!this->isWidgetRepresentationCreationEnabled())
    {
    return nullptr;
    }

  QScopedPointer<qCjyxScriptedLoadableModuleWidget> widget(new qCjyxScriptedLoadableModuleWidget);
  bool ret = widget->setPythonSource(d->PythonSource);
  if (!ret)
    {
    return nullptr;
    }

  return widget.take();
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxScriptedLoadableModule::createLogic()
{
//  Q_D(qCjyxScriptedLoadableModule);

  vtkCjyxScriptedLoadableModuleLogic* logic = vtkCjyxScriptedLoadableModuleLogic::New();

//  bool ret = logic->SetPythonSource(d->PythonSource.toStdString());
//  if (!ret)
//    {
//    logic->Delete();
//    return 0;
//    }

  return logic;
}

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QString&, setTitle, Title)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QString, title, Title)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QStringList&, setCategories, Categories)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QStringList, categories, Categories)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QStringList&, setContributors, Contributors)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QStringList, contributors, Contributors)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QStringList&, setAssociatedNodeTypes, AssociatedNodeTypes)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QStringList, associatedNodeTypes, AssociatedNodeTypes)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QString&, setHelpText, HelpText)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QString, helpText, HelpText)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QString&, setAcknowledgementText, AcknowledgementText)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QString, acknowledgementText, AcknowledgementText)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QVariantMap&, setExtensions, Extensions)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QVariantMap, extensions, Extensions)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QIcon&, setIcon, Icon)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QIcon, icon, Icon)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, bool, setHidden, Hidden)
CTK_GET_CPP(qCjyxScriptedLoadableModule, bool, isHidden, Hidden)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const QStringList&, setDependencies, Dependencies)
CTK_GET_CPP(qCjyxScriptedLoadableModule, QStringList, dependencies, Dependencies)

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxScriptedLoadableModule, const int, setIndex, Index)
CTK_GET_CPP(qCjyxScriptedLoadableModule, int, index, Index)
