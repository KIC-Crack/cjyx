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

#ifndef __qCjyxAbstractModulePanel_h
#define __qCjyxAbstractModulePanel_h

#include "qCjyxWidget.h"
#include "qCjyxBaseQTGUIExport.h"

class qCjyxAbstractCoreModule;
class qCjyxAbstractModulePanelPrivate;
class qCjyxModuleManager;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxAbstractModulePanel: public qCjyxWidget
{
  Q_OBJECT
public:
  qCjyxAbstractModulePanel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~qCjyxAbstractModulePanel() override;

  void addModule(const QString& moduleName);
  void removeModule(const QString& moduleName);
  virtual void removeAllModules() = 0;

  Q_INVOKABLE void setModuleManager(qCjyxModuleManager* moduleManager);
  Q_INVOKABLE qCjyxModuleManager* moduleManager()const;

signals:
  void moduleAdded(const QString& moduleName);
  //void moduleAboutToBeRemoved(const QString& moduleName);
  void moduleRemoved(const QString& moduleName);

protected:
  QScopedPointer<qCjyxAbstractModulePanelPrivate> d_ptr;

  virtual void addModule(qCjyxAbstractCoreModule* module) = 0;
  virtual void removeModule(qCjyxAbstractCoreModule* module) = 0;

private:
  Q_DECLARE_PRIVATE(qCjyxAbstractModulePanel);
  Q_DISABLE_COPY(qCjyxAbstractModulePanel);
};

#endif
