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

#ifndef __qCjyxSettingsPythonPanel_h
#define __qCjyxSettingsPythonPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QSettings;
class qCjyxSettingsPythonPanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsPythonPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsPythonPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsPythonPanel() override;

protected slots:
  void onFontChanged(const QFont& font);

protected:
  QScopedPointer<qCjyxSettingsPythonPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsPythonPanel);
  Q_DISABLE_COPY(qCjyxSettingsPythonPanel);
};

#endif
