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

  This file was originally developed by Kyle Sunderland, PerkLab, Queen's University
  and was supported through CANARIE's Research Software Program, and Cancer
  Care Ontario.

==============================================================================*/

#ifndef __qCjyxTextsModuleWidget_h
#define __qCjyxTextsModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

// Texts includes
#include "qCjyxTextsModuleExport.h"

class qCjyxTextsModuleWidgetPrivate;

class Q_CJYX_QTMODULES_TEXTS_EXPORT qCjyxTextsModuleWidget : public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxTextsModuleWidget(QWidget *parent=nullptr);
  ~qCjyxTextsModuleWidget() override;

  /// Support of node editing. Selects node in user interface that the user wants to edit
  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

protected:

  void setup() override;

protected:
  QScopedPointer<qCjyxTextsModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTextsModuleWidget);
  Q_DISABLE_COPY(qCjyxTextsModuleWidget);
};

#endif
