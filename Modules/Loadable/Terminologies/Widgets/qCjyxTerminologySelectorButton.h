/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

#ifndef __qCjyxTerminologySelectorButton_h
#define __qCjyxTerminologySelectorButton_h

// Qt includes
#include <QPushButton>

// Terminologies includes
#include "qCjyxTerminologiesModuleWidgetsExport.h"

#include "qCjyxTerminologyNavigatorWidget.h"
#include "vtkCjyxTerminologyEntry.h"

class qCjyxTerminologySelectorButtonPrivate;

/// \brief Button that opens terminology selector dialog
/// \ingroup CjyxRt_QtModules_Terminologies_Widgets
class Q_CJYX_MODULE_TERMINOLOGIES_WIDGETS_EXPORT qCjyxTerminologySelectorButton : public QPushButton
{
  Q_OBJECT

public:
  explicit qCjyxTerminologySelectorButton(QWidget* parent=nullptr);
  ~qCjyxTerminologySelectorButton() override;

#ifndef __VTK_WRAP__
  /// Get selected terminology and other metadata (name, color, auto-generated flags) into given info bundle object
  void terminologyInfo(qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo);
#endif

public slots:

#ifndef __VTK_WRAP__
  /// Set terminology and other metadata (name, color, auto-generated flags)
  void setTerminologyInfo(qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo);
#endif

  /// Opens a terminology dialog to select a new terminology.
  void changeTerminology();

signals:
  void terminologyChanged();
  void canceled();

protected slots:
  void onToggled(bool toggled=true);

protected:
  void paintEvent(QPaintEvent* event) override;

  QScopedPointer<qCjyxTerminologySelectorButtonPrivate> d_ptr;
private :
  Q_DECLARE_PRIVATE(qCjyxTerminologySelectorButton);
  Q_DISABLE_COPY(qCjyxTerminologySelectorButton);
};

#endif
