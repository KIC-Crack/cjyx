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

// Qt includes

// Cjyx includes
#include "qCjyxEventBrokerModuleWidget.h"
#include "ui_qCjyxEventBrokerModuleWidget.h"

#include <sstream>
#include <iostream>

//-----------------------------------------------------------------------------
class qCjyxEventBrokerModuleWidgetPrivate: public Ui_qCjyxEventBrokerModuleWidget
{
public:
  void setupUi(qCjyxWidget* widget);
};

//-----------------------------------------------------------------------------
void qCjyxEventBrokerModuleWidgetPrivate::setupUi(qCjyxWidget* widget)
{
  this->Ui_qCjyxEventBrokerModuleWidget::setupUi(widget);
  QObject::connect(this->EventBrokerWidget, SIGNAL(currentObjectChanged(vtkObject*)),
          widget, SLOT(onCurrentObjectChanged(vtkObject*)));
}

//-----------------------------------------------------------------------------
qCjyxEventBrokerModuleWidget::qCjyxEventBrokerModuleWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxEventBrokerModuleWidgetPrivate)
{
}

//-----------------------------------------------------------------------------
qCjyxEventBrokerModuleWidget::~qCjyxEventBrokerModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxEventBrokerModuleWidget::setup()
{
  Q_D(qCjyxEventBrokerModuleWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
void qCjyxEventBrokerModuleWidget::onCurrentObjectChanged(vtkObject* object)
{
  Q_D(qCjyxEventBrokerModuleWidget);
  if (!object)
    {
    return;
    }
  std::stringstream dumpStream;
  object->Print(dumpStream);
  d->TextEdit->setText(QString::fromStdString(dumpStream.str()));
}
