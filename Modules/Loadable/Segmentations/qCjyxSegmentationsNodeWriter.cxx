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
#include <QDebug>
#include <QFileInfo>

// QtGUI includes
#include "qCjyxSegmentationsNodeWriter.h"
#include "qCjyxSegmentationsNodeWriterOptionsWidget.h"

// QTCore includes
#include "qCjyxCoreApplication.h"
#include "qCjyxCoreIOManager.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSceneViewNode.h>
#include <vtkMRMLSegmentationStorageNode.h>
#include <vtkMRMLStorableNode.h>
#include <vtkMRMLStorageNode.h>

// VTK includes
#include <vtkStdString.h>
#include <vtkStringArray.h>

//----------------------------------------------------------------------------
qCjyxSegmentationsNodeWriter::qCjyxSegmentationsNodeWriter(QObject* parentObject)
  : qCjyxNodeWriter("Segmentation", QString("SegmentationFile"), QStringList() << "vtkMRMLSegmentationNode", true, parentObject)
{
}

//----------------------------------------------------------------------------
qCjyxSegmentationsNodeWriter::~qCjyxSegmentationsNodeWriter() = default;

//----------------------------------------------------------------------------
bool qCjyxSegmentationsNodeWriter::write(const qCjyxIO::IOProperties& properties)
{
  Q_ASSERT(!properties["nodeID"].toString().isEmpty());

  vtkMRMLStorableNode* node = vtkMRMLStorableNode::SafeDownCast(
    this->getNodeByID(properties["nodeID"].toString().toUtf8().data()));
  if (!this->canWriteObject(node))
    {
    return false;
    }
  vtkMRMLSegmentationStorageNode* snode = vtkMRMLSegmentationStorageNode::SafeDownCast(
    qCjyxCoreIOManager::createAndAddDefaultStorageNode(node));
  if (snode == nullptr)
    {
    qDebug() << "No storage node for node" << properties["nodeID"].toString();
    return false;
    }
  snode->SetCropToMinimumExtent(properties["cropToMinimumExtent"].toBool());

  return Superclass::write(properties);
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxSegmentationsNodeWriter::options() const
{
  qCjyxSegmentationsNodeWriterOptionsWidget* options = new qCjyxSegmentationsNodeWriterOptionsWidget;
  options->setShowUseCompression(this->supportUseCompression());
  return options;
}
