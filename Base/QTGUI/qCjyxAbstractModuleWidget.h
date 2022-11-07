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

#ifndef __qCjyxAbstractModuleWidget_h
#define __qCjyxAbstractModuleWidget_h

#if defined(_MSC_VER)
#pragma warning( disable:4250 )
#endif

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxAbstractModuleRepresentation.h"
#include "qCjyxWidget.h"
class qCjyxAbstractModuleWidgetPrivate;
class vtkMRMLNode;
///
/// Base class of all the Cjyx module widgets. The widget is added in the module panels.
/// Deriving from qCjyxWidget, it inherits the mrmlScene()/setMRMLScene() methods.
class Q_CJYX_BASE_QTGUI_EXPORT qCjyxAbstractModuleWidget
  :public qCjyxWidget, public qCjyxAbstractModuleRepresentation
{
  Q_OBJECT
  Q_PROPERTY(bool isEntered READ isEntered);
public:
  /// Constructor
  /// \sa QWidget
  qCjyxAbstractModuleWidget(QWidget *parent=nullptr);
  ~qCjyxAbstractModuleWidget() override;

  /// The enter and exit methods are called when the module panel changes.
  /// These give the module a chance to do any setup or shutdown operations
  /// as it becomes active and inactive.
  /// It is the responsibility of the module's manager to call the methods.
  /// \a enter() and \a exit() must be called when reimplementing these methods
  /// in order to have \a isEntered() valid.
  Q_INVOKABLE virtual void enter();
  Q_INVOKABLE virtual void exit();
  bool isEntered()const;

  /// Node editing
  Q_INVOKABLE bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;
  Q_INVOKABLE double nodeEditable(vtkMRMLNode* node) override;

protected:
  QScopedPointer<qCjyxAbstractModuleWidgetPrivate> d_ptr;

  void setup() override;

private:
  Q_DECLARE_PRIVATE(qCjyxAbstractModuleWidget);
  Q_DISABLE_COPY(qCjyxAbstractModuleWidget);
};

#endif
