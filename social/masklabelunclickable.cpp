#include "maskunclickablelabel.h"
#include <QPainter>

void MaskUnclickableLabel::paintEvent(QPaintEvent *e)
{
    if(pixmap()){
        QPainter painter(this);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
        QPainterPath path;
        int largerRound = qMin(width(), height());
        int round = static_cast<int>(largerRound * mRatio);
        int bias = (largerRound - round) >> 1;
        path.addEllipse(bias, bias, round, round);

        //paint shadow
        {
            painter.save ();
            QPen pen = painter.pen ();
            pen.setColor (QColor (24, 24, 24, 48));
            pen.setWidth (bias >> 1);
            painter.setPen (pen);
            painter.drawEllipse (bias >> 1, bias >> 1, round + bias, round + bias);
            painter.restore ();
        }

        painter.setClipPath(path);
        painter.drawPixmap(0, 0, width(), height(), *pixmap());
    }else{
        QLabel::paintEvent(e);
    }
}
