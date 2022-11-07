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

// Qt includes
#include <QVariant>

// Cjyx includes
#include "qCjyxRelativePathMapper.h"
#include "qCjyxCoreApplication.h"

//-----------------------------------------------------------------------------
class qCjyxRelativePathMapperPrivate
{
public:
  qCjyxRelativePathMapperPrivate();
  QByteArray PropertyName;
};

// --------------------------------------------------------------------------
qCjyxRelativePathMapperPrivate::qCjyxRelativePathMapperPrivate()
{
}

// --------------------------------------------------------------------------
// qCjyxRelativePathMapper methods

// --------------------------------------------------------------------------
qCjyxRelativePathMapper::qCjyxRelativePathMapper(
  QObject* targetObject, const QByteArray& property, const QByteArray& signal)
  : QObject(targetObject)
  , d_ptr(new qCjyxRelativePathMapperPrivate)
{
  Q_ASSERT(!property.isEmpty());
  Q_ASSERT(targetObject != nullptr);
  Q_D(qCjyxRelativePathMapper);
  d->PropertyName = property;
  if (!signal.isEmpty())
    {
    if (QString(this->targetObject()->property(this->propertyName()).typeName()).compare("QStringList") == 0)
      {
      // Property is a QStringList
      connect(targetObject, signal, this, SLOT(emitPathsChanged()));
      }
    else
      {
      connect(targetObject, signal, this, SLOT(emitPathChanged()));
      }
    }
}

// --------------------------------------------------------------------------
qCjyxRelativePathMapper::~qCjyxRelativePathMapper()
{
}

// --------------------------------------------------------------------------
QByteArray qCjyxRelativePathMapper::propertyName()const
{
  Q_D(const qCjyxRelativePathMapper);
  return d->PropertyName;
}

// --------------------------------------------------------------------------
QObject* qCjyxRelativePathMapper::targetObject()const
{
  return this->parent();
}

// --------------------------------------------------------------------------
QString qCjyxRelativePathMapper::path()const
{
  Q_D(const qCjyxRelativePathMapper);
  return this->targetObject()->property(this->propertyName()).toString();
}

// --------------------------------------------------------------------------
QStringList qCjyxRelativePathMapper::paths()const
{
  Q_D(const qCjyxRelativePathMapper);
  return this->targetObject()->property(this->propertyName()).toStringList();
}

// --------------------------------------------------------------------------
QString qCjyxRelativePathMapper::relativePath()const
{
  Q_D(const qCjyxRelativePathMapper);
  return qCjyxCoreApplication::application()->toCjyxHomeRelativePath(this->path());
}

// --------------------------------------------------------------------------
QStringList qCjyxRelativePathMapper::relativePaths()const
{
  Q_D(const qCjyxRelativePathMapper);
  return qCjyxCoreApplication::application()->toCjyxHomeRelativePaths(this->paths());
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::setPath(const QString& newPath)
{
  Q_D(qCjyxRelativePathMapper);
  if (this->path() == newPath)
    {
    return;
    }
  this->targetObject()->setProperty(this->propertyName(), QVariant(newPath));
  this->emitPathChanged();
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::setPaths(const QStringList& newPaths)
{
  Q_D(qCjyxRelativePathMapper);
  if (this->paths() == newPaths)
    {
    return;
    }
  this->targetObject()->setProperty(this->propertyName(), QVariant(newPaths));
  this->emitPathsChanged();
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::setRelativePath(const QString& newRelativePath)
{
  Q_D(qCjyxRelativePathMapper);
  this->setPath(qCjyxCoreApplication::application()->toCjyxHomeAbsolutePath(newRelativePath));
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::setRelativePaths(const QStringList& newRelativePaths)
{
  Q_D(qCjyxRelativePathMapper);
  this->setPaths(qCjyxCoreApplication::application()->toCjyxHomeAbsolutePaths(newRelativePaths));
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::emitPathChanged()
{
  emit pathChanged(this->path());
  emit relativePathChanged(this->relativePath());
}

// --------------------------------------------------------------------------
void qCjyxRelativePathMapper::emitPathsChanged()
{
  emit pathsChanged(this->paths());
  emit relativePathsChanged(this->relativePaths());
}
