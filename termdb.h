#ifndef TERMDB_H
#define TERMDB_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QJsonObject>
#include "handler.h"
#include <QDebug>

class TermDB : public QObject //学期表，提供查询，修改，增加的功能
{
    Q_OBJECT
public:
    TermDB(QObject *parent = Q_NULLPTR);

    void setId(int id){ this->id = id; }
    int getId() { return id; }
    void setTitle(QString title) { this->title = title; }
    QString getTitle() { return title; }
    void setTerm(QString term) { this->term = term; }
    QString getTerm() { return term; }
    void setYearStrat(QString yearstart) { this->yearstart = yearstart; }
    QString getYearStrat() { return yearstart; }
    void setOwnership(int ownership) { this->ownership = ownership; }
    int getOwnership() { return ownership; }
    void setStartTime(QDate starttime) { this->starttime = starttime; }
    QDate getStartTime() { return starttime; }
    void setEndTime(QDate endtime) { this->endtime = endtime; }
    QDate getEndTime() { return endtime; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void setState(int state) { this->state = state; }
    int getState() { return state; }

    void add(); //新增一个用户创建的学期
    void delUserTerm(); //删除一个用户创建的学期（state = 0）
    void updateUserTerm(); //更新一个用户创建的学期
    void queryAll(); //获取后台可用的所有学期
    void querySchool(); //获取由学校创建的可用的所有学期
    void queryUser(); //获取由用户创建的可用的所有学期

    void queryTermByDate(const QDate &date);

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
     * \var GetByDate，通过日期获取学期.
     */
    enum RequestCode{Add, Delete, Update, GetAll, AllBySchoolCreate, AllByUserCreate, GetByDate};
    /*!
     * \brief request, 向服务器发起请求.
     * \param sUrl, 请求连接字符串.
     */
    void request(const QString &sUrl);

private:
    int id;
    QString title;
    QString term;
    int state; //为保证数据安全性，本表不允许删除数据，使用状态来控制可以用的学期，0为不可用，1为可用
    QString yearstart; //所属学年
    int ownership; //0-school 1-user
    QDate starttime; //学期开始时间
    QDate endtime; //学期结束时间

//    QString baseUrl;

    /*!
     * \brief m_curRequestCode, 当前请求代码.
     */
    RequestCode m_curRequestCode;


signals:
    void addTermFinished(const QJsonObject &jsonObj); //发射信号更新前端界面
    void deleteTermFinished(const QJsonObject &jsonObj);
    void updateTermFinished(const QJsonObject &jsonObj);
    void queryAllTermFinished(const QJsonObject &jsonObj);
    void querySchoolTermFinished(const QJsonObject &jsonObj);
    void queryUserTermFinished(const QJsonObject &jsonObj);

    void queryTermByDateFinished(const QJsonObject &jsonObj);

private slots:
    void handleReply(const QJsonObject &jsonObj);

//public slots:
//    void addState(QJsonObject); //handler获取数据后返回给该槽函数
//    void delState(QJsonObject);
//    void updateState(QJsonObject);
//    void queryAllState(QJsonObject);
//    void querySchoolState(QJsonObject);
//    void queryUserState(QJsonObject);

};

#endif // TERMDB_H
