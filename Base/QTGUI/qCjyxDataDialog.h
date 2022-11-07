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

#ifndef __qCjyxDataDialog_h
#define __qCjyxDataDialog_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxFileDialog.h"
#include "qCjyxBaseQTGUIExport.h"

/// Forward declarations
class qCjyxDataDialogPrivate;
class QDropEvent;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxDataDialog : public qCjyxFileDialog
{
  Q_OBJECT
public:
  typedef qCjyxFileDialog Superclass;
  qCjyxDataDialog(QObject* parent =nullptr);
  ~qCjyxDataDialog() override;

  qCjyxIO::IOFileType fileType()const override;
  QString description()const override;
  qCjyxFileDialog::IOAction action()const override;

  bool isMimeDataAccepted(const QMimeData* mimeData)const override;
  void dropEvent(QDropEvent *event) override;

  /// run the dialog to select the file/files/directory
  Q_INVOKABLE bool exec(const qCjyxIO::IOProperties& readerProperties =
                    qCjyxIO::IOProperties()) override;

public Q_SLOTS:
  /// for programmatic population of dialog
  virtual void addFile(const QString filePath);
  virtual void addDirectory(const QString directoryPath);

protected:
  QScopedPointer<qCjyxDataDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDataDialog);
  Q_DISABLE_COPY(qCjyxDataDialog);
};

#endif
