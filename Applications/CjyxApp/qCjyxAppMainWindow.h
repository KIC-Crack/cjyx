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

#ifndef __qCjyxAppMainWindow_h
#define __qCjyxAppMainWindow_h

// Cjyx includes
#include "qCjyxAppExport.h"
#include "qCjyxMainWindow.h"

class qCjyxAppMainWindowPrivate;

class Q_CJYX_APP_EXPORT qCjyxAppMainWindow : public qCjyxMainWindow
{
  Q_OBJECT
public:
  typedef qCjyxMainWindow Superclass;

  qCjyxAppMainWindow(QWidget *parent=nullptr);
  ~qCjyxAppMainWindow() override;

public slots:
  void on_HelpKeyboardShortcutsAction_triggered();
  void on_HelpBrowseTutorialsAction_triggered();
  void on_HelpDocumentationAction_triggered();
  void on_HelpQuickStartAction_triggered();
  void on_HelpGetHelpAction_triggered();
  void on_HelpUserInterfaceAction_triggered();
  void on_HelpVisitCjyxForumAction_triggered();
  void on_HelpJoinUsOnTwitterAction_triggered();
  void on_HelpSearchFeatureRequestsAction_triggered();
  void on_HelpViewLicenseAction_triggered();
  void on_HelpHowToCiteAction_triggered();
  void on_HelpCjyxPublicationsAction_triggered();
  void on_HelpAcknowledgmentsAction_triggered();

  void on_HelpReportBugOrFeatureRequestAction_triggered();
  void on_HelpAboutCjyxAppAction_triggered();

protected:
  qCjyxAppMainWindow(qCjyxAppMainWindowPrivate* pimpl, QWidget* parent);

private:
  Q_DECLARE_PRIVATE(qCjyxAppMainWindow);
  Q_DISABLE_COPY(qCjyxAppMainWindow);
};

#endif
