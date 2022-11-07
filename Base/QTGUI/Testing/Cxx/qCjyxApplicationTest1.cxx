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
#include "qCjyxApplication.h"
#include "qCjyxCommandOptions.h"
#include "qCjyxIOManager.h"

// STD includes

int qCjyxApplicationTest1(int argc, char * argv[] )
{
  qCjyxApplication app(argc, argv);

  if (app.commandOptions() == nullptr)
    {
    std::cerr << "Problem with commandOptions()" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxCommandOptions * commandOptions = new qCjyxCommandOptions;
  app.setCoreCommandOptions(commandOptions);

  qCjyxCommandOptions * commandOptions2 = app.commandOptions();
  if (commandOptions2 != commandOptions)
    {
    std::cerr << "Problem with setCommandOptions()/commandOptions()" << std::endl;
    return EXIT_FAILURE;
    }

  if (app.ioManager() == nullptr)
    {
    std::cerr << "Problem with ioManager()" << std::endl;
    return EXIT_FAILURE;
    }

  qCjyxIOManager * ioManager = new qCjyxIOManager;
  app.setCoreIOManager(ioManager);

  qCjyxIOManager * ioManager2 = app.ioManager();
  if(ioManager2 != ioManager)
    {
    std::cerr << "Problem with setIOManager()/ioManager()" << std::endl;
    return EXIT_FAILURE;
    }

  /// Application shouldn't have ask for exit
  if (app.returnCode() != -1)
    {
    std::cerr << "Problem with the application::parseArguments function."
              << "Return code: " << app.returnCode() << std::endl;
    return EXIT_FAILURE;
    }

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QTimer::singleShot(100, qApp, SLOT(quit()));
    }

  return app.exec();
}

