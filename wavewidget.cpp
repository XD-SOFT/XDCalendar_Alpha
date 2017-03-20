#include "wavewidget.h"
#include <QPainterPath>
#include <QFont>
#include <QPaintEvent>
#include <QDebug>
#include <QRegion>
#include <cmath>

WaveWidget::WaveWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFixedSize(323,323);
    setProgress(0);
    QRegion maskedRegion(geometry(), QRegion::Ellipse);
    setMask(maskedRegion);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

WaveWidget::~WaveWidget()
{

}

void WaveWidget::paintEvent(QPaintEvent *event)
{
    const QRect& rect = event->rect();
    QPainter p(this);
    QPainterPath path1, path2;

    p.setPen(Qt::NoPen);

    p.setBrush(QBrush(Qt::white));
    p.drawRect(rect);

    p.setRenderHint(QPainter::Antialiasing, true);

    // draw water
    p.setBrush(QBrush(QColor(0,0,255,77)));

    qreal y = (1-this->progress()) * rect.height();
    qreal y1 = (1-this->progress()) * rect.height();

    qreal initY = y;

    path1.moveTo(0, y);

    for (qreal x=0; x<=rect.width()*1.00; x++) {
        y = sin((x/rect.width() + mFa/rect.width()) * M_PI )* mBigNum + (1-progress()) * rect.height();
        path1.lineTo(x, y);
    }

    path1.lineTo(rect.width(), rect.height());
    path1.lineTo(0, rect.height());
    path1.lineTo(0, initY);

    p.drawPath(path1);

    // draw another layer of water
    p.setBrush(QBrush(QColor(0,0, 255, 204)));

    path2.moveTo(0, y1);
    for (qreal x=0; x<=rect.width()*1.00; x++) {
        y1 = sin((x/rect.width() + mFa/rect.width() + 1) * M_PI )* mBigNum + (1-progress()) * rect.height();
        path2.lineTo(x, y1);
    }

    path2.lineTo(rect.width(), rect.height());
    path2.lineTo(0, rect.height());
    path2.lineTo(0, initY);

    p.drawPath(path2);

    // draw text
    QFont font;
    font.setBold(true);
    font.setPointSize(rect.height()*0.2);
    p.setFont(font);
    QPen pen(Qt::black);
    p.setPen(Qt::black);
    int pro = mProgress * 100;
    p.drawText(rect,Qt::AlignCenter,  QString("%1%").arg(pro));

    // draw outline
    pen.setColor(QColor(197,197,197));
    pen.setWidth(10);
    p.setPen(pen);
    p.setBrush(Qt::NoBrush);
    p.drawEllipse(rect);

}

qreal WaveWidget::speed() const
{
    return mSpeed;
}

void WaveWidget::setSpeed(const qreal &speed)
{
    mSpeed = speed;
}

double WaveWidget::progress() const
{
    return mProgress;
}

void WaveWidget::setProgress(double present)
{
    mProgress = present;
    if (mTimer != 0) {
        mTimer->stop();
        mTimer->deleteLater();
    }
    mTimer = new QTimer(this);
    mTimer->setInterval(1);

    if (present <= 0.5) {
        mBigNum = geometry().height()*0.1*present*2;
    } else {
        mBigNum = geometry().height()*0.1*(1-present)*2;
    }

    connect(mTimer, &QTimer::timeout, this, &WaveWidget::shot);

    mTimer->start();
}

void WaveWidget::shot()
{
//    qDebug() << "shot" << mFa;
    mFa = mFa + 100 * mSpeed;
    if (mFa >= geometry().width()*2) {
        mFa = 0;
    }
    update();
}

void WaveWidget::mousePressEvent(QMouseEvent *event)
{
    _speed = mSpeed;
    if (event->button() == Qt::LeftButton) {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void WaveWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (mSpeed < 1 && mSpeed < _speed*2) {
            mSpeed += 0.005;
        }
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void WaveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    mSpeed = _speed;
    event->accept();
}
