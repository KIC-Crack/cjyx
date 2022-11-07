/*==============================================================================

  Copyright (c) Kapteyn Astronomical Institute
  University of Groningen, Groningen, Netherlands. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Davide Punzo, Kapteyn Astronomical Institute,
  and was supported through the European Research Council grant nr. 291531.

==============================================================================*/

// Slice includes
#include <qCjyxCoreApplication.h>
#include <qCjyxCoreIOManager.h>
#include <qCjyxNodeWriter.h>

// Plots Logic includes
#include <vtkCjyxPlotsLogic.h>

// Plots includes
#include "qCjyxPlotsModule.h"
#include "qCjyxPlotsModuleWidget.h"
// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyPlotsPlugin.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxPlotsModulePrivate
{
public:
  qCjyxPlotsModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxPlotsModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxPlotsModulePrivate::qCjyxPlotsModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxPlotsModule methods

//-----------------------------------------------------------------------------
qCjyxPlotsModule::qCjyxPlotsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxPlotsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxPlotsModule::~qCjyxPlotsModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxPlotsModule::icon()const
{
  return QIcon(":/Icons/Plots.png");
}

//-----------------------------------------------------------------------------
QString qCjyxPlotsModule::helpText()const
{
  QString help = "The Plots module allows editing properties of plots.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxPlotsModule::acknowledgementText()const
{
  return "This module was originally developed by Davide Punzo, Kapteyn Astronomical Institute,"
    "and was supported through the European Research Council grant nr. 291531.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxPlotsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << "Davide Punzo (Kapteyn Astronomical Institute)";
  moduleContributors << "Andras Lasso (PerkLab)";
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QStringList qCjyxPlotsModule::categories() const
{
  return QStringList() << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxPlotsModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxPlotsModule::setup()
{
  this->Superclass::setup();

  // Register Subject Hierarchy core plugins

  vtkCjyxPlotsLogic* plotsLogic = vtkCjyxPlotsLogic::SafeDownCast(this->logic());
  qCjyxSubjectHierarchyPlotsPlugin* shPlugin = new qCjyxSubjectHierarchyPlotsPlugin();
  shPlugin->setPlotsLogic(plotsLogic);
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(shPlugin);
}


//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxPlotsModule::createWidgetRepresentation()
{
  return new qCjyxPlotsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxPlotsModule::createLogic()
{
  return vtkCjyxPlotsLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxPlotsModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLPlotChartNode" << "vtkMRMLPlotSeriesNode";
}
