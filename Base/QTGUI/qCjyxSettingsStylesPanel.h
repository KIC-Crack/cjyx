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

  This file was originally developed by Johan Andruejol, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxSettingsStylesPanel_h
#define __qCjyxSettingsStylesPanel_h

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QSettings;
class qCjyxSettingsStylesPanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsStylesPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  /// Holds the current style's name for the settings.
  /// The current style itself can be accessed using qCjyxApplication
  /// \sa currentStyle(), setCurrentStyle(), currentStyleChanged()
  Q_PROPERTY(QString currentStyle READ currentStyle WRITE setCurrentStyle NOTIFY currentStyleChanged)
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsStylesPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsStylesPanel() override;

  /// Return list of all available styles.
  ///
  /// Qt built-in styles are not included in the list.
  /// \sa setCurrentStyle
  static QStringList availableCjyxStyles();

  /// \sa currentStyle
  QString currentStyle() const;

public slots:
  /// Change the current style based on its name
  /// \sa currentStyle
  void setCurrentStyle(const QString&);

signals:
  /// Signal emitted when the current style is changed
  /// \sa currentStyle
  void currentStyleChanged(const QString&);

protected slots:
  void onAdditionalStylePathsChanged();
  void onAddStyleAdditionalPathClicked();
  void onRemoveStyleAdditionalPathClicked();
  void onStyleChanged(const QString&);

  void onFontChanged(const QFont& font);
  void onShowToolTipsToggled(bool);
  void onShowToolButtonTextToggled(bool enable);

protected:
  QScopedPointer<qCjyxSettingsStylesPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsStylesPanel);
  Q_DISABLE_COPY(qCjyxSettingsStylesPanel);
};

#endif
