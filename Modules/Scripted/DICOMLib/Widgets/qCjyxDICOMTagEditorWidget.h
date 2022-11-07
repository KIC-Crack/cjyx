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

#ifndef __qCjyxDICOMTagEditorWidget_h
#define __qCjyxDICOMTagEditorWidget_h

// Qt includes
#include "QWidget"

// DICOMLib includes
#include "qCjyxDICOMLibModuleWidgetsExport.h"

class qCjyxDICOMExportable;
class qCjyxDICOMTagEditorWidgetPrivate;
class vtkMRMLScene;

/// Widget for showing and editing pseudo-tags for series to export. Pseudo-tag is a
/// pair of strings (name, value) that DICOM plugins use to set values of real DICOM tags in
/// the exported data set.
/// \ingroup Cjyx_QtModules_SubjectHierarchy
class Q_CJYX_MODULE_DICOMLIB_WIDGETS_EXPORT qCjyxDICOMTagEditorWidget : public QWidget
{
  Q_OBJECT

public:
  typedef QWidget Superclass;
  qCjyxDICOMTagEditorWidget(QWidget *parent=nullptr);
  ~qCjyxDICOMTagEditorWidget() override;

public:
  /// Set exportables to show the series pseudo-tags to edit.
  /// Creates and populates tables.
  /// \param exportables List of exportable objects
  /// \return Empty string if input is valid, error message if
  ///   invalid (e.g. when the nodes referenced by exportables are
  /// in different study)
  QString setExportables(QList<qCjyxDICOMExportable*> exportables);

  /// Get exportable list
  QList<qCjyxDICOMExportable*> exportables()const;

  /// Clear exportables and tables
  void clear();

  /// Write edited tags into the subject hierarchy item attributes
  void commitChangesToItems();

signals:
  /// Signal emitted each time a tag was edited by the user
  void tagEdited();

public slots:
  /// Set MRML scene
  virtual void setMRMLScene(vtkMRMLScene* scene);

  /// Sets new tag value in corresponding exportable
  void tagsTableCellChanged(int row, int column);

protected:
  QScopedPointer<qCjyxDICOMTagEditorWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDICOMTagEditorWidget);
  Q_DISABLE_COPY(qCjyxDICOMTagEditorWidget);
};

#endif
