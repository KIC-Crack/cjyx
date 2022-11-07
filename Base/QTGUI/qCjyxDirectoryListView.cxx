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
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QListView>
#include <QMimeData>
#include <QStandardItemModel>
#include <QUrl>

// QtGUI includes
#include "qCjyxDirectoryListView.h"

// --------------------------------------------------------------------------
// qCjyxDirectoryListViewPrivate

//-----------------------------------------------------------------------------
class qCjyxDirectoryListViewPrivate
{
  Q_DECLARE_PUBLIC(qCjyxDirectoryListView);
protected:
  qCjyxDirectoryListView* const q_ptr;

public:
  qCjyxDirectoryListViewPrivate(qCjyxDirectoryListView& object);
  void init();

  void addDirectory(const QString& path);

  enum
    {
    AbsolutePathRole = Qt::UserRole + 1
    };

  QListView*         ListView;
  QStandardItemModel DirectoryListModel;
};

// --------------------------------------------------------------------------
// qCjyxDirectoryListViewPrivate methods

// --------------------------------------------------------------------------
qCjyxDirectoryListViewPrivate::qCjyxDirectoryListViewPrivate(qCjyxDirectoryListView& object)
  : q_ptr(&object)
  , ListView(nullptr)
{
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListViewPrivate::init()
{
  Q_Q(qCjyxDirectoryListView);

  this->ListView = new QListView();
  this->ListView->setSelectionBehavior(QAbstractItemView::SelectRows);
  this->ListView->setSelectionMode(QAbstractItemView::ExtendedSelection);
  this->ListView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QHBoxLayout * layout = new QHBoxLayout();
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(this->ListView);
  q->setLayout(layout);

  this->ListView->setModel(&this->DirectoryListModel);
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListViewPrivate::addDirectory(const QString& path)
{
  Q_Q(qCjyxDirectoryListView);
  QString absolutePath = QFileInfo(path).absoluteFilePath();
  if (!QFile::exists(absolutePath) || q->hasDirectory(absolutePath))
    {
    return;
    }
  QStandardItem * item = new QStandardItem(path);
  item->setData(QVariant(absolutePath), Qt::ToolTipRole);
  item->setData(QVariant(absolutePath), qCjyxDirectoryListViewPrivate::AbsolutePathRole);
  this->DirectoryListModel.appendRow(item);
}

// --------------------------------------------------------------------------
// qCjyxDirectoryListView methods

// --------------------------------------------------------------------------
qCjyxDirectoryListView::qCjyxDirectoryListView(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxDirectoryListViewPrivate(*this))
{
  Q_D(qCjyxDirectoryListView);
  d->init();
  setAcceptDrops(true);
}

// --------------------------------------------------------------------------
qCjyxDirectoryListView::~qCjyxDirectoryListView() = default;

// --------------------------------------------------------------------------
QStringList qCjyxDirectoryListView::directoryList(bool absolutePath)const
{
  Q_D(const qCjyxDirectoryListView);
  QStringList directoryList;
  int role = Qt::DisplayRole;
  if (absolutePath)
    {
    role = qCjyxDirectoryListViewPrivate::AbsolutePathRole;
    }
  for(int i = 0; i < d->DirectoryListModel.rowCount(); ++i)
    {
    directoryList << d->DirectoryListModel.data(d->DirectoryListModel.index(i, 0), role).toString();
    }
  return directoryList;
}

// --------------------------------------------------------------------------
QStringList qCjyxDirectoryListView::selectedDirectoryList(bool absolutePath)const
{
  Q_D(const qCjyxDirectoryListView);
  QStringList directoryList;
  int role = Qt::DisplayRole;
  if (absolutePath)
    {
    role = qCjyxDirectoryListViewPrivate::AbsolutePathRole;
    }
  QModelIndexList selectedIndexes = d->ListView->selectionModel()->selectedRows();
  foreach(const QModelIndex& index, selectedIndexes)
    {
    directoryList << d->DirectoryListModel.data(index, role).toString();
    }
  return directoryList;
}

// --------------------------------------------------------------------------
bool qCjyxDirectoryListView::hasDirectory(const QString& path)const
{
  Q_D(const qCjyxDirectoryListView);
  QString absolutePath = QFileInfo(path).absoluteFilePath();
  QModelIndexList foundIndexes = d->DirectoryListModel.match(
        d->DirectoryListModel.index(0, 0), qCjyxDirectoryListViewPrivate::AbsolutePathRole,
        QVariant(absolutePath), /* hits = */ 1, Qt::MatchExactly | Qt::MatchWrap);
  Q_ASSERT(foundIndexes.size() < 2);
  return (foundIndexes.size() != 0);
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::addDirectory(const QString& path)
{
  Q_D(qCjyxDirectoryListView);
  d->addDirectory(path);
  emit this->directoryListChanged();
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::removeDirectory(const QString& path)
{
  Q_D(qCjyxDirectoryListView);
  QList<QStandardItem*> foundItems = d->DirectoryListModel.findItems(path);
  Q_ASSERT(foundItems.count() < 2);
  if (foundItems.count() == 1)
    {
    d->DirectoryListModel.removeRow(foundItems.at(0)->row());
    emit this->directoryListChanged();
    }
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::removeSelectedDirectories()
{
  Q_D(qCjyxDirectoryListView);

  QModelIndexList selectedIndexes = d->ListView->selectionModel()->selectedRows();
  bool selectedCount = selectedIndexes.count();
  while(selectedIndexes.count() > 0)
    {
    d->DirectoryListModel.removeRow(selectedIndexes.at(0).row());
    selectedIndexes = d->ListView->selectionModel()->selectedRows();
    }
  if (selectedCount)
    {
    emit this->directoryListChanged();
    }
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::selectAllDirectories()
{
  Q_D(qCjyxDirectoryListView);
  d->ListView->selectAll();
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::clearDirectorySelection()
{
  Q_D(qCjyxDirectoryListView);
  d->ListView->clearSelection();
}

// --------------------------------------------------------------------------
void qCjyxDirectoryListView::setDirectoryList(const QStringList& paths)
{
  Q_D(qCjyxDirectoryListView);

  if (paths.count() == this->directoryList().count())
    {
    int found = 0;
    foreach(const QString& path, paths)
      {
      if (this->hasDirectory(path))
        {
        ++found;
        }
      }
    if (found == paths.count())
      {
      return;
      }
    }

  d->DirectoryListModel.removeRows(0, d->DirectoryListModel.rowCount());

  foreach(const QString& path, paths)
    {
    d->addDirectory(path);
    }
  emit this->directoryListChanged();
}

//---------------------------------------------------------------------------
void qCjyxDirectoryListView::dragEnterEvent(QDragEnterEvent *event)
{
  event->mimeData()->hasFormat("text/uri-list");
    {
    event->accept();
    }
}

//-----------------------------------------------------------------------------
void qCjyxDirectoryListView::dropEvent(QDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
    {
    if (!url.isValid() || url.isEmpty())
      {
      continue;
      }
    // convert QUrl to local path
    QString localPath = url.toLocalFile();
    QFileInfo pathInfo;
    pathInfo.setFile(localPath);
    if (pathInfo.isDir())
      {
      // it is a directory, add it as is
      addDirectory(localPath);
      }
    else if (pathInfo.isFile())
      {
      // it is a file, add the parent directory
      addDirectory(pathInfo.absolutePath());
      }
    }
}
