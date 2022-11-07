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

#ifndef __qCjyxSettingsDeveloperPanel_h
#define __qCjyxSettingsDeveloperPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QSettings;
class qCjyxSettingsDeveloperPanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsDeveloperPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsDeveloperPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsDeveloperPanel() override;

public slots:

protected slots:
  void enableDeveloperMode(bool value);
  void enableQtTesting(bool value);
  void preserveCLIModuleDataFiles(bool value);

protected:
  QScopedPointer<qCjyxSettingsDeveloperPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsDeveloperPanel);
  Q_DISABLE_COPY(qCjyxSettingsDeveloperPanel);
};

#endif
