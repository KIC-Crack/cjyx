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

#ifndef __qCjyxActionsDialog_h
#define __qCjyxActionsDialog_h

// Qt includes
#include <QDialog>
#include <QScopedPointer>

// Cjyx includes
#include "qCjyxBaseQTGUIExport.h"
class qCjyxActionsDialogPrivate;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxActionsDialog: public QDialog
{
  Q_OBJECT
public:
  typedef QDialog Superclass;
  qCjyxActionsDialog(QWidget* parentWidget =nullptr);
  ~qCjyxActionsDialog() override;

  void addAction(QAction* action, const QString& group=QString());
  void addActions(const QList<QAction*>& actions, const QString& group=QString());
  void clear();

  void setActionsWithNoShortcutVisible(bool visible);
  void setMenuActionsVisible(bool visible);

protected:
  QScopedPointer<qCjyxActionsDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxActionsDialog);
  Q_DISABLE_COPY(qCjyxActionsDialog);
};

#endif
