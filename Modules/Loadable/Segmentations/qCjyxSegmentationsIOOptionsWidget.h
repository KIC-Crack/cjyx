/*==============================================================================

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

#ifndef __qCjyxSegmentationsIOOptionsWidget_h
#define __qCjyxSegmentationsIOOptionsWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxIOOptionsWidget.h"

#include "qCjyxSegmentationsModuleExport.h"

class qCjyxSegmentationsIOOptionsWidgetPrivate;

/// \ingroup Cjyx_QtModules_Segmentations
class Q_CJYX_QTMODULES_SEGMENTATIONS_EXPORT qCjyxSegmentationsIOOptionsWidget :
  public qCjyxIOOptionsWidget
{
  Q_OBJECT
public:
  qCjyxSegmentationsIOOptionsWidget(QWidget *parent=nullptr);
  ~qCjyxSegmentationsIOOptionsWidget() override;

protected slots:
  /// Update IO plugin properties
  void updateProperties();

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr), qCjyxSegmentationsIOOptionsWidget);
  Q_DISABLE_COPY(qCjyxSegmentationsIOOptionsWidget);
};

#endif
