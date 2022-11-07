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

#ifndef __qCjyxLoadableModuleTemplateFooBarWidget_h
#define __qCjyxLoadableModuleTemplateFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qCjyxLoadableModuleTemplateModuleWidgetsExport.h"

class qCjyxLoadableModuleTemplateFooBarWidgetPrivate;

/// \ingroup Cjyx_QtModules_LoadableModuleTemplate
class Q_CJYX_MODULE_LOADABLEMODULETEMPLATE_WIDGETS_EXPORT qCjyxLoadableModuleTemplateFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qCjyxLoadableModuleTemplateFooBarWidget(QWidget *parent=0);
  ~qCjyxLoadableModuleTemplateFooBarWidget() override;

protected slots:

protected:
  QScopedPointer<qCjyxLoadableModuleTemplateFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxLoadableModuleTemplateFooBarWidget);
  Q_DISABLE_COPY(qCjyxLoadableModuleTemplateFooBarWidget);
};

#endif
