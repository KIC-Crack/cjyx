/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, Cancer
  Care Ontario, OpenAnatomy, and Brigham and Women's Hospital through NIH grant R01MH112748.

==============================================================================*/

// Qt includes

// qCjyx includes
#include "qCjyxNodeWriterOptionsWidget_p.h"
#include "qCjyxSegmentationsNodeWriterOptionsWidget.h"

// MRML includes
#include <vtkMRMLStorableNode.h>
#include <vtkMRMLSegmentationStorageNode.h>

//------------------------------------------------------------------------------
class qCjyxSegmentationsNodeWriterOptionsWidgetPrivate
  : public qCjyxNodeWriterOptionsWidgetPrivate
{
public:
  void setupUi(QWidget* widget) override;
  QCheckBox* CropToMinimumExtentCheckbox;
};

//------------------------------------------------------------------------------
void qCjyxSegmentationsNodeWriterOptionsWidgetPrivate::setupUi(QWidget* widget)
{
  this->qCjyxNodeWriterOptionsWidgetPrivate::setupUi(widget);
  this->CropToMinimumExtentCheckbox = new QCheckBox(widget);
  this->CropToMinimumExtentCheckbox->setObjectName(QStringLiteral("CropToMinimumExtentCheckBox"));
  this->CropToMinimumExtentCheckbox->setText("Crop to minimum extent");
  this->CropToMinimumExtentCheckbox->setToolTip("If enabled then segmentation labelmap representation is"
    " cropped to the minimum necessary size. This saves storage space but changes voxel coordinate system"
    " (physical coordinate system is not affected).");
  horizontalLayout->addWidget(CropToMinimumExtentCheckbox);
  QObject::connect(this->CropToMinimumExtentCheckbox, SIGNAL(toggled(bool)),
    widget, SLOT(setCropToMinimumExtent(bool)));
}

//------------------------------------------------------------------------------
qCjyxSegmentationsNodeWriterOptionsWidget::qCjyxSegmentationsNodeWriterOptionsWidget(QWidget* parentWidget)
  : Superclass(new qCjyxSegmentationsNodeWriterOptionsWidgetPrivate, parentWidget)
{
  Q_D(qCjyxSegmentationsNodeWriterOptionsWidget);
  d->setupUi(this);
}

//------------------------------------------------------------------------------
qCjyxSegmentationsNodeWriterOptionsWidget::~qCjyxSegmentationsNodeWriterOptionsWidget() = default;

//------------------------------------------------------------------------------
void qCjyxSegmentationsNodeWriterOptionsWidget::setObject(vtkObject* object)
{
  Q_D(qCjyxSegmentationsNodeWriterOptionsWidget);
  vtkMRMLStorableNode* storableNode = vtkMRMLStorableNode::SafeDownCast(object);
  if (storableNode)
    {
    vtkMRMLSegmentationStorageNode* storageNode = vtkMRMLSegmentationStorageNode::SafeDownCast(
      storableNode->GetStorageNode());
    if (storageNode)
      {
      d->CropToMinimumExtentCheckbox->setChecked(storageNode->GetCropToMinimumExtent());
      }
    }
  Superclass::setObject(object);
}

//------------------------------------------------------------------------------
void qCjyxSegmentationsNodeWriterOptionsWidget::setCropToMinimumExtent(bool crop)
{
  Q_D(qCjyxSegmentationsNodeWriterOptionsWidget);
  d->Properties["cropToMinimumExtent"] = crop;
}
