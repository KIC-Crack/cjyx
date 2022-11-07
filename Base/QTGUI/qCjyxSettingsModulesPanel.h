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

#ifndef __qCjyxSettingsModulesPanel_h
#define __qCjyxSettingsModulesPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QSettings;
class qCjyxSettingsModulesPanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsModulesPanel
  : public ctkSettingsPanel
{
  Q_OBJECT

  /// This property holds the names of the modules to ignore at registration time.
  ///
  /// It corresponds to the names of the modules that are expected to always
  /// be ignored by saving and restoring them from the application settings.
  ///
  /// \sa qCjyxAbstractModuleFactoryManager::modulesToIgnore
  Q_PROPERTY(QStringList modulesToAlwaysIgnore
             READ modulesToAlwaysIgnore
             WRITE setModulesToAlwaysIgnore
             NOTIFY modulesToAlwaysIgnoreChanged)

public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsModulesPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsModulesPanel() override;

  /// Get the \a modulesToAlwaysIgnore list.
  /// \sa setModulesToAlwaysIgnore(const QStringList& modulesNames)
  QStringList modulesToAlwaysIgnore()const;

public slots:

  /// Set the \a modulesToAlwaysIgnore list.
  ///
  /// If list is modified, the signal
  /// modulesToAlwaysIgnoreChanged(const QStringLists&) is emitted.
  ///
  /// \sa modulesToAlwaysIgnore()
  void setModulesToAlwaysIgnore(const QStringList& modulesNames);

protected slots:
  void onHomeModuleChanged(const QString& moduleName);
  void onTemporaryPathChanged(const QString& path);
  void onShowHiddenModulesChanged(bool);

  void onAdditionalModulePathsChanged();
  void onAddModulesAdditionalPathClicked();
  void onRemoveModulesAdditionalPathClicked();

signals:
  void modulesToAlwaysIgnoreChanged(const QStringList& modulesNames);

protected:
  QScopedPointer<qCjyxSettingsModulesPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsModulesPanel);
  Q_DISABLE_COPY(qCjyxSettingsModulesPanel);
};

#endif
