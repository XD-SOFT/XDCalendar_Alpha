#ifndef LESSONDETAILDB_H
#define LESSONDETAILDB_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include "handler.h"
#include <QQUeue>

///Mark 2017.02.13，这些数据管理类都应该设置为只能一个实例,这里名字也不适合.
/// 这个类设计有问题，即能给所有的Lesson访问数据库，又等是课程细节，以后分成两个类，一个专门管理课程对接数据库，一个课程细节.
class LessonDetailDB : public QObject
{
    Q_OBJECT
public:
    explicit LessonDetailDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setClassId(int classid) { this->classid = classid; }
    int getClassId() { return classid; }
    void setRoomId(int roomid) { this->roomid = roomid; }
    int getRoomId() { return roomid; }
    void setClassName(QString className) { this->className = className; }
    QString getClassName() { return className; }

    /*!
     * \brief setLessonId, 设置课程主表ID.
     * \param lessonId
     */
    void setLessonId(int lessonId) { this->lessonId = lessonId; }

    int getLessonId() { return lessonId; }

    void setRoom(QString room) { this->room = room; }
    QString getRoom() { return room; }
    void setWeekId(int weekId) { this->weekId = weekId; }
    int getWeekId() { return weekId; }
    void setSectionId(int sectionId) { this->sectionId = sectionId; }
    int getSectionId() { return sectionId; }
    void setStartDate(QDate startdate) { this->startDate = startdate; }
    QDate getStartDate() { return startDate; }
    void setEndDate(QDate enddate) { this->endDate = enddate; }
    QDate getEndDate() { return endDate; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setSubjectId(int subjectId) { this->subjectId = subjectId; }
    QString getSubject() { return subject; }
    void setSubject(QString subject) { this->subject = subject; }
    int getSubjectId() { return subjectId; }
    void setRepeat(int repeat);
    int getRepeat() { return repeat; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void add(); //新增一个课程格子
    void del(); //删除一个课程格子
    void update(); //修改一个课程格子
    void findAll(); //获取某一主课程表ID的所有课程格子

private slots:
    /*!
     * \brief replyArrived
     * \param jsonObj
     */
    void replyArrived(const QJsonObject &jsonObj);

private:
    /*!
     * \brief The RequestCode enum
     */
    enum RequestCode {Add, Delete, Update, FindAll};
    RequestCode m_curRequestCode;

    int id; //lessonDetail ID
    int roomid;
    int classid;
    QString subject;
    QString className;
    int lessonId; //课程主表id
    QString room;
    int weekId;
    int sectionId;
    QDate startDate;
    QDate endDate;
    int state;
    int subjectId;
    int repeat;

//    QString baseUrl;

signals:
    void addFinish(const QJsonObject&); //发射信号更新前端界面
    void findAllFinish(const QJsonObject&);
    void delFinish(int nDetailID, const QJsonObject&);
    void updateFinish(const QJsonObject&);

//public slots:
//    void addState(QJsonObject);
//    void findAllState(QJsonObject);
//    void delState(QJsonObject);
//    void updateState(QJsonObject);

private:
    QQueue<int> requestIDQUeue;

};

#endif // LESSONDETAILDB_H
