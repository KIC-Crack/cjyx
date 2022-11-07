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

#ifndef __qCjyxExtensionsServerWidget_h
#define __qCjyxExtensionsServerWidget_h

// CTK includes
#include <ctkErrorLogModel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"
#include "qCjyxWebWidget.h"

class qCjyxExtensionsServerWidgetPrivate;
class qCjyxExtensionsManagerModel;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxExtensionsServerWidget
  : public qCjyxWebWidget
{
  Q_OBJECT
  Q_PROPERTY(bool browsingEnabled READ isBrowsingEnabled WRITE setBrowsingEnabled)
public:
  /// Superclass typedef
  typedef qCjyxWebWidget Superclass;

  /// Constructor
  explicit qCjyxExtensionsServerWidget(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxExtensionsServerWidget() override;

  Q_INVOKABLE qCjyxExtensionsManagerModel* extensionsManagerModel()const;
  Q_INVOKABLE void setExtensionsManagerModel(qCjyxExtensionsManagerModel* model);

  bool isBrowsingEnabled() const;

public slots:
  /// Refresh the web page associated with the widget
  void refresh();

  void onExtensionInstalled(const QString& extensionName);

  void onExtensionScheduledForUninstall(const QString& extensionName);

  void onExtensionCancelledScheduleForUninstall(const QString& extensionName);

  void onCjyxRequirementsChanged();

  void onMessageLogged(const QString& text, ctkErrorLogLevel::LogLevels level);

  void setBrowsingEnabled(bool state);

protected:
  bool acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame) override;
  void changeEvent(QEvent *e) override;

protected slots:
  void initJavascript() override;
  void onLoadFinished(bool ok) override;
  void onLoadStarted() override;

private:
  Q_DECLARE_PRIVATE(qCjyxExtensionsServerWidget);
  Q_DISABLE_COPY(qCjyxExtensionsServerWidget);
};

#endif
