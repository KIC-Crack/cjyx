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

// TemplateKey Logic includes
#include <vtkCjyxTemplateKeyLogic.h>

// TemplateKey includes
#include "qCjyxTemplateKeyModule.h"
#include "qCjyxTemplateKeyModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxTemplateKeyModulePrivate
{
public:
  qCjyxTemplateKeyModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxTemplateKeyModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModulePrivate::qCjyxTemplateKeyModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qCjyxTemplateKeyModule methods

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModule::qCjyxTemplateKeyModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxTemplateKeyModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModule::~qCjyxTemplateKeyModule()
{
}

//-----------------------------------------------------------------------------
QString qCjyxTemplateKeyModule::helpText() const
{
  return "This is a loadable module that can be bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qCjyxTemplateKeyModule::acknowledgementText() const
{
  return "This work was partially funded by NIH grant NXNNXXNNNNNN-NNXN";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTemplateKeyModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("John Doe (AnyWare Corp.)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qCjyxTemplateKeyModule::icon() const
{
  return QIcon(":/Icons/TemplateKey.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxTemplateKeyModule::categories() const
{
  return QStringList() << "Examples";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTemplateKeyModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxTemplateKeyModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxTemplateKeyModule
::createWidgetRepresentation()
{
  return new qCjyxTemplateKeyModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTemplateKeyModule::createLogic()
{
  return vtkCjyxTemplateKeyLogic::New();
}
