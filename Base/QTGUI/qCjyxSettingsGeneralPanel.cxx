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
#include <QDebug>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QUrl>

// CTK includes
#include <ctkBooleanMapper.h>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxRelativePathMapper.h"
#include "qCjyxSettingsGeneralPanel.h"
#include "ui_qCjyxSettingsGeneralPanel.h"

#include "vtkCjyxConfigure.h" // For Cjyx_QM_OUTPUT_DIRS, Cjyx_BUILD_I18N_SUPPORT, Cjyx_USE_PYTHONQT

#ifdef Cjyx_USE_PYTHONQT
#include "PythonQt.h"
#endif

// --------------------------------------------------------------------------
// qCjyxSettingsGeneralPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsGeneralPanelPrivate: public Ui_qCjyxSettingsGeneralPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsGeneralPanel);
protected:
  qCjyxSettingsGeneralPanel* const q_ptr;

public:
  qCjyxSettingsGeneralPanelPrivate(qCjyxSettingsGeneralPanel& object);
  void init();

};

// --------------------------------------------------------------------------
// qCjyxSettingsGeneralPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsGeneralPanelPrivate::qCjyxSettingsGeneralPanelPrivate(qCjyxSettingsGeneralPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsGeneralPanelPrivate::init()
{
  Q_Q(qCjyxSettingsGeneralPanel);

  this->setupUi(q);

#ifdef Cjyx_BUILD_I18N_SUPPORT
  bool internationalizationEnabled =
      qCjyxApplication::application()->userSettings()->value("Internationalization/Enabled", true).toBool();

  this->LanguageLabel->setVisible(internationalizationEnabled);
  this->LanguageComboBox->setVisible(internationalizationEnabled);

  if (internationalizationEnabled)
    {
    // Disable showing country flags because not all regions have a flag (e.g., Latin America)
    this->LanguageComboBox->setCountryFlagsVisible(false);
    this->LanguageComboBox->setDefaultLanguage("en");
    this->LanguageComboBox->setDirectories(qCjyxCoreApplication::translationFolders());
    }
#else
  this->LanguageLabel->setVisible(false);
  this->LanguageComboBox->setVisible(false);
#endif

#ifdef Cjyx_USE_PYTHONQT
  if (!qCjyxCoreApplication::testAttribute(qCjyxCoreApplication::AA_DisablePython))
    {
    PythonQt::init();
    PythonQtObjectPtr context = PythonQt::self()->getMainModule();
    context.evalScript(QString("cjyxrcfilename = getCjyxRCFileName()\n"));
    QVariant cjyxrcFileNameVar = context.getVariable("cjyxrcfilename");
    this->CjyxRCFileValueLabel->setText(cjyxrcFileNameVar.toString());
    QIcon openFileIcon = QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton);
    this->CjyxRCFileOpenButton->setIcon(openFileIcon);
    QObject::connect(this->CjyxRCFileOpenButton, SIGNAL(clicked()), q, SLOT(openCjyxRCFile()));
    }
  else
    {
    this->CjyxRCFileOpenButton->setVisible(false);
    this->CjyxRCFileValueLabel->setVisible(false);
    }
#else
  this->CjyxRCFileLabel->setVisible(false);
  this->CjyxRCFileValueLabel->setVisible(false);
#endif

  // Default values

  this->DefaultScenePathButton->setDirectory(qCjyxCoreApplication::application()->defaultScenePath());
  qCjyxRelativePathMapper* relativePathMapper = new qCjyxRelativePathMapper(this->DefaultScenePathButton, "directory", SIGNAL(directoryChanged(QString)));
  q->registerProperty("DefaultScenePath", relativePathMapper, "relativePath",
                      SIGNAL(relativePathChanged(QString)),
                      "Default scene path");
  QObject::connect(this->DefaultScenePathButton, SIGNAL(directoryChanged(QString)),
                   q, SLOT(setDefaultScenePath(QString)));

  // Since currently there is only English language documentation on readthedocs, the default URL uses "en" language.
  this->DocumentationBaseURLLineEdit->setText("https://slicer.readthedocs.io/en/{version}");
  this->ModuleDocumentationURLLineEdit->setText("{documentationbaseurl}/user_guide/modules/{lowercasemodulename}.html");

  q->registerProperty("no-splash", this->ShowSplashScreenCheckBox, "checked",
                      SIGNAL(toggled(bool)));

  ctkBooleanMapper* restartMapper = new ctkBooleanMapper(this->ConfirmRestartCheckBox, "checked", SIGNAL(toggled(bool)));
  restartMapper->setTrueValue(static_cast<int>(QMessageBox::InvalidRole));
  restartMapper->setFalseValue(static_cast<int>(QMessageBox::Ok));
  q->registerProperty("MainWindow/DontConfirmRestart",
                      restartMapper,"valueAsInt", SIGNAL(valueAsIntChanged(int)));

  ctkBooleanMapper* exitMapper = new ctkBooleanMapper(this->ConfirmExitCheckBox, "checked", SIGNAL(toggled(bool)));
  exitMapper->setTrueValue(static_cast<int>(QMessageBox::InvalidRole));
  exitMapper->setFalseValue(static_cast<int>(QMessageBox::Ok));
  q->registerProperty("MainWindow/DontConfirmExit",
                      exitMapper, "valueAsInt", SIGNAL(valueAsIntChanged(int)));

  ctkBooleanMapper* sceneCloseMapper = new ctkBooleanMapper(this->ConfirmSceneCloseCheckBox, "checked", SIGNAL(toggled(bool)));
  sceneCloseMapper->setTrueValue(static_cast<int>(QMessageBox::InvalidRole));
  sceneCloseMapper->setFalseValue(static_cast<int>(QMessageBox::AcceptRole));
  q->registerProperty("MainWindow/DontConfirmSceneClose",
                      sceneCloseMapper, "valueAsInt", SIGNAL(valueAsIntChanged(int)));

  q->registerProperty("DocumentationBaseURL", this->DocumentationBaseURLLineEdit, "text",
                      SIGNAL(textChanged(QString)),
                      "Documentation location",
                      ctkSettingsPanel::OptionRequireRestart);
  q->registerProperty("ModuleDocumentationURL", this->ModuleDocumentationURLLineEdit, "text",
                      SIGNAL(textChanged(QString)),
                      "Documentation location",
                      ctkSettingsPanel::OptionRequireRestart);
  q->registerProperty("language", this->LanguageComboBox, "currentLanguage",
                      SIGNAL(currentLanguageNameChanged(const QString&)),
                      "Enable/Disable languages",
                      ctkSettingsPanel::OptionRequireRestart);
  q->registerProperty("RecentlyLoadedFiles/NumberToKeep", this->NumOfRecentlyLoadedFiles, "value",
                      SIGNAL(valueChanged(int)),
                      "Max. number of 'Recent' menu items",
                      ctkSettingsPanel::OptionRequireRestart);
}

// --------------------------------------------------------------------------
// qCjyxSettingsGeneralPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsGeneralPanel::qCjyxSettingsGeneralPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsGeneralPanelPrivate(*this))
{
  Q_D(qCjyxSettingsGeneralPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsGeneralPanel::~qCjyxSettingsGeneralPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsGeneralPanel::setDefaultScenePath(const QString& path)
{
  qCjyxCoreApplication::application()->setDefaultScenePath(path);
}

// --------------------------------------------------------------------------
void qCjyxSettingsGeneralPanel::openCjyxRCFile()
{
  Q_D(qCjyxSettingsGeneralPanel);
  QString cjyxRcFileName = d->CjyxRCFileValueLabel->text();
  QFileInfo fileInfo(cjyxRcFileName);
  if (!fileInfo.exists())
    {
    QFile outputFile(cjyxRcFileName);
    if (outputFile.open(QFile::WriteOnly | QFile::Truncate))
      {
      // cjyxrc file does not exist, create one with some default content
      QTextStream outputStream(&outputFile);
      outputStream <<
        "# Python commands in this file are executed on Cjyx startup\n"
        "\n"
        "# Examples:\n"
        "#\n"
        "# Load a scene file\n"
        "# cjyx.util.loadScene('c:/Users/SomeUser/Documents/CjyxScenes/SomeScene.mrb')\n"
        "#\n"
        "# Open a module (overrides default startup module in application settings / modules)\n"
        "# cjyx.util.mainWindow().moduleSelector().selectModule('SegmentEditor')\n"
        "#\n";
      outputFile.close();
      }
    }
  QDesktopServices::openUrl(QUrl("file:///" + cjyxRcFileName, QUrl::TolerantMode));
}
