/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __qCjyxRelativePathMapper_h
#define __qCjyxRelativePathMapper_h

// Qt includes
#include <QObject>

// CTK includes
#include "qCjyxBaseQTCoreExport.h"
class qCjyxRelativePathMapperPrivate;

//---------------------------------------------------------------------------
///
/// Example:
///   ctkDirectoryButton* directorySelector = new ctkDirectoryButton;
///   qCjyxRelativePathMapper* makeRelative =
///     new qCjyxRelativePathMapper("directory", SIGNAL("directoryChanged(QString)"), directorySelector);
///   makeRelative->setPath("some/folder");
///   // -> directorySelector->directory() == "applicationhome/some/folder"
///   makeRelative->setPath("/some/absolute/folder");
///   // -> directorySelector->directory() == "/some/absolute/folder"
///
/// Python example:
///   relativePathMapper = cjyx.qCjyxRelativePathMapper(directorySelector, "directory", "directoryChanged(QString)")
///   parent.registerProperty(
///     "settingsPropertyName", relativePathMapper, "relativePath", qt.SIGNAL("relativePathChanged(QString)"))
///
class Q_CJYX_BASE_QTCORE_EXPORT qCjyxRelativePathMapper : public QObject
{
  Q_OBJECT
  /// This property contains the name of the object mapped property.
  Q_PROPERTY(QByteArray propertyName READ propertyName)

  Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
  Q_PROPERTY(QStringList paths READ paths WRITE setPaths NOTIFY pathsChanged)

  Q_PROPERTY(QString relativePath READ relativePath WRITE setRelativePath NOTIFY relativePathChanged STORED false)
  Q_PROPERTY(QStringList relativePaths READ relativePaths WRITE setRelativePaths NOTIFY relativePathsChanged STORED false)
public:
  /// Map the property \a property of the object.
  /// The mapper becomes a child of \a object and will be destructed when
  /// \a object is destructed.
  /// property and object must be valid and non empty. If signal is 0,
  /// \a pathChanged(bool) and \a relativePathChanged(bool) won't be fired.
  qCjyxRelativePathMapper(QObject* targetObject, const QByteArray& propertyName, const QByteArray& signal);
  virtual ~qCjyxRelativePathMapper();

  QByteArray propertyName()const;

  /// The mapped object (the mapper parent)
  QObject* targetObject()const;

  QString path()const;
  QStringList paths()const;

  QString relativePath()const;
  QStringList relativePaths()const;
public Q_SLOTS:
  void setPath(const QString& path);
  void setPaths(const QStringList& path);
  void setRelativePath(const QString& path);
  void setRelativePaths(const QStringList& paths);

protected Q_SLOTS:
  void emitPathChanged();
  void emitPathsChanged();

Q_SIGNALS:
  void pathChanged(const QString& value);
  void pathsChanged(const QStringList& value);
  void relativePathChanged(const QString& value);
  void relativePathsChanged(const QStringList& value);

protected:
  QScopedPointer<qCjyxRelativePathMapperPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxRelativePathMapper);
  Q_DISABLE_COPY(qCjyxRelativePathMapper);
};

#endif
