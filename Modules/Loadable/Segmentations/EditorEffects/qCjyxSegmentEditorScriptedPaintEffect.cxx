/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// SubjectHierarchy includes
#include "qCjyxSegmentEditorScriptedPaintEffect.h"

// Qt includes
#include <QDebug>
#include <QFileInfo>

// Cjyx includes
#include "qCjyxScriptedUtils_p.h"

// PythonQt includes
#include <PythonQt.h>
#include <PythonQtConversion.h>

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkPythonUtil.h>

//-----------------------------------------------------------------------------
class qCjyxSegmentEditorScriptedPaintEffectPrivate
{
public:
  typedef qCjyxSegmentEditorScriptedPaintEffectPrivate Self;
  qCjyxSegmentEditorScriptedPaintEffectPrivate();
  virtual ~qCjyxSegmentEditorScriptedPaintEffectPrivate();

  enum {
    IconMethod = 0,
    HelpTextMethod,
    CloneMethod,
    ActivateMethod,
    DeactivateMethod,
    SetupOptionsFrameMethod,
    CreateCursorMethod,
    SetMRMLDefaultsMethod,
    ReferenceGeometryChangedMethod,
    MasterVolumeNodeChangedMethod,
    LayoutChangedMethod,
    UpdateGUIFromMRMLMethod,
    UpdateMRMLFromGUIMethod,
    PaintApplyMethod,
    };

  mutable qCjyxPythonCppAPI PythonCppAPI;

  QString PythonSource;
};

//-----------------------------------------------------------------------------
// qCjyxSegmentEditorScriptedPaintEffectPrivate methods

//-----------------------------------------------------------------------------
qCjyxSegmentEditorScriptedPaintEffectPrivate::qCjyxSegmentEditorScriptedPaintEffectPrivate()
{
  this->PythonCppAPI.declareMethod(Self::IconMethod, "icon");
  this->PythonCppAPI.declareMethod(Self::HelpTextMethod, "helpText");
  this->PythonCppAPI.declareMethod(Self::CloneMethod, "clone");
  this->PythonCppAPI.declareMethod(Self::ActivateMethod, "activate");
  this->PythonCppAPI.declareMethod(Self::DeactivateMethod, "deactivate");
  this->PythonCppAPI.declareMethod(Self::SetupOptionsFrameMethod, "setupOptionsFrame");
  this->PythonCppAPI.declareMethod(Self::CreateCursorMethod, "createCursor");
  this->PythonCppAPI.declareMethod(Self::SetMRMLDefaultsMethod, "setMRMLDefaults");
  this->PythonCppAPI.declareMethod(Self::ReferenceGeometryChangedMethod, "referenceGeometryChanged");
  this->PythonCppAPI.declareMethod(Self::MasterVolumeNodeChangedMethod, "masterVolumeNodeChanged");
  this->PythonCppAPI.declareMethod(Self::LayoutChangedMethod, "layoutChanged");
  this->PythonCppAPI.declareMethod(Self::UpdateGUIFromMRMLMethod, "updateGUIFromMRML");
  this->PythonCppAPI.declareMethod(Self::UpdateMRMLFromGUIMethod, "updateMRMLFromGUI");
  this->PythonCppAPI.declareMethod(Self::PaintApplyMethod, "paintApply");
}

//-----------------------------------------------------------------------------
qCjyxSegmentEditorScriptedPaintEffectPrivate::~qCjyxSegmentEditorScriptedPaintEffectPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSegmentEditorScriptedPaintEffect methods

//-----------------------------------------------------------------------------
qCjyxSegmentEditorScriptedPaintEffect::qCjyxSegmentEditorScriptedPaintEffect(QObject *parent)
  : Superclass(parent)
  , d_ptr(new qCjyxSegmentEditorScriptedPaintEffectPrivate)
{
  this->m_Name = QString("UnnamedScriptedPaintEffect");
}

//-----------------------------------------------------------------------------
qCjyxSegmentEditorScriptedPaintEffect::~qCjyxSegmentEditorScriptedPaintEffect() = default;

//-----------------------------------------------------------------------------
QString qCjyxSegmentEditorScriptedPaintEffect::pythonSource()const
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  return d->PythonSource;
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentEditorScriptedPaintEffect::setPythonSource(const QString newPythonSource)
{
  Q_D(qCjyxSegmentEditorScriptedPaintEffect);

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

  // In case the effect is within the main module file
  QString className = moduleName;
  if (!moduleName.endsWith("Effect"))
    {
    className.append("Effect");
    }

  // Get a reference to the main module and global dictionary
  PyObject * main_module = PyImport_AddModule("__main__");
  PyObject * global_dict = PyModule_GetDict(main_module);

  // Get a reference (or create if needed) the <moduleName> python module
  PyObject * module = PyImport_AddModule(moduleName.toUtf8());

  // Get a reference to the python module class to instantiate
  PythonQtObjectPtr classToInstantiate;
  if (PyObject_HasAttrString(module, className.toUtf8()))
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
                    QString("qCjyxSegmentEditorScriptedPaintEffect::setPythonSource - "
                            "Failed to load segment editor scripted effect: "
                            "class %1 was not found in %2").arg(className).arg(newPythonSource).toUtf8());
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
        "cjyx", className, self))
    {
    qCritical() << "Failed to set" << ("cjyx." + className);
    }

  return true;
}

//-----------------------------------------------------------------------------
PyObject* qCjyxSegmentEditorScriptedPaintEffect::self() const
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  return d->PythonCppAPI.pythonSelf();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::setName(QString name)
{
  this->m_Name = name;
}

//-----------------------------------------------------------------------------
QIcon qCjyxSegmentEditorScriptedPaintEffect::icon()
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  PyObject* result = d->PythonCppAPI.callMethod(d->IconMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::icon();
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::Icon);
  if (resultVariant.isNull())
    {
    return this->Superclass::icon();
    }
  return resultVariant.value<QIcon>();
}

//-----------------------------------------------------------------------------
const QString qCjyxSegmentEditorScriptedPaintEffect::helpText()const
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  PyObject* result = d->PythonCppAPI.callMethod(d->HelpTextMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::helpText();
    }

  // Parse result
  if (!PyString_Check(result))
    {
    qWarning() << d->PythonSource << ": qCjyxSegmentEditorScriptedPaintEffect: Function 'helpText' is expected to return a string!";
    return this->Superclass::helpText();
    }

  const char* role = PyString_AsString(result);
  return QString::fromLocal8Bit(role);
}

//-----------------------------------------------------------------------------
qCjyxSegmentEditorAbstractEffect* qCjyxSegmentEditorScriptedPaintEffect::clone()
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  PyObject* result = d->PythonCppAPI.callMethod(d->CloneMethod);
  if (!result)
    {
    qCritical() << d->PythonSource << ": clone: Failed to call mandatory clone method! If it is implemented, please see python output for errors.";
    return nullptr;
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result);
  qCjyxSegmentEditorAbstractEffect* clonedEffect = qobject_cast<qCjyxSegmentEditorAbstractEffect*>(
    resultVariant.value<QObject*>() );
  if (!clonedEffect)
    {
    qCritical() << d->PythonSource << ": clone: Invalid cloned effect object returned from python!";
    return nullptr;
    }
  return clonedEffect;
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::activate()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::activate();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->ActivateMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::deactivate()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::deactivate();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->DeactivateMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::setupOptionsFrame()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::setupOptionsFrame();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->SetupOptionsFrameMethod);
}

//-----------------------------------------------------------------------------
QCursor qCjyxSegmentEditorScriptedPaintEffect::createCursor(qMRMLWidget* viewWidget)
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PythonQtConv::QVariantToPyObject(QVariant::fromValue<QObject*>((QObject*)viewWidget)));
  PyObject* result = d->PythonCppAPI.callMethod(d->CreateCursorMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::createCursor(viewWidget);
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::Cursor);
  return resultVariant.value<QCursor>();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::setMRMLDefaults()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::setMRMLDefaults();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->SetMRMLDefaultsMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::referenceGeometryChanged()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::referenceGeometryChanged();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->ReferenceGeometryChangedMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::masterVolumeNodeChanged()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::masterVolumeNodeChanged();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->MasterVolumeNodeChangedMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::layoutChanged()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::layoutChanged();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->LayoutChangedMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::updateGUIFromMRML()
{
  if (!this->active())
    {
    // updateGUIFromMRML is called when the effect is activated
    return;
    }

  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::updateGUIFromMRML();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->UpdateGUIFromMRMLMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::updateMRMLFromGUI()
{
  // Base class implementation needs to be called before the effect-specific one
  this->Superclass::updateMRMLFromGUI();

  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  d->PythonCppAPI.callMethod(d->UpdateMRMLFromGUIMethod);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentEditorScriptedPaintEffect::paintApply(qMRMLWidget* viewWidget)
{
  Q_D(const qCjyxSegmentEditorScriptedPaintEffect);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PythonQtConv::QVariantToPyObject(QVariant::fromValue<QObject*>((QObject*)viewWidget)));
  PyObject* result = d->PythonCppAPI.callMethod(d->PaintApplyMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    this->Superclass::paintApply(viewWidget);
    }
}
