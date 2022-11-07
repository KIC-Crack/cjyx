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

// FooBar Widgets includes
#include "qCjyxTemplateKeyFooBarWidget.h"
#include "ui_qCjyxTemplateKeyFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_TemplateKey
class qCjyxTemplateKeyFooBarWidgetPrivate
  : public Ui_qCjyxTemplateKeyFooBarWidget
{
  Q_DECLARE_PUBLIC(qCjyxTemplateKeyFooBarWidget);
protected:
  qCjyxTemplateKeyFooBarWidget* const q_ptr;

public:
  qCjyxTemplateKeyFooBarWidgetPrivate(
    qCjyxTemplateKeyFooBarWidget& object);
  virtual void setupUi(qCjyxTemplateKeyFooBarWidget*);
};

// --------------------------------------------------------------------------
qCjyxTemplateKeyFooBarWidgetPrivate
::qCjyxTemplateKeyFooBarWidgetPrivate(
  qCjyxTemplateKeyFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxTemplateKeyFooBarWidgetPrivate
::setupUi(qCjyxTemplateKeyFooBarWidget* widget)
{
  this->Ui_qCjyxTemplateKeyFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qCjyxTemplateKeyFooBarWidget methods

//-----------------------------------------------------------------------------
qCjyxTemplateKeyFooBarWidget
::qCjyxTemplateKeyFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qCjyxTemplateKeyFooBarWidgetPrivate(*this) )
{
  Q_D(qCjyxTemplateKeyFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qCjyxTemplateKeyFooBarWidget
::~qCjyxTemplateKeyFooBarWidget()
{
}
