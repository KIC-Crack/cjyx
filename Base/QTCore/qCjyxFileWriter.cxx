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

/// QtCore includes
#include "qCjyxFileWriter.h"

//-----------------------------------------------------------------------------
class qCjyxFileWriterPrivate
{
public:
  QStringList WrittenNodes;
};

//----------------------------------------------------------------------------
qCjyxFileWriter::qCjyxFileWriter(QObject* parentObject)
  : qCjyxIO(parentObject)
  , d_ptr(new qCjyxFileWriterPrivate)
{
}

//----------------------------------------------------------------------------
qCjyxFileWriter::~qCjyxFileWriter() = default;

//----------------------------------------------------------------------------
bool qCjyxFileWriter::canWriteObject(vtkObject* object)const
{
  Q_UNUSED(object);
  return false;
}

//----------------------------------------------------------------------------
bool qCjyxFileWriter::write(const qCjyxIO::IOProperties& properties)
{
  Q_D(qCjyxFileWriter);
  Q_UNUSED(properties);
  d->WrittenNodes.clear();
  return false;
}

//----------------------------------------------------------------------------
void qCjyxFileWriter::setWrittenNodes(const QStringList& nodes)
{
  Q_D(qCjyxFileWriter);
  d->WrittenNodes = nodes;
}

//----------------------------------------------------------------------------
QStringList qCjyxFileWriter::writtenNodes()const
{
  Q_D(const qCjyxFileWriter);
  return d->WrittenNodes;
}
