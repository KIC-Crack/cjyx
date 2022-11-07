/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

// Cjyx includes
#include "qCjyxTextsModuleWidget.h"
#include "ui_qCjyxTextsModuleWidget.h"

// vtkCjyxLogic includes
#include "vtkCjyxTextsLogic.h"

// MRML includes
#include "vtkMRMLTextNode.h"

//-----------------------------------------------------------------------------
class qCjyxTextsModuleWidgetPrivate: public Ui_qCjyxTextsModuleWidget
{
  Q_DECLARE_PUBLIC(qCjyxTextsModuleWidget);
protected:
  qCjyxTextsModuleWidget* const q_ptr;
public:
  qCjyxTextsModuleWidgetPrivate(qCjyxTextsModuleWidget& object);
  vtkCjyxTextsLogic*      logic() const;
};

//-----------------------------------------------------------------------------
qCjyxTextsModuleWidgetPrivate::qCjyxTextsModuleWidgetPrivate(qCjyxTextsModuleWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
vtkCjyxTextsLogic* qCjyxTextsModuleWidgetPrivate::logic()const
{
  Q_Q(const qCjyxTextsModuleWidget);
  return vtkCjyxTextsLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
qCjyxTextsModuleWidget::qCjyxTextsModuleWidget(QWidget* _parentWidget)
  : Superclass(_parentWidget)
  , d_ptr(new qCjyxTextsModuleWidgetPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxTextsModuleWidget::~qCjyxTextsModuleWidget() = default;

//-----------------------------------------------------------------------------
void qCjyxTextsModuleWidget::setup()
{
  Q_D(qCjyxTextsModuleWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
bool qCjyxTextsModuleWidget::setEditedNode(
  vtkMRMLNode* node,
  QString role/*=QString()*/,
  QString context/*=QString()*/)
{
  Q_D(qCjyxTextsModuleWidget);
  Q_UNUSED(role);
  Q_UNUSED(context);
  if (vtkMRMLTextNode::SafeDownCast(node))
    {
    d->TextNodeSelector->setCurrentNode(node);
    return true;
    }
  return false;
}
