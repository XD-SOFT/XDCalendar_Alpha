#include "schedetdb.h"
#include "handler.h"
#include "config.h"
#include <QDebug>

ScheDetDB::ScheDetDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::scheDetDBReplyArrived, this, &ScheDetDB::replyArrived);

//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
}

void ScheDetDB::add()
{
    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addStaScheduleDetail?state=1&fk_scheduleid=" + scheId + "&name=" + name
            + "&sectionid=" + QString::number(secId) + "&starttime=" + starttime
            + "&endtime=" + endtime + "&type=" + type + Arg::tokenStr;

    qDebug()<<"url: "<<url<< endl;
    return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheDetDB);
//    pHandler->run();
}

void ScheDetDB::findBySche()
{
    m_curRequestCode = Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findStaScheduleByTermIdJSON?termId=" + QString::number(scheId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
    return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheDetDB);
//    pHandler->run();
}

void ScheDetDB::updateUserScheDet()
{
    m_curRequestCode = Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "updateStaScheduleJSON?id=" + QString::number(scheId) + "&name=" + name
            + "&section=" + QString::number(secId) + "&starttime=" + starttime
            + "&endtime=" + endtime + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
    return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheDetDB);
//    pHandler->run();
}

void ScheDetDB::replyArrived(const QJsonObject &jsonObj)
{
    switch (m_curRequestCode) {
    case Add: {
        emit addFinish(jsonObj);
    }
        break;

    case Delete:{
        emit delFinish(jsonObj);
    }
        break;

    case Find:{
        emit findFinish(jsonObj);
    }
        break;

    case Update:{
        emit updateFinish(jsonObj);
    }
        break;

    default:
        break;
    }
}

void ScheDetDB::delUserSchedule()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "delStaScheduleJSON?id=" + QString::number(scheId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheDetDB);
//    pHandler->run();
}

//void ScheDetDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void ScheDetDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void ScheDetDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}

//void ScheDetDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}
