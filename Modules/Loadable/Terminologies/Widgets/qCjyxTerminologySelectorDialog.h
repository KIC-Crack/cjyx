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

#ifndef __qCjyxTerminologySelectorDialog_h
#define __qCjyxTerminologySelectorDialog_h

// Qt includes
#include <QObject>

// Terminologies includes
#include "qCjyxTerminologiesModuleWidgetsExport.h"

#include "qCjyxTerminologyNavigatorWidget.h"

class qCjyxTerminologySelectorDialogPrivate;
class vtkCjyxTerminologyEntry;

/// \brief Qt dialog for selecting a terminology entry
/// \ingroup CjyxRt_QtModules_Terminologies_Widgets
class Q_CJYX_MODULE_TERMINOLOGIES_WIDGETS_EXPORT qCjyxTerminologySelectorDialog : public QObject
{
public:
  Q_OBJECT
  Q_PROPERTY(bool overrideSectionVisible READ overrideSectionVisible WRITE setOverrideSectionVisible)

public:
  typedef QObject Superclass;
  qCjyxTerminologySelectorDialog(QObject* parent = nullptr);
#ifndef __VTK_WRAP__
  qCjyxTerminologySelectorDialog(
    qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &initialTerminologyInfo, QObject* parent = nullptr );
#endif
  ~qCjyxTerminologySelectorDialog() override;

public:
#ifndef __VTK_WRAP__
  /// Convenience function to start dialog, initialized with a terminology entry
  /// \param terminology Initial terminology shown by the dialog. The selected terminology is set to this as well.
  /// \param name Initial name shown by the dialog. Selected name (only if custom) is set to this as well after closing
  /// \param color Initial color shown by the dialog. Selected color (only if custom) is set to this as well after closing
  /// \return Success flag
  static bool getTerminology(
    qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo, QObject* parent );

  /// Get selected terminology and other metadata (name, color, auto-generated flags) into given info bundle object
  void terminologyInfo(qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo);
#endif

  /// Convenience function to start dialog, initialized with a terminology entry.
  /// Does not show name and color overrides.
  /// \param terminology Initial terminology shown by the dialog. The selected terminology is set to this as well.
  /// \return Success flag
  static bool getTerminology(vtkCjyxTerminologyEntry* terminologyEntry, QObject* parent);

  /// Get whether name and color override section is visible
  bool overrideSectionVisible() const;

  /// Show dialog
  /// \param nodeToSelect Node is selected in the tree if given
  /// \return Success flag (if dialog result is not Accepted then false)
  virtual bool exec();

  /// Python compatibility function for showing dialog (calls \a exec)
  Q_INVOKABLE bool execDialog() { return this->exec(); };

protected slots:
  void setSelectButtonEnabled(bool);

  /// Show/hide name and color override section
  void setOverrideSectionVisible(bool);

protected:
  QScopedPointer<qCjyxTerminologySelectorDialogPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTerminologySelectorDialog);
  Q_DISABLE_COPY(qCjyxTerminologySelectorDialog);
};

#endif
