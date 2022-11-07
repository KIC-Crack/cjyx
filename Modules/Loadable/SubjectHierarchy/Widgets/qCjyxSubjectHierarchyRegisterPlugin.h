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

#ifndef __qCjyxSubjectHierarchyRegisterPlugin_h
#define __qCjyxSubjectHierarchyRegisterPlugin_h

// SubjectHierarchy Plugins includes
#include "qCjyxSubjectHierarchyAbstractPlugin.h"

#include "qCjyxSubjectHierarchyModuleWidgetsExport.h"

class qCjyxSubjectHierarchyRegisterPluginPrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy_Widgets
class Q_CJYX_MODULE_SUBJECTHIERARCHY_WIDGETS_EXPORT qCjyxSubjectHierarchyRegisterPlugin : public qCjyxSubjectHierarchyAbstractPlugin
{
public:
  Q_OBJECT

public:
  typedef qCjyxSubjectHierarchyAbstractPlugin Superclass;
  qCjyxSubjectHierarchyRegisterPlugin(QObject* parent = nullptr);
  ~qCjyxSubjectHierarchyRegisterPlugin() override;

public:
  /// Get item context menu item actions to add to tree view
  QList<QAction*> itemContextMenuActions()const override;

  /// Show context menu actions valid for a given subject hierarchy item.
  /// \param itemID Subject Hierarchy item to show the context menu items for
  void showContextMenuActionsForItem(vtkIdType itemID) override;

protected slots:
  /// Start registration process by selecting the current item as the 'from' item.
  /// Saves item in \sa m_RegisterFromItem and shows "Register * to this using..."
  /// context menu option offering the possible registration methods,
  void registerCurrentItemTo();

  /// Register saved 'from' item to current item using image based rigid registration.
  /// (Switch to registration module corresponding to selected method, set chosen
  /// input items, offer a best guess parameter set based on modalities etc.)
  void registerImageBasedRigid();

  /// Register saved 'from' item to current item using image based BSpline registration
  /// (Switch to registration module corresponding to selected method, set chosen
  /// input items, offer a best guess parameter set based on modalities etc.)
  void registerImageBasedBSpline();

  /// Register saved 'from' item to current item using interactive landmark registration
  /// (Switch to registration module corresponding to selected method, set chosen
  /// input items, offer a best guess parameter set based on modalities etc.)
  void registerInteractiveLandmark();

  /// Cancel registration (un-select first volume)
  void cancel();

protected:
  vtkIdType m_RegisterFromItem;

protected:
  QScopedPointer<qCjyxSubjectHierarchyRegisterPluginPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyRegisterPlugin);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyRegisterPlugin);
};

#endif
