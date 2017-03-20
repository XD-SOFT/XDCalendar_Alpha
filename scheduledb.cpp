#include "scheduledb.h"
#include <QDebug>
#include "config.h"

void ScheduleDB::queryByTerm()
{
    m_curRequestCode = RequestCode::QueryByTerm;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findStaScheduleByTermIdJSON?termId=" + QString::number(termId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryByTermState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

void ScheduleDB::replyArrived(const QJsonObject &json)
{
    switch(m_curRequestCode){
    case Add:{
         emit addFinish(json);
    }
        break;

    case Delete:{
        emit delFinish(json);
    }
        break;

    case Update: {
        emit updateFinish(json);
    }
        break;

    case Find: {
        emit findFinish(json);
    }
        break;

    case QueryByTerm:{
        emit queryByTermFinish(json);
    }
        break;

    default:
        break;
    }
}

ScheduleDB::ScheduleDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::scheduleReplyArrived, this, &ScheduleDB::replyArrived);

//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
////    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryByTermState(QJsonObject)));
}

void ScheduleDB::add()
{
    m_curRequestCode = RequestCode::Add;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addStaScheduleJSON?name=" + name + "&termId=" + QString::number(termId)
            + "&section=" + QString::number(section) + "&startdate=" + startdate.toString("yyyy-MM-dd")
            + "&enddate=" + enddate.toString("yyyy-MM-dd") + "&grades=" + gradesStr + Arg::tokenStr;

    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

void ScheduleDB::findByTerm()
{
    m_curRequestCode = RequestCode::Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findStaScheduleByTermIdJSON?term_Id=" + QString::number(termId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

void ScheduleDB::findById()
{
    m_curRequestCode = RequestCode::Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findStaScheduleByIdJSON?id=" + QString::number(id) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

void ScheduleDB::updateUserSche()
{
    m_curRequestCode = RequestCode::Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "updateStaScheduleJSON?id=" + QString::number(id) + "&name=" + name
            + "&section=" + QString::number(section) + "&startdate=" + startdate.toString("yyyy-M-dd")
            + "&enddate=" + enddate.toString("yyyy-M-d") + "&grades=" + gradesStr + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

void ScheduleDB::delUserSchedule()
{
    m_curRequestCode = RequestCode::Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "delStaScheduleJSON?id=" + QString::number(id) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Schedule);
//    pHandler->run();
}

//void ScheduleDB::queryByTermState(QJsonObject json)
//{
//    qDebug()<<"come in queryByTermState"<<endl;
//    emit queryByTermFinish(json);
//}

//void ScheduleDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void ScheduleDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void ScheduleDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}

//void ScheduleDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}
