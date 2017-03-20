#ifndef LESSONREMARKDB_H
#define LESSONREMARKDB_H

#include <QObject>
#include <QDate>
#include <QJsonObject>
#include "handler.h"

class Lesson;

class LessonRemarkDB : public QObject
{
    Q_OBJECT
public:
    explicit LessonRemarkDB(QObject *parent = Q_NULLPTR);

    void setId(int id);

    int getId();

    void setLessonDetailId(int lessonDetailId);

    int getLessonDetailId();

    void setRemark(const QString &remark);

    QString getRemark();

    void setTeachingDate(const QDate &teachingDate);

    QDate getTeachingDate();

    void setState(int state);

    int getState();

//    void setBaseUrl(const QString &url);

//    QString getBaseUrl();

    void add(); //为某一课程格子添加备注
    void del(); //删除某一课程格子的备注
    void update(); //更新某一课程格子的备注
    void find(); //查找某一课程格子的备注

private slots:
    void replyArrived(const QJsonObject &jsonObj);

private:
    enum RequestCode {Add, Find, Delete, Update};
    RequestCode m_curRequestCode;

    int id;
    int lessonDetailId;
    QString remark;
    QDate teachingDate; //上课日期
    int state;
//    QString baseUrl;

signals:
    void addFinish(QJsonObject); //发射信号更新前端界面
    void findFinish(int nDetailID, const QString &sRemark);
    void delFinish(QJsonObject);
    void updateFinish(QJsonObject);

//public slots:
//    void addState(QJsonObject);
//    void findState(QJsonObject);
//    void delState(QJsonObject);
//    void updateState(QJsonObject);
};

#endif // LESSONREMARKDB_H
