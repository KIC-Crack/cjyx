/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Terminologies includes
#include "qCjyxTerminologiesModule.h"
#include "qCjyxTerminologiesModuleWidget.h"
#include "qCjyxTerminologiesReader.h"
#include "vtkCjyxTerminologiesModuleLogic.h"

// Qt includes
#include <QDebug>
#include <QDir>

// Cjyx includes
#include <qCjyxApplication.h>
#include <qCjyxModuleManager.h>
#include "qCjyxIOManager.h"

//-----------------------------------------------------------------------------
/// \ingroup CjyxRt_QtModules_Terminologies
class qCjyxTerminologiesModulePrivate
{
public:
  qCjyxTerminologiesModulePrivate();
};

//-----------------------------------------------------------------------------
// qCjyxTerminologiesModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxTerminologiesModulePrivate::qCjyxTerminologiesModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxTerminologiesModule methods

//-----------------------------------------------------------------------------
qCjyxTerminologiesModule::qCjyxTerminologiesModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxTerminologiesModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxTerminologiesModule::~qCjyxTerminologiesModule() = default;

//-----------------------------------------------------------------------------
QString qCjyxTerminologiesModule::helpText()const
{
  QString help =
    "The Terminologies module enables viewing and editing terminology dictionaries used for segmentation.";
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxTerminologiesModule::acknowledgementText()const
{
  return "This work is part of SparKit project, funded by Cancer Care Ontario (CCO)'s ACRU program and Ontario Consortium for Adaptive Interventions in Radiation Oncology (OCAIRO).";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTerminologiesModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Csaba Pinter (Queen's)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QStringList qCjyxTerminologiesModule::categories()const
{
  return QStringList() << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxTerminologiesModule::dependencies()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qCjyxTerminologiesModule::setup()
{
  this->Superclass::setup();

  vtkCjyxTerminologiesModuleLogic* terminologiesLogic = vtkCjyxTerminologiesModuleLogic::SafeDownCast(this->logic());

  // Register IOs
  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();
  ioManager->registerIO(new qCjyxTerminologiesReader(terminologiesLogic, this));
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxTerminologiesModule::createWidgetRepresentation()
{
  return new qCjyxTerminologiesModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxTerminologiesModule::createLogic()
{
  // Determine the settings folder path
  QDir settingsDir(qCjyxCoreApplication::application()->cjyxUserSettingsFilePath());
  settingsDir.cdUp(); // Remove Cjyx.ini from the path
  QString settingsDirPath = settingsDir.absolutePath();
  settingsDirPath.append("/Terminologies");

  // Setup logic
  vtkCjyxTerminologiesModuleLogic* logic = vtkCjyxTerminologiesModuleLogic::New();
  logic->SetUserContextsPath(settingsDirPath.toUtf8().constData());

  return logic;
}
