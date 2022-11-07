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

#ifndef __qCjyxFileDialog_h
#define __qCjyxFileDialog_h

// Qt includes
#include <QObject>
#include <QStringList>
class QMimeData;
class QDropEvent;

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxIO.h"
#include "qCjyxBaseQTGUIExport.h"

/// Forward declarations
class qCjyxIOManager;
//class qCjyxFileDialogPrivate;
class qCjyxIOOptions;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxFileDialog : public QObject
{
  Q_OBJECT
  Q_ENUMS(IOAction)
  Q_PROPERTY(QString description READ description)

public:
  typedef QObject Superclass;
  qCjyxFileDialog(QObject* parent =nullptr);
  ~qCjyxFileDialog() override;

  virtual qCjyxIO::IOFileType fileType()const = 0;

  /// Unique name of the reader/writer
  /// \sa filetype()
  virtual QString description()const = 0;

  enum IOAction
  {
    Read,
    Write
  };
  virtual qCjyxFileDialog::IOAction action()const = 0;
  /// run the dialog to select the file/files/directory
  /// Properties availables with IOPorperties: fileMode, multipleFiles, fileType.
  virtual bool exec(const qCjyxIO::IOProperties& ioProperties =
                    qCjyxIO::IOProperties()) = 0;

  /// TBD: move in qCjyxCoreIOManager or qCjyxIOManager ?
  /// Return the namefilters of all the readers in IOManager corresponding to
  /// fileType
  static QStringList nameFilters(qCjyxIO::IOFileType fileType =
                                 QString("NoFile"));

  /// Accept or ignore mimedata.
  /// Returns false by default.
  /// Can be reimplemented in subclass to support drag&drop.
  /// \sa dropEvent()
  virtual bool isMimeDataAccepted(const QMimeData*mimeData)const;

  /// Handle drop events: populate the dialog with the dropped mime data.
  /// Can be reimplemented in subclass to support drag&drop.
  /// Do nothing by default.
  /// If it does something, acceptProposedAction() or accept() must be called.
  /// \sa isMimeDataAccepted()
  virtual void dropEvent(QDropEvent *event);

  /// Return the list of nodes created by exec().
  /// To be reimplemented.
  /// \sa qCjyxFileReader::loadedNodes()
  virtual QStringList loadedNodes()const;
private:
//  Q_DECLARE_PRIVATE(qCjyxFileDialog);
  Q_DISABLE_COPY(qCjyxFileDialog);
};

Q_DECLARE_METATYPE(qCjyxFileDialog::IOAction)

class qCjyxStandardFileDialogPrivate;
class ctkFileDialog;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxStandardFileDialog
: public qCjyxFileDialog
{
  Q_OBJECT
  Q_PROPERTY(qCjyxIO::IOFileType fileType READ fileType WRITE setFileType)
  Q_PROPERTY(QString description READ description WRITE setDescription)
  /// This property controls which action the dialog is doing: read or write.
  /// Read by default.
  Q_PROPERTY(IOAction action READ action WRITE setAction)

public:
  qCjyxStandardFileDialog(QObject* parent=nullptr);
  ~qCjyxStandardFileDialog() override;

  /// Reimplemented to return the fileType set by setFileType()
  /// \sa fileType, setFileType()
  qCjyxIO::IOFileType fileType()const override;
  virtual void setFileType(qCjyxIO::IOFileType fileType);

  /// Reimplemented to return the description set by setDescription()
  /// \sa description, setDescription()
  QString description() const override;
  virtual void setDescription(const QString& description);

  /// Reimplemented to return the IOAction set by setAction()
  /// \sa action, setAction()
  qCjyxFileDialog::IOAction action()const override;
  /// Set the action of the file dialog. To be called by python.
  /// \sa action, action()
  void setAction(IOAction action);

  bool exec(const qCjyxIO::IOProperties& ioProperties =
                    qCjyxIO::IOProperties()) override;

  /// Properties availables with IOPorperties: fileMode, multipleFiles, fileType.
  static QStringList getOpenFileName(qCjyxIO::IOProperties ioProperties =
                                     qCjyxIO::IOProperties());
  static QString getExistingDirectory(qCjyxIO::IOProperties ioProperties =
                                      qCjyxIO::IOProperties());

  /// Return the list of nodes created by exec().
  QStringList loadedNodes()const override;

protected:
  static ctkFileDialog* createFileDialog(const qCjyxIO::IOProperties& ioProperties =
                                         qCjyxIO::IOProperties(), QWidget* parent = nullptr);

  qCjyxIOOptions* options(const qCjyxIO::IOProperties& ioProperties)const;

protected:
  QScopedPointer<qCjyxStandardFileDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxStandardFileDialog);
  Q_DISABLE_COPY(qCjyxStandardFileDialog);
};

#endif
