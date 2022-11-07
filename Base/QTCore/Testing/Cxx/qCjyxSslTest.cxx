
// Qt includes
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslSocket>

// CTK includes
#include <ctkTest.h>

// Cjyx includes
#include "qCjyxCoreApplication.h"

// ----------------------------------------------------------------------------
class qCjyxSslTester: public QObject
{
  Q_OBJECT
  typedef qCjyxSslTester Self;

private slots:
  void testSupportsSsl();
  void testLoadCaCertificates();
  void testHttpsConnection();
  void testHttpsConnection_data();
};

// ----------------------------------------------------------------------------
void qCjyxSslTester::testSupportsSsl()
{
  QVERIFY(QSslSocket::supportsSsl());
}

// ----------------------------------------------------------------------------
void qCjyxSslTester::testLoadCaCertificates()
{
  QVERIFY(qCjyxCoreApplication::loadCaCertificates(
            QProcessEnvironment::systemEnvironment().value("CJYX_HOME")));
}

// ----------------------------------------------------------------------------
class SslEventLoop : public QEventLoop
{
  Q_OBJECT
public:
  SslEventLoop() = default;
public slots:
  void onSslErrors(QNetworkReply* reply, const QList<QSslError>& sslErrors)
  {
    Q_UNUSED(reply);
    foreach(const QSslError& sslError, sslErrors)
      {
      this->SslErrors << sslError.error();
      this->SslErrorStrings << sslError.errorString();
      }
    this->quit();
  }
public:
  QList<QSslError::SslError> SslErrors;
  QStringList SslErrorStrings;
};

Q_DECLARE_METATYPE(QList<QSslError::SslError>)

// ----------------------------------------------------------------------------
void qCjyxSslTester::testHttpsConnection()
{
  QFETCH(QString, url);
  QFETCH(QList<QSslError::SslError>, expectedSslErrors);
  QFETCH(QNetworkReply::NetworkError, expectedNetworkError);
  QFETCH(int, expectedStatusCode);

  qCjyxCoreApplication::loadCaCertificates(
        QProcessEnvironment::systemEnvironment().value("CJYX_HOME"));

  QNetworkAccessManager * manager = new QNetworkAccessManager(this);

  SslEventLoop eventLoop;
  QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                   &eventLoop, SLOT(quit()));
  QObject::connect(manager, SIGNAL(sslErrors(QNetworkReply*, QList<QSslError>)),
            &eventLoop, SLOT(onSslErrors(QNetworkReply*, QList<QSslError>)));

  QNetworkReply * reply = manager->get(QNetworkRequest(QUrl(url)));
  eventLoop.exec();

  QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

  QCOMPARE(eventLoop.SslErrors, expectedSslErrors);
  QCOMPARE(reply->error(), expectedNetworkError);
  QCOMPARE(statusCode.toInt(), expectedStatusCode);
}

// ----------------------------------------------------------------------------
void qCjyxSslTester::testHttpsConnection_data()
{
  QTest::addColumn<QString>("url");
  QTest::addColumn<QList<QSslError::SslError> >("expectedSslErrors");
  QTest::addColumn<QNetworkReply::NetworkError>("expectedNetworkError");
  QTest::addColumn<int>("expectedStatusCode");

  QTest::newRow("invalid-HostNotFoundError-0")
      << "http://i.n.v.a.l.i.d"
      << (QList<QSslError::SslError>())
      << QNetworkReply::HostNotFoundError << 0;

  QTest::newRow("cjyx-clear-with-redirect-NoError-301")
      << "http://slicer.org"
      << (QList<QSslError::SslError>())
      << QNetworkReply::NoError << 301;

  QTest::newRow("cjyx-secured-MovedPermanently-301")
      << "https://slicer.org"
      << (QList<QSslError::SslError>())
      << QNetworkReply::NoError << 301;

  QTest::newRow("other-secured-NoError-200")
      << "https://www.eff.org/https-everywhere"
      << (QList<QSslError::SslError>())
      << QNetworkReply::NoError << 200;
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxSslTest)
#include "moc_qCjyxSslTest.cxx"
