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

#ifndef __qCjyxIO_h
#define __qCjyxIO_h

// Qt includes
#include <QMap>
#include <QMetaType>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

// QtCore includes
#include "qCjyxBaseQTCoreExport.h"
#include "qCjyxObject.h"

class qCjyxIOOptions;
class qCjyxIOPrivate;
class vtkMRMLMessageCollection;

/// Base class for qCjyxFileReader and qCjyxFileWriter
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxIO
  : public QObject
  , public qCjyxObject
{
  Q_OBJECT

public:
  typedef QObject Superclass;
  explicit qCjyxIO(QObject* parent = nullptr);
  ~qCjyxIO() override;

  typedef QString     IOFileType;
  typedef QVariantMap IOProperties;

  /// Unique name of the reader/writer
  Q_INVOKABLE virtual QString description()const = 0;

  /// Multiple readers can share the same file type
  Q_INVOKABLE virtual IOFileType fileType()const = 0;

  /// Returns a list of options for the reader. qCjyxIOOptions can be
  /// derived and have a UI associated to it (i.e. qCjyxIOOptionsWidget).
  /// Warning: you are responsible for freeing the memory of the returned
  /// options
  Q_INVOKABLE virtual qCjyxIOOptions* options()const;

  /// Additional warning or error messages occurred during IO operation.
  Q_INVOKABLE vtkMRMLMessageCollection* userMessages()const;

protected:
  QScopedPointer<qCjyxIOPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxIO);
  Q_DISABLE_COPY(qCjyxIO);
};

Q_DECLARE_METATYPE(qCjyxIO::IOFileType)
Q_DECLARE_METATYPE(qCjyxIO::IOProperties)

#endif
