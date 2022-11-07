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
#include "qCjyxColorsReader.h"

// Logic includes
#include "vtkCjyxColorLogic.h"

// MRML includes
#include <vtkMRMLColorNode.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxColorsReaderPrivate
{
public:
  vtkSmartPointer<vtkCjyxColorLogic> ColorLogic;
};

//-----------------------------------------------------------------------------
qCjyxColorsReader::qCjyxColorsReader(
  vtkCjyxColorLogic* _colorLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxColorsReaderPrivate)
{
  this->setColorLogic(_colorLogic);
}

//-----------------------------------------------------------------------------
qCjyxColorsReader::~qCjyxColorsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxColorsReader::setColorLogic(vtkCjyxColorLogic* newColorLogic)
{
  Q_D(qCjyxColorsReader);
  d->ColorLogic = newColorLogic;
}

//-----------------------------------------------------------------------------
vtkCjyxColorLogic* qCjyxColorsReader::colorLogic()const
{
  Q_D(const qCjyxColorsReader);
  return d->ColorLogic;
}

//-----------------------------------------------------------------------------
QString qCjyxColorsReader::description()const
{
  return "Color";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxColorsReader::fileType()const
{
  return QString("ColorTableFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxColorsReader::extensions()const
{
  return QStringList() << "Color (*.txt *.ctbl *.cxml)";
}

//-----------------------------------------------------------------------------
bool qCjyxColorsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxColorsReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  if (d->ColorLogic.GetPointer() == nullptr)
    {
    return false;
    }

  vtkMRMLColorNode* node = d->ColorLogic->LoadColorFile(fileName.toUtf8());
  QStringList loadedNodes;
  if (node)
    {
    loadedNodes << QString(node->GetID());
    }
  this->setLoadedNodes(loadedNodes);
  return node != nullptr;
}

// TODO: add the save() method. Use vtkCjyxColorLogic::SaveColor()
