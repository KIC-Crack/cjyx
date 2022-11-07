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

#ifndef __qCjyxScriptedFileWriter_h
#define __qCjyxScriptedFileWriter_h

// Cjyx includes
#include "qCjyxFileWriter.h"
#include "qCjyxBaseQTCoreExport.h"

// Forward Declare PyObject*
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif
class qCjyxScriptedFileWriterPrivate;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxScriptedFileWriter
  : public qCjyxFileWriter
{
  Q_OBJECT

  /// This property allows the writer to report back what nodes it was able to write
  Q_PROPERTY(QStringList writtenNodes READ writtenNodes WRITE setWrittenNodes)

public:
  typedef qCjyxFileWriter Superclass;
  qCjyxScriptedFileWriter(QObject* parent = nullptr);
  ~qCjyxScriptedFileWriter() override;

  QString pythonSource()const;

  /// \warning Setting the source is a no-op. See detailed comment in the source code.
  /// If missingClassIsExpected is true (default) then missing class is expected and not treated as an error.
  bool setPythonSource(const QString& newPythonSource, const QString& className = QLatin1String(""), bool missingClassIsExpected = true);

  /// Convenience method allowing to retrieve the associated scripted instance
  Q_INVOKABLE PyObject* self() const;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxIO::description()
  QString description()const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxIO::fileType()
  IOFileType fileType()const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxFileWriter::canWriteObject()
  bool canWriteObject(vtkObject* object)const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxFileWriter::extensions()
  QStringList extensions(vtkObject* object)const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxFileWriter::write()
  bool write(const qCjyxIO::IOProperties& properties) override;

  /// Added so node writers can report back written nodes
  /// \sa qCjyxFileWriter::writtenNodex()
  void addWrittenNode(const QString& writtenNode);

  /// Reimplemented to support python methods and q_property
  /// Exposes setWrittenNodes, which is protected in superclass
  /// \sa qCjyxFileWriter::writtenNodes()
  /// \sa qCjyxFileReader::loadedNodes()
  QStringList writtenNodes()const override {
    return Superclass::writtenNodes();
  };
  void setWrittenNodes(const QStringList& nodes) override {
    Superclass::setWrittenNodes(nodes);
  };

protected:
  QScopedPointer<qCjyxScriptedFileWriterPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxScriptedFileWriter);
  Q_DISABLE_COPY(qCjyxScriptedFileWriter);
};

#endif
