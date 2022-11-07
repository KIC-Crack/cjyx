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

// Cjyx includes
#include "qCjyxTransformsReader.h"

// Logic includes
#include "vtkCjyxTransformLogic.h"

// MRML includes
#include <vtkMRMLTransformNode.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxTransformsReaderPrivate
{
public:
  vtkSmartPointer<vtkCjyxTransformLogic> TransformLogic;
};

//-----------------------------------------------------------------------------
qCjyxTransformsReader::qCjyxTransformsReader(
  vtkCjyxTransformLogic* _transformLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxTransformsReaderPrivate)
{
  this->setTransformLogic(_transformLogic);
}


//-----------------------------------------------------------------------------
qCjyxTransformsReader::~qCjyxTransformsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxTransformsReader::setTransformLogic(vtkCjyxTransformLogic* newTransformLogic)
{
  Q_D(qCjyxTransformsReader);
  d->TransformLogic = newTransformLogic;
}

//-----------------------------------------------------------------------------
vtkCjyxTransformLogic* qCjyxTransformsReader::transformLogic()const
{
  Q_D(const qCjyxTransformsReader);
  return d->TransformLogic;
}

//-----------------------------------------------------------------------------
QString qCjyxTransformsReader::description()const
{
  return "Transform";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxTransformsReader::fileType()const
{
  return QString("TransformFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxTransformsReader::extensions()const
{
  return QStringList() << "Transform (*.h5 *.tfm *.mat *.nrrd *.nhdr *.mha *.mhd *.nii *.nii.gz *.txt *.hdf5 *.he5)";
}

//-----------------------------------------------------------------------------
bool qCjyxTransformsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxTransformsReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  if (d->TransformLogic.GetPointer() == nullptr)
    {
    return false;
    }
  vtkMRMLTransformNode* node = d->TransformLogic->AddTransform(
    fileName.toUtf8(), this->mrmlScene());
  if (node)
    {
    this->setLoadedNodes(QStringList(QString(node->GetID())));
    }
  else
    {
    this->setLoadedNodes(QStringList());
    }
  return node != nullptr;
}

// TODO: add the save() method. Use vtkCjyxTransformLogic::SaveTransform()
