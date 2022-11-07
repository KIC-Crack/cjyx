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

#ifndef __qCjyxPersistentCookieJar_h
#define __qCjyxPersistentCookieJar_h

// Qt includes
#include <QNetworkCookieJar>

// QtCore includes
#include "qCjyxBaseQTCoreExport.h"

class qCjyxPersistentCookieJarPrivate;

/// qCjyxPersistentCookieJar provides a mechanism allowing to store persistently cookies
/// when associated with an instance of QNetworkAccessManager.
///
/// The cookies will be storted in a INI config file. By default, the config file will
/// be located in the directory associated with the current application settings and will be
/// named cookies.ini. This could be overwritten using qCjyxPersistentCookieJar::setFilePath
///
/// A cookie jar can be associated with a QNetworkAccessManager using method QNetworkAccessManager::setCookieJar
///
/// \sa QSettings::IniFormat, QNetworkAccessManager::setCookieJar
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxPersistentCookieJar: public QNetworkCookieJar
{
public:
  typedef QNetworkCookieJar Superclass;
  qCjyxPersistentCookieJar(QObject *parent = nullptr);
  ~qCjyxPersistentCookieJar() override;

  QString filePath()const;
  void setFilePath(const QString& filePath);

  QList<QNetworkCookie> cookiesForUrl(const QUrl & url) const override;
  bool setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url) override;

protected:
  QScopedPointer<qCjyxPersistentCookieJarPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxPersistentCookieJar);
  Q_DISABLE_COPY(qCjyxPersistentCookieJar);
};

#endif

