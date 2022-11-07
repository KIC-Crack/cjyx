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
#include <QApplication>

// Cjyx includes
#include "vtkCjyxConfigure.h"

// Cjyx includes
#include "qCjyxWidget.h"

// MRML includes
#include <vtkMRMLScene.h>

// VTK includes
#include "qMRMLWidget.h"

// STD includes

int qCjyxWidgetTest1(int argc, char * argv[] )
{
  qMRMLWidget::preInitializeApplication();
  QApplication app(argc, argv);
  qMRMLWidget::postInitializeApplication();
  qCjyxWidget widget;
  if (widget.mrmlScene() != nullptr)
    {
    std::cerr << "scene incorrectly initialized." << std::endl;
    return EXIT_FAILURE;
    }
  // check for infinite loop
  QObject::connect(&widget, SIGNAL(mrmlSceneChanged(vtkMRMLScene*)),
                   &widget, SLOT(setMRMLScene(vtkMRMLScene*)));
  vtkMRMLScene* scene = vtkMRMLScene::New();
  widget.setMRMLScene(scene);
  if (widget.mrmlScene() != scene)
    {
    std::cerr << "scene incorrectly set." << std::endl;
    return EXIT_FAILURE;
    }
  scene->Delete();
  if (widget.mrmlScene() != scene)
    {
    std::cerr << "scene has been deleted, qCjyxWidget is supposed to keep a ref on it." << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}

