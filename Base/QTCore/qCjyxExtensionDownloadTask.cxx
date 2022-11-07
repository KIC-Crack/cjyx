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

// Self includes
#include "qCjyxExtensionDownloadTask.h"

// Qt includes
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSettings>
#include <QStandardItemModel>
#include <QTemporaryFile>
#include <QTextStream>
#include <QUrl>

// QtCore includes
#include "qCjyxExtensionsManagerModel.h"

//-----------------------------------------------------------------------------
class qCjyxExtensionDownloadTaskPrivate
{
public:
  QNetworkReply* Reply;
  QString ExtensionName;
  QString ArchiveName;
  QVariantMap Metadata;
  bool InstallDependencies{true};
};

/*
  QString extensionName = extensionMetadata.value("extensionname").toString();
  QString archiveName = extensionMetadata.value("archivename").toString();
*/

//-----------------------------------------------------------------------------
qCjyxExtensionDownloadTask::qCjyxExtensionDownloadTask(
  QNetworkReply* reply, QObject* parent)
  : QObject(parent), d_ptr(new qCjyxExtensionDownloadTaskPrivate)
{
  Q_D(qCjyxExtensionDownloadTask);

  reply->setParent(this);

  connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
          this, SLOT(emitProgress(qint64,qint64)));
  connect(reply, SIGNAL(finished()), this, SLOT(emitFinished()));
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
          this, SLOT(emitError(QNetworkReply::NetworkError)));

  d->Reply = reply;
}

//-----------------------------------------------------------------------------
qCjyxExtensionDownloadTask::~qCjyxExtensionDownloadTask() = default;

//-----------------------------------------------------------------------------
QVariantMap qCjyxExtensionDownloadTask::metadata() const
{
  Q_D(const qCjyxExtensionDownloadTask);
  return d->Metadata;
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::setMetadata(const QVariantMap& md)
{
  Q_D(qCjyxExtensionDownloadTask);
  d->Metadata = md;
  if (d->ExtensionName.isEmpty())
    {
    d->ExtensionName = md.value("extensionname").toString();
    }
  if (d->ArchiveName.isEmpty())
    {
    d->ArchiveName = md.value("archivename").toString();
    }
}

//-----------------------------------------------------------------------------
QString qCjyxExtensionDownloadTask::extensionName() const
{
  Q_D(const qCjyxExtensionDownloadTask);
  return d->ExtensionName;
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::setExtensionName(const QString& name)
{
  Q_D(qCjyxExtensionDownloadTask);
  d->ExtensionName = name;
}

//-----------------------------------------------------------------------------
QString qCjyxExtensionDownloadTask::archiveName() const
{
  Q_D(const qCjyxExtensionDownloadTask);
  return d->ArchiveName;
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::setArchiveName(const QString& name)
{
  Q_D(qCjyxExtensionDownloadTask);
  d->ArchiveName = name;
}

//-----------------------------------------------------------------------------
QNetworkReply* qCjyxExtensionDownloadTask::reply() const
{
  Q_D(const qCjyxExtensionDownloadTask);
  return d->Reply;
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::emitProgress(qint64 received, qint64 total)
{
  emit this->progress(this, received, total);
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::emitFinished()
{
  emit this->finished(this);
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::emitError(QNetworkReply::NetworkError error)
{
  emit this->error(this, error);
}

//-----------------------------------------------------------------------------
bool qCjyxExtensionDownloadTask::installDependencies() const
{
  Q_D(const qCjyxExtensionDownloadTask);
  return d->InstallDependencies;
}

//-----------------------------------------------------------------------------
void qCjyxExtensionDownloadTask::setInstallDependencies(bool install)
{
  Q_D(qCjyxExtensionDownloadTask);
  d->InstallDependencies = install;
}
