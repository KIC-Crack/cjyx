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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

// Qt includes
#include <QDebug>
#include <QDir>
#include <QElapsedTimer>
#include <QFileInfo>

// CTK includes
#include <ctkUtils.h>

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxCoreIOManager.h"
#include "qCjyxFileReader.h"
#include "qCjyxFileWriter.h"

// MRML includes
#include <vtkMRMLApplicationLogic.h>
#include <vtkMRMLMessageCollection.h>
#include <vtkMRMLNode.h>
#include <vtkMRMLTransformableNode.h>
#include <vtkMRMLTransformNode.h>
#include <vtkMRMLScene.h>
#include <vtkMRMLStorableNode.h>
#include <vtkMRMLStorageNode.h>

// VTK includes
#include <vtkCollection.h>
#include <vtkDataFileFormatHelper.h> // for GetFileExtensionFromFormatString()
#include <vtkNew.h>
#include <vtkStringArray.h>
#include <vtkGeneralTransform.h>

//-----------------------------------------------------------------------------
class qCjyxCoreIOManagerPrivate
{
public:
  qCjyxCoreIOManagerPrivate();
  ~qCjyxCoreIOManagerPrivate();
  vtkMRMLScene* currentScene()const;

  qCjyxFileReader* reader(const QString& fileName)const;
  QList<qCjyxFileReader*> readers(const QString& fileName)const;

  QList<qCjyxFileWriter*> writers(
    const qCjyxIO::IOFileType &fileType,
    const qCjyxIO::IOProperties& parameters,
    vtkMRMLScene* scene = nullptr
  ) const;

  QSettings*        ExtensionFileType;
  QList<qCjyxFileReader*> Readers;
  QList<qCjyxFileWriter*> Writers;
  QMap<qCjyxIO::IOFileType, QStringList> FileTypes;

  QString DefaultSceneFileType;
};

//-----------------------------------------------------------------------------
qCjyxCoreIOManagerPrivate::qCjyxCoreIOManagerPrivate() = default;

//-----------------------------------------------------------------------------
qCjyxCoreIOManagerPrivate::~qCjyxCoreIOManagerPrivate() = default;

//-----------------------------------------------------------------------------
vtkMRMLScene* qCjyxCoreIOManagerPrivate::currentScene()const
{
  return qCjyxCoreApplication::application()->mrmlScene();
}

//-----------------------------------------------------------------------------
qCjyxFileReader* qCjyxCoreIOManagerPrivate::reader(const QString& fileName)const
{
  QList<qCjyxFileReader*> matchingReaders = this->readers(fileName);
  return matchingReaders.count() ? matchingReaders[0] : 0;
}

//-----------------------------------------------------------------------------
QList<qCjyxFileReader*> qCjyxCoreIOManagerPrivate::readers(const QString& fileName)const
{
  // Use a map so that we can access readers sorted by confidence.
  // The more specific the filter that was matched, the higher confidence
  // that the reader is more appropriate (e.g., *.seg.nrrd is more specific than *.nrrd;
  // *.nrrd is more specific than *.*)
  QMultiMap<int, qCjyxFileReader*> matchingReadersSortedByConfidence;
  foreach(qCjyxFileReader* reader, this->Readers)
    {
    // reader->supportedNameFilters will return the length of the longest matched file extension
    // in longestExtensionMatch variable.
    int longestExtensionMatch = 0;
    QStringList matchedNameFilters = reader->supportedNameFilters(fileName, &longestExtensionMatch);
    if (!matchedNameFilters.empty() && reader->canLoadFile(fileName))
      {
      matchingReadersSortedByConfidence.insert(longestExtensionMatch, reader);
      }
    }
  // Put matching readers in a list, with highest confidence readers pushed to the front
  QList<qCjyxFileReader*> matchingReaders;
  QMapIterator<int, qCjyxFileReader*> i(matchingReadersSortedByConfidence);
  while (i.hasNext())
    {
    i.next();
    matchingReaders.push_front(i.value());
    }
  return matchingReaders;
}

//-----------------------------------------------------------------------------
QList<qCjyxFileWriter*> qCjyxCoreIOManagerPrivate::writers(
  const qCjyxIO::IOFileType& fileType,
  const qCjyxIO::IOProperties& parameters,
  vtkMRMLScene* scene /*=nullptr*/
) const
{
  QString fileName = parameters.value("fileName").toString();
  QString nodeID = parameters.value("nodeID").toString();

  if (!scene)
    {
    scene = this->currentScene();
    }

  vtkObject * object = scene->GetNodeByID(nodeID.toUtf8());
  if (!object)
    {
    qWarning() << Q_FUNC_INFO << "warning: Unable to find node with ID" << nodeID << "in the given scene.";
    }
  QFileInfo file(fileName);

  QList<qCjyxFileWriter*> matchingWriters;
  // Some writers ("Cjyx Data Bundle (*)" can support any file,
  // they are called generic writers. The following code ensures
  // that writers associated with specific file extension are
  // considered first.
  QList<qCjyxFileWriter*> genericWriters;
  foreach(qCjyxFileWriter* writer, this->Writers)
    {
    if (writer->fileType() != fileType)
      {
      continue;
      }
    QStringList matchingNameFilters;
    foreach(const QString& nameFilter, writer->extensions(object))
      {
      foreach(const QString& extension, ctk::nameFilterToExtensions(nameFilter))
        {
        // HACK - See https://github.com/Slicer/Slicer/issues/3322
        QString extensionWithStar(extension);
        if (!extensionWithStar.startsWith("*"))
          {
          extensionWithStar.prepend("*");
          }
        QRegExp regExp(extensionWithStar, Qt::CaseInsensitive, QRegExp::Wildcard);
        Q_ASSERT(regExp.isValid());
        if (regExp.exactMatch(file.absoluteFilePath()))
          {
          matchingNameFilters << nameFilter;
          }
        }
      }
    if (matchingNameFilters.count() == 0)
      {
      continue;
      }
    // Generic readers must be added to the end
    foreach(const QString& nameFilter, matchingNameFilters)
      {
      if (nameFilter.contains( "*.*" ) || nameFilter.contains("(*)"))
        {
        genericWriters << writer;
        continue;
        }
      if (!matchingWriters.contains(writer))
        {
        matchingWriters << writer;
        }
      }
    }
  foreach(qCjyxFileWriter* writer, genericWriters)
    {
    if (!matchingWriters.contains(writer))
      {
      matchingWriters << writer;
      }
    }
  return matchingWriters;
}

//-----------------------------------------------------------------------------
qCjyxCoreIOManager::qCjyxCoreIOManager(QObject* _parent)
  :QObject(_parent)
  , d_ptr(new qCjyxCoreIOManagerPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxCoreIOManager::~qCjyxCoreIOManager() = default;

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxCoreIOManager::fileType(const QString& fileName)const
{
  QList<qCjyxIO::IOFileType> matchingFileTypes = this->fileTypes(fileName);
  return matchingFileTypes.count() ? matchingFileTypes[0] : QString("NoFile");
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxCoreIOManager
::fileTypeFromDescription(const QString& fileDescription)const
{
  qCjyxFileReader* reader = this->reader(fileDescription);
  return reader ? reader->fileType() : QString("NoFile");
}

//-----------------------------------------------------------------------------
qCjyxIO::IOFileType qCjyxCoreIOManager
::fileWriterFileType(vtkObject* object, const QString& format/*=QString()*/)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxIO::IOFileType> matchingFileTypes;
  // closest match is the writer that supports the node type but not
  // that specific extension
  QString closestMatch = QString("NoFile");
  foreach (const qCjyxFileWriter* writer, d->Writers)
    {
    if (writer->canWriteObject(object))
      {
      closestMatch = writer->fileType();
      if (format.isEmpty() || writer->extensions(object).contains(format))
        {
        return writer->fileType();
        }
      }
    }
  return closestMatch;
}

//-----------------------------------------------------------------------------
QList<qCjyxIO::IOFileType> qCjyxCoreIOManager::fileTypes(const QString& fileName)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxIO::IOFileType> matchingFileTypes;
  foreach (const qCjyxIO* matchingReader, d->readers(fileName))
    {
    matchingFileTypes << matchingReader->fileType();
    }
  return matchingFileTypes;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::fileDescriptions(const QString& fileName)const
{
  Q_D(const qCjyxCoreIOManager);
  QStringList matchingDescriptions;
  foreach(qCjyxFileReader* reader, d->readers(fileName))
    {
    matchingDescriptions << reader->description();
    }
  return matchingDescriptions;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::
fileDescriptionsByType(const qCjyxIO::IOFileType fileType)const
{
  QStringList matchingDescriptions;
  foreach(qCjyxFileReader* reader, this->readers())
    {
    if (reader->fileType() == fileType)
      {
      matchingDescriptions << reader->description();
      }
    }
  return matchingDescriptions;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::fileWriterDescriptions(
  const qCjyxIO::IOFileType& fileType)const
{
  QStringList matchingDescriptions;
  foreach(qCjyxFileWriter* writer, this->writers(fileType))
    {
    matchingDescriptions << writer->description();
    }
  return matchingDescriptions;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::fileWriterExtensions(
  vtkObject* object)const
{
  Q_D(const qCjyxCoreIOManager);
  QStringList matchingExtensions;
  foreach(qCjyxFileWriter* writer, d->Writers)
    {
    if (writer->canWriteObject(object))
      {
      matchingExtensions << writer->extensions(object);
      }
    }
  matchingExtensions.removeDuplicates();
  return matchingExtensions;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::allWritableFileExtensions()const
{
  Q_D(const qCjyxCoreIOManager);

  QStringList extensions;

  if (!d->currentScene())
    {
    qWarning() << "allWritableFileExtensions: manager has no scene defined";
    return extensions;
    }
  // check for all extensions that can be used to write storable nodes
  int numRegisteredNodeClasses = d->currentScene()->GetNumberOfRegisteredNodeClasses();
  for (int i = 0; i < numRegisteredNodeClasses; ++i)
    {
    vtkMRMLNode *mrmlNode = d->currentScene()->GetNthRegisteredNodeClass(i);
    if (mrmlNode && mrmlNode->IsA("vtkMRMLStorageNode"))
      {
      vtkMRMLStorageNode* snode = vtkMRMLStorageNode::SafeDownCast(mrmlNode);
      if (snode)
        {
        vtkNew<vtkStringArray> supportedFileExtensions;
        snode->GetFileExtensionsFromFileTypes(snode->GetSupportedWriteFileTypes(), supportedFileExtensions.GetPointer());
        const int formatCount = supportedFileExtensions->GetNumberOfValues();
        for (int formatIt = 0; formatIt < formatCount; ++formatIt)
          {
          QString extension = QString::fromStdString(supportedFileExtensions->GetValue(formatIt));
          extensions << extension;
          }
        }
      }
    }
  extensions.removeDuplicates();
  return extensions;
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreIOManager::allReadableFileExtensions()const
{
  Q_D(const qCjyxCoreIOManager);

  QStringList extensions;

  if (!d->currentScene())
    {
    qWarning() << "allReadableFileExtensions: manager has no scene defined";
    return extensions;
    }
  // check for all extensions that can be used to read storable nodes
  int numRegisteredNodeClasses = d->currentScene()->GetNumberOfRegisteredNodeClasses();
  for (int i = 0; i < numRegisteredNodeClasses; ++i)
    {
    vtkMRMLNode *mrmlNode = d->currentScene()->GetNthRegisteredNodeClass(i);
    if (mrmlNode && mrmlNode->IsA("vtkMRMLStorageNode"))
      {
      vtkMRMLStorageNode* snode = vtkMRMLStorageNode::SafeDownCast(mrmlNode);
      if (snode)
        {
        vtkNew<vtkStringArray> supportedFileExtensions;
        snode->GetFileExtensionsFromFileTypes(snode->GetSupportedReadFileTypes(), supportedFileExtensions.GetPointer());
        const int formatCount = supportedFileExtensions->GetNumberOfValues();
        for (int formatIt = 0; formatIt < formatCount; ++formatIt)
          {
          QString extension = QString::fromStdString(supportedFileExtensions->GetValue(formatIt));
          extensions << extension;
          }
        }
      }
    }
  extensions.removeDuplicates();
  return extensions;
}

//-----------------------------------------------------------------------------
QRegExp qCjyxCoreIOManager::fileNameRegExp(const QString& extension /*= QString()*/)
{
  QRegExp regExp("[A-Za-z0-9\\ \\-\\_\\.\\(\\)\\$\\!\\~\\#\\'\\%\\^\\{\\}]{1,255}");

  if (!extension.isEmpty())
    {
    regExp.setPattern(regExp.pattern() + extension);
    }
  return regExp;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreIOManager::forceFileNameValidCharacters(const QString& filename)
{
  // Remove characters that are likely to cause problems in filename
  QString sanitizedFilename;
  QRegExp regExp = fileNameRegExp();

  for (int i = 0; i < filename.size(); ++i)
    {
    if (regExp.exactMatch(QString(filename[i])))
      {
      sanitizedFilename += filename[i];
      }
    }

  // Remove leading and trailing spaces
  sanitizedFilename = sanitizedFilename.trimmed();

  return sanitizedFilename;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreIOManager::extractKnownExtension(const QString& fileName, vtkObject* object)
{
  QString longestMatchedExtension;
  foreach(const QString& nameFilter, this->fileWriterExtensions(object))
    {
    QString extension = QString::fromStdString(
      vtkDataFileFormatHelper::GetFileExtensionFromFormatString(nameFilter.toUtf8()));
    if (!extension.isEmpty() && fileName.endsWith(extension))
      {
      if (extension.length() > longestMatchedExtension.length())
        {
        longestMatchedExtension = extension;
        }
      }
    }
  return longestMatchedExtension;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreIOManager::stripKnownExtension(const QString& fileName, vtkObject* object)
{
  QString strippedFileName(fileName);

  QString knownExtension = extractKnownExtension(fileName, object);
  if (!knownExtension.isEmpty())
    {
    strippedFileName.chop(knownExtension.length());

    // recursively chop any further copies of the extension,
    // which sometimes appear when the filename+extension is
    // constructed from a filename that already had an extension
    if (strippedFileName.endsWith(knownExtension))
      {
      return stripKnownExtension(strippedFileName, object);
      }
    }
  return strippedFileName;
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxCoreIOManager::fileOptions(const QString& readerDescription)const
{
  Q_D(const qCjyxCoreIOManager);
  qCjyxFileReader* reader = this->reader(readerDescription);
  if (!reader)
    {
    return nullptr;
    }
  reader->setMRMLScene(d->currentScene());
  return reader->options();
}

//-----------------------------------------------------------------------------
qCjyxIOOptions* qCjyxCoreIOManager::fileWriterOptions(
  vtkObject* object, const QString& extension)const
{
  Q_D(const qCjyxCoreIOManager);
  qCjyxFileWriter* bestWriter = nullptr;
  foreach(qCjyxFileWriter* writer, d->Writers)
    {
    if (writer->canWriteObject(object))
      {
      if (writer->extensions(object).contains(extension))
        {
        writer->setMRMLScene(d->currentScene());
        bestWriter = writer;
        }
      }
    }
  return bestWriter ? bestWriter->options() : nullptr;
}

//-----------------------------------------------------------------------------
QString qCjyxCoreIOManager::completeCjyxWritableFileNameSuffix(vtkMRMLStorableNode *node)const
{
  vtkMRMLStorageNode* storageNode = node->GetStorageNode();
  if (!storageNode)
    {
    qWarning() << Q_FUNC_INFO << " failed: no storage node is available";
    return QString(".");
    }
  QString ext = QString::fromStdString(storageNode->GetSupportedFileExtension(nullptr, false, true));
  if (!ext.isEmpty())
    {
    // found
    return ext;
    }
  // otherwise return an empty suffix
  return QString(".");
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::loadScene(const QString& fileName, bool clear, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  qCjyxIO::IOProperties properties;
  properties["fileName"] = fileName;
  properties["clear"] = clear;
  return this->loadNodes(QString("SceneFile"), properties, nullptr, userMessages);
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::loadFile(const QString& fileName, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  qCjyxIO::IOProperties properties;
  properties["fileName"] = fileName;
  return this->loadNodes(this->fileType(fileName), properties, nullptr, userMessages);
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::loadNodes(const qCjyxIO::IOFileType& fileType,
                                     const qCjyxIO::IOProperties& parameters,
                                     vtkCollection* loadedNodes,
                                     vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  Q_D(qCjyxCoreIOManager);

  Q_ASSERT(parameters.contains("fileName"));
  if (parameters["fileName"].type() == QVariant::StringList)
    {
    bool res = true;
    QStringList fileNames = parameters["fileName"].toStringList();
    QStringList names = parameters["name"].toStringList();
    int nameId = 0;
    foreach(const QString& fileName, fileNames)
      {
      qCjyxIO::IOProperties fileParameters = parameters;
      fileParameters["fileName"] = fileName;
      if (!names.isEmpty())
        {
        fileParameters["name"] = nameId < names.size() ? names[nameId] : names.last();
        ++nameId;
        }
      res &= this->loadNodes(fileType, fileParameters, loadedNodes, userMessages);
      }
    return res;
    }
  Q_ASSERT(!parameters["fileName"].toString().isEmpty());

  qCjyxIO::IOProperties loadedFileParameters = parameters;
  loadedFileParameters.insert("fileType", fileType);

  const QList<qCjyxFileReader*>& readers = this->readers(fileType);

  // If no readers were able to read and load the file(s), success will remain false
  bool success = false;
  int numberOfUserMessagesBefore = userMessages ? userMessages->GetNumberOfMessages() : 0;
  QString userMessagePrefix = QString("Loading %1 - ").arg(parameters["fileName"].toString());

  QStringList nodes;
  foreach (qCjyxFileReader* reader, readers)
    {
    QElapsedTimer timeProbe;
    timeProbe.start();
    reader->userMessages()->ClearMessages();
    reader->setMRMLScene(d->currentScene());
    if (!reader->canLoadFile(parameters["fileName"].toString()))
      {
      continue;
      }
    bool currentFileSuccess = reader->load(parameters);
    if (userMessages)
      {
      userMessages->AddMessages(reader->userMessages(), userMessagePrefix.toStdString());
      }
    if (!currentFileSuccess)
      {
      continue;
      }
    float elapsedTimeInSeconds = timeProbe.elapsed() / 1000.0;
    qDebug() << reader->description() << "Reader has successfully read the file"
             << parameters["fileName"].toString()
             << QString("[%1s]").arg(
                  QString::number(elapsedTimeInSeconds,'f', 2));
    nodes << reader->loadedNodes();
    success = true;
    break;
    }

  if (!success && userMessages != nullptr && userMessages->GetNumberOfMessages() == numberOfUserMessagesBefore)
    {
    // Make sure that at least one message is logged if reading failed.
    userMessages->AddMessage(vtkCommand::ErrorEvent, (QString(tr("%1 load failed.")).arg(userMessagePrefix)).toStdString());
    }

  loadedFileParameters.insert("nodeIDs", nodes);

  emit newFileLoaded(loadedFileParameters);

  if (loadedNodes)
    {
    foreach(const QString& node, nodes)
      {
      vtkMRMLNode* loadedNode = d->currentScene()->GetNodeByID(node.toUtf8());
      if (!loadedNode)
        {
        qWarning() << Q_FUNC_INFO << " error: cannot find node by ID " << node;
        continue;
        }
      loadedNodes->AddItem(loadedNode);
      }
    }

  return success;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::loadNodes(const QList<qCjyxIO::IOProperties>& files,
          vtkCollection* loadedNodes, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  bool success = true;
  foreach(qCjyxIO::IOProperties fileProperties, files)
    {
    int numberOfUserMessagesBefore = userMessages ? userMessages->GetNumberOfMessages() : 0;
    success = this->loadNodes(
      static_cast<qCjyxIO::IOFileType>(fileProperties["fileType"].toString()),
      fileProperties, loadedNodes, userMessages)
      && success;
    // Add a separator between nodes
    if (userMessages && userMessages->GetNumberOfMessages() > numberOfUserMessagesBefore)
      {
      userMessages->AddSeparator();
      }
    }
  return success;
}

//-----------------------------------------------------------------------------
vtkMRMLNode* qCjyxCoreIOManager::loadNodesAndGetFirst(qCjyxIO::IOFileType fileType,
  const qCjyxIO::IOProperties& parameters, vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  vtkNew<vtkCollection> loadedNodes;
  this->loadNodes(fileType, parameters, loadedNodes.GetPointer(), userMessages);

  vtkMRMLNode* node = vtkMRMLNode::SafeDownCast(loadedNodes->GetItemAsObject(0));
  Q_ASSERT(node);

  return node;
}

//-----------------------------------------------------------------------------
vtkMRMLStorageNode* qCjyxCoreIOManager::createAndAddDefaultStorageNode(
    vtkMRMLStorableNode* node)
{
  if (!node)
    {
    qCritical() << Q_FUNC_INFO << " failed: invalid input node";
    return nullptr;
    }
  if (!node->AddDefaultStorageNode())
    {
    qCritical() << Q_FUNC_INFO << " failed: error while adding default storage node";
    return nullptr;
    }
  return node->GetStorageNode();
}

//-----------------------------------------------------------------------------
void qCjyxCoreIOManager::emitNewFileLoaded(const QVariantMap& loadedFileParameters)
{
  emit this->newFileLoaded(loadedFileParameters);
}

//-----------------------------------------------------------------------------
void qCjyxCoreIOManager::emitFileSaved(const QVariantMap& savedFileParameters)
{
  emit this->fileSaved(savedFileParameters);
}

//-----------------------------------------------------------------------------
void qCjyxCoreIOManager::addDefaultStorageNodes()
{
  Q_D(qCjyxCoreIOManager);
  int numNodes = d->currentScene()->GetNumberOfNodes();
  for (int i = 0; i < numNodes; ++i)
    {
    vtkMRMLStorableNode* storableNode = vtkMRMLStorableNode::SafeDownCast(d->currentScene()->GetNthNode(i));
    if (!storableNode)
      {
      continue;
      }
    if (!storableNode->GetSaveWithScene())
      {
      continue;
      }
    vtkMRMLStorageNode* storageNode = storableNode->GetStorageNode();
    if (storageNode)
      {
      // this node already has a storage node
      continue;
      }
    storableNode->AddDefaultStorageNode();
    storageNode = storableNode->GetStorageNode();
    if (!storageNode)
      {
      // no need for storage node to store this node
      // (some nodes can be saved either into the scene or into a separate file)
      continue;
      }
    std::string fileName(storageNode->GetFileName() ? storageNode->GetFileName() : "");
    if (!fileName.empty())
      {
      // filename is already set
      continue;
      }
    if (!storableNode->GetName())
      {
      // no node name is specified, cannot create a default file name
      continue;
      }
    // Default storage node usually has empty file name (if Save dialog is not opened yet)
    // file name is encoded to handle : or / characters in the node names
    std::string fileBaseName = vtkMRMLApplicationLogic::PercentEncode(storableNode->GetName());
    std::string extension = storageNode->GetDefaultWriteFileExtension();
    std::string storageFileName = fileBaseName + std::string(".") + extension;
    }
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::saveNodes(qCjyxIO::IOFileType fileType,
  const qCjyxIO::IOProperties& parameters,
  vtkMRMLMessageCollection* userMessages/*=nullptr*/,
  vtkMRMLScene* scene/*=nullptr*/)
{
  Q_D(qCjyxCoreIOManager);

  if (!scene)
    {
    scene = d->currentScene();
    }

  if (!parameters.contains("fileName") || !parameters["fileName"].canConvert<QString>())
    {
    qCritical() << Q_FUNC_INFO << "failed: \"fileName\" must be included as a string parameter.";
    return false;
    }
  QString fileName = parameters["fileName"].toString();
  if (fileName.isEmpty())
    {
    qCritical() << Q_FUNC_INFO << "failed: \"fileName\" parameter must not be empty.";
    return false;
    }

  // HACK - See https://github.com/Slicer/Slicer/issues/3322
  //        Sort writers to ensure generic ones are last.
  const QList<qCjyxFileWriter*> writers = d->writers(fileType, parameters, scene);
  if (writers.isEmpty())
    {
    qWarning() << "No writer found to write file" << fileName
               << "of type" << fileType;
    return false;
    }

  // Create the directory that the file will be saved to, if it does not exist
  if (!QFileInfo(fileName).dir().mkpath(".")) // Note that if the directory already exists, mkpath simply returns true
    {
    qWarning() << Q_FUNC_INFO << ": Unable to create directory" << QFileInfo(fileName).absolutePath();
    }

  QStringList nodes;
  bool writeSuccess=false;
  foreach (qCjyxFileWriter* writer, writers)
    {
    writer->setMRMLScene(scene);
    writer->userMessages()->ClearMessages();
    bool currentWriterSuccess = writer->write(parameters);
    if (userMessages)
      {
      userMessages->AddMessages(writer->userMessages());
      }
    if (!currentWriterSuccess)
      {
      continue;
      }
    nodes << writer->writtenNodes();
    emit fileSaved(parameters);
    writeSuccess = true;
    break;
    }

  if (!writeSuccess)
    {
    // no appropriate writer was found
    return false;
    }

  if (nodes.count() == 0 &&
      fileType != QString("SceneFile"))
    {
    // the writer did not report error
    // but did not report any successfully written nodes either
    return false;
    }

  return true;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::exportNodes(
  const QStringList& nodeIDs,
  const QStringList& fileNames,
  const qCjyxIO::IOProperties& commonParameterMap,
  bool hardenTransforms,
  vtkMRMLMessageCollection* userMessages/*=nullptr*/
)
{
  if (nodeIDs.length() != fileNames.length())
    {
    qCritical() << Q_FUNC_INFO << " failed: Mismatch in number of nodeIDs and filenames";
    return false;
    }
  QList<qCjyxIO::IOProperties> parameterMaps;
  int nodeCount = nodeIDs.length();
  for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
    {
    qCjyxIO::IOProperties parameterMap = commonParameterMap;
    parameterMap["nodeID"] = nodeIDs[nodeIndex];
    parameterMap["fileName"] = fileNames[nodeIndex];
    parameterMaps << parameterMap;
    }
  return this->exportNodes(parameterMaps, hardenTransforms, userMessages);
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::exportNodes(
  const QList<qCjyxIO::IOProperties>& parameterMaps,
  bool hardenTransforms,
  vtkMRMLMessageCollection* userMessages/*=nullptr*/
)
{
  Q_D(qCjyxCoreIOManager);

  // Create a temporary scene to use for exporting only and to be destroyed when done exporting
  vtkNew<vtkMRMLScene> temporaryScene;
  d->currentScene()->CopyDefaultNodesToScene(temporaryScene);
  d->currentScene()->CopyRegisteredNodesToScene(temporaryScene);
  d->currentScene()->CopySingletonNodesToScene(temporaryScene);
  temporaryScene->SetDataIOManager(d->currentScene()->GetDataIOManager());

  bool success = true;
  for (const auto& parameters : parameterMaps)
    {
    // Validate parameters
    for (const char* requiredKey : {"nodeID", "fileName", "fileFormat"})
      {
      if (!parameters.contains(requiredKey) || !parameters[requiredKey].canConvert<QString>())
        {
        qCritical() << Q_FUNC_INFO << "failed:" << requiredKey << "must be included as a string parameter.";
        return false;
        }
      }
    QString nodeID = parameters["nodeID"].toString();

    // Copy over each node to be exported
    vtkMRMLStorableNode* storableNode = vtkMRMLStorableNode::SafeDownCast(d->currentScene()->GetNodeByID(nodeID.toUtf8()));
    if (!storableNode)
      {
      if (userMessages)
        {
        userMessages->AddMessage(vtkCommand::ErrorEvent,
          (tr("Unable to find a storable node with ID %1").arg(nodeID)).toStdString()
        );
        }
      success = false;
      continue;
      }
    vtkMRMLStorableNode* temporaryStorableNode = vtkMRMLStorableNode::SafeDownCast(temporaryScene->AddNewNodeByClass(storableNode->GetClassName()));
    if (!temporaryStorableNode)
      {
      qCritical() << Q_FUNC_INFO << "error: Unable to add node to temporary scene";
      if (userMessages)
        {
        userMessages->AddMessage(vtkCommand::ErrorEvent,
          (tr("Error encountered while exporting %1.").arg(storableNode->GetName())).toStdString()
        );
        }
      success = false;
      continue;
      }
    // We will do a shallow copy, unless transform hardening was requested. Transform hardening
    // can sometimes affect the underlying data of the transformable node, so it's worth doing
    // a deep copy to be certain that the original node is not modified during export.
    temporaryStorableNode->CopyContent(storableNode, /*deepCopy=*/hardenTransforms);

    // If transform hardening was requested and node is transformable, then put transforms in the temporaryScene and apply hardening.
    vtkMRMLTransformableNode* nodeAsTransformable = vtkMRMLTransformableNode::SafeDownCast(storableNode);
    if (hardenTransforms && nodeAsTransformable)
      {
      vtkMRMLTransformableNode* temporaryNodeAsTransformable = vtkMRMLTransformableNode::SafeDownCast(temporaryStorableNode);

      if (!temporaryNodeAsTransformable)
        {
        qCritical() << Q_FUNC_INFO << " failed: Node is transformable but its copy is not... this should never happen.";
        return false;
        }

      vtkMRMLTransformNode* parentTransform = nodeAsTransformable->GetParentTransformNode();
      if (parentTransform)
        {
        vtkSmartPointer<vtkGeneralTransform> generalTransform =  vtkSmartPointer<vtkGeneralTransform>::New();
        parentTransform->GetTransformFromWorld(generalTransform);
        vtkMRMLTransformNode* compositeTransformNode =
          vtkMRMLTransformNode::SafeDownCast(temporaryScene->AddNewNodeByClass("vtkMRMLTransformNode"));
        if (!compositeTransformNode)
          {
          qCritical() << Q_FUNC_INFO << "Unable to add a transform node to temporary scene";
          if (userMessages)
            {
            userMessages->AddMessage(vtkCommand::ErrorEvent,
              (tr("Error encountered while exporting %1.").arg(storableNode->GetName())).toStdString()
            );
            }
          success = false;
          continue;
          }
        compositeTransformNode->SetAndObserveTransformFromParent(generalTransform);
        temporaryNodeAsTransformable->SetAndObserveTransformNodeID(compositeTransformNode->GetID());
        temporaryNodeAsTransformable->HardenTransform();
        }
      }

    // Copy parameters map; we will need to set the nodeID parameter to correspond to the node in the temporary scene
    qCjyxIO::IOProperties temporarySceneParameters = parameters;
    temporarySceneParameters["nodeID"] = temporaryStorableNode->GetID();

    // Deduce "fileType" from "fileFormat" parameter; saveNodes will want both
    qCjyxIO::IOFileType fileType = this->fileWriterFileType(storableNode, parameters["fileFormat"].toString());

    // Add default storage node into the temporary scene. This is sometimes needed.
    if (!temporaryStorableNode->AddDefaultStorageNode())
      {
      qCritical() << Q_FUNC_INFO << "error: Unable to create default storage in temporary scene";
      if (userMessages)
        {
        userMessages->AddMessage(vtkCommand::ErrorEvent,
          (tr("Unable to create default storage node for %1 in temporary scene.").arg(storableNode->GetName())).toStdString()
        );
        }
      success = false;
      continue;
      }

    // Finally, applying saving logic to the the temporary scene
    if (!this->saveNodes(fileType, temporarySceneParameters, userMessages, temporaryScene))
      {
      if (userMessages)
        {
        userMessages->AddMessage(vtkCommand::ErrorEvent,
          (tr("Error encountered while exporting %1.").arg(storableNode->GetName())).toStdString()
        );
        }
      success = false;
      }

      // Pick up any user messages that were saved to temporaryStorableNode's storage node
      if (userMessages && temporaryStorableNode->GetStorageNode()
          && temporaryStorableNode->GetStorageNode()->GetUserMessages())
        {
        userMessages->AddMessages(temporaryStorableNode->GetStorageNode()->GetUserMessages());
        }

    }
  return success;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::saveScene(const QString& fileName, QImage screenShot,
  vtkMRMLMessageCollection* userMessages/*=nullptr*/)
{
  qCjyxIO::IOProperties properties;
  properties["fileName"] = fileName;
  properties["screenShot"] = screenShot;

  return this->saveNodes(QString("SceneFile"), properties, userMessages);
}

//-----------------------------------------------------------------------------
const QList<qCjyxFileReader*>& qCjyxCoreIOManager::readers()const
{
  Q_D(const qCjyxCoreIOManager);
  return d->Readers;
}

//-----------------------------------------------------------------------------
const QList<qCjyxFileWriter*>& qCjyxCoreIOManager::writers()const
{
  Q_D(const qCjyxCoreIOManager);
  return d->Writers;
}

//-----------------------------------------------------------------------------
QList<qCjyxFileReader*> qCjyxCoreIOManager::readers(const qCjyxIO::IOFileType& fileType)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxFileReader*> res;
  foreach(qCjyxFileReader* io, d->Readers)
    {
    if (io->fileType() == fileType)
      {
      res << io;
      }
    }
  return res;
}

//-----------------------------------------------------------------------------
QList<qCjyxFileWriter*> qCjyxCoreIOManager::writers(const qCjyxIO::IOFileType& fileType)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxFileWriter*> res;
  foreach(qCjyxFileWriter* io, d->Writers)
    {
    if (io->fileType() == fileType)
      {
      res << io;
      }
    }
  return res;
}

//-----------------------------------------------------------------------------
qCjyxFileReader* qCjyxCoreIOManager::reader(const QString& ioDescription)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxFileReader*> res;
  foreach(qCjyxFileReader* io, d->Readers)
    {
    if (io->description() == ioDescription)
      {
      res << io;
      }
    }
  Q_ASSERT(res.count() < 2);
  return res.count() ? res[0] : 0;
}

//-----------------------------------------------------------------------------
void qCjyxCoreIOManager::registerIO(qCjyxIO* io)
{
  Q_ASSERT(io);
  Q_D(qCjyxCoreIOManager);
  qCjyxFileReader* fileReader = qobject_cast<qCjyxFileReader*>(io);
  qCjyxFileWriter* fileWriter = qobject_cast<qCjyxFileWriter*>(io);
  if (fileWriter)
    {
    d->Writers << fileWriter;
    }
  else if (fileReader)
    {
    d->Readers << fileReader;
    }

  // Reparent - this will make sure the object is destroyed properly
  if (io)
    {
    io->setParent(this);
    }
}

//-----------------------------------------------------------------------------
QString qCjyxCoreIOManager::defaultSceneFileType()const
{
  Q_D(const qCjyxCoreIOManager);
  return d->DefaultSceneFileType;
}

//-----------------------------------------------------------------------------
void qCjyxCoreIOManager::setDefaultSceneFileType(QString fileType)
{
  Q_D(qCjyxCoreIOManager);
  d->DefaultSceneFileType = fileType;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreIOManager::examineFileInfoList(QFileInfoList &fileInfoList, QFileInfo &archetypeFileInfo, QString &readerDescription, qCjyxIO::IOProperties &ioProperties)const
{
  Q_D(const qCjyxCoreIOManager);
  QList<qCjyxFileReader*> res;
  foreach(qCjyxFileReader* reader, d->Readers)
    {
    // TODO: currently the first reader that accepts the list will be used, but nothing
    // guarantees that the first reader is the most suitable choice (e.g., volume reader
    // grabs all file sequences, while they may not be sequence of frames but sequence of models, etc.).
    // There should be a mechanism (e.g., using confidence values or based on most specific extension)
    // to decide which reader should be used.
    // Multiple readers cannot be returned because they might not remove exactly the same set of files from the info list.
    if (reader->examineFileInfoList(fileInfoList, archetypeFileInfo, ioProperties))
      {
      readerDescription = reader->description();
      return true;
      }
    }
  return false;
}
