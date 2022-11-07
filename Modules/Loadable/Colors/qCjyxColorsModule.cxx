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

// Qt includes
#include <QSettings>

// CTK includes
#include <ctkColorDialog.h>

#include <vtkMRMLSliceViewDisplayableManagerFactory.h>
#include <vtkMRMLThreeDViewDisplayableManagerFactory.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include "qCjyxNodeWriter.h"

// Colors includes
#include "qCjyxColorsModule.h"
#include "qCjyxColorsModuleWidget.h"
#include "qCjyxColorsReader.h"

// qMRML includes
#include <qMRMLColorPickerWidget.h>

// Cjyx Logic includes
#include <vtkCjyxApplicationLogic.h>
#include "vtkCjyxColorLogic.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyColorLegendPlugin.h"

// DisplayableManager initialization
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkCjyxColorsModuleMRMLDisplayableManager)

//-----------------------------------------------------------------------------
class qCjyxColorsModulePrivate
{
public:
  qCjyxColorsModulePrivate();
  QSharedPointer<qMRMLColorPickerWidget> ColorDialogPickerWidget;
};

//-----------------------------------------------------------------------------
qCjyxColorsModulePrivate::qCjyxColorsModulePrivate()
{
  this->ColorDialogPickerWidget =
    QSharedPointer<qMRMLColorPickerWidget>(new qMRMLColorPickerWidget(nullptr));
}

//-----------------------------------------------------------------------------
qCjyxColorsModule::qCjyxColorsModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxColorsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxColorsModule::~qCjyxColorsModule() = default;

//-----------------------------------------------------------------------------
QStringList qCjyxColorsModule::categories()const
{
  return QStringList() << "Informatics";
}

//-----------------------------------------------------------------------------
QIcon qCjyxColorsModule::icon()const
{
  return QIcon(":/Icons/Colors.png");
}

//-----------------------------------------------------------------------------
void qCjyxColorsModule::setup()
{
  Q_D(qCjyxColorsModule);

  // DisplayableManager initialization
  // Register color legend displayable manager for slice and 3D views
  vtkMRMLThreeDViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager(
    "vtkMRMLColorLegendDisplayableManager");
  vtkMRMLSliceViewDisplayableManagerFactory::GetInstance()->RegisterDisplayableManager(
    "vtkMRMLColorLegendDisplayableManager");

  qCjyxApplication * app = qCjyxApplication::application();
  if (!app)
    {
    return;
    }
  vtkCjyxColorLogic* colorLogic = vtkCjyxColorLogic::SafeDownCast(this->logic());
  if (this->appLogic() != nullptr)
    {
    this->appLogic()->SetColorLogic(colorLogic);
    }
  app->coreIOManager()->registerIO(
    new qCjyxColorsReader(colorLogic, this));
  app->coreIOManager()->registerIO(new qCjyxNodeWriter(
    "Colors", QString("ColorTableFile"),
    QStringList() << "vtkMRMLColorNode", true, this));

  QStringList paths = qCjyxCoreApplication::application()->toCjyxHomeAbsolutePaths(
    app->userSettings()->value("QTCoreModules/Colors/ColorFilePaths").toStringList());
#ifdef Q_OS_WIN32
  QString joinedPaths = paths.join(";");
#else
  QString joinedPaths = paths.join(":");
#endif
  // Warning: If the logic has already created the color nodes (AddDefaultColorNodes),
  // setting the user color file paths doesn't trigger any action to add new nodes.
  // It's something that must be fixed into the logic, not here
  colorLogic->SetUserColorFilePaths(joinedPaths.toUtf8());

  // Color picker
  d->ColorDialogPickerWidget->setMRMLColorLogic(colorLogic);
  ctkColorDialog::addDefaultTab(d->ColorDialogPickerWidget.data(),
                                "Labels", SIGNAL(colorSelected(QColor)),
                                SIGNAL(colorNameSelected(QString)));
  ctkColorDialog::setDefaultTab(1);

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyColorLegendPlugin* colorLegendPlugin = new qCjyxSubjectHierarchyColorLegendPlugin();
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(colorLegendPlugin);
}

//-----------------------------------------------------------------------------
void qCjyxColorsModule::setMRMLScene(vtkMRMLScene* scene)
{
  Q_D(qCjyxColorsModule);
  /// tbd: might be set too late ?
  d->ColorDialogPickerWidget->setMRMLScene(scene);
  this->Superclass::setMRMLScene(scene);
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxColorsModule::createWidgetRepresentation()
{
  return new qCjyxColorsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxColorsModule::createLogic()
{
  return vtkCjyxColorLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxColorsModule::helpText()const
{
  QString help =
    "The <b>Colors Module</b> manages color look up tables, stored in Color nodes.<br>"
    "These tables translate between a numeric value and a color "
    "for displaying of various data types, such as volumes and models.<br>"
    "Two lookup table types are available:<br>"
    "<ul>"
    "<li>Discrete table: List of named colors are specified (example: GenericAnatomyColors). "
    "Discrete tables can be used for continuous mapping as well, in this case the colors "
    "are used as samples at equal distance within the specified range, and smoothly "
    "interpolating between them (example: Grey).</li>"
    "<li>Continuous scale: Color is specified for arbitrarily chosen numerical values "
    "and color value can be computed by smoothly interpolating between these values "
    "(example: PET-DICOM). No names are specified for colors.</li>"
    "All built-in color tables are read-only. To edit colors, create a copy "
    "of the color table by clicking on the 'copy' folder icon.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxColorsModule::acknowledgementText()const
{
  QString about =
    "This work was supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community.";
  return about;
}

//-----------------------------------------------------------------------------
QStringList qCjyxColorsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Ron Kikinis (SPL, BWH)");
  moduleContributors << QString("Mikhail Polkovnikov (IHEP)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
bool qCjyxColorsModule::isHidden()const
{
  return false;
}

//-----------------------------------------------------------------------------
QStringList qCjyxColorsModule::associatedNodeTypes() const
{
  return QStringList() << "vtkMRMLColorNode";
}
