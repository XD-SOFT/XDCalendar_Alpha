#include "database.h"
#include <QDate>
#include <QColor>

DataBase* DataBase::_DataBase = nullptr;
DailyData DailyData::DefaultDayliData(QDate(2016,1,1),QColor(0,50,29),0.6);
EventData EventData::DefaultEventData(QColor(0,255,0),1.0);
int DataBase::EventListTotalID = 0;

DailyData::DailyData()
{
    (*this)=DefaultDayliData;
}

EventData::EventData()
{
    (*this)=DefaultEventData;
}

EventData::EventData(QString s)
{
    (*this)=DefaultEventData;
    text=s;
}


void EventData::setNowColor(QColor c)
{
    DefaultEventData.Background_Color=c;
}

DataBase::~DataBase()
{
}
DailyData* DataBase::getDate(QDate date)
{
    QMap<QDate,DailyData*>::iterator iter;
    iter = getDataBase()->data.find(date);
    if(iter == getDataBase()->data.end())
        getDataBase()->data[date] = new DailyData;
    return getDataBase()->data[date];
}
DataBase* DataBase::getDataBase()
{
    if(_DataBase == nullptr)
    {
        _DataBase = new DataBase();
    }
    return _DataBase;
}
void DataBase::remove(int EventListID)
{
    for(QMap<QDate,DailyData*>::iterator iter = data.begin();iter!=data.end();iter++)
    {
        for(int i=0;i<iter.value()->text.size();i++)
        {
            EventData *e=iter.value()->text[i];
            if(e->EventListID == EventListID)
            {
                if(Const::deleteEverytime)delete e;
                iter.value()->text.removeAt(i);
                i--;
            }
        }
    }
}
void DataBase::remove(EventData* e)
{
    for(QMap<QDate,DailyData*>::iterator iter = data.begin();iter!=data.end();iter++)
    {
        for(int i=0;i<iter.value()->text.size();i++)
        {
            if(e == iter.value()->text[i])
            {
                if(Const::deleteEverytime)delete e;
                iter.value()->text.removeAt(i);
                return;
            }
        }
    }
    qDebug()<<"remove wrong!!!";
}



QDataStream& operator<<(QDataStream &out,const DailyData &DB)
{
    out<<DB.date;
    out<<DB.text.size();
    DB.save(out);
    for(QList<EventData*>::const_iterator iter = DB.text.begin();iter!=DB.text.end();iter++)
    {
        out<<(**iter);
    }
    out<<DB.urls.size();
    for(QList<QUrl>::const_iterator iter = DB.urls.begin();iter!=DB.urls.end();iter++)
    {
        out<<(*iter);
    }
    return out;
}
QDataStream& operator>>(QDataStream &in,DailyData &DB)
{
    int size;
    in>>DB.date;
    in>>size;
    DB.load(in);
    if(Arg::mergeLoad==false)
    {
        DB.text.clear();
        DB.urls.clear();
    }
    for(int i=0;i<size;i++)
    {
        EventData *E=new EventData;
        in>>(*E);
        DB.text.append(E);
    }
    in>>size;
    for(int i=0;i<size;i++)
    {
        QUrl U;
        in>>U;
        DB.urls.append(U);
    }
    return in;
}
