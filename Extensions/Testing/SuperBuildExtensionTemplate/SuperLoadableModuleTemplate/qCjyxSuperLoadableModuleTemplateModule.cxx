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

// SuperLoadableModuleTemplate Logic includes
#include <vtkCjyxSuperLoadableModuleTemplateLogic.h>

// SuperLoadableModuleTemplate includes
#include "qCjyxSuperLoadableModuleTemplateModule.h"
#include "qCjyxSuperLoadableModuleTemplateModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxSuperLoadableModuleTemplateModulePrivate
{
public:
  qCjyxSuperLoadableModuleTemplateModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxSuperLoadableModuleTemplateModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModulePrivate::qCjyxSuperLoadableModuleTemplateModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qCjyxSuperLoadableModuleTemplateModule methods

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModule::qCjyxSuperLoadableModuleTemplateModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSuperLoadableModuleTemplateModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModule::~qCjyxSuperLoadableModuleTemplateModule()
{
}

//-----------------------------------------------------------------------------
QString qCjyxSuperLoadableModuleTemplateModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qCjyxSuperLoadableModuleTemplateModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSuperLoadableModuleTemplateModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxSuperLoadableModuleTemplateModule::icon() const
{
  return QIcon(":/Icons/SuperLoadableModuleTemplate.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxSuperLoadableModuleTemplateModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qCjyxSuperLoadableModuleTemplateModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxSuperLoadableModuleTemplateModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxSuperLoadableModuleTemplateModule
::createWidgetRepresentation()
{
  return new qCjyxSuperLoadableModuleTemplateModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxSuperLoadableModuleTemplateModule::createLogic()
{
  return vtkCjyxSuperLoadableModuleTemplateLogic::New();
}
