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
#include <QProcess>
#include <QStandardPaths>

// Cjyx includes
#include "qCjyxCLIExecutableModuleFactory.h"
#include "qCjyxCLIModule.h"
#include "qCjyxCLIModuleFactoryHelper.h"
#include "qCjyxUtils.h"
#include <vtkCjyxCLIModuleLogic.h>

//-----------------------------------------------------------------------------
QString findPython()
{
  QString python_path = QStandardPaths::findExecutable("python-real");
  if (python_path.isEmpty())
    {
    python_path = QStandardPaths::findExecutable("python");
    }

  QFileInfo python(python_path);
  if (!(python.exists() && python.isExecutable()))
    {
    return QString();
    }
  return python_path;

}

//-----------------------------------------------------------------------------
qCjyxCLIExecutableModuleFactoryItem::qCjyxCLIExecutableModuleFactoryItem(
  const QString& newTempDirectory)
  : TempDirectory(newTempDirectory)
  , CLIModule(nullptr)
{
}

//-----------------------------------------------------------------------------
bool qCjyxCLIExecutableModuleFactoryItem::load()
{
  return true;
}

//-----------------------------------------------------------------------------
QString qCjyxCLIExecutableModuleFactoryItem::xmlModuleDescriptionFilePath()
{
  QFileInfo info = QFileInfo(this->path());
  return QDir(info.path()).filePath(info.baseName() + ".xml");
}

//-----------------------------------------------------------------------------
qCjyxAbstractCoreModule* qCjyxCLIExecutableModuleFactoryItem::instanciator()
{
  // Using a scoped pointer ensures the memory will be cleaned if instantiator
  // fails before returning the module. See QScopedPointer::take()
  QScopedPointer<qCjyxCLIModule> module(new qCjyxCLIModule());
  module->setModuleType("CommandLineModule");
  module->setEntryPoint(this->path());

  // Identify CLI-only .py scripts by `#!` first line
  // then set up interpreter path in SEM module `Location` parameter.
  if (QFileInfo(this->path()).suffix().toLower() == "py")
    {
      QString python_path = findPython();
      if (python_path.isEmpty())
        {
        this->appendInstantiateErrorString(
          QString("Failed to find python interpreter for CLI: %1").arg(this->path()));
        return nullptr;
        }

      module->setEntryPoint("python");
      module->moduleDescription().SetLocation(python_path.toStdString());
      module->moduleDescription().SetTarget(this->path().toStdString());
    }

  QString xmlFilePath = this->xmlModuleDescriptionFilePath();

  //
  // If the xml file exists, read it and associate it with the module
  // description. If not, run the CLI executable with "--xml".
  //
  QString xmlDescription;
  if (QFile::exists(xmlFilePath))
    {
    QFile xmlFile(xmlFilePath);
    if (xmlFile.open(QIODevice::ReadOnly))
      {
      xmlDescription = QTextStream(&xmlFile).readAll();
      }
    else
      {
      this->appendInstantiateErrorString(QString("CLI description: %1").arg(xmlFilePath));
      this->appendInstantiateErrorString("Failed to read Xml Description");
      }
    }
  else
    {
    xmlDescription = this->runCLIWithXmlArgument();
    }
  if (xmlDescription.isEmpty())
    {
    return nullptr;
    }

  module->setXmlModuleDescription(xmlDescription.toUtf8());
  module->setTempDirectory(this->TempDirectory);
  module->setPath(this->path());
  module->setInstalled(qCjyxCLIModuleFactoryHelper::isInstalled(this->path()));
  module->setBuiltIn(qCjyxCLIModuleFactoryHelper::isBuiltIn(this->path()));

  this->CLIModule = module.data();

  return module.take();
}

//-----------------------------------------------------------------------------
QString qCjyxCLIExecutableModuleFactoryItem::runCLIWithXmlArgument()
{
  ctkScopedCurrentDir scopedCurrentDir(QFileInfo(this->path()).path());

  int cliProcessTimeoutInMs = 5000;
  QProcess cli;
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("ITK_AUTOLOAD_PATH", "");
  cli.setProcessEnvironment(env);
  cli.start(this->path(), QStringList(QString("--xml")));
  bool res = cli.waitForFinished(cliProcessTimeoutInMs);
  if (!res)
    {
    this->appendInstantiateErrorString(QString("CLI executable: %1").arg(this->path()));
    QString errorString;
    switch(cli.error())
      {
      case QProcess::FailedToStart:
        errorString = QLatin1String(
              "The process failed to start. Either the invoked program is missing, or "
              "you may have insufficient permissions to invoke the program.");
        break;
      case QProcess::Crashed:
        errorString = QLatin1String(
              "The process crashed some time after starting successfully.");
        break;
      case QProcess::Timedout:
        errorString = QString(
              "The process timed out after %1 msecs.").arg(cliProcessTimeoutInMs);
        break;
      case QProcess::WriteError:
        errorString = QLatin1String(
              "An error occurred when attempting to read from the process. "
              "For example, the process may not be running.");
        break;
      case QProcess::ReadError:
        errorString = QLatin1String(
              "An error occurred when attempting to read from the process. "
              "For example, the process may not be running.");
        break;
      case QProcess::UnknownError:
        errorString = QLatin1String(
              "Failed to execute process. An unknown error occurred.");
        break;
      }
    this->appendInstantiateErrorString(errorString);
    return nullptr;
    }
  QString errors = cli.readAllStandardError();
  if (!errors.isEmpty())
    {
    this->appendInstantiateErrorString(QString("CLI executable: %1").arg(this->path()));
    this->appendInstantiateErrorString(errors);
    // TODO: More investigation for the following behavior:
    // on my machine (Ubuntu 10.04 with ITKv4), having standard error trims the
    // standard output results. The following readAllStandardOutput() is then
    // missing chars and makes the XML invalid. I'm not sure if it's just on my
    // machine so there is a chance it succeeds to parse the XML description
    // on other machines.
    }
  QString xmlDescription = cli.readAllStandardOutput();
  if (xmlDescription.isEmpty())
    {
    this->appendInstantiateErrorString(QString("CLI executable: %1").arg(this->path()));
    this->appendInstantiateErrorString("Failed to retrieve Xml Description");
    return QString();
    }
  if (!xmlDescription.startsWith("<?xml"))
    {
    this->appendInstantiateWarningString(QString("CLI executable: %1").arg(this->path()));
    this->appendInstantiateWarningString(QLatin1String("XML description doesn't start right away."));
    this->appendInstantiateWarningString(QString("Output before '<?xml' is [%1]").arg(
                                           xmlDescription.mid(0, xmlDescription.indexOf("<?xml"))));
    xmlDescription.remove(0, xmlDescription.indexOf("<?xml"));
    }
  return xmlDescription;
}

//-----------------------------------------------------------------------------
void qCjyxCLIExecutableModuleFactoryItem::uninstantiate()
{
  this->CLIModule->cliModuleLogic()->KillProcesses();
  this->ctkAbstractFactoryFileBasedItem<qCjyxAbstractCoreModule>::uninstantiate();
}

//-----------------------------------------------------------------------------
// qCjyxCLIExecutableModuleFactoryPrivate

//-----------------------------------------------------------------------------
class qCjyxCLIExecutableModuleFactoryPrivate
{
  Q_DECLARE_PUBLIC(qCjyxCLIExecutableModuleFactory);
protected:
  qCjyxCLIExecutableModuleFactory* const q_ptr;
public:
  typedef qCjyxCLIExecutableModuleFactoryPrivate Self;
  qCjyxCLIExecutableModuleFactoryPrivate(qCjyxCLIExecutableModuleFactory& object);

private:
  QString TempDirectory;
};

//-----------------------------------------------------------------------------
qCjyxCLIExecutableModuleFactoryPrivate::qCjyxCLIExecutableModuleFactoryPrivate(qCjyxCLIExecutableModuleFactory& object)
:q_ptr(&object)
{
  this->TempDirectory = QDir::tempPath();
}

//-----------------------------------------------------------------------------
// qCjyxCLIExecutableModuleFactory

//-----------------------------------------------------------------------------
qCjyxCLIExecutableModuleFactory::qCjyxCLIExecutableModuleFactory()
  : d_ptr(new qCjyxCLIExecutableModuleFactoryPrivate(*this))
{
}

//-----------------------------------------------------------------------------
qCjyxCLIExecutableModuleFactory::~qCjyxCLIExecutableModuleFactory() = default;

//-----------------------------------------------------------------------------
void qCjyxCLIExecutableModuleFactory::registerItems()
{
  QStringList modulePaths = qCjyxCLIModuleFactoryHelper::modulePaths();
  this->registerAllFileItems(modulePaths);
}

//-----------------------------------------------------------------------------
bool qCjyxCLIExecutableModuleFactory::isValidFile(const QFileInfo& file)const
{
  if (!this->Superclass::isValidFile(file))
    {
    return false;
    }

  // consider .py files to be executable. interpreter is set in ::instanciator
  if ((!file.isExecutable()) &&
      (!file.filePath().endsWith(".py", Qt::CaseInsensitive)))
    {
    return false;
    }
  return qCjyxUtils::isCLIExecutable(file.absoluteFilePath());
}

//-----------------------------------------------------------------------------
ctkAbstractFactoryItem<qCjyxAbstractCoreModule>* qCjyxCLIExecutableModuleFactory
::createFactoryFileBasedItem()
{
  Q_D(qCjyxCLIExecutableModuleFactory);
  return new qCjyxCLIExecutableModuleFactoryItem(d->TempDirectory);
}

//-----------------------------------------------------------------------------
QString qCjyxCLIExecutableModuleFactory::fileNameToKey(const QString& executableName)const
{
  return qCjyxUtils::extractModuleNameFromLibraryName(executableName);
}

//-----------------------------------------------------------------------------
void qCjyxCLIExecutableModuleFactory::setTempDirectory(const QString& newTempDirectory)
{
  Q_D(qCjyxCLIExecutableModuleFactory);
  d->TempDirectory = newTempDirectory;
}
