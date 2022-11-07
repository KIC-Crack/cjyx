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
#include "vtkCjyxConfigure.h" // For Cjyx_USE_PYTHONQT

// CTK includes
#ifdef Cjyx_USE_PYTHONQT
# include <ctkPythonConsole.h>
#endif

#include "qMRMLWidget.h"// CjyxApp includes
#include "qCjyxApplication.h"
#include "qCjyxAppMainWindow.h"
#ifdef Cjyx_USE_PYTHONQT
# include "qCjyxPythonManager.h"
#endif

// STD includes

int qCjyxAppMainWindowTest1(int argc, char * argv[] )
{
  qMRMLWidget::preInitializeApplication();
  qCjyxApplication app(argc, argv);
  qMRMLWidget::postInitializeApplication();

  qCjyxAppMainWindow mainWindow;
  mainWindow.show();

#ifdef Cjyx_USE_PYTHONQT
  // Create python console
  Q_ASSERT(qCjyxApplication::application()->pythonManager());
  ctkPythonConsole pythonConsole;
  pythonConsole.initialize(qCjyxApplication::application()->pythonManager());
  pythonConsole.resize(600, 280);
#endif

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    QTimer::singleShot(100, qApp, SLOT(quit()));
    }

  return app.exec();
}

