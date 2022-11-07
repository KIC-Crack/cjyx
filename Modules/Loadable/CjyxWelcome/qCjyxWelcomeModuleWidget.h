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

#ifndef __qCjyxWelcomeModuleWidget_h
#define __qCjyxWelcomeModuleWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"
#include "qCjyxWelcomeModuleExport.h"

class qCjyxWelcomeModuleWidgetPrivate;

/// \ingroup Cjyx_QtModules_CjyxWelcome
class Q_CJYX_QTMODULES_WELCOME_EXPORT qCjyxWelcomeModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxWelcomeModuleWidget(QWidget *parent=nullptr);
  ~qCjyxWelcomeModuleWidget() override;


public slots:

  bool loadNonDicomData();
  bool loadRemoteSampleData();
  bool loadDicomData();
  void editApplicationSettings();
  bool exploreLoadedData();
  void setExtensionUpdatesAvailable(bool updateAvailable);

protected:
  void setup() override;

protected slots:
  void loadSource(QWidget*);

protected:
  QScopedPointer<qCjyxWelcomeModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxWelcomeModuleWidget);
  Q_DISABLE_COPY(qCjyxWelcomeModuleWidget);
};

#endif
