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

#ifndef __qCjyxCLIModuleWidget_h
#define __qCjyxCLIModuleWidget_h

// CTK includes
#include <ctkPimpl.h>
#include <ctkVTKObject.h>

// Cjyx includes
#include "qCjyxAbstractModuleWidget.h"

#include "qCjyxBaseQTCLIExport.h"

class ModuleDescription;
class vtkMRMLNode;
class vtkMRMLCommandLineModuleNode;
class qCjyxCLIModuleWidgetPrivate;

class Q_CJYX_BASE_QTCLI_EXPORT qCjyxCLIModuleWidget : public qCjyxAbstractModuleWidget
{
  Q_OBJECT
  QVTK_OBJECT
public:

  typedef qCjyxAbstractModuleWidget Superclass;
  qCjyxCLIModuleWidget(QWidget *parent=nullptr);
  ~qCjyxCLIModuleWidget() override;

  /// Get the current \a commandLineModuleNode
  Q_INVOKABLE vtkMRMLCommandLineModuleNode * currentCommandLineModuleNode()const;

  // Node editing
  bool setEditedNode(vtkMRMLNode* node, QString role = QString(), QString context = QString()) override;
  double nodeEditable(vtkMRMLNode* node) override;

public slots:
  /// Set the current \a commandLineModuleNode
  void setCurrentCommandLineModuleNode(vtkMRMLNode* commandLineModuleNode);

  void apply(bool wait = false);
  void cancel();
  void reset();

  void setAutoRun(bool enable);
  void setAutoRunWhenParameterChanged(bool enable);
  void setAutoRunWhenInputModified(bool enable);
  void setAutoRunOnOtherInputEvents(bool enable);
  void setAutoRunCancelsRunningProcess(bool enable);
protected:
  ///
  void setup() override;

  /// Set up the GUI from mrml when entering
  void enter() override;

  /// Run a command line module given \a parameterNode
  /// If \a waitForCompletion is True, the call will return only upon completion of
  /// the module execution.
  void run(vtkMRMLCommandLineModuleNode* parameterNode, bool waitForCompletion = false);

  /// Abort the execution of the module associated with \a node
  void cancel(vtkMRMLCommandLineModuleNode* node);
protected:
  QScopedPointer<qCjyxCLIModuleWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qCjyxCLIModuleWidget);
  Q_DISABLE_COPY(qCjyxCLIModuleWidget);
};

#endif
