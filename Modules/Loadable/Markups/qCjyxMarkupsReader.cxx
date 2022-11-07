/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QFileInfo>

// Cjyx includes
#include "qCjyxMarkupsReader.h"

// Logic includes
#include <vtkCjyxApplicationLogic.h>
#include "vtkCjyxMarkupsLogic.h"

// MRML includes
#include "vtkMRMLMessageCollection.h"

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxMarkupsReaderPrivate
{
  public:
  vtkSmartPointer<vtkCjyxMarkupsLogic> MarkupsLogic;
};

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Annotations
//-----------------------------------------------------------------------------
qCjyxMarkupsReader::qCjyxMarkupsReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxMarkupsReaderPrivate)
{
}

qCjyxMarkupsReader::qCjyxMarkupsReader(vtkCjyxMarkupsLogic* logic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxMarkupsReaderPrivate)
{
  this->setMarkupsLogic(logic);
}

//-----------------------------------------------------------------------------
qCjyxMarkupsReader::~qCjyxMarkupsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxMarkupsReader::setMarkupsLogic(vtkCjyxMarkupsLogic* logic)
{
  Q_D(qCjyxMarkupsReader);
  d->MarkupsLogic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxMarkupsLogic* qCjyxMarkupsReader::markupsLogic()const
{
  Q_D(const qCjyxMarkupsReader);
  return d->MarkupsLogic.GetPointer();
}

//-----------------------------------------------------------------------------
QString qCjyxMarkupsReader::description()const
{
  return "Markups";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxMarkupsReader::fileType()const
{
  return QString("MarkupsFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxMarkupsReader::extensions()const
{
  return QStringList()
    << "Markups (*.mrk.json)"
    << "Markups (*.json)"
    << "Markups Fiducials (*.fcsv)"
    << "Annotation Fiducial (*.acsv)";
}

//-----------------------------------------------------------------------------
bool qCjyxMarkupsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxMarkupsReader);

  // get the properties
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  QString name;
  if (properties.contains("name"))
    {
    name = properties["name"].toString();
    }

  if (d->MarkupsLogic.GetPointer() == nullptr)
    {
    return false;
    }

  // pass to logic to do the loading
  this->userMessages()->ClearMessages();
  char * nodeIDs = d->MarkupsLogic->LoadMarkups(fileName.toUtf8(), name.toUtf8(), this->userMessages());
  if (nodeIDs)
    {
    // returned a comma separated list of ids of the nodes that were loaded
    QStringList nodeIDList;
    char *ptr = strtok(nodeIDs, ",");

    while (ptr)
      {
      nodeIDList.append(ptr);
      ptr = strtok(nullptr, ",");
      }
    this->setLoadedNodes(nodeIDList);
    }
  else
    {
    this->setLoadedNodes(QStringList());
    return false;
    }

  return nodeIDs != nullptr;
}
