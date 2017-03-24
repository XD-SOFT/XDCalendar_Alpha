#include "filenumwidget.h"
#include <QDebug>
#include <QPainter>
void FileNumWidget::paintEvent(QPaintEvent*)
{
 //qDebug() << "filNumWidget update: " << fileNum << endl;
    QPainter paint(this);

    if(m_bDisplayFileNumEnabled) {
        paint.setRenderHint (QPainter::Antialiasing);
        paint.setRenderHint (QPainter::HighQualityAntialiasing);
        paint.setBrush(QBrush(Qt::red,Qt::SolidPattern));//设置画刷形式
        paint.setPen (Qt::NoPen);
        paint.drawEllipse (0,0,16,16);
        paint.setPen(QPen(Qt::white));
        QString fs = QString::number(fileNum, 10);

        if (fileNum < 10 && fileNum > 0)
        {
            paint.drawText(QRect(1, 0, 15, 15), Qt::AlignCenter, fs);
        }
        else
        {
            if (fileNum >= 10)
            {
                paint.drawText (QRect(1, -3, 15, 15), Qt::AlignCenter, "...");
            }
            //        else
            //        {
            //            //paint->setBrush(QBrush(Qt::white,Qt::SolidPattern));
            //            //paint->drawEllipse (0,0,16,16);
            //            //do nothing;
            //            this->setVisible(false);
            //        }
        }
    }
    else {
        QColor color(255, 255, 255, 0);
        paint.fillRect(rect(), QBrush(color));
    }

    paint.end();
}

FileNumWidget::FileNumWidget(QWidget*parent, int num):QWidget(parent), fileNum(num)
{
   // fileNum = 0;
    //setGeometry(100,100,200,200);
    m_bDisplayFileNumEnabled = true;
}

void FileNumWidget::setDisplayFileNumEnabled(bool bEnabled)
{
    m_bDisplayFileNumEnabled = bEnabled;

    update();
}
