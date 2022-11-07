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
#include <QStyle>
#include <QUrlQuery>
#include <QWebEngineView>

// CTK includes
#include <ctkPimpl.h>

// QtGUI includes
#include "qCjyxExtensionsServerWidget.h"
#include "qCjyxExtensionsServerWidget_p.h"
#include "qCjyxExtensionsManagerModel.h"

// --------------------------------------------------------------------------
void ExtensionInstallWidgetWebChannelProxy::refresh()
{
  this->InstallWidget->refresh();
}

// --------------------------------------------------------------------------
qCjyxExtensionsServerWidgetPrivate::qCjyxExtensionsServerWidgetPrivate(qCjyxExtensionsServerWidget& object)
  : qCjyxWebWidgetPrivate(object),
    q_ptr(&object),
    BrowsingEnabled(true)
{
  Q_Q(qCjyxExtensionsServerWidget);
  this->ExtensionsManagerModel = nullptr;
  this->InstallWidgetForWebChannel = new ExtensionInstallWidgetWebChannelProxy;
  this->InstallWidgetForWebChannel->InstallWidget = q;
  this->HandleExternalUrlWithDesktopService = true;
}

// --------------------------------------------------------------------------
qCjyxExtensionsServerWidgetPrivate::~qCjyxExtensionsServerWidgetPrivate()
{
  delete this->InstallWidgetForWebChannel;
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::setFailurePage(const QStringList& errors)
{
  Q_Q(qCjyxExtensionsServerWidget);
  QString html =
      "<style type='text/css'>"
      "  div.viewWrapperCjyx{"
      "    font-family:'Lucida Grande','Lucida Sans Unicode',helvetica,arial,Verdana,sans-serif;"
      "    font-size:13px;margin-left:8px;color:#777777;"
      "  }"
      "  div.extensionsHeader,div.extensionsBody{margin-right:10px;}"
      "  div.extensionsHeader{height:45px;border-bottom:1px solid #d0d0d0;}"
      "  div.extensionsTitle{float:left;font-size:24px;font-weight:bolder;margin-top:10px;}"
      "  div.extensionsBodyLeftColumn{float:left;width:230px;border-right:1px solid #d0d0d0;min-height:450px;}"
      "  div.extensionsBodyRightColumn{margin-left:230px;}"
      "  div.error{"
      "      position: relative;"
      "      min-width: 13em; max-width: 52em; margin: 4em auto;"
      "      border: 1px solid threedshadow; border-radius: 10px 10px 10px 10px;"
      "      padding: 3em;"
      "      -webkit-padding-start: 30px;"
      "      background: url('qrc:Icons/ExtensionError.svg') no-repeat scroll left 0px content-box border-box;"
      "     }"
      "   #errorTitle, #errorDescription {-webkit-margin-start:80px;}"
      "   #errorTitle h1 {margin:0px 0px 0.6em;}"
      "   #errorDescription ul{"
      "     list-style: square outside none;"
      "     margin: 0px; -webkit-margin-start: 1.5em; padding: 0px;"
      "     }"
      "   #errorDescription ul > li{margin-bottom: 0.5em;}"
      "   #errorTryAgain{margin-top: 2em;}"
      "</style>"
      "<div class='viewWrapperCjyx'>"
      "  <div class='extensionsHeader'>"
      "    <div class='extensionsTitle'>Cjyx Extensions</div>"
      "  </div>"
      "  <div class='extensionsBody'>"
      "    <!-- Following layout and associated CSS style are inspired from Mozilla error message. -->"
      "    <!-- It is originally covered by https://mozilla.org/MPL/2.0/ license -->"
      "    <!-- MPL 2.0 license is compatible with Cjyx (BSD-like) license -->"
      "    <div class='error'>"
      "      <div id='errorTitle'><h1>Extensions can not be installed.</h1></div>"
      "      <div id='errorDescription'>"
      "        <ul>"
      "%1"
#ifdef Q_OS_MAC
    "          <li><b>Extensions manager requires <em>3D Cjyx</em> to be installed. "
    "Open the disk image (.dmg) file, drag <em>Cjyx.app</em> into the the <em>Applications</em> folder, "
    "and launch <em>3D Cjyx</em> from the <em>Applications</em> folder.</b> "
#else
    "          <li>Check that <b>3D Cjyx</b> is properly installed. "
#endif
      "<a href='https://slicer.readthedocs.io/en/latest/user_guide/getting_started.html#installing-3d-slicer'>Read more...</a></li>"
      "        </ul>"
      "        <button id='errorTryAgain' onclick='window.extensions_install_widget.refresh();' autofocus='true'>Try Again</button>"
      "      </div>"
      "    </div>"
      "  </div>"
      "</div>";

  QStringList htmlErrors;
  foreach(const QString& error, errors)
    {
    htmlErrors << QString("<li>%1</li>").arg(error);
    }
  q->webView()->setHtml(html.arg(htmlErrors.join("/n")));
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::updateTheme()
{
  Q_Q(qCjyxExtensionsServerWidget);
  this->setDarkThemeEnabled(q->style()->objectName().compare("Dark Cjyx", Qt::CaseInsensitive) == 0);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::setDarkThemeEnabled(bool enabled)
{
  Q_Q(qCjyxExtensionsServerWidget);
  if(!this->BrowsingEnabled)
    {
    return;
    }
  int serverAPI = this->ExtensionsManagerModel->serverAPI();
  if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
    {
    q->evalJS(QString("app.$vuetify.theme.dark = %1;").arg(enabled ? "true" : "false"));
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::initializeWebChannelTransport(QByteArray& webChannelScript)
{
  this->Superclass::initializeWebChannelTransport(webChannelScript);
  webChannelScript.append(
      " window.extensions_manager_model = channel.objects.extensions_manager_model;\n"
      // See ExtensionInstallWidgetWebChannelProxy
      " window.extensions_install_widget = channel.objects.extensions_install_widget;\n"
      );
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::initializeWebChannel(QWebChannel* webChannel)
{
  this->Superclass::initializeWebChannel(webChannel);
  webChannel->registerObject(
        "extensions_install_widget", this->InstallWidgetForWebChannel);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidgetPrivate::registerExtensionsManagerModel(
    qCjyxExtensionsManagerModel* oldModel, qCjyxExtensionsManagerModel* newModel)
{
  Q_Q(qCjyxExtensionsServerWidget);
  QWebChannel* webChannel = q->webView()->page()->webChannel();
  if (oldModel)
    {
    webChannel->deregisterObject(oldModel);
    }
  if (newModel)
    {
    webChannel->registerObject("extensions_manager_model", newModel);
    }
}

// --------------------------------------------------------------------------
qCjyxExtensionsServerWidget::qCjyxExtensionsServerWidget(QWidget* _parent)
  : Superclass(new qCjyxExtensionsServerWidgetPrivate(*this), _parent)
{
  Q_D(qCjyxExtensionsServerWidget);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxExtensionsServerWidget::~qCjyxExtensionsServerWidget() = default;

// --------------------------------------------------------------------------
qCjyxExtensionsManagerModel* qCjyxExtensionsServerWidget::extensionsManagerModel()const
{
  Q_D(const qCjyxExtensionsServerWidget);
  return d->ExtensionsManagerModel;
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::setExtensionsManagerModel(qCjyxExtensionsManagerModel* model)
{
  Q_D(qCjyxExtensionsServerWidget);
  if (this->extensionsManagerModel() == model)
    {
    return;
    }

  disconnect(this, SLOT(onExtensionInstalled(QString)));
  //disconnect(this, SLOT(onExtensionUninstalled(QString)));
  disconnect(this, SLOT(onExtensionScheduledForUninstall(QString)));
  disconnect(this, SLOT(onCjyxRequirementsChanged()));
  disconnect(this, SLOT(onMessageLogged(QString,ctkErrorLogLevel::LogLevels)));
  disconnect(this, SLOT(onDownloadStarted(QNetworkReply*)));
  disconnect(this, SLOT(onDownloadFinished(QNetworkReply*)));

  d->registerExtensionsManagerModel(
        /* oldModel= */ d->ExtensionsManagerModel, /* newModel= */ model);

  d->ExtensionsManagerModel = model;

  if (model)
    {
    this->onCjyxRequirementsChanged();

    QObject::connect(model, SIGNAL(extensionInstalled(QString)),
                     this, SLOT(onExtensionInstalled(QString)));

    QObject::connect(model, SIGNAL(extensionScheduledForUninstall(QString)),
                     this, SLOT(onExtensionScheduledForUninstall(QString)));

    QObject::connect(model, SIGNAL(extensionCancelledScheduleForUninstall(QString)),
                     this, SLOT(onExtensionCancelledScheduleForUninstall(QString)));

    QObject::connect(model, SIGNAL(cjyxRequirementsChanged(QString,QString,QString)),
                     this, SLOT(onCjyxRequirementsChanged()));

    QObject::connect(model, SIGNAL(messageLogged(QString,ctkErrorLogLevel::LogLevels)),
                     this, SLOT(onMessageLogged(QString,ctkErrorLogLevel::LogLevels)));

    QObject::connect(model, SIGNAL(downloadStarted(QNetworkReply*)),
                     this, SLOT(onDownloadStarted(QNetworkReply*)));

    QObject::connect(model, SIGNAL(downloadFinished(QNetworkReply*)),
                     this, SLOT(onDownloadFinished(QNetworkReply*)));
    }
}

// --------------------------------------------------------------------------
CTK_GET_CPP(qCjyxExtensionsServerWidget, bool, isBrowsingEnabled, BrowsingEnabled)
CTK_SET_CPP(qCjyxExtensionsServerWidget, bool, setBrowsingEnabled, BrowsingEnabled)

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::refresh()
{
  Q_D(qCjyxExtensionsServerWidget);
  if (!d->ExtensionsManagerModel)
    {
    return;
    }
  QStringList errors = this->extensionsManagerModel()->checkInstallPrerequisites();
  if (!errors.empty())
    {
    d->setFailurePage(errors);
    return;
    }
  this->webView()->setUrl(this->extensionsManagerModel()->extensionsListUrl());
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onExtensionInstalled(const QString& extensionName)
{
  Q_D(qCjyxExtensionsServerWidget);
  if(d->BrowsingEnabled)
    {
    int serverAPI = d->ExtensionsManagerModel->serverAPI();
    if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
      {
      this->evalJS(QString("app.setExtensionButtonState('%1', 'Installed');").arg(extensionName));
      }
    else
      {
      qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
      }
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onExtensionScheduledForUninstall(const QString& extensionName)
{
  Q_D(qCjyxExtensionsServerWidget);
  if(d->BrowsingEnabled)
    {
    int serverAPI = d->ExtensionsManagerModel->serverAPI();
    if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
      {
      this->evalJS(QString("app.setExtensionButtonState('%1', 'ScheduledForUninstall');").arg(extensionName));
      }
    else
      {
      qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
      }
    }
}

// -------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onExtensionCancelledScheduleForUninstall(const QString& extensionName)
{
  this->onExtensionInstalled(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onCjyxRequirementsChanged()
{
  Q_D(qCjyxExtensionsServerWidget);
  if (d->BrowsingEnabled)
    {
    this->refresh();
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onMessageLogged(const QString& text, ctkErrorLogLevel::LogLevels level)
{
  Q_D(qCjyxExtensionsServerWidget);
  if(!d->BrowsingEnabled)
    {
    return;
    }
  QString escapedText = QString(text).replace("'", "\\'");
  QString delay = "2500";
  QString state;
  if (level == ctkErrorLogLevel::Warning)
    {
    delay = "10000";
    state = "warning";
    }
  else if(level == ctkErrorLogLevel::Critical || level == ctkErrorLogLevel::Fatal)
    {
    delay = "10000";
    state = "error";
    }
  int serverAPI = d->ExtensionsManagerModel->serverAPI();
  if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
    {
    this->evalJS(QString("app.createNotice('%1', %2, '%3')").arg(escapedText).arg(delay).arg(state));
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onLoadStarted()
{
  this->Superclass::onLoadStarted();
  this->initJavascript();
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::initJavascript()
{
  Q_D(qCjyxExtensionsServerWidget);
  this->Superclass::initJavascript();
  // This is done in qCjyxExtensionsServerWidgetPrivate::initializeWebChannel()
  // and qCjyxExtensionsServerWidgetPrivate::registerExtensionsManagerModel()
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::onLoadFinished(bool ok)
{
  Q_D(qCjyxExtensionsServerWidget);
  this->Superclass::onLoadFinished(ok);
  if(!ok && d->NavigationRequestAccepted)
    {
    d->setFailurePage(QStringList() << QString("Failed to load extension page using this URL: <strong>%1</strong>")
                      .arg(this->extensionsManagerModel()->extensionsListUrl().toString()));
    }
  if (ok)
    {
    d->updateTheme();
    }
}

// --------------------------------------------------------------------------
bool qCjyxExtensionsServerWidget::acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
  Q_D(qCjyxExtensionsServerWidget);
  d->InternalHosts = QStringList() << this->extensionsManagerModel()->frontendServerUrl().host();
  return Superclass::acceptNavigationRequest(url, type, isMainFrame);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsServerWidget::changeEvent(QEvent *e)
{
  Q_D(qCjyxExtensionsServerWidget);
  switch (e->type())
    {
    case QEvent::StyleChange:
      d->updateTheme();
    break;
    default:
    break;
    }
  this->Superclass::changeEvent(e);
}
