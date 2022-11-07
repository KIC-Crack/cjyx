#ifndef __qCjyxIOManager_h
#define __qCjyxIOManager_h

// Qt includes
#include <QList>
#include <QString>
#include <QUrl>

// CTK includes
#include <ctkVTKObject.h>

// CjyxQ includes
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include "qCjyxFileDialog.h"

#include "qCjyxBaseQTGUIExport.h"

/// Qt declarations
class QDragEnterEvent;
class QDropEvent;
class QWidget;

class qCjyxIOManagerPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxIOManager : public qCjyxCoreIOManager
{
  Q_OBJECT;
  QVTK_OBJECT;
public:
  typedef qCjyxCoreIOManager Superclass;
  qCjyxIOManager(QObject* parent = nullptr);
  ~qCjyxIOManager() override;

  /// Search for the most appropriate dialog based on the action and fileType,
  /// and open it. Once the user select the file(s), the action (read or write)
  /// is done. Note that to write a node, the "nodeID" property must be passed.
  /// If no dialog is registered for a given fileType (e.g.
  /// QString("SceneFile")), a default dialog (qCjyxStandardFileDialog) is
  /// used.
  /// If reading files (action == qCjyxFileDialog::Read) and if loadedNodes
  /// is not null, the loadedNodes collection is being populated with the
  /// loaded nodes.
  /// Returns true on success, false otherwise.
  Q_INVOKABLE bool openDialog(qCjyxIO::IOFileType fileType,
                              qCjyxFileDialog::IOAction action,
                              qCjyxIO::IOProperties ioProperties = qCjyxIO::IOProperties(),
                              vtkCollection* loadedNodes = nullptr);

  void addHistory(const QString& path);
  const QStringList& history()const;

  void setFavorites(const QList<QUrl>& urls);
  const QList<QUrl>& favorites()const;

  /// Takes ownership. Any previously set dialog corresponding to the same
  /// fileType (only 1 dialog per filetype) is overridden.
  void registerDialog(qCjyxFileDialog* dialog);

  /// Displays a progress dialog if it takes too long to load
  /// There is no way to know in advance how long the loading will take, so the
  /// progress dialog listens to the scene and increment the progress anytime
  /// a node is added.
  Q_INVOKABLE bool loadNodes(const qCjyxIO::IOFileType& fileType, const qCjyxIO::IOProperties& parameters,
    vtkCollection* loadedNodes = nullptr, vtkMRMLMessageCollection* userMessages = nullptr) override;
  /// If you have a list of nodes to load, it's best to use this function
  /// in order to have a unique progress dialog instead of multiple ones.
  /// It internally calls loadNodes() for each file.
  bool loadNodes(const QList<qCjyxIO::IOProperties>& files, vtkCollection* loadedNodes = nullptr,
    vtkMRMLMessageCollection* userMessages = nullptr) override;

  /// Helper function to display result of loadNodes.
  /// If success is set false then an error popup is displayed.
  /// If success is set to true then a popup is only displayed if error or warning messages are logged.
  /// If a popup is displayed then all the user-displayable messages are displayed in a collapsed "Details" section.
  /// The dialog is not displayed if the application is launched with testing mode enabled.
  Q_INVOKABLE static void showLoadNodesResultDialog(bool success, vtkMRMLMessageCollection* userMessages);

  /// dragEnterEvents can be forwarded to the IOManager, if a registered dialog
  /// supports it, the event is accepted, otherwise ignored.
  /// \sa dropEvent()
  void dragEnterEvent(QDragEnterEvent *event);

  /// Search, in the list of registered readers, the first dialog that
  /// handles the drop event. If the event is accepted by the dialog (
  /// usually the is also used to populate the dialog), the manager opens the dialog,
  /// otherwise the next dialog is tested. The order in which dialogs are
  /// being tested is the opposite of the dialogs are registered.
  /// \sa dragEnterEvent()
  void dropEvent(QDropEvent *event);

public slots:

  void openScreenshotDialog();
  void openSceneViewsDialog();
  bool openLoadSceneDialog();
  bool openAddSceneDialog();
  inline bool openAddDataDialog();
  inline bool openAddDataDialog(QString fileName);
  inline bool openAddVolumeDialog();
  inline bool openAddVolumesDialog();
  inline bool openAddModelDialog();
  inline bool openAddScalarOverlayDialog();
  inline bool openAddTransformDialog();
  inline bool openAddColorTableDialog();
  inline bool openAddFiducialDialog();
  inline bool openAddMarkupsDialog();
  inline bool openAddFiberBundleDialog();
  inline bool openSaveDataDialog();

protected slots:
  void updateProgressDialog();
  void execDelayedFileDialog();

protected:
  friend class qCjyxFileDialog;
  using qCjyxCoreIOManager::readers;
protected:
  QScopedPointer<qCjyxIOManagerPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxIOManager);
  Q_DISABLE_COPY(qCjyxIOManager);
};

//------------------------------------------------------------------------------
bool qCjyxIOManager::openAddDataDialog(QString fileName)
{
  qCjyxIO::IOProperties ioProperties;
  ioProperties["fileName"] = fileName;
  return this->openDialog(QString("NoFile"), qCjyxFileDialog::Read, ioProperties);
}

//------------------------------------------------------------------------------
bool qCjyxIOManager::openAddDataDialog()
{
  return this->openDialog(QString("NoFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddVolumeDialog()
{
  return this->openDialog(QString("VolumeFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddVolumesDialog()
{
  qCjyxIO::IOProperties ioProperties;
  ioProperties["multipleFiles"] = true;
  return this->openDialog(QString("VolumeFile"), qCjyxFileDialog::Read, ioProperties);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddModelDialog()
{
  return this->openDialog(QString("ModelFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddScalarOverlayDialog()
{
  return this->openDialog(QString("ScalarOverlayFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddTransformDialog()
{
  return this->openDialog(QString("TransformFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddColorTableDialog()
{
  return this->openDialog(QString("ColorTableFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddFiducialDialog()
{
  return this->openDialog(QString("FiducialListFile"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddMarkupsDialog()
{
  return this->openDialog(QString("MarkupsFiducials"), qCjyxFileDialog::Read);
}

//-----------------------------------------------------------------------------
bool qCjyxIOManager::openAddFiberBundleDialog()
{
  return this->openDialog(QString("FiberBundleFile"), qCjyxFileDialog::Read);
}

//------------------------------------------------------------------------------
bool qCjyxIOManager::openSaveDataDialog()
{
  return this->openDialog(QString("NoFile"), qCjyxFileDialog::Write);
}

#endif
