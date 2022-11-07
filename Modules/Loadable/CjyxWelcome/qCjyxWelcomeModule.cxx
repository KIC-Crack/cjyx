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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Cjyx includes
#include "qCjyxWelcomeModule.h"
#include "qCjyxWelcomeModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CjyxWelcome
class qCjyxWelcomeModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxWelcomeModule::qCjyxWelcomeModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxWelcomeModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxWelcomeModule::~qCjyxWelcomeModule() = default;

//-----------------------------------------------------------------------------
QStringList qCjyxWelcomeModule::categories()const
{
  return QStringList() << "";
}

//-----------------------------------------------------------------------------
QIcon qCjyxWelcomeModule::icon()const
{
  return QIcon(":/Icons/CjyxWelcome.png");
}

//-----------------------------------------------------------------------------
QString qCjyxWelcomeModule::helpText()const
{
  return QString();
}

//-----------------------------------------------------------------------------
QString qCjyxWelcomeModule::acknowledgementText()const
{
  return "This work was supported by NA-MIC, NAC, BIRN, NCIGT, CTSC and the Cjyx Community. "
      "See <a href=\"https://www.slicer.org\">https://www.slicer.org</a> for details. We would also like to express our sincere "
      "thanks to members of the Cjyx User Community who have helped us to design the contents "
      "of this Welcome Module, and whose feedback continues to improve functionality, usability "
      "and Cjyx user experience\n.";
}

//-----------------------------------------------------------------------------
QStringList qCjyxWelcomeModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Wendy Plesniak (SPL, BWH)");
  moduleContributors << QString("Jean-Christophe Fillion-Robin (Kitware)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxWelcomeModule::createWidgetRepresentation()
{
  return new qCjyxWelcomeModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxWelcomeModule::createLogic()
{
  return nullptr;
}
