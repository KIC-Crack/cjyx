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

#ifndef __qCjyxObject_h
#define __qCjyxObject_h

// Qt includes
#include <QScopedPointer>

// CTK includes
#include "qCjyxBaseQTCoreExport.h"

class vtkMRMLScene;
class qCjyxObjectPrivate;

class Q_CJYX_BASE_QTCORE_EXPORT qCjyxObject
{
public:
  qCjyxObject();
  virtual ~qCjyxObject();

  /// Return a pointer on the MRML scene
  vtkMRMLScene* mrmlScene() const;

  /// Set the current MRML scene to the widget
  virtual void setMRMLScene(vtkMRMLScene*);

protected:
  QScopedPointer<qCjyxObjectPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxObject);
  Q_DISABLE_COPY(qCjyxObject);
};

#endif
