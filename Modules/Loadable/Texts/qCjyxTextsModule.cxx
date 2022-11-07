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

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include <qCjyxNodeWriter.h>

// Texts includes
#include "qCjyxTextsModule.h"
#include "qCjyxTextsModuleWidget.h"
#include "qCjyxTextsReader.h"
#include "vtkCjyxTextsLogic.h"

// VTK includes
#include "vtkSmartPointer.h"

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyPluginHandler.h"
#include "qCjyxSubjectHierarchyTextsPlugin.h"

//-----------------------------------------------------------------------------
class qCjyxTextsModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxTextsModule::qCjyxTextsModule(QObject* _parentObject)
  : Superclass(_parentObject)
  , d_ptr(new qCjyxTextsModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxTextsModule::~qCjyxTextsModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxTextsModule::icon()const
{
  return QIcon(":/Icons/CjyxTexts.png");
}


//-----------------------------------------------------------------------------
QStringList qCjyxTextsModule::categories() const
{
  return QStringList() << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTextsModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxTextsModule::createWidgetRepresentation()
{
  return new qCjyxTextsModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTextsModule::createLogic()
{
  return vtkCjyxTextsLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxTextsModule::helpText()const
{
  QString help = "A module to create, edit and manage text data in the scene.<br>";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxTextsModule::acknowledgementText()const
{
  QString acknowledgement =
    "This work was supported through CANARIE's Research Software Program, and Cancer Care Ontario.<br>"
    "The Texts module was contributed by Kyle Sunderland and Andras Lasso (Perk Lab, Queen's University)";
  return acknowledgement;
}

//-----------------------------------------------------------------------------
QStringList qCjyxTextsModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Kyle Sunderland (PerkLab, Queen's)");
  moduleContributors << QString("Andras Lasso (PerkLab, Queen's)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
void qCjyxTextsModule::setup()
{
  qCjyxApplication * app = qCjyxApplication::application();
  if (!app)
    {
    return;
    }

  qCjyxTextsReader* textFileReader = new qCjyxTextsReader(this);
  app->coreIOManager()->registerIO(textFileReader);
  app->coreIOManager()->registerIO(new qCjyxNodeWriter("TextFileImporter", textFileReader->fileType(), QStringList() << "vtkMRMLTextNode", false, this));

  // Register Subject Hierarchy core plugins
  qCjyxSubjectHierarchyPluginHandler::instance()->registerPlugin(new qCjyxSubjectHierarchyTextsPlugin());
}

//-----------------------------------------------------------------------------
QStringList qCjyxTextsModule::associatedNodeTypes() const
{
  return QStringList()
    << "vtkMRMLTextNode";
}
