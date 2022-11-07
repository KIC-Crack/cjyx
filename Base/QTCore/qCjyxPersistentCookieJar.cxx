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
#include <QDir>
#include <QFileInfo>
#include <QFileInfo>
#include <QNetworkCookie>
#include <QSettings>
#include <QStringList>

// QtCore includes
#include "qCjyxPersistentCookieJar.h"

//-----------------------------------------------------------------------------
class qCjyxPersistentCookieJarPrivate
{
  Q_DECLARE_PUBLIC(qCjyxPersistentCookieJar);
protected:
  qCjyxPersistentCookieJar* const q_ptr;
public:
  qCjyxPersistentCookieJarPrivate(qCjyxPersistentCookieJar& object);

  void init();

  QString FilePath;
};

//-----------------------------------------------------------------------------
// qCjyxPersistentCookieJarPrivate methods

// --------------------------------------------------------------------------
qCjyxPersistentCookieJarPrivate::
qCjyxPersistentCookieJarPrivate(qCjyxPersistentCookieJar& object) :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxPersistentCookieJarPrivate::init()
{
  QSettings::Format savedFormat = QSettings::defaultFormat();
  QSettings::setDefaultFormat(QSettings::IniFormat);
  this->FilePath = QFileInfo(QFileInfo(QSettings().fileName()).dir(), "cookies.ini").filePath();
  QSettings::setDefaultFormat(savedFormat);
}

//-----------------------------------------------------------------------------
// qCjyxPersistentCookieJar methods

//-----------------------------------------------------------------------------
qCjyxPersistentCookieJar::qCjyxPersistentCookieJar(QObject * parent)
  :Superclass(parent), d_ptr(new qCjyxPersistentCookieJarPrivate(*this))
{
  Q_D(qCjyxPersistentCookieJar);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxPersistentCookieJar::~qCjyxPersistentCookieJar() = default;

//-----------------------------------------------------------------------------
QString qCjyxPersistentCookieJar::filePath()const
{
  Q_D(const qCjyxPersistentCookieJar);
  return d->FilePath;
}

//-----------------------------------------------------------------------------
void qCjyxPersistentCookieJar::setFilePath(const QString& filePath)
{
  Q_D(qCjyxPersistentCookieJar);
  d->FilePath = filePath;
}

//-----------------------------------------------------------------------------
QList<QNetworkCookie> qCjyxPersistentCookieJar::cookiesForUrl( const QUrl & url)const
{
  Q_D(const qCjyxPersistentCookieJar);
  QSettings settings(d->FilePath, QSettings::IniFormat);
  QList<QNetworkCookie> cookieList;
  settings.beginGroup(url.host());
  QStringList keys = settings.childKeys();
  foreach(const QString& key, keys)
    {
    cookieList << QNetworkCookie(key.toUtf8(), settings.value(key).toString().toUtf8());
    }
  return cookieList;
}

//-----------------------------------------------------------------------------
bool qCjyxPersistentCookieJar::
setCookiesFromUrl(const QList<QNetworkCookie> & cookieList, const QUrl & url)
{
  Q_D(qCjyxPersistentCookieJar);
  QSettings settings(d->FilePath, QSettings::IniFormat);
  settings.beginGroup(url.host());
  foreach(const QNetworkCookie& cookie, cookieList)
    {
    settings.setValue(cookie.name(), cookie.value());
    }
  settings.sync();
  return settings.status() == QSettings::NoError;
}
