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

#ifndef __qCjyxVolumesIOOptionsWidget_h
#define __qCjyxVolumesIOOptionsWidget_h

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxIOOptionsWidget.h"

// Volumes includes
#include "qCjyxVolumesModuleExport.h"

class qCjyxVolumesIOOptionsWidgetPrivate;

/// \ingroup Cjyx_QtModules_Volumes
class Q_CJYX_QTMODULES_VOLUMES_EXPORT qCjyxVolumesIOOptionsWidget :
  public qCjyxIOOptionsWidget
{
  Q_OBJECT
public:
  qCjyxVolumesIOOptionsWidget(QWidget *parent=nullptr);
  ~qCjyxVolumesIOOptionsWidget() override;

  /// Allows custom handling of image sets as volumes
  /// \sa qCjyxVolumesReader
  /// \sa qCjyxDataDialog::addDirectory
  void updateGUI(const qCjyxIO::IOProperties& ioProperties) override;

public slots:
  void setFileName(const QString& fileName) override;
  void setFileNames(const QStringList& fileNames) override;

protected slots:
  /// Update the name, labelmap, center, singleFile, discardOrientation,
  /// colorNodeID properties
  void updateProperties();
  /// Update the color node selection to the default label map
  /// or volume color node depending on the label map checkbox state.
  void updateColorSelector();

private:
  Q_DECLARE_PRIVATE_D(qGetPtrHelper(qCjyxIOOptions::d_ptr), qCjyxVolumesIOOptionsWidget);
  Q_DISABLE_COPY(qCjyxVolumesIOOptionsWidget);
};

#endif
