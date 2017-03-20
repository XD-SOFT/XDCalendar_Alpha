#include "colorbutton.h"
#include <QPainter>
#include <algorithm>

ColorButton::ColorButton(QWidget* parent):QPushButton(parent)
{
    myColor = QColor(100,100,100);
}

void ColorButton::setColor(QColor c)
{
    myColor = c;
    update();
}

void ColorButton::paintEvent(QPaintEvent *)
{
    QPainter paint(this);
    paint.setRenderHint (QPainter::Antialiasing);
    paint.setRenderHint (QPainter::HighQualityAntialiasing);
    paint.setBrush(QBrush(myColor,Qt::SolidPattern));//设置画刷形式
    paint.setPen (Qt::NoPen);
    int radius = std::min(this->width () << 1, this->height () << 1);
    int rectHeight = std::max(this->height () -radius, 0);
    paint.drawRect(0,radius >> 1,this->width (),rectHeight);
    //paint.drawEllipse(0,0,15,15);
    paint.drawPie(0,0,radius,radius,1440,1440);
    //paint.drawEllipse(0,85,15,15);
    paint.drawPie(0,this->height () - radius,radius,radius,2880,1440);
    paint.end();
}
