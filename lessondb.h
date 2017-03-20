#ifndef LESSONDB_H
#define LESSONDB_H

#include <QObject>
#include <QJsonObject>
#include "handler.h"

class LessonDB : public QObject //教师课程表主表，与用户ID和作息表ID关联，可以增、删、改、查
{
    Q_OBJECT
public:
    explicit LessonDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setUserId(int userId) { this->userId = userId; }
    int getUserId() { return userId; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setScheduleId(int scheduleId) { this->scheduleId = scheduleId; }

    int getScheduleID() const;

//    void setBaseUrl(const QString &url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void add(); //插入一条记录
    void find(); //查找一条记录
    void del(); //删除一条记录（state = 0）

private slots:
    void replyArrived(const QJsonObject &jsonObj);

private:
    enum RequestCode{Add, Find, Delete, Create};
    RequestCode m_curRequestCode;

    int id;
    int userId;
    int state; //0-不可用，1-可用
    int scheduleId; //关联作息时间表

//    QString baseUrl;

signals:
    void addFinish(QJsonObject); //发射信号更新前端界面
    void findFinish(const QJsonObject&);
    void delFinish(QJsonObject);

//public slots:
//    void addState(QJsonObject);
//    void findState(QJsonObject);
//    void delState(QJsonObject);

};

#endif // LESSONDB_H
