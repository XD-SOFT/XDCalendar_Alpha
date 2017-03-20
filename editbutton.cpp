#include "editbutton.h"
#include <QPainter>
#include <QDebug>

EditButton::EditButton(QWidget *parent, bool del) : QPushButton(parent), isDelete(del)
{

}

void EditButton::mousePressEvent(QMouseEvent *e)
{
    qDebug()<<"press edit button"<<endl;
    emit clicked();
}

void EditButton::paintEvent(QPaintEvent *e)
{
    QPainter paint(this);
    paint.setRenderHint (QPainter::Antialiasing);
    paint.setRenderHint (QPainter::HighQualityAntialiasing);

    if(!isDelete)
    {
        paint.setBrush(QBrush(Qt::blue,Qt::SolidPattern));//设置画刷形式
        paint.setPen (Qt::NoPen);
        //paint.drawRect(15,10,15,5);
        //paint.drawRect(20,5,5,15);

        paint.drawRect(0,9,15,5);
        paint.drawRect(5,5,5,15);
    }
    else
    {
        paint.setBrush(QBrush(Qt::blue,Qt::SolidPattern));//设置画刷形式
        paint.setPen (Qt::NoPen);
        paint.drawRect(15,10,15,5);
        //paint.drawRect(20,5,5,15);
    }

    //paint.drawEllipse(0,0,15,15);
    //paint.drawPie(0,0,30,30,1440,1440);
    //paint.drawEllipse(0,85,15,15);
    //paint.drawPie(0,70,30,30,2880,1440);
    paint.end();
}

