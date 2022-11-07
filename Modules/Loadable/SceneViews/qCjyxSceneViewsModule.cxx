
// QTGUI includes
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include <qCjyxNodeWriter.h>

// SceneViewsModule includes
#include "qCjyxSceneViewsModule.h"

#include <qCjyxSceneViewsModuleWidget.h>
#include <vtkCjyxSceneViewsModuleLogic.h>

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchySceneViewsPlugin.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SceneViews
class qCjyxSceneViewsModulePrivate
{
  public:
};

//-----------------------------------------------------------------------------
qCjyxSceneViewsModule::qCjyxSceneViewsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSceneViewsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxSceneViewsModule::~qCjyxSceneViewsModule() = default;

//-----------------------------------------------------------------------------
void qCjyxSceneViewsModule::setup()
{
  qCjyxCoreIOManager* ioManager =
    qCjyxApplication::application()->coreIOManager();
  ioManager->registerIO(new qCjyxNodeWriter(
    "SceneViews", QString("SceneViewFile"),
    QStringList() << "vtkMRMLSceneViewNode", true, this));

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(
    new qCjyxSubjectHierarchySceneViewsPlugin());
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxSceneViewsModule::createWidgetRepresentation()
{
  return new qCjyxSceneViewsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxSceneViewsModule::createLogic()
{

  return vtkCjyxSceneViewsModuleLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxSceneViewsModule::helpText() const
{
  QString help =
    "The SceneViews module. Create, edit, restore, delete scene views. Scene "
    "views capture the state of the MRML scene at a given point. The "
    "recommended way to use them is to load all of your data and then adjust "
    "visibility of the elements and capture interesting scene views. "
    "Unexpected behavior may occur if you add or delete data from the scene "
    "while saving and restoring scene views.\n";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxSceneViewsModule::acknowledgementText() const
{
  return "This module was developed by Daniel Haehn and Kilian Pohl. The research was funded by an ARRA supplement to NIH NCRR (P41 RR13218).";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSceneViewsModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  moduleContributors << QString("Wendy Plesniak (SPL, BWH)");
  moduleContributors << QString("Daniel Haehn (UPenn)");
  moduleContributors << QString("Kilian Pohl (UPenn)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxSceneViewsModule::icon() const
{
  return QIcon(":/Icons/SelectCameras.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxSceneViewsModule::categories() const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
void qCjyxSceneViewsModule::showSceneViewDialog()
{
  Q_ASSERT(this->widgetRepresentation());
  dynamic_cast<qCjyxSceneViewsModuleWidget*>(this->widgetRepresentation())
    ->showSceneViewDialog();
}

//-----------------------------------------------------------------------------
QStringList qCjyxSceneViewsModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLSceneViewNode";
}
