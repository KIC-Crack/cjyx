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
#include "qCjyxSuperLoadableModuleTemplateModuleWidget.h"
#include "ui_qCjyxSuperLoadableModuleTemplateModuleWidget.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_ExtensionTemplate
class qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate: public Ui_qCjyxSuperLoadableModuleTemplateModuleWidget
{
public:
  qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate();
};

//-----------------------------------------------------------------------------
// qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate::qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qCjyxSuperLoadableModuleTemplateModuleWidget methods

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModuleWidget::qCjyxSuperLoadableModuleTemplateModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qCjyxSuperLoadableModuleTemplateModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qCjyxSuperLoadableModuleTemplateModuleWidget::~qCjyxSuperLoadableModuleTemplateModuleWidget()
{
}

//-----------------------------------------------------------------------------
void qCjyxSuperLoadableModuleTemplateModuleWidget::setup()
{
  Q_D(qCjyxSuperLoadableModuleTemplateModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}
