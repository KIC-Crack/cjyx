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

// Qt includes
#include <QDebug>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxMarkupsSettingsPanel.h"
#include "ui_qCjyxMarkupsSettingsPanel.h"

// Markups Logic includes
#include <vtkCjyxMarkupsLogic.h>

// Markups MRML includes

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>

// --------------------------------------------------------------------------
// qCjyxMarkupsSettingsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxMarkupsSettingsPanelPrivate: public Ui_qCjyxMarkupsSettingsPanel
{
  Q_DECLARE_PUBLIC(qCjyxMarkupsSettingsPanel);
protected:
  qCjyxMarkupsSettingsPanel* const q_ptr;

public:
  qCjyxMarkupsSettingsPanelPrivate(qCjyxMarkupsSettingsPanel& object);
  void init();

  vtkSmartPointer<vtkCjyxMarkupsLogic> MarkupsLogic;
};

// --------------------------------------------------------------------------
// qCjyxMarkupsSettingsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxMarkupsSettingsPanelPrivate
::qCjyxMarkupsSettingsPanelPrivate(qCjyxMarkupsSettingsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanelPrivate::init()
{
  Q_Q(qCjyxMarkupsSettingsPanel);

  this->setupUi(q);
}



// --------------------------------------------------------------------------
// qCjyxMarkupsSettingsPanel methods

// --------------------------------------------------------------------------
qCjyxMarkupsSettingsPanel::qCjyxMarkupsSettingsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxMarkupsSettingsPanelPrivate(*this))
{
  Q_D(qCjyxMarkupsSettingsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxMarkupsSettingsPanel::~qCjyxMarkupsSettingsPanel() = default;

// --------------------------------------------------------------------------
vtkCjyxMarkupsLogic* qCjyxMarkupsSettingsPanel
::markupsLogic()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);
  return d->MarkupsLogic;
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel
::setMarkupsLogic(vtkCjyxMarkupsLogic* logic)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  qvtkReconnect(d->MarkupsLogic, logic, vtkCommand::ModifiedEvent,
                this, SLOT(onMarkupsLogicModified()));
  d->MarkupsLogic = logic;

  this->onMarkupsLogicModified();

  this->registerProperty("Markups/GlyphType", this,
                         "defaultGlyphType", SIGNAL(defaultGlyphTypeChanged(QString)));
  this->registerProperty("Markups/SelectedColor", this,
                         "defaultSelectedColor", SIGNAL(defaultSelectedColorChanged(QColor)));
  this->registerProperty("Markups/UnselectedColor", this,
                         "defaultUnselectedColor", SIGNAL(defaultUnselectedColorChanged(QColor)));
  this->registerProperty("Markups/ActiveColor", this,
                         "defaultActiveColor", SIGNAL(defaultActiveColorChanged(QColor)));
  this->registerProperty("Markups/GlyphScale", this,
                         "defaultGlyphScale", SIGNAL(defaultGlyphScaleChanged(double)));
  this->registerProperty("Markups/TextScale", this,
                         "defaultTextScale", SIGNAL(defaultTextScaleChanged(double)));
  this->registerProperty("Markups/Opacity", this,
                         "defaultOpacity", SIGNAL(defaultOpacityChanged(double)));
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel
::onMarkupsLogicModified()
{
  Q_D(qCjyxMarkupsSettingsPanel);
/* disable it for now; if we want a settings panel then use the same pattern that is used for default view options

  // update the gui to match the logic
  QString glyphType = QString(d->MarkupsLogic->GetDefaultMarkupsDisplayNodeGlyphTypeAsString().c_str());

  QObject::connect(d->defaultGlyphTypeComboBox, SIGNAL(currentIndexChanged(int)),
                   this, SLOT(onDefaultGlyphTypeChanged(int)),Qt::UniqueConnection);

  // TODO: do I need to use the strings?
//  d->defaultGlyphTypeComboBox->setCurrentIndex(glyphType - 1);
  int glyphTypeIndex = d->defaultGlyphTypeComboBox->findData(glyphType);
  if (glyphTypeIndex != -1)
    {
    d->defaultGlyphTypeComboBox->setCurrentIndex(glyphTypeIndex);
    }


  double *unselectedColor = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeColor();
  QObject::connect(d->defaultUnselectedColorPickerButton, SIGNAL(colorChanged(QColor)),
                   this, SLOT(onDefaultUnselectedColorChanged(QColor)));
  QColor qcolor = QColor::fromRgbF(unselectedColor[0], unselectedColor[1], unselectedColor[2]);
  d->defaultUnselectedColorPickerButton->setColor(qcolor);

  double *selectedColor =  d->MarkupsLogic->GetDefaultMarkupsDisplayNodeSelectedColor();
  QObject::connect(d->defaultSelectedColorPickerButton, SIGNAL(colorChanged(QColor)),
                   this, SLOT(onDefaultSelectedColorChanged(QColor)),Qt::UniqueConnection);
  qcolor = QColor::fromRgbF(selectedColor[0], selectedColor[1], selectedColor[2]);
  d->defaultSelectedColorPickerButton->setColor(qcolor);

  double glyphScale = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeGlyphScale();
  QObject::connect(d->defaultGlyphScaleSliderWidget, SIGNAL(valueChanged(double)),
                   this, SLOT(onDefaultGlyphScaleChanged(double)),Qt::UniqueConnection);
  d->defaultGlyphScaleSliderWidget->setValue(glyphScale);

  double textScale = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeTextScale();
  QObject::connect(d->defaultTextScaleSliderWidget, SIGNAL(valueChanged(double)),
                   this, SLOT(onDefaultTextScaleChanged(double)),Qt::UniqueConnection);
  d->defaultTextScaleSliderWidget->setValue(textScale);

  double opacity = d->MarkupsLogic->GetDefaultMarkupsDisplayNodeOpacity();
  QObject::connect(d->defaultOpacitySliderWidget, SIGNAL(valueChanged(double)),
                   this, SLOT(onDefaultOpacityChanged(double)),Qt::UniqueConnection);
  d->defaultOpacitySliderWidget->setValue(opacity);
  */
}

// --------------------------------------------------------------------------
QString qCjyxMarkupsSettingsPanel::defaultGlyphType()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  int currentIndex  = d->defaultGlyphTypeComboBox->currentIndex();
  QString glyphType;
  if (currentIndex != -1)
    {
    glyphType =
      d->defaultGlyphTypeComboBox->itemText(currentIndex);
    }
  return glyphType;
}

// --------------------------------------------------------------------------
QColor qCjyxMarkupsSettingsPanel::defaultUnselectedColor()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  QColor color = d->defaultUnselectedColorPickerButton->color();

  return color;
}

// --------------------------------------------------------------------------
QColor qCjyxMarkupsSettingsPanel::defaultSelectedColor()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  QColor color = d->defaultSelectedColorPickerButton->color();

  return color;
}

// --------------------------------------------------------------------------
QColor qCjyxMarkupsSettingsPanel::defaultActiveColor()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  QColor color = d->defaultActiveColorPickerButton->color();

  return color;
}

// --------------------------------------------------------------------------
double qCjyxMarkupsSettingsPanel::defaultGlyphScale()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  double glyphScale = d->defaultGlyphScaleSliderWidget->value();

  return glyphScale;
}

// --------------------------------------------------------------------------
double qCjyxMarkupsSettingsPanel::defaultTextScale()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  double textScale = d->defaultTextScaleSliderWidget->value();

  return textScale;
}

// --------------------------------------------------------------------------
double qCjyxMarkupsSettingsPanel::defaultOpacity()const
{
  Q_D(const qCjyxMarkupsSettingsPanel);

  double opacity = d->defaultOpacitySliderWidget->value();

  return opacity;
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultGlyphType(const QString& glyphType)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  int glyphTypeIndex = d->defaultGlyphTypeComboBox->findData(glyphType);

  if (glyphTypeIndex != -1)
    {
    d->defaultGlyphTypeComboBox->setCurrentIndex(glyphTypeIndex);
    }
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultUnselectedColor(const QColor color)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultUnselectedColorPickerButton->setColor(color);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultSelectedColor(const QColor color)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultSelectedColorPickerButton->setColor(color);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultActiveColor(const QColor color)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultActiveColorPickerButton->setColor(color);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultGlyphScale(const double glyphScale)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultGlyphScaleSliderWidget->setValue(glyphScale);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultTextScale(const double glyphScale)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultTextScaleSliderWidget->setValue(glyphScale);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::setDefaultOpacity(const double opacity)
{
  Q_D(qCjyxMarkupsSettingsPanel);

  d->defaultOpacitySliderWidget->setValue(opacity);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultGlyphTypeChanged(int index)
{
//   Q_D(qCjyxMarkupsSettingsPanel);
  Q_UNUSED(index);

  this->updateMarkupsLogicDefaultGlyphType();
  emit defaultGlyphTypeChanged(this->defaultGlyphType());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultGlyphType()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeGlyphTypeFromString(this->defaultGlyphType().toUtf8());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultUnselectedColorChanged(QColor color)
{
  Q_UNUSED(color);
  this->updateMarkupsLogicDefaultUnselectedColor();
  emit defaultUnselectedColorChanged(this->defaultUnselectedColor());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultUnselectedColor()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  QColor qcolor = this->defaultUnselectedColor();

  double color[3];

  color[0] = qcolor.redF();
  color[1] = qcolor.greenF();
  color[2] = qcolor.blueF();

  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeColor(color);
  Q_UNUSED(color);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultSelectedColorChanged(QColor color)
{
  Q_UNUSED(color);
  this->updateMarkupsLogicDefaultSelectedColor();
  emit defaultSelectedColorChanged(this->defaultSelectedColor());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultSelectedColor()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  QColor qcolor = this->defaultSelectedColor();

  double color[3];

  color[0] = qcolor.redF();
  color[1] = qcolor.greenF();
  color[2] = qcolor.blueF();

  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeSelectedColor(color);
  Q_UNUSED(color);
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultGlyphScaleChanged(double scale)
{
  Q_UNUSED(scale);
  this->updateMarkupsLogicDefaultGlyphScale();
  emit defaultGlyphScaleChanged(this->defaultGlyphScale());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultGlyphScale()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeGlyphScale(this->defaultGlyphScale());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultTextScaleChanged(double scale)
{
  Q_UNUSED(scale);
  this->updateMarkupsLogicDefaultTextScale();
  emit defaultTextScaleChanged(this->defaultTextScale());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultTextScale()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeTextScale(this->defaultTextScale());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::onDefaultOpacityChanged(double opacity)
{
  Q_UNUSED(opacity);
  this->updateMarkupsLogicDefaultOpacity();
  emit defaultOpacityChanged(this->defaultOpacity());
}

// --------------------------------------------------------------------------
void qCjyxMarkupsSettingsPanel::updateMarkupsLogicDefaultOpacity()
{
  Q_D(qCjyxMarkupsSettingsPanel);

  if (d->MarkupsLogic == nullptr)
    {
    return;
    }
  // disable it for now; if we want a settings panel then use the same pattern that is used for default view options
  // d->MarkupsLogic->SetDefaultMarkupsDisplayNodeOpacity(this->defaultOpacity());
}
