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

#ifndef __qCjyxSaveDataDialog_h
#define __qCjyxSaveDataDialog_h

// Cjyx includes
#include "qCjyxFileDialog.h"
#include "qCjyxBaseQTGUIExport.h"

/// Forward declarations
class qCjyxSaveDataDialogPrivate;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSaveDataDialog : public qCjyxFileDialog
{
  Q_OBJECT
public:
  typedef QObject Superclass;
  qCjyxSaveDataDialog(QObject* parent = nullptr);
  ~qCjyxSaveDataDialog() override;

  qCjyxIO::IOFileType fileType()const override;
  QString description()const override;
  qCjyxFileDialog::IOAction action()const override;

  /// Open the data dialog and save the nodes/scene
  bool exec(const qCjyxIO::IOProperties& readerProperties =
                    qCjyxIO::IOProperties()) override;

protected:
  QScopedPointer<qCjyxSaveDataDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSaveDataDialog);
  Q_DISABLE_COPY(qCjyxSaveDataDialog);
};

#endif
