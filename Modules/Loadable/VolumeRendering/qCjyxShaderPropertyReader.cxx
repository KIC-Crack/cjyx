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

  This file was originally developed by Simon Drouin, Brigham and Women's
  Hospital, Boston, MA.

==============================================================================*/

// Qt includes
#include <QFileInfo>

// Cjyx includes
#include "qCjyxShaderPropertyReader.h"

// Logic includes
#include "vtkCjyxVolumeRenderingLogic.h"

// MRMLLogic includes
#include <vtkMRMLApplicationLogic.h>

// MRML includes
#include <vtkMRMLScalarVolumeNode.h>
#include <vtkMRMLSelectionNode.h>
#include "vtkMRMLShaderPropertyNode.h"

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxShaderPropertyReaderPrivate
{
  public:
  vtkSmartPointer<vtkCjyxVolumeRenderingLogic> VolumeRenderingLogic;
};

//-----------------------------------------------------------------------------
qCjyxShaderPropertyReader::qCjyxShaderPropertyReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxShaderPropertyReaderPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxShaderPropertyReader::qCjyxShaderPropertyReader(vtkCjyxVolumeRenderingLogic* logic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxShaderPropertyReaderPrivate)
{
  this->setVolumeRenderingLogic(logic);
}

//-----------------------------------------------------------------------------
qCjyxShaderPropertyReader::~qCjyxShaderPropertyReader() = default;

//-----------------------------------------------------------------------------
void qCjyxShaderPropertyReader::setVolumeRenderingLogic(vtkCjyxVolumeRenderingLogic* logic)
{
  Q_D(qCjyxShaderPropertyReader);
  d->VolumeRenderingLogic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxVolumeRenderingLogic* qCjyxShaderPropertyReader::volumeRenderingLogic()const
{
  Q_D(const qCjyxShaderPropertyReader);
  return d->VolumeRenderingLogic.GetPointer();
}

//-----------------------------------------------------------------------------
QString qCjyxShaderPropertyReader::description()const
{
  return "GPU Shader Property";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxShaderPropertyReader::fileType()const
{
  return QString("ShaderPropertyFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxShaderPropertyReader::extensions()const
{
  return QStringList()
    << "Shader Property (*.sp)";
}

//-----------------------------------------------------------------------------
bool qCjyxShaderPropertyReader::load(const IOProperties& properties)
{
  Q_D(qCjyxShaderPropertyReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();
  // Name is ignored
  //QString name = QFileInfo(fileName).baseName();
  //if (properties.contains("name"))
  //  {
  //  name = properties["name"].toString();
  //  }
  if (d->VolumeRenderingLogic.GetPointer() == nullptr)
    {
    return false;
    }
  vtkMRMLShaderPropertyNode* node =
    d->VolumeRenderingLogic->AddShaderPropertyFromFile(fileName.toUtf8());
  QStringList loadedNodes;
  if (node)
    {
    loadedNodes << QString(node->GetID());
    }
  this->setLoadedNodes(loadedNodes);
  return node != nullptr;
}
