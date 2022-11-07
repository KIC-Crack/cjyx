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
#ifndef __qMRMLMarkupsToolBar_p_h
#define __qMRMLMarkupsToolBar_p_h

// Qt includes
#include <QDebug>
#include <QToolButton>
#include <QMenu>
#include <QCheckBox>

// CTK includes
#include <ctkPimpl.h>
#include <ctkSignalMapper.h>
#include <ctkVTKObject.h>
#include "qCjyxBaseQTGUIExport.h"

// Cjyx includes
#include "qCjyxCoreApplication.h"
#include "qCjyxApplication.h"
#include "qCjyxLayoutManager.h"

// MRML includes
#include "qMRMLNodeComboBox.h"
#include "qMRMLThreeDView.h"
#include "qMRMLThreeDWidget.h"
#include "qMRMLSliceView.h"
#include "qMRMLSliceWidget.h"
#include <vtkMRMLScene.h>
#include <vtkMRMLInteractionNode.h>
#include <vtkMRMLLayoutLogic.h>
#include <vtkMRMLSelectionNode.h>
#include <vtkMRMLSliceNode.h>
#include <vtkMRMLViewNode.h>
#include <vtkMRMLWindowLevelWidget.h>
#include <qMRMLMarkupsToolBar.h>
#include <qCjyxMarkupsPlaceWidget.h>

//Logic includes
#include <vtkCjyxApplicationLogic.h>
#include <vtkMRMLApplicationLogic.h>
#include <vtkCjyxMarkupsLogic.h>

// VTK includes
#include <vtkWeakPointer.h>
#include <vtkSmartPointer.h>

#include "qCjyxMouseModeToolBar_p.h"

class qMRMLMarkupsToolBarPrivate;
class QAction;
class QActionGroup;
class QToolButton;

//-----------------------------------------------------------------------------
class qMRMLMarkupsToolBarPrivate : public QObject
{
  Q_OBJECT
    QVTK_OBJECT
    Q_DECLARE_PUBLIC(qMRMLMarkupsToolBar);

protected:
  qMRMLMarkupsToolBar* const q_ptr;

public:
  qMRMLMarkupsToolBarPrivate(qMRMLMarkupsToolBar& object);
  void init();
  void setMRMLScene(vtkMRMLScene* newScene);
  QCursor cursorFromIcon(QIcon& icon);
  void addSetModuleButton(vtkCjyxMarkupsLogic* markupsLogic, const QString& moduleName);

public slots:
  void onMRMLSceneStartBatchProcess();
  void onMRMLSceneEndBatchProcess();
  void updateWidgetFromMRML();
  void onActivePlaceNodeClassNameChangedEvent();
  void onPlaceNodeClassNameListModifiedEvent();
  void onSetModule(const QString& moduleName);

public:
  vtkSmartPointer<vtkMRMLScene>            MRMLScene;
  vtkSmartPointer<vtkMRMLApplicationLogic> MRMLAppLogic;
  vtkWeakPointer<vtkMRMLInteractionNode>   InteractionNode;
  vtkWeakPointer<vtkMRMLSelectionNode>     SelectionNode;

  QString DefaultPlaceClassName;
  QAction* NodeSelectorAction{nullptr};
  qMRMLNodeComboBox* MarkupsNodeSelector{nullptr};
  qCjyxMarkupsPlaceWidget* MarkupsPlaceWidget{nullptr};
};

#endif