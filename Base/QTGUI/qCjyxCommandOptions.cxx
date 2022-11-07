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

#include "qCjyxCommandOptions.h"
#include "qCjyxCoreApplication.h"

// Cjyx includes

//-----------------------------------------------------------------------------
// qCjyxCommandOptions methods

//-----------------------------------------------------------------------------
qCjyxCommandOptions::qCjyxCommandOptions():Superclass()
{
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::disableToolTips()const
{
  return this->parsedArgs().value("disable-tooltips").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::noSplash() const
{
  return this->parsedArgs().value("no-splash").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::noMainWindow() const
{
  return this->parsedArgs().value("no-main-window").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::showPythonInteractor() const
{
  return this->parsedArgs().value("show-python-interactor").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::enableQtTesting()const
{
#ifdef Cjyx_USE_QtTesting
  return this->parsedArgs().value("qt-testing").toBool();
#else
  return false;
#endif
}

//-----------------------------------------------------------------------------
bool qCjyxCommandOptions::exitAfterStartup()const
{
  return this->parsedArgs().value("exit-after-startup").toBool();
}

//-----------------------------------------------------------------------------
void qCjyxCommandOptions::addArguments()
{
  this->Superclass::addArguments();

  this->addArgument("disable-tooltips", "", QVariant::Bool,
                    "Disable toolstips in the user interface.");

  this->addArgument("no-splash", "", QVariant::Bool,
                    "Disable the startup splash screen.");

  this->addArgument("no-main-window", "", QVariant::Bool,
                    "Disable display of the main cjyx window.  Use with --python-script for alternate interface");

#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    this->addArgument("show-python-interactor", "", QVariant::Bool,
                      "Show Python interactor at startup.");
    }
#endif

#ifdef Cjyx_USE_QtTesting
  this->addArgument("qt-testing", "", QVariant::Bool,
                    "Enable QtTesting in the user interface");
#endif

  this->addArgument("exit-after-startup", "", QVariant::Bool,
                    "Exit after startup is complete. Useful for measuring startup time");
}
