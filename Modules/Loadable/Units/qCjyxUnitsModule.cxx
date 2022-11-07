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

  This file was originally developed by Johan Andruejol, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Cjyx includes
#include <qCjyxApplication.h>
#include <vtkCjyxUnitsLogic.h>

// Units includes
#include "qCjyxUnitsModule.h"
#include "qCjyxUnitsSettingsPanel.h"

//-----------------------------------------------------------------------------
class qCjyxUnitsModulePrivate
{
public:
  qCjyxUnitsModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxUnitsModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxUnitsModulePrivate
::qCjyxUnitsModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxUnitsModule methods

//-----------------------------------------------------------------------------
qCjyxUnitsModule
::qCjyxUnitsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxUnitsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxUnitsModule::~qCjyxUnitsModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxUnitsModule::helpText()const
{
  QString help =
    "This module controls the Units of the scene."
    " It gives the option to create, edit and remove units.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxUnitsModule::acknowledgementText()const
{
  QString acknowledgement = QString(
    "<center><table border=\"0\"><tr>"
    "<td><img src=\":Logos/NAMIC.png\" alt\"NA-MIC\"></td>"
    "<td><img src=\":Logos/NAC.png\" alt\"NAC\"></td>"
    "</tr><tr>"
    "<td><img src=\":Logos/BIRN-NoText.png\" alt\"BIRN\"></td>"
    "<td><img src=\":Logos/NCIGT.png\" alt\"NCIGT\"></td>"
    "</tr></table></center>"
    "This work was supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community.");
  return acknowledgement;
}
//-----------------------------------------------------------------------------
QStringList qCjyxUnitsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Johan Andruejol (Kitware)")
    << QString("Julien Finet (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxUnitsModule::icon()const
{
  return QIcon("");
}

//-----------------------------------------------------------------------------
QStringList qCjyxUnitsModule::categories() const
{
  return QStringList() << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxUnitsModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
bool qCjyxUnitsModule::isHidden() const
{
  return true;
}

//-----------------------------------------------------------------------------
void qCjyxUnitsModule::setup()
{
  this->Superclass::setup();

  vtkCjyxUnitsLogic* logic =
    vtkCjyxUnitsLogic::SafeDownCast(this->logic());
  if (logic && qCjyxApplication::application())
    {
    qCjyxUnitsSettingsPanel* panel = new qCjyxUnitsSettingsPanel;
    qCjyxApplication::application()->settingsDialog()->addPanel(
      "Units", panel);
    panel->setUnitsLogic(logic);
    }
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxUnitsModule
::createWidgetRepresentation()
{
  return nullptr;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxUnitsModule::createLogic()
{
  return vtkCjyxUnitsLogic::New();
}
