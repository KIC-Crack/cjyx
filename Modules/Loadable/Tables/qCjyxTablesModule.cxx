/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright 2015 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Andras Lasso (PerkLab, Queen's
  University) and Kevin Wang (Princess Margaret Hospital, Toronto) and was
  supported through OCAIRO and the Applied Cancer Research Unit program of
  Cancer Care Ontario.

==============================================================================*/

// Slice includes
#include <qCjyxCoreApplication.h>
#include <qCjyxCoreIOManager.h>
#include <qCjyxNodeWriter.h>

// Tables Logic includes
#include <vtkCjyxTablesLogic.h>

// Tables includes
#include "qCjyxTablesModule.h"
#include "qCjyxTablesReader.h"
#include "qCjyxTablesModuleWidget.h"
// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyTablesPlugin.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxTablesModulePrivate
{
public:
  qCjyxTablesModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxTablesModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxTablesModulePrivate::qCjyxTablesModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxTablesModule methods

//-----------------------------------------------------------------------------
qCjyxTablesModule::qCjyxTablesModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxTablesModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxTablesModule::~qCjyxTablesModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxTablesModule::icon()const
{
  return QIcon(":/Icons/Tables.png");
}

//-----------------------------------------------------------------------------
QString qCjyxTablesModule::helpText()const
{
  QString help =
    "The Tables module allows displaying and editing of spreadsheets.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxTablesModule::acknowledgementText()const
{
  return "This work was was partially funded by OCAIRO, the Applied Cancer Research Unit program of Cancer Care Ontario, and Department of Anesthesia and Critical Care Medicine, Children's Hospital of Philadelphia.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTablesModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Andras Lasso (PerkLab), Kevin Wang (PMH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QStringList qCjyxTablesModule::categories() const
{
  return QStringList() << "Informatics";
}


//-----------------------------------------------------------------------------
QStringList qCjyxTablesModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxTablesModule::setup()
{
  this->Superclass::setup();
  vtkCjyxTablesLogic* TablesLogic =
    vtkCjyxTablesLogic::SafeDownCast(this->logic());

  qCjyxCoreIOManager* ioManager =
    qCjyxCoreApplication::application()->coreIOManager();
  ioManager->registerIO(new qCjyxTablesReader(TablesLogic,this));
  ioManager->registerIO(new qCjyxNodeWriter(
    "Table", QString("TableFile"),
    QStringList() << "vtkMRMLTableNode", false, this));
  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyTablesPlugin());
}


//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxTablesModule::createWidgetRepresentation()
{
  return new qCjyxTablesModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTablesModule::createLogic()
{
  return vtkCjyxTablesLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxTablesModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLTableNode";
}
