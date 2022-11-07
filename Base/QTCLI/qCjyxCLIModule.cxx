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

#include "qCjyxCLIModule.h"

// Qt includes
#include <QDebug>
#include <QSettings>

// CTK includes
#include <ctkWidgetsUtils.h>

// Cjyx includes
#include "qMRMLNodeComboBox.h"
#include "qCjyxCLIModuleWidget.h"
#include "vtkCjyxCLIModuleLogic.h"

// CjyxExecutionModel includes
#include <ModuleDescription.h>
#include <ModuleDescriptionParser.h>
#include <ModuleLogo.h>

//-----------------------------------------------------------------------------
class qCjyxCLIModulePrivate
{
public:
  typedef qCjyxCLIModulePrivate Self;
  qCjyxCLIModulePrivate();

  QString           TempDirectory;

  ModuleDescription                 Desc;
};

//-----------------------------------------------------------------------------
// qCjyxCLIModulePrivate methods

//-----------------------------------------------------------------------------
qCjyxCLIModulePrivate::qCjyxCLIModulePrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxCLIModule methods

//-----------------------------------------------------------------------------
qCjyxCLIModule::qCjyxCLIModule(QWidget* _parent):Superclass(_parent)
  , d_ptr(new qCjyxCLIModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxCLIModule::~qCjyxCLIModule() = default;

//-----------------------------------------------------------------------------
void qCjyxCLIModule::setup()
{
#ifndef QT_NO_DEBUG
  Q_D(qCjyxCLIModule);
  // Temporary directory should be set before the module is initialized
  Q_ASSERT(!d->TempDirectory.isEmpty());
#endif
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation * qCjyxCLIModule::createWidgetRepresentation()
{
  return new qCjyxCLIModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxCLIModule::createLogic()
{
  Q_D(qCjyxCLIModule);
  vtkCjyxCLIModuleLogic* logic = vtkCjyxCLIModuleLogic::New();
  logic->SetDefaultModuleDescription(d->Desc);

  // In developer mode keep the CLI modules input and output files
  QSettings settings;
  bool developerModeEnabled = settings.value("Developer/PreserveCLIModuleDataFiles", false).toBool();
  if (developerModeEnabled)
    {
    logic->DeleteTemporaryFilesOff();
    }

  if (d->Desc.GetParameterValue("AllowInMemoryTransfer") == "false")
    {
    logic->SetAllowInMemoryTransfer(0);
    }

  return logic;
}

//-----------------------------------------------------------------------------
QString qCjyxCLIModule::title()const
{
  Q_D(const qCjyxCLIModule);
  return QString::fromStdString(d->Desc.GetTitle());
}

//-----------------------------------------------------------------------------
QStringList qCjyxCLIModule::categories()const
{
  Q_D(const qCjyxCLIModule);
  return QString::fromStdString(d->Desc.GetCategory()).split(';');
}

//-----------------------------------------------------------------------------
QStringList qCjyxCLIModule::contributors()const
{
  Q_D(const qCjyxCLIModule);
  return QStringList() << QString::fromStdString(d->Desc.GetContributor());
}

//-----------------------------------------------------------------------------
int qCjyxCLIModule::index()const
{
  Q_D(const qCjyxCLIModule);
  bool ok = false;
  int index = QString::fromStdString(d->Desc.GetIndex()).toInt(&ok);
  return (ok ? index : -1);
}

//-----------------------------------------------------------------------------
QString qCjyxCLIModule::acknowledgementText()const
{
  Q_D(const qCjyxCLIModule);
  return QString::fromStdString(d->Desc.GetAcknowledgements());
}

//-----------------------------------------------------------------------------
QImage qCjyxCLIModule::logo()const
{
  Q_D(const qCjyxCLIModule);
  return this->moduleLogoToImage(d->Desc.GetLogo());
}

//-----------------------------------------------------------------------------
QString qCjyxCLIModule::helpText()const
{
  Q_D(const qCjyxCLIModule);
  QString help = QString::fromStdString(d->Desc.GetDescription());
  if (!d->Desc.GetDocumentationURL().empty())
    {
    help += QString("<p>For more information see the <a href=\"%1\">online documentation</a>.</p>")
      .arg(QString::fromStdString(d->Desc.GetDocumentationURL()));
    }
  return help;
}

//-----------------------------------------------------------------------------
CTK_SET_CPP(qCjyxCLIModule, const QString&, setTempDirectory, TempDirectory);
CTK_GET_CPP(qCjyxCLIModule, QString, tempDirectory, TempDirectory);

//-----------------------------------------------------------------------------
void qCjyxCLIModule::setEntryPoint(const QString& entryPoint)
{
  Q_D(qCjyxCLIModule);
  d->Desc.SetTarget(entryPoint.toStdString());
}

//-----------------------------------------------------------------------------
QString qCjyxCLIModule::entryPoint()const
{
  Q_D(const qCjyxCLIModule);
  return QString::fromStdString(d->Desc.GetTarget());
}

//-----------------------------------------------------------------------------
void qCjyxCLIModule::setModuleType(const QString& moduleType)
{
  Q_D(qCjyxCLIModule);
  d->Desc.SetType(moduleType.toStdString());
}

//-----------------------------------------------------------------------------
QString qCjyxCLIModule::moduleType()const
{
  Q_D(const qCjyxCLIModule);
  return QString::fromStdString(d->Desc.GetType());
}

//-----------------------------------------------------------------------------
void qCjyxCLIModule::setXmlModuleDescription(const QString& xmlModuleDescription)
{
  Q_D(qCjyxCLIModule);
  //qDebug() << "xmlModuleDescription:" << xmlModuleDescription;
  Q_ASSERT(!this->entryPoint().isEmpty());

  // Parse module description
  ModuleDescriptionParser parser;
  if (parser.Parse(xmlModuleDescription.toStdString(), d->Desc) != 0)
    {
    qWarning() << "Failed to parse xml module description:\n"
               << xmlModuleDescription;
    return;
    }

  // Set properties

  // Register the module description in the master list
  vtkMRMLCommandLineModuleNode::RegisterModuleDescription(d->Desc);
}

//-----------------------------------------------------------------------------
vtkCjyxCLIModuleLogic* qCjyxCLIModule::cliModuleLogic()
{
  vtkCjyxCLIModuleLogic* myLogic = vtkCjyxCLIModuleLogic::SafeDownCast(this->logic());
  return myLogic;
}

//-----------------------------------------------------------------------------
void qCjyxCLIModule::setLogo(const ModuleLogo& logo)
{
  Q_D(qCjyxCLIModule);
  d->Desc.SetLogo(logo);
}

//-----------------------------------------------------------------------------
QImage qCjyxCLIModule::moduleLogoToImage(const ModuleLogo& logo)
{
  if (logo.GetBufferLength() == 0)
    {
    return QImage();
    }
  return ctk::kwIconToQImage(reinterpret_cast<const unsigned char*>(logo.GetLogo()),
                             logo.GetWidth(), logo.GetHeight(),
                             logo.GetPixelSize(), logo.GetBufferLength(),
                             logo.GetOptions());
}

//-----------------------------------------------------------------------------
ModuleDescription& qCjyxCLIModule::moduleDescription()
{
  Q_D(qCjyxCLIModule);
  return d->Desc;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCLIModule::associatedNodeTypes()const
{
  return QStringList() << "vtkMRMLCommandLineModuleNode";
}
