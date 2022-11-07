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

#ifndef __qCjyxModelsDialog_h
#define __qCjyxModelsDialog_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxFileDialog.h"
#include "qCjyxBaseQTGUIExport.h"

class qCjyxModelsDialogPrivate;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxModelsDialog : public qCjyxFileDialog
{
  Q_OBJECT
public:
  typedef qCjyxFileDialog Superclass;
  qCjyxModelsDialog(QObject* parent =nullptr);
  ~qCjyxModelsDialog() override;

  qCjyxIO::IOFileType fileType()const override;
  QString description()const override;
  qCjyxFileDialog::IOAction action()const override;

  /// run the dialog to select the file/files/directory
  bool exec(const qCjyxIO::IOProperties& readerProperties =
                    qCjyxIO::IOProperties()) override;

  QStringList loadedNodes()const override;
protected:
  QScopedPointer<qCjyxModelsDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModelsDialog);
  Q_DISABLE_COPY(qCjyxModelsDialog);
};

#endif
