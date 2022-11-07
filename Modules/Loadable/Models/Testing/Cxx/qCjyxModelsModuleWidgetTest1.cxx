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

// Qt includes
#include <QTimer>
#include <QWidget>

// Cjyx includes
#include "vtkCjyxConfigure.h"

// Cjyx includes
#include <qCjyxAbstractModuleRepresentation.h>
#include <qCjyxApplication.h>
#include <qCjyxSubjectHierarchyAbstractPlugin.h>
#include <qCjyxSubjectHierarchyPluginLogic.h>
#include <qCjyxSubjectHierarchyPluginHandler.h>
#include <vtkCjyxColorLogic.h>
#include <vtkCjyxSubjectHierarchyModuleLogic.h>
#include <vtkCjyxTerminologiesModuleLogic.h>

// Volumes includes
#include "qCjyxModelsModule.h"
#include "vtkCjyxModelsLogic.h"

// MRML includes
#include <vtkMRMLModelHierarchyNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkNew.h>
#include "qMRMLWidget.h"

//-----------------------------------------------------------------------------
int qCjyxModelsModuleWidgetTest1( int argc, char * argv[] )
{
  qMRMLWidget::preInitializeApplication();
  qCjyxApplication app(argc, argv);
  qMRMLWidget::postInitializeApplication();

  if (argc < 2)
    {
    std::cerr << "Usage: qCjyxModelsModuleWidgetTest1 sceneFilePath [-I]"
              << std::endl;
    return EXIT_FAILURE;
    }

  vtkCjyxApplicationLogic* appLogic = app.applicationLogic();
  vtkNew<vtkMRMLScene> scene;

  // Add Color logic (used by Models logic)
  vtkNew<vtkCjyxColorLogic> colorLogic;
  colorLogic->SetMRMLScene(scene.GetPointer());
  colorLogic->SetMRMLApplicationLogic(appLogic);
  appLogic->SetModuleLogic("Colors", colorLogic);

  // Set up Models module
  qCjyxModelsModule module;
  // Set path just to avoid a runtime warning at module initialization
  module.setPath(app.cjyxHome() + '/' + app.cjyxSharePath() + "/qt-loadable-modules/Models");
  module.setMRMLScene(scene.GetPointer());
  module.initialize(appLogic);
  vtkCjyxModelsLogic* modelsLogic = vtkCjyxModelsLogic::SafeDownCast(module.logic());

  // Set up Terminologies logic (needed for subject hierarchy tree view color/terminology selector)
  vtkNew<vtkCjyxTerminologiesModuleLogic> terminologiesLogic;
  QString terminologiesSharePath = app.cjyxHome() + '/' + app.cjyxSharePath() + "/qt-loadable-modules/Terminologies";
  terminologiesLogic->SetModuleShareDirectory(terminologiesSharePath.toStdString());
  terminologiesLogic->SetMRMLScene(scene.GetPointer());
  terminologiesLogic->SetMRMLApplicationLogic(appLogic);
  appLogic->SetModuleLogic("Terminologies", terminologiesLogic);

  // Set up Subject Hierarchy logic (needed for the subject hierarchy tree view)
  vtkNew<vtkCjyxSubjectHierarchyModuleLogic> shModuleLogic;
  shModuleLogic->SetMRMLScene(scene);
  QScopedPointer<qCjyxSubjectHierarchyPluginLogic> pluginLogic(new qCjyxSubjectHierarchyPluginLogic());
  pluginLogic->setMRMLScene(scene);
  qCjyxSubjectHierarchyPluginHandler::instance()->setPluginLogic(pluginLogic.data());
  qCjyxSubjectHierarchyPluginHandler::instance()->setMRMLScene(scene);

  // Add a model node
  vtkMRMLModelNode* modelNode = modelsLogic->AddModel(argv[1]);
  if (!modelNode)
    {
    std::cerr << "Bad model file:" << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  // Create a few folders
  vtkMRMLSubjectHierarchyNode* shNode = vtkMRMLSubjectHierarchyNode::GetSubjectHierarchyNode(scene);
  vtkIdType folderA = shNode->CreateFolderItem(shNode->GetSceneItemID(), "Folder A");
  vtkIdType folderB = shNode->CreateFolderItem(shNode->GetSceneItemID(), "Folder B");
  vtkIdType folderC = shNode->CreateFolderItem(shNode->GetSceneItemID(), "Folder C");
  shNode->SetItemParent(folderC, folderB);

  // Add more model nodes
  vtkMRMLModelNode* modelNode2 = modelsLogic->AddModel(argv[1]);
  shNode->SetItemParent(shNode->GetItemByDataNode(modelNode2), folderC);

  // Show module GUI
  dynamic_cast<QWidget*>(module.widgetRepresentation())->show();

  // Add some more model nodes
  modelsLogic->AddModel(argv[1]);
  shNode->SetItemParent(shNode->GetItemByDataNode(modelNode2), folderA);

  modelsLogic->AddModel(argv[1]);

  if (argc < 3 || QString(argv[2]) != "-I")
    {
    QTimer::singleShot(200, &app, SLOT(quit()));
    }
  return app.exec();
}
