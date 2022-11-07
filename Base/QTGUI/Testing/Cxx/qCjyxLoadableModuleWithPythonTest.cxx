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

// Cjyx includes
#include "qCjyxLoadableModule.h"
#include "qCjyxPythonManager.h"

// ----------------------------------------------------------------------------
class qCjyxLoadableHelloWorldModule : public qCjyxLoadableModule
{
  Q_OBJECT
public:

  typedef qCjyxLoadableModule Superclass;
  qCjyxLoadableHelloWorldModule(QObject *parent=nullptr):Superclass(parent){}
  ~qCjyxLoadableHelloWorldModule() override = default;

  QString helpText()const override { return QString("helpText"); }
  QString acknowledgementText()const override { return QString("acknowledgementText"); }

  qCjyxGetTitleMacro("Loadable Hello world");

protected:
  /// Create and return the widget representation associated to this module
  qCjyxAbstractModuleRepresentation * createWidgetRepresentation() override
  {
    return nullptr;
  }

  /// Create and return the logic associated to this module
  vtkMRMLAbstractLogic* createLogic() override
  {
    return nullptr;
  }
};

// ----------------------------------------------------------------------------
class qCjyxLoadableModuleWithPythonTester: public QObject
{
  Q_OBJECT

private:
  qCjyxPythonManager PythonManager;

  QHash<QString, qCjyxAbstractModule*> Modules;

private slots:
  void testInitialize();

  void testAddModuleToCjyxModules();
  void testAddModuleToCjyxModules_data();

  void testAddModuleNameToCjyxModuleNames();
  void testAddModuleNameToCjyxModuleNames_data();
};

// ----------------------------------------------------------------------------
void qCjyxLoadableModuleWithPythonTester::testInitialize()
{
  this->PythonManager.initialize();
  this->PythonManager.executeString("import cjyx");

  this->Modules.insert("LoadableHelloWorld", new qCjyxLoadableHelloWorldModule(this));
}

// ----------------------------------------------------------------------------
void qCjyxLoadableModuleWithPythonTester::testAddModuleToCjyxModules()
{
  QFETCH(bool, validPythonManager);
  QFETCH(QString, moduleName);
  QFETCH(bool, expectedResult);

  qCjyxAbstractModule * module = this->Modules.value(moduleName);
  QVERIFY(moduleName.isEmpty() ? true : module != nullptr);

  bool currentResult = qCjyxLoadableModule::addModuleToCjyxModules(
        validPythonManager ? &this->PythonManager : nullptr,
        module,
        moduleName);
  QCOMPARE(currentResult, expectedResult);

  if (expectedResult)
    {
    this->PythonManager.executeString(QString("dir(cjyx.modules.%1)").arg(moduleName.toLower()));
    QCOMPARE(!this->PythonManager.pythonErrorOccured(), expectedResult);
    }
}

// ----------------------------------------------------------------------------
void qCjyxLoadableModuleWithPythonTester::testAddModuleToCjyxModules_data()
{
  QTest::addColumn<bool>("validPythonManager");
  QTest::addColumn<QString>("moduleName");
  QTest::addColumn<bool>("expectedResult");

  QTest::newRow("1") << true << "LoadableHelloWorld" << true;
  QTest::newRow("2") << true << "" << false;
  QTest::newRow("3") << false << "" << false;
}

// ----------------------------------------------------------------------------
void qCjyxLoadableModuleWithPythonTester::testAddModuleNameToCjyxModuleNames()
{
  QFETCH(bool, validPythonManager);
  QFETCH(QString, moduleName);
  QFETCH(bool, expectedResult);

  bool currentResult = qCjyxLoadableModule::addModuleNameToCjyxModuleNames(
        validPythonManager ? &this->PythonManager : nullptr, moduleName);
  QCOMPARE(currentResult, expectedResult);

  if (expectedResult)
    {
    this->PythonManager.executeString(QString("dir(cjyx.moduleNames.%1)").arg(moduleName.toLower()));
    QCOMPARE(!this->PythonManager.pythonErrorOccured(), expectedResult);
    }
}

// ----------------------------------------------------------------------------
void qCjyxLoadableModuleWithPythonTester::testAddModuleNameToCjyxModuleNames_data()
{
  QTest::addColumn<bool>("validPythonManager");
  QTest::addColumn<QString>("moduleName");
  QTest::addColumn<bool>("expectedResult");

  QTest::newRow("0") << true << "CoreHelloWorld" << true;
  QTest::newRow("1") << true << "LoadableHelloWorld" << true;
  QTest::newRow("2") << true << "" << false;
  QTest::newRow("3") << false << "" << false;
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxLoadableModuleWithPythonTest)
#include "moc_qCjyxLoadableModuleWithPythonTest.cxx"
