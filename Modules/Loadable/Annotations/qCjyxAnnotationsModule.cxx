
// MRMLDisplayableManager includes
#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>

// QTGUI includes
#include <qCjyxApplication.h>
#include <qCjyxCoreApplication.h>
#include <qCjyxIOManager.h>
#include <qCjyxNodeWriter.h>
#include <vtkCjyxConfigure.h> // For Cjyx_USE_PYTHONQT

// AnnotationModule includes
#include "qCjyxAnnotationsModule.h"
#include "GUI/qCjyxAnnotationModuleWidget.h"
#include "vtkCjyxAnnotationModuleLogic.h"
#include "qCjyxAnnotationsReader.h"

// PythonQt includes
#ifdef Cjyx_USE_PYTHONQT
#include "PythonQt.h"
#endif

// DisplayableManager initialization
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkCjyxAnnotationsModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Annotation
class qCjyxAnnotationsModulePrivate
{
  public:
};

//-----------------------------------------------------------------------------
qCjyxAnnotationsModule::qCjyxAnnotationsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxAnnotationsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxAnnotationsModule::~qCjyxAnnotationsModule() = default;

//-----------------------------------------------------------------------------
void qCjyxAnnotationsModule::setup()
{
  /// Register Displayable Managers:

  // 3D
  QStringList threeDdisplayableManagers;
  threeDdisplayableManagers
      << "Fiducial"
      << "Ruler"
      << "ROI"
      ;

  foreach(const QString& name, threeDdisplayableManagers)
    {
    vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager(
        QString("vtkMRMLAnnotation%1DisplayableManager").arg(name).toUtf8());
    }

  // 2D
  QStringList cjyxViewDisplayableManagers;
  cjyxViewDisplayableManagers
      << "Fiducial"
      << "Ruler"
      << "ROI"
      ;
  foreach(const QString& name, cjyxViewDisplayableManagers)
    {
    vtkMRMLSliceViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager(
        QString("vtkMRMLAnnotation%1DisplayableManager").arg(name).toUtf8());
    }

  /// Register IO
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  ioManager->registerIO(
    new qCjyxAnnotationsReader(vtkCjyxAnnotationModuleLogic::SafeDownCast(this->logic()), this));

  ioManager->registerIO(new qCjyxNodeWriter(
    "Annotations", QString("AnnotationFile"),
    QStringList() << "vtkMRMLAnnotationNode", true, this));

  // Register subject hierarchy plugin
#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    PythonQt::init();
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    context.evalScript( QString(
      "from SubjectHierarchyPlugins import AnnotationsSubjectHierarchyPlugin \n"
      "scriptedPlugin = cjyx.qCjyxSubjectHierarchyScriptedPlugin(None) \n"
      "scriptedPlugin.setPythonSource(AnnotationsSubjectHierarchyPlugin.filePath) \n"
      ) );
    }
#endif
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxAnnotationsModule::createWidgetRepresentation()
{
  return new qCjyxAnnotationModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxAnnotationsModule::createLogic()
{
  return vtkCjyxAnnotationModuleLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxAnnotationsModule::helpText() const
{
  QString help = QString(
  "Annotations module, create and edit supplementary information associated with a scene.<br>"
  "The module will soon be replaced by Markups module."
  "Currently supported annotations are fiducial points, rulers, and regions of interest (ROIs).<br>");
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxAnnotationsModule::acknowledgementText() const
{
  return "<img src=':/Icons/UPenn_logo.png'><br><br>This module was "
      "developed by Daniel Haehn, Kilian Pohl and Yong Zhang. "
      "Thank you to Nicole Aucoin, Wendy Plesniak, Steve Pieper, Ron Kikinis and Kitware. "
      "The research was funded by an ARRA supplement to NIH NCRR (P41 RR13218).";
}

//-----------------------------------------------------------------------------
QStringList qCjyxAnnotationsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  moduleContributors << QString("Daniel Haehn (UPenn)");
  moduleContributors << QString("Kilian Pohl (UPenn)");
  moduleContributors << QString("Yong Zhang (IBM)");
  moduleContributors << QString("Wendy Plesniak (SPL, BWH)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxAnnotationsModule::icon() const
{
  return QIcon(":/Icons/Annotation.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxAnnotationsModule::categories() const
{
  return QStringList() << "Legacy";
}

//-----------------------------------------------------------------------------
QStringList qCjyxAnnotationsModule::dependencies() const
{
  QStringList moduleDependencies;
  moduleDependencies << "SubjectHierarchy";
  return moduleDependencies;
}

//-----------------------------------------------------------------------------
void qCjyxAnnotationsModule::showScreenshotDialog()
{
  Q_ASSERT(this->widgetRepresentation());
  dynamic_cast<qCjyxAnnotationModuleWidget*>(this->widgetRepresentation())
      ->grabSnapShot();
}

//-----------------------------------------------------------------------------
QStringList qCjyxAnnotationsModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLAnnotationNode"
    << "vtkMRMLAnnotationDisplayNode"
    << "vtkMRMLAnnotationStorageNode"
    << "vtkMRMLAnnotationHierarchyNode";
}
