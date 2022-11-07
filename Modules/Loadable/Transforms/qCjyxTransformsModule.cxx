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

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include "qCjyxNodeWriter.h"

#include "vtkCjyxTransformLogic.h"
#include "vtkMRMLSliceViewDisplayableManagerFactory.h"
#include "vtkMRMLThreeDViewDisplayableManagerFactory.h"

// Transforms includes
#include "qCjyxTransformsModule.h"
#include "qCjyxTransformsModuleWidget.h"
#include "qCjyxTransformsReader.h"

// VTK includes
#include "vtkSmartPointer.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyTransformsPlugin.h"

// DisplayableManager initialization
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkCjyxTransformsModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
class qCjyxTransformsModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxTransformsModule::qCjyxTransformsModule(QObject* _parentObject)
  : Superclass(_parentObject)
  , d_ptr(new qCjyxTransformsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxTransformsModule::~qCjyxTransformsModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxTransformsModule::icon()const
{
  return QIcon(":/Icons/Transforms.png");
}


//-----------------------------------------------------------------------------
QStringList qCjyxTransformsModule::categories() const
{
  return QStringList() << "" << "Registration";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTransformsModule::dependencies() const
{
  QStringList moduleDependencies;
  moduleDependencies << "Units";
  return moduleDependencies;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxTransformsModule::createWidgetRepresentation()
{
  return new qCjyxTransformsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTransformsModule::createLogic()
{
  return vtkCjyxTransformLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxTransformsModule::helpText()const
{
  QString help = "The Transforms Module creates and edits transforms.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxTransformsModule::acknowledgementText()const
{
  QString acknowledgement =
    "This work was supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community.";
  return acknowledgement;
}

//-----------------------------------------------------------------------------
QStringList qCjyxTransformsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Alex Yarmarkovich (Isomics)");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Andras Lasso (PerkLab, Queen's)");
  moduleContributors << QString("Franklin King (PerkLab, Queen's)");
  moduleContributors << QString("Ron Kikinis (SPL, BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
void qCjyxTransformsModule::setup()
{
  qCjyxApplication * app = qCjyxApplication::application();
  if (!app)
    {
    return;
    }
  vtkCjyxTransformLogic* transformLogic =
    vtkCjyxTransformLogic::SafeDownCast(this->logic());
  app->coreIOManager()->registerIO(
    new qCjyxTransformsReader(transformLogic, this));
  app->coreIOManager()->registerIO(new qCjyxNodeWriter(
    "Transforms", QString("TransformFile"),
    QStringList() << "vtkMRMLTransformNode", true, this));

  // Register displayable managers
  vtkMRMLSliceViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager("vtkMRMLTransformsDisplayableManager2D");
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager("vtkMRMLTransformsDisplayableManager3D");
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager("vtkMRMLLinearTransformsDisplayableManager3D");

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyTransformsPlugin());
}

//-----------------------------------------------------------------------------
QStringList qCjyxTransformsModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLTransformNode"
    << "vtkMRMLTransformDisplayNode";
}
