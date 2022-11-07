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

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// SubjectHierarchy includes
#include "qCjyxSubjectHierarchyScriptedPlugin.h"
#include "qCjyxSubjectHierarchyPluginHandler.h"

// Qt includes
#include <QDebug>
#include <QFileInfo>
#include <QAction>

// Cjyxncludes
#include "qCjyxScriptedUtils_p.h"

// PythonQt includes
#include <PythonQt.h>
#include <PythonQtConversion.h>

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkPythonUtil.h>

//-----------------------------------------------------------------------------
class qCjyxSubjectHierarchyScriptedPluginPrivate
{
public:
  typedef qCjyxSubjectHierarchyScriptedPluginPrivate Self;
  qCjyxSubjectHierarchyScriptedPluginPrivate();
  virtual ~qCjyxSubjectHierarchyScriptedPluginPrivate();

  enum {
    CanOwnSubjectHierarchyItemMethod = 0,
    RoleForPluginMethod,
    HelpTextMethod,
    IconMethod,
    VisibilityIconMethod,
    EditPropertiesMethod,
    DisplayedItemNameMethod,
    TooltipMethod,
    SetDisplayVisibilityMethod,
    GetDisplayVisibilityMethod,
    ItemContextMenuActionsMethod,
    ViewContextMenuActionsMethod,
    SceneContextMenuActionsMethod,
    ShowContextMenuActionsForItemMethod,
    ShowViewContextMenuActionsForItemMethod,
    CanAddNodeToSubjectHierarchyMethod,
    CanReparentItemInsideSubjectHierarchyMethod,
    ReparentItemInsideSubjectHierarchyMethod
    };

  mutable qCjyxPythonCppAPI PythonCppAPI;

  QString PythonSource;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyScriptedPluginPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyScriptedPluginPrivate::qCjyxSubjectHierarchyScriptedPluginPrivate()
{
  // Role-related methods
  this->PythonCppAPI.declareMethod(Self::CanOwnSubjectHierarchyItemMethod, "canOwnSubjectHierarchyItem");
  this->PythonCppAPI.declareMethod(Self::RoleForPluginMethod, "roleForPlugin");
  this->PythonCppAPI.declareMethod(Self::HelpTextMethod, "helpText");
  this->PythonCppAPI.declareMethod(Self::IconMethod, "icon");
  this->PythonCppAPI.declareMethod(Self::VisibilityIconMethod, "visibilityIcon");
  this->PythonCppAPI.declareMethod(Self::EditPropertiesMethod, "editProperties");
  this->PythonCppAPI.declareMethod(Self::DisplayedItemNameMethod, "displayedItemName");
  this->PythonCppAPI.declareMethod(Self::TooltipMethod, "tooltip");
  this->PythonCppAPI.declareMethod(Self::SetDisplayVisibilityMethod, "setDisplayVisibility");
  this->PythonCppAPI.declareMethod(Self::GetDisplayVisibilityMethod, "getDisplayVisibility");
  // Function related methods
  this->PythonCppAPI.declareMethod(Self::ItemContextMenuActionsMethod, "itemContextMenuActions");
  this->PythonCppAPI.declareMethod(Self::ViewContextMenuActionsMethod, "viewContextMenuActions");
  this->PythonCppAPI.declareMethod(Self::SceneContextMenuActionsMethod, "sceneContextMenuActions");
  this->PythonCppAPI.declareMethod(Self::ShowContextMenuActionsForItemMethod, "showContextMenuActionsForItem");
  this->PythonCppAPI.declareMethod(Self::ShowViewContextMenuActionsForItemMethod, "showViewContextMenuActionsForItem");
  // Parenting related methods (with default implementation)
  this->PythonCppAPI.declareMethod(Self::CanAddNodeToSubjectHierarchyMethod, "canAddNodeToSubjectHierarchy");
  this->PythonCppAPI.declareMethod(Self::CanReparentItemInsideSubjectHierarchyMethod, "canReparentItemInsideSubjectHierarchy");
  this->PythonCppAPI.declareMethod(Self::ReparentItemInsideSubjectHierarchyMethod, "reparentItemInsideSubjectHierarchy");
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyScriptedPluginPrivate::~qCjyxSubjectHierarchyScriptedPluginPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyScriptedPlugin methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyScriptedPlugin::qCjyxSubjectHierarchyScriptedPlugin(QObject *parent)
  : Superclass(parent)
  , d_ptr(new qCjyxSubjectHierarchyScriptedPluginPrivate)
{
  this->m_Name = QString("UnnamedScriptedPlugin");
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyScriptedPlugin::~qCjyxSubjectHierarchyScriptedPlugin() = default;

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyScriptedPlugin::pythonSource()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  return d->PythonSource;
}

//-----------------------------------------------------------------------------
bool qCjyxSubjectHierarchyScriptedPlugin::setPythonSource(const QString newPythonSource)
{
  Q_D(qCjyxSubjectHierarchyScriptedPlugin);

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

  // In case the plugin is within the main module file
  QString className = moduleName;
  if (!moduleName.endsWith("SubjectHierarchyPlugin"))
    {
    className.append("SubjectHierarchyPlugin");
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
                    QString("qCjyxSubjectHierarchyScriptedPlugin::setPythonSource - "
                            "Failed to load subject hierarchy scripted plugin: "
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
    qCritical() << Q_FUNC_INFO << ": Failed to set" << ("cjyx." + className);
    }

  return true;
}

//-----------------------------------------------------------------------------
PyObject* qCjyxSubjectHierarchyScriptedPlugin::self() const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  return d->PythonCppAPI.pythonSelf();
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyScriptedPlugin::setName(QString name)
{
  this->m_Name = name;
}

//-----------------------------------------------------------------------------
double qCjyxSubjectHierarchyScriptedPlugin::canOwnSubjectHierarchyItem(vtkIdType itemID)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->CanOwnSubjectHierarchyItemMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::canOwnSubjectHierarchyItem(itemID);
    }

  // Parse result
  if (!PyFloat_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'canOwnSubjectHierarchyItem' is expected to return a floating point number!";
    return this->Superclass::canOwnSubjectHierarchyItem(itemID);
    }

  return PyFloat_AsDouble(result);
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyScriptedPlugin::roleForPlugin()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* result = d->PythonCppAPI.callMethod(d->RoleForPluginMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::roleForPlugin();
    }

  // Parse result
  if (!PyString_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'roleForPlugin' is expected to return a string!";
    return this->Superclass::roleForPlugin();
    }

  const char* role = PyString_AsString(result);
  return QString::fromLocal8Bit(role);
}

//---------------------------------------------------------------------------
const QString qCjyxSubjectHierarchyScriptedPlugin::helpText()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* result = d->PythonCppAPI.callMethod(d->HelpTextMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::helpText();
    }

  // Parse result
  if (!PyString_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'helpText' is expected to return a string!";
    return this->Superclass::helpText();
    }

  const char* role = PyString_AsString(result);
  return QString::fromLocal8Bit(role);
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyScriptedPlugin::icon(vtkIdType itemID)
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->IconMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::icon(itemID);
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::Icon);
  if (resultVariant.isNull())
    {
    return this->Superclass::icon(itemID);
    }
  return resultVariant.value<QIcon>();
}

//---------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyScriptedPlugin::visibilityIcon(int visible)
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyInt_FromLong(visible));
  PyObject* result = d->PythonCppAPI.callMethod(d->VisibilityIconMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::visibilityIcon(visible);
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::Icon);
  if (resultVariant.isNull())
    {
    return this->Superclass::visibilityIcon(visible);
    }
  return resultVariant.value<QIcon>();
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyScriptedPlugin::editProperties(vtkIdType itemID)
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->EditPropertiesMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    this->Superclass::editProperties(itemID);
    }
}

//-----------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyScriptedPlugin::itemContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* result = d->PythonCppAPI.callMethod(d->ItemContextMenuActionsMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::itemContextMenuActions();
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::List);
  if (resultVariant.isNull())
    {
    return this->Superclass::itemContextMenuActions();
    }
  QList<QVariant> resultVariantList = resultVariant.toList();
  QList<QAction*> actionList;
  foreach(QVariant actionVariant, resultVariantList)
    {
    QAction* action = qobject_cast<QAction*>( actionVariant.value<QObject*>() );
    actionList << action;
    }
  return actionList;
}

//-----------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyScriptedPlugin::viewContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* result = d->PythonCppAPI.callMethod(d->ViewContextMenuActionsMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::viewContextMenuActions();
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::List);
  if (resultVariant.isNull())
    {
    return this->Superclass::viewContextMenuActions();
    }
  QList<QVariant> resultVariantList = resultVariant.toList();
  QList<QAction*> actionList;
  foreach(QVariant actionVariant, resultVariantList)
    {
    QAction* action = qobject_cast<QAction*>( actionVariant.value<QObject*>() );
    actionList << action;
    }
  return actionList;
}

//-----------------------------------------------------------------------------
QList<QAction*> qCjyxSubjectHierarchyScriptedPlugin::sceneContextMenuActions()const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* result = d->PythonCppAPI.callMethod(d->SceneContextMenuActionsMethod);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::sceneContextMenuActions();
    }

  // Parse result
  QVariant resultVariant = PythonQtConv::PyObjToQVariant(result, QVariant::List);
  if (resultVariant.isNull())
    {
    return this->Superclass::sceneContextMenuActions();
    }
  QList<QVariant> resultVariantList = resultVariant.toList();
  QList<QAction*> actionList;
  foreach(QVariant actionVariant, resultVariantList)
    {
    QAction* action = qobject_cast<QAction*>( actionVariant.value<QObject*>() );
    actionList << action;
    }
  return actionList;
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyScriptedPlugin::showContextMenuActionsForItem(vtkIdType itemID)
{
  Q_D(qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->ShowContextMenuActionsForItemMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    this->Superclass::showContextMenuActionsForItem(itemID);
    }
}

//---------------------------------------------------------------------------
void qCjyxSubjectHierarchyScriptedPlugin::showViewContextMenuActionsForItem(vtkIdType itemID, QVariantMap eventData)
{
  Q_D(qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(2);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyTuple_SET_ITEM(arguments, 1, PythonQtConv::QVariantMapToPyObject(eventData));
  PyObject* result = d->PythonCppAPI.callMethod(d->ShowViewContextMenuActionsForItemMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    this->Superclass::showContextMenuActionsForItem(itemID);
    }
}

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyScriptedPlugin::canAddNodeToSubjectHierarchy(
  vtkMRMLNode* node,
  vtkIdType parentItemID/*=vtkMRMLSubjectHierarchyNode::INVALID_ITEM_ID*/)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(2);
  PyTuple_SET_ITEM(arguments, 0, vtkPythonUtil::GetObjectFromPointer(node));
  PyTuple_SET_ITEM(arguments, 1, PyLong_FromLongLong(parentItemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->CanAddNodeToSubjectHierarchyMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::canAddNodeToSubjectHierarchy(node, parentItemID);
    }

  // Parse result
  if (!PyFloat_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'canAddNodeToSubjectHierarchy' is expected to return a floating point number!";
    return this->Superclass::canAddNodeToSubjectHierarchy(node, parentItemID);
    }

  return PyFloat_AsDouble(result);
}

//----------------------------------------------------------------------------
double qCjyxSubjectHierarchyScriptedPlugin::canReparentItemInsideSubjectHierarchy(
  vtkIdType itemID,
  vtkIdType parentItemID)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(2);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyTuple_SET_ITEM(arguments, 1, PyLong_FromLongLong(parentItemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->CanReparentItemInsideSubjectHierarchyMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::canReparentItemInsideSubjectHierarchy(itemID, parentItemID);
    }

  // Parse result
  if (!PyFloat_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'canReparentItemInsideSubjectHierarchy' is expected to return a floating point number!";
    return this->Superclass::canReparentItemInsideSubjectHierarchy(itemID, parentItemID);
    }

  return PyFloat_AsDouble(result);
}

//---------------------------------------------------------------------------
bool qCjyxSubjectHierarchyScriptedPlugin::reparentItemInsideSubjectHierarchy(
  vtkIdType itemID,
  vtkIdType parentItemID)
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(2);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyTuple_SET_ITEM(arguments, 1, PyLong_FromLongLong(parentItemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->ReparentItemInsideSubjectHierarchyMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::reparentItemInsideSubjectHierarchy(itemID, parentItemID);
    }

  // Parse result
  if (!PyBool_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'reparentItemInsideSubjectHierarchy' is expected to return a boolean!";
    return this->Superclass::reparentItemInsideSubjectHierarchy(itemID, parentItemID);
    }

  return result == Py_True;
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyScriptedPlugin::displayedItemName(vtkIdType itemID)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->DisplayedItemNameMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::displayedItemName(itemID);
    }

  // Parse result
  if (!PyString_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'displayedItemName' is expected to return a string!";
    return this->Superclass::displayedItemName(itemID);
    }

  return PyString_AsString(result);
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyScriptedPlugin::tooltip(vtkIdType itemID)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->TooltipMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::tooltip(itemID);
    }

  // Parse result
  if (!PyString_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'tooltip' is expected to return a string!";
    return this->Superclass::tooltip(itemID);
    }

  return PyString_AsString(result);
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyScriptedPlugin::setDisplayVisibility(vtkIdType itemID, int visible)
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(2);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyTuple_SET_ITEM(arguments, 1, PyInt_FromLong(visible));
  PyObject* result = d->PythonCppAPI.callMethod(d->SetDisplayVisibilityMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    this->Superclass::setDisplayVisibility(itemID, visible);
    }
}

//-----------------------------------------------------------------------------
int qCjyxSubjectHierarchyScriptedPlugin::getDisplayVisibility(vtkIdType itemID)const
{
  Q_D(const qCjyxSubjectHierarchyScriptedPlugin);
  PyObject* arguments = PyTuple_New(1);
  PyTuple_SET_ITEM(arguments, 0, PyLong_FromLongLong(itemID));
  PyObject* result = d->PythonCppAPI.callMethod(d->GetDisplayVisibilityMethod, arguments);
  Py_DECREF(arguments);
  if (!result)
    {
    // Method call failed (probably an omitted function), call default implementation
    return this->Superclass::getDisplayVisibility(itemID);
    }

  // Parse result
  if (!PyInt_Check(result))
    {
    qWarning() << d->PythonSource << ": " << Q_FUNC_INFO << ": Function 'getDisplayVisibility' is expected to return an integer!";
    return this->Superclass::getDisplayVisibility(itemID);
    }

  return (int)PyInt_AsLong(result);
}
