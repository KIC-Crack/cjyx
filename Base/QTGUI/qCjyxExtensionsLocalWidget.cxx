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
#include <QAbstractTextDocumentLayout>
#include <QDebug>
#include <QFileInfo>
#include <QLabel>
#include <QListWidget>
#include <QMouseEvent>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QSignalMapper>
#include <QStyledItemDelegate>
#include <QTextBlock>
#include <QTextDocument>
#include <QUrlQuery>

// Cjyx includes
#include "qCjyxExtensionsManagerModel.h"
#include "qCjyxExtensionsLocalWidget.h"
#include "ui_qCjyxExtensionsButtonBox.h"

//-----------------------------------------------------------------------------
class qCjyxExtensionsLocalWidgetPrivate
{
  Q_DECLARE_PUBLIC(qCjyxExtensionsLocalWidget);
protected:
  qCjyxExtensionsLocalWidget* const q_ptr;

public:
  typedef qCjyxExtensionsLocalWidgetPrivate Self;
  typedef qCjyxExtensionsManagerModel::ExtensionMetadataType ExtensionMetadataType;


  enum DataRoles
    {
    NameRole = Qt::UserRole,
    InstalledExtensionRevisionRole,
    InstalledExtensionCjyxVersionRole,
    InstalledExtensionUpdatedRole,
    OnServerExtensionRevisionRole,
    OnServerExtensionMissingRole,  // confirmed that the extension is missing from the server
    OnServerExtensionUpdatedRole,
    UpdateAvailableRole,
    MoreLinkRole,
    DescriptionRole,
    CompatibleRole,
    LoadedRole,
    InstalledRole,
    BookmarkedRole,
    EnabledRole,
    ScheduledForUpdateRole,
    ScheduledForUninstallRole
    };

  qCjyxExtensionsLocalWidgetPrivate(qCjyxExtensionsLocalWidget& object);
  void init();

  QListWidgetItem* extensionItem(const QString &extensionName) const;

  // Add/update/remove extension item
  QListWidgetItem* updateExtensionItem(const QString& extensionName);

  QString extensionIconPath(const QString& extensionName,
                            const QUrl& extensionIconUrl);
  QIcon extensionIcon(const QString& extensionName,
                      const QUrl& extensionIconUrl);

  QSignalMapper InstallButtonMapper;
  QSignalMapper AddBookmarkButtonMapper;
  QSignalMapper RemoveBookmarkButtonMapper;
  QSignalMapper EnableButtonMapper;
  QSignalMapper DisableButtonMapper;
  QSignalMapper ScheduleUninstallButtonMapper;
  QSignalMapper CancelScheduledUninstallButtonMapper;
  QSignalMapper ScheduleUpdateButtonMapper;
  QSignalMapper CancelScheduledUpdateButtonMapper;

  QNetworkAccessManager IconDownloadManager;
  QSignalMapper IconDownloadMapper;
  QHash<QString, QNetworkReply*> IconDownloads;

  QString SearchText;

  qCjyxExtensionsManagerModel* ExtensionsManagerModel;
};

// --------------------------------------------------------------------------
qCjyxExtensionsLocalWidgetPrivate::qCjyxExtensionsLocalWidgetPrivate(qCjyxExtensionsLocalWidget& object)
  :q_ptr(&object)
{
  this->ExtensionsManagerModel = nullptr;
}

// --------------------------------------------------------------------------
namespace
{

// --------------------------------------------------------------------------
class qCjyxExtensionsButtonBox : public QWidget, public Ui_qCjyxExtensionsButtonBox
{
public:
  typedef QWidget Superclass;
  qCjyxExtensionsButtonBox(QListWidgetItem* widgetItem, QWidget* parent = nullptr)
    : Superclass(parent)
    , WidgetItem(widgetItem)
    {
    this->setupUi(this);
    this->InstallProgress->setVisible(false);
    this->UpdateProgress->setVisible(false);
  }

  void updateFromWidgetItem()
  {
    bool installed = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledRole).toBool();
    bool compatible = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::CompatibleRole).toBool();
    bool loaded = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::LoadedRole).toBool();
    bool bookmarked = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::BookmarkedRole).toBool();
    bool enabled = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::EnabledRole).toBool();
    bool scheduledForUpdate = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::ScheduledForUpdateRole).toBool();
    bool scheduledForUninstall = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::ScheduledForUninstallRole).toBool();
    bool updateAvailable = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::UpdateAvailableRole).toBool();
    // Available if already installed (e.g., from file) or has found a revision on server
    bool available = installed || !this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::OnServerExtensionRevisionRole).toString().isEmpty();

    this->InstallButton->setVisible(!installed);
    this->InstallButton->setEnabled(compatible && available);

    if (installed && !loaded)
      {
      this->StatusLabel->setVisible(true);
      this->StatusLabel->setText(tr("Install pending restart"));
      }
    else if (scheduledForUpdate)
      {
      this->StatusLabel->setVisible(true);
      this->StatusLabel->setText(tr("Update pending restart"));
      }
    else if (scheduledForUninstall)
      {
      this->StatusLabel->setVisible(true);
      this->StatusLabel->setText(tr("Uninstall pending restart"));
      }
    else
      {
      this->StatusLabel->setVisible(false);
      }

    this->AddBookmarkButton->setVisible(!bookmarked);
    this->RemoveBookmarkButton->setVisible(bookmarked);

    this->EnableButton->setVisible(!enabled && installed && !scheduledForUninstall);
    this->EnableButton->setEnabled(compatible);
    this->DisableButton->setVisible(enabled && loaded && installed && !scheduledForUninstall && !scheduledForUpdate);

    this->ScheduleForUninstallButton->setVisible(!scheduledForUninstall && installed);
    this->CancelScheduledForUninstallButton->setVisible(scheduledForUninstall);

    this->UpdateOptionsWidget->setVisible(updateAvailable);

    this->ScheduleForUpdateButton->setVisible(!scheduledForUpdate && !scheduledForUninstall);
    this->CancelScheduledForUpdateButton->setVisible(scheduledForUpdate);
  }

  QListWidgetItem* WidgetItem;
};

} // end of anonymous namespace

// --------------------------------------------------------------------------
class qCjyxExtensionsItemDelegate : public QStyledItemDelegate
{
public:
  qCjyxExtensionsItemDelegate(qCjyxExtensionsLocalWidget * list,
                                QObject * parent = nullptr)
    : QStyledItemDelegate(parent), List(list) {}

  // --------------------------------------------------------------------------
  void paint(QPainter * painter, const QStyleOptionViewItem& option,
                     const QModelIndex& index) const override
  {
    QStyleOptionViewItem modifiedOption = option;
    QListWidgetItem * const item = this->List->itemFromIndex(index);
    if (item && !item->data(qCjyxExtensionsLocalWidgetPrivate::EnabledRole).toBool())
      {
      modifiedOption.state &= ~QStyle::State_Enabled;
      }
    QStyledItemDelegate::paint(painter, modifiedOption, index);
  }

protected:
  qCjyxExtensionsLocalWidget * const List;
};

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidgetPrivate::init()
{
  Q_Q(qCjyxExtensionsLocalWidget);

  q->setAlternatingRowColors(true);
  q->setSelectionMode(QAbstractItemView::NoSelection);
  q->setIconSize(QSize(64, 64));
  q->setSpacing(1);
  q->setItemDelegate(new qCjyxExtensionsItemDelegate(q, q));

  QObject::connect(&this->InstallButtonMapper, SIGNAL(mapped(QString)), q, SLOT(installExtension(QString)));
  QObject::connect(&this->AddBookmarkButtonMapper, SIGNAL(mapped(QString)), q, SLOT(addBookmark(QString)));
  QObject::connect(&this->RemoveBookmarkButtonMapper, SIGNAL(mapped(QString)), q, SLOT(removeBookmark(QString)));
  QObject::connect(&this->EnableButtonMapper, SIGNAL(mapped(QString)), q, SLOT(setExtensionEnabled(QString)));
  QObject::connect(&this->DisableButtonMapper, SIGNAL(mapped(QString)), q, SLOT(setExtensionDisabled(QString)));
  QObject::connect(&this->ScheduleUninstallButtonMapper, SIGNAL(mapped(QString)), q, SLOT(scheduleExtensionForUninstall(QString)));
  QObject::connect(&this->CancelScheduledUninstallButtonMapper, SIGNAL(mapped(QString)), q, SLOT(cancelExtensionScheduledForUninstall(QString)));
  QObject::connect(&this->ScheduleUpdateButtonMapper, SIGNAL(mapped(QString)), q, SLOT(scheduleExtensionForUpdate(QString)));
  QObject::connect(&this->CancelScheduledUpdateButtonMapper, SIGNAL(mapped(QString)), q, SLOT(cancelExtensionScheduledForUpdate(QString)));
  QObject::connect(&this->IconDownloadMapper, SIGNAL(mapped(QString)), q, SLOT(onIconDownloadComplete(QString)));
}

// --------------------------------------------------------------------------
QString qCjyxExtensionsLocalWidgetPrivate::extensionIconPath(
  const QString& extensionName, const QUrl& extensionIconUrl)
{
  return QString("%1/%2-icon.%3").arg(
    this->ExtensionsManagerModel->extensionsInstallPath(),
    extensionName, QFileInfo(extensionIconUrl.path()).suffix());
}

// --------------------------------------------------------------------------
QIcon qCjyxExtensionsLocalWidgetPrivate::extensionIcon(
  const QString& extensionName, const QUrl& extensionIconUrl)
{
  Q_Q(qCjyxExtensionsLocalWidget);

  if (extensionIconUrl.isValid())
    {
    const QString iconPath = this->extensionIconPath(extensionName, extensionIconUrl);
    if (QFileInfo(iconPath).exists())
      {
      QPixmap pixmap(iconPath);
      pixmap = pixmap.scaled(q->iconSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

      if (pixmap.isNull())
        {
        // Use default icon if unable to load extension icon
        return this->extensionIcon(QString(), QUrl());
        }

      QPixmap canvas(pixmap.size());
      canvas.fill(Qt::transparent);

      QPainter painter;
      painter.begin(&canvas);
      painter.setPen(Qt::NoPen);
      painter.setBrush(pixmap);
      painter.setRenderHint(QPainter::Antialiasing, true);
      painter.drawRoundedRect(QRect(QPoint(0, 0), pixmap.size()), 5, 5);
      painter.end();

      return QIcon(canvas);
      }

    if (!this->IconDownloads.contains(extensionName))
      {
      // Try to download icon
      QNetworkRequest req(extensionIconUrl);
      // Icons are hosted on random servers, which often use redirects (301 redirect) to get the actual download URL.
      // In Qt6, redirects are followed by default, but it has to be manually enabled in Qt5.
      req.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
      QNetworkReply* const reply = this->IconDownloadManager.get(req);

      this->IconDownloads.insert(extensionName, reply);
      this->IconDownloadMapper.setMapping(reply, extensionName);

      QObject::connect(reply, SIGNAL(finished()), &this->IconDownloadMapper, SLOT(map()));
      }
    else
      {
      qDebug() << "Icon download for " << extensionName << " already in progress. Active icon downloads: " << this->IconDownloads.keys();
      }

    }

  return QIcon(":/Icons/ExtensionDefaultIcon.png");
}

// --------------------------------------------------------------------------
QListWidgetItem* qCjyxExtensionsLocalWidgetPrivate::extensionItem(const QString& extensionName) const
{
  Q_Q(const qCjyxExtensionsLocalWidget);

  QAbstractItemModel* model = q->model();
  const QModelIndexList indices = model->match(model->index(0, 0, QModelIndex()),
    Self::NameRole, extensionName, 2, Qt::MatchExactly);

  Q_ASSERT(indices.count() < 2);
  if (indices.count() == 1)
    {
    return q->item(indices.first().row());
    }
  return nullptr;
}

namespace
{

// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
class qCjyxExtensionsDescriptionLabel : public QLabel
{
public:
  typedef QLabel Superclass;

  // --------------------------------------------------------------------------
  qCjyxExtensionsDescriptionLabel(const QString& extensionName, const QString& cjyxRevision, QListWidgetItem* item)
    : QLabel(), ExtensionName(extensionName), CjyxRevision(cjyxRevision), WidgetItem(item)
    {
    QTextOption textOption = this->Text.defaultTextOption();
    textOption.setWrapMode(QTextOption::NoWrap);
    this->Text.setDefaultTextOption(textOption);
    this->MoreLinkText = tr("More");
    }

  // --------------------------------------------------------------------------
  void updateFromWidgetItem()
    {
    this->LastWidth = -1; // force update in next paint
    this->update();
    }

  QListWidgetItem* widgetItem()
    {
    return this->WidgetItem;
    }

  // --------------------------------------------------------------------------
  QSize sizeHint() const override
    {
    QSize hint = this->Superclass::sizeHint();
    hint.setHeight(qRound(this->Text.size().height() + 0.5) +
                   this->margin() * 2);
    return hint;
    }


protected:

  QString versionString(const QString& revision, const QString& isoDateStr)
  {
    // Get formatted date
    QString formattedDate;
    QDateTime date = QDateTime::fromString(isoDateStr, Qt::ISODate);
    if (date.isValid())
      {
      formattedDate = date.toString("yyyy-MM-dd");
      }
    if (!revision.isEmpty() && !formattedDate.isEmpty())
      {
      return QString("%1 (%2)").arg(revision).arg(formattedDate);
      }
    else if (revision.isEmpty() && formattedDate.isEmpty())
      {
      return tr("unknown");
      }
    else
      {
      return revision + formattedDate;
      }
  }

  // --------------------------------------------------------------------------
  void labelText(const QString& elidedDescription)
    {
    QString extensionDescription = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::DescriptionRole).toString();

    QString labelText;

    labelText += QString("<h2>%1</h2>").arg(this->ExtensionName);

    // Warnings/notices
    bool compatible = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::CompatibleRole).toBool();
    bool enabled = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::EnabledRole).toBool();
    bool installed = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledRole).toBool();
    // Available if already installed (e.g., from file) or has found a revision on server
    bool available = installed || !this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::OnServerExtensionRevisionRole).toString().isEmpty();
    // Confirmed to be missing from server
    bool missing = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::OnServerExtensionMissingRole).toBool();
    bool scheduledForUpdate = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::ScheduledForUpdateRole).toBool();

    QString installedVersion = this->versionString(
      this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledExtensionRevisionRole).toString(),
      this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledExtensionUpdatedRole).toString());

    // Status line
    QString statusText;
    if (!available && missing) // "missing" is checked so that we don't display this message when there have been no update checks before
      {
      statusText += QString("<p style=\"font-weight: bold; font-size: 80%; color: %1;\">"
        "<img style=\"float: left\" src=\":/Icons/ExtensionIncompatible.svg\"/> "
        "Not found for this version of the application (r%2)</p>")
        .arg(this->WarningColor)
        .arg(this->CjyxRevision);
      }
    if (!compatible)
      {
      statusText += QString("<p style=\"font-weight: bold; font-size: 80%; color: %1;\">"
        "<img style=\"float: left\" src=\":/Icons/ExtensionIncompatible.svg\"/> "
        "Incompatible with Cjyx r%2 [built for r%3]</p>")
        .arg(this->WarningColor)
        .arg(this->CjyxRevision)
        .arg(this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledExtensionCjyxVersionRole).toString());
      }
    if (this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::UpdateAvailableRole).toBool() && !scheduledForUpdate)
      {
      QString onServerVersion = this->versionString(
        this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::OnServerExtensionRevisionRole).toString(),
        this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::OnServerExtensionUpdatedRole).toString());

      statusText += QString("<p style=\"font-weight: bold; font-size: 80%; color: %1;\">"
        "<img style=\"float: left\""
        " src=\":/Icons/ExtensionUpdateAvailable.svg\"/> "
        "An update is available. Installed version: %2. Available version: %3 </p>")
        .arg(this->InfoColor)
        .arg(installedVersion)
        .arg(onServerVersion);
      }
    if (statusText.isEmpty())
      {
      // if there are no notices than just add version information or an empty line (to make the layout more balanced)
      if (installed)
        {
        // Version

        if (!enabled || !compatible)
          {
          statusText += tr("<p>Version: %1. Disabled.</p>").arg(installedVersion);
          }
        else
          {
          statusText += tr("<p>Version: %1</p>").arg(installedVersion);
          }
        }
      else
        {
        //labelText += "<p>&nbsp;</p>";
        statusText += "<p>Not installed.</p>";
        }
      }
    labelText += statusText;

    // Description and link
    QString linkText;
    QString moreLink = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::MoreLinkRole).toString();
    if (!moreLink.isEmpty() && !missing)
      {
      linkText = QString(" <a href=\"%1\">%2</a>").arg(moreLink).arg(this->MoreLinkText);
      }
    labelText += QString("<p>%1%2</p>").arg(elidedDescription).arg(linkText);

    this->Text.setHtml(labelText);
    this->setToolTip(extensionDescription);
    this->setMouseTracking(!moreLink.isEmpty());
    }

  // --------------------------------------------------------------------------
  void paintEvent(QPaintEvent *) override
    {
    QPainter painter(this);
    const QRect cr = this->contentsRect();

    if (this->LastWidth != cr.width())
      {
      // Changed description text or available space, update elided text
      int margin = this->margin() * 2;
      bool moreLinkSpecified = !this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::MoreLinkRole).toString().isEmpty();
      if (moreLinkSpecified)
        {
        QString moreLinkText = QString(" %1").arg(this->MoreLinkText);
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
        margin += this->fontMetrics().horizontalAdvance(moreLinkText);
#else
        margin += this->fontMetrics().width(moreLinkText);
#endif
        }
      QString extensionDescription = this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::DescriptionRole).toString();
      QString elidedExtensionDescription = this->fontMetrics().elidedText(extensionDescription, Qt::ElideRight, cr.width() - margin);
      this->labelText(elidedExtensionDescription);
      this->LastWidth = cr.width();
      this->Text.setTextWidth(this->LastWidth);
      }

    QAbstractTextDocumentLayout::PaintContext context;
    context.palette = this->palette();
    if (!this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::InstalledRole).toBool()
      || this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::ScheduledForUninstallRole).toBool()
      || !this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::CompatibleRole).toBool()
      || !this->WidgetItem->data(qCjyxExtensionsLocalWidgetPrivate::EnabledRole).toBool())
      {
      context.palette.setCurrentColorGroup(QPalette::Disabled);
      }

    painter.translate(cr.topLeft());
    this->Text.documentLayout()->draw(&painter, context);
    }

  // --------------------------------------------------------------------------
  QString linkUnderCursor(const QPoint& pos)
    {
    const int caretPos =
      this->Text.documentLayout()->hitTest(pos, Qt::FuzzyHit);
    if (caretPos < 0)
      {
      return QString();
      }

    const QTextBlock& block = this->Text.findBlock(caretPos);
    for (QTextBlock::iterator iter = block.begin(); !iter.atEnd(); ++iter)
      {
      const QTextFragment& fragment = iter.fragment();
      const int fp = fragment.position();
      if (fp <= caretPos && fp + fragment.length() > caretPos)
        {
        return fragment.charFormat().anchorHref();
        }
      }

    return QString();
    }

  // --------------------------------------------------------------------------
  void mouseMoveEvent(QMouseEvent * e) override
    {
    Superclass::mouseMoveEvent(e);

    QString href = this->linkUnderCursor(e->pos());
    if (href != this->LinkUnderCursor)
      {
      this->LinkUnderCursor = href;
      if (href.isEmpty())
        {
        this->unsetCursor();
        }
      else
        {
        this->setCursor(Qt::PointingHandCursor);
        emit this->linkHovered(href);
        }
      }
    }

  // --------------------------------------------------------------------------
  void mouseReleaseEvent(QMouseEvent * e) override
    {
    Superclass::mouseReleaseEvent(e);
    if (e->button() == Qt::LeftButton)
      {
      QString href = this->linkUnderCursor(e->pos());
      if (!href.isEmpty())
        {
        emit this->linkActivated(href);
        }
      }
    }

  const QString ExtensionName;
  const QString CjyxRevision;
  const QString WarningColor{ "#bd8530" };
  const QString InfoColor{ "#2c70c8" };
  QListWidgetItem* WidgetItem;

  QString MoreLinkText; // used for computing available space for the extension description
  QString LinkUnderCursor; // used for detecting if the mouse is over the "More" link
  QTextDocument Text; // stores displayed text and format
  int LastWidth{ -1 };  // to avoid assembling Text on every paint
};

// --------------------------------------------------------------------------
class qCjyxExtensionsItemWidget : public QWidget
{
public:
  qCjyxExtensionsItemWidget(qCjyxExtensionsDescriptionLabel* label, QWidget* parent = nullptr)
    : QWidget(parent), Label(label)
  {
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(label, 1);
    this->ButtonBox = new qCjyxExtensionsButtonBox(label->widgetItem());
    layout->addWidget(this->ButtonBox);
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);

    label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  }

  // --------------------------------------------------------------------------
  void updateFromWidgetItem()
    {
    this->Label->updateFromWidgetItem();
    this->ButtonBox->updateFromWidgetItem();
    }

  qCjyxExtensionsDescriptionLabel* Label;
  qCjyxExtensionsButtonBox* ButtonBox;
};

} // end of anonymous namespace

// --------------------------------------------------------------------------
QListWidgetItem* qCjyxExtensionsLocalWidgetPrivate::updateExtensionItem(const QString& extensionName)
{
  Q_Q(qCjyxExtensionsLocalWidget);
  if (extensionName.isEmpty())
    {
    qCritical() << "Missing metadata identified with 'extensionname' key";
    return nullptr;
    }
  QListWidgetItem* item = this->extensionItem(extensionName);
  qCjyxExtensionsManagerModel::ExtensionMetadataType metadata =
    this->ExtensionsManagerModel->extensionMetadata(extensionName);
  qCjyxExtensionsManagerModel::ExtensionMetadataType metadataFromServer =
    this->ExtensionsManagerModel->extensionMetadata(extensionName, qCjyxExtensionsManagerModel::MetadataServer);

  // Remove the item if it is no longer bookmarked nor installed
  bool removeItem = false;
  if (!metadata["bookmarked"].toBool() && !metadata["installed"].toBool())
    {
    removeItem = true;
    }

  // Remove the imte if does not match search text
  if (!removeItem && !this->SearchText.isEmpty())
    {
    // filtering by text is enabled, we may need to remove this item

    // Include extension if search text is found in extension name or description
    bool includeExtension = extensionName.contains(this->SearchText, Qt::CaseInsensitive);
    if (!includeExtension)
      {
      QString description = this->ExtensionsManagerModel->extensionDescription(extensionName);
      includeExtension = description.contains(this->SearchText, Qt::CaseInsensitive);
      }
    if (!includeExtension)
      {
      removeItem = true;
      }
    }

  // remove item as needed
  if (removeItem)
    {
    if (item)
      {
      // this item should not exist
      QAbstractItemModel* model = q->model();
      const QModelIndexList indices = model->match(model->index(0, 0, QModelIndex()), Self::NameRole, extensionName, 1, Qt::MatchExactly);
      if (indices.count() > 0)
        {
        q->model()->removeRow(indices.first().row());
        }
      }
    return nullptr;
    }

  // add new item as needed
  bool newItemCreated = false;
  if (!item)
    {
    item = new QListWidgetItem();
    newItemCreated = true;
    }

  item->setIcon(this->extensionIcon(extensionName, metadata.value("iconurl").toUrl()));

  QString moreLinkTarget;
  int serverAPI = q->extensionsManagerModel()->serverAPI();
  if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
    {
    moreLinkTarget = QString("cjyx:%1").arg(extensionName);
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
    }

  // Save some metadata fields into the item to allow filtering and search

  item->setData(Self::NameRole, extensionName);
  item->setData(Self::InstalledExtensionRevisionRole, metadata["installed"].toBool() ? metadata["revision"].toString() : QString());
  item->setData(Self::InstalledExtensionUpdatedRole, metadata["installed"].toBool() ? metadata["updated"].toString() : QString());
  item->setData(Self::InstalledExtensionCjyxVersionRole, metadata["installed"].toBool() ? metadata["cjyx_revision"].toString() : QString());
  item->setData(Self::OnServerExtensionRevisionRole, metadataFromServer["revision"].toString());
  item->setData(Self::OnServerExtensionMissingRole, metadataFromServer["revision"].toString().isEmpty()
    && this->ExtensionsManagerModel->lastUpdateTimeExtensionsMetadataFromServer().isValid());
  item->setData(Self::OnServerExtensionUpdatedRole, metadataFromServer["updated"].toString());
  item->setData(Self::UpdateAvailableRole, q->extensionsManagerModel()->isExtensionUpdateAvailable(extensionName));
  item->setData(Self::MoreLinkRole, moreLinkTarget);
  item->setData(Self::DescriptionRole, metadata["description"].toString());
  item->setData(Self::CompatibleRole, q->extensionsManagerModel()->isExtensionCompatible(extensionName).isEmpty());
  item->setData(Self::LoadedRole, metadata["loaded"].toBool());
  item->setData(Self::InstalledRole, metadata["installed"].toBool());
  item->setData(Self::BookmarkedRole, metadata["bookmarked"].toBool());
  item->setData(Self::EnabledRole, metadata["enabled"].toBool());
  item->setData(Self::ScheduledForUpdateRole, q->extensionsManagerModel()->isExtensionScheduledForUpdate(extensionName));
  item->setData(Self::ScheduledForUninstallRole, q->extensionsManagerModel()->isExtensionScheduledForUninstall(extensionName));

  if (newItemCreated)
    {

    // Initialize label
    qCjyxExtensionsDescriptionLabel* label = new qCjyxExtensionsDescriptionLabel(extensionName, q->extensionsManagerModel()->cjyxRevision(), item);
    label->setMargin(6);
    QObject::connect(label, SIGNAL(linkActivated(QString)), q, SLOT(onLinkActivated(QString)));

    qCjyxExtensionsItemWidget* widget = new qCjyxExtensionsItemWidget(label);

    widget->updateFromWidgetItem();


    this->AddBookmarkButtonMapper.setMapping(widget->ButtonBox->AddBookmarkButton, extensionName);
    QObject::connect(widget->ButtonBox->AddBookmarkButton, SIGNAL(clicked()), &this->AddBookmarkButtonMapper, SLOT(map()));
    this->RemoveBookmarkButtonMapper.setMapping(widget->ButtonBox->RemoveBookmarkButton, extensionName);
    QObject::connect(widget->ButtonBox->RemoveBookmarkButton, SIGNAL(clicked()), &this->RemoveBookmarkButtonMapper, SLOT(map()));

    this->InstallButtonMapper.setMapping(widget->ButtonBox->InstallButton, extensionName);
    QObject::connect(widget->ButtonBox->InstallButton, SIGNAL(clicked()), &this->InstallButtonMapper, SLOT(map()));

    this->ScheduleUpdateButtonMapper.setMapping(widget->ButtonBox->ScheduleForUpdateButton, extensionName);
    QObject::connect(widget->ButtonBox->ScheduleForUpdateButton, SIGNAL(clicked()), &this->ScheduleUpdateButtonMapper, SLOT(map()));
    this->CancelScheduledUpdateButtonMapper.setMapping(widget->ButtonBox->CancelScheduledForUpdateButton, extensionName);
    QObject::connect(widget->ButtonBox->CancelScheduledForUpdateButton, SIGNAL(clicked()), &this->CancelScheduledUpdateButtonMapper, SLOT(map()));

    this->EnableButtonMapper.setMapping(widget->ButtonBox->EnableButton, extensionName);
    QObject::connect(widget->ButtonBox->EnableButton, SIGNAL(clicked()), &this->EnableButtonMapper, SLOT(map()));
    this->DisableButtonMapper.setMapping(widget->ButtonBox->DisableButton, extensionName);
    QObject::connect(widget->ButtonBox->DisableButton, SIGNAL(clicked()), &this->DisableButtonMapper, SLOT(map()));

    this->ScheduleUninstallButtonMapper.setMapping(widget->ButtonBox->ScheduleForUninstallButton, extensionName);
    QObject::connect(widget->ButtonBox->ScheduleForUninstallButton, SIGNAL(clicked()), &this->ScheduleUninstallButtonMapper, SLOT(map()));
    this->CancelScheduledUninstallButtonMapper.setMapping(widget->ButtonBox->CancelScheduledForUninstallButton, extensionName);
    QObject::connect(widget->ButtonBox->CancelScheduledForUninstallButton, SIGNAL(clicked()), &this->CancelScheduledUninstallButtonMapper, SLOT(map()));

    QSize hint = label->sizeHint();
    hint.setWidth(hint.width() + 64);
    hint.setHeight(qMax(hint.height(), widget->ButtonBox->minimumSizeHint().height()));
    item->setSizeHint(hint);

    q->addItem(item);
    q->setItemWidget(item, widget);
    }
  else
    {
    qCjyxExtensionsItemWidget* widget = dynamic_cast<qCjyxExtensionsItemWidget*>(q->itemWidget(item));
    if (!widget)
      {
      qWarning() << Q_FUNC_INFO << " failed: cannot update list item for extension " << extensionName;
      return nullptr;
      }
    widget->updateFromWidgetItem();
    }
  return item;
}

// --------------------------------------------------------------------------
qCjyxExtensionsLocalWidget::qCjyxExtensionsLocalWidget(QWidget* _parent)
  : Superclass(_parent)
  , d_ptr(new qCjyxExtensionsLocalWidgetPrivate(*this))
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->init();
}

// --------------------------------------------------------------------------
qCjyxExtensionsLocalWidget::~qCjyxExtensionsLocalWidget() = default;

// --------------------------------------------------------------------------
qCjyxExtensionsManagerModel* qCjyxExtensionsLocalWidget::extensionsManagerModel()const
{
  Q_D(const qCjyxExtensionsLocalWidget);
  return d->ExtensionsManagerModel;
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionsManagerModel(qCjyxExtensionsManagerModel* model)
{
  Q_D(qCjyxExtensionsLocalWidget);

  if (this->extensionsManagerModel() == model)
    {
    return;
    }

  disconnect(this, SLOT(onModelUpdated()));
  disconnect(this, SLOT(onExtensionInstalled(QString)));
  disconnect(this, SLOT(onExtensionUninstalled(QString)));
  disconnect(this, SLOT(onExtensionMetadataUpdated(QString)));
  disconnect(this, SLOT(onExtensionScheduledForUninstall(QString)));
  disconnect(this, SLOT(onExtensionCancelledScheduleForUninstall(QString)));
  disconnect(this, SLOT(onModelExtensionEnabledChanged(QString,bool)));
  disconnect(this, SLOT(onExtensionBookmarkedChanged(QString,bool)));
  disconnect(this, SLOT(setExtensionUpdateDownloadProgress(QString,qint64,qint64)));
  disconnect(this, SLOT(setExtensionInstallDownloadProgress(QString,qint64,qint64)));


  d->ExtensionsManagerModel = model;

  if (d->ExtensionsManagerModel)
    {
    connect(d->ExtensionsManagerModel, SIGNAL(modelUpdated()), this, SLOT(onModelUpdated()));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionInstalled(QString)), this, SLOT(onExtensionInstalled(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionUninstalled(QString)), this, SLOT(onExtensionUninstalled(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionMetadataUpdated(QString)), this, SLOT(onExtensionMetadataUpdated(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionBookmarkedChanged(QString, bool)), this, SLOT(onExtensionBookmarkedChanged(QString,bool)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionScheduledForUninstall(QString)), this, SLOT(onExtensionScheduledForUninstall(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionCancelledScheduleForUninstall(QString)), this, SLOT(onExtensionCancelledScheduleForUninstall(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionEnabledChanged(QString,bool)), this, SLOT(onModelExtensionEnabledChanged(QString,bool)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionUpdateAvailable(QString)), this, SLOT(setExtensionUpdateAvailable(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionScheduledForUpdate(QString)), this, SLOT(setExtensionUpdateScheduled(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(extensionCancelledScheduleForUpdate(QString)), this, SLOT(setExtensionUpdateCanceled(QString)));
    connect(d->ExtensionsManagerModel, SIGNAL(updateDownloadProgress(QString,qint64,qint64)),
      this, SLOT(setExtensionUpdateDownloadProgress(QString,qint64,qint64)));
    connect(d->ExtensionsManagerModel, SIGNAL(installDownloadProgress(QString, qint64, qint64)),
      this, SLOT(setExtensionInstallDownloadProgress(QString, qint64, qint64)));
    this->onModelUpdated();
  }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::displayExtensionDetails(const QString& extensionName)
{
  Q_UNUSED(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::installExtension(const QString& extensionName)
{
  Q_D(const qCjyxExtensionsLocalWidget);
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->downloadAndInstallExtensionByName(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::addBookmark(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->setExtensionBookmarked(extensionName, true);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::removeBookmark(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->setExtensionBookmarked(extensionName, false);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionEnabled(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->setExtensionEnabled(extensionName, true);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionDisabled(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->setExtensionEnabled(extensionName, false);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::scheduleExtensionForUninstall(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  if (this->extensionsManagerModel()->isExtensionLoaded(extensionName))
    {
    // Extension is loaded into memory, it cannot be uninstalled immediately,
    // but only at the next startup
    this->extensionsManagerModel()->scheduleExtensionForUninstall(extensionName);
    }
  else
    {
    // Extension is not loaded into memory yet, we can uninstall now
    this->extensionsManagerModel()->uninstallExtension(extensionName);
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::cancelExtensionScheduledForUninstall(const QString& extensionName)
{
  if (!this->extensionsManagerModel())
    {
    return;
    }
  this->extensionsManagerModel()->cancelExtensionScheduledForUninstall(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionMetadataUpdated(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionInstalled(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionUninstalled(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionBookmarkedChanged(const QString& extensionName, bool bookmarked)
{
  Q_UNUSED(bookmarked);
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionScheduledForUninstall(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// -------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onExtensionCancelledScheduleForUninstall(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionUpdateAvailable(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::scheduleExtensionForUpdate(const QString& extensionName)
{
  qCjyxExtensionsManagerModel* const model = this->extensionsManagerModel();
  if (!model)
    {
    return;
    }
  model->scheduleExtensionForUpdate(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::cancelExtensionScheduledForUpdate(const QString& extensionName)
{
  qCjyxExtensionsManagerModel* const model = this->extensionsManagerModel();
  if (!model)
    {
    return;
    }
  model->cancelExtensionScheduledForUpdate(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionUpdateScheduled(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionUpdateCanceled(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionUpdateDownloadProgress(
  const QString& extensionName, qint64 received, qint64 total)
{
  Q_D(qCjyxExtensionsLocalWidget);
  QListWidgetItem* const item = d->extensionItem(extensionName);
  Q_ASSERT(item);
  qCjyxExtensionsItemWidget* const widget = dynamic_cast<qCjyxExtensionsItemWidget*>(this->itemWidget(item));
  Q_ASSERT(widget);

  if (total < 0)
    {
    widget->ButtonBox->UpdateProgress->setRange(0, 0);
    widget->ButtonBox->UpdateProgress->setValue(0);
    }
  else
    {
    while (total > (1LL << 31))
      {
      total >>= 1;
      received >>= 1;
      }

    widget->ButtonBox->UpdateProgress->setRange(0, static_cast<int>(total));
    widget->ButtonBox->UpdateProgress->setValue(static_cast<int>(received));
    }

  if (received == total)
    {
    d->updateExtensionItem(extensionName);
    widget->ButtonBox->UpdateProgress->setVisible(false);
    }
  else
    {
    d->updateExtensionItem(extensionName);
    widget->ButtonBox->UpdateProgress->setVisible(true);
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setExtensionInstallDownloadProgress(
  const QString& extensionName, qint64 received, qint64 total)
{
  Q_D(qCjyxExtensionsLocalWidget);
  QListWidgetItem* const item = d->extensionItem(extensionName);
  if (!item)
    {
    return;
    }
  qCjyxExtensionsItemWidget* const widget = dynamic_cast<qCjyxExtensionsItemWidget*>(this->itemWidget(item));
  if (!widget)
    {
    return;
    }

  if (total < 0)
    {
    widget->ButtonBox->InstallProgress->setRange(0, 0);
    widget->ButtonBox->InstallProgress->setValue(0);
    }
  else
    {
    while (total > (1LL << 31))
      {
      total >>= 1;
      received >>= 1;
      }

    widget->ButtonBox->InstallProgress->setRange(0, static_cast<int>(total));
    widget->ButtonBox->InstallProgress->setValue(static_cast<int>(received));
    }

  if (received == total)
    {
    widget->ButtonBox->InstallProgress->setVisible(false);
    d->updateExtensionItem(extensionName);
  }
  else
    {
    d->updateExtensionItem(extensionName);
    widget->ButtonBox->InstallProgress->setVisible(true);
    widget->ButtonBox->InstallButton->setVisible(false);
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onModelExtensionEnabledChanged(const QString &extensionName, bool enabled)
{
  Q_UNUSED(enabled);
  Q_D(qCjyxExtensionsLocalWidget);
  d->updateExtensionItem(extensionName);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onModelUpdated()
{
  Q_D(qCjyxExtensionsLocalWidget);
  this->clear();
  QStringList managedExtensions = d->ExtensionsManagerModel->managedExtensions();
  foreach(const QString& extensionName, managedExtensions)
    {
    d->updateExtensionItem(extensionName);
    }
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onLinkActivated(const QString& link)
{
  Q_D(qCjyxExtensionsLocalWidget);

  QUrl url = d->ExtensionsManagerModel->frontendServerUrl();
  int serverAPI = this->extensionsManagerModel()->serverAPI();
  if (serverAPI == qCjyxExtensionsManagerModel::Girder_v1)
    {
    QString extensionName = link.mid(7); // remove leading "cjyx:"
    url.setPath(url.path() + QString("/view/%1/%2/%3")
                .arg(extensionName)
                .arg(this->extensionsManagerModel()->cjyxRevision())
                .arg(this->extensionsManagerModel()->cjyxOs()));
    }
  else
    {
    qWarning() << Q_FUNC_INFO << " failed: missing implementation for serverAPI" << serverAPI;
    return;
    }

  emit this->linkActivated(url);
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::onIconDownloadComplete(const QString& extensionName)
{
  Q_D(qCjyxExtensionsLocalWidget);

  QNetworkReply* const reply = d->IconDownloads.take(extensionName);
  if (!reply)
    {
    qWarning() << Q_FUNC_INFO << " failed: invalid reply";
    return;
    }

  if (reply->error() == QNetworkReply::NoError)
    {
    QFile iconFile(d->extensionIconPath(extensionName, reply->url()));
    if (iconFile.open(QIODevice::WriteOnly))
      {
      iconFile.write(reply->readAll());
      iconFile.close(); // Ensure file written to disk before we try to load it

      QListWidgetItem* item = d->extensionItem(extensionName);
      if (item)
        {
        item->setIcon(d->extensionIcon(extensionName, reply->url()));
        }
      }
    }

  reply->deleteLater();
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::setSearchText(const QString& newText)
{
  Q_D(qCjyxExtensionsLocalWidget);
  if (d->SearchText == newText)
    {
    return;
    }
  d->SearchText = newText;
  this->onModelUpdated();
}

// --------------------------------------------------------------------------
QString qCjyxExtensionsLocalWidget::searchText() const
{
  Q_D(const qCjyxExtensionsLocalWidget);
  return d->SearchText;
}

// --------------------------------------------------------------------------
void qCjyxExtensionsLocalWidget::refresh()
{
  Q_D(const qCjyxExtensionsLocalWidget);
  if (this->extensionsManagerModel())
    {
    this->extensionsManagerModel()->updateExtensionsMetadataFromServer();
    this->extensionsManagerModel()->updateModel();
    }
}
