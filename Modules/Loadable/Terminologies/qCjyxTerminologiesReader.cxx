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

// Terminologies includes
#include "qCjyxTerminologiesReader.h"

// Logic includes
#include "vtkCjyxTerminologiesModuleLogic.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxTerminologiesReaderPrivate
{
public:
  vtkSmartPointer<vtkCjyxTerminologiesModuleLogic> TerminologiesLogic;
};

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Terminologies
qCjyxTerminologiesReader::qCjyxTerminologiesReader(vtkCjyxTerminologiesModuleLogic* terminologiesLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxTerminologiesReaderPrivate)
{
  this->setTerminologiesLogic(terminologiesLogic);
}

//-----------------------------------------------------------------------------
qCjyxTerminologiesReader::~qCjyxTerminologiesReader() = default;

//-----------------------------------------------------------------------------
void qCjyxTerminologiesReader::setTerminologiesLogic(vtkCjyxTerminologiesModuleLogic* newTerminologiesLogic)
{
  Q_D(qCjyxTerminologiesReader);
  d->TerminologiesLogic = newTerminologiesLogic;
}

//-----------------------------------------------------------------------------
vtkCjyxTerminologiesModuleLogic* qCjyxTerminologiesReader::terminologiesLogic()const
{
  Q_D(const qCjyxTerminologiesReader);
  return d->TerminologiesLogic;
}

//-----------------------------------------------------------------------------
QString qCjyxTerminologiesReader::description()const
{
  return "Terminology";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxTerminologiesReader::fileType()const
{
  return QString("TerminologyFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxTerminologiesReader::extensions()const
{
  return QStringList() << "Terminology (*.term.json)" << "Terminology (*.json)";
}

//-----------------------------------------------------------------------------
bool qCjyxTerminologiesReader::load(const IOProperties& properties)
{
  Q_D(qCjyxTerminologiesReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  this->setLoadedNodes(QStringList());
  if (d->TerminologiesLogic.GetPointer() == nullptr)
    {
    return false;
    }

  bool contextLoaded = d->TerminologiesLogic->LoadContextFromFile(fileName.toUtf8().constData());
  if (!contextLoaded)
    {
    this->setLoadedNodes(QStringList());
    return false;
    }

  return true;
}
