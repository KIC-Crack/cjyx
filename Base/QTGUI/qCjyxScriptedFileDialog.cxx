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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDropEvent>
#include <QFileInfo>

// PythonQt includes
#include <PythonQt.h>

// Cjyx includes
#include "qCjyxScriptedFileDialog.h"
#include "qCjyxScriptedUtils_p.h"

//-----------------------------------------------------------------------------
class qCjyxScriptedFileDialogPrivate
{
public:
  typedef qCjyxScriptedFileDialogPrivate Self;
  qCjyxScriptedFileDialogPrivate();
  virtual ~qCjyxScriptedFileDialogPrivate();

  qCjyxIO::IOFileType FileType;
  QString Description;
  qCjyxFileDialog::IOAction Action;
  qCjyxIO::IOProperties Properties;
  mutable const QMimeData* MimeData;
  mutable bool MimeDataAccepted;
  QDropEvent* DropEvent;

  enum {
    ExecMethod = 0,
    IsMimeDataAcceptedMethod,
    DropEventMethod,
    };

  mutable qCjyxPythonCppAPI PythonCppAPI;

  QString    PythonSource;
  QString    PythonClassName;
};

//-----------------------------------------------------------------------------
// qCjyxScriptedFileDialogPrivate methods

//-----------------------------------------------------------------------------
qCjyxScriptedFileDialogPrivate::qCjyxScriptedFileDialogPrivate()
{
  this->Action = qCjyxFileDialog::Read;
  this->MimeData = nullptr;
  this->MimeDataAccepted = false;
  this->DropEvent = nullptr;

  this->PythonCppAPI.declareMethod(Self::ExecMethod, "execDialog");
  this->PythonCppAPI.declareMethod(Self::IsMimeDataAcceptedMethod, "isMimeDataAccepted");
  this->PythonCppAPI.declareMethod(Self::DropEventMethod, "dropEvent");
}

//-----------------------------------------------------------------------------
qCjyxScriptedFileDialogPrivate::~qCjyxScriptedFileDialogPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxScriptedFileDialog methods

//-----------------------------------------------------------------------------
qCjyxScriptedFileDialog::qCjyxScriptedFileDialog(QObject* parent)
  : Superclass(parent)
  , d_ptr(new qCjyxScriptedFileDialogPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxScriptedFileDialog::~qCjyxScriptedFileDialog() = default;

//-----------------------------------------------------------------------------
QString qCjyxScriptedFileDialog::pythonSource()const
{
  Q_D(const qCjyxScriptedFileDialog);
  return d->PythonSource;
}

//-----------------------------------------------------------------------------
bool qCjyxScriptedFileDialog::setPythonSource(const QString& newPythonSource, const QString& _className, bool missingClassIsExpected)
{
  Q_D(qCjyxScriptedFileDialog);

  if (!Py_IsInitialized())
    {
    return false;
    }

  if(!newPythonSource.endsWith(".py") && !newPythonSource.endsWith(".pyc"))
    {
    return false;
    }

  // Extract moduleName from the provided filename
  QString moduleName = QFileInfo(newPythonSource).baseName();

  QString className = _className;
  if (className.isEmpty())
    {
    className = moduleName;
    if (!moduleName.endsWith("FileDialog"))
      {
      className.append("FileDialog");
      }
    }

  d->PythonCppAPI.setObjectName(className);
  d->PythonClassName = className;

  // Get a reference (or create if needed) the <moduleName> python module
  PyObject * module = PyImport_AddModule(moduleName.toUtf8());

  // Get a reference to the python module class to instantiate
  PythonQtObjectPtr classToInstantiate;
  if (PyObject_HasAttrString(module, className.toUtf8()))
    {
    classToInstantiate.setNewRef(PyObject_GetAttrString(module, className.toUtf8()));
    }
  else if (missingClassIsExpected)
    {
    // Class is not defined for this object, but this is expected, not an error
    return false;
    }

  if (!classToInstantiate)
    {
    // HACK The file dialog class definition is expected to be available after executing the
    //      associated module class, trying to load the file a second time will (1) cause all the
    //      classes within the file to be associated with module corresponding to __name__
    //      variable and (2) raise a 'TypeError' exception.
    //
    //      For example, if a file name 'Bar.py' having a class Bar, is loaded by
    //      qCjyxScriptedLoadableModule, the associated class name will be: Bar.Bar
    //
    //      Then, if the qCjyxScriptedFileDialog also attempt to load the Bar.py file,
    //      the class name associated with the same class will then be: BarDialog.Bar or
    //      __main__.Bar if __name__ is not explicitly overridden
    //
    //      In other word, if BarPlugin.py contain code like:
    //
    //        from SomeLig import MyBasePlugin
    //        class BarPlugin(MyBasePlugin):
    //           def __init__(self):
    //           print("################")
    //           print(BarPlugin)
    //           print(self.__class__)
    //           print(isinstance(self, BarPlugin))
    //           print("################")
    //           super(BarPlugin,self).__init__()
    //
    //      the following will be printed:
    //
    //        ################
    //        <class '__main__.BarPlugin'>  // or <class 'BarPluginFileDialog.BarPlugin'>
    //        <class 'BarPlugin.BarPlugin'>
    //        False
    //        ################
    //
    //      and will raise the following exception:
    //         TypeError: super(type, obj): obj must be an instance or subtype of type
    //
    //      More details about the exception :
    //        https://thingspython.wordpress.com/2010/09/27/another-super-wrinkle-raising-typeerror/
    //
    return false;
    }

  PyObject* self = d->PythonCppAPI.instantiateClass(this, className, classToInstantiate);
  if (!self)
    {
    return false;
    }

  d->PythonSource = newPythonSource;

  return true;
}

//-----------------------------------------------------------------------------
PyObject* qCjyxScriptedFileDialog::self() const
{
  Q_D(const qCjyxScriptedFileDialog);
  return d->PythonCppAPI.pythonSelf();
}

//-----------------------------------------------------------------------------
bool qCjyxScriptedFileDialog::exec(const qCjyxIO::IOProperties& ioProperties)
{
  Q_D(qCjyxScriptedFileDialog);
  d->Properties = ioProperties;
  PyObject * result = d->PythonCppAPI.callMethod(d->ExecMethod);
  if (!result)
    {
    return false;
    }
  if (!PyBool_Check(result))
    {
    qWarning() << d->PythonSource
               << " - In" << d->PythonClassName << "class, function 'execDialog' "
               << "is expected to return a boolean";
    return false;
    }
  return result == Py_True;
}

//-----------------------------------------------------------------------------
bool qCjyxScriptedFileDialog::isMimeDataAccepted(const QMimeData* mimeData)const
{
  Q_D(const qCjyxScriptedFileDialog);
  d->MimeData = mimeData;
  d->MimeDataAccepted = false;
  PyObject * result = d->PythonCppAPI.callMethod(d->IsMimeDataAcceptedMethod);
  if (!result)
    {
    return false;
    }
  return d->MimeDataAccepted;
}

//-----------------------------------------------------------------------------
void qCjyxScriptedFileDialog::dropEvent(QDropEvent* event)
{
  Q_D(qCjyxScriptedFileDialog);
  d->DropEvent =  event;
  d->MimeData = event->mimeData();
  d->PythonCppAPI.callMethod(d->DropEventMethod);
}

//-----------------------------------------------------------------------------
const qCjyxIO::IOProperties& qCjyxScriptedFileDialog::ioProperties()const
{
  Q_D(const qCjyxScriptedFileDialog);
  return d->Properties;
}

//-----------------------------------------------------------------------------
const QMimeData* qCjyxScriptedFileDialog::mimeData()const
{
  Q_D(const qCjyxScriptedFileDialog);
  return d->MimeData;
}

//-----------------------------------------------------------------------------
QDropEvent* qCjyxScriptedFileDialog::dropEvent()const
{
  Q_D(const qCjyxScriptedFileDialog);
  return d->DropEvent;
}

//-----------------------------------------------------------------------------
void qCjyxScriptedFileDialog::acceptMimeData(bool accept)
{
  Q_D(qCjyxScriptedFileDialog);
  d->MimeDataAccepted = accept;
}