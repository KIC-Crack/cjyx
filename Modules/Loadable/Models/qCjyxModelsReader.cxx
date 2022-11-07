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
#include <QDebug>
#include <QFileInfo>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxLayoutManager.h"
#include "qCjyxModelsIOOptionsWidget.h"
#include "qCjyxModelsReader.h"

// Logic includes
#include "vtkCjyxModelsLogic.h"

// MRML includes
#include "vtkMRMLDisplayNode.h"
#include "vtkMRMLMessageCollection.h"
#include "vtkMRMLModelNode.h"
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkSmartPointer.h>

//-----------------------------------------------------------------------------
class qCjyxModelsReaderPrivate
{
public:
  vtkSmartPointer<vtkCjyxModelsLogic> ModelsLogic;
};

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Models
qCjyxModelsReader::qCjyxModelsReader(vtkCjyxModelsLogic* _modelsLogic, QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxModelsReaderPrivate)
{
  this->setModelsLogic(_modelsLogic);
}

//-----------------------------------------------------------------------------
qCjyxModelsReader::~qCjyxModelsReader() = default;

//-----------------------------------------------------------------------------
void qCjyxModelsReader::setModelsLogic(vtkCjyxModelsLogic* newModelsLogic)
{
  Q_D(qCjyxModelsReader);
  d->ModelsLogic = newModelsLogic;
}

//-----------------------------------------------------------------------------
vtkCjyxModelsLogic* qCjyxModelsReader::modelsLogic()const
{
  Q_D(const qCjyxModelsReader);
  return d->ModelsLogic;
}

//-----------------------------------------------------------------------------
QString qCjyxModelsReader::description()const
{
  return "Model";
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxModelsReader::fileType()const
{
  return QString("ModelFile");
}

//-----------------------------------------------------------------------------
QStringList qCjyxModelsReader::extensions()const
{
  return QStringList()
    << "Model (*.vtk *.vtp  *.vtu *.g *.byu *.stl *.ply *.orig"
         " *.inflated *.sphere *.white *.smoothwm *.pial *.obj *.ucd)";
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxModelsReader::options()const
{
  qCjyxIOOptionsWidget* options = new qCjyxModelsIOOptionsWidget;
  options->setMRMLScene(this->mrmlScene());
  return options;
}

//-----------------------------------------------------------------------------
bool qCjyxModelsReader::load(const IOProperties& properties)
{
  Q_D(qCjyxModelsReader);
  Q_ASSERT(properties.contains("fileName"));
  QString fileName = properties["fileName"].toString();

  this->setLoadedNodes(QStringList());
  if (!d->ModelsLogic)
    {
    qCritical() << Q_FUNC_INFO << (" failed: Models logic is invalid.");
    return false;
    }
  int coordinateSystem = vtkMRMLStorageNode::CoordinateSystemLPS; // default
  if (properties.contains("coordinateSystem"))
    {
    coordinateSystem = properties["coordinateSystem"].toInt();
    }
  this->userMessages()->ClearMessages();
  vtkMRMLModelNode* node = d->ModelsLogic->AddModel(
    fileName.toUtf8(), coordinateSystem, this->userMessages());
  if (!node)
    {
    // errors are already logged and userMessages contain details that can be displayed to users
    return false;
    }
  this->setLoadedNodes( QStringList(QString(node->GetID())) );
  if (properties.contains("name"))
    {
    std::string uname = this->mrmlScene()->GetUniqueNameByString(
      properties["name"].toString().toUtf8());
    node->SetName(uname.c_str());
    }

  // If no other nodes are displayed then reset the field of view
  bool otherNodesAreAlreadyVisible = false;
  vtkSmartPointer<vtkCollection> displayNodes = vtkSmartPointer<vtkCollection>::Take(
    this->mrmlScene()->GetNodesByClass("vtkMRMLDisplayNode"));
  for(int displayNodeIndex = 0; displayNodeIndex < displayNodes->GetNumberOfItems(); ++displayNodeIndex)
    {
    vtkMRMLDisplayNode* displayNode = vtkMRMLDisplayNode::SafeDownCast(
      displayNodes->GetItemAsObject(displayNodeIndex));
    if (displayNode->GetDisplayableNode()
      && displayNode->GetVisibility()
      && displayNode->GetDisplayableNode() != node)
      {
      otherNodesAreAlreadyVisible = true;
      break;
      }
    }
  if (!otherNodesAreAlreadyVisible)
    {
    qCjyxApplication* app = qCjyxApplication::application();
    if (app && app->layoutManager())
      {
      app->layoutManager()->resetThreeDViews();
      }
    }

  return true;
}
