/*==============================================================================

Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
Queen's University, Kingston, ON, Canada. All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
and was supported through CANARIE's Research Software Program, and Cancer
Care Ontario.

==============================================================================*/

// QtGUI includes
#include "qCjyxSingletonViewFactory.h"

#include <QDebug>
#include <QMap>
#include <QSharedPointer>
#include <QWidget>

//-----------------------------------------------------------------------------
class qCjyxSingletonViewFactoryPrivate
{
  Q_DECLARE_PUBLIC(qCjyxSingletonViewFactory);
protected:
  qCjyxSingletonViewFactory* q_ptr;
public:
  qCjyxSingletonViewFactoryPrivate(qCjyxSingletonViewFactory& object);
  virtual ~qCjyxSingletonViewFactoryPrivate();

  virtual void init();

  // Widget returned from createViewFromXML
  QWidget* Widget;
  // Tag used for widget in createViewFromXML
  QString TagName;
  // Internal QWidget used to hold the widget until it is taken over by the layout manager
  QSharedPointer<QWidget> InternalWidget;
};

//-----------------------------------------------------------------------------
// qCjyxSingletonViewFactoryPrivate methods

qCjyxSingletonViewFactoryPrivate
::qCjyxSingletonViewFactoryPrivate(qCjyxSingletonViewFactory& object)
  : q_ptr(&object)
  , Widget(nullptr)
  , InternalWidget(new QWidget())
{
}

//-----------------------------------------------------------------------------
qCjyxSingletonViewFactoryPrivate::~qCjyxSingletonViewFactoryPrivate()
{
  Q_Q(qCjyxSingletonViewFactory);
  q->setWidget(nullptr);
}

//-----------------------------------------------------------------------------
void qCjyxSingletonViewFactoryPrivate::init()
{
}

//-----------------------------------------------------------------------------
// qCjyxSingletonViewFactory methods

//-----------------------------------------------------------------------------
qCjyxSingletonViewFactory::qCjyxSingletonViewFactory(QObject* parent)
  : Superclass(parent)
  , d_ptr(new qCjyxSingletonViewFactoryPrivate(*this))
{
  Q_D(qCjyxSingletonViewFactory);
  d->init();
  this->setUseCachedViews(false);
}

//-----------------------------------------------------------------------------
qCjyxSingletonViewFactory::~qCjyxSingletonViewFactory() = default;

//-----------------------------------------------------------------------------
QWidget* qCjyxSingletonViewFactory::widget()
{
  Q_D(qCjyxSingletonViewFactory);
  return d->Widget;
}

//-----------------------------------------------------------------------------
void qCjyxSingletonViewFactory::setWidget(QWidget* widget)
{
  Q_D(qCjyxSingletonViewFactory);
  if (d->Widget == widget)
    {
    return;
    }

  if (d->Widget)
    {
    QObject::disconnect(d->Widget, &QWidget::destroyed, this, &qCjyxSingletonViewFactory::onWidgetDestroyed);
    }

  d->Widget = widget;
  if (d->Widget)
    {
    d->Widget->setParent(d->InternalWidget.data()); // Hold the widget in the internal widget until the layout manager can take it
    QObject::connect(d->Widget, &QWidget::destroyed, this, &qCjyxSingletonViewFactory::onWidgetDestroyed);
    }
}

//-----------------------------------------------------------------------------
void qCjyxSingletonViewFactory::onWidgetDestroyed()
{
  this->setWidget(nullptr);
}

//-----------------------------------------------------------------------------
QString qCjyxSingletonViewFactory::tagName()
{
  Q_D(qCjyxSingletonViewFactory);
  return d->TagName;
}

//-----------------------------------------------------------------------------
void qCjyxSingletonViewFactory::setTagName(QString tagName)
{
  Q_D(qCjyxSingletonViewFactory);
  d->TagName = tagName;
}

//-----------------------------------------------------------------------------
QStringList qCjyxSingletonViewFactory::supportedElementNames() const
{
  Q_D(const qCjyxSingletonViewFactory);
  return QStringList() << d->TagName;
}

//---------------------------------------------------------------------------
QWidget* qCjyxSingletonViewFactory::createViewFromXML(QDomElement layoutElement)
{
  Q_UNUSED(layoutElement);
  Q_D(qCjyxSingletonViewFactory);
  if (d->Widget && d->Widget->isVisible())
    {
    qCritical() << "qCjyxSingletonViewFactory::createViewFromXML - Widget for view \"" << d->TagName << "\" is already in use within the current layout!";
    }

  return this->widget();
}
