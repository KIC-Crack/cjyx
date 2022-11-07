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
#include <QBrush>
#include <QDebug>
#include <QMimeData>
#include <QSortFilterProxyModel>
#include <QStandardItem>
#include <QStandardItemModel>

// QtGUI includes
#include "qCjyxModuleFactoryFilterModel.h"

// --------------------------------------------------------------------------
// qCjyxModuleFactoryFilterModelPrivate

//-----------------------------------------------------------------------------
class qCjyxModuleFactoryFilterModelPrivate
{
  Q_DECLARE_PUBLIC(qCjyxModuleFactoryFilterModel);
protected:
  qCjyxModuleFactoryFilterModel* const q_ptr;

public:
  qCjyxModuleFactoryFilterModelPrivate(qCjyxModuleFactoryFilterModel& object);
  void decodeDataRecursive(QDataStream &stream, QStandardItem *item);
  bool ShowToLoad;
  bool ShowToIgnore;
  bool ShowLoaded;
  bool ShowIgnored;
  bool ShowFailed;
  bool ShowBuiltIn;
  bool ShowTesting;
  bool ShowHidden;

  QStringList ShowModules;
  bool HideAllWhenShowModulesIsEmpty;
};

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModelPrivate::decodeDataRecursive(QDataStream &stream, QStandardItem *item)
{
    int colCount, childCount;
    stream >> *item;
    stream >> colCount >> childCount;
    item->setColumnCount(colCount);

    int childPos = childCount;

    while(childPos > 0) {
        childPos--;
        QStandardItem *child = new QStandardItem();
        decodeDataRecursive(stream, child);
        item->setChild( childPos / colCount, childPos % colCount, child);
    }
}

// --------------------------------------------------------------------------
// qCjyxModulesListViewPrivate methods

// --------------------------------------------------------------------------
qCjyxModuleFactoryFilterModelPrivate::qCjyxModuleFactoryFilterModelPrivate(qCjyxModuleFactoryFilterModel& object)
  :q_ptr(&object)
{
  this->ShowToLoad = true;
  this->ShowLoaded = true;
  this->ShowToIgnore = true;
  this->ShowIgnored = true;
  this->ShowFailed = true;
  this->ShowBuiltIn = true;
  this->ShowTesting = true;
  this->ShowHidden = true;
  this->HideAllWhenShowModulesIsEmpty = false;
}

// --------------------------------------------------------------------------
qCjyxModuleFactoryFilterModel::qCjyxModuleFactoryFilterModel(QObject* parent)
  : Superclass(parent)
  , d_ptr(new qCjyxModuleFactoryFilterModelPrivate(*this))
{
  this->setFilterCaseSensitivity(Qt::CaseInsensitive);
  this->setDynamicSortFilter(true);
}

// --------------------------------------------------------------------------
qCjyxModuleFactoryFilterModel::~qCjyxModuleFactoryFilterModel() = default;

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showToLoad()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowToLoad;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowToLoad(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowToLoad = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showToIgnore()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowToIgnore;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowToIgnore(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowToIgnore = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showLoaded()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowLoaded;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowLoaded(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowLoaded = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showIgnored()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowIgnored;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowIgnored(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowIgnored = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showFailed()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowFailed;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowFailed(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowFailed = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showBuiltIn()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowBuiltIn;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowBuiltIn(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowBuiltIn = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showHidden()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowHidden;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowHidden(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowHidden = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::showTesting()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowTesting;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowTesting(bool show)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->ShowTesting = show;
  this->invalidateFilter();
}

// --------------------------------------------------------------------------
QStringList qCjyxModuleFactoryFilterModel::showModules()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->ShowModules;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setShowModules(const QStringList& modules)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  if (modules == d->ShowModules)
    {
    return;
    }
  d->ShowModules = modules;
  this->setFilterRole(Qt::UserRole);
  if (d->HideAllWhenShowModulesIsEmpty && modules.isEmpty())
    {
    this->setFilterWildcard("hide all modules");
    }
  else
    {
    this->setFilterRegExp(QString("\\b(") + d->ShowModules.join("|") + QString(")\\b"));
    }
  this->sort(0);
  emit showModulesChanged(d->ShowModules);
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::hideAllWhenShowModulesIsEmpty()const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  return d->HideAllWhenShowModulesIsEmpty;
}

// --------------------------------------------------------------------------
void qCjyxModuleFactoryFilterModel::setHideAllWhenShowModulesIsEmpty(bool hide)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  d->HideAllWhenShowModulesIsEmpty = hide;
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::lessThan(const QModelIndex& leftIndex,
                                               const QModelIndex& rightIndex)const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  QString leftModule = this->sourceModel()->data(leftIndex, Qt::UserRole).toString();
  QString rightModule = this->sourceModel()->data(rightIndex, Qt::UserRole).toString();
  if (d->ShowModules.contains(leftModule) &&
      d->ShowModules.contains(rightModule))
    {
    return d->ShowModules.indexOf(leftModule) < d->ShowModules.indexOf(rightModule);
    }
  return this->Superclass::lessThan(leftIndex, rightIndex);
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent)const
{
  Q_D(const qCjyxModuleFactoryFilterModel);
  QModelIndex sourceIndex = this->sourceModel()->index(sourceRow, 0, sourceParent);
  if (!d->ShowToLoad)
    {
    if (this->sourceModel()->data(sourceIndex, Qt::CheckStateRole).toUInt() == Qt::Checked)
      {
      return false;
      }
    }
  if (!d->ShowToIgnore)
    {
    if (this->sourceModel()->data(sourceIndex, Qt::CheckStateRole).toUInt() == Qt::Unchecked)
      {
      return false;
      }
    }
  if (!d->ShowLoaded)
    {
    if (this->sourceModel()->data(sourceIndex, Qt::ForegroundRole).value<QBrush>() == QBrush())
      {
      return false;
      }
    }
  if (!d->ShowIgnored)
    {
    if (this->sourceModel()->data(sourceIndex, Qt::ForegroundRole).value<QBrush>() != QBrush() &&
        this->sourceModel()->data(sourceIndex, Qt::ForegroundRole).value<QBrush>() != QBrush(Qt::red))
      {
      return false;
      }
    }
  if (!d->ShowFailed)
    {
    if (this->sourceModel()->data(sourceIndex, Qt::ForegroundRole).value<QBrush>() == QBrush(Qt::red))
      {
      return false;
      }
    }
  if (!d->ShowBuiltIn)
    {
    // qCjyxModulesListViewPrivate::IsBuiltInRole = Qt::UserRole+1
    if (this->sourceModel()->data(sourceIndex, Qt::UserRole+1).toBool())
      {
      return false;
      }
    }
  if (!d->ShowTesting)
    {
    // qCjyxModulesListViewPrivate::IsTestingRole = Qt::UserRole+2
    if (this->sourceModel()->data(sourceIndex, Qt::UserRole+2).toBool())
      {
      return false;
      }
    }
  if (!d->ShowHidden)
    {
    // qCjyxModulesListViewPrivate::IsHiddenRole = Qt::UserRole+3
    if (this->sourceModel()->data(sourceIndex, Qt::UserRole+3).toBool())
      {
      return false;
      }
    }

  return this->Superclass::filterAcceptsRow(sourceRow, sourceParent);
}

// --------------------------------------------------------------------------
Qt::DropActions qCjyxModuleFactoryFilterModel::supportedDropActions()const
{
  return Qt::CopyAction;
}

// --------------------------------------------------------------------------
bool qCjyxModuleFactoryFilterModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                                      int row, int column, const QModelIndex &parent)
{
  Q_D(qCjyxModuleFactoryFilterModel);
  // check if the action is supported
  if (!data || !(action == Qt::CopyAction))
    return false;
  // check if the format is supported
  QString format = QLatin1String("application/x-qstandarditemmodeldatalist");
  if (!data->hasFormat(format))
    return QAbstractItemModel::dropMimeData(data, action, row, column, parent);

  if (row > rowCount(parent))
    row = rowCount(parent);
  if (row == -1)
    row = rowCount(parent);
  if (column == -1)
    column = 0;

  // decode and insert
  QByteArray encoded = data->data(format);
  QDataStream stream(&encoded, QIODevice::ReadOnly);


  //code based on QAbstractItemModel::decodeData
  // adapted to work with QStandardItem
  int top = INT_MAX;
  int left = INT_MAX;
  int bottom = 0;
  int right = 0;
  QVector<int> rows, columns;
  QVector<QStandardItem *> items;

  while (!stream.atEnd())
    {
    int r, c;
    QStandardItem *item = new QStandardItem;
    stream >> r >> c;
    d->decodeDataRecursive(stream, item);

    rows.append(r);
    columns.append(c);
    items.append(item);
    top = qMin(r, top);
    left = qMin(c, left);
    bottom = qMax(r, bottom);
    right = qMax(c, right);
    }
  QStringList newShowModules = this->showModules();
  foreach(QStandardItem* item, items)
    {
    newShowModules << item->data(Qt::UserRole).toString();
    }
  newShowModules.removeDuplicates();
  this->setShowModules(newShowModules);
  return true;
}
