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

// Cjyx includes
#include <qCjyxApplication.h>
#include "vtkCjyxConfigure.h"

// VolumeRendering includes
#include "qCjyxVolumeRenderingModule.h"
#include "qCjyxVolumeRenderingModuleWidget.h"

// Volumes includes
#include <vtkCjyxVolumesLogic.h>

// MRML includes
#include <vtkMRMLViewNode.h>

// VTK includes
#include <vtkSmartPointer.h>

// ITK includes
#include <itkConfigure.h>
#include <itkFactoryRegistration.h>

//-----------------------------------------------------------------------------
int qCjyxVolumeRenderingModuleWidgetTest2( int argc, char * argv[] )
{
  itk::itkFactoryRegistration();

  qCjyxApplication app(argc, argv);

  if (argc < 2)
    {
    std::cerr << "Usage: qCjyxVolumeRenderingModuleWidgetTest2 volumeName [-I]" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxVolumeRenderingModule module;
  module.setMRMLScene(app.mrmlScene());
  module.initialize(nullptr);

  qCjyxVolumeRenderingModuleWidget* moduleWidget =
    dynamic_cast<qCjyxVolumeRenderingModuleWidget*>(
      module.widgetRepresentation());

  vtkSmartPointer<vtkCjyxVolumesLogic> volumesLogic =
    vtkSmartPointer<vtkCjyxVolumesLogic>::New();
  volumesLogic->SetMRMLScene(app.mrmlScene());

  vtkMRMLVolumeNode* volumeNode = volumesLogic->AddArchetypeVolume(argv[1], "volume");
  if (!volumeNode)
    {
    std::cerr << "Bad volume file:" << argv[1] << std::endl;
    return EXIT_FAILURE;
    }

  vtkSmartPointer<vtkMRMLViewNode> view = vtkSmartPointer<vtkMRMLViewNode>::New();
  app.mrmlScene()->AddNode(view);
  vtkSmartPointer<vtkMRMLViewNode> view2 = vtkSmartPointer<vtkMRMLViewNode>::New();
  app.mrmlScene()->AddNode(view2);

  moduleWidget->show();

  // HACK, manually select the node, should be automatic
  moduleWidget->setMRMLVolumeNode(volumeNode);

  if (argc < 3 || QString(argv[2]) != "-I")
    {
    QTimer::singleShot(100, qApp, SLOT(quit()));
    }

  return app.exec();
}
