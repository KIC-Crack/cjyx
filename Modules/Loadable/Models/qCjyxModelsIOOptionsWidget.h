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

#ifndef __qCjyxModelsIOOptionsWidget_h
#define __qCjyxModelsIOOptionsWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxIOOptionsWidget.h"
#include "qCjyxModelsModuleExport.h"

class qCjyxModelsIOOptionsWidgetPrivate;

class Q_CJYX_QTMODULES_MODELS_EXPORT qCjyxModelsIOOptionsWidget
  : public qCjyxIOOptionsWidget
{
  Q_OBJECT
public:
  typedef qCjyxIOOptionsWidget Superclass;
  qCjyxModelsIOOptionsWidget(QWidget *parent=nullptr);
  ~qCjyxModelsIOOptionsWidget() override;

protected slots:
  void updateProperties();

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr), qCjyxModelsIOOptionsWidget);
  Q_DISABLE_COPY(qCjyxModelsIOOptionsWidget);
};

#endif
