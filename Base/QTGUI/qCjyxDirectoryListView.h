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

#ifndef __qCjyxDirectoryListView_h
#define __qCjyxDirectoryListView_h

// Qt includes
#include <QWidget>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QDragEnterEvent;
class QDropEvent;
class qCjyxDirectoryListViewPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxDirectoryListView : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(QStringList directoryList READ directoryList WRITE setDirectoryList NOTIFY directoryListChanged);
public:
  /// Superclass typedef
  typedef QWidget Superclass;

  /// Constructor
  explicit qCjyxDirectoryListView(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxDirectoryListView() override;

  /// Return True if the \a path has already been added
  bool hasDirectory(const QString& path)const;

  QStringList directoryList(bool absolutePath = false)const;

  QStringList selectedDirectoryList(bool absolutePath = false)const;

  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

public slots:

  /// If \a path exists, add it to the view and emit signal directoryListChanged().
  /// \sa directoryListChanged()
  void addDirectory(const QString& path);

  /// Remove all entries and set \a paths has current list.
  /// The signal directoryListChanged() is emitted if the current list of directories is
  /// different from the provided one.
  /// \sa addDirectory(), directoryListChanged()
  void setDirectoryList(const QStringList& paths);

  /// Remove \a path from the list.
  /// The signal directoryListChanged() is emitted if the path was in the list.
  /// \sa directoryListChanged()
  void removeDirectory(const QString& path);

  /// \sa selectAllDirectories()
  void removeSelectedDirectories();

  /// Select all directories.
  void selectAllDirectories();

  /// Clear the current directory selection.
  void clearDirectorySelection();

signals:
  /// This signal is emitted when a directory is added to the view.
  void directoryListChanged();

protected:
  QScopedPointer<qCjyxDirectoryListViewPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDirectoryListView);
  Q_DISABLE_COPY(qCjyxDirectoryListView);

};

#endif

