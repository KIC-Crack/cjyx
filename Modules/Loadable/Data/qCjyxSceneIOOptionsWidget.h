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

#ifndef __qCjyxSceneIOOptionsWidget_h
#define __qCjyxSceneIOOptionsWidget_h

// Cjyx includes
#include "qCjyxIOOptionsWidget.h"

// Cameras includes
#include "qCjyxDataModuleExport.h"

class qCjyxSceneIOOptionsWidgetPrivate;

/// \ingroup Cjyx_QtModules_Scene
class Q_CJYX_QTMODULES_DATA_EXPORT qCjyxSceneIOOptionsWidget
  : public qCjyxIOOptionsWidget
{
  Q_OBJECT
public:
  qCjyxSceneIOOptionsWidget(QWidget *parent=nullptr);
  ~qCjyxSceneIOOptionsWidget() override;

  // Update checkboxes in the widget based on
  // "clear" and "copyCameras" Boolean properties.
  void updateGUI(const qCjyxIO::IOProperties& ioProperties) override;

protected slots:
  void updateProperties();

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr),
                      qCjyxSceneIOOptionsWidget);
  Q_DISABLE_COPY(qCjyxSceneIOOptionsWidget);
};

#endif
