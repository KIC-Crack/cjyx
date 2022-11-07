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

#include "qCjyxAppMainWindow.h"
#include "qCjyxAppMainWindow_p.h"

// Qt includes
#include <QDesktopServices>
#include <QLabel>
#include <QPixmap>
#include <QStyle>
#include <QUrl>

// Cjyx includes
#include "qCjyxAboutDialog.h"
#include "qCjyxAbstractModule.h"
#include "qCjyxActionsDialog.h"
#include "qCjyxApplication.h"
#include "qCjyxErrorReportDialog.h"
#include "qCjyxModuleManager.h"
#include "vtkCjyxVersionConfigure.h" // For Cjyx_VERSION_MAJOR,Cjyx_VERSION_MINOR

namespace
{

//-----------------------------------------------------------------------------
void setThemeIcon(QAction* action, const QString& name)
{
  action->setIcon(QIcon::fromTheme(name, action->icon()));
}

} // end of anonymous namespace

//-----------------------------------------------------------------------------
// qCjyxAppMainWindowPrivate methods

qCjyxAppMainWindowPrivate::qCjyxAppMainWindowPrivate(qCjyxAppMainWindow& object)
  : Superclass(object)
{
}

//-----------------------------------------------------------------------------
qCjyxAppMainWindowPrivate::~qCjyxAppMainWindowPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxAppMainWindowPrivate::init()
{
  Q_Q(qCjyxMainWindow);
  this->Superclass::init();
}

//-----------------------------------------------------------------------------
void qCjyxAppMainWindowPrivate::setupUi(QMainWindow * mainWindow)
{
  //----------------------------------------------------------------------------
  // Add actions
  //----------------------------------------------------------------------------
  QAction* helpKeyboardShortcutsAction = new QAction(mainWindow);
  helpKeyboardShortcutsAction->setObjectName("HelpKeyboardShortcutsAction");
  helpKeyboardShortcutsAction->setText(qCjyxAppMainWindow::tr("&Keyboard Shortcuts Reference"));
  helpKeyboardShortcutsAction->setToolTip(qCjyxAppMainWindow::tr("Raise a window that lists commonly-used keyboard shortcuts."));

  QAction* helpDocumentationAction = new QAction(mainWindow);
  helpDocumentationAction->setObjectName("HelpDocumentationAction");
  helpDocumentationAction->setText(qCjyxAppMainWindow::tr("Documentation"));
  helpDocumentationAction->setShortcut(QKeySequence(qCjyxAppMainWindow::tr("Ctrl+1", "Documentation")));

  QAction* helpQuickStartAction = new QAction(mainWindow);
  helpQuickStartAction->setObjectName("HelpQuickStartAction");
  helpQuickStartAction->setText(qCjyxAppMainWindow::tr("Quick Start"));

  QAction* helpGetHelpAction = new QAction(mainWindow);
  helpGetHelpAction->setObjectName("HelpGetHelpAction");
  helpGetHelpAction->setText(qCjyxAppMainWindow::tr("Get Help"));

  QAction* helpUserInterfaceAction = new QAction(mainWindow);
  helpUserInterfaceAction->setObjectName("HelpUserInterfaceAction");
  helpUserInterfaceAction->setText(qCjyxAppMainWindow::tr("User Interface"));

  QAction* helpVisitCjyxForumAction = new QAction(mainWindow);
  helpVisitCjyxForumAction->setObjectName("HelpVisitCjyxForumAction");
  helpVisitCjyxForumAction->setText(qCjyxAppMainWindow::tr("Visit the Cjyx Forum"));

  QAction* helpBrowseTutorialsAction = new QAction(mainWindow);
  helpBrowseTutorialsAction->setObjectName("HelpBrowseTutorialsAction");
  helpBrowseTutorialsAction->setText(qCjyxAppMainWindow::tr("Browse Tutorials"));
  helpBrowseTutorialsAction->setToolTip(qCjyxAppMainWindow::tr("Raise the training pages in your favorite web browser"));

  QAction* helpJoinUsOnTwitterAction = new QAction(mainWindow);
  helpJoinUsOnTwitterAction->setObjectName("HelpJoinUsOnTwitterAction");
  helpJoinUsOnTwitterAction->setText(qCjyxAppMainWindow::tr("Join Us on Twitter"));

  QAction* helpSearchFeatureRequestsAction = new QAction(mainWindow);
  helpSearchFeatureRequestsAction->setObjectName("HelpSearchFeatureRequestsAction");
  helpSearchFeatureRequestsAction->setText(qCjyxAppMainWindow::tr("Search Feature Requests"));

  QAction* helpViewLicenseAction = new QAction(mainWindow);
  helpViewLicenseAction->setObjectName("HelpViewLicenseAction");
  helpViewLicenseAction->setText(qCjyxAppMainWindow::tr("View License"));

  QAction* helpHowToCiteAction = new QAction(mainWindow);
  helpHowToCiteAction->setObjectName("HelpHowToCiteAction");
  helpHowToCiteAction->setText(qCjyxAppMainWindow::tr("How to Cite"));

  QAction* helpCjyxPublicationsAction = new QAction(mainWindow);
  helpCjyxPublicationsAction->setObjectName("HelpCjyxPublicationsAction");
  helpCjyxPublicationsAction->setText(qCjyxAppMainWindow::tr("Cjyx Publications"));

  QAction* helpAcknowledgmentsAction = new QAction(mainWindow);
  helpAcknowledgmentsAction->setObjectName("HelpAcknowledgmentsAction");
  helpAcknowledgmentsAction->setText(qCjyxAppMainWindow::tr("Acknowledgments"));

  QAction* helpReportBugOrFeatureRequestAction = new QAction(mainWindow);
  helpReportBugOrFeatureRequestAction->setObjectName("HelpReportBugOrFeatureRequestAction");
  helpReportBugOrFeatureRequestAction->setText(qCjyxAppMainWindow::tr("Report a Bug"));
  helpReportBugOrFeatureRequestAction->setToolTip(qCjyxAppMainWindow::tr("Report error or request enhancement or new feature."));

  QAction* helpAboutCjyxAppAction = new QAction(mainWindow);
  helpAboutCjyxAppAction->setObjectName("HelpAboutCjyxAppAction");
  helpAboutCjyxAppAction->setText(qCjyxAppMainWindow::tr("About 3D Cjyx"));
  helpAboutCjyxAppAction->setToolTip(qCjyxAppMainWindow::tr("Provides a description of the Cjyx effort and its support."));

  //----------------------------------------------------------------------------
  // Calling "setupUi()" after adding the actions above allows the call
  // to "QMetaObject::connectSlotsByName()" done in "setupUi()" to
  // successfully connect each slot with its corresponding action.
  this->Superclass::setupUi(mainWindow);

  //----------------------------------------------------------------------------
  // Configure
  //----------------------------------------------------------------------------
  mainWindow->setWindowTitle("3D Cjyx");
  mainWindow->setWindowIcon(QIcon(":/Icons/Medium/DesktopIcon.png"));

  QLabel* logoLabel = new QLabel();
  logoLabel->setObjectName("LogoLabel");
  logoLabel->setPixmap(qMRMLWidget::pixmapFromIcon(QIcon(":/ModulePanelLogo.png")));
  this->PanelDockWidget->setTitleBarWidget(logoLabel);

  this->HelpMenu->addAction(helpDocumentationAction);
  this->HelpMenu->addAction(helpQuickStartAction);
  this->HelpMenu->addAction(helpGetHelpAction);
  this->HelpMenu->addAction(helpUserInterfaceAction);
  this->HelpMenu->addSeparator();
  this->HelpMenu->addAction(helpKeyboardShortcutsAction);
  this->HelpMenu->addAction(helpBrowseTutorialsAction);
  this->HelpMenu->addSeparator();
  this->HelpMenu->addAction(helpVisitCjyxForumAction);
  this->HelpMenu->addAction(helpJoinUsOnTwitterAction);
  this->HelpMenu->addAction(helpSearchFeatureRequestsAction);
  this->HelpMenu->addAction(helpReportBugOrFeatureRequestAction);
  this->HelpMenu->addSeparator();
  this->HelpMenu->addAction(helpViewLicenseAction);
  this->HelpMenu->addAction(helpHowToCiteAction);
  this->HelpMenu->addAction(helpCjyxPublicationsAction);
  this->HelpMenu->addAction(helpAcknowledgmentsAction);
  this->HelpMenu->addSeparator();
  this->HelpMenu->addAction(helpAboutCjyxAppAction);

  //----------------------------------------------------------------------------
  // Icons in the menu
  //----------------------------------------------------------------------------
  // Customize QAction icons with standard pixmaps
  QIcon networkIcon = mainWindow->style()->standardIcon(QStyle::SP_DriveNetIcon);
  QIcon informationIcon = mainWindow->style()->standardIcon(QStyle::SP_MessageBoxInformation);
  QIcon questionIcon = mainWindow->style()->standardIcon(QStyle::SP_MessageBoxQuestion);

  helpAboutCjyxAppAction->setIcon(informationIcon);
  helpReportBugOrFeatureRequestAction->setIcon(questionIcon);

  setThemeIcon(helpAboutCjyxAppAction, "help-about");
  setThemeIcon(helpReportBugOrFeatureRequestAction, "tools-report-bug");
}

//-----------------------------------------------------------------------------
// qCjyxAppMainWindow methods

//-----------------------------------------------------------------------------
qCjyxAppMainWindow::qCjyxAppMainWindow(QWidget *_parent)
  : Superclass(new qCjyxAppMainWindowPrivate(*this), _parent)
{
  Q_D(qCjyxAppMainWindow);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxAppMainWindow::qCjyxAppMainWindow(qCjyxAppMainWindowPrivate* pimpl,
                                           QWidget* windowParent)
  : Superclass(pimpl, windowParent)
{
  // init() is called by derived class.
}

//-----------------------------------------------------------------------------
qCjyxAppMainWindow::~qCjyxAppMainWindow() = default;

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpKeyboardShortcutsAction_triggered()
{
  qCjyxActionsDialog actionsDialog(this);
  actionsDialog.setActionsWithNoShortcutVisible(false);
  actionsDialog.setMenuActionsVisible(false);
  actionsDialog.addActions(this->findChildren<QAction*>(), "Cjyx Application");

  // scan the modules for their actions
  QList<QAction*> moduleActions;
  qCjyxModuleManager * moduleManager = qCjyxApplication::application()->moduleManager();
  foreach(const QString& moduleName, moduleManager->modulesNames())
    {
    qCjyxAbstractModule* module =
      qobject_cast<qCjyxAbstractModule*>(moduleManager->module(moduleName));
    if (module)
      {
      moduleActions << module->action();
      }
    }
  if (moduleActions.size())
    {
    actionsDialog.addActions(moduleActions, "Modules");
    }
  // TODO add more actions
  actionsDialog.exec();
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpBrowseTutorialsAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/getting_started.html#tutorials"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpDocumentationAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl()));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpQuickStartAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/getting_started.html#quick-start"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpGetHelpAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/get_help.html"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpUserInterfaceAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/user_interface.html"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpVisitCjyxForumAction_triggered()
{
  QDesktopServices::openUrl(QUrl("https://discourse.slicer.org/"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpJoinUsOnTwitterAction_triggered()
{
  QDesktopServices::openUrl(QUrl("https://twitter.com/3dslicerapp"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpSearchFeatureRequestsAction_triggered()
{
  QDesktopServices::openUrl(QUrl("https://discourse.slicer.org/c/support/feature-requests/9"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpViewLicenseAction_triggered()
{
  QDesktopServices::openUrl(QUrl("https://github.com/Slicer/Slicer/blob/master/License.txt"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpHowToCiteAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/about.html#how-to-cite"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpCjyxPublicationsAction_triggered()
{
  QDesktopServices::openUrl(QUrl("https://scholar.google.com/scholar?&as_sdt=1%2C22&as_vis=1&q=%28%223D+Slicer%22+OR+%22slicer+org%22+OR+Slicer3D%29+-Slic3r+&btnG="));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpAcknowledgmentsAction_triggered()
{
  QDesktopServices::openUrl(QUrl(qCjyxApplication::application()->documentationBaseUrl() + "/user_guide/about.html#acknowledgments"));
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpAboutCjyxAppAction_triggered()
{
  qCjyxAboutDialog about(this);
  about.setLogo(QPixmap(":/Logo.png"));
  about.exec();
}

//---------------------------------------------------------------------------
void qCjyxAppMainWindow::on_HelpReportBugOrFeatureRequestAction_triggered()
{
  qCjyxErrorReportDialog errorReport(this);
  errorReport.exec();
}
