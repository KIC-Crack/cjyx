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
#include "qCjyxAnnotationsIOOptionsWidget.h"
#include "qCjyxAnnotationsReader.h"

// Logic includes
#include <vtkCjyxApplicationLogic.h>
#include "vtkCjyxAnnotationModuleLogic.h"

// MRML includes
#include <vtkMRMLNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxAnnotationsReaderPrivate
{
  public:
  vtkSmartPointer<vtkCjyxAnnotationModuleLogic> AnnotationLogic;
};


//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Annotations
//-----------------------------------------------------------------------------
qCjyxAnnotationsReader::qCjyxAnnotationsReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxAnnotationsReaderPrivate)
{
}

qCjyxAnnotationsReader::qCjyxAnnotationsReader(vtkCjyxAnnotationModuleLogic* logic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxAnnotationsReaderPrivate)
{
  this->setAnnotationLogic(logic);
}

//-----------------------------------------------------------------------------
qCjyxAnnotationsReader::~qCjyxAnnotationsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxAnnotationsReader::setAnnotationLogic(vtkCjyxAnnotationModuleLogic* logic)
{
  Q_D(qCjyxAnnotationsReader);
  d->AnnotationLogic = logic;
}

//-----------------------------------------------------------------------------
vtkCjyxAnnotationModuleLogic* qCjyxAnnotationsReader::annotationLogic()const
{
  Q_D(const qCjyxAnnotationsReader);
  return d->AnnotationLogic.GetPointer();
}

//-----------------------------------------------------------------------------
QString qCjyxAnnotationsReader::description()const
{
  return "Annotation";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxAnnotationsReader::fileType()const
{
  return QString("AnnotationFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxAnnotationsReader::extensions()const
{
  return QStringList()
    << "Annotations (*.acsv)";
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxAnnotationsReader::options()const
{
  return new qCjyxAnnotationsIOOptionsWidget;
}

//-----------------------------------------------------------------------------
bool qCjyxAnnotationsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxAnnotationsReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  QString name = QFileInfo(fileName).baseName();
  if (properties.contains("name"))
    {
    name = properties["name"].toString();
    }

  if (d->AnnotationLogic.GetPointer() == nullptr)
    {
    return false;
    }

  // file type
  int fileType = vtkCjyxAnnotationModuleLogic::None;
  if (properties.contains("fiducial") && properties["fiducial"].toBool() == true)
    {
    fileType = vtkCjyxAnnotationModuleLogic::Fiducial;
    }
  else if (properties.contains("ruler") && properties["ruler"].toBool() == true)
    {
    fileType = vtkCjyxAnnotationModuleLogic::Ruler;
    }
  else if (properties.contains("roi") && properties["roi"].toBool() == true)
    {
    fileType = vtkCjyxAnnotationModuleLogic::ROI;
    }

  char * nodeID = d->AnnotationLogic->LoadAnnotation(
    fileName.toUtf8(), name.toUtf8(), fileType);
  if (!nodeID)
    {
    this->setLoadedNodes(QStringList());
    return false;
    }
  this->setLoadedNodes( QStringList(QString(nodeID)) );
  if (properties.contains("name"))
    {
    std::string uname = this->mrmlScene()->GetUniqueNameByString(
      properties["name"].toString().toUtf8());
    this->mrmlScene()->GetNodeByID(nodeID)->SetName(uname.c_str());
    }
  return true;
}
