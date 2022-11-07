/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Alex Yarmakovich, Isomics Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// QtGUI includes
#include <qCjyxApplication.h>
#include <qCjyxCoreIOManager.h>
#include <qCjyxNodeWriter.h>

// VolumeRendering Logic includes
#include <vtkCjyxVolumeRenderingLogic.h>
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>

// VolumeRendering includes
#include "qCjyxVolumeRenderingModule.h"
#include "qCjyxVolumeRenderingModuleWidget.h"
#include "qCjyxVolumeRenderingReader.h"
#include "qCjyxShaderPropertyReader.h"
#include "qCjyxVolumeRenderingSettingsPanel.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyVolumeRenderingPlugin.h"

// DisplayableManager initialization
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkCjyxVolumeRenderingModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_VolumeRendering
class qCjyxVolumeRenderingModulePrivate
{
public:
  qCjyxVolumeRenderingModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxVolumeRenderingModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingModulePrivate::qCjyxVolumeRenderingModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxVolumeRenderingModule methods

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingModule::qCjyxVolumeRenderingModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumeRenderingModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingModule::~qCjyxVolumeRenderingModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingModule::helpText()const
{
  QString help = QString(
    "Volume Rendering Module provides advanced tools for toggling interactive "
    "volume rendering of datasets.<br/>"
    "If supported, hardware accelerated volume rendering is made available."
    "The module permits selection of preset transfer functions to colorize and set opacity "
    "of data in a task-appropriate way, and tools to customize the transfer functions that specify "
    "these parameters.<br/>");
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingModule::acknowledgementText()const
{
  QString acknowledgement =
    "<center><table border=\"0\"><tr>"
    "<td><img src=\":Logos/NAMIC.png\" alt\"NA-MIC\"></td>"
    "<td><img src=\":Logos/NAC.png\" alt\"NAC\"></td>"
    "</tr><tr>"
    "<td><img src=\":Logos/BIRN-NoText.png\" alt\"BIRN\"></td>"
    "<td><img src=\":Logos/NCIGT.png\" alt\"NCIGT\"></td>"
    "</tr></table></center>"
    "This work is supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community."
    "Some of the transfer functions were contributed by Kitware Inc. (VolView)";
  return acknowledgement;
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumeRenderingModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Alex Yarmarkovich (Isomics)");
  moduleContributors << QString("Yanling Liu (SAIC-Frederick, NCI-Frederick)");
  moduleContributors << QString("Andreas Freudling (SPL, BWH)");
  moduleContributors << QString("Ron Kikinis (SPL, BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxVolumeRenderingModule::icon()const
{
  return QIcon(":/Icons/VolumeRendering.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumeRenderingModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingModule::setup()
{
  this->Superclass::setup();
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager(
    "vtkMRMLVolumeRenderingDisplayableManager");

  vtkCjyxVolumeRenderingLogic* volumeRenderingLogic =
    vtkCjyxVolumeRenderingLogic::SafeDownCast(this->logic());
  if (qCjyxApplication::application())
    {
    qCjyxVolumeRenderingSettingsPanel* panel = new qCjyxVolumeRenderingSettingsPanel;
    qCjyxApplication::application()->settingsDialog()->addPanel("Volume rendering", panel);
    panel->setVolumeRenderingLogic(volumeRenderingLogic);
    }

  // Register VolumeProperty reader/writer
  qCjyxCoreIOManager* coreIOManager = qCjyxCoreApplication::application()->coreIOManager();
  coreIOManager->registerIO(new qCjyxVolumeRenderingReader(volumeRenderingLogic, this));
  coreIOManager->registerIO(new qCjyxNodeWriter("Transfer Function", QString("TransferFunctionFile"),
    QStringList() << "vtkMRMLVolumePropertyNode", true, this));

  // Register ShaderProperty reader/writer
  coreIOManager->registerIO(new qCjyxShaderPropertyReader(volumeRenderingLogic,this));
  coreIOManager->registerIO(new qCjyxNodeWriter("Shader Property", QString("ShaderPropertyFile"),
    QStringList() << "vtkMRMLShaderPropertyNode", true, this ));

  // Register Subject Hierarchy core plugins
  vtkCjyxVolumeRenderingLogic* logic = vtkCjyxVolumeRenderingLogic::SafeDownCast(this->logic());
  qCjyxSubjectHierarchyVolumeRenderingPlugin* shPlugin = new qCjyxSubjectHierarchyVolumeRenderingPlugin();
  shPlugin->setVolumeRenderingLogic(logic);
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(shPlugin);
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxVolumeRenderingModule::createWidgetRepresentation()
{
  return new qCjyxVolumeRenderingModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxVolumeRenderingModule::createLogic()
{
  return vtkCjyxVolumeRenderingLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumeRenderingModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLVolumePropertyNode"
    << "vtkMRMLShaderPropertyNode"
    << "vtkMRMLVolumeRenderingDisplayNode"
    << "vtkMRMLAnnotationROINode" // volume rendering clipping box
    << "vtkMRMLMarkupsROINode"; // volume rendering clipping box
}
