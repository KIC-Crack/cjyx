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

  This file was originally developed by Michael Jeulin-Lagarrigue, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QButtonGroup>
#include <QMenu>
#include <QString>

// Cjyx includes
#include "qMRMLSliceControllerWidget_p.h" // For updateSliceOrientationSelector
#include "vtkMRMLSliceNode.h"
#include "vtkCjyxReformatLogic.h"

#include "qCjyxReformatModuleWidget.h"
#include "ui_qCjyxReformatModuleWidget.h"

// MRML includes
#include "vtkMRMLApplicationLogic.h"
#include "vtkMRMLCameraNode.h"
#include "vtkMRMLScene.h"
#include "vtkMRMLSliceCompositeNode.h"
#include "vtkMRMLSliceLogic.h"
#include "vtkMRMLVolumeNode.h"

// VTK includes
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkNew.h>
#include <vtkTransform.h>

//------------------------------------------------------------------------------
class qCjyxReformatModuleWidgetPrivate :
public Ui_qCjyxReformatModuleWidget
{
  Q_DECLARE_PUBLIC(qCjyxReformatModuleWidget);
protected:
  qCjyxReformatModuleWidget* const q_ptr;

public:
  qCjyxReformatModuleWidgetPrivate(
    qCjyxReformatModuleWidget& object);

  /// Update the widget interface
  void updateUi();

  /// Update the visibility controllers
  void updateVisibilityControllers();

  /// Update slice offset range and resolution (increment)
  void updateOffsetSlidersGroupBox();

  /// Update the origin position
  void updateOriginCoordinates();

  /// Update orientation selector state
  void updateOrientationGroupBox();

  /// Reset the slider
  void resetSlider(qMRMLLinearTransformSlider*);

  /// Setup the reformate option menu associated to the button
  void setupReformatOptionsMenu();

  QButtonGroup* OriginCoordinateReferenceButtonGroup;
  vtkMRMLSliceNode* MRMLSliceNode;
  vtkMRMLSliceLogic* MRMLSliceLogic;
  double LastRotationValues[3]; // LR, PA, IS
};

//------------------------------------------------------------------------------
// qCjyxReformatModuleWidgetPrivate methods

//------------------------------------------------------------------------------
qCjyxReformatModuleWidgetPrivate::
qCjyxReformatModuleWidgetPrivate(
  qCjyxReformatModuleWidget& object)
  : q_ptr(&object)
{
  this->OriginCoordinateReferenceButtonGroup = nullptr;
  this->MRMLSliceNode = nullptr;
  this->MRMLSliceLogic = nullptr;
  this->LastRotationValues[qCjyxReformatModuleWidget::axisX] = 0;
  this->LastRotationValues[qCjyxReformatModuleWidget::axisY] = 0;
  this->LastRotationValues[qCjyxReformatModuleWidget::axisZ] = 0;
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::setupReformatOptionsMenu()
{
  Q_Q(qCjyxReformatModuleWidget);

  QMenu* reformatMenu =
    new QMenu(qCjyxReformatModuleWidget::tr("Reformat"),this->ShowReformatWidgetToolButton);

  reformatMenu->addAction(this->actionLockNormalToCamera);

  QObject::connect(this->actionLockNormalToCamera, SIGNAL(triggered(bool)),
                   q, SLOT(onLockReformatWidgetToCamera(bool)));

  this->ShowReformatWidgetToolButton->setMenu(reformatMenu);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::updateUi()
{
  this->updateVisibilityControllers();
  this->updateOffsetSlidersGroupBox();
  this->updateOriginCoordinates();
  this->updateOrientationGroupBox();
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::updateVisibilityControllers()
{
  // Check slice visibility
  bool wasVisibilityCheckBoxBlocking =
    this->VisibilityCheckBox->blockSignals(true);

  this->VisibilityCheckBox->setEnabled(this->MRMLSliceNode != nullptr);

  int visibility =
    (this->MRMLSliceNode) ? this->MRMLSliceNode->GetSliceVisible() : 0;
  this->VisibilityCheckBox->setChecked(visibility);

  this->VisibilityCheckBox->blockSignals(wasVisibilityCheckBoxBlocking);

  // Check reformat widget visibility
  bool wasVisibilityReformatWidgetCheckBoxBlocking =
    this->ShowReformatWidgetToolButton->blockSignals(true);
  bool wasLockReformatWidgetCheckBoxBlocking =
    this->actionLockNormalToCamera->blockSignals(true);
  bool wasLockReformatWidgetCheckBoxButtonBlocking =
    this->NormalToCameraCheckablePushButton->blockSignals(true);

  this->ShowReformatWidgetToolButton->setEnabled(this->MRMLSliceNode != nullptr);

  int widgetVisibility =
    (this->MRMLSliceNode) ? this->MRMLSliceNode->GetWidgetVisible() : 0;
  int lockWidgetNormal = (this->MRMLSliceNode) ?
    this->MRMLSliceNode->GetWidgetNormalLockedToCamera() : 0;

  this->ShowReformatWidgetToolButton->setChecked(widgetVisibility);
  this->actionLockNormalToCamera->setChecked(lockWidgetNormal);
  this->NormalToCameraCheckablePushButton->setChecked(lockWidgetNormal);
  this->NormalToCameraCheckablePushButton->setCheckState(
    (lockWidgetNormal) ? Qt::Checked : Qt::Unchecked);

  this->ShowReformatWidgetToolButton->blockSignals(
    wasVisibilityReformatWidgetCheckBoxBlocking);
  this->actionLockNormalToCamera->blockSignals(
    wasLockReformatWidgetCheckBoxBlocking);
  this->NormalToCameraCheckablePushButton->blockSignals(
    wasLockReformatWidgetCheckBoxButtonBlocking);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::updateOffsetSlidersGroupBox()
{
  if (!this->MRMLSliceNode || !this->MRMLSliceLogic)
    {
    return;
    }

  bool wasBlocking = this->OffsetSlider->blockSignals(true);

  // Set the scale increments to match the z spacing (rotated into slice space)
  const double * sliceSpacing =
    this->MRMLSliceLogic->GetLowestVolumeSliceSpacing();
  Q_ASSERT(sliceSpacing);
  double offsetResolution = sliceSpacing ? sliceSpacing[2] : 0;
  this->OffsetSlider->setSingleStep(offsetResolution);
  this->OffsetSlider->setPageStep(offsetResolution);

  // Set slice offset range to match the field of view
  // Calculate the number of slices in the current range
  double sliceBounds[6] = {0, -1, 0, -1, 0, -1};
  this->MRMLSliceLogic->GetLowestVolumeSliceBounds(sliceBounds);
  Q_ASSERT(sliceBounds[4] <= sliceBounds[5]);
  this->OffsetSlider->setRange(sliceBounds[4], sliceBounds[5]);

  // Update slider position
  this->OffsetSlider->setValue(this->MRMLSliceLogic->GetSliceOffset());
  this->OffsetSlider->blockSignals(wasBlocking);
}

void qCjyxReformatModuleWidgetPrivate::updateOriginCoordinates()
{
  Q_Q(qCjyxReformatModuleWidget);

  vtkCjyxReformatLogic* reformatLogic =
    vtkCjyxReformatLogic::SafeDownCast(q->logic());

  if (!this->MRMLSliceNode || !reformatLogic)
    {
    return;
    }

  // Block signals
  //bool wasOnPlaneXBlocking = this->OnPlaneXdoubleSpinBox->blockSignals(true);
  //bool wasOnPlaneYBlocking = this->OnPlaneYdoubleSpinBox->blockSignals(true);
  bool wasInVolumeBlocking = this->InVolumeCoordinatesWidget->blockSignals(true);

  // Update volumes extremums
  double volumeBounds[6] = {0, 0, 0, 0, 0, 0};
  vtkCjyxReformatLogic::GetVolumeBounds(this->MRMLSliceNode, volumeBounds);

  /// TODO: set min/max per element
  double minimum = qMin(volumeBounds[0], qMin(volumeBounds[2], volumeBounds[4]));
  double maximum = qMax(volumeBounds[1], qMax(volumeBounds[3], volumeBounds[5]));
  this->InVolumeCoordinatesWidget->setMinimum(minimum);
  this->InVolumeCoordinatesWidget->setMaximum(maximum);

  // TODO : Update plane extremums
  /*
  double sliceBounds[6] = {0, 0, 0, 0, 0, 0};
  this->MRMLSliceLogic->GetLowestVolumeSliceBounds(sliceBounds);

  this->OnPlaneXdoubleSpinBox->setMinimum(sliceBounds[0]);
  this->OnPlaneXdoubleSpinBox->setMaximum(sliceBounds[1]);
  this->OnPlaneYdoubleSpinBox->setMinimum(sliceBounds[2]);
  this->OnPlaneYdoubleSpinBox->setMaximum(sliceBounds[3]);
  */

  // Update volumes origin coordinates
  vtkMatrix4x4* sliceToRAS = this->MRMLSliceNode->GetSliceToRAS();
  this->InVolumeCoordinatesWidget->setCoordinates(sliceToRAS->GetElement(0,3),
                                                  sliceToRAS->GetElement(1,3),
                                                  sliceToRAS->GetElement(2,3));

  // TODO : Update plane origin coordinates

  // Reset signals blocking
  //this->OnPlaneXdoubleSpinBox->blockSignals(wasOnPlaneXBlocking);
  //this->OnPlaneYdoubleSpinBox->blockSignals(wasOnPlaneYBlocking);
  this->InVolumeCoordinatesWidget->blockSignals(wasInVolumeBlocking);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::updateOrientationGroupBox()
{
  if (!this->MRMLSliceNode)
    {
    this->SliceOrientationSelector->setCurrentIndex(-1);
    return;
    }

  qMRMLSliceControllerWidgetPrivate::updateSliceOrientationSelector(
        this->MRMLSliceNode, this->SliceOrientationSelector);

  // Update the normal spinboxes
  bool wasNormalBlocking = this->NormalCoordinatesWidget->blockSignals(true);

  double normal[3];
  vtkMatrix4x4* sliceToRAS = this->MRMLSliceNode->GetSliceToRAS();

  normal[0] = sliceToRAS->GetElement(0,2);
  normal[1] = sliceToRAS->GetElement(1,2);
  normal[2] = sliceToRAS->GetElement(2,2);

  this->NormalCoordinatesWidget->setCoordinates(normal);
  this->NormalCoordinatesWidget->blockSignals(wasNormalBlocking);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidgetPrivate::
resetSlider(qMRMLLinearTransformSlider* slider)
{
  bool wasSliderBlocking = slider->blockSignals(true);
  slider->reset();

  if (slider == this->LRSlider)
    {
    this->LastRotationValues[qCjyxReformatModuleWidget::axisX] = slider->value();
    }
  else if (slider == this->PASlider)
    {
    this->LastRotationValues[qCjyxReformatModuleWidget::axisY] = slider->value();
    }
  else if (slider == this->ISSlider)
    {
    this->LastRotationValues[qCjyxReformatModuleWidget::axisZ] = slider->value();
    }

  slider->blockSignals(wasSliderBlocking);
}

//------------------------------------------------------------------------------
// qCjyxReformatModuleWidget methods

//------------------------------------------------------------------------------
qCjyxReformatModuleWidget::qCjyxReformatModuleWidget(
  QWidget* _parent) : Superclass( _parent ),
  d_ptr( new qCjyxReformatModuleWidgetPrivate(*this) )
{
}

//------------------------------------------------------------------------------
qCjyxReformatModuleWidget::~qCjyxReformatModuleWidget() = default;

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setup()
{
  Q_D(qCjyxReformatModuleWidget);
  d->setupUi(this);

  // Populate the Linked menu
  d->setupReformatOptionsMenu();

  // Connect node selector with module itself
  this->connect(d->VisibilityCheckBox,
                   SIGNAL(toggled(bool)),
                   this, SLOT(onSliceVisibilityChanged(bool)));
  this->connect(d->ShowReformatWidgetToolButton,
                   SIGNAL(toggled(bool)),
                   this, SLOT(onReformatWidgetVisibilityChanged(bool)));

  this->connect(d->SliceNodeSelector,
                SIGNAL(currentNodeChanged(vtkMRMLNode*)),
                SLOT(onNodeSelected(vtkMRMLNode*)));

  // Connect Slice offset slider
  this->connect(d->OffsetSlider, SIGNAL(valueChanged(double)),
                this, SLOT(setSliceOffsetValue(double)), Qt::QueuedConnection);
  this->connect(d->OffsetSlider, SIGNAL(valueIsChanging(double)),
                this, SLOT(onTrackSliceOffsetValueChanged(double)),
                Qt::QueuedConnection);

  // Add origin coordinate reference button to a button group
  d->OriginCoordinateReferenceButtonGroup =
    new QButtonGroup(d->OriginCoordinateReferenceButtonGroup);
  d->OriginCoordinateReferenceButtonGroup->addButton(d->OnPlaneRadioButton,
    qCjyxReformatModuleWidget::ONPLANE);
  d->OriginCoordinateReferenceButtonGroup->addButton(d->InVolumeRadioButton,
    qCjyxReformatModuleWidget::INVOLUME);

  // Plane coordinate system is not supported for now
  d->CoordinateReferenceGroupBox->setHidden(true);
  d->InVolumeRadioButton->setChecked(true);
  d->OnPlaneGroupBox->setHidden(true);

  // Connect button group
  this->connect(d->OriginCoordinateReferenceButtonGroup,
                SIGNAL(buttonPressed(int)),
                SLOT(onOriginCoordinateReferenceButtonPressed(int)));

  // Connect World Coordinates of origin spinBoxes
  this->connect(d->InVolumeCoordinatesWidget, SIGNAL(coordinatesChanged(double*)),
                this, SLOT(setWorldPosition(double*)));

  // Connect Orientation selector
  this->connect(d->SliceOrientationSelector, SIGNAL(currentIndexChanged(QString)),
                this, SLOT(onSliceOrientationChanged(QString)));

  // Connect the recenter
  this->connect(d->CenterPushButton, SIGNAL(pressed()),
                this, SLOT(centerSliceNode()));

  // Connect slice normal spinBoxes
  this->connect(d->NormalCoordinatesWidget, SIGNAL(coordinatesChanged(double*)),
                this, SLOT(setSliceNormal(double*)));

  // Connect slice normal pushButtons
  this->connect(d->NormalXPushButton, SIGNAL(pressed()),
                this, SLOT(setNormalToAxisX()));
  this->connect(d->NormalYPushButton, SIGNAL(pressed()),
                this, SLOT(setNormalToAxisY()));
  this->connect(d->NormalZPushButton, SIGNAL(pressed()),
                this, SLOT(setNormalToAxisZ()));

  QObject::connect(d->NormalToCameraCheckablePushButton, SIGNAL(clicked()),
                   this, SLOT(setNormalToCamera()));
  QObject::connect(d->NormalToCameraCheckablePushButton,
                   SIGNAL(checkBoxToggled(bool)),
                   this, SLOT(onLockReformatWidgetToCamera(bool)));

  // Connect Slice rotation sliders
  this->connect(d->LRSlider, SIGNAL(valueChanged(double)),
                this, SLOT(onSliderRotationChanged(double)));
  this->connect(d->PASlider, SIGNAL(valueChanged(double)),
                this, SLOT(onSliderRotationChanged(double)));
  this->connect(d->ISSlider, SIGNAL(valueChanged(double)),
                this, SLOT(onSliderRotationChanged(double)));
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onMRMLSliceNodeModified(vtkObject* caller)
{
  Q_D(qCjyxReformatModuleWidget);

  vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(caller);
  if (!sliceNode)
    {
    return;
    }

  d->updateUi();
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::onNodeSelected(vtkMRMLNode* node)
{
  Q_D(qCjyxReformatModuleWidget);

  vtkMRMLSliceNode* sliceNode = vtkMRMLSliceNode::SafeDownCast(node);

  // Listen for SliceNode changes
  this->qvtkReconnect(d->MRMLSliceNode, sliceNode,
    vtkCommand::ModifiedEvent,
    this, SLOT(onMRMLSliceNodeModified(vtkObject*)));

  d->MRMLSliceNode = sliceNode;
  d->MRMLSliceLogic =
    this->logic()->GetMRMLApplicationLogic()->GetSliceLogic(d->MRMLSliceNode);

  d->updateUi();
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::onSliceVisibilityChanged(bool visible)
{
  std::cout << "onSliceVisibilityChanged" << std::endl;
  Q_D(qCjyxReformatModuleWidget);
  if (!d->MRMLSliceNode)
    {
    return;
    }

  d->MRMLSliceNode->SetSliceVisible(visible);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onReformatWidgetVisibilityChanged(bool visible)
{
  Q_D(qCjyxReformatModuleWidget);
  if (!d->MRMLSliceNode)
    {
    return;
    }

  if (visible)
    {
    d->MRMLSliceNode->SetSliceVisible(visible);
    }

  d->MRMLSliceNode->SetWidgetVisible(visible);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::onLockReformatWidgetToCamera(bool lock)
{
  Q_D(qCjyxReformatModuleWidget);
  if (!d->MRMLSliceNode)
    {
    return;
    }
  if (lock)
    {
    // "Lock to slice plane" only works if widget is visible, show it now
    d->MRMLSliceNode->SetWidgetVisible(true);
    }

  d->MRMLSliceNode->SetWidgetNormalLockedToCamera(lock);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onOriginCoordinateReferenceButtonPressed(int ref)
{
  Q_D(qCjyxReformatModuleWidget);

  d->OnPlaneGroupBox->setHidden(ref == qCjyxReformatModuleWidget::INVOLUME);
  d->InVolumeCoordinatesWidget->setHidden(ref != qCjyxReformatModuleWidget::INVOLUME);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
setSliceOffsetValue(double offset)
{
  Q_D(qCjyxReformatModuleWidget);
  if (!d->MRMLSliceLogic)
    {
    return;
    }

  d->MRMLSliceLogic->StartSliceOffsetInteraction();
  d->MRMLSliceLogic->SetSliceOffset(offset);
  d->MRMLSliceLogic->EndSliceOffsetInteraction();
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onTrackSliceOffsetValueChanged(double offset)
{
  Q_D(qCjyxReformatModuleWidget);
  if (!d->MRMLSliceLogic)
    {
    return;
    }

  d->MRMLSliceLogic->StartSliceOffsetInteraction();
  d->MRMLSliceLogic->SetSliceOffset(offset);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setWorldPosition(double* worldCoordinates)
{
  Q_D(qCjyxReformatModuleWidget);

  vtkCjyxReformatLogic* reformatLogic =
    vtkCjyxReformatLogic::SafeDownCast(this->logic());

  if (!d->MRMLSliceNode || !reformatLogic)
    {
    return;
    }

  // Insert the widget translation
  vtkCjyxReformatLogic::SetSliceOrigin(d->MRMLSliceNode, worldCoordinates);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setSliceNormal(double x, double y, double z)
{
  double sliceNormal[3] = {x,y,z};
  this->setSliceNormal(sliceNormal);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setNormalToCamera()
{
  Q_D(qCjyxReformatModuleWidget);

  vtkCjyxReformatLogic* reformatLogic =
    vtkCjyxReformatLogic::SafeDownCast(this->logic());

  if (!reformatLogic)
    {
    return;
    }

  // NOTE: We use the first Camera because there is no notion of active scene
  // Code to be changed when methods available.
  vtkMRMLCameraNode* cameraNode = vtkMRMLCameraNode::SafeDownCast(
    reformatLogic->GetMRMLScene()->GetFirstNodeByClass("vtkMRMLCameraNode"));

  if (!cameraNode)
    {
    return;
    }

  // When the user clicks the "Normal to camera button" and the checkbox was checked,
  // then make sure the checkbox becomes unchecked, too, to make it clear to the user
  // that the slice view does not follow the camera normal anymore
  if (d->NormalToCameraCheckablePushButton->checkState() == Qt::Checked)
    {
    d->NormalToCameraCheckablePushButton->setCheckState(Qt::Unchecked);
    }

  double camNormal[3];
  cameraNode->GetCamera()->GetViewPlaneNormal(camNormal);
  this->setSliceNormal(camNormal);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setNormalToAxisX()
{
  this->onSliceNormalToAxisChanged(axisX);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setNormalToAxisY()
{
  this->onSliceNormalToAxisChanged(axisY);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setNormalToAxisZ()
{
  this->onSliceNormalToAxisChanged(axisZ);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::onSliceNormalToAxisChanged(AxesReferenceType
                                                             axis)
{
  double sliceNormal[3];
  sliceNormal[0] = (axis == axisX) ? 1. : 0.;
  sliceNormal[1] = (axis == axisY) ? 1. : 0.;
  sliceNormal[2] = (axis == axisZ) ? 1. : 0.;

  // Insert the widget rotation
  this->setSliceNormal(sliceNormal);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::setSliceNormal(double* sliceNormal)
{
  Q_D(qCjyxReformatModuleWidget);

  vtkCjyxReformatLogic* reformatLogic =
    vtkCjyxReformatLogic::SafeDownCast(this->logic());

  if (!d->MRMLSliceNode || !reformatLogic)
    {
    return;
    }

  // Reset rotation sliders
  d->resetSlider(d->LRSlider);
  d->resetSlider(d->PASlider);
  d->resetSlider(d->ISSlider);

  double normalizedSliceNormal[3] = {sliceNormal[0], sliceNormal[1], sliceNormal[2]};
  vtkMath::Normalize(normalizedSliceNormal);

  // Insert the widget rotation
  vtkCjyxReformatLogic::SetSliceNormal(d->MRMLSliceNode, normalizedSliceNormal);
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onSliceOrientationChanged(const QString& orientation)
{
  Q_D(qCjyxReformatModuleWidget);

  if (!d->MRMLSliceNode)
    {
    return;
    }

  // Reset the Rotation Sliders
  d->resetSlider(d->LRSlider);
  d->resetSlider(d->PASlider);
  d->resetSlider(d->ISSlider);

  d->MRMLSliceNode->SetOrientation(orientation.toUtf8());
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::
onSliderRotationChanged(double rotation)
{
  Q_D(qCjyxReformatModuleWidget);

  vtkNew<vtkTransform> transform;
  transform->SetMatrix(d->MRMLSliceNode->GetSliceToRAS());

  if (this->sender() == d->LRSlider)
    {
    // Reset PA & IS sliders
    d->resetSlider(d->PASlider);
    d->resetSlider(d->ISSlider);

    // Rotate on LR given the angle with the last value reccorded
    transform->RotateX(rotation-d->LastRotationValues[axisX]);

    // Update last value and apply the transform
    d->LastRotationValues[axisX] = rotation;
    }
  else if (this->sender() == d->PASlider)
    {
    // Reset LR & IS sliders
    d->resetSlider(d->LRSlider);
    d->resetSlider(d->ISSlider);

    // Rotate on PA given the angle with the last value reccorded
    transform->RotateY(rotation-d->LastRotationValues[axisY]);

    // Update last value and apply the transform
    d->LastRotationValues[axisY] = rotation;
    }
  else if (this->sender() == d->ISSlider)
    {
      // Reset LR & PA sliders
      d->resetSlider(d->LRSlider);
      d->resetSlider(d->PASlider);

      // Rotate on PA given the angle with the last value reccorded
      transform->RotateZ(rotation-d->LastRotationValues[axisZ]);

      // Update last value and apply the transform
      d->LastRotationValues[axisZ] = rotation;
    }

  // Apply the transform
  d->MRMLSliceNode->GetSliceToRAS()->DeepCopy(transform->GetMatrix());
  d->MRMLSliceNode->UpdateMatrices();
}

//------------------------------------------------------------------------------
void qCjyxReformatModuleWidget::centerSliceNode()
{
  Q_D(qCjyxReformatModuleWidget);

  vtkCjyxReformatLogic* reformatLogic =
    vtkCjyxReformatLogic::SafeDownCast(this->logic());

  if (!d->MRMLSliceNode || !d->MRMLSliceLogic || !reformatLogic)
    {
    return;
    }

  // TODO add the recenter given the Plane Referentiel

  // Retrieve the center given the volume bounds
  double bounds[6], center[3];
  vtkCjyxReformatLogic::GetVolumeBounds(d->MRMLSliceNode, bounds);
  vtkCjyxReformatLogic::GetCenterFromBounds(bounds, center);

  // Apply the center
  vtkCjyxReformatLogic::SetSliceOrigin(d->MRMLSliceNode, center);
}

//-----------------------------------------------------------
bool qCjyxReformatModuleWidget::setEditedNode(vtkMRMLNode* node,
                                                QString role /* = QString()*/,
                                                QString context /* = QString()*/)
{
  Q_D(qCjyxReformatModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);

  if (vtkMRMLSliceNode::SafeDownCast(node))
    {
    d->SliceNodeSelector->setCurrentNode(node);
    return true;
    }

  if (vtkMRMLSliceCompositeNode::SafeDownCast(node))
    {
    vtkMRMLSliceCompositeNode* sliceCompositeNode = vtkMRMLSliceCompositeNode::SafeDownCast(node);
    vtkMRMLSliceNode* sliceNode = vtkMRMLSliceLogic::GetSliceNode(sliceCompositeNode);
    if (!sliceNode)
      {
      return false;
      }
    d->SliceNodeSelector->setCurrentNode(sliceNode);
    return true;
    }

  return false;
}
