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

// Qt includes
#include <QPushButton>
#include <QTemporaryFile>
#include <QTimer>

// CTK includes
#include <ctkCallback.h>

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxCLIExecutableModuleFactory.h"
#include "qCjyxScriptedLoadableModuleFactory.h"
#include "qCjyxCLIModule.h"
#include "qCjyxCLIModuleWidget.h"
#include "qCjyxModuleFactoryManager.h"
#include "qCjyxModuleManager.h"

// MRMLCLI includes
#include <vtkMRMLCommandLineModuleNode.h>
#include <vtkCjyxCLIModuleLogic.h>

// STD includes

namespace
{
//-----------------------------------------------------------------------------
qCjyxCLIModule * CLIModule;
QString            ErrorString;

//-----------------------------------------------------------------------------
void runCli(void * data)
{
  Q_ASSERT(CLIModule);
  Q_UNUSED(data);

  QTemporaryFile outputFile("qCjyxCLIModuleTest1-outputFile-XXXXXX");
  if (!outputFile.open())
    {
    ErrorString = "Failed to create temporary file";
    return;
    }
  //outputFile.close();

  // Create node
  vtkMRMLCommandLineModuleNode * cliModuleNode =
    CLIModule->cliModuleLogic()->CreateNodeInScene();

  // Values
  int inputValue1 = 4;
  int inputValue2 = 3;

  // Set node parameters
  cliModuleNode->SetParameterAsInt("InputValue1", inputValue1);
  cliModuleNode->SetParameterAsInt("InputValue2", inputValue2);
  cliModuleNode->SetParameterAsString("OperationType", "Addition");
  cliModuleNode->SetParameterAsString("OutputFile", outputFile.fileName().toStdString());

  // Execute synchronously so that we can check the content of the file after the module execution
  CLIModule->cliModuleLogic()->ApplyAndWait(cliModuleNode);

  // Read outputFile
  QTextStream stream(&outputFile);
  QString operationResult = stream.readAll().trimmed();

  QString expectedResult = QString::number(inputValue1 + inputValue2);
  if (operationResult.compare(expectedResult) != 0)
    {
    ErrorString = QString("OutputFile doesn't contain the expected result !\n"
                          "\tExpected:%1\n"
                          "\tCurrent:%2").arg(expectedResult).arg(operationResult);
    return;
    }

  outputFile.close();
}

} // end anonymous namespace

//-----------------------------------------------------------------------------
int qCjyxPyCLIModuleTest1(int argc, char * argv[])
{
  // The PyCLI4Test module (PyCLIModule4Test) has already been installed as
  // a normal Python CLI module.
  // Cjyx-build/lib/Cjyx-X.Y/cli-modules[/Debug|Release]
  QString cliModuleName("PyCLI4Test");

  qCjyxApplication::setAttribute(qCjyxApplication::AA_DisablePython);
  qCjyxApplication app(argc, argv);

  qCjyxModuleManager * moduleManager = app.moduleManager();
  if (!moduleManager)
    {
    std::cerr << "Line " << __LINE__
              << " - Problem with qCjyxApplication::moduleManager()" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxModuleFactoryManager* moduleFactoryManager = moduleManager->factoryManager();
  if (!moduleFactoryManager)
    {
    std::cerr << "Line " << __LINE__
              << " - Problem with qCjyxModuleManager::factoryManager()" << std::endl;
    return EXIT_FAILURE;
    }

  QString cliPath = app.cjyxHome() + "/" + Cjyx_CLIMODULES_LIB_DIR + "/";
  QStringList loadPaths = { cliPath, cliPath + app.intDir() };

  //===========================================================================
  // Ensure that PyCLI is not recognized by the scripted factory.
  moduleFactoryManager->registerFactory(new qCjyxScriptedLoadableModuleFactory);
  moduleFactoryManager->addSearchPaths(loadPaths);
  moduleFactoryManager->registerModules();
  moduleFactoryManager->instantiateModules();

  QStringList moduleNames = moduleFactoryManager->instantiatedModuleNames();
  if (moduleNames.contains(cliModuleName))
    {
    std::cerr << "Line " << __LINE__ << " - Problem with qCjyxScriptedLoadableModuleFactory"
              << " - Improperly registered '" << qPrintable(cliModuleName) << "' as scripted module" << std::endl;
    return EXIT_FAILURE;
    }

  moduleFactoryManager->unregisterFactories();


  //===========================================================================
  // Now test PyCLI for real
  moduleFactoryManager->registerFactory(new qCjyxCLIExecutableModuleFactory);
  moduleFactoryManager->addSearchPaths(loadPaths);

  // Register and instantiate modules
  moduleFactoryManager->registerModules();
  moduleFactoryManager->instantiateModules();

  moduleNames = moduleFactoryManager->instantiatedModuleNames();
  if (!moduleNames.contains(cliModuleName))
    {
    std::cerr << "Line " << __LINE__ << " - Problem with qCjyxCLIExecutableModuleFactory"
              << " - Failed to register '" << qPrintable(cliModuleName) << "' module" << std::endl;
    return EXIT_FAILURE;
    }

  foreach(const QString& name, moduleNames)
    {
    moduleFactoryManager->loadModule(name);
    }

  qCjyxAbstractCoreModule * module = moduleManager->module(cliModuleName);
  if (!module)
    {
    std::cerr << "Line " << __LINE__
              << " - Problem with qCjyxModuleManager::module()"
              << " - Failed to retrieve module named '" << qPrintable(cliModuleName) << "'" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxCLIModule * cliModule = qobject_cast<qCjyxCLIModule*>(module);
  if (!cliModule)
    {
    std::cerr << "Line " << __LINE__
              << " - Failed to cast module named '" << qPrintable(cliModuleName) << "' "
              << "from [qCjyxAbstractCoreModule*] into [qCjyxCLIModule*]" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxAbstractModuleRepresentation * widgetRepresentation = cliModule->widgetRepresentation();
  if (!widgetRepresentation)
    {
    std::cerr << "Line " << __LINE__
              << " - Problem with qCjyxCLIModule::widgetRepresentation()"
              << " - Failed to retrieve representation associated with module named '"
              << qPrintable(cliModuleName) << "'" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxCLIModuleWidget* cliWidget =
    dynamic_cast<qCjyxCLIModuleWidget*>(widgetRepresentation);
  if (!cliWidget)
    {
    std::cerr << "Line " << __LINE__
              << " - Failed to cast module '" << qPrintable(cliModuleName) << "' representation "
              << "from [qCjyxAbstractModuleRepresentation*] into [qCjyxCLIModuleWidget*]" << std::endl;
    return EXIT_FAILURE;
    }

  cliWidget->show();

  QPushButton button("Simulate CLI programmatic start");
  CLIModule = cliModule;
  ctkCallback callback(runCli);
  QObject::connect(&button, SIGNAL(clicked()), &callback, SLOT(invoke()));

  button.show();

  QTimer::singleShot(0, &callback, SLOT(invoke()));

  bool checkResult = false;
  if (argc < 2 || QString(argv[1]) != "-I" )
    {
    QTimer::singleShot(500, &app, SLOT(quit()));
    checkResult = true;
    }

  int status = app.exec();
  if (status == EXIT_FAILURE)
    {
    std::cerr << "Line " << __LINE__ << " - Problem with qCjyxApplication::exec()";
    return EXIT_FAILURE;
    }

  if (checkResult && !ErrorString.isEmpty())
    {
    std::cerr << "Line " << __LINE__ << " - Problem executing command line module - "
              << qPrintable(ErrorString) << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
