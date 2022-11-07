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
#include "qCjyxModulePanel.h"

// STD includes

int qCjyxModulePanelTest1(int argc, char * argv[] )
{
  qCjyxApplication app(argc, argv);

  qCjyxModulePanel modulePanel;

  // helpAndAcknowledgmentVisible property
  {
    bool expected = true;
    bool current = modulePanel.isHelpAndAcknowledgmentVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with helpAndAcknowledgmentVisible property !\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }

  modulePanel.setHelpAndAcknowledgmentVisible(false);

  {
    bool expected = false;
    bool current = modulePanel.isHelpAndAcknowledgmentVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with helpAndAcknowledgmentVisible property !\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }

  modulePanel.setModuleManager(app.moduleManager());
  if (modulePanel.moduleManager() != app.moduleManager())
    {
    std::cerr << "qCjyxModulePanel::setModuleManager() failed" << std::endl;
    return EXIT_FAILURE;
    }

  modulePanel.show();

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QTimer::singleShot(100, qApp, SLOT(quit()));
    }

  return app.exec();
}

