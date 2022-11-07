/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#ifndef __qCjyxMarkupsSettingsPanel_h
#define __qCjyxMarkupsSettingsPanel_h

// CTK includes
#include <ctkVTKObject.h>
#include <ctkSettingsPanel.h>

// Markups includes
#include "qCjyxMarkupsModuleExport.h"
class qCjyxMarkupsSettingsPanelPrivate;
class vtkCjyxMarkupsLogic;

class Q_CJYX_QTMODULES_MARKUPS_EXPORT qCjyxMarkupsSettingsPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
  QVTK_OBJECT
  Q_PROPERTY(QString defaultGlyphType READ defaultGlyphType WRITE setDefaultGlyphType NOTIFY defaultGlyphTypeChanged)
  Q_PROPERTY(QColor defaultUnselectedColor READ defaultUnselectedColor WRITE setDefaultUnselectedColor NOTIFY defaultUnselectedColorChanged)
  Q_PROPERTY(QColor defaultSelectedColor READ defaultSelectedColor WRITE setDefaultSelectedColor NOTIFY defaultSelectedColorChanged)
  Q_PROPERTY(QColor defaultActiveColor READ defaultActiveColor WRITE setDefaultActiveColor NOTIFY defaultActiveColorChanged)
  Q_PROPERTY(double defaultGlyphScale READ defaultGlyphScale WRITE setDefaultGlyphScale NOTIFY defaultGlyphScaleChanged)
  Q_PROPERTY(double defaultTextScale READ defaultTextScale WRITE setDefaultTextScale NOTIFY defaultTextScaleChanged)
  Q_PROPERTY(double defaultOpacity READ defaultOpacity WRITE setDefaultOpacity NOTIFY defaultOpacityChanged)

public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxMarkupsSettingsPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxMarkupsSettingsPanel() override;

  /// Markups logic is synchronized with the settings.
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeGlyphType
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeGlyphScale
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeTextScale
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeOpacity
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeColor
  /// \sa vtkCjyxMarkupsLogic::SetDefaultMarkupsDisplayNodeSelectedColor
  void setMarkupsLogic(vtkCjyxMarkupsLogic* logic);
  vtkCjyxMarkupsLogic* markupsLogic()const;

  QString defaultGlyphType() const;
  QColor defaultUnselectedColor() const;
  QColor defaultSelectedColor() const;
  QColor defaultActiveColor() const;
  double defaultGlyphScale() const;
  double defaultTextScale() const;
  double defaultOpacity() const;

public slots:
  void setDefaultGlyphType(const QString& type);
  void setDefaultUnselectedColor(const QColor color);
  void setDefaultSelectedColor(const QColor color);
  void setDefaultActiveColor(const QColor color);
  void setDefaultGlyphScale(const double scale);
  void setDefaultTextScale(const double scale);
  void setDefaultOpacity(const double scale);

signals:
  void defaultGlyphTypeChanged(const QString&);
  void defaultUnselectedColorChanged(QColor);
  void defaultSelectedColorChanged(QColor);
  void defaultActiveColorChanged(QColor);
  void defaultGlyphScaleChanged(const double);
  void defaultTextScaleChanged(const double);
  void defaultOpacityChanged(const double);

protected slots:
  void onMarkupsLogicModified();

  void onDefaultGlyphTypeChanged(int);
  void updateMarkupsLogicDefaultGlyphType();

  void onDefaultSelectedColorChanged(QColor);
  void updateMarkupsLogicDefaultSelectedColor();

  void onDefaultUnselectedColorChanged(QColor);
  void updateMarkupsLogicDefaultUnselectedColor();

  void onDefaultGlyphScaleChanged(double);
  void updateMarkupsLogicDefaultGlyphScale();

  void onDefaultTextScaleChanged(double);
  void updateMarkupsLogicDefaultTextScale();

  void onDefaultOpacityChanged(double);
  void updateMarkupsLogicDefaultOpacity();

protected:
  QScopedPointer<qCjyxMarkupsSettingsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxMarkupsSettingsPanel);
  Q_DISABLE_COPY(qCjyxMarkupsSettingsPanel);
};

#endif
