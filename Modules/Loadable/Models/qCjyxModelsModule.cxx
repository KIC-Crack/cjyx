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

// Models includes
#include "qCjyxModelsModule.h"
#include "qCjyxModelsModuleWidget.h"
#include "qCjyxModelsReader.h"

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxIOManager.h"
#include "qCjyxModelsDialog.h"
#include "qCjyxModuleManager.h"
#include "qCjyxNodeWriter.h"

// Cjyx logic includes
#include <vtkCjyxApplicationLogic.h>
#include <vtkCjyxModelsLogic.h>

// MRML includes
#include "vtkMRMLColorLogic.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyModelsPlugin.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Models
class qCjyxModelsModulePrivate
{
public:
  qCjyxModelsModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxModelsModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxModelsModulePrivate::qCjyxModelsModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxModelsModule methods

//-----------------------------------------------------------------------------
qCjyxModelsModule::qCjyxModelsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxModelsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxModelsModule::~qCjyxModelsModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxModelsModule::helpText()const
{
  QString help =
    "The Models Module loads and adjusts display parameters of models such as Color, Transparency, and Clipping.<br>"
    "Save models via the File menu, Save button.<br>"
    "The Add 3D model or a model directory button will allow you to load any "
    "model that Cjyx can read, as well as all the VTK models in a directory. "
    "Add Scalar Overlay will load a scalar file and associate it with the "
    "currently active model.<br>You can adjust the display properties of the "
    "models in the Display pane. Select the model you wish to work on from the "
    "model selector drop down menu. Scalar overlays are loaded with a default "
    "color look up table, but can be reassigned manually. Once a new scalar "
    "overlay is chosen, currently the old color map is still used, so that "
    "must be adjusted in conjunction with the overlay.<br>"
    "Clipping is turned on for a model in the Display pane, and the slice "
    "planes that will clip the model are selected in the Clipping pane.<br>"
    "The Model Hierarchy pane allows you to group models together and set the "
    "group's properties.";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxModelsModule::acknowledgementText()const
{
  return "This work was partially funded by NIH grants 3P41RR013218-12S1 and R01CA184354.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Alex Yarmakovich (Isomics)");
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  moduleContributors << QString("Alexis Girault (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxModelsModule::icon()const
{
  return QIcon(":/Icons/Large/CjyxModels.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsModule::dependencies() const
{
  QStringList moduleDependencies;
  moduleDependencies << "Colors";
  return moduleDependencies;
}

//-----------------------------------------------------------------------------
void qCjyxModelsModule::setup()
{
  this->Superclass::setup();
  // Configure models logic
  vtkCjyxModelsLogic* modelsLogic =
    vtkCjyxModelsLogic::SafeDownCast(this->logic());
  if (qCjyxApplication::application())
    {
    // Register IOs
    qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
    ioManager->registerIO(new qCjyxModelsReader(modelsLogic, this));
    ioManager->registerDialog(new qCjyxModelsDialog(this));
    ioManager->registerIO(new qCjyxNodeWriter(
      "Models", QString("ModelFile"),
      QStringList() << "vtkMRMLModelNode", true, this));
    }

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyModelsPlugin());
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxModelsModule::createWidgetRepresentation()
{
  qCjyxModelsModuleWidget* widget = new qCjyxModelsModuleWidget;
  return widget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxModelsModule::createLogic()
{
  return vtkCjyxModelsLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLModelNode"
    << "vtkMRMLModelDisplayNode";
}
