#include "calendarwidget.h"
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

class CalendarWidgetPrivate
{
    QRect getSquare(const QRect &rect)
    {
        int width;

        QRect centrlRect;
        if(rect.width() > rect.height())
        {
            width = rect.height();
            centrlRect.setX(rect.x() + width/4 -1);
            centrlRect.setY(rect.y());
        }else{
            width = rect.width();
            centrlRect.setX(rect.x());
            centrlRect.setY(rect.y() + width/4 -1);
        }

        centrlRect.setWidth(width);
        centrlRect.setHeight(width);

        return centrlRect;
    }
    QVector<QDate> clickWeek; //保存点击日期的一周（7天）.
    QDate curDate;          //保存当前日期
    QDate clickDate;         //保存点击日期
    QDate activeDate;

    friend class CalendarWidget;
};

static CalendarWidgetPrivate calendarPrivate;

CalendarWidget::CalendarWidget(QWidget *parent)
    :QCalendarWidget(parent)
{
    connect(this, SIGNAL(clicked(const QDate &)), this, SLOT(dateClick(const QDate &)));

    connect(this, SIGNAL(currentPageChanged(int,int)), this, SLOT(currentPageChanged(int,int)));


    QPushButton *today = new QPushButton("回到今日");
    today->setObjectName("today");
    today->setStyleSheet("QPushButton#today{background:transparent;}"
                         "QPushButton#today:hover{background:transparent;color:rgb(42, 120, 219);}");
    connect(today, SIGNAL(clicked(bool)), this, SLOT(comeToday()));

    QFrame *line = new QFrame;
    line->setStyleSheet("background:rgb(233, 233, 233)");
    line->setFixedHeight(2);
    layout()->setSpacing(0);
    layout()->addWidget(line);
    layout()->setSpacing(2);
    layout()->addWidget(today);
    layout()->setSpacing(0);

    this->dateClick(QDate::currentDate());

}

void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{

    if(calendarPrivate.clickWeek.contains(date))
    {
        QRect square = calendarPrivate.getSquare(rect);
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(Qt::black);
        QRectF r = painter->boundingRect(rect, Qt::AlignCenter,  QString::number(date.day()));

        if(calendarPrivate.curDate == date)
        {
            painter->setPen(Qt::white);
            painter->setBrush(QBrush(QColor(42, 120, 219)));
            painter->save();
            painter->setPen(Qt::transparent);
            painter->drawEllipse(square);
            painter->restore();
            painter->drawText(r, QString::number(date.day()));

        }else {

            if(calendarPrivate.clickDate == date)
            {
                painter->setBrush(QBrush(Qt::gray));
                painter->save();
                painter->setPen(Qt::transparent);
                painter->drawEllipse(square);
                painter->restore();
                painter->drawText(r, QString::number(date.day()));
            }
            else {
                painter->setBrush(QColor(229, 229, 229));
                painter->save();
                painter->setPen(Qt::transparent);
                painter->drawEllipse(square);
                painter->restore();
                painter->drawText(r, QString::number(date.day()));
            }

        }

        painter->restore();
    }
    else {
        QCalendarWidget::paintCell(painter, rect, date);

    }
}


void CalendarWidget::dateClick(const QDate &date)
{
    calendarPrivate.clickWeek.clear();
    int weekNumber = date.dayOfWeek();

    this->setSelectedDate(date);

    if(date.month() == QDate::currentDate().month())
    {
        calendarPrivate.clickWeek.push_back(QDate::currentDate());
    }

    //获取点击日期一周
    if(weekNumber == 1)
    {
        for(int i = 0; i< 7; i++)
        {
            calendarPrivate.clickWeek.push_back(date.addDays(i));
        }
    }else if(weekNumber == 7)
    {
        for(int i = 0; i< 7; i++)
        {
            calendarPrivate.clickWeek.push_back(date.addDays(-i));
        }
    }else {
        for(int i = 0; i < weekNumber; i++)
        {
            calendarPrivate.clickWeek.push_back(date.addDays(-i));
        }

        for(int i = weekNumber; i <= 7; i++)
        {
            calendarPrivate.clickWeek.push_back(date.addDays(i -weekNumber));
        }
    }

    calendarPrivate.curDate = QDate::currentDate();
    calendarPrivate.clickDate = date;
    updateCells();
}

void CalendarWidget::currentPageChanged(int year, int month)
{
    if(year == QDate::currentDate().year() &&
            month == QDate::currentDate().month())
    {
        //dateClick(QDate::currentDate());
        emit clicked(QDate::currentDate());
    }
    else {

        emit clicked(selectedDate().addMonths(month- selectedDate().month()).
                                     addYears(year- selectedDate().year()));
//        dateClick(selectedDate().addMonths(month- selectedDate().month()).
//                  addYears(year- selectedDate().year()));
    }

}

void CalendarWidget::comeToday()
{
    this->showToday();
    //dateClick(QDate::currentDate());
    emit clicked(QDate::currentDate());
}
