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

#ifndef __qCjyxDTISliceDisplayWidget_h
#define __qCjyxDTISliceDisplayWidget_h

// Qt includes

// CTK includes
#include <ctkVTKObject.h>

// Cjyx includes
#include <qCjyxWidget.h>
#include "qCjyxVolumesModuleWidgetsExport.h"

class vtkMRMLNode;
class vtkMRMLDiffusionTensorVolumeSliceDisplayNode;
class vtkMRMLDiffusionTensorDisplayPropertiesNode;
class qCjyxDTISliceDisplayWidgetPrivate;

class Q_CJYX_QTMODULES_VOLUMES_WIDGETS_EXPORT qCjyxDTISliceDisplayWidget
  : public qCjyxWidget
{
  Q_OBJECT
  QVTK_OBJECT
  Q_PROPERTY(bool visibilityHidden READ isVisibilityHidden WRITE setVisibilityHidden )

public:
  /// Constructors
  typedef qCjyxWidget Superclass;
  explicit qCjyxDTISliceDisplayWidget(QWidget* parent=nullptr);
  ~qCjyxDTISliceDisplayWidget() override;

  vtkMRMLDiffusionTensorVolumeSliceDisplayNode* displayNode()const;
  vtkMRMLDiffusionTensorDisplayPropertiesNode* displayPropertiesNode()const;

  /// True by default
  bool isVisibilityHidden()const;
  void setVisibilityHidden(bool hide);

public slots:
  /// Set the MRML node of interest
  void setMRMLDTISliceDisplayNode(vtkMRMLDiffusionTensorVolumeSliceDisplayNode* displayNode);
  /// Utility function to easily connect signals/slots
  void setMRMLDTISliceDisplayNode(vtkMRMLNode* displayNode);

  void setColorGlyphBy(int);
  void setColorMap(vtkMRMLNode* colorNode);
  void setOpacity(double);
  void setVisibility(bool);
  void setManualScalarRange(bool);
  void setScalarRange(double, double);
  void setGlyphGeometry(int);
  void setGlyphScaleFactor(double);
  void setGlyphSpacing(double);
  void setGlyphEigenVector(int);

protected slots:
  void updateWidgetFromMRML();

protected:
  QScopedPointer<qCjyxDTISliceDisplayWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxDTISliceDisplayWidget);
  Q_DISABLE_COPY(qCjyxDTISliceDisplayWidget);
};

#endif
