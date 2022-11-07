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
#include <QDesktopServices>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QSignalMapper>
#include <QTextStream>

// Cjyx includes
#include "vtkCjyxConfigure.h" // For Cjyx_BUILD_DICOM_SUPPORT
#include "vtkCjyxVersionConfigure.h"

// Cjyx includes
#include "qCjyxWelcomeModuleWidget.h"
#include "ui_qCjyxWelcomeModuleWidget.h"
#include "qCjyxApplication.h"
#include "qCjyxIO.h"
#include "qCjyxIOManager.h"
#include "qCjyxLayoutManager.h"
#include "qCjyxModuleManager.h"
#include "qCjyxAbstractCoreModule.h"
#include "qCjyxModulePanel.h"
#include "qCjyxUtils.h"
#include "qCjyxExtensionsManagerModel.h"

// CTK includes
#include "ctkButtonGroup.h"

// qMRML includes
#include "qMRMLWidget.h"

class qCjyxAppMainWindow;

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_CjyxWelcome
class qCjyxWelcomeModuleWidgetPrivate: public Ui_qCjyxWelcomeModuleWidget
{
  Q_DECLARE_PUBLIC(qCjyxWelcomeModuleWidget);
protected:
  qCjyxWelcomeModuleWidget* const q_ptr;
public:
  qCjyxWelcomeModuleWidgetPrivate(qCjyxWelcomeModuleWidget& object);
  void setupUi(qCjyxWidget* widget);

  bool selectModule(const QString& moduleName);

  QSignalMapper CollapsibleButtonMapper;
};

//-----------------------------------------------------------------------------
// qCjyxWelcomeModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxWelcomeModuleWidgetPrivate::qCjyxWelcomeModuleWidgetPrivate(qCjyxWelcomeModuleWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
void qCjyxWelcomeModuleWidgetPrivate::setupUi(qCjyxWidget* widget)
{
  Q_Q(qCjyxWelcomeModuleWidget);

  this->Ui_qCjyxWelcomeModuleWidget::setupUi(widget);

  // QLabel's pixmap property loads the base image (ignores high-resolution @2x versions),
  // therefore we need to retrieve and set the best icon version manually.
  this->label->setPixmap(qMRMLWidget::pixmapFromIcon(QIcon(":/Images/WelcomeLogo.png")));

  // Create the button group ensuring that only one collabsibleWidgetButton will be open at a time
  ctkButtonGroup * group = new ctkButtonGroup(widget);

  // Add all collabsibleWidgetButton to a button group
  QList<ctkCollapsibleButton*> collapsibles = widget->findChildren<ctkCollapsibleButton*>();
  foreach(ctkCollapsibleButton* collapsible, collapsibles)
    {
    group->addButton(collapsible);
    }

  // Lazily set the fitted browser source to avoid overhead when the module
  // is loaded.
  this->FeedbackCollapsibleWidget->setProperty("source", ":HTML/Feedback.html");
  this->WelcomeAndAboutCollapsibleWidget->setProperty("source", ":HTML/About.html");
  this->OtherUsefulHintsCollapsibleWidget->setProperty("source", ":HTML/OtherUsefulHints.html");
  this->AcknowledgmentCollapsibleWidget->setProperty("source", ":HTML/Acknowledgment.html");

  foreach(QWidget* widget, QWidgetList()
          << this->FeedbackCollapsibleWidget
          << this->WelcomeAndAboutCollapsibleWidget
          << this->OtherUsefulHintsCollapsibleWidget
          << this->AcknowledgmentCollapsibleWidget
          )
    {
    this->CollapsibleButtonMapper.setMapping(widget, widget);
    QObject::connect(widget, SIGNAL(contentsCollapsed(bool)),
                     &this->CollapsibleButtonMapper, SLOT(map()));
    }

  QObject::connect(&this->CollapsibleButtonMapper, SIGNAL(mapped(QWidget*)),
                   q, SLOT(loadSource(QWidget*)));
}

//-----------------------------------------------------------------------------
void qCjyxWelcomeModuleWidget::loadSource(QWidget* widget)
{
  // Lookup fitted browser
  ctkFittedTextBrowser* fittedTextBrowser =
      widget->findChild<ctkFittedTextBrowser*>();
  Q_ASSERT(fittedTextBrowser);
  if (fittedTextBrowser->source().isEmpty())
    {
    // Read content
    QString url = widget->property("source").toString();
    QFile source(url);
    if(!source.open(QIODevice::ReadOnly))
      {
      qWarning() << Q_FUNC_INFO << ": Failed to read" << url;
      return;
      }
    QTextStream in(&source);
    QString html = in.readAll();
    source.close();

    qCjyxCoreApplication* app = qCjyxCoreApplication::application();

    // Update occurrences of documentation URLs
    html = qCjyxUtils::replaceDocumentationUrlVersion(html,
      QUrl(app->documentationBaseUrl()).host(), app->documentationVersion());

    fittedTextBrowser->setHtml(html);
    }
}

//-----------------------------------------------------------------------------
bool qCjyxWelcomeModuleWidgetPrivate::selectModule(const QString& moduleName)
{
  Q_Q(qCjyxWelcomeModuleWidget);
  qCjyxModuleManager * moduleManager = qCjyxCoreApplication::application()->moduleManager();
  if (!moduleManager)
    {
    return false;
    }
  qCjyxAbstractCoreModule * module = moduleManager->module(moduleName);
  if(!module)
    {
    QMessageBox::warning(
          q, qCjyxWelcomeModuleWidget::tr("Raising %1 Module:").arg(moduleName),
          qCjyxWelcomeModuleWidget::tr("Unfortunately, this requested module is not available in this Cjyx session."),
          QMessageBox::Ok);
    return false;
    }
  qCjyxLayoutManager * layoutManager = qCjyxApplication::application()->layoutManager();
  if (!layoutManager)
    {
    return false;
    }
  layoutManager->setCurrentModule(moduleName);
  return true;
}

//-----------------------------------------------------------------------------
// qCjyxWelcomeModuleWidget methods

//-----------------------------------------------------------------------------
qCjyxWelcomeModuleWidget::qCjyxWelcomeModuleWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxWelcomeModuleWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxWelcomeModuleWidget::~qCjyxWelcomeModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxWelcomeModuleWidget::setup()
{
  Q_D(qCjyxWelcomeModuleWidget);
  d->setupUi(this);

  connect(d->LoadDicomDataButton, SIGNAL(clicked()),
          this, SLOT (loadDicomData()));
  connect(d->LoadNonDicomDataButton, SIGNAL(clicked()),
          this, SLOT (loadNonDicomData()));
  connect(d->LoadSampleDataButton, SIGNAL(clicked()),
          this, SLOT (loadRemoteSampleData()));
  connect(d->EditApplicationSettingsButton, SIGNAL(clicked()),
          this, SLOT (editApplicationSettings()));
  connect(d->ExploreLoadedDataPushButton, SIGNAL(clicked()),
          this, SLOT (exploreLoadedData()));

#ifndef Cjyx_BUILD_DICOM_SUPPORT
  d->LoadDicomDataButton->hide();
#endif

#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  qCjyxApplication* app = qCjyxApplication::application();
  bool extensionsManagerEnabled = app && app->revisionUserSettings()->value("Extensions/ManagerEnabled").toBool();
  if (extensionsManagerEnabled)
    {
    QObject::connect(d->OpenExtensionsManagerButton, SIGNAL(clicked()),
      qCjyxApplication::application(), SLOT(openExtensionsManagerDialog()));
    if (app->extensionsManagerModel())
      {
      QObject::connect(app->extensionsManagerModel(), SIGNAL(extensionUpdatesAvailable(bool)),
        this, SLOT(setExtensionUpdatesAvailable(bool)));
      this->setExtensionUpdatesAvailable(!app->extensionsManagerModel()->availableUpdateExtensions().empty());
      }
    }
  else
    {
    d->OpenExtensionsManagerButton->hide();
    }
#else
  d->OpenExtensionsManagerButton->hide();
#endif

  this->Superclass::setup();

  d->FeedbackCollapsibleWidget->setCollapsed(false);
}


//-----------------------------------------------------------------------------
void qCjyxWelcomeModuleWidget::editApplicationSettings()
{
  qCjyxApplication::application()->openSettingsDialog();
}



//-----------------------------------------------------------------------------
bool qCjyxWelcomeModuleWidget::loadDicomData()
{
  Q_D(qCjyxWelcomeModuleWidget);
  return d->selectModule("DICOM");
}


//-----------------------------------------------------------------------------
bool qCjyxWelcomeModuleWidget::loadNonDicomData()
{
  qCjyxIOManager *ioManager = qCjyxApplication::application()->ioManager();
  if (!ioManager)
    {
    return false;
    }
  return ioManager->openAddDataDialog();
}


//-----------------------------------------------------------------------------
bool qCjyxWelcomeModuleWidget::loadRemoteSampleData()
{
  Q_D(qCjyxWelcomeModuleWidget);
  return d->selectModule("SampleData");
}


//-----------------------------------------------------------------------------
bool qCjyxWelcomeModuleWidget::exploreLoadedData()
{
  Q_D(qCjyxWelcomeModuleWidget);
  return d->selectModule("Data");
}

//---------------------------------------------------------------------------
void qCjyxWelcomeModuleWidget::setExtensionUpdatesAvailable(bool updateAvailable)
{
#ifdef Cjyx_BUILD_EXTENSIONMANAGER_SUPPORT
  Q_D(qCjyxWelcomeModuleWidget);
  qCjyxApplication* app = qCjyxApplication::application();
  if (!app || !app->revisionUserSettings()->value("Extensions/ManagerEnabled").toBool())
    {
    return;
    }

  // Check if there was a change
  const char extensionUpdateAvailablePropertyName[] = "extensionUpdateAvailable";
  if (d->OpenExtensionsManagerButton->property(extensionUpdateAvailablePropertyName).toBool() == updateAvailable)
    {
    // no change
    return;
    }
  d->OpenExtensionsManagerButton->setProperty(extensionUpdateAvailablePropertyName, updateAvailable);

  if (updateAvailable)
    {
    d->OpenExtensionsManagerButton->setIcon(QIcon(":/Icons/ExtensionNotificationIcon.png"));
    }
  else
    {
    d->OpenExtensionsManagerButton->setIcon(QIcon(":/Icons/ExtensionDefaultIcon.png"));
    }
#else
  Q_UNUSED(updateAvailable);
#endif
}
