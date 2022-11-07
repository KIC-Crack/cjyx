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
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

#ifndef __qCjyxSubjectHierarchyOpacityPlugin_h
#define __qCjyxSubjectHierarchyOpacityPlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class qCjyxSubjectHierarchyOpacityPluginPrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qCjyxSubjectHierarchyOpacityPlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyOpacityPlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyOpacityPlugin() override;

public:
  /// Get visibility context menu item actions to add to tree view.
  /// These item visibility context menu actions can be shown in the implementations of \sa showVisibilityContextMenuActionsForItem
  QList<QAction*> visibilityContextMenuActions()const override;

  /// Show visibility context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the visibility context menu items for
  void showVisibilityContextMenuActionsForItem(vtkIdType itemID) override;

protected slots:
  /// Set opacity for current subject hierarchy item
  void setOpacityForCurrentItem(double opacity);

protected:
  QScopedPointer<qCjyxSubjectHierarchyOpacityPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyOpacityPlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyOpacityPlugin);
};

#endif
