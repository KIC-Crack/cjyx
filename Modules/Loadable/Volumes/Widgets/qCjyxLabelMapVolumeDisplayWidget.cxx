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

#include "qCjyxLabelMapVolumeDisplayWidget.h"
#include "ui_qCjyxLabelMapVolumeDisplayWidget.h"

// Qt includes

// MRML includes
#include "vtkMRMLColorNode.h"
#include "vtkMRMLLabelMapVolumeDisplayNode.h"
#include "vtkMRMLScalarVolumeNode.h"

// VTK includes

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Volumes
class qCjyxLabelMapVolumeDisplayWidgetPrivate:
                                          public Ui_qCjyxLabelMapVolumeDisplayWidget
{
  Q_DECLARE_PUBLIC(qCjyxLabelMapVolumeDisplayWidget);
protected:
  qCjyxLabelMapVolumeDisplayWidget* const q_ptr;
public:
  qCjyxLabelMapVolumeDisplayWidgetPrivate(qCjyxLabelMapVolumeDisplayWidget& object);
  ~qCjyxLabelMapVolumeDisplayWidgetPrivate();
  void init();

  vtkWeakPointer<vtkMRMLScalarVolumeNode> VolumeNode;
};

//-----------------------------------------------------------------------------
qCjyxLabelMapVolumeDisplayWidgetPrivate::qCjyxLabelMapVolumeDisplayWidgetPrivate(qCjyxLabelMapVolumeDisplayWidget& object)
  : q_ptr(&object)
{
  this->VolumeNode = nullptr;
}

//-----------------------------------------------------------------------------
qCjyxLabelMapVolumeDisplayWidgetPrivate::~qCjyxLabelMapVolumeDisplayWidgetPrivate() = default;

//-----------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidgetPrivate::init()
{
  Q_Q(qCjyxLabelMapVolumeDisplayWidget);

  this->setupUi(q);
  QObject::connect(this->ColorTableComboBox, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
                   q, SLOT(setColorNode(vtkMRMLNode*)));
  QObject::connect(this->SliceIntersectionThicknessSpinBox, SIGNAL(valueChanged(int)),
                   q, SLOT(setSliceIntersectionThickness(int)));

  // disable as there is not MRML Node associated with the widget
  q->setEnabled(false);
}

// --------------------------------------------------------------------------
qCjyxLabelMapVolumeDisplayWidget::qCjyxLabelMapVolumeDisplayWidget(QWidget* _parent) : Superclass(_parent)
  , d_ptr(new qCjyxLabelMapVolumeDisplayWidgetPrivate(*this))
{
  Q_D(qCjyxLabelMapVolumeDisplayWidget);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxLabelMapVolumeDisplayWidget::~qCjyxLabelMapVolumeDisplayWidget() = default;

// --------------------------------------------------------------------------
vtkMRMLScalarVolumeNode* qCjyxLabelMapVolumeDisplayWidget::volumeNode()const
{
  Q_D(const qCjyxLabelMapVolumeDisplayWidget);
  return d->VolumeNode;
}

// --------------------------------------------------------------------------
vtkMRMLLabelMapVolumeDisplayNode* qCjyxLabelMapVolumeDisplayWidget::volumeDisplayNode()const
{
  Q_D(const qCjyxLabelMapVolumeDisplayWidget);
  return d->VolumeNode ? vtkMRMLLabelMapVolumeDisplayNode::SafeDownCast(
    d->VolumeNode->GetVolumeDisplayNode()) : nullptr;
}

// --------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidget::setMRMLVolumeNode(vtkMRMLNode* node)
{
  this->setMRMLVolumeNode(vtkMRMLScalarVolumeNode::SafeDownCast(node));
}

// --------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidget::setMRMLVolumeNode(vtkMRMLScalarVolumeNode* volumeNode)
{
  Q_D(qCjyxLabelMapVolumeDisplayWidget);
  vtkMRMLLabelMapVolumeDisplayNode* oldVolumeDisplayNode = this->volumeDisplayNode();

  qvtkReconnect(oldVolumeDisplayNode, volumeNode ? volumeNode->GetVolumeDisplayNode() : nullptr,
                vtkCommand::ModifiedEvent,
                this, SLOT(updateWidgetFromMRML()));
  d->VolumeNode = volumeNode;
  this->setEnabled(volumeNode != nullptr);
  this->updateWidgetFromMRML();
}

// --------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidget::updateWidgetFromMRML()
{
  Q_D(qCjyxLabelMapVolumeDisplayWidget);
  vtkMRMLLabelMapVolumeDisplayNode* displayNode =
    this->volumeDisplayNode();
  if (displayNode)
    {
    d->ColorTableComboBox->setCurrentNode(displayNode->GetColorNode());
    d->SliceIntersectionThicknessSpinBox->setValue(
       displayNode->GetSliceIntersectionThickness());
    }
}

// --------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidget::setColorNode(vtkMRMLNode* colorNode)
{
  vtkMRMLLabelMapVolumeDisplayNode* displayNode =
    this->volumeDisplayNode();
  if (!displayNode || !colorNode)
    {
    return;
    }
  Q_ASSERT(vtkMRMLColorNode::SafeDownCast(colorNode));
  displayNode->SetAndObserveColorNodeID(colorNode->GetID());
}

// --------------------------------------------------------------------------
void qCjyxLabelMapVolumeDisplayWidget::setSliceIntersectionThickness(int thickness)
{
  vtkMRMLLabelMapVolumeDisplayNode* displayNode =
    this->volumeDisplayNode();
  if (!displayNode)
    {
    return;
    }
  displayNode->SetSliceIntersectionThickness(thickness);
}

//------------------------------------------------------------------------------
int qCjyxLabelMapVolumeDisplayWidget::sliceIntersectionThickness()const
{
  Q_D(const qCjyxLabelMapVolumeDisplayWidget);
  return d->SliceIntersectionThicknessSpinBox->value();
}
