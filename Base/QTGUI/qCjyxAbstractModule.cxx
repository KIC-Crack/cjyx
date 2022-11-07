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
#include <QAction>

// Cjyx includes
#include "qCjyxAbstractModule.h"

class qCjyxAbstractModulePrivate
{
public:
  qCjyxAbstractModulePrivate();
  QAction* Action;
};

//-----------------------------------------------------------------------------
qCjyxAbstractModulePrivate::qCjyxAbstractModulePrivate()
{
  this->Action = nullptr;
}

//-----------------------------------------------------------------------------
qCjyxAbstractModule::qCjyxAbstractModule(QObject* parentObject)
  :Superclass(parentObject)
  , d_ptr(new qCjyxAbstractModulePrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxAbstractModule::~qCjyxAbstractModule() = default;

//-----------------------------------------------------------------------------
QIcon qCjyxAbstractModule::icon()const
{
  return QIcon(":Icons/Extension.png");
}

//-----------------------------------------------------------------------------
QImage qCjyxAbstractModule::logo()const
{
  return QImage();
}

//-----------------------------------------------------------------------------
QAction* qCjyxAbstractModule::action()
{
  Q_D(qCjyxAbstractModule);
  if (d->Action == nullptr)
    {
    d->Action = new QAction(this->icon(), this->title(), this);
    d->Action->setObjectName(QString("action%1").arg(this->name()));
    d->Action->setData(this->name());
    d->Action->setIconVisibleInMenu(true);
    d->Action->setProperty("index", this->index());
    }
  return d->Action;
}
