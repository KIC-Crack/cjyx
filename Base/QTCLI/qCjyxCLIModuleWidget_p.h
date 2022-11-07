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

#ifndef __qCjyxCLIModuleWidget_p_h
#define __qCjyxCLIModuleWidget_p_h

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

// Qt includes
#include <QHash>
#include <QList>
class QAction;

// VTK includes
#include <vtkWeakPointer.h>

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxCLIModuleWidget.h"
#include "ui_qCjyxCLIModuleWidget.h"
#include "qCjyxCLIModuleWidget.h"
#include "qCjyxWidget.h"

// ModuleDescriptionParser includes
#include <ModuleDescription.h>

// STD includes
#include <vector>

class QBoxLayout;
class QFormLayout;
class qCjyxCLIModule;
class vtkMRMLCommandLineModuleNode;
class vtkCjyxCLIModuleLogic;

//-----------------------------------------------------------------------------
class qCjyxCLIModuleUIHelper;

//-----------------------------------------------------------------------------
class qCjyxCLIModuleWidgetPrivate: public QObject,
                                     public Ui_qCjyxCLIModuleWidget
{
  Q_OBJECT
  Q_DECLARE_PUBLIC(qCjyxCLIModuleWidget);
protected:
  qCjyxCLIModuleWidget* const q_ptr;
public:
  typedef qCjyxCLIModuleWidgetPrivate Self;
  qCjyxCLIModuleWidgetPrivate(qCjyxCLIModuleWidget& object);

  ///
  /// Convenient function to cast vtkCjyxLogic into vtkCjyxCLIModuleLogic
  vtkCjyxCLIModuleLogic* logic()const;

  ///
  /// Convenient function to cast vtkMRMLNode into vtkMRMLCommandLineModuleNode
  vtkMRMLCommandLineModuleNode* commandLineModuleNode()const;

  /// Convenient method to cast qCjyxAbstractModule into qCjyxCLIModule
  qCjyxCLIModule * module()const;


  typedef std::vector<ModuleParameterGroup>::const_iterator ParameterGroupConstIterator;
  typedef std::vector<ModuleParameterGroup>::iterator       ParameterGroupIterator;

  typedef std::vector<ModuleParameter>::const_iterator ParameterConstIterator;
  typedef std::vector<ModuleParameter>::iterator       ParameterIterator;


  ///
  /// Calling this method will loop through the structure resulting
  /// from the XML parsing and generate the corresponding UI.
  virtual void setupUi(qCjyxWidget* widget);

  ///
  void addParameterGroups();
  void addParameterGroup(QBoxLayout* layout,
                         const ModuleParameterGroup& parameterGroup);

  ///
  void addParameters(QFormLayout* layout, const ModuleParameterGroup& parameterGroup);
  void addParameter(QFormLayout* layout, const ModuleParameter& moduleParameter);

public slots:

  /// Update the ui base on the command line module node
  void updateUiFromCommandLineModuleNode(vtkObject* commandLineModuleNode);
  void updateCommandLineModuleNodeFromUi(vtkObject* commandLineModuleNode);

  void setDefaultNodeValue(vtkMRMLNode* commandLineModuleNode);
  void onValueChanged(const QString& name, const QVariant& type);

public:
  qCjyxCLIModuleUIHelper* CLIModuleUIHelper;

  vtkWeakPointer<vtkMRMLCommandLineModuleNode> CommandLineModuleNode;
  QAction* AutoRunWhenParameterChanged;
  QAction* AutoRunWhenInputModified;
  QAction* AutoRunOnOtherInputEvents;
  QAction* AutoRunCancelsRunningProcess;
};


#endif
