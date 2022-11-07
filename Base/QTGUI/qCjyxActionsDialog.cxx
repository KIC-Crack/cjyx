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

#include "vtkCjyxConfigure.h" // For Cjyx_BUILD_WEBENGINE_SUPPORT

// Qt includes
#include <QGridLayout>
#include <QtGlobal>
#ifdef Cjyx_BUILD_WEBENGINE_SUPPORT
#include <QWebEngineView>
#endif

// Cjyx includes
#include "qCjyxActionsDialog.h"
#include "qCjyxCoreApplication.h"
#include "ui_qCjyxActionsDialog.h"
#include "vtkCjyxVersionConfigure.h"

//-----------------------------------------------------------------------------
class qCjyxActionsDialogPrivate: public Ui_qCjyxActionsDialog
{
  Q_DECLARE_PUBLIC(qCjyxActionsDialog);
protected:
  qCjyxActionsDialog* const q_ptr;

public:
  qCjyxActionsDialogPrivate(qCjyxActionsDialog& object);
  void init();

#ifdef Cjyx_BUILD_WEBENGINE_SUPPORT
  QWebEngineView* WebView;
#endif

};

// --------------------------------------------------------------------------
qCjyxActionsDialogPrivate::qCjyxActionsDialogPrivate(qCjyxActionsDialog& object)
  : q_ptr(&object)
#ifdef Cjyx_BUILD_WEBENGINE_SUPPORT
  , WebView(nullptr)
#endif
{
}

// --------------------------------------------------------------------------
void qCjyxActionsDialogPrivate::init()
{
  Q_Q(qCjyxActionsDialog);

  this->setupUi(q);
#ifdef Cjyx_BUILD_WEBENGINE_SUPPORT
  this->WebView = new QWebEngineView();
  this->WebView->setObjectName("WebView");
  this->gridLayout->addWidget(this->WebView, 0, 0);
  qCjyxCoreApplication* app = qCjyxCoreApplication::application();
  QString shortcutsUrl = QString(q->tr("%1/user_guide/user_interface.html#mouse-keyboard-shortcuts")).arg(app->documentationBaseUrl());
  this->WebView->setUrl( shortcutsUrl );
#else
  this->tabWidget->setTabEnabled(this->tabWidget->indexOf(this->WikiTab), false);
#endif
}

//------------------------------------------------------------------------------
qCjyxActionsDialog::qCjyxActionsDialog(QWidget* parentWidget)
  : Superclass(parentWidget)
  , d_ptr(new qCjyxActionsDialogPrivate(*this))
{
  Q_D(qCjyxActionsDialog);
  d->init();
}

//------------------------------------------------------------------------------
qCjyxActionsDialog::~qCjyxActionsDialog() = default;

//------------------------------------------------------------------------------
void qCjyxActionsDialog::addAction(QAction* action, const QString& group)
{
  Q_D(qCjyxActionsDialog);
  d->ActionsWidget->addAction(action, group);
}

//------------------------------------------------------------------------------
void qCjyxActionsDialog::addActions(const QList<QAction*>& actions,
                                      const QString& group)
{
  Q_D(qCjyxActionsDialog);
  d->ActionsWidget->addActions(actions, group);
}

//------------------------------------------------------------------------------
void qCjyxActionsDialog::clear()
{
  Q_D(qCjyxActionsDialog);
  d->ActionsWidget->clear();
}

//------------------------------------------------------------------------------
void qCjyxActionsDialog::setActionsWithNoShortcutVisible(bool visible)
{
  Q_D(qCjyxActionsDialog);
  d->ActionsWidget->setActionsWithNoShortcutVisible(visible);
}

//------------------------------------------------------------------------------
void qCjyxActionsDialog::setMenuActionsVisible(bool visible)
{
  Q_D(qCjyxActionsDialog);
  d->ActionsWidget->setMenuActionsVisible(visible);
}
