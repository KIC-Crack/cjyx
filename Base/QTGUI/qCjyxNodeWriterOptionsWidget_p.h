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

#ifndef __qCjyxNodeWriterOptionsWidget_p_h
#define __qCjyxNodeWriterOptionsWidget_p_h

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Cjyx API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "qCjyxBaseQTGUIExport.h"
#include "qCjyxIOOptions_p.h"
#include "ui_qCjyxNodeWriterOptionsWidget.h"

//------------------------------------------------------------------------------
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxNodeWriterOptionsWidgetPrivate
  : public qCjyxIOOptionsPrivate
  , public Ui_qCjyxNodeWriterOptionsWidget
{
public:
  ~qCjyxNodeWriterOptionsWidgetPrivate() override;
  virtual void setupUi(QWidget* widget);
};

#endif
