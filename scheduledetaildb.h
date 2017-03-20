#ifndef SCHEDULEDETAILDB_H
#define SCHEDULEDETAILDB_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include "handler.h"

///Mark 2017.02.13，这个类是不是应该只有一个实例？？？.
class ScheduleDetailDB : public QObject //作息时间明细表，可以进行增、删、改、查
{
    Q_OBJECT
public:
    explicit ScheduleDetailDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setScheduleId(int scheduleId) { this->scheduleId = scheduleId; }
    int getScheduleId() { return scheduleId; }
    void setName(QString name) { this->name = name; }
    QString getName() { return name; }
    void setStartTime(QString starttime) { this->starttime = starttime; }
    QString getStartTime() { return starttime; }
    void setEndTime(QString endtime) { this->endtime = endtime; }
    QString getEndTime() { return endtime; }
    void setType(int type) { this->type = type; }
    int getType() { return type; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setSectionId(int sectionId) { this->sectionId = sectionId; }
    int getSectionId() { return sectionId; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void add(); //为某一个作息时间记录增加一条作息明细记录（用户）
    void del(); //删除一条用户作息明细记录（state = 0）
    void update(); //更新一条作息明细记录（用户）
    void find(); //根据作息时间主表ID获取所有记录

private slots:
    /*!
     * \brief replyArrived
     * \param json
     */
    void replyArrived(const QJsonObject &json);

private:
    QString name;
    QString starttime;
    QString endtime;

    /*!
     * \brief The RequestCode enum
     */
    enum RequestCode{Add, Delete, Update, Find};
    RequestCode m_curRequestCode;

    int id;
    int scheduleId;
    int type; //时间段 0 早上 1 上午 2 下午 3 晚上
    int state; //0-不可用 1-可用
    int sectionId; //课程坐标值

//    QString baseUrl;


signals:
    void addFinish(QJsonObject); //发射信号更新前端界面
    void delFinish(QJsonObject);
    void updateFinish(QJsonObject);
    void findFinish(const QJsonObject &);

//public slots:
//    void addState(QJsonObject);
//    void delState(QJsonObject);
//    void updateState(QJsonObject);
//    void findState(QJsonObject);

};

#endif // SCHEDULEDETAILDB_H
