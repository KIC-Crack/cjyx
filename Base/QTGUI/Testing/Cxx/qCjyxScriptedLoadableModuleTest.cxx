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

// CTK includes
#include <ctkTest.h>
#include <ctkUtils.h>

// Cjyx includes
#include "qCjyxPythonManager.h"
#include "qCjyxScriptedLoadableModule.h"

#include <PythonQt.h>
// ----------------------------------------------------------------------------
class qCjyxScriptedLoadableModuleTester: public QObject
{
  Q_OBJECT

private:

  QString preparePythonSource(const QString& scriptName);

  qCjyxPythonManager PythonManager;

  bool resetTmp();
  QDir Tmp;
  QString TemporaryDirName;

private slots:
  void initTestCase();
  void cleanupTestCase();

  void testSetPythonSource();
  void testSetPythonSource_data();

  void testSetup();
  void testSetup_data();

};

// ----------------------------------------------------------------------------
QString qCjyxScriptedLoadableModuleTester::preparePythonSource(const QString& scriptName)
{
  QFile::copy(":" + scriptName, this->Tmp.filePath(scriptName));
  return this->Tmp.filePath(scriptName);
}

// ----------------------------------------------------------------------------
bool qCjyxScriptedLoadableModuleTester::resetTmp()
{
  if (this->TemporaryDirName.isEmpty())
    {
    return false;
    }
  QDir tmp = QDir::temp();
  ctk::removeDirRecursively(tmp.filePath(this->TemporaryDirName));
  tmp.mkdir(this->TemporaryDirName);
  tmp.cd(this->TemporaryDirName);
  this->Tmp = tmp;
  return this->Tmp.exists();
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::initTestCase()
{
  QVERIFY(this->PythonManager.initialize());

  QVERIFY(QDir::temp().exists());

  this->TemporaryDirName =
      QString("qCjyxScriptedLoadableModuleTester.%1").arg(QTime::currentTime().toString("hhmmsszzz"));
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::cleanupTestCase()
{
  if (this->Tmp != QDir::current() && this->Tmp.exists())
    {
    ctk::removeDirRecursively(this->Tmp.absolutePath());
    this->Tmp = QDir();
    }
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::testSetPythonSource()
{
  QVERIFY(this->resetTmp());

  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModule m;
  QVERIFY(m.pythonSource().isEmpty());

  m.setPythonSource(scriptPath);
  QFETCH(bool, syntaxErrorExpected);
  QString expectedScriptPath = syntaxErrorExpected ? QString() : scriptPath;
  QCOMPARE(m.pythonSource(), expectedScriptPath);

  QVERIFY(!PyErr_Occurred());
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::testSetPythonSource_data()
{
  QTest::addColumn<QString>("scriptName");
  QTest::addColumn<bool>("syntaxErrorExpected");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTest.py" << false;
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleSyntaxErrorTest.py" << true;
  QTest::newRow("2") << "qCjyxScriptedLoadableModuleNewStyleTest.py" << false;
}

namespace
{
// ----------------------------------------------------------------------------
class qCjyxScriptedLoadableModuleSetup : public qCjyxScriptedLoadableModule
{
public:
  void callSetup() { this->setup(); }
};

} // end of anonymous namespace

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::testSetup()
{
  QVERIFY(this->resetTmp());
  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModuleSetup m;
  m.setPythonSource(scriptPath);

  QVERIFY(!m.property("setup_called_within_Python").toBool());
  m.callSetup();
  QVERIFY(m.property("setup_called_within_Python").toBool());
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleTester::testSetup_data()
{
  QTest::addColumn<QString>("scriptName");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTest.py";
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleNewStyleTest.py";
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxScriptedLoadableModuleTest)
#include "moc_qCjyxScriptedLoadableModuleTest.cxx"
