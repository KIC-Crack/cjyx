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
#include <QPointer>

// CTK includes
#include <ctkPythonConsole.h>

// QtGUI includes
#include "qCjyxApplication.h"
#include "qCjyxSettingsPythonPanel.h"
#include "ui_qCjyxSettingsPythonPanel.h"

// --------------------------------------------------------------------------
// qCjyxSettingsPythonPanelPrivate

//-----------------------------------------------------------------------------
class qCjyxSettingsPythonPanelPrivate: public Ui_qCjyxSettingsPythonPanel
{
  Q_DECLARE_PUBLIC(qCjyxSettingsPythonPanel);
protected:
  qCjyxSettingsPythonPanel* const q_ptr;

public:
  qCjyxSettingsPythonPanelPrivate(qCjyxSettingsPythonPanel& object);
  void init();

  QPointer<ctkPythonConsole> PythonConsole;

};

// --------------------------------------------------------------------------
// qCjyxSettingsPythonPanelPrivate methods

// --------------------------------------------------------------------------
qCjyxSettingsPythonPanelPrivate::qCjyxSettingsPythonPanelPrivate(qCjyxSettingsPythonPanel& object)
  :q_ptr(&object)
{
}

// --------------------------------------------------------------------------
void qCjyxSettingsPythonPanelPrivate::init()
{
  Q_Q(qCjyxSettingsPythonPanel);

  this->setupUi(q);
  this->PythonConsole = qCjyxApplication::application()->pythonConsole();
  if (this->PythonConsole.isNull())
    {
    qWarning() << "qCjyxSettingsPythonPanelPrivate requires a python console";
    return;
    }

  // Set default properties

  this->pythonFontButton->setCurrentFont(this->PythonConsole->shellFont());

  //
  // Connect panel widgets with associated slots
  //

  QObject::connect(this->pythonFontButton, SIGNAL(currentFontChanged(QFont)),
                   q, SLOT(onFontChanged(QFont)));

  //
  // Register settings with their corresponding widgets
  //
  q->registerProperty("Python/DockableWindow", this->DockableWindowCheckBox,
    "checked", SIGNAL(toggled(bool)),
    "Display Python interactor in a window that can be placed inside the main window.",
    ctkSettingsPanel::OptionRequireRestart);

  q->registerProperty("Python/Font", this->pythonFontButton, "currentFont",
                      SIGNAL(currentFontChanged(QFont)));
}

// --------------------------------------------------------------------------
// qCjyxSettingsPythonPanel methods

// --------------------------------------------------------------------------
qCjyxSettingsPythonPanel::qCjyxSettingsPythonPanel(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxSettingsPythonPanelPrivate(*this))
{
  Q_D(qCjyxSettingsPythonPanel);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxSettingsPythonPanel::~qCjyxSettingsPythonPanel() = default;

// --------------------------------------------------------------------------
void qCjyxSettingsPythonPanel::onFontChanged(const QFont& font)
{
  Q_D(qCjyxSettingsPythonPanel);
  d->PythonConsole->setShellFont(font);
}
