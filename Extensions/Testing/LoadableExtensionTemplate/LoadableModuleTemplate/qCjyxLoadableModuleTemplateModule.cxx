/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// LoadableModuleTemplate Logic includes
#include <vtkCjyxLoadableModuleTemplateLogic.h>

// LoadableModuleTemplate includes
#include "qCjyxLoadableModuleTemplateModule.h"
#include "qCjyxLoadableModuleTemplateModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxLoadableModuleTemplateModulePrivate
{
public:
  qCjyxLoadableModuleTemplateModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxLoadableModuleTemplateModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxLoadableModuleTemplateModulePrivate::qCjyxLoadableModuleTemplateModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qCjyxLoadableModuleTemplateModule methods

//-----------------------------------------------------------------------------
qCjyxLoadableModuleTemplateModule::qCjyxLoadableModuleTemplateModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxLoadableModuleTemplateModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxLoadableModuleTemplateModule::~qCjyxLoadableModuleTemplateModule()
{
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModuleTemplateModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qCjyxLoadableModuleTemplateModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qCjyxLoadableModuleTemplateModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxLoadableModuleTemplateModule::icon() const
{
  return QIcon(":/Icons/LoadableModuleTemplate.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxLoadableModuleTemplateModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qCjyxLoadableModuleTemplateModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxLoadableModuleTemplateModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxLoadableModuleTemplateModule
::createWidgetRepresentation()
{
  return new qCjyxLoadableModuleTemplateModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxLoadableModuleTemplateModule::createLogic()
{
  return vtkCjyxLoadableModuleTemplateLogic::New();
}
