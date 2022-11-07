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

#ifndef __qCjyxSubjectHierarchyVisibilityPlugin_h
#define __qCjyxSubjectHierarchyVisibilityPlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class qCjyxSubjectHierarchyVisibilityPluginPrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qCjyxSubjectHierarchyVisibilityPlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyVisibilityPlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyVisibilityPlugin() override;

public:
  /// Get visibility context menu item actions to add to tree view.
  /// These item visibility context menu actions can be shown in the implementations of \sa showVisibilityContextMenuActionsForItem
  QList<QAction*> visibilityContextMenuActions()const override;

  /// Show visibility context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the visibility context menu items for
  void showVisibilityContextMenuActionsForItem(vtkIdType itemID) override;

protected slots:
  /// Toggle 2D visibility on currently selected subject hierarchy item
  void toggleCurrentItemVisibility2D(bool on);
  /// Toggle 3D visibility on currently selected subject hierarchy item
  void toggleCurrentItemVisibility3D(bool on);
  /// Makes the node visible in all views (otherwise it is just visible in selected views)
  void showInAllViews();

protected:
  QScopedPointer<qCjyxSubjectHierarchyVisibilityPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyVisibilityPlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyVisibilityPlugin);
};

#endif
