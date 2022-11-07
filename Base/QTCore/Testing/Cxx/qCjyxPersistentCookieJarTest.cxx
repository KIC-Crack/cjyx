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
#include <QApplication>

// CTK includes
#include <ctkTest.h>

// QtCore includes
#include "qCjyxPersistentCookieJar.h"

// ----------------------------------------------------------------------------
class qCjyxPersistentCookieJarTester: public QObject
{
  Q_OBJECT
  typedef qCjyxPersistentCookieJarTester Self;

private slots:
  void testDefaults();
};

// ----------------------------------------------------------------------------
void qCjyxPersistentCookieJarTester::testDefaults()
{
  qCjyxPersistentCookieJar cookieJar;
  // No initialization required
  QVERIFY(cookieJar.filePath().endsWith("cookies.ini"));
}

// ----------------------------------------------------------------------------
CTK_TEST_MAIN(qCjyxPersistentCookieJarTest)
#include "moc_qCjyxPersistentCookieJarTest.cxx"

