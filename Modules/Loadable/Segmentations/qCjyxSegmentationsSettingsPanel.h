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

#ifndef __qCjyxSegmentationsSettingsPanel_h
#define __qCjyxSegmentationsSettingsPanel_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkSettingsPanel.h>

#include "qCjyxSegmentationsModuleExport.h"

class QSettings;
class qCjyxSegmentationsSettingsPanelPrivate;
class vtkCjyxSegmentationsModuleLogic;

class Q_CJYX_QTMODULES_SEGMENTATIONS_EXPORT qCjyxSegmentationsSettingsPanel
  : public ctkSettingsPanel
{
  Q_OBJECT

  Q_PROPERTY(QString defaultTerminologyEntry READ defaultTerminologyEntry WRITE setDefaultTerminologyEntry)
public:
  typedef ctkSettingsPanel Superclass;

  explicit qCjyxSegmentationsSettingsPanel(QWidget* parent = nullptr);
  ~qCjyxSegmentationsSettingsPanel() override;

  /// Segmentations logic is used for configuring default settings
  void setSegmentationsLogic(vtkCjyxSegmentationsModuleLogic* logic);
  vtkCjyxSegmentationsModuleLogic* segmentationsLogic()const;

  QString defaultTerminologyEntry();

public slots:

protected slots:
  void setAutoOpacities(bool on);
  void setDefaultSurfaceSmoothing(bool on);
  void onEditDefaultTerminologyEntry();
  void setDefaultTerminologyEntry(QString);
  void updateDefaultSegmentationNodeFromWidget();

signals:
  void defaultTerminologyEntryChanged(QString terminologyStr);

protected:
  QScopedPointer<qCjyxSegmentationsSettingsPanelPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxSegmentationsSettingsPanel);
  Q_DISABLE_COPY(qCjyxSegmentationsSettingsPanel);
};

#endif
