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
#include <QPointer>

#include "qCjyxWidget.h"

// Cjyx includes
#include <qCjyxApplication.h>

// MRML includes
#include <vtkCjyxApplicationLogic.h>

// VTK includes

//-----------------------------------------------------------------------------
class qCjyxWidgetPrivate
{
  Q_DECLARE_PUBLIC(qCjyxWidget);
protected:
  qCjyxWidget* const q_ptr;

public:
  qCjyxWidgetPrivate(qCjyxWidget& object);

  QPointer<QWidget>                          ParentContainer;
};

//-----------------------------------------------------------------------------
qCjyxWidgetPrivate::qCjyxWidgetPrivate(qCjyxWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
qCjyxWidget::qCjyxWidget(QWidget * _parent, Qt::WindowFlags f)
  :QWidget(_parent, f)
  , d_ptr(new qCjyxWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxWidget::~qCjyxWidget() = default;

//-----------------------------------------------------------------------------

void qCjyxWidget::setMRMLScene(vtkMRMLScene* scene)
{
  bool emitSignal = this->mrmlScene() != scene;
  this->qCjyxObject::setMRMLScene(scene);
  if (emitSignal)
    {
    emit mrmlSceneChanged(scene);
    }
}

//-----------------------------------------------------------------------------
vtkCjyxApplicationLogic* qCjyxWidget::appLogic()const
{
  Q_D(const qCjyxWidget);
  if (!qCjyxApplication::application())
    {
    return nullptr;
    }
  return qCjyxApplication::application()->applicationLogic();
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qCjyxWidget::moduleLogic(const QString& moduleName)const
{
  Q_D(const qCjyxWidget);
  vtkCjyxApplicationLogic* applicationLogic = this->appLogic();
  if (!applicationLogic)
    {
    return nullptr;
    }
  return applicationLogic->GetModuleLogic(moduleName.toUtf8());
}
