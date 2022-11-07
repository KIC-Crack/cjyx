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

// Cjyx includes
#include "qCjyxCamerasModule.h"
#include "qCjyxCamerasModuleWidget.h"
#include "vtkCjyxCamerasModuleLogic.h"

//-----------------------------------------------------------------------------
class qCjyxCamerasModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxCamerasModule::qCjyxCamerasModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxCamerasModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxCamerasModule::~qCjyxCamerasModule() = default;

//-----------------------------------------------------------------------------
QStringList qCjyxCamerasModule::categories()const
{
  return QStringList() << "Developer Tools";
}

//-----------------------------------------------------------------------------
QIcon qCjyxCamerasModule::icon()const
{
  return QIcon(":/Icons/Cameras.png");
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxCamerasModule::createWidgetRepresentation()
{
  return new qCjyxCamerasModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxCamerasModule::createLogic()
{
  return vtkCjyxCamerasModuleLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxCamerasModule::helpText()const
{
  QString help = tr(
    "Manage 3D views and cameras.<br>"
    "The view pulldown menu below can be used to create new views and select "
    "the active view. Switch the layout to \"Tabbed 3D Layout\" from the "
    "layout icon in the toolbar to access multiple views. The view selected in "
    "\"Tabbed 3D Layout\" becomes the active view and replaces the 3D view in "
    "all other layouts. The camera pulldown menu below can be used to set the "
    "active camera for the selected view.<br>"
    "WARNING: this is rather experimental at the moment (fiducials, IO/data, "
    "closing the scene are probably broken for new views).<br>");
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxCamerasModule::acknowledgementText()const
{
  QString acknowledgement = tr(
    "<center><table border=\"0\"><tr>"
    "<td><img src=\":Logos/NAMIC.png\" alt\"NA-MIC\"></td>"
    "<td><img src=\":Logos/NAC.png\" alt\"NAC\"></td>"
    "</tr><tr>"
    "<td><img src=\":Logos/BIRN-NoText.png\" alt\"BIRN\"></td>"
    "<td><img src=\":Logos/NCIGT.png\" alt\"NCIGT\"></td>"
    "</tr></table></center>"
    "This work is supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community.");
  return acknowledgement;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCamerasModule::contributors()const
{
  QStringList contributors;
  contributors << QString("Julien Finet (Kitware)");
  contributors << QString("Sebastien Barr&eacute; (Kitware)");
  return contributors;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCamerasModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLCameraNode"
    << "vtkMRMLViewNode";
}
