/*=========================================================================

  Library:   CTK

  Copyright (c) Kitware Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0.txt

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=========================================================================*/

// Terminology includes
#include "qCjyxTerminologySelectorButton.h"
#include "qCjyxTerminologySelectorDialog.h"
#include "vtkCjyxTerminologyEntry.h"

// Qt includes
#include <QApplication>
#include <QDebug>
#include <QIcon>
#include <QPainter>
#include <QPixmap>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStylePainter>

class qCjyxTerminologySelectorButtonPrivate
{
  Q_DECLARE_PUBLIC(qCjyxTerminologySelectorButton);
protected:
  qCjyxTerminologySelectorButton* const q_ptr;
public:
  qCjyxTerminologySelectorButtonPrivate(qCjyxTerminologySelectorButton& object);
  void init();
  void computeIcon();
  QString text()const;

  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle TerminologyInfo;
  QIcon Icon;
  mutable QSize CachedSizeHint;
};

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorButtonPrivate::qCjyxTerminologySelectorButtonPrivate(qCjyxTerminologySelectorButton& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButtonPrivate::init()
{
  Q_Q(qCjyxTerminologySelectorButton);
  q->setCheckable(true);
  QObject::connect(q, SIGNAL(toggled(bool)),
                   q, SLOT(onToggled(bool)));
  this->computeIcon();
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButtonPrivate::computeIcon()
{
  Q_Q(qCjyxTerminologySelectorButton);

  QColor iconColor;
  if (this->TerminologyInfo.Color.isValid())
    {
    // If custom color was chosen then use that
    iconColor = this->TerminologyInfo.Color;
    }
  else
    {
    // If recommended color is used then show that
    iconColor = qCjyxTerminologyNavigatorWidget::recommendedColorFromTerminology(
      this->TerminologyInfo.GetTerminologyEntry() );
    }

  int _iconSize = q->style()->pixelMetric(QStyle::PM_SmallIconSize);
  QPixmap pix(_iconSize, _iconSize);
  pix.fill(iconColor.isValid() ? q->palette().button().color() : Qt::transparent);
  QPainter p(&pix);
  p.setPen(QPen(Qt::gray));
  p.setBrush(iconColor.isValid() ? iconColor : QBrush(Qt::NoBrush));
  p.drawRect(2, 2, pix.width() - 5, pix.height() - 5);

  this->Icon = QIcon(pix);
}

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorButton::qCjyxTerminologySelectorButton(QWidget* _parent)
  : QPushButton(_parent)
  , d_ptr(new qCjyxTerminologySelectorButtonPrivate(*this))
{
  Q_D(qCjyxTerminologySelectorButton);
  d->init();
}

//-----------------------------------------------------------------------------
qCjyxTerminologySelectorButton::~qCjyxTerminologySelectorButton() = default;

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButton::changeTerminology()
{
  Q_D(qCjyxTerminologySelectorButton);
  if (qCjyxTerminologySelectorDialog::getTerminology(d->TerminologyInfo, this))
    {
    d->computeIcon();
    this->update();
    emit terminologyChanged();
    }
  else
    {
    emit canceled();
    }
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButton::onToggled(bool change)
{
  if (change)
    {
    this->changeTerminology();
    this->setChecked(false);
    }
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButton::terminologyInfo(
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo )
{
  Q_D(qCjyxTerminologySelectorButton);
  terminologyInfo = d->TerminologyInfo;
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButton::setTerminologyInfo(
  qCjyxTerminologyNavigatorWidget::TerminologyInfoBundle &terminologyInfo )
{
  Q_D(qCjyxTerminologySelectorButton);

  d->TerminologyInfo = terminologyInfo;

  d->computeIcon();
  this->update();
}

//-----------------------------------------------------------------------------
void qCjyxTerminologySelectorButton::paintEvent(QPaintEvent *)
{
  Q_D(qCjyxTerminologySelectorButton);
  QStylePainter p(this);
  QStyleOptionButton option;
  this->initStyleOption(&option);
  option.icon = d->Icon;
  p.drawControl(QStyle::CE_PushButton, option);
}
