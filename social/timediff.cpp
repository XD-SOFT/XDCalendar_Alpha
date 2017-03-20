#include "timediff.h"
#include <QDateTime>
#include <QObject>
#include <QDebug>

QString TimeDiff::Between(const QDateTime& from, const QDateTime& to){
    qint64 day = from.daysTo(to);

    int yFrom = from.date().year(), yTo = to.date().year();
    if(day > 240 && yFrom != yTo)//年显示
        return QString::number(yTo - yFrom) + QObject::tr("年");

    int mFrom = from.date().month(), mTo = to.date().month();
    if(day > 20 && mFrom != mTo)//月份显示
        return QString::number(mTo < mFrom ? mTo + 12 - mFrom : mTo - mFrom) + QObject::tr("个月");

    qint64 sec = from.secsTo(to);
    if(sec > 57600 && day > 0)
        return QString::number(day) + QObject::tr("天");

    int hFrom = from.time().hour(), hTo = to.time().hour();
    if(sec > 2400 && hFrom != hTo)
        return QString::number(hTo < hFrom ? hTo + 24 - hFrom : hTo - hFrom) + QObject::tr("小时");

    int miFrom = from.time().minute(), miTo = to.time().minute();
    if(sec > 40 && miFrom != miTo)
        return QString::number(miTo < miFrom ? miTo + 60 - miFrom : miTo - miFrom) + QObject::tr("分钟");

    if(sec >= 0)
        return QString::number(sec) + QObject::tr("秒");

    return QString('-') + TimeDiff::Between(to, from);
}

void TimeDiff::Test(){
    qDebug() << "social/timediff: Test" << endl;
    qDebug() << TimeDiff::Between(QDateTime(QDate(2016,1,4),QTime(15,10,1)), QDateTime::currentDateTime())<<endl;
    qDebug() << TimeDiff::Between(QDateTime(QDate(2016,11,4),QTime(15,50,1)), QDateTime::currentDateTimeUtc())<<endl;
}
