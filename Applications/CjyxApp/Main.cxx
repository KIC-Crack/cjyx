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

// Cjyx includes
#include "qCjyxApplication.h"
#include "qCjyxApplicationHelper.h"
#include "qCjyxStyle.h"

// CjyxApp includes
#include "qCjyxAppMainWindow.h"

namespace
{

//----------------------------------------------------------------------------
int CjyxAppMain(int argc, char* argv[])
{
  typedef qCjyxAppMainWindow CjyxMainWindowType;
  typedef qCjyxStyle CjyxAppStyle;

  qCjyxApplicationHelper::preInitializeApplication(argv[0], new CjyxAppStyle);

  qCjyxApplication app(argc, argv);
  if (app.returnCode() != -1)
    {
    return app.returnCode();
    }

  QScopedPointer<CjyxMainWindowType> window;
  QScopedPointer<QSplashScreen> splashScreen;

  int exitCode = qCjyxApplicationHelper::postInitializeApplication<CjyxMainWindowType>(
        app, splashScreen, window);
  if (exitCode != 0)
    {
    return exitCode;
    }

  if (!window.isNull())
    {
    QString windowTitle = QString("%1 %2").arg(window->windowTitle()).arg(Cjyx_VERSION_FULL);
    window->setWindowTitle(windowTitle);
    }

  return app.exec();
}

} // end of anonymous namespace

#include "qCjyxApplicationMainWrapper.cxx"
