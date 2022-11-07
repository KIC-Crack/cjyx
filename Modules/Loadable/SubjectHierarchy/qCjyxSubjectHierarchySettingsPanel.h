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

#ifndef __qCjyxSubjectHierarchySettingsPanel_h
#define __qCjyxSubjectHierarchySettingsPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>

#include "qCjyxSubjectHierarchyModuleExport.h"

class QSettings;
class qCjyxSubjectHierarchySettingsPanelPrivate;

class Q_CJYX_QTMODULES_SUBJECTHIERARCHY_EXPORT qCjyxSubjectHierarchySettingsPanel
  : public ctkSettingsPanel
{
  Q_OBJECT
public:
  /// Superclass typedef
  typedef ctkSettingsPanel Superclass;

  /// Constructor
  explicit qCjyxSubjectHierarchySettingsPanel(QWidget* parent = nullptr);

  /// Destructor
  ~qCjyxSubjectHierarchySettingsPanel() override;

public slots:

protected slots:
  void setAutoDeleteSubjectHierarchyChildrenEnabled(bool on);
  void setDisplayPatientIDEnabled(bool on);
  void setDisplayPatientBirthDateEnabled(bool on);
  void setDisplayStudyIDEnabled(bool on);
  void setDisplayStudyDateEnabled(bool on);

protected:
  QScopedPointer<qCjyxSubjectHierarchySettingsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSubjectHierarchySettingsPanel);
  Q_DISABLE_COPY(qCjyxSubjectHierarchySettingsPanel);
};

#endif
