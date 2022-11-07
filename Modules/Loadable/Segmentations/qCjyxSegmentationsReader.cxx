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

  This file was originally developed by Adam Rankin, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Segmentations includes
#include "qCjyxSegmentationsReader.h"
#include "qCjyxSegmentationsIOOptionsWidget.h"

// Qt includes
#include <QFileInfo>

// Logic includes
#include "vtkCjyxSegmentationsModuleLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLSegmentationNode.h>
#include <vtkMRMLSegmentationDisplayNode.h>
#include <vtkMRMLStorageNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelStorageNode.h>

// VTK includes
#include <vtkNew.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxSegmentationsReaderPrivate
{
public:
  vtkSmartPointer<vtkCjyxSegmentationsModuleLogic> SegmentationsLogic;
};

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Segmentations
qCjyxSegmentationsReader::qCjyxSegmentationsReader(vtkCjyxSegmentationsModuleLogic* segmentationsLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSegmentationsReaderPrivate)
{
  this->setSegmentationsLogic(segmentationsLogic);
}

//-----------------------------------------------------------------------------
qCjyxSegmentationsReader::~qCjyxSegmentationsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxSegmentationsReader::setSegmentationsLogic(vtkCjyxSegmentationsModuleLogic* newSegmentationsLogic)
{
  Q_D(qCjyxSegmentationsReader);
  d->SegmentationsLogic = newSegmentationsLogic;
}

//-----------------------------------------------------------------------------
vtkCjyxSegmentationsModuleLogic* qCjyxSegmentationsReader::segmentationsLogic()const
{
  Q_D(const qCjyxSegmentationsReader);
  return d->SegmentationsLogic;
}

//-----------------------------------------------------------------------------
QString qCjyxSegmentationsReader::description()const
{
  return "Segmentation";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxSegmentationsReader::fileType()const
{
  return QString("SegmentationFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxSegmentationsReader::extensions()const
{
  return QStringList()
    << "Segmentation (*.seg.nrrd)" << "Segmentation (*.seg.nhdr)" << "Segmentation (*.seg.vtm)"
    << "Segmentation (*.nrrd)" << "Segmentation (*.nhdr)" << "Segmentation (*.vtm)"
    << "Segmentation (*.nii.gz)" << "Segmentation (*.nii)" << "Segmentation (*.hdr)"
    << "Segmentation (*.stl)" << "Segmentation (*.obj)";
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxSegmentationsReader::options()const
{
  qCjyxIOOptionsWidget* options = new qCjyxSegmentationsIOOptionsWidget;
  options->setMRMLScene(this->mrmlScene());
  return options;
}

//-----------------------------------------------------------------------------
bool qCjyxSegmentationsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxSegmentationsReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  this->setLoadedNodes(QStringList());
  if (d->SegmentationsLogic.GetPointer() == nullptr)
    {
    return false;
    }

  QString name;
  if (properties.contains("name"))
    {
    name = properties["name"].toString();
    }

  std::string extension = vtkMRMLStorageNode::GetLowercaseExtensionFromFileName(fileName.toStdString());

  if (extension.compare(".stl") == 0 || extension.compare(".obj") == 0)
    {
    vtkSmartPointer<vtkPolyData> closedSurfaceRepresentation;
    vtkNew<vtkMRMLModelStorageNode> modelStorageNode;
    modelStorageNode->SetFileName(fileName.toStdString().c_str());
    vtkNew<vtkMRMLModelNode> modelNode;
    if (!modelStorageNode->ReadData(modelNode))
    {
      return false;
    }
    closedSurfaceRepresentation = modelNode->GetPolyData();

    // Remove all arrays, because they could slow down all further processing
      // and consume significant amount of memory.
    if (closedSurfaceRepresentation != nullptr && closedSurfaceRepresentation->GetPointData() != nullptr)
    {
      vtkPointData* pointData = closedSurfaceRepresentation->GetPointData();
      while (pointData->GetNumberOfArrays() > 0)
      {
        pointData->RemoveArray(0);
      }
    }

    if (closedSurfaceRepresentation == nullptr)
      {
      return false;
      }

    if (name.isEmpty())
      {
      name = QFileInfo(fileName).completeBaseName();
      }

    vtkNew<vtkSegment> segment;
    segment->SetName(name.toUtf8().constData());
    segment->AddRepresentation(vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName(), closedSurfaceRepresentation);

    vtkMRMLSegmentationNode* segmentationNode = vtkMRMLSegmentationNode::SafeDownCast(
      this->mrmlScene()->AddNewNodeByClass("vtkMRMLSegmentationNode", this->mrmlScene()->GetUniqueNameByString(name.toUtf8())));
    segmentationNode->SetMasterRepresentationToClosedSurface();
    segmentationNode->CreateDefaultDisplayNodes();
    vtkMRMLSegmentationDisplayNode* displayNode = vtkMRMLSegmentationDisplayNode::SafeDownCast(segmentationNode->GetDisplayNode());
    if (displayNode)
      {
      // Show slice intersections using closed surface representation (don't create binary labelmap for display)
      displayNode->SetPreferredDisplayRepresentationName2D(vtkSegmentationConverter::GetSegmentationClosedSurfaceRepresentationName());
      }

    segmentationNode->GetSegmentation()->AddSegment(segment.GetPointer());

    this->setLoadedNodes(QStringList(QString(segmentationNode->GetID())));
    }
  else
    {
    // Non-STL file, load using segmentation storage node
    bool autoOpacities = true;
    if (properties.contains("autoOpacities"))
      {
      autoOpacities = properties["autoOpacities"].toBool();
      }

    vtkMRMLColorTableNode* colorTableNode = nullptr;
    if (properties.contains("colorNodeID"))
      {
      std::string nodeID = properties["colorNodeID"].toString().toStdString();
      colorTableNode = vtkMRMLColorTableNode::SafeDownCast(this->mrmlScene()->GetNodeByID(nodeID));
      }

    vtkMRMLSegmentationNode* node = d->SegmentationsLogic->LoadSegmentationFromFile(
      fileName.toUtf8().constData(), autoOpacities, name.toUtf8(), colorTableNode);
    if (!node)
      {
      this->setLoadedNodes(QStringList());
      return false;
      }

    this->setLoadedNodes( QStringList(QString(node->GetID())) );
    }

  return true;
}
