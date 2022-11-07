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

// Cjyx includes
#include "qCjyxEventBrokerModule.h"
#include "qCjyxEventBrokerModuleWidget.h"

//-----------------------------------------------------------------------------
class qCjyxEventBrokerModulePrivate
{
public:
};

//-----------------------------------------------------------------------------
qCjyxEventBrokerModule::qCjyxEventBrokerModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxEventBrokerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxEventBrokerModule::~qCjyxEventBrokerModule() = default;

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxEventBrokerModule::createWidgetRepresentation()
{
  return new qCjyxEventBrokerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxEventBrokerModule::createLogic()
{
  return nullptr;
}

//-----------------------------------------------------------------------------
QStringList qCjyxEventBrokerModule::categories()const
{
  return QStringList() << "Developer Tools";
}

//-----------------------------------------------------------------------------
QString qCjyxEventBrokerModule::helpText()const
{
  QString help = "Profiling tool for the developers.";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxEventBrokerModule::acknowledgementText()const
{
  QString acknowledgement = "This module was developed by Julien Finet, Kitware Inc. "
      "This work was supported by NIH grant 3P41RR013218-12S1, "
      "NA-MIC, NAC and Cjyx community.";
  return acknowledgement;
}

//-----------------------------------------------------------------------------
QStringList qCjyxEventBrokerModule::contributors()const
{
  QStringList contributors;
  contributors << QString("Julien Finet (Kitware)");
  return contributors;
}
