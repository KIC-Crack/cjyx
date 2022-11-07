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

#ifndef __qCjyxSubjectHierarchyModuleWidget_h
#define __qCjyxSubjectHierarchyModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxSubjectHierarchyModuleExport.h"

class qCjyxSubjectHierarchyModuleWidgetPrivate;
class qMRMLSubjectHierarchyModel;
class qCjyxSubjectHierarchyPluginLogic;
class qCjyxSubjectHierarchyAbstractPlugin;

/// \ingroup Cjyx_QtModules_SubjectHierarchy
class Q_CJYX_QTMODULES_SUBJECTHIERARCHY_EXPORT qCjyxSubjectHierarchyModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:
  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxSubjectHierarchyModuleWidget(QWidget *parent=nullptr);
  ~qCjyxSubjectHierarchyModuleWidget() override;

  Q_INVOKABLE qCjyxSubjectHierarchyPluginLogic* pluginLogic();
  Q_INVOKABLE void setPluginLogic(qCjyxSubjectHierarchyPluginLogic* pluginLogic);

protected:
  QScopedPointer<qCjyxSubjectHierarchyModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyModuleWidget);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyModuleWidget);
};

#endif
