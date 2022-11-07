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

==============================================================================*/

#ifndef __qCjyxModuleFinderDialog_h
#define __qCjyxModuleFinderDialog_h

// Qt includes
#include <QDialog>

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxBaseQTGUIExport.h"

/// Forward declarations
class QItemSelection;
class qCjyxAbstractModuleFactoryManager;
class qCjyxModuleFinderDialogPrivate;

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxModuleFinderDialog : public QDialog
{
  Q_OBJECT
  Q_PROPERTY(QString currentModuleName READ currentModuleName)
public:
  typedef QDialog Superclass;
  qCjyxModuleFinderDialog(QWidget* parent=nullptr);
  ~qCjyxModuleFinderDialog() override;

  QString currentModuleName() const;

  Q_INVOKABLE void setFocusToModuleTitleFilter();

public Q_SLOTS:
  /// Set the module factory manager that contains the list of modules
  void setFactoryManager(qCjyxAbstractModuleFactoryManager* manager);
  void setModuleTitleFilterText(const QString& text);
  void setSearchInAllText(bool searchAll);
  void setShowBuiltInModules(bool show);
  void setShowTestingModules(bool show);

protected Q_SLOTS:
  void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
  void onModuleTitleFilterTextChanged();

protected:
  QScopedPointer<qCjyxModuleFinderDialogPrivate> d_ptr;
  bool eventFilter(QObject* target, QEvent* event) override;

private:
  Q_DECLARE_PRIVATE(qCjyxModuleFinderDialog);
  Q_DISABLE_COPY(qCjyxModuleFinderDialog);
};

#endif
