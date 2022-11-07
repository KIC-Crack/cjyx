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

// qCjyx includes
#include "qCjyxNodeWriterOptionsWidget.h"
#include "qCjyxNodeWriterOptionsWidget_p.h"

// MRML includes
#include <vtkMRMLStorableNode.h>
#include <vtkMRMLStorageNode.h>

//------------------------------------------------------------------------------
qCjyxNodeWriterOptionsWidgetPrivate::~qCjyxNodeWriterOptionsWidgetPrivate() = default;

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidgetPrivate::setupUi(QWidget* widget)
{
  this->Ui_qCjyxNodeWriterOptionsWidget::setupUi(widget);
  QObject::connect(this->UseCompressionCheckBox, SIGNAL(toggled(bool)),
                   widget, SLOT(setUseCompression(bool)));
  QObject::connect(this->CompressionParameterSelector, SIGNAL(currentIndexChanged(int)),
                   widget, SLOT(setCompressionParameter(int)));
}

//------------------------------------------------------------------------------
qCjyxNodeWriterOptionsWidget
::qCjyxNodeWriterOptionsWidget(qCjyxNodeWriterOptionsWidgetPrivate* pimpl,
                                   QWidget* parentWidget)
  : Superclass(pimpl, parentWidget)
{
}

//------------------------------------------------------------------------------
qCjyxNodeWriterOptionsWidget::qCjyxNodeWriterOptionsWidget(QWidget* parentWidget)
  : Superclass(new qCjyxNodeWriterOptionsWidgetPrivate, parentWidget)
{
  Q_D(qCjyxNodeWriterOptionsWidget);
  d->setupUi(this);
}

//------------------------------------------------------------------------------
qCjyxNodeWriterOptionsWidget::~qCjyxNodeWriterOptionsWidget() = default;

//------------------------------------------------------------------------------
bool qCjyxNodeWriterOptionsWidget::isValid()const
{
  Q_D(const qCjyxNodeWriterOptionsWidget);
  return d->Properties.contains("nodeID") &&
         d->Properties.contains("fileName");
}

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidget::setObject(vtkObject* object)
{
  Q_D(qCjyxNodeWriterOptionsWidget);
  vtkMRMLStorableNode* storableNode = vtkMRMLStorableNode::SafeDownCast(object);
  if (storableNode != nullptr)
    {
    d->Properties["nodeID"] = storableNode->GetID();
    }
  else
    {
    d->Properties.remove("nodeID");
    }
  vtkMRMLStorageNode* storageNode = storableNode->GetStorageNode();
  d->UseCompressionCheckBox->setEnabled(storageNode != nullptr);
  if (storageNode)
    {
    d->UseCompressionCheckBox->setChecked(
      (storageNode->GetUseCompression() == 1));

    std::vector<vtkMRMLStorageNode::CompressionPreset> presets = storageNode->GetCompressionPresets();
    d->CompressionParameterSelector->clear();
    std::vector<vtkMRMLStorageNode::CompressionPreset>::iterator presetIt;
    for (presetIt = presets.begin(); presetIt != presets.end(); ++presetIt)
      {
      QString name = QString::fromStdString(presetIt->DisplayName);
      QString parameter = QString::fromStdString(presetIt->CompressionParameter);
      d->CompressionParameterSelector->addItem(name, parameter);
      }
    this->setCompressionParameter(QString::fromStdString(storageNode->GetCompressionParameter()));
    }
  d->CompressionParameterSelector->setVisible(d->CompressionParameterSelector->count() > 0);
  d->CompressionParameterSelector->setEnabled(storageNode != nullptr && d->UseCompressionCheckBox->isChecked());

  this->updateValid();
}

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidget::setUseCompression(bool use)
{
  Q_D(qCjyxNodeWriterOptionsWidget);
  d->Properties["useCompression"] = (use ? 1 : 0);
  d->CompressionParameterSelector->setEnabled(d->UseCompressionCheckBox->isChecked());
}

//------------------------------------------------------------------------------
bool qCjyxNodeWriterOptionsWidget::showUseCompression()const
{
  Q_D(const qCjyxNodeWriterOptionsWidget);
  return d->UseCompressionCheckBox->isVisibleTo(
    const_cast<qCjyxNodeWriterOptionsWidget*>(this));
}

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidget::setShowUseCompression(bool show)
{
  Q_D(qCjyxNodeWriterOptionsWidget);
  d->UseCompressionCheckBox->setVisible(show);
  d->CompressionParameterSelector->setVisible(show && d->CompressionParameterSelector->count() > 0);
}

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidget::setCompressionParameter(int index)
{
  Q_D(qCjyxNodeWriterOptionsWidget);

  QString parameter = d->CompressionParameterSelector->itemData(index).toString();
  d->Properties["compressionParameter"] = parameter;
}

//------------------------------------------------------------------------------
void qCjyxNodeWriterOptionsWidget::setCompressionParameter(QString parameter)
{
  Q_D(qCjyxNodeWriterOptionsWidget);

  int index = d->CompressionParameterSelector->findData(parameter);
  d->CompressionParameterSelector->setCurrentIndex(index);
  d->Properties["compressionParameter"] = parameter;
}
