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

#include "qCjyxDTISliceDisplayWidget.h"
#include "ui_qCjyxDTISliceDisplayWidget.h"

// Qt includes
#include <QDebug>

// MRML includes
#include "vtkMRMLDiffusionTensorDisplayPropertiesNode.h"
#include "vtkMRMLDiffusionTensorVolumeSliceDisplayNode.h"

// VTK includes
#include "vtkPolyData.h"

// STD includes

//-----------------------------------------------------------------------------
class qCjyxDTISliceDisplayWidgetPrivate
  : public Ui_qCjyxDTISliceDisplayWidget
{
  Q_DECLARE_PUBLIC(qCjyxDTISliceDisplayWidget);
protected:
  qCjyxDTISliceDisplayWidget* const q_ptr;
public:
  qCjyxDTISliceDisplayWidgetPrivate(qCjyxDTISliceDisplayWidget& object);
  ~qCjyxDTISliceDisplayWidgetPrivate();
  void init();
  void computeScalarBounds(double scalarBounds[2]);
  vtkWeakPointer<vtkMRMLDiffusionTensorVolumeSliceDisplayNode> DisplayNode;
};

//-----------------------------------------------------------------------------
qCjyxDTISliceDisplayWidgetPrivate
::qCjyxDTISliceDisplayWidgetPrivate(
  qCjyxDTISliceDisplayWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qCjyxDTISliceDisplayWidgetPrivate
::~qCjyxDTISliceDisplayWidgetPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidgetPrivate::init()
{
  Q_Q(qCjyxDTISliceDisplayWidget);

  this->setupUi(q);

  this->LineEigenVectorComboBox->setItemData(0, vtkMRMLDiffusionTensorDisplayPropertiesNode::Major);
  this->LineEigenVectorComboBox->setItemData(1, vtkMRMLDiffusionTensorDisplayPropertiesNode::Middle);
  this->LineEigenVectorComboBox->setItemData(2, vtkMRMLDiffusionTensorDisplayPropertiesNode::Minor);

  QObject::connect(this->GlyphVisibilityCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setVisibility(bool)));
  QObject::connect(this->GlyphOpacitySliderWidget, SIGNAL(valueChanged(double)),
                   q, SLOT(setOpacity(double)));
  QObject::connect(this->GlyphScalarColorTableComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
                   q, SLOT(setColorMap(vtkMRMLNode*)));
  QObject::connect(this->GlyphColorByScalarComboBox, SIGNAL(scalarInvariantChanged(int)),
                   q, SLOT(setColorGlyphBy(int)));
  QObject::connect(this->GlyphManualScalarRangeCheckBox, SIGNAL(toggled(bool)),
                   q, SLOT(setManualScalarRange(bool)));
  QObject::connect(this->GlyphScalarRangeWidget, SIGNAL(valuesChanged(double,double)),
                   q, SLOT(setScalarRange(double,double)));
  QObject::connect(this->GlyphGeometryComboBox, SIGNAL(currentIndexChanged(int)),
                   q, SLOT(setGlyphGeometry(int)));
  QObject::connect(this->GlyphScaleSliderWidget, SIGNAL(valueChanged(double)),
                   q, SLOT(setGlyphScaleFactor(double)));
  QObject::connect(this->GlyphSpacingSliderWidget, SIGNAL(valueChanged(double)),
                   q, SLOT(setGlyphSpacing(double)));
  QObject::connect(this->LineEigenVectorComboBox, SIGNAL(currentIndexChanged(int)),
                   q, SLOT(setGlyphEigenVector(int)));
  QObject::connect(this->TubeEigenVectorComboBox, SIGNAL(currentIndexChanged(int)),
                   q, SLOT(setGlyphEigenVector(int)));
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidgetPrivate::computeScalarBounds(double scalarBounds[2])
{
  Q_Q(qCjyxDTISliceDisplayWidget);
  const int ScalarInvariant = (q->displayPropertiesNode() ?
         q->displayPropertiesNode()->GetColorGlyphBy() : -1);

  if (vtkMRMLDiffusionTensorDisplayPropertiesNode::ScalarInvariantHasKnownScalarRange(ScalarInvariant))
    {
    vtkMRMLDiffusionTensorDisplayPropertiesNode::ScalarInvariantKnownScalarRange(ScalarInvariant, scalarBounds);
    }
  else
    {
    vtkPolyData* glyphs = this->DisplayNode ? this->DisplayNode->GetOutputPolyData() : nullptr;
    if (glyphs)
      {
      glyphs->GetScalarRange(scalarBounds);
      }
    }
//  Commented this so the glyphs and bundles are colored consistently
//  this->DisplayNode->GetPolyData();
//  scalarBounds[0] = qMin (scalarBounds[0], q->displayNode()->GetScalarRange()[0]);
//  scalarBounds[1] = qMax (scalarBounds[1], q->displayNode()->GetScalarRange()[1]);
}

// --------------------------------------------------------------------------
qCjyxDTISliceDisplayWidget
::qCjyxDTISliceDisplayWidget(QWidget* parentWidget)
  : Superclass(parentWidget)
  , d_ptr(new qCjyxDTISliceDisplayWidgetPrivate(*this))
{
  Q_D(qCjyxDTISliceDisplayWidget);
  d->init();

  // disable as there is no MRML Node associated with the widget
  this->setEnabled(d->DisplayNode != nullptr);
}

// --------------------------------------------------------------------------
qCjyxDTISliceDisplayWidget
::~qCjyxDTISliceDisplayWidget() = default;

// --------------------------------------------------------------------------
vtkMRMLDiffusionTensorDisplayPropertiesNode* qCjyxDTISliceDisplayWidget
::displayPropertiesNode()const
{
  Q_D(const qCjyxDTISliceDisplayWidget);
  return d->DisplayNode ?
    d->DisplayNode->GetDiffusionTensorDisplayPropertiesNode() : nullptr;
}

// --------------------------------------------------------------------------
vtkMRMLDiffusionTensorVolumeSliceDisplayNode* qCjyxDTISliceDisplayWidget
::displayNode()const
{
  Q_D(const qCjyxDTISliceDisplayWidget);
  return d->DisplayNode;
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setMRMLDTISliceDisplayNode(vtkMRMLNode* node)
{
  this->setMRMLDTISliceDisplayNode(
    vtkMRMLDiffusionTensorVolumeSliceDisplayNode::SafeDownCast(node));
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setMRMLDTISliceDisplayNode(
  vtkMRMLDiffusionTensorVolumeSliceDisplayNode* displayNode)
{
  Q_D(qCjyxDTISliceDisplayWidget);

  vtkMRMLDiffusionTensorVolumeSliceDisplayNode* oldDisplayNode = nullptr;
  vtkMRMLDiffusionTensorDisplayPropertiesNode* oldDisplayPropertiesNode = nullptr;
  if (displayNode)
    {
    oldDisplayNode = this->displayNode();
    oldDisplayPropertiesNode = this->displayPropertiesNode();
    }
  d->DisplayNode = displayNode;

  if (displayNode)
    {
    qvtkReconnect(oldDisplayNode, this->displayNode(),vtkCommand::ModifiedEvent,
                  this, SLOT(updateWidgetFromMRML()));
    qvtkReconnect(oldDisplayPropertiesNode, this->displayPropertiesNode(),
                  vtkCommand::ModifiedEvent, this, SLOT(updateWidgetFromMRML()));

    this->updateWidgetFromMRML();
    }
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::updateWidgetFromMRML()
{
  Q_D(qCjyxDTISliceDisplayWidget);
  this->setEnabled(d->DisplayNode != nullptr);
  if (!d->DisplayNode)
    {
    return;
    }

  d->GlyphVisibilityCheckBox->setChecked(
    d->DisplayNode->GetVisibility());
  d->GlyphOpacitySliderWidget->setValue(
    d->DisplayNode->GetOpacity());
  d->GlyphScalarColorTableComboBox->setCurrentNode(
    d->DisplayNode->GetColorNode());
  d->GlyphManualScalarRangeCheckBox->setChecked(
    d->DisplayNode->GetAutoScalarRange() == 0);
  double scalarBounds[2];
  d->computeScalarBounds(scalarBounds);
  double singleStep = qAbs(scalarBounds[1] - scalarBounds[0]) / 100.;
  double i = 1.;
  int decimals = 0;
  while (i > singleStep)
    {
    ++decimals;
    i /= 10.;
    }
  // TBD: blockSignals are not very important, just reduce the noise resulting
  // from unnecessary updates.
  d->GlyphScalarRangeWidget->blockSignals(true);
  d->GlyphScalarRangeWidget->setDecimals(decimals);
  d->GlyphScalarRangeWidget->setSingleStep(i);
  d->GlyphScalarRangeWidget->setRange(scalarBounds[0], scalarBounds[1]);
  d->GlyphScalarRangeWidget->blockSignals(false);
  double scalarRange[2];
  d->DisplayNode->GetScalarRange(scalarRange);
  d->GlyphScalarRangeWidget->setValues(scalarRange[0], scalarRange[1]);

  vtkMRMLDiffusionTensorDisplayPropertiesNode* displayPropertiesNode =
    this->displayPropertiesNode();
  if (displayPropertiesNode)
    {
    d->GlyphColorByScalarComboBox->setScalarInvariant(displayPropertiesNode->GetColorGlyphBy());
    d->GlyphGeometryComboBox->setCurrentIndex(displayPropertiesNode->GetGlyphGeometry());
    d->GlyphScaleSliderWidget->setValue(displayPropertiesNode->GetGlyphScaleFactor());
    d->GlyphSpacingSliderWidget->setValue(
      displayPropertiesNode->GetLineGlyphResolution());
    int index = d->LineEigenVectorComboBox->findData(
      QVariant(displayPropertiesNode->GetGlyphEigenvector()));
    d->LineEigenVectorComboBox->setCurrentIndex(index);
    d->TubeEigenVectorComboBox->setCurrentIndex(index);
    }
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setColorGlyphBy(int scalarInvariant)
{
  Q_D(qCjyxDTISliceDisplayWidget);

  if (!this->displayPropertiesNode())
    {
    return;
    }
  this->displayPropertiesNode()->SetColorGlyphBy(scalarInvariant);

  if ( d->DisplayNode && (
    this->displayPropertiesNode()->GetColorGlyphBy() == vtkMRMLDiffusionTensorDisplayPropertiesNode::ColorOrientation ||
    this->displayPropertiesNode()->GetColorGlyphBy() == vtkMRMLDiffusionTensorDisplayPropertiesNode::ColorOrientationMiddleEigenvector ||
    this->displayPropertiesNode()->GetColorGlyphBy() == vtkMRMLDiffusionTensorDisplayPropertiesNode::ColorOrientationMinEigenvector
    ) )
  {
    d->GlyphScalarColorTableComboBox->setEnabled(false);
    d->DisplayNode->AutoScalarRangeOn();
  } else {
    d->GlyphScalarColorTableComboBox->setEnabled(true);
  }

  if (d->DisplayNode && (d->DisplayNode->GetAutoScalarRange()))
  {
    double scalarRange[2];
    d->DisplayNode->GetScalarRange(scalarRange);

    this->setScalarRange(scalarRange[0], scalarRange[1]);
  }
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setColorMap(vtkMRMLNode* colorNode)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!d->DisplayNode || !colorNode)
    {
    return;
    }
  d->DisplayNode->SetAndObserveColorNodeID(colorNode ? colorNode->GetID() : "");
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setOpacity(double opacity)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!d->DisplayNode)
    {
    return;
    }
  d->DisplayNode->SetOpacity(opacity);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setVisibility(bool visible)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!d->DisplayNode)
    {
    return;
    }
  d->DisplayNode->SetVisibility(visible);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setManualScalarRange(bool manual)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!d->DisplayNode)
    {
    return;
    }
  d->DisplayNode->SetAutoScalarRange(manual ? 0 : 1);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setScalarRange(double min, double max)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!d->DisplayNode)
    {
    return;
    }
  d->DisplayNode->SetScalarRange(min, max);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setGlyphGeometry(int index)
{
  if (!this->displayPropertiesNode())
    {
    return;
    }
  // 0 = Lines
  // 1 = Tubes
  // 2 = Ellipsoids
  // 3 = Superquadrics
  this->displayPropertiesNode()->SetGlyphGeometry(index);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setGlyphScaleFactor(double scaleFactor)
{
  if (!this->displayPropertiesNode())
    {
    return;
    }
  this->displayPropertiesNode()->SetGlyphScaleFactor(scaleFactor);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setGlyphSpacing(double spacing)
{
  if (!this->displayPropertiesNode())
    {
    return;
    }
  this->displayPropertiesNode()->SetLineGlyphResolution(spacing);
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setGlyphEigenVector(int index)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  if (!this->displayPropertiesNode())
    {
    return;
    }
  int eigenVector = d->LineEigenVectorComboBox->itemData(index).toInt();
  this->displayPropertiesNode()->SetGlyphEigenvector(eigenVector);
}

// --------------------------------------------------------------------------
bool qCjyxDTISliceDisplayWidget::isVisibilityHidden()const
{
  Q_D(const qCjyxDTISliceDisplayWidget);
  return d->GlyphVisibilityLabel->isVisibleTo(
    const_cast<qCjyxDTISliceDisplayWidget*>(this));
}

// --------------------------------------------------------------------------
void qCjyxDTISliceDisplayWidget::setVisibilityHidden(bool hide)
{
  Q_D(qCjyxDTISliceDisplayWidget);
  d->GlyphVisibilityLabel->setVisible(!hide);
  d->GlyphVisibilityCheckBox->setVisible(!hide);
}
