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
#include <QDebug>
#include <QDir>

// CTK includes
#include <ctkPimpl.h>

// Cjyx includes
#include "qCjyxCoreCommandOptions.h"

//-----------------------------------------------------------------------------
class qCjyxCoreCommandOptionsPrivate
{
  Q_DECLARE_PUBLIC(qCjyxCoreCommandOptions);
protected:
  qCjyxCoreCommandOptions* q_ptr;
public:
  qCjyxCoreCommandOptionsPrivate(qCjyxCoreCommandOptions& object);

  void init();

  QHash<QString, QVariant> ParsedArgs;
  QString                  ExtraPythonScript;
  bool                     RunPythonAndExit;
};

//-----------------------------------------------------------------------------
// qCjyxCoreCommandOptionsPrivate methods

//-----------------------------------------------------------------------------
qCjyxCoreCommandOptionsPrivate::qCjyxCoreCommandOptionsPrivate(qCjyxCoreCommandOptions& object)
  : q_ptr(&object)
  , RunPythonAndExit(false)
{
}

//-----------------------------------------------------------------------------
void qCjyxCoreCommandOptionsPrivate::init()
{
  Q_Q(qCjyxCoreCommandOptions);
  q->setArgumentPrefix("--", "-"); // Use Unix-style argument names
  q->enableSettings("disable-settings"); // Enable QSettings support
}

//-----------------------------------------------------------------------------
// qCjyxCoreCommandOptions methods

//-----------------------------------------------------------------------------
qCjyxCoreCommandOptions::qCjyxCoreCommandOptions():Superclass()
, d_ptr(new qCjyxCoreCommandOptionsPrivate(*this))
{
  Q_D(qCjyxCoreCommandOptions);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxCoreCommandOptions::~qCjyxCoreCommandOptions() = default;

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::parse(const QStringList& arguments)
{
  Q_D(qCjyxCoreCommandOptions);

  this->addArguments();

  bool ok = false;
  d->ParsedArgs = this->parseArguments(arguments, &ok);
  if (!ok)
    {
    return false;
    }

  // If first unparsed argument is python script, enable 'shebang' mode
  QStringList unparsedArguments = this->unparsedArguments();
  if (unparsedArguments.size() > 0 && unparsedArguments.at(0).endsWith(".py"))
    {
    if(!this->pythonScript().isEmpty())
      {
      qWarning() << "Ignore script specified using '--python-script'";
      }
    this->setExtraPythonScript(unparsedArguments.at(0));
    this->setRunPythonAndExit(true);
    }

  if (!d->ParsedArgs.value("c").toString().isEmpty())
    {
    this->setRunPythonAndExit(true);
    }

  return true;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayHelpAndExit()const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("help").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::ignoreRest() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("ignore-rest").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::ignoreCjyxRC()const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("ignore-cjyxrc").toBool() ||
      this->isTestingEnabled();
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreCommandOptions::additionalModulePaths()const
{
  Q_D(const qCjyxCoreCommandOptions);
  QStringList allAdditionalModulePaths;

  // note the singular form: 'path' not 'paths'
  QString additionalModulePath = d->ParsedArgs.value("additional-module-path").toString();
  if (!additionalModulePath.isEmpty())
    {
    allAdditionalModulePaths << additionalModulePath;
    }
  // handle rest of pathS
  allAdditionalModulePaths.append(d->ParsedArgs.value("additional-module-paths").toStringList());
  return allAdditionalModulePaths;
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableBuiltInModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-builtin-modules").toBool();
}

//-----------------------------------------------------------------------------
QStringList qCjyxCoreCommandOptions::modulesToIgnore() const
{
  Q_D(const qCjyxCoreCommandOptions);
  QString modulesToIgnore = d->ParsedArgs.value("modules-to-ignore").toString();
  return modulesToIgnore.size() == 0 ? QStringList() : modulesToIgnore.split(",");
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableCLIModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-cli-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableBuiltInCLIModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-builtin-cli-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableLoadableModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-loadable-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableBuiltInLoadableModules() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-builtin-loadable-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableScriptedLoadableModules()const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-scripted-loadable-modules").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableBuiltInScriptedLoadableModules()const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-builtin-scripted-loadable-modules").toBool();
}

//-----------------------------------------------------------------------------
QString qCjyxCoreCommandOptions::pythonScript() const
{
  Q_D(const qCjyxCoreCommandOptions);
  // QDir::fromNativeSeparators is needed as users may specify path
  // with native separators, for example
  //     Cjyx.exe --python-script c:\folder\subfolder\script.py
  // but Python requires / as directory separator.
  return QDir::fromNativeSeparators(d->ParsedArgs.value("python-script").toString());
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxCoreCommandOptions, QString, extraPythonScript, ExtraPythonScript);
CTK_SET_CPP(qCjyxCoreCommandOptions, const QString&, setExtraPythonScript, ExtraPythonScript);

//-----------------------------------------------------------------------------
QString qCjyxCoreCommandOptions::pythonCode() const
{
  Q_D(const qCjyxCoreCommandOptions);
  QString pythonCode = d->ParsedArgs.value("python-code").toString();
  if(!pythonCode.isEmpty())
    {
    return pythonCode;
    }
  else
    {
    return d->ParsedArgs.value("c").toString();
    }
}

//-----------------------------------------------------------------------------
CTK_GET_CPP(qCjyxCoreCommandOptions, bool, runPythonAndExit, RunPythonAndExit);
CTK_SET_CPP(qCjyxCoreCommandOptions, bool, setRunPythonAndExit, RunPythonAndExit);

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayVersionAndExit() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("version").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayProgramPathAndExit() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("program-path").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayHomePathAndExit() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("home").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displaySettingsPathAndExit() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("settings-path").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayTemporaryPathAndExit() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("temporary-path").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayMessageAndExit() const
{
  return
      this->displayHelpAndExit()
      || this->displayVersionAndExit()
      || this->displayProgramPathAndExit()
      || this->displayHomePathAndExit()
      || this->displaySettingsPathAndExit()
      || this->displayTemporaryPathAndExit();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::displayApplicationInformation() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("application-information").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::verboseModuleDiscovery() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("verbose-module-discovery").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::verbose()const
{
  return !this->runPythonAndExit();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableMessageHandlers() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-message-handlers").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::disableTerminalOutputs()const
{
  Q_D(const qCjyxCoreCommandOptions);
#if defined (Q_OS_WIN32) && !defined (Cjyx_BUILD_WIN32_CONSOLE)
  return true;
#else
  return d->ParsedArgs.value("disable-terminal-outputs").toBool();
#endif
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::settingsDisabled() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-settings").toBool() ||
      this->isTestingEnabled();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::keepTemporarySettings() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("keep-temporary-settings").toBool();
}

//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::isTestingEnabled() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("testing").toBool();
}

#ifdef Cjyx_USE_PYTHONQT
//-----------------------------------------------------------------------------
bool qCjyxCoreCommandOptions::isPythonDisabled() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs.value("disable-python").toBool();
}
#endif

//-----------------------------------------------------------------------------
void qCjyxCoreCommandOptions::addArguments()
{
  this->addArgument("ignore-rest", "-", QVariant::Bool,
                    "Ignores the rest of the labeled arguments following this flag.",
                    QVariant(false), true);

  this->addArgument("help", "h", QVariant::Bool,
                    "Display available command line arguments.");

#ifdef Cjyx_USE_PYTHONQT
  QString testingDescription = "Activate testing mode. It implies --disable-settings and --ignore-cjyxrc.";
#else
  QString testingDescription = "Activate testing mode. It implies --disable-settings.";
#endif
  this->addArgument("testing", "", QVariant::Bool,
                    testingDescription,
                    QVariant(false));

#ifdef Cjyx_USE_PYTHONQT
  this->addArgument("disable-python", "", QVariant::Bool,
                    "Disable python support. This is equivalent to build the application with Cjyx_USE_PYTHONQT=OFF.");

  this->addArgument("python-script", "", QVariant::String,
                    "Python script to execute after cjyx loads.");

  this->addArgument("python-code", "", QVariant::String,
                    "Python code to execute after cjyx loads.");

  this->addArgument("", "c", QVariant::String,
                    "Python code to execute after cjyx loads. By default, no modules are loaded and Cjyx exits afterward.");

  this->addArgument("ignore-cjyxrc", "", QVariant::Bool,
                    "Do not load the Cjyx resource file (~/.cjyxrc.py).");
#endif

  this->addArgument("additional-module-path", "", QVariant::String,
                    "Additional module path to consider when searching for modules to load.");

  this->addArgument("additional-module-paths", "", QVariant::StringList,
                    "List of additional module path to consider when searching for modules to load.");

  this->addArgument("modules-to-ignore", "", QVariant::String,
                    "Comma separated list of modules that should *NOT* be loaded.");

  this->addArgument("disable-modules", "", QVariant::Bool,
                    "Disable the loading of any Modules.");

  this->addArgument("disable-builtin-modules", "", QVariant::Bool,
                    "Disable the loading of builtin Modules.");

#ifdef Cjyx_BUILD_CLI_SUPPORT
  this->addArgument("disable-cli-modules", "", QVariant::Bool,
                    "Disable the loading of any Command Line Modules.");

  this->addArgument("disable-builtin-cli-modules", "", QVariant::Bool,
                    "Disable the loading of builtin Command Line Modules.");
#endif

  this->addArgument("disable-loadable-modules", "", QVariant::Bool,
                    "Disable the loading of any Loadable Modules.");

  this->addArgument("disable-builtin-loadable-modules", "", QVariant::Bool,
                    "Disable the loading of builtin Loadable Modules.");

#ifdef Cjyx_USE_PYTHONQT
  this->addArgument("disable-scripted-loadable-modules", "", QVariant::Bool,
                    "Disable the loading of any Scripted Loadable Modules.");

  this->addArgument("disable-builtin-scripted-loadable-modules", "", QVariant::Bool,
                    "Disable the loading of builtinScripted Loadable Modules.");
#endif

  this->addArgument("version", "", QVariant::Bool,
                    "Display version information and exits.");

  this->addArgument("program-path", "", QVariant::Bool,
                    "Display application program path and exits.");

  this->addArgument("home", "", QVariant::Bool,
                    "Display home path and exits.");

  this->addArgument("settings-path", "", QVariant::Bool,
                    "Display settings path and exits.");

  this->addArgument("temporary-path", "", QVariant::Bool,
                    "Display temporary path and exits.");

  this->addArgument("application-information", "", QVariant::Bool,
                    "Display application information in the terminal.");

  this->addArgument("verbose-module-discovery", "", QVariant::Bool,
                    "Enable verbose output during module discovery process.");

  this->addArgument("disable-settings", "", QVariant::Bool,
                    "Start application ignoring user settings and using new temporary settings.");

  this->addArgument("keep-temporary-settings", "", QVariant::Bool,
                    "Indicate whether temporary settings should be maintained.");

  this->addArgument("disable-message-handlers", "", QVariant::Bool,
                    "Start application disabling the 'terminal' message handlers.");

#if defined (Q_OS_WIN32) && !defined (Cjyx_BUILD_WIN32_CONSOLE)
#else
  this->addArgument("disable-terminal-outputs", "", QVariant::Bool,
                    "Start application disabling stdout/stderr outputs and capturing outputs only using the error log.");
#endif
}

//-----------------------------------------------------------------------------
QHash<QString, QVariant> qCjyxCoreCommandOptions::parsedArgs() const
{
  Q_D(const qCjyxCoreCommandOptions);
  return d->ParsedArgs;
}
