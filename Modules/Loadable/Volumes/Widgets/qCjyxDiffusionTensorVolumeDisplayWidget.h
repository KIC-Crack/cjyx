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

#ifndef __qCjyxDiffusionTensorVolumeDisplayWidget_h
#define __qCjyxDiffusionTensorVolumeDisplayWidget_h

// Qt includes
#include <QWidget>

// CTK includes
#include <ctkVTKObject.h>

// Cjyx includes
#include <qCjyxWidget.h>

#include "qCjyxVolumesModuleWidgetsExport.h"

class vtkMRMLNode;
class vtkMRMLDiffusionTensorVolumeDisplayNode;
class vtkMRMLDiffusionTensorVolumeNode;
class vtkMRMLDiffusionTensorSliceDisplayNode;
class vtkMRMLGlyphableVolumeSliceDisplayNode;
class qCjyxDiffusionTensorVolumeDisplayWidgetPrivate;

/// \ingroup Cjyx_QtModules_Volumes
class Q_CJYX_QTMODULES_VOLUMES_WIDGETS_EXPORT qCjyxDiffusionTensorVolumeDisplayWidget : public qCjyxWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:
  /// Constructors
  typedef qCjyxWidget Superclass;
  explicit qCjyxDiffusionTensorVolumeDisplayWidget(QWidget* parent = nullptr);
  ~qCjyxDiffusionTensorVolumeDisplayWidget() override;

  vtkMRMLDiffusionTensorVolumeNode* volumeNode()const;
  vtkMRMLDiffusionTensorVolumeDisplayNode* volumeDisplayNode()const;
  QList<vtkMRMLGlyphableVolumeSliceDisplayNode*> sliceDisplayNodes()const;
public slots:

  /// Set the MRML node of interest
  void setMRMLVolumeNode(vtkMRMLDiffusionTensorVolumeNode* volumeNode);
  void setMRMLVolumeNode(vtkMRMLNode* node);

  void setVolumeScalarInvariant(int scalarInvariant);
  void setRedSliceVisible(bool visible);
  void setYellowSliceVisible(bool visible);
  void setGreenSliceVisible(bool visible);
protected slots:
  void updateWidgetFromMRML();
  void synchronizeSliceDisplayNodes();

protected:
  QScopedPointer<qCjyxDiffusionTensorVolumeDisplayWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDiffusionTensorVolumeDisplayWidget);
  Q_DISABLE_COPY(qCjyxDiffusionTensorVolumeDisplayWidget);
};

#endif
