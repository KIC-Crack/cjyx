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

/// Qt includes
#include <QFileInfo>

// CTK includes
#include <ctkUtils.h>

/// QtCore includes
#include "qCjyxFileReader.h"

//-----------------------------------------------------------------------------
class qCjyxFileReaderPrivate
{
public:
  QStringList LoadedNodes;
};

//----------------------------------------------------------------------------
qCjyxFileReader::qCjyxFileReader(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxFileReaderPrivate)
{
}

//----------------------------------------------------------------------------
qCjyxFileReader::~qCjyxFileReader() = default;

//----------------------------------------------------------------------------
QStringList qCjyxFileReader::extensions()const
{
  return QStringList() << "*.*";
}

//----------------------------------------------------------------------------
bool qCjyxFileReader::canLoadFile(const QString& fileName)const
{
  QStringList res = this->supportedNameFilters(fileName);
  return res.count() > 0;
}

//----------------------------------------------------------------------------
QStringList qCjyxFileReader::supportedNameFilters(const QString& fileName, int* longestExtensionMatchPtr /* =nullptr */)const
{
  if (longestExtensionMatchPtr)
    {
    (*longestExtensionMatchPtr) = 0;
    }
  QStringList matchingNameFilters;
  QFileInfo file(fileName);
  if (!file.isFile() ||
      !file.isReadable() ||
      file.suffix().contains('~')) //temporary file
    {
    return matchingNameFilters;
    }
  foreach(const QString& nameFilter, this->extensions())
    {
    foreach(QString extension, ctk::nameFilterToExtensions(nameFilter))
      {
      QRegExp regExp(extension, Qt::CaseInsensitive, QRegExp::Wildcard);
      Q_ASSERT(regExp.isValid());
      if (regExp.exactMatch(file.absoluteFilePath()))
        {
        extension.remove('*'); // wildcard does not count, that's not a specific match
        int matchedExtensionLength = extension.size();
        if (longestExtensionMatchPtr && (*longestExtensionMatchPtr) < matchedExtensionLength)
          {
          (*longestExtensionMatchPtr) = matchedExtensionLength;
          }
        matchingNameFilters << nameFilter;
        }
      }
    }
  matchingNameFilters.removeDuplicates();
  return matchingNameFilters;
}


//----------------------------------------------------------------------------
bool qCjyxFileReader::load(const IOProperties& properties)
{
  Q_D(qCjyxFileReader);
  Q_UNUSED(properties);
  d->LoadedNodes.clear();
  return false;
}

//----------------------------------------------------------------------------
void qCjyxFileReader::setLoadedNodes(const QStringList& nodes)
{
  Q_D(qCjyxFileReader);
  d->LoadedNodes = nodes;
}

//----------------------------------------------------------------------------
QStringList qCjyxFileReader::loadedNodes()const
{
  Q_D(const qCjyxFileReader);
  return d->LoadedNodes;
}

//----------------------------------------------------------------------------
bool qCjyxFileReader::examineFileInfoList(QFileInfoList &fileInfoList, QFileInfo &archetypeFileInfo, qCjyxIO::IOProperties &ioProperties)const
{
  Q_UNUSED(fileInfoList);
  Q_UNUSED(archetypeFileInfo);
  Q_UNUSED(ioProperties);
  return(false);
}
