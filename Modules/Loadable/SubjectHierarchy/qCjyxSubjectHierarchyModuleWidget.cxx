/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Csaba Pinter, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

// Subject Hierarchy includes
#include "qCjyxSubjectHierarchyModuleWidget.h"
#include "ui_qCjyxSubjectHierarchyModule.h"

#include "qCjyxSubjectHierarchyPluginLogic.h"

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_SubjectHierarchy
class qCjyxSubjectHierarchyModuleWidgetPrivate: public Ui_qCjyxSubjectHierarchyModule
{
  Q_DECLARE_PUBLIC(qCjyxSubjectHierarchyModuleWidget);
protected:
  qCjyxSubjectHierarchyModuleWidget* const q_ptr;
public:
  qCjyxSubjectHierarchyModuleWidgetPrivate(qCjyxSubjectHierarchyModuleWidget& object);
  ~qCjyxSubjectHierarchyModuleWidgetPrivate();
public:
  /// Subject hierarchy plugin logic
  qCjyxSubjectHierarchyPluginLogic* PluginLogic;
};

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModuleWidgetPrivate::qCjyxSubjectHierarchyModuleWidgetPrivate(qCjyxSubjectHierarchyModuleWidget& object)
  : q_ptr(&object)
  , PluginLogic(nullptr)
{
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModuleWidgetPrivate::~qCjyxSubjectHierarchyModuleWidgetPrivate() = default;

//-----------------------------------------------------------------------------
// qCjyxSubjectHierarchyModuleWidget methods

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModuleWidget::qCjyxSubjectHierarchyModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qCjyxSubjectHierarchyModuleWidgetPrivate(*this) )
{
}

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyModuleWidget::~qCjyxSubjectHierarchyModuleWidget() = default;

//-----------------------------------------------------------------------------
qCjyxSubjectHierarchyPluginLogic* qCjyxSubjectHierarchyModuleWidget::pluginLogic()
{
  Q_D(qCjyxSubjectHierarchyModuleWidget);
  return d->PluginLogic;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyModuleWidget::setPluginLogic(qCjyxSubjectHierarchyPluginLogic* pluginLogic)
{
  Q_D(qCjyxSubjectHierarchyModuleWidget);
  d->PluginLogic = pluginLogic;
}

//-----------------------------------------------------------------------------
void qCjyxSubjectHierarchyModuleWidget::setup()
{
  Q_D(qCjyxSubjectHierarchyModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();
}
