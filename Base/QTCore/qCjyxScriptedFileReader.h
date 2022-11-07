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

#ifndef __qCjyxScriptedFileReader_h
#define __qCjyxScriptedFileReader_h

// Cjyx includes
#include "qCjyxFileReader.h"
#include "qCjyxBaseQTCoreExport.h"

// Forward Declare PyObject*
#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
#endif
class qCjyxScriptedFileReaderPrivate;
class vtkObject;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxScriptedFileReader
  : public qCjyxFileReader
{
  Q_OBJECT

  /// This property allows the reader to report back what nodes it was able to load
  Q_PROPERTY(QStringList loadedNodes READ loadedNodes WRITE setLoadedNodes)

public:
  typedef qCjyxFileReader Superclass;
  qCjyxScriptedFileReader(QObject* parent = nullptr);
  ~qCjyxScriptedFileReader() override;

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
  /// \sa qCjyxFileReader::extensions()
  QStringList extensions()const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxFileReader::canLoadFile()
  bool canLoadFile(const QString& file)const override;

  /// Reimplemented to propagate to python methods
  /// \sa qCjyxFileReader::write()
  bool load(const qCjyxIO::IOProperties& properties) override;

  /// Reimplemented to support python methods and q_property
  /// Exposes setLoadedNodes, which is protected in superclass
  /// \sa qCjyxFileReader::loadedNodes()
  /// \sa qCjyxFileWriter::writtenNodes()
  QStringList loadedNodes()const override {
    return Superclass::loadedNodes();
  };
  void setLoadedNodes(const QStringList& nodes) override {
    Superclass::setLoadedNodes(nodes);
  };

protected:
  QScopedPointer<qCjyxScriptedFileReaderPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxScriptedFileReader);
  Q_DISABLE_COPY(qCjyxScriptedFileReader);
};

#endif
