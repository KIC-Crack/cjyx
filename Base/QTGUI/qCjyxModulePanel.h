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

#ifndef __qCjyxModulePanel_h
#define __qCjyxModulePanel_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModulePanel.h"

#include "qCjyxBaseQTGUIExport.h"

class qCjyxModulePanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxModulePanel
  : public qCjyxAbstractModulePanel
{
  Q_OBJECT

  /// This property controls whether the help and acknowledgment
  /// section is visible or not.
  /// If the property is set to \a true and the current module doesn't have
  /// any help text, the section will remain hidden.
  /// If the property has already been set to \a true and a module
  /// with a help text is set, the section will be visible.
  /// \sa isHelpAndAcknowledgmentVisible()
  /// \sa setHelpAndAcknowledgmentVisible()
  /// \sa qCjyxAbstractCoreModule::helpText(), setModule()
  Q_PROPERTY(bool helpAndAcknowledgmentVisible READ isHelpAndAcknowledgmentVisible WRITE setHelpAndAcknowledgmentVisible)

public:
  typedef qCjyxAbstractModulePanel Superclass;
  qCjyxModulePanel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~qCjyxModulePanel() override;

  /// Get the helpAndAcknowledgmentVisible property value.
  /// \sa helpAndAcknowledgmentVisible, isHelpAndAcknowledgmentVisible()
  void setHelpAndAcknowledgmentVisible(bool value);

  /// Set the canShowHelpAndAcknowledgment property value.
  /// \sa helpAndAcknowledgmentVisible, setHelpAndAcknowledgmentVisible()
  bool isHelpAndAcknowledgmentVisible()const;

  void removeAllModules() override;
  qCjyxAbstractCoreModule* currentModule()const;
  QString currentModuleName()const;

  bool eventFilter(QObject* watchedModule, QEvent* event) override;
  QSize minimumSizeHint()const override;

public slots:
  void setModule(const QString& moduleName);

protected:
  void addModule(qCjyxAbstractCoreModule* module) override;
  void removeModule(qCjyxAbstractCoreModule* module) override;
  void setModule(qCjyxAbstractCoreModule* module);

protected:
  QScopedPointer<qCjyxModulePanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxModulePanel);
  Q_DISABLE_COPY(qCjyxModulePanel);
};

#endif
