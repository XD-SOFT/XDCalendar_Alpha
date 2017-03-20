#ifndef DATABASE_H
#define DATABASE_H
#include <QMap>
#include <QDate>
#include <QList>
#include <QVector>
#include <QUrl>
#include <QColor>
#include "mybutton.h"
#include "config.h"

class ModifyBaseClass//可被修改外貌属性数据的基类
{
public:
    bool BeUsed;//是否被人为设置过属性
    QColor Background_Color;
    double Background_Alpha;
    void setDefaultModify(){BeUsed=false;}
    void setBackgroundColor(QColor c){BeUsed=true;Background_Color=c;}
    void setBackgroundAlpha(double a){BeUsed=true;Background_Alpha=a;}
    void save(QDataStream &out) const
    {
        out<<BeUsed<<Background_Color<<Background_Alpha;
    }
    void load(QDataStream &in)
    {
        in>>BeUsed>>Background_Color>>Background_Alpha;
    }
};

class EventData : public ModifyBaseClass
{
public:
    static EventData DefaultEventData;
    int EventListID;//该事件是否属于的事件序列ID(-1为空)
    QString text;//事件文本
    EventData();
    EventData(QString s);
    EventData(QColor qcolor,double alpha)//默认对象的构造函数
    {
        EventListID=-1;
        Background_Color=qcolor,Background_Alpha=alpha;
        BeUsed=true;
    }
    static void setNowColor(QColor c);
    friend QDataStream& operator<<(QDataStream &out,const EventData &DB)
    {
        out<<DB.text<<DB.EventListID;
        DB.save(out);
        return out;
    }
    friend QDataStream& operator>>(QDataStream &in,EventData &DB)
    {
        in>>DB.text>>DB.EventListID;
        DB.load(in);
        return in;
    }

};

class DailyData : public ModifyBaseClass
{
public:
    static DailyData DefaultDayliData;
    QList<EventData*>text;//事件集合
    QDate date;//当天日期内省
    QList<QUrl>urls;//文件集合
    DailyData();
    DailyData(QDate a1,QColor a2,double a3)
    {
        date=a1,Background_Color=a2,Background_Alpha=a3;
        BeUsed=false;
    }
    void remove(EventData *e)
    {
        for(int i=0;i<text.size();i++)
        {
            if((text[i])==e)
            {
                if(Const::deleteEverytime)delete e;
                text.removeAt(i);
                return;
            }
        }
    }
    friend QDataStream& operator<<(QDataStream &out,const DailyData &DB);
    friend QDataStream& operator>>(QDataStream &in,DailyData &DB);
};

class DataBase
{
private:
    QMap<QDate,DailyData*> data;
public:
    static int EventListTotalID;
    ~DataBase();
    static DailyData* getDate(QDate date);
    static DataBase* getDataBase();
    void remove(int EventListID);
    void remove(EventData* e);
    friend QDataStream& operator<<(QDataStream &out,const DataBase &DB)
    {
        out<<DataBase::EventListTotalID<<DB.data.size();
        out<<DailyData::DefaultDayliData;
        out<<EventData::DefaultEventData;
        for(QMap<QDate,DailyData*>::const_iterator iter=DB.data.begin();iter!=DB.data.end();iter++)
        {
            out<<iter.key()<<(*iter.value());
        }
        return out;
    }
    friend QDataStream& operator>>(QDataStream &in,DataBase &DB)
    {
        int size=0;
        in>>DataBase::EventListTotalID>>size;
        in>>DailyData::DefaultDayliData;
        in>>EventData::DefaultEventData;
        for(int i=0;i<size;i++)
        {
            QDate a;
            DailyData *b;
            if(Arg::mergeLoad == false)
            {
                b = new DailyData;
                in>>a>>(*b);
                DB.data[a]=b;
            }
            else
            {
                in>>a;
                b=DB.getDate(a);
                in>>(*b);
            }
        }
        return in;
    }

private:
    static DataBase *_DataBase;
    //单件模式
    DataBase(){}
    DataBase(const DataBase&){}
    DataBase& operator=(const DataBase &)
    {
        return *this;
    }
};

#endif // DATABASE_H
