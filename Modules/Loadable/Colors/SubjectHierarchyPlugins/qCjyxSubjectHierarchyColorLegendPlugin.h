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

==============================================================================*/

#ifndef __qCjyxSubjectHierarchyColorLegendPlugin_h
#define __qCjyxSubjectHierarchyColorLegendPlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxColorsSubjectHierarchyPluginsExport.h"

class qCjyxSubjectHierarchyColorLegendPluginPrivate;
class vtkMRMLSliceNode;
class vtkMRMLViewNode;
class vtkMRMLAbstractViewNode;
class vtkMRMLDisplayNode;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_COLORS_SUBJECT_HIERARCHY_PLUGINS_EXPORT qCjyxSubjectHierarchyColorLegendPlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyColorLegendPlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyColorLegendPlugin() override;

public:

  /// Get visibility context menu item actions to add to tree view.
  /// These item visibility context menu actions can be shown in the implementations of \sa showVisibilityContextMenuActionsForItem
  QList<QAction*> visibilityContextMenuActions() const override;

  /// Show visibility context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the visibility context menu items for
  void showVisibilityContextMenuActionsForItem(vtkIdType itemID) override;

  /// Show an item in a selected view.
  /// Calls Volumes plugin's showItemInView implementation and adds support for showing a color legend in 2D and 3D views.
  /// Returns true on success.
  bool showItemInView(vtkIdType itemID, vtkMRMLAbstractViewNode* viewNode, vtkIdList* allItemsToShow) override;

  /// Show/hide color legend in a view.
  /// If viewNode is nullptr then it is displayed in all views in the current layout.
  bool showColorLegendInView( bool show, vtkIdType itemID, vtkMRMLViewNode* viewNode=nullptr);
  bool showColorLegendInSlice( bool show, vtkIdType itemID, vtkMRMLSliceNode* sliceNode=nullptr);

protected slots:
  /// Toggle color legend option for current volume item
  void toggleVisibilityForCurrentItem(bool);

protected:
  QScopedPointer<qCjyxSubjectHierarchyColorLegendPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyColorLegendPlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyColorLegendPlugin);
};

#endif
