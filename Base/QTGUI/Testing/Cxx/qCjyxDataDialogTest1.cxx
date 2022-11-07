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
#include "qCjyxApplication.h"
#include "qCjyxCoreIOManager.h"
#include "qCjyxDataDialog.h"
#include "qCjyxIO.h"
#include "qCjyxIOOptionsWidget.h"

// STD includes


//-----------------------------------------------------------------------------
class qCjyxDummyIOOptionsWidget
  : public qCjyxIOOptionsWidget
{
public:
  qCjyxDummyIOOptionsWidget(QWidget *parent=nullptr): qCjyxIOOptionsWidget(parent){}
  ~qCjyxDummyIOOptionsWidget() override = default;
  QSize minimumSizeHint()const override {return QSize(300, 30);}
  QSize sizeHint()const override{return QSize(500,30);}

private:
  Q_DISABLE_COPY(qCjyxDummyIOOptionsWidget);
};

//-----------------------------------------------------------------------------
/// \ingroup Cjyx_QtModules_Dummy
class qCjyxDummyIO: public qCjyxIO
{
public:
  qCjyxDummyIO(QObject* parent = nullptr):qCjyxIO(parent){}
  ~qCjyxDummyIO() override = default;
  QString description()const override{return "Dummy";}
  IOFileType fileType()const override{return QString("UserFile");}
  virtual QStringList extensions()const{return QStringList(QString("All Files(*)"));}
  qCjyxIOOptions* options()const override{return new qCjyxDummyIOOptionsWidget;}

  virtual bool load(const IOProperties& properties);
};

//-----------------------------------------------------------------------------
bool qCjyxDummyIO::load(const IOProperties& properties)
{
  Q_UNUSED(properties);
  return true;
}

//-----------------------------------------------------------------------------
int qCjyxDataDialogTest1(int argc, char * argv[] )
{
  qCjyxApplication app(argc, argv);
  app.coreIOManager()->registerIO(new qCjyxDummyIO(nullptr));

  qCjyxDataDialog dataDialog;

  if (argc < 2 || QString(argv[1]) != "-I")
    {
    // Quit the dialog
    QTimer::singleShot(100, &app, SLOT(quit()));
    // Quit the app
    QTimer::singleShot(120, &app, SLOT(quit()));
    }

  return dataDialog.exec();
}

