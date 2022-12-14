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

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QSettings>

// CTK includes

// QtGUI includes
#include "qCjyxRelativePathMapper.h"
#include "qCjyxSettingsCachePanel.h"
#include "ui_qCjyxSettingsCachePanel.h"

// MRML includes
#include <vtkCacheManager.h>

// VTK includes
#include <vtkCommand.h>

// --------------------------------------------------------------------------
// qCjyxSettingsCachePanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsCachePanelPrivate: public Ui_qCjyxSettingsCachePanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsCachePanel);
protected:
  qCjyxSettingsCachePanel* const q_ptr;

public:
  qCjyxSettingsCachePanelPrivate(qCjyxSettingsCachePanel& object);
  void init();

  vtkCacheManager* CacheManager;
};

// --------------------------------------------------------------------------
// qCjyxSettingsCachePanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsCachePanelPrivate::qCjyxSettingsCachePanelPrivate(qCjyxSettingsCachePanel& object)
  :q_ptr(&object)
{
  this->CacheManager = nullptr;
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanelPrivate::init()
{
  Q_Q(qCjyxSettingsCachePanel);

  this->setupUi(q);
  QObject::connect(this->CachePathButton, SIGNAL(directoryChanged(QString)),
                   q, SLOT(setCachePath(QString)));
  QObject::connect(this->CacheSizeSpinBox, SIGNAL(valueChanged(int)),
                   q, SLOT(setCacheSize(int)));
  QObject::connect(this->CacheFreeBufferSpinBox, SIGNAL(valueChanged(int)),
                   q, SLOT(setCacheFreeBufferSize(int)));
  QObject::connect(this->ForceRedownloadCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setForceRedownload(bool)));
  QObject::connect(this->ClearCachePushButton, SIGNAL(clicked()),
                   q, SLOT(clearCache()));

  // hide for now
  //this->FilesListLabel->setVisible(false);
  //this->FilesListWidget->setVisible(false);
}

// --------------------------------------------------------------------------
// qCjyxSettingsCachePanel methods

// --------------------------------------------------------------------------
qCjyxSettingsCachePanel::qCjyxSettingsCachePanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsCachePanelPrivate(*this))
{
  Q_D(qCjyxSettingsCachePanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsCachePanel::~qCjyxSettingsCachePanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::setCacheManager(vtkCacheManager* cacheManager)
{
  Q_D(qCjyxSettingsCachePanel);
  if (d->CacheManager == cacheManager)
    {
    return;
    }
  qvtkReconnect(d->CacheManager, cacheManager, vtkCommand::ModifiedEvent,
                this, SLOT(updateFromCacheManager()));
  d->CacheManager = cacheManager;

  // Default values
  this->updateFromCacheManager();

  qCjyxRelativePathMapper* relativePathMapper = new qCjyxRelativePathMapper(d->CachePathButton, "directory", SIGNAL(directoryChanged(QString)));
  this->registerProperty("Cache/Path", relativePathMapper, "relativePath",
                         SIGNAL(relativePathChanged(QString)));
  this->registerProperty("Cache/Size", d->CacheSizeSpinBox, "value",
                         SIGNAL(valueChanged(int)));
  this->registerProperty("Cache/FreeBufferSize", d->CacheFreeBufferSpinBox, "value",
                         SIGNAL(valueChanged(int)));
  this->registerProperty("Cache/ForceRedownload", d->ForceRedownloadCheckBox, "checked",
                         SIGNAL(toggled(bool)));
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::updateFromCacheManager()
{
  Q_D(qCjyxSettingsCachePanel);
  this->setEnabled(d->CacheManager != nullptr);
  if (d->CacheManager == nullptr)
    {
    return;
    }
  d->CachePathButton->setDirectory(
    QString(d->CacheManager->GetRemoteCacheDirectory()));
  d->CacheSizeSpinBox->setValue(
    d->CacheManager->GetRemoteCacheLimit() );

  d->CacheManager->CacheSizeCheck();
  d->UsedCacheSizeLabel->setText(
    tr("%1MB used").arg(
    QString::number(qMax(d->CacheManager->GetCurrentCacheSize(), 0.f), 'f',2)) );
  d->CacheManager->FreeCacheBufferCheck();
  d->FreeCacheSizeLabel->setText(
    tr("%1MB free").arg(
    QString::number(d->CacheManager->GetFreeCacheSpaceRemaining(), 'f', 2)) );
  QPalette palette = this->palette();
  if (d->CacheManager->GetCurrentCacheSize() >
      d->CacheManager->GetRemoteCacheLimit())
    {
    palette.setColor(d->UsedCacheSizeLabel->foregroundRole(), Qt::red);
    }
  else if (d->CacheManager->GetFreeCacheSpaceRemaining() <
           d->CacheManager->GetRemoteCacheFreeBufferSize())
    {
    palette.setColor(d->UsedCacheSizeLabel->foregroundRole(), QColor("orange"));
    }
  d->UsedCacheSizeLabel->setPalette(palette);
  d->FreeCacheSizeLabel->setPalette(palette);

  d->CacheFreeBufferSpinBox->setRange(
    0, d->CacheManager->GetRemoteCacheLimit());
  d->CacheFreeBufferSpinBox->setValue(
    d->CacheManager->GetRemoteCacheFreeBufferSize() );
  d->ForceRedownloadCheckBox->setChecked(
    d->CacheManager->GetEnableForceRedownload() == 1 );

  d->FilesListWidget->clear();
  std::vector<std::string> cachedFiles = d->CacheManager->GetCachedFiles();
  for (std::vector<std::string>::const_iterator it = cachedFiles.begin(),
       end = cachedFiles.end();
       it != end; ++it)
    {
    QFileInfo file(it->c_str());
    QListWidgetItem* fileItem = new QListWidgetItem;
    fileItem->setText(file.fileName());
    //fileItem->setToolTip(it->first.c_str());
    d->FilesListWidget->addItem(fileItem);
    }
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::setCachePath(const QString& path)
{
  Q_D(qCjyxSettingsCachePanel);
  d->CacheManager->SetRemoteCacheDirectory(path.toUtf8());
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::setCacheSize(int sizeInMB)
{
  Q_D(qCjyxSettingsCachePanel);
  d->CacheManager->SetRemoteCacheLimit(sizeInMB);
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::setCacheFreeBufferSize(int sizeInMB)
{
  Q_D(qCjyxSettingsCachePanel);
  d->CacheManager->SetRemoteCacheFreeBufferSize(sizeInMB);
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::setForceRedownload(bool force)
{
  Q_D(qCjyxSettingsCachePanel);
  d->CacheManager->SetEnableForceRedownload(force ? 1 : 0);
}

// --------------------------------------------------------------------------
void qCjyxSettingsCachePanel::clearCache()
{
  Q_D(qCjyxSettingsCachePanel);
  Q_ASSERT(d->CacheManager);
  d->CacheManager->ClearCache();
  //this->updateFromCacheManager();
}
