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
#include <QDebug>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxIOManager.h"
#include "qCjyxModuleManager.h"

// Data includes
#include "qCjyxDataDialog.h"
#include "qCjyxDataModule.h"
#include "qCjyxDataModuleWidget.h"
#include "qCjyxSaveDataDialog.h"
#include "qCjyxExportNodeDialog.h"
#include "qCjyxSceneBundleReader.h"
#include "qCjyxSceneReader.h"
#include "qCjyxSceneWriter.h"

// CjyxLogic includes
#include <vtkCjyxApplicationLogic.h>

// Data Logic includes
#include "vtkCjyxDataModuleLogic.h"

// Logic includes
#include <vtkCjyxCamerasModuleLogic.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxDataModulePrivate
{
};

//-----------------------------------------------------------------------------
qCjyxDataModule::qCjyxDataModule(QObject* parentObject)
  :Superclass(parentObject)
  , d_ptr(new qCjyxDataModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxDataModule::~qCjyxDataModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxDataModule::icon()const
{
  return QIcon(":/Icons/SubjectHierarchy.png");
}

//-----------------------------------------------------------------------------
QStringList qCjyxDataModule::categories() const
{
  return QStringList() << "" << "Informatics";
}

//-----------------------------------------------------------------------------
QStringList qCjyxDataModule::dependencies() const
{
  QStringList moduleDependencies;
  // Cameras: Required in qCjyxSceneReader
  moduleDependencies << "Cameras";
  return moduleDependencies;
}

//-----------------------------------------------------------------------------
void qCjyxDataModule::setup()
{
  Q_D(const qCjyxDataModule);

  this->Superclass::setup();

  vtkCjyxCamerasModuleLogic* camerasLogic =
    vtkCjyxCamerasModuleLogic::SafeDownCast(this->moduleLogic("Cameras"));
  // NOTE: here we assume that camerasLogic with a nullptr value can be passed
  // to the qCjyxSceneReader. Therefore we trigger a warning but don't return
  // immediately.
  if (!camerasLogic)
    {
    qCritical() << Q_FUNC_INFO << ": Cameras module is not found";
    }

  qCjyxIOManager* ioManager = qCjyxApplication::application()->ioManager();

  // Readers
  ioManager->registerIO(new qCjyxSceneReader(camerasLogic, this));
  ioManager->registerIO(new qCjyxSceneBundleReader(this));

  // Writers
  ioManager->registerIO(new qCjyxSceneWriter(this));

  // Dialogs
  ioManager->registerDialog(new qCjyxDataDialog(this));
  ioManager->registerDialog(new qCjyxSaveDataDialog(this));
  ioManager->registerDialog(new qCjyxExportNodeDialog(this));
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxDataModule::createWidgetRepresentation()
{
  qCjyxDataModuleWidget *widget = new qCjyxDataModuleWidget;
  return widget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxDataModule::createLogic()
{
  return vtkCjyxDataModuleLogic::New();
}

//-----------------------------------------------------------------------------
QString qCjyxDataModule::helpText()const
{
  QString help = QString(
    "The Data module is the central data-organizing point where all loaded data is "
    "presented for access and manipulation is the Data module. It allows organizing "
    "the data in folders or patient/study trees (automatically done for DICOM), "
    "visualizing any displayable data, transformation of whole branches, and a "
    "multitude of data type specific features.");
  help += this->defaultDocumentationLink();
  return help;
}

//-----------------------------------------------------------------------------
QString qCjyxDataModule::acknowledgementText()const
{
  QString about =
    "<center><table border=\"0\"><tr>"
    "<td><img src=\":Logos/NAMIC.png\" alt\"NA-MIC\"></td>"
    "<td><img src=\":Logos/NAC.png\" alt\"NAC\"></td>"
    "</tr><tr>"
    "<td><img src=\":Logos/BIRN-NoText.png\" alt\"BIRN\"></td>"
    "<td><img src=\":Logos/NCIGT.png\" alt\"NCIGT\"></td>"
    "</tr></table></center>"
    "This work was supported by NA-MIC, NAC, BIRN, NCIGT, CTSC, and the Cjyx "
    "Community.";
  return about;
}

//-----------------------------------------------------------------------------
QStringList qCjyxDataModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Csaba Pinter (Queen's)");
  moduleContributors << QString("Julien Finet (Kitware)");
  moduleContributors << QString("Alex Yarmarkovich (Isomics)");
  moduleContributors << QString("Nicole Aucoin (SPL, BWH)");
  return moduleContributors;
}
