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

#ifndef __qCjyxSubjectHierarchyParseLocalDataPlugin_h
#define __qCjyxSubjectHierarchyParseLocalDataPlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class qCjyxSubjectHierarchyParseLocalDataPluginPrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qCjyxSubjectHierarchyParseLocalDataPlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyParseLocalDataPlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyParseLocalDataPlugin() override;

public:
  /// Get scene context menu item actions to add to tree view
  /// Separate method is needed for the scene, as its actions are set to the
  /// tree by a different method \sa itemContextMenuActions
  QList<QAction*> sceneContextMenuActions()const override;

  /// Show context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the context menu items for
  void showContextMenuActionsForItem(vtkIdType itemID) override;

public slots:
  /// Create subject hierarchy from loaded local directories.
  /// Organizes all items in subject hierarchy that have storable data nodes and has a valid storage node with a file
  /// name (meaning it has been loaded from local disk). Creates patient/study/series hierarchies according to the
  /// paths of the loaded files, ignoring the part that is identical (if everything has been loaded from the same directory,
  /// then only creates subject hierarchy nodes for the directories within that directory).
  //TODO: Port and move this function to SH logic, and add option to perform this parsing step after loading more than
  // one file. See https://discourse.slicer.org/t/python-called-qslicersubjecthierarchyparselocaldataplugin-function/18600
  void createHierarchyFromLoadedDirectoryStructure();

protected:
  QScopedPointer<qCjyxSubjectHierarchyParseLocalDataPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyParseLocalDataPlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyParseLocalDataPlugin);
};

#endif
