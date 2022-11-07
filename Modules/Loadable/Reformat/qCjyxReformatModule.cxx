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

  This file was originally developed by Michael Jeulin-Lagarrigue, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// QTGUI includes
#include "qCjyxApplication.h"

// Reformat Logic includes
#include <vtkCjyxReformatLogic.h>

// Reformat includes
#include "qCjyxReformatModule.h"
#include "qCjyxReformatModuleWidget.h"

//------------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Reformat
class qCjyxReformatModulePrivate
{
public:
  qCjyxReformatModulePrivate();
};

//------------------------------------------------------------------------------
// qCjyxReformatModulePrivate methods

//------------------------------------------------------------------------------
qCjyxReformatModulePrivate::qCjyxReformatModulePrivate() = default;

//------------------------------------------------------------------------------
// qCjyxReformatModule methods

//------------------------------------------------------------------------------
qCjyxReformatModule::
qCjyxReformatModule(QObject* _parent) : Superclass(_parent),
  d_ptr(new qCjyxReformatModulePrivate)
{
}

//------------------------------------------------------------------------------
qCjyxReformatModule::~qCjyxReformatModule() = default;

//------------------------------------------------------------------------------
QString qCjyxReformatModule::helpText()const
{
  QString help =
      "The Transforms Reformat Widget Module creates "
      "and edits the Slice Node transforms.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//------------------------------------------------------------------------------
QString qCjyxReformatModule::acknowledgementText()const
{
  QString acknowledgement =
    "This work was supported by NA-MIC, NAC, BIRN, NCIGT, and the Cjyx Community.";
  return acknowledgement;
}

//------------------------------------------------------------------------------
QIcon qCjyxReformatModule::icon()const
{
  return QIcon(":/Icons/Reformat.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxReformatModule::categories()const
{
  return QStringList() << "Registration.Specialized";
}

//-----------------------------------------------------------------------------
QStringList qCjyxReformatModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Michael Jeulin-Lagarrigue (Kitware)");
  return moduleContributors;
}

//------------------------------------------------------------------------------
void qCjyxReformatModule::setup()
{
  this->Superclass::setup();
}

//------------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxReformatModule::
createWidgetRepresentation()
{
  return new qCjyxReformatModuleWidget;
}

//------------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxReformatModule::createLogic()
{
  return vtkCjyxReformatLogic::New();
}

//-----------------------------------------------------------------------------
QStringList qCjyxReformatModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLSliceNode"
    << "vtkMRMLSliceCompositeNode";
}
