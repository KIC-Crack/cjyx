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

// Qt includes
#include <QFileInfo>

// Cjyx includes
#include "qCjyxVolumeRenderingReader.h"

// Logic includes
#include "vtkCjyxVolumeRenderingLogic.h"

// MRMLLogic includes
#include <vtkMRMLApplicationLogic.h>

// MRML includes
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLSelectionNode.h>
#include "vtkMRMLVolumePropertyNode.h"

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxVolumeRenderingReaderPrivate
{
  public:
  vtkSmartPointer<vtkCjyxVolumeRenderingLogic> VolumeRenderingLogic;
};

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingReader::qCjyxVolumeRenderingReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumeRenderingReaderPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingReader::qCjyxVolumeRenderingReader(vtkCjyxVolumeRenderingLogic* logic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxVolumeRenderingReaderPrivate)
{
  this->setVolumeRenderingLogic(logic);
}

//-----------------------------------------------------------------------------
qCjyxVolumeRenderingReader::~qCjyxVolumeRenderingReader() = default;

//-----------------------------------------------------------------------------
void qCjyxVolumeRenderingReader::setVolumeRenderingLogic(vtkCjyxVolumeRenderingLogic* logic)
{
  Q_D(qCjyxVolumeRenderingReader);
  d->VolumeRenderingLogic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxVolumeRenderingLogic* qCjyxVolumeRenderingReader::volumeRenderingLogic()const
{
  Q_D(const qCjyxVolumeRenderingReader);
  return d->VolumeRenderingLogic.GetPointer();
}

//-----------------------------------------------------------------------------
QString qCjyxVolumeRenderingReader::description()const
{
  return "Transfer Function";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxVolumeRenderingReader::fileType()const
{
  return QString("TransferFunctionFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxVolumeRenderingReader::extensions()const
{
  // pic files are bio-rad images (see itkBioRadImageIO)
  return QStringList()
    << "Transfer Function (*.vp)";
}

//-----------------------------------------------------------------------------
bool qCjyxVolumeRenderingReader::load(const IOProperties& properties)
{
  Q_D(qCjyxVolumeRenderingReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();
  if (d->VolumeRenderingLogic.GetPointer() == nullptr)
    {
    return false;
    }
  vtkMRMLVolumePropertyNode* node =
    d->VolumeRenderingLogic->AddVolumePropertyFromFile(fileName.toUtf8());
  QStringList loadedNodes;
  if (node)
    {
    loadedNodes << QString(node->GetID());
    }
  this->setLoadedNodes(loadedNodes);
  return node != nullptr;
}
