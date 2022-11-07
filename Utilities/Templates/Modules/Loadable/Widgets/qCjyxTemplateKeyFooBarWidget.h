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

#ifndef __qCjyxTemplateKeyFooBarWidget_h
#define __qCjyxTemplateKeyFooBarWidget_h

// Qt includes
#include <QWidget>

// FooBar Widgets includes
#include "qCjyxTemplateKeyModuleWidgetsExport.h"

class qCjyxTemplateKeyFooBarWidgetPrivate;

/// \ingroup Cjyx_QtModules_TemplateKey
class Q_CJYX_MODULE_TEMPLATEKEY_WIDGETS_EXPORT qCjyxTemplateKeyFooBarWidget
  : public QWidget
{
  Q_OBJECT
public:
  typedef QWidget Superclass;
  qCjyxTemplateKeyFooBarWidget(QWidget *parent=0);
  ~qCjyxTemplateKeyFooBarWidget() override;

protected slots:

protected:
  QScopedPointer<qCjyxTemplateKeyFooBarWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTemplateKeyFooBarWidget);
  Q_DISABLE_COPY(qCjyxTemplateKeyFooBarWidget);
};

#endif
