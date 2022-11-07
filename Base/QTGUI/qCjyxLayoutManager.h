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

#ifndef __qCjyxLayoutManager_h
#define __qCjyxLayoutManager_h

// CTK includes
#include "qCjyxBaseQTGUIExport.h"

// MRMLWidgets includes
#include <qMRMLLayoutManager.h>

class qCjyxLayoutManagerPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxLayoutManager : public qMRMLLayoutManager
{
  Q_OBJECT
public:
  /// Constructors
  explicit qCjyxLayoutManager(QWidget* widget = nullptr);

  /// Set the directory from which built-in scripted
  /// displayableManagers should be sourced from.
  Q_INVOKABLE void setScriptedDisplayableManagerDirectory(const QString& scriptedDisplayableManagerDirectory);

  void setCurrentModule(const QString& moduleName);

signals:
  void selectModule(const QString& moduleName);

private:
  Q_DECLARE_PRIVATE(qCjyxLayoutManager);
  Q_DISABLE_COPY(qCjyxLayoutManager);
};

#endif
