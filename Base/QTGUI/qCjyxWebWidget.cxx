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
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QPalette>
#include <QSettings>
#include <QTime>
#include <QUrl>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QWebEngineView>
#include <QWebChannel>
#include <QWebEngineDownloadItem>
#include <QWebEngineScript>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineScriptCollection>
#include <QFile>

// QtCore includes
#include <qCjyxPersistentCookieJar.h>

// QtGUI includes
#include "qCjyxWebWidget.h"
#include "qCjyxWebWidget_p.h"

// Cjyx includes
#include "qCjyxWebDownloadWidget.h"
#include "qCjyxWebPythonProxy.h"

// --------------------------------------------------------------------------
namespace
{
class qCjyxWebEngineView : public QWebEngineView
{
public:
  qCjyxWebEngineView(QWidget *parent = Q_NULLPTR) : QWebEngineView(parent){}
  ~qCjyxWebEngineView() override = default;
  QSize sizeHint() const override
  {
    // arbitrary values to address https://issues.slicer.org/view.php?id=4613
    return QSize(150, 150);
  }
};
}

// --------------------------------------------------------------------------
qCjyxWebEnginePage::qCjyxWebEnginePage(QWebEngineProfile *profile, QObject *parent)
  : QWebEnginePage(profile, parent),
    WebWidget(nullptr),
    JavaScriptConsoleMessageLoggingEnabled(false)
{
}

// --------------------------------------------------------------------------
qCjyxWebEnginePage::~qCjyxWebEnginePage() = default;

// --------------------------------------------------------------------------
qCjyxWebWidgetPrivate::qCjyxWebWidgetPrivate(qCjyxWebWidget& object)
  :q_ptr(&object)
  , HandleExternalUrlWithDesktopService(false)
  , NavigationRequestAccepted(true)
{
}

// --------------------------------------------------------------------------
qCjyxWebWidgetPrivate::~qCjyxWebWidgetPrivate() = default;

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::initializeWebChannel(QWebChannel* webChannel)
{
  webChannel->registerObject("cjyxPython", this->PythonProxy);
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::init()
{
  Q_Q(qCjyxWebWidget);

  this->setupUi(q);
  this->WebView = new qCjyxWebEngineView();

  QSettings settings;
  bool developerModeEnabled = settings.value("Developer/DeveloperMode", false).toBool();
  if (developerModeEnabled)
    {
    // Enable dev tools by default for the test browser
    if (qgetenv("QTWEBENGINE_REMOTE_DEBUGGING").isNull())
      {
      qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "1337");
      }
    }

  this->PythonProxy = new qCjyxWebPythonProxy(q);
  QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();

  this->WebEnginePage = new qCjyxWebEnginePage(profile, this->WebView);
  this->WebEnginePage->JavaScriptConsoleMessageLoggingEnabled = developerModeEnabled;
  this->WebEnginePage->WebWidget = q;
  this->WebView->setPage(this->WebEnginePage);

  this->initializeWebEngineProfile(profile);

  this->WebChannel = new QWebChannel(this->WebView->page());
  this->initializeWebChannel(this->WebChannel);
  this->WebView->page()->setWebChannel(this->WebChannel);


  // XXX Since relying on automatic deletion of QWebEngineView when the application
  // exit causes the application to crash. This is a workaround for explicitly
  // deleting the object before the application exit.
  // See https://bugreports.qt.io/browse/QTBUG-50160#comment-305211
  QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()),
                   this, SLOT(onAppAboutToQuit()));
  this->verticalLayout->insertWidget(0, this->WebView);

  this->WebView->installEventFilter(q);

  QObject::connect(this->WebView, SIGNAL(loadStarted()),
                   q, SLOT(onLoadStarted()));

  QObject::connect(this->WebView, SIGNAL(loadProgress(int)),
                   q, SLOT(onLoadProgress(int)));

  QObject::connect(this->WebView, SIGNAL(loadFinished(bool)),
                   q, SLOT(onLoadFinished(bool)));

  QObject::connect(this->WebView, SIGNAL(loadProgress(int)),
                   this->ProgressBar, SLOT(setValue(int)));

  QObject::connect(this->WebEnginePage, SIGNAL(pdfPrintingFinished(QString, bool)),
                   q, SIGNAL(pdfPrintingFinished(QString, bool)));

  this->ProgressBar->setVisible(false);

  // Set background color behind the document's body to match current theme
  this->WebEnginePage->setBackgroundColor(q->palette().color(QPalette::Window));
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::onAppAboutToQuit()
{
  if (this->WebView)
    {
    this->WebView->setParent(nullptr);
    delete this->WebView;
    this->WebView = nullptr;
    }
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::updateWebChannelScript(QByteArray& webChannelScript)
{
  webChannelScript.append(
        "\n"
        "new QWebChannel(qt.webChannelTransport, function(channel) {\n"
        );
  this->initializeWebChannelTransport(webChannelScript);
  webChannelScript.append(
        "});\n"
        );
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::initializeWebChannelTransport(QByteArray& webChannelScript)
{
  webChannelScript.append(" window.cjyxPython = channel.objects.cjyxPython;\n");
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::initializeWebEngineProfile(QWebEngineProfile* profile)
{
  if (!profile)
    {
    qWarning() << Q_FUNC_INFO << "Invalid profile";
    return;
    }

  if (!this->WebEnginePage->scripts().findScript("qwebchannel_appended.js").isNull())
    {
    // profile is already initialized
    return;
    }

  QFile webChannelJsFile(":/qtwebchannel/qwebchannel.js");

  if (!webChannelJsFile.open(QIODevice::ReadOnly))
    {
    qWarning() << QString("Couldn't open qwebchannel.js file: %1").arg(webChannelJsFile.errorString());
    }
  else
    {
    QByteArray webChannelJs = webChannelJsFile.readAll();
    this->updateWebChannelScript(webChannelJs);
    QWebEngineScript script;
    script.setSourceCode(webChannelJs);
    script.setName("qwebchannel_appended.js");
    script.setWorldId(QWebEngineScript::MainWorld);
    script.setInjectionPoint(QWebEngineScript::DocumentCreation);
    script.setRunsOnSubFrames(false);
    this->WebEnginePage->scripts().insert(script);
    }

  // setup default download handler shared across all widgets
  QObject::connect(profile, SIGNAL(downloadRequested(QWebEngineDownloadItem*)),
                    this, SLOT(handleDownload(QWebEngineDownloadItem*)));

}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::setDocumentWebkitHidden(bool value)
{
  Q_Q(qCjyxWebWidget);
  q->evalJS(QString("document.webkitHidden = %1").arg(value ? "true" : "false"));
}

// --------------------------------------------------------------------------
void qCjyxWebWidgetPrivate::handleDownload(QWebEngineDownloadItem* download)
{
  Q_Q(qCjyxWebWidget);

  if (this->WebEnginePage != download->page())
    {
    // Since the download request is emitted by the default profile observed by
    // all web widget instances, we ignore the request if it does not originate
    // from the page associated with this web widget instance.
    return;
    }

  qCjyxWebDownloadWidget *downloader = new qCjyxWebDownloadWidget(q);
  downloader->setAttribute(Qt::WA_DeleteOnClose);
  downloader->show();
  downloader->handleDownload(download);
}

// --------------------------------------------------------------------------
qCjyxWebWidget::qCjyxWebWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxWebWidgetPrivate(*this))
{
  Q_D(qCjyxWebWidget);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxWebWidget::qCjyxWebWidget(
  qCjyxWebWidgetPrivate* pimpl, QWidget* _parent)
  : Superclass(_parent), d_ptr(pimpl)
{
  // Note: You are responsible to call init() in the constructor of derived class.
}

// --------------------------------------------------------------------------
qCjyxWebWidget::~qCjyxWebWidget() = default;

// --------------------------------------------------------------------------
bool qCjyxWebWidget::handleExternalUrlWithDesktopService() const
{
  Q_D(const qCjyxWebWidget);
  return d->HandleExternalUrlWithDesktopService;
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::setHandleExternalUrlWithDesktopService(bool enable)
{
  Q_D(qCjyxWebWidget);
  d->HandleExternalUrlWithDesktopService = enable;
}

// --------------------------------------------------------------------------
QStringList qCjyxWebWidget::internalHosts() const
{
  Q_D(const qCjyxWebWidget);
  return d->InternalHosts;
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::setInternalHosts(const QStringList& hosts)
{
  Q_D(qCjyxWebWidget);
  d->InternalHosts = hosts;
}

// --------------------------------------------------------------------------
bool qCjyxWebWidget::javaScriptConsoleMessageLoggingEnabled() const
{
  Q_D(const qCjyxWebWidget);
  return d->WebEnginePage->JavaScriptConsoleMessageLoggingEnabled;
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::setJavaScriptConsoleMessageLoggingEnabled(bool enable)
{
  Q_D(qCjyxWebWidget);
  d->WebEnginePage->JavaScriptConsoleMessageLoggingEnabled = enable;
}

// --------------------------------------------------------------------------
QWebEngineView *
qCjyxWebWidget::webView()
{
  Q_D(qCjyxWebWidget);
  return d->WebView;
}

//-----------------------------------------------------------------------------
QString qCjyxWebWidget::evalJS(const QString &js)
{
  Q_D(qCjyxWebWidget);

  // NOTE: Beginning Qt5.7, the call to runJavaScript are asynchronous,
  // and take a function (lambda) which is called once
  // the script evaluation is completed.
  // Connect to the "evalResult(QString,QString)" signal to get
  // results from the WebView.
  d->WebView->page()->runJavaScript(js, [this,js](const QVariant &v) {
//    qDebug() << js << " returns " << v.toString();
    emit evalResult(js, v.toString());
  });
  return QString();

}

//-----------------------------------------------------------------------------
void qCjyxWebWidget::setHtml(const QString &html, const QUrl &baseUrl)
{
  Q_D(qCjyxWebWidget);

  d->WebView->setHtml(html, baseUrl);
}

//-----------------------------------------------------------------------------
void qCjyxWebWidget::setUrl(const QString &url)
{
  Q_D(qCjyxWebWidget);

  d->WebView->setUrl(QUrl(url));
}

//-----------------------------------------------------------------------------
QString qCjyxWebWidget::url()
{
  Q_D(qCjyxWebWidget);

  return d->WebView->url().toString();
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onDownloadStarted(QNetworkReply* reply)
{
  Q_D(qCjyxWebWidget);
  connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
          SLOT(onDownloadProgress(qint64,qint64)));
  d->DownloadTime.start();
  d->ProgressBar->setVisible(true);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
  Q_D(qCjyxWebWidget);

  // Calculate the download speed
  double speed = bytesReceived * 1000.0 / d->DownloadTime.elapsed();
  QString unit;
  if (speed < 1024)
    {
    unit = "bytes/sec";
    }
  else if (speed < 1024*1024) {
    speed /= 1024;
    unit = "kB/s";
    }
  else
    {
    speed /= 1024*1024;
    unit = "MB/s";
    }

  d->ProgressBar->setFormat(QString("%p% (%1 %2)").arg(speed, 3, 'f', 1).arg(unit));
  d->ProgressBar->setMaximum(bytesTotal);
  d->ProgressBar->setValue(bytesReceived);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onDownloadFinished(QNetworkReply* reply)
{
  Q_D(qCjyxWebWidget);
  Q_UNUSED(reply);
  d->ProgressBar->reset();
  d->ProgressBar->setVisible(false);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::printToPdf(const QString& filePath)
{
  Q_D(qCjyxWebWidget);
  d->WebEnginePage->printToPdf(filePath);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::printToPdf(const QString& filePath, const QPageLayout& pageLayout)
{
    Q_D(qCjyxWebWidget);
    d->WebEnginePage->printToPdf(filePath, pageLayout);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::initJavascript()
{
  Q_D(qCjyxWebWidget);
  d->setDocumentWebkitHidden(!d->WebView->isVisible());
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onLoadStarted()
{
  Q_D(qCjyxWebWidget);
  d->ProgressBar->setFormat("%p%");
  d->ProgressBar->setVisible(true);
  emit loadStarted();
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onLoadProgress(int progress)
{
  emit loadProgress(progress);
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::onLoadFinished(bool ok)
{
  Q_UNUSED(ok);
  Q_D(qCjyxWebWidget);
  d->ProgressBar->reset();
  d->ProgressBar->setVisible(false);
  emit loadFinished(ok);
}

// --------------------------------------------------------------------------
bool qCjyxWebWidget::acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
  Q_D(qCjyxWebWidget);
  Q_ASSERT(d->WebEnginePage);
  if(d->InternalHosts.contains(url.host())
    || url.scheme() == "data" // QWebEngineView::setHtml creates a special URL, which encodes data in the URL, always internal
    || !d->HandleExternalUrlWithDesktopService // all requests are internal
    )
    {
    d->NavigationRequestAccepted = d->WebEnginePage->webEnginePageAcceptNavigationRequest(url, type, isMainFrame);
    }
  else
    {
    if(!QDesktopServices::openUrl(url))
      {
      qWarning() << "Failed to open url:" << url;
      }
    d->NavigationRequestAccepted = false;
    }
  return d->NavigationRequestAccepted;
}

// --------------------------------------------------------------------------
void qCjyxWebWidget::handleSslErrors(QNetworkReply* reply,
                                       const QList<QSslError> &errors)
{
#ifdef QT_NO_SSL
  Q_UNUSED(reply)
  Q_UNUSED(errors)
#else
  foreach (QSslError e, errors)
    {
    qDebug() << "[SSL] [" << qPrintable(reply->url().host().trimmed()) << "]"
             << qPrintable(e.errorString());
    }
#endif
}

// --------------------------------------------------------------------------
bool qCjyxWebWidget::eventFilter(QObject* obj, QEvent* event)
{
  Q_D(qCjyxWebWidget);
  Q_ASSERT(d->WebView == obj);
  if (d->WebView == obj && !event->spontaneous() &&
      (event->type() == QEvent::Show || event->type() == QEvent::Hide))
    {
    d->setDocumentWebkitHidden(!d->WebView->isVisible());
    this->evalJS("if (typeof $ != 'undefined') {"
                 "  $.event.trigger({type: 'webkitvisibilitychange'})"
                 "} else { console.info('JQuery not loaded - Failed to trigger webkitvisibilitychange') }");
    }
  return QObject::eventFilter(obj, event);
}
