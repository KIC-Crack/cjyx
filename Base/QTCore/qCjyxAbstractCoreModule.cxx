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

// Qt includes
#include <QDebug>
#include <QList>

// Cjyx includes
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxAbstractModuleRepresentation.h"
#include "qCjyxCoreApplication.h"

// CjyxLogic includes
#include "vtkCjyxModuleLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxAbstractCoreModulePrivate
{
public:
  qCjyxAbstractCoreModulePrivate();
  virtual ~qCjyxAbstractCoreModulePrivate();

  bool                                       Hidden;
  QString                                    Name;
  QString                                    Path;
  bool                                       Installed;
  bool                                       BuiltIn;
  bool                                       WidgetRepresentationCreationEnabled;
  qCjyxAbstractModuleRepresentation*       WidgetRepresentation;
  QList<qCjyxAbstractModuleRepresentation*> WidgetRepresentations;
  vtkSmartPointer<vtkMRMLScene>              MRMLScene;
  vtkSmartPointer<vtkCjyxApplicationLogic> AppLogic;
  vtkSmartPointer<vtkMRMLAbstractLogic>      Logic;
};

//-----------------------------------------------------------------------------
// qCjyxAbstractCoreModulePrivate methods
qCjyxAbstractCoreModulePrivate::qCjyxAbstractCoreModulePrivate()
{
  this->Hidden = false;
  this->Name = "NA";
  this->WidgetRepresentation = nullptr;
  this->Installed = false;
  this->BuiltIn = true;
  this->WidgetRepresentationCreationEnabled = true;
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModulePrivate::~qCjyxAbstractCoreModulePrivate()
{
  // Delete the widget representation
  if (this->WidgetRepresentation)
    {
    delete this->WidgetRepresentation;
    }
  qDeleteAll(this->WidgetRepresentations.begin(), this->WidgetRepresentations.end());
  this->WidgetRepresentations.clear();
}

//-----------------------------------------------------------------------------
// qCjyxAbstractCoreModule methods

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule::qCjyxAbstractCoreModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxAbstractCoreModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule::~qCjyxAbstractCoreModule() = default;

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::initialize(vtkCjyxApplicationLogic* _appLogic)
{
  this->setAppLogic(_appLogic);
  this->logic(); // Create the logic if it hasn't been created already.
  this->setup(); // Setup is a virtual pure method overloaded in subclass
}

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::printAdditionalInfo()
{
}

//-----------------------------------------------------------------------------
QString qCjyxAbstractCoreModule::name()const
{
  Q_D(const qCjyxAbstractCoreModule);
  return d->Name;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::setName(const QString& _name)
{
  Q_D(qCjyxAbstractCoreModule);
  d->Name = _name;
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractCoreModule::categories()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractCoreModule::contributors()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
int qCjyxAbstractCoreModule::index()const
{
  return -1;
}

//-----------------------------------------------------------------------------
QString qCjyxAbstractCoreModule::helpText()const
{
  return QString();
}

//-----------------------------------------------------------------------------
QString qCjyxAbstractCoreModule::acknowledgementText()const
{
  return QString();
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, vtkMRMLScene*, mrmlScene, MRMLScene);

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::setMRMLScene(vtkMRMLScene* _mrmlScene)
{
  Q_D(qCjyxAbstractCoreModule);
  if (d->MRMLScene == _mrmlScene)
    {
    return;
    }
  d->MRMLScene = _mrmlScene;
  // Since we don't want 'setMRMLScene' to instantiate explicitly the logic,
  // we just check the pointer (instead of calling 'this->logic()')
  if (d->Logic)
    {// logic should be updated first (because it doesn't depends on the widget
    d->Logic->SetMRMLScene(_mrmlScene);
    }
  if (d->WidgetRepresentation)
    {
    d->WidgetRepresentation->setMRMLScene(_mrmlScene);
    }
}

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::setAppLogic(vtkCjyxApplicationLogic* newAppLogic)
{
  Q_D(qCjyxAbstractCoreModule);
  d->AppLogic = newAppLogic;
  // here we don't want to create a logic if no logic exists yet. it's not setAppLogic
  // role to create logics.
  vtkCjyxModuleLogic* moduleLogic =
    vtkCjyxModuleLogic::SafeDownCast(d->Logic);
  if (moduleLogic)
    {
    moduleLogic->SetMRMLApplicationLogic(newAppLogic);
    }
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, vtkCjyxApplicationLogic*, appLogic, AppLogic);

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxAbstractCoreModule::moduleLogic(const QString& moduleName) const
{
  if (!this->appLogic())
    {
    return nullptr;
    }
  return this->appLogic()->GetModuleLogic(moduleName.toUtf8());
}

//-----------------------------------------------------------------------------
bool qCjyxAbstractCoreModule::isHidden()const
{
  return this->isWidgetRepresentationCreationEnabled() ? false : true;
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractCoreModule::dependencies()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, QString, path, Path);
CTK_SET_CPP(qCjyxAbstractCoreModule, const QString&, setPath, Path);

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, bool, isInstalled, Installed);
CTK_SET_CPP(qCjyxAbstractCoreModule, bool, setInstalled, Installed);

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, bool, isBuiltIn, BuiltIn);
CTK_SET_CPP(qCjyxAbstractCoreModule, bool, setBuiltIn, BuiltIn);

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxAbstractCoreModule, bool, isWidgetRepresentationCreationEnabled, WidgetRepresentationCreationEnabled);
CTK_SET_CPP(qCjyxAbstractCoreModule, bool, setWidgetRepresentationCreationEnabled, WidgetRepresentationCreationEnabled);

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxAbstractCoreModule::widgetRepresentation()
{
  Q_D(qCjyxAbstractCoreModule);

  // If required, create widgetRepresentation
  if (!d->WidgetRepresentation)
    {
    d->WidgetRepresentation = this->createNewWidgetRepresentation();
    }
  return d->WidgetRepresentation;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleRepresentation* qCjyxAbstractCoreModule::createNewWidgetRepresentation()
{
  Q_D(qCjyxAbstractCoreModule);

  if (!this->isWidgetRepresentationCreationEnabled())
    {
    return nullptr;
    }

  // Since 'logic()' should have been called in 'initialize(), let's make
  // sure the 'logic()' method call is consistent and won't create a
  // diffent logic object
#ifndef QT_NO_DEBUG // Required to avoid undefined variable warning
  vtkMRMLAbstractLogic* currentLogic = d->Logic;
  Q_ASSERT(currentLogic == this->logic());
#endif

  qCjyxAbstractModuleRepresentation *newWidgetRepresentation;
  newWidgetRepresentation = this->createWidgetRepresentation();

  if (newWidgetRepresentation == nullptr)
    {
    qDebug() << "Warning, the module "<<this->name()<< "has no widget representation";
    return nullptr;
    }
  // internally sets the logic and call setup,
  newWidgetRepresentation->setModule(this);
  // Note: setMRMLScene should be called after setup (just to make sure widgets
  // are well written and can handle empty mrmlscene
  newWidgetRepresentation->setMRMLScene(this->mrmlScene());
  //d->WidgetRepresentation->setWindowTitle(this->title());
  // Add the copy of the widget representation to the list
  d->WidgetRepresentations << newWidgetRepresentation;

  return newWidgetRepresentation;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxAbstractCoreModule::logic()
{
  Q_D(qCjyxAbstractCoreModule);

  // Return a logic object is one already exists
  if (d->Logic)
    {
    return d->Logic;
    }
  // Attempt to create a logic object
  d->Logic.TakeReference(this->createLogic());

  // If createLogic return a valid object, set its Scene and AppLogic
  // Note also that, in case no logic is associated with the module,
  // 'createLogic()' could return 0
  if (d->Logic)
    {
    vtkCjyxModuleLogic* moduleLogic = vtkCjyxModuleLogic::SafeDownCast(d->Logic);
    if (moduleLogic)
      {
      moduleLogic->SetMRMLApplicationLogic(d->AppLogic);
      moduleLogic->SetModuleShareDirectory(vtkCjyxApplicationLogic::GetModuleShareDirectory(
                                       this->name().toStdString(), this->path().toStdString()));
      }
    d->Logic->SetMRMLScene(this->mrmlScene());
    }
  return d->Logic;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractCoreModule::representationDeleted(qCjyxAbstractModuleRepresentation *representation)
{
  Q_D(qCjyxAbstractCoreModule);

  // Just remove the list entry, the object storage has already been
  // deleted by caller.
  if (d->WidgetRepresentation == representation)
    {
    d->WidgetRepresentation = nullptr;
    }
  d->WidgetRepresentations.removeAll(representation);
}

//-----------------------------------------------------------------------------
QStringList qCjyxAbstractCoreModule::associatedNodeTypes()const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
QString qCjyxAbstractCoreModule::defaultDocumentationLink()const
{
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  if (!app)
    {
    // base URL stored in application settings, therefore if there is no
    // valid application then we cannot get default documentation link
    return "";
    }
  QString url = app->moduleDocumentationUrl(this->name());
  QString linkText = QString("<p>For more information see the <a href=\"%1\">online documentation</a>.</p>").arg(url);
  return linkText;
}
