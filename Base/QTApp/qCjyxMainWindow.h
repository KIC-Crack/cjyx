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

#ifndef __qCjyxMainWindow_h
#define __qCjyxMainWindow_h

// Qt includes
#include <QMainWindow>
#include <QVariantMap>

// CTK includes
#include <ctkErrorLogModel.h>

// Cjyx includes
#include "qCjyxBaseQTAppExport.h"
#include "qCjyxIO.h"
#include "vtkCjyxConfigure.h" // For Cjyx_BUILD_DICOM_SUPPORT, Cjyx_USE_PYTHONQT, Cjyx_USE_QtTesting

class ctkErrorLogWidget;
class ctkPythonConsole;
class qCjyxModuleSelectorToolBar;
class qCjyxMainWindowPrivate;
class vtkMRMLAbstractViewNode;
class vtkObject;

class Q_CJYX_BASE_QTAPP_EXPORT qCjyxMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  typedef QMainWindow Superclass;

  qCjyxMainWindow(QWidget *parent=nullptr);
  ~qCjyxMainWindow() override;

  /// Return a pointer to the module selector toolbar that can change the
  /// current module.
  /// \sa pythonConsole(), errorLogWidget()
  Q_INVOKABLE qCjyxModuleSelectorToolBar* moduleSelector()const;

#ifdef Cjyx_USE_PYTHONQT
  /// Return a pointer to the python console.
  /// \sa moduleSelector(), errorLogWidget()
  Q_INVOKABLE ctkPythonConsole* pythonConsole()const;
#endif
  /// Return a pointer to the error log widget.
  /// \sa moduleSelector(), pythonConsole()
  Q_INVOKABLE ctkErrorLogWidget* errorLogWidget()const;

  /// Return the view node that is temporarily shown maximized in the view layout.
  Q_INVOKABLE vtkMRMLAbstractViewNode* layoutMaximizedViewNode();

public slots:
  virtual void setHomeModuleCurrent();
  virtual void restoreToolbars();

  virtual void on_FileFavoriteModulesAction_triggered();
  virtual void on_FileAddDataAction_triggered();
  virtual void on_FileLoadDataAction_triggered();
  virtual void on_FileImportSceneAction_triggered();
  virtual void on_FileLoadSceneAction_triggered();
  virtual void on_FileAddVolumeAction_triggered();
  virtual void on_FileAddTransformAction_triggered();
  virtual void on_FileSaveSceneAction_triggered();
  virtual void on_FileExitAction_triggered();
  virtual void onFileRecentLoadedActionTriggered();
  virtual void on_SDBSaveToDirectoryAction_triggered();
  virtual void on_SDBSaveToMRBAction_triggered();
  virtual void on_FileCloseSceneAction_triggered();
  virtual void on_LoadDICOMAction_triggered();

  virtual void on_EditRecordMacroAction_triggered();
  virtual void on_EditPlayMacroAction_triggered();
  virtual void on_EditUndoAction_triggered();
  virtual void on_EditRedoAction_triggered();

  virtual void on_ModuleHomeAction_triggered();

  virtual void onLayoutActionTriggered(QAction* action);
  virtual void onLayoutCompareActionTriggered(QAction* action);
  virtual void onLayoutCompareWidescreenActionTriggered(QAction* action);
  virtual void onLayoutCompareGridActionTriggered(QAction* action);

  /// Set the view layout.
  virtual void setLayout(int);

  /// Makes a view displayed maximized (taking the entire area) of the view layout.
  /// Setting the value to nullptr restores the original view layout.
  virtual void setLayoutMaximizedViewNode(vtkMRMLAbstractViewNode*);

  virtual void setLayoutNumberOfCompareViewRows(int);
  virtual void setLayoutNumberOfCompareViewColumns(int);

  virtual void onPythonConsoleToggled(bool);

  virtual void on_WindowErrorLogAction_triggered();
  virtual void on_WindowToolbarsResetToDefaultAction_triggered();

  virtual void on_EditApplicationSettingsAction_triggered();
  virtual void on_CutAction_triggered();
  virtual void on_CopyAction_triggered();
  virtual void on_PasteAction_triggered();
  virtual void on_ViewExtensionsManagerAction_triggered();

  virtual void on_ShowStatusBarAction_triggered(bool);

  /// Write GUI state to application settings.
  ///
  /// GUI state includes:
  /// - main window state and geometry (only if MainWindow/geometry application setting is
  ///   enabled or force argument is set to true)
  /// - current view layout ID
  /// - favorite modules
  /// - recently loaded files
  ///
  /// \sa restoreGUIState()
  virtual void saveGUIState(bool force=false);

  /// Read GUI state from application settings and update the user interface accordingly.
  /// \sa saveGUIState()
  virtual void restoreGUIState(bool force=false);

  virtual void addFileToRecentFiles(const qCjyxIO::IOProperties& fileProperties);

signals:
  /// Emitted when the window is first shown to the user.
  /// \sa showEvent(QShowEvent *)
  void initialWindowShown();

protected slots:
  virtual void onModuleLoaded(const QString& moduleName);
  virtual void onModuleAboutToBeUnloaded(const QString& moduleName);
  virtual void onNewFileLoaded(const qCjyxIO::IOProperties &fileProperties);
  virtual void onFileSaved(const qCjyxIO::IOProperties& fileProperties);

  virtual void onMRMLSceneModified(vtkObject*);
  virtual void onLayoutChanged(int);
  virtual void onWarningsOrErrorsOccurred(ctkErrorLogLevel::LogLevel logLevel);

  // Show/hide update indicator on Extensions Manager toolbar icon
  void setExtensionUpdatesAvailable(bool updateAvailable);

#ifdef Cjyx_USE_PYTHONQT
  virtual void onPythonConsoleUserInput(const QString&);
#endif

protected:
  /// Connect MainWindow action with slots defined in MainWindowCore
  virtual void setupMenuActions();

  /// Open Python interactor if it was requested
  virtual void pythonConsoleInitialDisplay();

  /// Open a popup to warn the user Cjyx is not for clinical use.
  virtual void disclaimer();

  /// Forward the dragEnterEvent to the IOManager which will
  /// decide if it could accept a drag/drop or not.
  /// \sa dropEvent()
  void dragEnterEvent(QDragEnterEvent *event) override;

  /// Forward the dropEvent to the IOManager.
  /// \sa dragEnterEvent()
  void dropEvent(QDropEvent *event) override;

  /// Reimplemented to catch activationChange/show/hide events.
  /// More specifically it allows to:
  ///  1. update the state of the errorLog and python console QAction when
  ///  associated dialog are visible or not.
  ///  2. set the state of ErrorLog button based on the activation state of
  ///  the error log dialog.
  bool eventFilter(QObject* object, QEvent* event) override;

  void closeEvent(QCloseEvent *event) override;
  void showEvent(QShowEvent *event) override;

  void changeEvent(QEvent* event) override;

protected:
  QScopedPointer<qCjyxMainWindowPrivate> d_ptr;

  qCjyxMainWindow(qCjyxMainWindowPrivate* pimpl, QWidget* parent);

private:
  Q_DECLARE_PRIVATE(qCjyxMainWindow);
  Q_DISABLE_COPY(qCjyxMainWindow);
};

#endif
