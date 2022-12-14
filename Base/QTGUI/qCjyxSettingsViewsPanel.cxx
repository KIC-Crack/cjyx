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

  This file was originally developed by Eric Larson.

==============================================================================*/

// Qt includes
#include <QDebug>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxSettingsViewsPanel.h"
#include "ui_qCjyxSettingsViewsPanel.h"

// CTK includes
#include <ctkVTKAbstractView.h>
#include <ctkComboBox.h>

// --------------------------------------------------------------------------
// qCjyxSettingsViewsPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsViewsPanelPrivate: public Ui_qCjyxSettingsViewsPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsViewsPanel);
protected:
  qCjyxSettingsViewsPanel* const q_ptr;

public:
  qCjyxSettingsViewsPanelPrivate(qCjyxSettingsViewsPanel& object);
  void init();
};

// --------------------------------------------------------------------------
// qCjyxSettingsViewsPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsViewsPanelPrivate
::qCjyxSettingsViewsPanelPrivate(qCjyxSettingsViewsPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanelPrivate::init()
{
  Q_Q(qCjyxSettingsViewsPanel);

  this->setupUi(q);

  // MSAA Setting
  // "Off" is the default, and should be the zeroth index to ensure that
  // poorly-formatted (or old) entries in the .ini default to "Off"
  // (toInt() returns zero when formatting is bad).
  this->MSAAComboBox->addItem("Off", 0);
  this->MSAAComboBox->addItem("Auto", -1);
  this->MSAAComboBox->addItem("2x", 2);
  this->MSAAComboBox->addItem("4x", 4);
  this->MSAAComboBox->addItem("8x", 8);
  this->MSAAComboBox->addItem("16x", 16);
  this->MSAAComboBox->setCurrentIndex(this->MSAAComboBox->findText("Off"));

  // Actions to propagate to the application when settings are changed
  QObject::connect(this->MSAAComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SLOT(onMSAAChanged(QString)));
  QObject::connect(this->MSAAComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentMSAAChanged(QString)));
  q->registerProperty("Views/MSAA", q,
                      "currentMSAA", SIGNAL(currentMSAAChanged(QString)),
                      "Multisampling (MSAA)",
                      ctkSettingsPanel::OptionRequireRestart);

  QObject::connect(this->SliceOrientationMarkerTypeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentSliceOrientationMarkerTypeChanged(QString)));
  q->registerProperty("DefaultSliceView/OrientationMarkerType", q,
                      "sliceOrientationMarkerType", SIGNAL(currentSliceOrientationMarkerTypeChanged(QString)),
                      "Slice view orientation marker type",
                      ctkSettingsPanel::OptionRequireRestart);
  QObject::connect(this->SliceOrientationMarkerSizeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentSliceOrientationMarkerSizeChanged(QString)));
  q->registerProperty("DefaultSliceView/OrientationMarkerSize", q,
                      "sliceOrientationMarkerSize", SIGNAL(currentSliceOrientationMarkerSizeChanged(QString)),
                      "Slice view orientation marker size",
                      ctkSettingsPanel::OptionRequireRestart);
  QObject::connect(this->SliceRulerTypeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentSliceRulerTypeChanged(QString)));
  q->registerProperty("DefaultSliceView/RulerType", q,
                      "sliceRulerType", SIGNAL(currentSliceRulerTypeChanged(QString)),
                      "Slice view ruler type",
                      ctkSettingsPanel::OptionRequireRestart);

  this->SliceViewOrientationComboBox->addItem(QWidget::tr("patient right is screen left (default)"), QString("PatientRightIsScreenLeft"));
  this->SliceViewOrientationComboBox->addItem(QWidget::tr("patient right is screen right"), QString("PatientRightIsScreenRight"));
  q->registerProperty("DefaultSliceView/Orientation", this->SliceViewOrientationComboBox,
    "currentUserDataAsString", SIGNAL(currentIndexChanged(int)),
    "Default slice view orientation",
    ctkSettingsPanel::OptionRequireRestart);
  QObject::connect(this->SliceViewOrientationComboBox, SIGNAL(activated(int)),
    q, SLOT(sliceViewOrientationChangedByUser()));

  q->registerProperty("Default3DView/BoxVisibility", this->ThreeDBoxVisibilityCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "3D view cube visibility");
  q->registerProperty("Default3DView/AxisLabelsVisibility", this->ThreeDAxisLabelsVisibilityCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "3D view axis label visibility");
  QObject::connect(this->ThreeDOrientationMarkerTypeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentThreeDOrientationMarkerTypeChanged(QString)));
  q->registerProperty("Default3DView/OrientationMarkerType", q,
                      "threeDOrientationMarkerType", SIGNAL(currentThreeDOrientationMarkerTypeChanged(QString)),
                      "3D view orientation marker type",
                      ctkSettingsPanel::OptionRequireRestart);
  QObject::connect(this->ThreeDOrientationMarkerSizeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentThreeDOrientationMarkerSizeChanged(QString)));
  q->registerProperty("Default3DView/OrientationMarkerSize", q,
                      "threeDOrientationMarkerSize", SIGNAL(currentThreeDOrientationMarkerSizeChanged(QString)),
                      "3D view orientation marker size",
                      ctkSettingsPanel::OptionRequireRestart);
  QObject::connect(this->ThreeDRulerTypeComboBox, SIGNAL(currentIndexChanged(QString)),
                   q, SIGNAL(currentThreeDRulerTypeChanged(QString)));
  q->registerProperty("Default3DView/RulerType", q,
                      "threeDRulerType", SIGNAL(currentThreeDRulerTypeChanged(QString)),
                      "3D view ruler type",
                      ctkSettingsPanel::OptionRequireRestart);
  q->registerProperty("Default3DView/UseDepthPeeling", this->ThreeDUseDepthPeelingCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "3D depth peeling");
  q->registerProperty("Default3DView/UseOrthographicProjection", this->ThreeDUseOrthographicProjectionCheckBox,
                      "checked", SIGNAL(toggled(bool)),
                      "Orthographic projection");

}

// --------------------------------------------------------------------------
// qCjyxSettingsViewsPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsViewsPanel::qCjyxSettingsViewsPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsViewsPanelPrivate(*this))
{
  Q_D(qCjyxSettingsViewsPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsViewsPanel::~qCjyxSettingsViewsPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::onMSAAChanged(const QString& text)
{
  /// For "ctkVTKAbstractView"s (the main data views for the program),
  /// the multisampling properties should be set *before* creating any
  /// OpenGL contexts, otherwise the setting may have no effect. This
  /// means that we must read in the user's MSAA settings from QSettings
  /// before setting up the MainWindow UI, since setting up the UI
  /// creates all the view panels (and their associated OpenGL contexts).
  ///
  /// Before the MainWindow is set up, the ViewsPanel is added to the
  /// main settings dialog, . If the saved value is true (the default is
  /// false), this triggers this method to be called, allowing it to be
  /// set prior to creation of the OpenGL contexts.

  Q_D(const qCjyxSettingsViewsPanel);
  const int index = d->MSAAComboBox->findText(text);
  const int nSamples = d->MSAAComboBox->itemData(index).toInt();
  ctkVTKAbstractView::setMultiSamples(nSamples);
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::currentMSAA() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->MSAAComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setCurrentMSAA(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to "Off" (0) if conversion fails
  d->MSAAComboBox->setCurrentIndex(qMax(d->MSAAComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::sliceOrientationMarkerType() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->SliceOrientationMarkerTypeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setSliceOrientationMarkerType(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->SliceOrientationMarkerTypeComboBox->setCurrentIndex(qMax(d->SliceOrientationMarkerTypeComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::sliceOrientationMarkerSize() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->SliceOrientationMarkerSizeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setSliceOrientationMarkerSize(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->SliceOrientationMarkerSizeComboBox->setCurrentIndex(qMax(d->SliceOrientationMarkerSizeComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::sliceRulerType() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->SliceRulerTypeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setSliceRulerType(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->SliceRulerTypeComboBox->setCurrentIndex(qMax(d->SliceRulerTypeComboBox->findText(text), 0));
}


// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::threeDOrientationMarkerType() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->ThreeDOrientationMarkerTypeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setThreeDOrientationMarkerType(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->ThreeDOrientationMarkerTypeComboBox->setCurrentIndex(qMax(d->ThreeDOrientationMarkerTypeComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::threeDOrientationMarkerSize() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->ThreeDOrientationMarkerSizeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setThreeDOrientationMarkerSize(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->ThreeDOrientationMarkerSizeComboBox->setCurrentIndex(qMax(d->ThreeDOrientationMarkerSizeComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
QString qCjyxSettingsViewsPanel::threeDRulerType() const
{
  Q_D(const qCjyxSettingsViewsPanel);
  return d->ThreeDRulerTypeComboBox->currentText();
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::setThreeDRulerType(const QString& text)
{
  Q_D(qCjyxSettingsViewsPanel);
  // default to first item if conversion fails
  d->ThreeDRulerTypeComboBox->setCurrentIndex(qMax(d->ThreeDRulerTypeComboBox->findText(text), 0));
}

// --------------------------------------------------------------------------
void qCjyxSettingsViewsPanel::sliceViewOrientationChangedByUser()
{
  Q_D(qCjyxSettingsViewsPanel);
  if (d->SliceViewOrientationComboBox->currentUserDataAsString() == "PatientRightIsScreenRight")
    {
    if (d->SliceOrientationMarkerTypeComboBox->currentText() == "none")
      {
      // Non-default orientation is chosen and no orientation marker is displayed.
      // To ensure that there is no accidental mixup of orientations, show the orientation marker.
      d->SliceOrientationMarkerTypeComboBox->setCurrentText("axes");
      }
    }
}
