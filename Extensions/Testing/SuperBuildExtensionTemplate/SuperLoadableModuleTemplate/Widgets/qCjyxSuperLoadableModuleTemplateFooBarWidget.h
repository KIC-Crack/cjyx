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

#ifndef __qCjyxSuperLoadableModuleTemplateFooBarWidget_h
#define __qCjyxSuperLoadableModuleTemplateFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qCjyxSuperLoadableModuleTemplateModuleWidgetsExport.h"

class qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate;

/// \ingroup Cjyx_QtModules_SuperLoadableModuleTemplate
class Q_CJYX_MODULE_SUPERLOADABLEMODULETEMPLATE_WIDGETS_EXPORT qCjyxSuperLoadableModuleTemplateFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qCjyxSuperLoadableModuleTemplateFooBarWidget(QWidget *parent=0);
  ~qCjyxSuperLoadableModuleTemplateFooBarWidget() override;

protected slots:

protected:
  QScopedPointer<qCjyxSuperLoadableModuleTemplateFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSuperLoadableModuleTemplateFooBarWidget);
  Q_DISABLE_COPY(qCjyxSuperLoadableModuleTemplateFooBarWidget);
};

#endif
