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

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#ifndef __qCjyxTransformsModuleWidget_h
#define __qCjyxTransformsModuleWidget_h

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

// Transforms includes
#include "qCjyxTransformsModuleExport.h"

class vtkMatrix4x4;
class vtkMRMLNode;
class qCjyxTransformsModuleWidgetPrivate;

class Q_CJYX_QTMODULES_TRANSFORMS_EXPORT qCjyxTransformsModuleWidget :
  public qCjyxAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxTransformsModuleWidget(QWidget *parent=nullptr);
  ~qCjyxTransformsModuleWidget() override;

  /// Reimplemented for internal reasons
  void setMRMLScene(vtkMRMLScene* scene) override;

  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;

public slots:

  /// Set the transform to identity. Only for linear transforms.
  /// The sliders are reset to the position 0.
  void identity();

  /// Invert the transform.
  void invert();

  /// Split composite transform to its components
  void split();

protected:

  void setup() override;

protected slots:

  void onTranslateFirstButtonPressed(bool checked);
  void onNodeSelected(vtkMRMLNode* node);
  void onMRMLTransformNodeModified(vtkObject* caller);

  void copyTransform();
  void pasteTransform();

  void transformSelectedNodes();
  void untransformSelectedNodes();
  void hardenSelectedNodes();

  void onDisplaySectionClicked(bool);
  void onTransformableSectionClicked(bool);

  void updateConvertButtonState();
  void convert();

protected:
  ///
  /// Convenient method to return the coordinate system currently selected
  int coordinateReference()const;

protected:
  QScopedPointer<qCjyxTransformsModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxTransformsModuleWidget);
  Q_DISABLE_COPY(qCjyxTransformsModuleWidget);
};

#endif
