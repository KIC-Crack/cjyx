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
#include "qCjyxCorePythonManager.h"

// ----------------------------------------------------------------------------
class qCjyxCorePythonManagerWithoutApplicationTester: public QObject
{
  Q_OBJECT

private:
  qCjyxCorePythonManager PythonManager;

private slots:
  void testInitialize();
  void toPythonStringLiteral();
};

// ----------------------------------------------------------------------------
void qCjyxCorePythonManagerWithoutApplicationTester::testInitialize()
{
  this->PythonManager.initialize();
}

// ----------------------------------------------------------------------------
void qCjyxCorePythonManagerWithoutApplicationTester::toPythonStringLiteral()
{
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("simple string"),
                                                  QString("'simple string'"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("C:\\folder1\\folder2"),
                                                  QString("'C:\\\\folder1\\\\folder2'"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("C:/folder1/folder2"),
                                                  QString("'C:/folder1/folder2'"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("this \"special\" string contains double-quotes"),
                                                  QString("'this \"special\" string contains double-quotes'"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("this name O'Neil contains a single-quote"),
                                                  QString("'this name O\\'Neil contains a single-quote'"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("'single-quoted string'"),
                                               QString("'\\\'single-quoted string\\\''"));
  QCOMPARE(qCjyxCorePythonManager::toPythonStringLiteral("\"double-quoted string\""),
                                                  QString("'\"double-quoted string\"'"));
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxCorePythonManagerWithoutApplicationTest)
#include "moc_qCjyxCorePythonManagerWithoutApplicationTest.cxx"

