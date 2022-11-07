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

// Cjyx includes
#include "qCjyxAbstractModule.h"
#include "qCjyxAbstractModuleWidget.h"

//-----------------------------------------------------------------------------
class qCjyxAbstractModuleWidgetPrivate
{
public:
  qCjyxAbstractModuleWidgetPrivate();
  bool IsEntered;
};

//-----------------------------------------------------------------------------
qCjyxAbstractModuleWidgetPrivate::qCjyxAbstractModuleWidgetPrivate()
{
  this->IsEntered = false;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleWidget::qCjyxAbstractModuleWidget(QWidget* parentWidget)
  : qCjyxWidget(parentWidget)
  , d_ptr(new qCjyxAbstractModuleWidgetPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxAbstractModuleWidget::~qCjyxAbstractModuleWidget()
{
  Q_ASSERT(this->isEntered() == false);
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleWidget::enter()
{
  Q_D(qCjyxAbstractModuleWidget);
  Q_ASSERT(d->IsEntered == false);
  d->IsEntered = true;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleWidget::exit()
{
  Q_D(qCjyxAbstractModuleWidget);
  Q_ASSERT(d->IsEntered == true);
  d->IsEntered = false;
}

//-----------------------------------------------------------------------------
bool qCjyxAbstractModuleWidget::isEntered()const
{
  Q_D(const qCjyxAbstractModuleWidget);
  return d->IsEntered;
}

//-----------------------------------------------------------------------------
void qCjyxAbstractModuleWidget::setup()
{
  const qCjyxAbstractModule* m =
    qobject_cast<const qCjyxAbstractModule*>(this->module());
  if (m)
    {
    this->setObjectName(QString("%1ModuleWidget").arg(m->name()));
    this->setWindowTitle(m->title());
    this->setWindowIcon(m->icon());
    }
}

//-----------------------------------------------------------
bool qCjyxAbstractModuleWidget::setEditedNode(vtkMRMLNode* node,
                                                QString role /* = QString()*/,
                                                QString context /* = QString()*/)
{
  // this method is redefined here to make it Q_INVOKABLE
  return qCjyxAbstractModuleRepresentation::setEditedNode(node, role, context);
}

//-----------------------------------------------------------
double qCjyxAbstractModuleWidget::nodeEditable(vtkMRMLNode* node)
{
  // this method is redefined here to make it Q_INVOKABLE
  return qCjyxAbstractModuleRepresentation::nodeEditable(node);
}
