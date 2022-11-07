/*==============================================================================

  Program: 3D Cjyx

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

// Qt includes
#include <QDebug>

// Cjyx includes
#include "qCjyxTemplateKeyModuleWidget.h"
#include "ui_qCjyxTemplateKeyModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxTemplateKeyModuleWidgetPrivate: public Ui_qCjyxTemplateKeyModuleWidget
{
public:
  qCjyxTemplateKeyModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qCjyxTemplateKeyModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModuleWidgetPrivate::qCjyxTemplateKeyModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qCjyxTemplateKeyModuleWidget methods

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModuleWidget::qCjyxTemplateKeyModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qCjyxTemplateKeyModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qCjyxTemplateKeyModuleWidget::~qCjyxTemplateKeyModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qCjyxTemplateKeyModuleWidget::setup()
{
  Q_D(qCjyxTemplateKeyModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}
