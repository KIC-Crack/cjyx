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

#ifndef __qCjyxMainWindow_p_h
#define __qCjyxMainWindow_p_h

// Qt includes
#include <QQueue>
class QToolButton;

// Cjyx includes
#include "qCjyxBaseQTAppExport.h"
#include "qCjyxMainWindow.h"
#include <qCjyxIO.h>
#include "ui_qCjyxMainWindow.h"

class qCjyxModuleSelectorToolBar;
class qCjyxMainWindowPrivate;
class qCjyxLayoutManager;

//-----------------------------------------------------------------------------
class Q_CJYX_BASE_QTAPP_EXPORT qCjyxMainWindowPrivate
  : public Ui_qCjyxMainWindow
{
  Q_DECLARE_PUBLIC(qCjyxMainWindow);
protected:
  qCjyxMainWindow* const q_ptr;

public:
  typedef qCjyxMainWindowPrivate Self;
  qCjyxMainWindowPrivate(qCjyxMainWindow& object);
  virtual ~qCjyxMainWindowPrivate();

  virtual void init();
  virtual void setupUi(QMainWindow * mainWindow);
  virtual void setupStatusBar();

  virtual void setupRecentlyLoadedMenu(const QList<qCjyxIO::IOProperties>& fileProperties);

  virtual void filterRecentlyLoadedFileProperties();

  static QList<qCjyxIO::IOProperties> readRecentlyLoadedFiles();
  static void writeRecentlyLoadedFiles(const QList<qCjyxIO::IOProperties>& fileProperties);

  virtual bool confirmCloseApplication();
  virtual bool confirmCloseScene();

  void setErrorLogIconHighlighted(bool);

  void updatePythonConsolePalette();

#ifdef Cjyx_USE_PYTHONQT
  QDockWidget*                    PythonConsoleDockWidget;
  QAction*                        PythonConsoleToggleViewAction;
#endif
  ctkErrorLogWidget*              ErrorLogWidget;
  QToolButton*                    ErrorLogToolButton;
  QToolButton*                    LayoutButton;
  qCjyxModuleSelectorToolBar*   ModuleSelectorToolBar;
  QStringList                     FavoriteModules;
  qCjyxLayoutManager*           LayoutManager;
  QQueue<qCjyxIO::IOProperties> RecentlyLoadedFileProperties;

  QByteArray                      StartupState;

  bool                            WindowInitialShowCompleted;
  bool                            IsClosing;
};

#endif
