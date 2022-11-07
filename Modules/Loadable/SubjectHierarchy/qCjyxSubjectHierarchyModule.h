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

#ifndef __qCjyxSubjectHierarchyModule_h
#define __qCjyxSubjectHierarchyModule_h

// CTK includes
#include "ctkVTKObject.h"

// Cjyx includes
#include "qCjyxLoadableModule.h"

#include "qCjyxSubjectHierarchyModuleExport.h"

class vtkObject;
class qCjyxSubjectHierarchyModulePrivate;

/// \ingroup Cjyx_QtModules_SubjectHierarchy
class Q_CJYX_QTMODULES_SUBJECTHIERARCHY_EXPORT qCjyxSubjectHierarchyModule :
  public qCjyxLoadableModule
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.cjyx.modules.loadable.qCjyxLoadableModule/1.0");
  Q_INTERFACES(qCjyxLoadableModule);
  QVTK_OBJECT

public:
  typedef qCjyxLoadableModule Superclass;
  explicit qCjyxSubjectHierarchyModule(QObject *parent=nullptr);
  ~qCjyxSubjectHierarchyModule() override;

  qCjyxGetTitleMacro(QTMODULE_TITLE);

  /// Help to use the module
  QString helpText()const override;

  /// Return acknowledgments
  QString acknowledgementText()const override;

  /// Return the authors of the module
  QStringList contributors()const override;

  /// Return a custom icon for the module
  QIcon icon()const override;

  /// Return the categories for the module
  QStringList categories()const override;

  /// Make this module hidden
  bool isHidden()const override { return true; };

protected:
  /// Initialize the module. Register the volumes reader/writer
  void setup() override;

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override;

  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation* createWidgetRepresentation() override;

protected slots:
  /// Handles logic modified event, which happens on major changes in the
  /// scene (new scene, batch processing, import etc.)
  void onLogicModified();

protected:
  QScopedPointer<qCjyxSubjectHierarchyModulePrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchyModule);
  Q_DISABLE_COPY(qCjyxSubjectHierarchyModule);
};

#endif
