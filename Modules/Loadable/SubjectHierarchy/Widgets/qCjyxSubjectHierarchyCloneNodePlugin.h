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

#ifndef __qCjyxSubjectHierarchyCloneNodePlugin_h
#define __qCjyxSubjectHierarchyCloneNodePlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class qCjyxSubjectHierarchyCloneNodePluginPrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qCjyxSubjectHierarchyCloneNodePlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyCloneNodePlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyCloneNodePlugin() override;

public:
  Q_INVOKABLE static const QString getCloneNodeNamePostfix();

public:
  /// Get item context menu item actions to add to tree view
  QList<QAction*> itemContextMenuActions()const override;

  /// Show context menu actions valid for  given subject hierarchy node.
  /// \param node Subject Hierarchy node to show the context menu items for. If nullptr, then shows menu items for the scene
  void showContextMenuActionsForItem(vtkIdType itemID) override;

protected slots:
  /// Clone currently selected subject hierarchy item and associated data node
  void cloneCurrentItem();

protected:
  QScopedPointer<qCjyxSubjectHierarchyCloneNodePluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyCloneNodePlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyCloneNodePlugin);
};

#endif
