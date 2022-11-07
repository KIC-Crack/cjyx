/*==============================================================================

  Copyright (c) The Intervention Centre
  Oslo University Hospital, Oslo, Norway. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Rafael Palomar (The Intervention Centre,
  Oslo University Hospital) and was supported by The Research Council of Norway
  through the ALive project (grant nr. 311393).

==============================================================================*/

#include "qCjyxTemplateKeyModule.h"


// Qt includes
#include <QDebug>

// TemplateKey Logic includes
#include "vtkCjyxTemplateKeyLogic.h"

// TemplateKey Widgets includes
#include "qMRMLMarkupsTestLineWidget.h"

// Markups Widgets includes
#include "qMRMLMarkupsOptionsWidgetsFactory.h"

#include <qCjyxModuleManager.h>
#include <qCjyxApplication.h>

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
bool qCjyxTemplateKeyModule::isHidden() const
{
  // The module has no GUI.
  // Widget options will be shown in Markups module.
  return true;
}

//-----------------------------------------------------------------------------
QString qCjyxTemplateKeyModule::helpText() const
{
  return "This module contains fundamental markups to be used in the Cjyx-Liver extension.";
}

//-----------------------------------------------------------------------------
QString qCjyxTemplateKeyModule::acknowledgementText() const
{
  return "This work has been partially funded by The Research Council of Norway (grant nr. 311393)";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTemplateKeyModule::contributors() const
{
  QStringList moduleContributors;
  moduleContributors << QString("Rafael Palomar (Oslo University Hospital / NTNU)");
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
  return QStringList() << "Testing.TestCases";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTemplateKeyModule::dependencies() const
{
  return QStringList() << "Markups";
}

//-----------------------------------------------------------------------------
void qCjyxTemplateKeyModule::setup()
{
  this->Superclass::setup();

  // This is a test class, therefore we do not register anything if
  // not in testing mode (to avoid cluttering the markups module).
  bool isTestingEnabled = qCjyxApplication::testAttribute(qCjyxCoreApplication::AA_EnableTesting);
  if (!isTestingEnabled)
    {
    return;
    }

  // Create and configure the options widgets
  auto optionsWidgetFactory = qMRMLMarkupsOptionsWidgetsFactory::instance();
  optionsWidgetFactory->registerOptionsWidget(new qMRMLMarkupsTestLineWidget());
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxTemplateKeyModule::createWidgetRepresentation()
{
  return nullptr;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTemplateKeyModule::createLogic()
{
  // This is a test class, therefore we do not register anything (to avoid cluttering the markups module)
  // unless the application is in testing mode.
  bool isTestingEnabled = qCjyxApplication::testAttribute(qCjyxCoreApplication::AA_EnableTesting);
  if (!isTestingEnabled)
    {
    return nullptr;
    }

  return vtkCjyxTemplateKeyLogic::New();
}
