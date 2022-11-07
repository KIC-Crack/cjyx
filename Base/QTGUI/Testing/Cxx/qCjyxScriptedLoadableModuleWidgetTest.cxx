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

// VTK includes
#include <vtkNew.h>

// Cjyx includes
#include "qCjyxPythonManager.h"
#include "qCjyxScriptedLoadableModuleWidget.h"
#include "vtkMRMLModelNode.h"

#include <PythonQt.h>
// ----------------------------------------------------------------------------
class qCjyxScriptedLoadableModuleWidgetTester: public QObject
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

  void testEnterExit();
  void testEnterExit_data();

  void testSetup();
  void testSetup_data();

  void testNodeEdit();
  void testNodeEdit_data();

};

// ----------------------------------------------------------------------------
QString qCjyxScriptedLoadableModuleWidgetTester::preparePythonSource(const QString& scriptName)
{
  QFile::copy(":" + scriptName, this->Tmp.filePath(scriptName));
  return this->Tmp.filePath(scriptName);
}

// ----------------------------------------------------------------------------
bool qCjyxScriptedLoadableModuleWidgetTester::resetTmp()
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
void qCjyxScriptedLoadableModuleWidgetTester::initTestCase()
{
  QVERIFY(this->PythonManager.initialize());

  QVERIFY(QDir::temp().exists());

  this->TemporaryDirName =
      QString("qCjyxScriptedLoadableModuleWidgetTester.%1").arg(QTime::currentTime().toString("hhmmsszzz"));
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::cleanupTestCase()
{
  if (this->Tmp != QDir::current() && this->Tmp.exists())
    {
    ctk::removeDirRecursively(this->Tmp.absolutePath());
    this->Tmp = QDir();
    }
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testSetPythonSource()
{
  QVERIFY(this->resetTmp());

  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModuleWidget w;
  QVERIFY(w.pythonSource().isEmpty());

  w.setPythonSource(scriptPath);
  QFETCH(bool, syntaxErrorExpected);
  QString expectedScriptPath = syntaxErrorExpected ? QString() : scriptPath;
  QCOMPARE(w.pythonSource(), expectedScriptPath);

  QVERIFY(!PyErr_Occurred());
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testSetPythonSource_data()
{
  QTest::addColumn<QString>("scriptName");
  QTest::addColumn<bool>("syntaxErrorExpected");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTestWidget.py" << false;
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleTest.py" << false;
  QTest::newRow("2") << "qCjyxScriptedLoadableModuleSyntaxErrorTestWidget.py" << true;
  QTest::newRow("3") << "qCjyxScriptedLoadableModuleNewStyleTestWidget.py" << false;
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testEnterExit()
{
  QVERIFY(this->resetTmp());

  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModuleWidget w;
  w.setPythonSource(scriptPath);

  QVERIFY(!w.property("enter_called_within_Python").toBool());
  w.enter();
  QVERIFY(w.property("enter_called_within_Python").toBool());

  QVERIFY(!w.property("exit_called_within_Python").toBool());
  w.exit();
  QVERIFY(w.property("exit_called_within_Python").toBool());
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testEnterExit_data()
{
  QTest::addColumn<QString>("scriptName");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTestWidget.py";
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleNewStyleTestWidget.py";
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testNodeEdit()
{
  QVERIFY(this->resetTmp());

  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModuleWidget w;
  w.setPythonSource(scriptPath);

  vtkNew<vtkMRMLModelNode> node;
  node->SetName("Some");

  QVERIFY(w.nodeEditable(nullptr) == 0.3);
  QVERIFY(w.property("editableNodeName").toString() == QString(""));
  QVERIFY(w.nodeEditable(node.GetPointer()) == 0.7);
  QVERIFY(w.property("editableNodeName").toString() == QString("Some"));

  QVERIFY(w.setEditedNode(nullptr) == false);
  QVERIFY(w.property("editedNodeName").toString() == QString(""));
  QVERIFY(w.setEditedNode(node.GetPointer(), "someRole", "someContext") == true);
  QVERIFY(w.property("editedNodeName").toString() == QString("Some"));
  QVERIFY(w.property("editedNodeRole").toString() == QString("someRole"));
  QVERIFY(w.property("editedNodeContext").toString() == QString("someContext"));
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testNodeEdit_data()
{
  QTest::addColumn<QString>("scriptName");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTestWidget.py";
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleNewStyleTestWidget.py";
}

namespace
{
// ----------------------------------------------------------------------------
class qCjyxScriptedLoadableModuleWidgetTestSetup : public qCjyxScriptedLoadableModuleWidget
{
public:
  void callSetup() { this->setup(); }
};

} // end of anonymous namespace

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testSetup()
{
  QVERIFY(this->resetTmp());
  QFETCH(QString, scriptName);
  QString scriptPath = this->preparePythonSource(scriptName);
  QVERIFY(QFile::exists(scriptPath));

  qCjyxScriptedLoadableModuleWidgetTestSetup w;
  w.setPythonSource(scriptPath);

  QVERIFY(!w.property("setup_called_within_Python").toBool());
  w.callSetup();
  QVERIFY(w.property("setup_called_within_Python").toBool());
}

// ----------------------------------------------------------------------------
void qCjyxScriptedLoadableModuleWidgetTester::testSetup_data()
{
  QTest::addColumn<QString>("scriptName");

  QTest::newRow("0") << "qCjyxScriptedLoadableModuleTestWidget.py";
  QTest::newRow("1") << "qCjyxScriptedLoadableModuleNewStyleTestWidget.py";
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxScriptedLoadableModuleWidgetTest)
#include "moc_qCjyxScriptedLoadableModuleWidgetTest.cxx"
