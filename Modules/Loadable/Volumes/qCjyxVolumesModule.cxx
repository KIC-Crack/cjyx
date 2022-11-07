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
#include <QDebug>

// Cjyx includes
#include <qCjyxCoreApplication.h>
#include <qCjyxIOManager.h>
#include <qCjyxModuleManager.h>
#include <qCjyxNodeWriter.h>

// Volumes Logic includes
#include <vtkCjyxVolumesLogic.h>

// Volumes QTModule includes
#include "qCjyxVolumesReader.h"
#include "qCjyxVolumesModule.h"
#include "qCjyxVolumesModuleWidget.h"

// MRML Logic includes
#include <vtkMRMLColorLogic.h>

// MRML includes
#include <vtkMRMLScene.h>

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyVolumesPlugin.h"
#include "qCjyxSubjectHierarchyLabelMapsPlugin.h"
#include "qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Volumes
class qCjyxVolumesModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxVolumesModule::qCjyxVolumesModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumesModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxVolumesModule::~qCjyxVolumesModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxVolumesModule::helpText()const
{
  QString help = QString(
    "The Volumes Module is the interface for adjusting Window, Level, Threshold, "
    "Color LUT and other parameters that control the display of volume image data "
    "in the scene.<br>");
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxVolumesModule::acknowledgementText()const
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
QStringList qCjyxVolumesModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Steve Pieper (Isomics)");
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Alex Yarmarkovich (Isomics)");
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  moduleContributors << QString("Ron Kikinis (SPL, BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxVolumesModule::icon()const
{
  return QIcon(":/Icons/Medium/CjyxVolumes.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumesModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumesModule::dependencies() const
{
  QStringList moduleDependencies;
  moduleDependencies << "Colors" << "Units";
  return moduleDependencies;
}

//-----------------------------------------------------------------------------
void qCjyxVolumesModule::setup()
{
  this->Superclass::setup();

  vtkCjyxVolumesLogic* volumesLogic =
    vtkCjyxVolumesLogic::SafeDownCast(this->logic());

  qCjyxCoreIOManager* ioManager =
    qCjyxCoreApplication::application()->coreIOManager();
  ioManager->registerIO(new qCjyxVolumesReader(volumesLogic,this));
  ioManager->registerIO(new qCjyxNodeWriter(
    "Volumes", QString("VolumeFile"),
    QStringList() << "vtkMRMLVolumeNode", true, this));

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyVolumesPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyLabelMapsPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyDiffusionTensorVolumesPlugin());
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxVolumesModule::createWidgetRepresentation()
{
  return new qCjyxVolumesModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxVolumesModule::createLogic()
{
  return vtkCjyxVolumesLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumesModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLVolumeNode"
    << "vtkMRMLVolumeDisplayNode";
}
