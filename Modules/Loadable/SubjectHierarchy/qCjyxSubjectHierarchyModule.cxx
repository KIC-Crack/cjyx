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

// QtGUI includes
#include <qCjyxApplication.h>

// SubjectHierarchy includes
#include "qCjyxSubjectHierarchyModule.h"
#include "qCjyxSubjectHierarchyModuleWidget.h"
#include "qCjyxSubjectHierarchySettingsPanel.h"
#include "vtkCjyxSubjectHierarchyModuleLogic.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginLogic.h"
#include "qCjyxSubjectHierarchyPluginHandler.h"

// MRML includes
#include <vtkMRMLScene.h>

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy
class qCjyxSubjectHierarchyModulePrivate
{
public:
  qCjyxSubjectHierarchyModulePrivate();
  ~qCjyxSubjectHierarchyModulePrivate();

  qCjyxSubjectHierarchyPluginLogic* PluginLogic{nullptr};
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModulePrivate::qCjyxSubjectHierarchyModulePrivate() = default;

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModulePrivate::~qCjyxSubjectHierarchyModulePrivate()
{
  if (this->PluginLogic)
    {
    delete this->PluginLogic;
    this->PluginLogic = nullptr;
    }
}

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyModule methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModule::qCjyxSubjectHierarchyModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSubjectHierarchyModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModule::~qCjyxSubjectHierarchyModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyModule::helpText()const
{
  QString help =
    "The Subject hierarchy module provides a nice and intuitive tree view of the loaded data."
    " It acts as a convenient central organizing point for many of the operations that 3D Cjyx and its extensions perform.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxSubjectHierarchyModule::acknowledgementText()const
{
  return "This work is part of SparKit project, funded by Cancer Care Ontario (CCO)'s ACRU program and Ontario Consortium for Adaptive Interventions in Radiation Oncology (OCAIRO).";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSubjectHierarchyModule::categories() const
{
  return QStringList() << "" << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSubjectHierarchyModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Csaba Pinter (Queen's)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxSubjectHierarchyModule::icon()const
{
  return QIcon(":/Icons/SubjectHierarchy.png");
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyModule::setup()
{
  this->Superclass::setup();

  if (qCjyxApplication::application())
    {
    qCjyxSubjectHierarchySettingsPanel* panel = new qCjyxSubjectHierarchySettingsPanel();
    qCjyxApplication::application()->settingsDialog()->addPanel("Subject hierarchy", panel);
    }
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxSubjectHierarchyModule::createLogic()
{
  Q_D(qCjyxSubjectHierarchyModule);

  // Create logic
  vtkCjyxSubjectHierarchyModuleLogic* logic = vtkCjyxSubjectHierarchyModuleLogic::New();
  // Handle scene change event if occurs
  qvtkConnect( logic, vtkCommand::ModifiedEvent, this, SLOT( onLogicModified() ) );

  // Create plugin logic
  d->PluginLogic = new qCjyxSubjectHierarchyPluginLogic();
  // Set plugin logic to plugin handler
  qCjyxSubjectHierarchyPluginHandler::instance()->setPluginLogic(d->PluginLogic);

  return logic;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxSubjectHierarchyModule::createWidgetRepresentation()
{
  Q_D(qCjyxSubjectHierarchyModule);

  qCjyxSubjectHierarchyModuleWidget* moduleWidget = new qCjyxSubjectHierarchyModuleWidget();
  if (!d->PluginLogic)
    {
    this->createLogic();
    }
  moduleWidget->setPluginLogic(d->PluginLogic);

  return moduleWidget;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyModule::onLogicModified()
{
  Q_D(qCjyxSubjectHierarchyModule);

  vtkMRMLScene* scene = this->mrmlScene();
  if (d->PluginLogic && scene != d->PluginLogic->mrmlScene())
    {
    // Set the new scene to the plugin logic
    d->PluginLogic->setMRMLScene(scene);
    }
}
