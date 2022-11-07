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

// std includes
#include <vector>
#include <algorithm>

// Qt includes
#include <QDebug>
#include <QFileInfo>

// Cjyx includes
#include "qCjyxVolumesIOOptionsWidget.h"
#include "qCjyxVolumesReader.h"

// Logic includes
#include <vtkCjyxApplicationLogic.h>
#include "vtkCjyxVolumesLogic.h"

// MRML includes
#include <vtkMRMLDisplayNode.h>
#include <vtkMRMLLabelMapVolumeNode.h>
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLSelectionNode.h>

// VTK includes
#include <vtkSmartPointer.h>
#include <vtkStringArray.h>

// ITK includes
#include <itkArchetypeSeriesFileNames.h>

//-----------------------------------------------------------------------------
class qCjyxVolumesReaderPrivate
{
  public:
  vtkSmartPointer<vtkCjyxVolumesLogic> Logic;
};

//-----------------------------------------------------------------------------
qCjyxVolumesReader::qCjyxVolumesReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumesReaderPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxVolumesReader::qCjyxVolumesReader(vtkCjyxVolumesLogic* logic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumesReaderPrivate)
{
  this->setLogic(logic);
}

//-----------------------------------------------------------------------------
qCjyxVolumesReader::~qCjyxVolumesReader() = default;

//-----------------------------------------------------------------------------
void qCjyxVolumesReader::setLogic(vtkCjyxVolumesLogic* logic)
{
  Q_D(qCjyxVolumesReader);
  d->Logic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxVolumesLogic* qCjyxVolumesReader::logic()const
{
  Q_D(const qCjyxVolumesReader);
  return d->Logic.GetPointer();
}

//-----------------------------------------------------------------------------
QString qCjyxVolumesReader::description()const
{
  return "Volume";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxVolumesReader::fileType()const
{
  return QString("VolumeFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumesReader::extensions()const
{
  // pic files are bio-rad images (see itkBioRadImageIO)
  return QStringList()
    << "Volume (*.hdr *.nhdr *.nrrd *.mhd *.mha *.mnc *.nii *.nii.gz *.mgh *.mgz *.mgh.gz *.img *.img.gz *.pic)"
    << "Dicom (*.dcm *.ima)"
    << "Image (*.png *.tif *.tiff *.jpg *.jpeg)"
    << "All Files (*)";
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxVolumesReader::options()const
{
  // set the mrml scene on the options widget to allow selecting a color node
  qCjyxIOOptionsWidget* options = new qCjyxVolumesIOOptionsWidget;
  options->setMRMLScene(this->mrmlScene());
  return options;
}

//-----------------------------------------------------------------------------
bool qCjyxVolumesReader::load(const IOProperties& properties)
{
  Q_D(qCjyxVolumesReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  QString name = QFileInfo(fileName).baseName();
  if (properties.contains("name"))
    {
    name = properties["name"].toString();
    }
  int options = 0;
  if (properties.contains("labelmap"))
    {
    options |= properties["labelmap"].toBool() ? 0x1 : 0x0;
    }
  if (properties.contains("center"))
    {
    options |= properties["center"].toBool() ? 0x2 : 0x0;
    }
  if (properties.contains("singleFile"))
    {
    options |= properties["singleFile"].toBool() ? 0x4 : 0x0;
    }
  if (properties.contains("autoWindowLevel"))
    {
    options |= properties["autoWindowLevel"].toBool() ? 0x8: 0x0;
    }
  if (properties.contains("discardOrientation"))
    {
    options |= properties["discardOrientation"].toBool() ? 0x10 : 0x0;
    }
  bool propagateVolumeSelection = true;
  if (properties.contains("show"))
    {
    propagateVolumeSelection = properties["show"].toBool();
    }
  vtkSmartPointer<vtkStringArray> fileList;
  if (properties.contains("fileNames"))
    {
    fileList = vtkSmartPointer<vtkStringArray>::New();
    foreach(QString file, properties["fileNames"].toStringList())
      {
      fileList->InsertNextValue(file.toUtf8());
      }
    }
  Q_ASSERT(d->Logic);
  vtkMRMLVolumeNode* node = d->Logic->AddArchetypeVolume(
    fileName.toUtf8(),
    name.toUtf8(),
    options,
    fileList.GetPointer());
  if (node)
    {
    QString colorNodeID = properties.value("colorNodeID", QString()).toString();
    if (!colorNodeID.isEmpty())
      {
      vtkMRMLVolumeDisplayNode* displayNode = node->GetVolumeDisplayNode();
      if (displayNode)
        {
        displayNode->SetAndObserveColorNodeID(colorNodeID.toUtf8());
        }
      }
    if (propagateVolumeSelection)
      {
      vtkCjyxApplicationLogic* appLogic =
        d->Logic->GetApplicationLogic();
      vtkMRMLSelectionNode* selectionNode =
        appLogic ? appLogic->GetSelectionNode() : nullptr;
      if (selectionNode)
        {
        if (vtkMRMLLabelMapVolumeNode::SafeDownCast(node))
          {
          selectionNode->SetActiveLabelVolumeID(node->GetID());
          }
        else
          {
          selectionNode->SetActiveVolumeID(node->GetID());
          }
        if (appLogic)
          {
          appLogic->PropagateVolumeSelection(); // includes FitSliceToAll by default
          }
        }
      }
    this->setLoadedNodes(QStringList(QString(node->GetID())));
    }
  else
    {
    this->setLoadedNodes(QStringList());
    }
  return node != nullptr;
}

//-----------------------------------------------------------------------------
bool qCjyxVolumesReader::examineFileInfoList(QFileInfoList &fileInfoList, QFileInfo &archetypeFileInfo, qCjyxIO::IOProperties &ioProperties)const
{

  //
  // Check each file to see if it's recognzied as part of a series.  If so,
  // keep it as the archetype and remove all the others from the list
  //
  foreach(QFileInfo fileInfo, fileInfoList)
    {
    itk::ArchetypeSeriesFileNames::Pointer seriesNames = itk::ArchetypeSeriesFileNames::New();
    std::vector<std::string> candidateFiles;
    seriesNames->SetArchetype(fileInfo.absoluteFilePath().toStdString());
    candidateFiles = seriesNames->GetFileNames();
    if (candidateFiles.size() > 1)
      {
      archetypeFileInfo = fileInfo;
      QMutableListIterator<QFileInfo> fileInfoIterator(fileInfoList);
      while (fileInfoIterator.hasNext())
        {
        const QString &path = fileInfoIterator.next().absoluteFilePath();
        if (path == archetypeFileInfo.absoluteFilePath())
          {
          continue;
          }
        if (std::find(candidateFiles.begin(), candidateFiles.end(), path.toStdString()) != candidateFiles.end())
          {
          fileInfoIterator.remove();
          }
        }
      ioProperties["singleFile"] = false;
      return true;
      }
    }
  return false;
}
