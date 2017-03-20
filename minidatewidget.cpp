#include "minidatewidget.h"
#include "minimode.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTime>
#include <QStyleOption>
#include <QPainter>
using namespace MiniMode;
MiniDateWidget::MiniDateWidget(QWidget *parent) : QWidget(parent) {
  setup();

  mUpdateTimer.setInterval(500);
  mUpdateTimer.setSingleShot(false);
  connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateRemaintimeLabel()));
  connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(updateTodayLabel()));
  setInterval (0);
  updateTodayLabel ();

  ///Modified by BiXiaoxia 2016.12.23,调整布局，添加横线
  QVBoxLayout *pMainLayout = new QVBoxLayout;
  pMainLayout->setContentsMargins(0, 6, 0, 0);

  QHBoxLayout *pHLayout = new QHBoxLayout;
  pHLayout->addWidget(mRemainTimeLabel);
  pHLayout->addWidget(mTodayLabel);

  //横线.
  QFrame *pHLine = new QFrame;
  pHLine->setFrameShape(QFrame::HLine);
  pHLine->setFrameShadow(QFrame::Plain);
  pHLine->setFixedHeight(1);
  pHLine->setStyleSheet("border: 1px solid #e9e9e9");

  pMainLayout->addLayout(pHLayout);
  pMainLayout->addWidget(pHLine);

  setLayout(pMainLayout);

  setStyleSheet("border-width:0px");
}

void MiniDateWidget::setInterval(int msec) {
  mRemainTime.setInterval(msec);
  updateRemaintimeLabel();
}

void MiniDateWidget::setTimer(bool on) {
  if (on)
  {
      mRemainTime.start ();
      mUpdateTimer.start ();
  }
  else
  {
      mRemainTime.stop ();
      mUpdateTimer.stop ();
  }
  updateTodayLabel();
}

void MiniDateWidget::setup() {
  mRemainTimeLabel = new QLabel(this);
  mRemainTimeLabel->setAlignment(Qt::AlignCenter);
  mRemainTimeLabel->setObjectName (QStringLiteral ("Time"));
  mTodayLabel = new QLabel(this);
  mTodayLabel->setObjectName (QStringLiteral ("Date"));
  mTodayLabel->setAlignment(Qt::AlignCenter);
}

void MiniDateWidget::updateRemaintimeLabel() {
  int remainTime = mRemainTime.remainingTime() / 1000;
  int hours = remainTime / 3600;
  remainTime %= 3600;
  int minutes = remainTime / 60;
  remainTime %= 60;
  int seconds = remainTime;
  mRemainTimeLabel->setText(
      QString("%1:%2:%3").arg(hours).arg(minutes).arg(seconds));
}
void MiniDateWidget::updateTodayLabel() {
    mToday = QDate::currentDate ();
    mTodayLabel->setText(mToday.toString("M月d日 dddd"));
}


void MiniDateWidget::paintEvent (QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
