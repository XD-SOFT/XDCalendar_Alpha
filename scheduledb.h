#ifndef SCHEDULEDB_H
#define SCHEDULEDB_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QDate>
#include <QJsonObject>
#include "handler.h"

class ScheduleDB : public QObject //作息时间表主表, 通过termId区分是用户数据还是学校数据，可以进行增、删、改、查
{
    Q_OBJECT
public:
    ScheduleDB(QObject *parent = Q_NULLPTR);
    void setTitle(QString title){ this->name = title; }
    QString getTitle() { return name; }

    void setTerm(QString term) { this->term = term; }
    QString getTerm() { return term; }

    void setYear(QString year) { this->year = year; }
    QString getYear() { return year; }

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setTermId(int termId) { this->termId = termId; }
    int getTermId() { return termId; }
    void setSection(int section) { this->section = section; }
    int getSection() { return section; }
    void setStartDate(QDate startdate) { this->startdate = startdate; }
    QDate getStartDate() { return startdate; }
    void setEndDate(QDate enddate) { this->enddate = enddate; }
    QDate getEndDate() { return enddate; }
    void setCreateDate(QDate createtime) { this->createtime = createtime; }
    QDate getCreateDate() { return createtime; }
    void setUpdateDate(QDate updatetime) { this->updatetime = updatetime; }
    QDate getUpdateDate() { return updatetime; }
    QVector<int> getGrades() { return grades; }
    void setGrades(QVector<int> grades) { this->grades = grades; }

    void setGradesStr(QString gradesStr){ this->gradesStr = gradesStr; }
    QString getGradesStr() { return this->gradesStr; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void add(); //新增一条记录
    void findByTerm(); //根据学期Id查找一条记录
    void findById(); //根据作息时间Id查找一条记录
    void updateUserSche(); //更新一个用户的记录
    void delUserSchedule(); //删除一条用户的作息时间记录（state = 0）

    void queryByTerm(); //获取后台可用的所有作息时间

    /*
    void querySchool(); //获取由学校创建的可用的所有作息时间
    void queryUser(); //获取由用户创建的可用的所有作息时间
    */

private:
    /*!
     * \enum RequestCode.
     * \brief 学期类功能请求代码.
     * \var Add, 添加学期.
     * \var Delete, 删除学期.
     * \var Update, 更新学期.
     * \var GetAll,获取所有学期,包括学校用户创建.
     * \var AllBySchoolCreate，学校创建学期 ？.
     * \var AllByUserCreate,用户创建所有学期？
     */
    enum RequestCode{Add, Find, Update, Delete, QueryByTerm};

    /*!
     * \brief request, 向服务器发起请求.
     * \param sUrl, 请求连接字符串.
     */
    void request(const QString &sUrl);

private:
    int id;
    int termId;
    QString year; //学年
    QString name;
    QString term;
    int section; //课节数
    QDate createtime; //创建时间
    QDate updatetime; //最后更新时间
    QDate startdate;
    QDate enddate;
    QVector<int> grades; //适用年级
    QString gradesStr;

//    QString baseUrl;

    ///< \brief 当前请求码.
    RequestCode m_curRequestCode;

signals:
    void addFinish(QJsonObject); //发射信号更新前端界面
    void findFinish(const QJsonObject&);
    void updateFinish(QJsonObject);
    void delFinish(QJsonObject);
    void queryByTermFinish(QJsonObject);

    /*
    void querySchoolFinish(QJsonObject);
    void queryUserFinish(QJsonObject);
    */


public slots:
    /*!
     * \brief replyArrived,请求应答处理函数.
     * \param json
     */
    void replyArrived(const QJsonObject &json);

//    void addState(QJsonObject); //handler获取数据后返回给该槽函数
//    void findState(QJsonObject);
//    void updateState(QJsonObject);
//    void delState(QJsonObject);
//    void queryByTermState(QJsonObject);

    /*
    void querySchoolState(QJsonObject);
    void queryUserState(QJsonObject);
    */
};

#endif // SCHEDULEDB_H
