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

#ifndef __qCjyxExtensionsManagerDialog_h
#define __qCjyxExtensionsManagerDialog_h

// Qt includes
#include <QDialog>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class qCjyxExtensionsManagerDialogPrivate;
class qCjyxExtensionsManagerModel;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxExtensionsManagerDialog
  : public QDialog
{
  Q_OBJECT
  Q_PROPERTY(bool restartRequested READ restartRequested WRITE setRestartRequested)
public:
  /// Superclass typedef
  typedef QDialog Superclass;

  /// Constructor
  explicit qCjyxExtensionsManagerDialog(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxExtensionsManagerDialog() override;

  Q_INVOKABLE qCjyxExtensionsManagerModel* extensionsManagerModel()const;
  Q_INVOKABLE void setExtensionsManagerModel(qCjyxExtensionsManagerModel* model);

  /// Return True if the application is expected to be restarted.
  bool restartRequested()const;

  /// \sa restartRequested()
  void setRestartRequested(bool value);

  void closeEvent(QCloseEvent* event) override;
  void accept() override;
  void reject() override;

protected slots:
  void onModelUpdated();
  void onBatchProcessingChanged();

protected:
  QScopedPointer<qCjyxExtensionsManagerDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxExtensionsManagerDialog);
  Q_DISABLE_COPY(qCjyxExtensionsManagerDialog);
};

#endif
