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
#include "qCjyxLoadableModuleTemplateFooBarWidget.h"
#include "ui_qCjyxLoadableModuleTemplateFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_LoadableModuleTemplate
class qCjyxLoadableModuleTemplateFooBarWidgetPrivate
  : public Ui_qCjyxLoadableModuleTemplateFooBarWidget
{
  Q_DECLARE_PUBLIC(qCjyxLoadableModuleTemplateFooBarWidget);
protected:
  qCjyxLoadableModuleTemplateFooBarWidget* const q_ptr;

public:
  qCjyxLoadableModuleTemplateFooBarWidgetPrivate(
    qCjyxLoadableModuleTemplateFooBarWidget& object);
  virtual void setupUi(qCjyxLoadableModuleTemplateFooBarWidget*);
};

// --------------------------------------------------------------------------
qCjyxLoadableModuleTemplateFooBarWidgetPrivate
::qCjyxLoadableModuleTemplateFooBarWidgetPrivate(
  qCjyxLoadableModuleTemplateFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxLoadableModuleTemplateFooBarWidgetPrivate
::setupUi(qCjyxLoadableModuleTemplateFooBarWidget* widget)
{
  this->Ui_qCjyxLoadableModuleTemplateFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qCjyxLoadableModuleTemplateFooBarWidget methods

//-----------------------------------------------------------------------------
qCjyxLoadableModuleTemplateFooBarWidget
::qCjyxLoadableModuleTemplateFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qCjyxLoadableModuleTemplateFooBarWidgetPrivate(*this) )
{
  Q_D(qCjyxLoadableModuleTemplateFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qCjyxLoadableModuleTemplateFooBarWidget
::~qCjyxLoadableModuleTemplateFooBarWidget()
{
}
