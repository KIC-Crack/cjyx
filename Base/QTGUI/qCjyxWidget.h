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

#ifndef __qCjyxWidget_h
#define __qCjyxWidget_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>

#include "qCjyxObject.h"
#include "qCjyxBaseQTGUIExport.h"

class vtkMRMLAbstractLogic;
class vtkMRMLScene;
class vtkCjyxApplicationLogic;
class QScrollArea;
class qCjyxWidgetPrivate;

class Q_CJYX_BASE_QTGUI_EXPORT qCjyxWidget : public QWidget, public virtual qCjyxObject
{
  Q_OBJECT
  QVTK_OBJECT
public:
  qCjyxWidget(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
  ~qCjyxWidget() override;

  // Convenience method for getting application logic from the application.
  vtkCjyxApplicationLogic* appLogic()const;

  // Convenience method for getting a module logic from the application.
  vtkMRMLAbstractLogic* moduleLogic(const QString& moduleName)const;

public slots:
  void setMRMLScene(vtkMRMLScene* scene) override;

signals:
  void mrmlSceneChanged(vtkMRMLScene*);

protected:
  QScopedPointer<qCjyxWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxWidget);
  Q_DISABLE_COPY(qCjyxWidget);
};

#endif
