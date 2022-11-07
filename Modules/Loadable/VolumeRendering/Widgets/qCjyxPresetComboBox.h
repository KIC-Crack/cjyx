/*==============================================================================

  Program: 3D Cjyx

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Julien Finet, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxPresetComboBox_h
#define __qCjyxPresetComboBox_h

// MRMLWidgets includes
#include <qMRMLNodeComboBox.h>

// Cjyx includes
#include "qCjyxVolumeRenderingModuleWidgetsExport.h"

class vtkMRMLNode;
class qCjyxPresetComboBoxPrivate;

class Q_CJYX_MODULE_VOLUMERENDERING_WIDGETS_EXPORT qCjyxPresetComboBox
  : public qMRMLNodeComboBox
{
  Q_OBJECT
  Q_PROPERTY(bool showIcons READ showIcons WRITE setShowIcons)

public:
  /// Constructors
  typedef qMRMLNodeComboBox Superclass;
  explicit qCjyxPresetComboBox(QWidget* parent=nullptr);
  ~qCjyxPresetComboBox() override;

  bool showIcons()const;
  void setShowIcons(bool show);

public slots:
  void setIconToPreset(vtkMRMLNode* presetNode);

protected slots:
  void updateComboBoxTitleAndIcon(vtkMRMLNode* presetNode);

protected:
  QScopedPointer<qCjyxPresetComboBoxPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxPresetComboBox);
  Q_DISABLE_COPY(qCjyxPresetComboBox);
};

#endif
