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

// CjyxApp includes
#include "qCjyxAboutDialog.h"
#include "qCjyxApplication.h"
#include "ui_qCjyxAboutDialog.h"

//-----------------------------------------------------------------------------
class qCjyxAboutDialogPrivate: public Ui_qCjyxAboutDialog
{
public:
};

//-----------------------------------------------------------------------------
// qCjyxAboutDialogPrivate methods


//-----------------------------------------------------------------------------
// qCjyxAboutDialog methods
qCjyxAboutDialog::qCjyxAboutDialog(QWidget* parentWidget)
 :QDialog(parentWidget)
  , d_ptr(new qCjyxAboutDialogPrivate)
{
  Q_D(qCjyxAboutDialog);
  d->setupUi(this);

  qCjyxApplication* cjyx = qCjyxApplication::application();
  d->CreditsTextBrowser->setFontPointSize(25);
  d->CreditsTextBrowser->append(cjyx->applicationName());
  d->CreditsTextBrowser->setFontPointSize(11);
  d->CreditsTextBrowser->append("");
  if (!cjyx->isCustomMainApplication())
    {
    d->CreditsTextBrowser->append(cjyx->applicationVersion() + " " + "r" + cjyx->revision()
      + " / " + cjyx->repositoryRevision());
    d->CreditsTextBrowser->append("");
    d->CreditsTextBrowser->append("");
    d->CreditsTextBrowser->insertHtml("<a href=\"https://download.slicer.org/\">Download</a> a newer version<br />");
    d->CreditsTextBrowser->append("");
    }
  else
    {
    d->CreditsTextBrowser->append(cjyx->applicationVersion() + " (" + cjyx->mainApplicationRepositoryRevision() + ")");
    d->CreditsTextBrowser->append("");
    }
  d->CreditsTextBrowser->insertHtml(cjyx->acknowledgment());
  d->CreditsTextBrowser->insertHtml(cjyx->libraries());
  d->CjyxLinksTextBrowser->insertHtml(cjyx->copyrights());
  d->CreditsTextBrowser->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);

  connect(d->ButtonBox, SIGNAL(rejected()), this, SLOT(close()));
}

//-----------------------------------------------------------------------------
void qCjyxAboutDialog::setLogo(const QPixmap& newLogo)
{
  Q_D(qCjyxAboutDialog);
  d->CjyxLabel->setPixmap(newLogo);
}

//-----------------------------------------------------------------------------
qCjyxAboutDialog::~qCjyxAboutDialog() = default;
