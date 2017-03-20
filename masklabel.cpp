#include "masklabel.h"

#include <QPainter>

MaskLabel::MaskLabel(QWidget *parent) : QLabel(parent)
{
    m_bEnabledShadow = true;
}

void MaskLabel::setEnableShaodw(bool bEnabled)
{
    m_bEnabledShadow = bEnabled;
}

void MaskLabel::paintEvent(QPaintEvent *e)
{
    if(pixmap()){
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

        int largerRound = qMin(width(), height());

        if(m_bEnabledShadow) {
            int round = static_cast<int>(largerRound * 0.95);
            int bias = (largerRound - round) >> 1;
            painter.save();
            QPen pen;
            pen.setWidth(2);
            pen.setColor(QColor (24, 24, 24, 48));
            painter.setPen(pen);
            painter.drawEllipse(bias, bias, round, round);
            painter.restore();
        }

        QPainterPath path;
        int round = static_cast<int>(largerRound * 0.9);
        int bias = (largerRound - round) >> 1;
        path.addEllipse(bias, bias, round, round);

        //paint shadow
//        {
//            painter.save ();
//            QPen pen = painter.pen ();
//            pen.setColor (QColor (24, 24, 24, 48));
//            pen.setWidth (bias >> 1);
//            painter.setPen (pen);
//            painter.drawEllipse ((bias >> 1) + 4, (bias >> 1) + 4, round + bias - 8, round + bias - 8);
//            painter.restore ();
//        }

        painter.setClipPath(path);
        painter.drawPixmap(0, 0, width(), height(), *pixmap());
    }else{
        QLabel::paintEvent(e);
    }
}

void MaskLabel::mousePressEvent (QMouseEvent *ev)
{
    emit this->clicked ();

    QLabel::mousePressEvent(ev);
}
