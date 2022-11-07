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
#include <QDockWidget>
#include <QTimer>

// Cjyx includes
#include <qCjyxApplication.h>
#include <qCjyxCoreModuleFactory.h>
#include <qCjyxModuleManager.h>
#include <qCjyxModuleFactoryManager.h>
#include <qCjyxModulePanel.h>
#include <qCjyxStyle.h>

// STD includes

int qCjyxModulePanelTest2(int argc, char * argv[] )
{
  QApplication::setDesktopSettingsAware(false);
  QApplication::setStyle(new qCjyxStyle);
  qCjyxApplication app(argc, argv);

  // Register core module factories for testing purpose
  qCjyxCoreModuleFactory* coreModuleFactory = new qCjyxCoreModuleFactory();
  app.moduleManager()->factoryManager()->registerFactory(coreModuleFactory);
  app.moduleManager()->factoryManager()->registerModules();
  app.moduleManager()->factoryManager()->instantiateModules();
  app.moduleManager()->factoryManager()->loadModules();

  QDockWidget dockWidget;
  qCjyxModulePanel* modulePanel = new qCjyxModulePanel(&dockWidget);
  dockWidget.setWidget(modulePanel);
  //QHBoxLayout* hbox = new QHBoxLayout;
  //hbox->addWidget(&modulePanel);
  //dockWidget.setLayout(hbox);

  modulePanel->setModuleManager(app.moduleManager());

  modulePanel->setModule("EventBroker");
  if (modulePanel->currentModuleName() != "EventBroker")
    {
    std::cerr<< "qCjyxModulePanel::setModule() failed "
             << qPrintable(modulePanel->currentModuleName()) << std::endl;
    return EXIT_FAILURE;
    }

  dockWidget.show();

  // helpAndAcknowledgmentVisible property
  {
    QWidget* helpCollapsibleButton =
        modulePanel->findChild<QWidget*>("HelpCollapsibleButton");
    if (!helpCollapsibleButton)
      {
      std::cerr << "Line " << __LINE__ << " - Couldn't find 'HelpCollapsibleButton' widget !\n" << std::endl;
      return EXIT_FAILURE;
      }

    bool expected = false;
    modulePanel->setHelpAndAcknowledgmentVisible(expected);
    bool current = modulePanel->isHelpAndAcknowledgmentVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with helpAndAcknowledgmentVisible property !\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }

    current = helpCollapsibleButton->isVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with HelpCollapsibleButton visibility!\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }

    expected = true;
    modulePanel->setHelpAndAcknowledgmentVisible(expected);
    current = modulePanel->isHelpAndAcknowledgmentVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with helpAndAcknowledgmentVisible property !\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }

    current = helpCollapsibleButton->isVisible();
    if (current != expected)
      {
      std::cerr << "Line " << __LINE__ << " - Problem with HelpCollapsibleButton visibility!\n"
                << " current:" << current << "\n"
                << " expected:" << expected << std::endl;
      return EXIT_FAILURE;
      }
  }

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    modulePanel->setModule("");
    QTimer::singleShot(100, qApp, SLOT(quit()));
    }

  return app.exec();
}

