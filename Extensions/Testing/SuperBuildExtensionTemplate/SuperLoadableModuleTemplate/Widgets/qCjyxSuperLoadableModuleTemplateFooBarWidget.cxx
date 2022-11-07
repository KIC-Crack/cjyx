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
#include "qCjyxSuperLoadableModuleTemplateFooBarWidget.h"
#include "ui_qCjyxSuperLoadableModuleTemplateFooBarWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SuperLoadableModuleTemplate
class qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate
  : public Ui_qCjyxSuperLoadableModuleTemplateFooBarWidget
{
  Q_DECLARE_PUBLIC(qCjyxSuperLoadableModuleTemplateFooBarWidget);
protected:
  qCjyxSuperLoadableModuleTemplateFooBarWidget* const q_ptr;

public:
  qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate(
    qCjyxSuperLoadableModuleTemplateFooBarWidget& object);
  virtual void setupUi(qCjyxSuperLoadableModuleTemplateFooBarWidget*);
};

// --------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate
::qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate(
  qCjyxSuperLoadableModuleTemplateFooBarWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate
::setupUi(qCjyxSuperLoadableModuleTemplateFooBarWidget* widget)
{
  this->Ui_qCjyxSuperLoadableModuleTemplateFooBarWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qCjyxSuperLoadableModuleTemplateFooBarWidget methods

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateFooBarWidget
::qCjyxSuperLoadableModuleTemplateFooBarWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate(*this) )
{
  Q_D(qCjyxSuperLoadableModuleTemplateFooBarWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateFooBarWidget
::~qCjyxSuperLoadableModuleTemplateFooBarWidget()
{
}
