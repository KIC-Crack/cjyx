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

#ifndef __qCjyxSettingsViewsPanel_h
#define __qCjyxSettingsViewsPanel_h

// CTK includes
#include <ctkSettingsPanel.h>

// QtGUI includes
#include "qCjyxBaseQTGUIExport.h"

class QSettings;
class qCjyxSettingsViewsPanelPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxSettingsViewsPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  /// Holds the current MSAA's name for the settings.
  /// The current MSAA setting can be accessed using qCjyxApplication
  /// \sa currentMSAA(), setCurrentMSAA()
  Q_PROPERTY(QString currentMSAA READ currentMSAA WRITE setCurrentMSAA NOTIFY currentMSAAChanged)
  Q_PROPERTY(QString sliceOrientationMarkerType READ sliceOrientationMarkerType WRITE setSliceOrientationMarkerType NOTIFY currentSliceOrientationMarkerTypeChanged)
  Q_PROPERTY(QString sliceOrientationMarkerSize READ sliceOrientationMarkerSize WRITE setSliceOrientationMarkerSize NOTIFY currentSliceOrientationMarkerSizeChanged)
  Q_PROPERTY(QString sliceRulerType READ sliceRulerType WRITE setSliceRulerType NOTIFY currentSliceRulerTypeChanged)
  Q_PROPERTY(QString threeDOrientationMarkerType READ threeDOrientationMarkerType WRITE setThreeDOrientationMarkerType NOTIFY currentThreeDOrientationMarkerTypeChanged)
  Q_PROPERTY(QString threeDOrientationMarkerSize READ threeDOrientationMarkerSize WRITE setThreeDOrientationMarkerSize NOTIFY currentThreeDOrientationMarkerSizeChanged)
  Q_PROPERTY(QString threeDRulerType READ threeDRulerType WRITE setThreeDRulerType NOTIFY currentThreeDRulerTypeChanged)
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSettingsViewsPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSettingsViewsPanel() override;

  /// Get current value as string
  QString currentMSAA() const;
  QString sliceOrientationMarkerType() const;
  QString sliceOrientationMarkerSize() const;
  QString sliceRulerType() const;
  QString threeDOrientationMarkerType() const;
  QString threeDOrientationMarkerSize() const;
  QString threeDRulerType() const;

public slots:
  /// Change the current value based on its name
  void setCurrentMSAA(const QString&);
  void setSliceOrientationMarkerType(const QString&);
  void setSliceOrientationMarkerSize(const QString&);
  void setSliceRulerType(const QString&);
  void setThreeDOrientationMarkerType(const QString&);
  void setThreeDOrientationMarkerSize(const QString&);
  void setThreeDRulerType(const QString&);
  void sliceViewOrientationChangedByUser();

signals:
  /// Signal emitted when the current value is changed
  void currentMSAAChanged(const QString&);
  void currentSliceOrientationMarkerTypeChanged(const QString&);
  void currentSliceOrientationMarkerSizeChanged(const QString&);
  void currentSliceRulerTypeChanged(const QString&);
  void currentThreeDOrientationMarkerTypeChanged(const QString&);
  void currentThreeDOrientationMarkerSizeChanged(const QString&);
  void currentThreeDRulerTypeChanged(const QString&);

protected slots:
  void onMSAAChanged(const QString&);

protected:
  QScopedPointer<qCjyxSettingsViewsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSettingsViewsPanel);
  Q_DISABLE_COPY(qCjyxSettingsViewsPanel);
};

#endif
