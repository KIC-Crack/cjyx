/*==============================================================================

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

// Segmentations includes
#include "qCjyxSegmentationsModule.h"
#include "qCjyxSegmentationsModuleWidget.h"
#include "qCjyxSegmentationsReader.h"
#include "qCjyxSegmentationsSettingsPanel.h"
#include "qCjyxSubjectHierarchySegmentationsPlugin.h"
#include "qCjyxSubjectHierarchySegmentsPlugin.h"
#include "vtkCjyxSegmentationsModuleLogic.h"
#include "vtkMRMLSegmentationsDisplayableManager3D.h"
#include "vtkMRMLSegmentationsDisplayableManager2D.h"
#include "qCjyxSegmentationsNodeWriter.h"

// Segment editor effects includes
#include "qCjyxSegmentEditorEffectFactory.h"
#include "qCjyxSegmentEditorPaintEffect.h"
#include "qCjyxSegmentEditorScissorsEffect.h"
#include "qCjyxSegmentEditorEraseEffect.h"

// Cjyx includes
#include <qCjyxIOManager.h>
#include <qCjyxNodeWriter.h>
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include <qCjyxCoreApplication.h>
#include <qCjyxModuleManager.h>
#include <vtkCjyxConfigure.h> // For Cjyx_USE_PYTHONQT

// Subject Hierarchy includes
#include "qCjyxSubjectHierarchyPluginHandler.h"

// Terminologies includes
#include "vtkCjyxTerminologiesModuleLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSubjectHierarchyNode.h>

// PythonQt includes
#ifdef Cjyx_USE_PYTHONQT
#include "PythonQt.h"
#endif

// Qt includes
#include <QDebug>

// DisplayableManager initialization
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkCjyxSegmentationsModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
/// \ingroup CjyxRt_QtModules_Segmentations
class qCjyxSegmentationsModulePrivate
{
public:
  qCjyxSegmentationsModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxSegmentationsModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxSegmentationsModulePrivate::qCjyxSegmentationsModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSegmentationsModule methods

//-----------------------------------------------------------------------------
qCjyxSegmentationsModule::qCjyxSegmentationsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSegmentationsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxSegmentationsModule::~qCjyxSegmentationsModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxSegmentationsModule::helpText()const
{
  QString help =
    "Segmentations module manages segmentations. Each segmentation can contain"
    " multiple segments, which correspond to one structure or ROI. Each segment"
    " can contain multiple data representations for the same structure, and the"
    " module supports automatic conversion between these representations"
    " as well as advanced display settings and import/export features.";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxSegmentationsModule::acknowledgementText()const
{
  return "This work is part of SparKit project, funded by Cancer Care Ontario (CCO)'s ACRU program and Ontario Consortium for Adaptive Interventions in Radiation Oncology (OCAIRO).";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSegmentationsModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSegmentationsModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Csaba Pinter (Queen's)");
  moduleContributors << QString("Adam Rankin (Robarts)");
  moduleContributors << QString("Andras Lasso (Queen's)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QStringList qCjyxSegmentationsModule::dependencies()const
{
  return QStringList() << "Terminologies";
}

//-----------------------------------------------------------------------------
QIcon qCjyxSegmentationsModule::icon()const
{
  return QIcon(":/Icons/Segmentations.png");
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModule::setMRMLScene(vtkMRMLScene* scene)
{
  // Connect scene node added event to make connections enabling per-segment subject hierarchy actions
  qvtkReconnect( this->mrmlScene(), scene, vtkMRMLScene::NodeAddedEvent, this, SLOT( onNodeAdded(vtkObject*,vtkObject*) ) );

  Superclass::setMRMLScene(scene);

  // Subject hierarchy is instantiated before Segmentations, so need to connect to existing the quasi-singleton subject hierarchy node
  vtkCollection* shNodeCollection = scene->GetNodesByClass("vtkMRMLSubjectHierarchyNode");
  vtkMRMLSubjectHierarchyNode*  subjectHierarchyNode = vtkMRMLSubjectHierarchyNode::SafeDownCast(
    shNodeCollection->GetItemAsObject(0) );
  shNodeCollection->Delete();
  this->onNodeAdded(scene, subjectHierarchyNode);
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModule::setup()
{
  this->Superclass::setup();

  vtkCjyxSegmentationsModuleLogic* segmentationsLogic = vtkCjyxSegmentationsModuleLogic::SafeDownCast(this->logic());

  // Register subject hierarchy plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchySegmentationsPlugin());
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchySegmentsPlugin());

  // Register IOs
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  ioManager->registerIO(new qCjyxSegmentationsNodeWriter(this));
  ioManager->registerIO(new qCjyxSegmentationsReader(segmentationsLogic, this));

  // Register settings panel
  if (qCjyxApplication::application())
    {
    qCjyxSegmentationsSettingsPanel* panel = new qCjyxSegmentationsSettingsPanel();
    qCjyxApplication::application()->settingsDialog()->addPanel("Segmentations", panel);
    panel->setSegmentationsLogic(segmentationsLogic);
    }

  // Use the displayable manager class to make sure the the containing library is loaded
  vtkSmartPointer<vtkMRMLSegmentationsDisplayableManager3D> dm3d = vtkSmartPointer<vtkMRMLSegmentationsDisplayableManager3D>::New();
  vtkSmartPointer<vtkMRMLSegmentationsDisplayableManager2D> dm2d = vtkSmartPointer<vtkMRMLSegmentationsDisplayableManager2D>::New();
  // Register displayable managers
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager("vtkMRMLSegmentationsDisplayableManager3D");
  vtkMRMLSliceViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager("vtkMRMLSegmentationsDisplayableManager2D");

  // Register default segment editor effects
  // C++ effects
  qCjyxSegmentEditorEffectFactory::instance()->registerEffect(new qCjyxSegmentEditorPaintEffect());
  qCjyxSegmentEditorEffectFactory::instance()->registerEffect(new qCjyxSegmentEditorEraseEffect());
  qCjyxSegmentEditorEffectFactory::instance()->registerEffect(new qCjyxSegmentEditorScissorsEffect());
  // Python effects
  // (otherwise it would be the responsibility of the module that embeds the segment editor widget)
#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    PythonQt::init();
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    context.evalScript(QString("import SegmentEditorEffects; SegmentEditorEffects.registerEffects()"));
    }
#endif
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxSegmentationsModule::createWidgetRepresentation()
{
  qCjyxSegmentationsModuleWidget* moduleWidget = new qCjyxSegmentationsModuleWidget();
  return moduleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxSegmentationsModule::createLogic()
{
  return vtkCjyxSegmentationsModuleLogic::New();
}

//-----------------------------------------------------------------------------
void qCjyxSegmentationsModule::onNodeAdded(vtkObject* sceneObject, vtkObject* nodeObject)
{
  vtkMRMLScene* scene = vtkMRMLScene::SafeDownCast(sceneObject);
  if (!scene)
    {
    return;
    }

  // Get segmentations subject hierarchy plugin
  qCjyxSubjectHierarchySegmentationsPlugin* segmentationsPlugin = qobject_cast<qCjyxSubjectHierarchySegmentationsPlugin*>(
    qCjyxSubjectHierarchyPluginHandler::instance()->pluginByName("Segmentations") );
  if (!segmentationsPlugin)
    {
    qCritical() << Q_FUNC_INFO << ": Failed to access segmentations subject hierarchy plugin";
    return;
    }

  // Connect segment added and removed events to plugin to update subject hierarchy accordingly
  vtkMRMLSegmentationNode* segmentationNode = vtkMRMLSegmentationNode::SafeDownCast(nodeObject);
  if (segmentationNode)
    {
    qvtkConnect( segmentationNode, vtkSegmentation::SegmentAdded,
      segmentationsPlugin, SLOT( onSegmentAdded(vtkObject*,void*) ) );
    qvtkConnect( segmentationNode, vtkSegmentation::SegmentRemoved,
      segmentationsPlugin, SLOT( onSegmentRemoved(vtkObject*,void*) ) );
    qvtkConnect( segmentationNode, vtkSegmentation::SegmentModified,
      segmentationsPlugin, SLOT( onSegmentModified(vtkObject*,void*) ) );
    qvtkConnect(segmentationNode, vtkMRMLSegmentationNode::DisplayModifiedEvent,
      segmentationsPlugin, SLOT( onDisplayNodeModified(vtkObject*) ) );
    }

  // Connect subject hierarchy modified event to handle renaming segments from subject hierarchy
  vtkMRMLSubjectHierarchyNode* subjectHierarchyNode = vtkMRMLSubjectHierarchyNode::SafeDownCast(nodeObject);
  if (subjectHierarchyNode)
    {
    qvtkConnect( subjectHierarchyNode, vtkMRMLSubjectHierarchyNode::SubjectHierarchyItemModifiedEvent,
      segmentationsPlugin, SLOT( onSubjectHierarchyItemModified(vtkObject*,void*) ) );
    qvtkConnect( subjectHierarchyNode, vtkMRMLSubjectHierarchyNode::SubjectHierarchyItemAboutToBeRemovedEvent,
      segmentationsPlugin, SLOT( onSubjectHierarchyItemAboutToBeRemoved(vtkObject*,void*) ) );
    }
}

//-----------------------------------------------------------------------------
QStringList qCjyxSegmentationsModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLSegmentationNode";
}
