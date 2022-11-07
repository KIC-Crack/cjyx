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

// QtGUI includes
#include "qCjyxNodeWriter.h"
#include "qCjyxNodeWriterOptionsWidget.h"

// QTCore includes
#include "qCjyxCoreApplication.h"
#include "qCjyxCoreIOManager.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLStorableNode.h>
#include <vtkMRMLStorageNode.h>
#include <vtkMRMLSceneViewNode.h>
#include <vtkMRMLMessageCollection.h>

// VTK includes
#include <vtkStdString.h>
#include <vtkStringArray.h>

//-----------------------------------------------------------------------------
class qCjyxNodeWriterPrivate
{
public:
  QString Description;
  qCjyxIO::IOFileType FileType;
  QStringList NodeClassNames;
  bool SupportUseCompression;
};

//----------------------------------------------------------------------------
qCjyxNodeWriter::qCjyxNodeWriter(const QString& description,
                                     const qCjyxIO::IOFileType& fileIO,
                                     const QStringList& nodeClassNames,
                                     bool supportUseCompression,
                                     QObject* parentObject)
  : Superclass(parentObject)
  , d_ptr(new qCjyxNodeWriterPrivate)
{
  Q_D(qCjyxNodeWriter);
  d->Description = description;
  d->FileType = fileIO;
  d->SupportUseCompression = supportUseCompression;
  this->setNodeClassNames(nodeClassNames);
}

//----------------------------------------------------------------------------
qCjyxNodeWriter::~qCjyxNodeWriter() = default;

//----------------------------------------------------------------------------
QString qCjyxNodeWriter::description()const
{
  Q_D(const qCjyxNodeWriter);
  return d->Description;
}

//----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxNodeWriter::fileType()const
{
  Q_D(const qCjyxNodeWriter);
  return d->FileType;
}

//----------------------------------------------------------------------------
bool qCjyxNodeWriter::canWriteObject(vtkObject* object)const
{
  Q_D(const qCjyxNodeWriter);
  vtkMRMLStorableNode* node = vtkMRMLStorableNode::SafeDownCast(object);
  if (node)
    {
    foreach(QString className, d->NodeClassNames)
      {
      if (node->IsA(className.toUtf8()))
        {
        return true;
        }
      }
    }
  return false;
}

//----------------------------------------------------------------------------
QStringList qCjyxNodeWriter::extensions(vtkObject* object)const
{
  QStringList supportedExtensions;
  vtkMRMLStorageNode* snode =
      qCjyxCoreIOManager::createAndAddDefaultStorageNode(vtkMRMLStorableNode::SafeDownCast(object));
  if (snode)
    {
    const int formatCount = snode->GetSupportedWriteFileTypes()->GetNumberOfValues();
    for (int formatIt = 0; formatIt < formatCount; ++formatIt)
      {
      vtkStdString format =
        snode->GetSupportedWriteFileTypes()->GetValue(formatIt);
      supportedExtensions << QString::fromStdString(format);
      }
    }
  return supportedExtensions;
}

//----------------------------------------------------------------------------
bool qCjyxNodeWriter::write(const qCjyxIO::IOProperties& properties)
{
  this->setWrittenNodes(QStringList());

  Q_ASSERT(!properties["nodeID"].toString().isEmpty());

  vtkMRMLStorableNode* node = vtkMRMLStorableNode::SafeDownCast(
    this->getNodeByID(properties["nodeID"].toString().toUtf8().data()));
  if (!this->canWriteObject(node))
    {
    return false;
    }
  vtkMRMLStorageNode* snode = qCjyxCoreIOManager::createAndAddDefaultStorageNode(node);
  if (snode == nullptr)
    {
    qDebug() << "No storage node for node" << properties["nodeID"].toString();
    return false;
    }

  Q_ASSERT(!properties["fileName"].toString().isEmpty());
  QString fileName = properties["fileName"].toString();
  snode->SetFileName(fileName.toUtf8());

  qCjyxCoreIOManager* coreIOManager =
    qCjyxCoreApplication::application()->coreIOManager();

  QString fileFormat =
    properties.value("fileFormat", coreIOManager->completeCjyxWritableFileNameSuffix(node)).toString();
  snode->SetWriteFileFormat(fileFormat.toUtf8());
  snode->SetURI(nullptr);
  if (properties.contains("useCompression"))
    {
    snode->SetUseCompression(properties["useCompression"].toInt());
    if (properties.contains("compressionParameter"))
      {
      snode->SetCompressionParameter(properties["compressionParameter"].toString().toStdString());
      }
    }
  bool res = snode->WriteData(node);

  if (res)
    {
    this->setWrittenNodes(QStringList() << node->GetID());
    }

  this->userMessages()->AddMessages(snode->GetUserMessages());

  return res;
}

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxNodeWriter::getNodeByID(const char *id)const
{
  vtkMRMLNode *node = this->mrmlScene()->GetNodeByID(id);
  if (node == nullptr)
    {
    // search in SceneView nodes
    std::string sID(id);
    std::vector<vtkMRMLNode *> nodes;
    this->mrmlScene()->GetNodesByClass("vtkMRMLSceneViewNode", nodes);
    std::vector<vtkMRMLNode *>::iterator it;

    for (it = nodes.begin(); it != nodes.end(); it++)
      {
      vtkMRMLSceneViewNode *svNode = vtkMRMLSceneViewNode::SafeDownCast(*it);
      // skip "Master Scene View" since it contains the same nodes as the scene
      if (svNode->GetName() && std::string("Master Scene View") == std::string(svNode->GetName()))
        {
        continue;
        }
      std::vector<vtkMRMLNode *> snodes;
      svNode->GetNodesByClass("vtkMRMLStorableNode", snodes);
      std::vector<vtkMRMLNode *>::iterator sit;
      for (sit = snodes.begin(); sit != snodes.end(); sit++)
        {
        vtkMRMLNode* snode = (*sit);
        if (std::string(snode->GetID()) == sID)
          {
          return snode;
          }
        }
      }
    }
  return node;
}

//----------------------------------------------------------------------------
void qCjyxNodeWriter::setNodeClassNames(const QStringList& nodeClassNames)
{
  Q_D(qCjyxNodeWriter);
  d->NodeClassNames = nodeClassNames;
}

//----------------------------------------------------------------------------
QStringList qCjyxNodeWriter::nodeClassNames()const
{
  Q_D(const qCjyxNodeWriter);
  return d->NodeClassNames;
}

//----------------------------------------------------------------------------
void qCjyxNodeWriter::setSupportUseCompression(bool support)
{
  Q_D(qCjyxNodeWriter);
  d->SupportUseCompression = support;
}

//----------------------------------------------------------------------------
bool qCjyxNodeWriter::supportUseCompression()const
{
  Q_D(const qCjyxNodeWriter);
  return d->SupportUseCompression;
}
//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxNodeWriter::options()const
{
  Q_D(const qCjyxNodeWriter);
  qCjyxNodeWriterOptionsWidget* options = new qCjyxNodeWriterOptionsWidget;
  options->setShowUseCompression(d->SupportUseCompression);
  return options;
}
