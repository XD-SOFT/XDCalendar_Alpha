#include "scheduledetaildb.h"
#include "config.h"
#include <QDebug>

ScheduleDetailDB::ScheduleDetailDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::scheduleDetailDBReplyArrived, this, &ScheduleDetailDB::replyArrived);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void ScheduleDetailDB::add()
{
    m_curRequestCode = Add;
    //addStaScheduleDetail
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addStaScheduleDetailJSON?int_fk_scheduleid=" + QString::number(scheduleId) + "&name=" + name
            + "&sectionid=" + QString::number(sectionId) + "&starttime=" + starttime + "&endtime=" + endtime
            + "&type=" + QString::number(type) + "&state=1" + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheduleDetailDB);
//    pHandler->run();
}

void ScheduleDetailDB::del()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //delStaScheduleDetail
    QString url = sBaseUrl + "delStaScheduleDetailByScheduleIdJSON?int_fk_scheduleid=" + QString::number(scheduleId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;

//    //return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheduleDetailDB);
//    pHandler->run();
}

void ScheduleDetailDB::update()
{
    m_curRequestCode = Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //updateStaScheduleDetail
    QString url = sBaseUrl + "updateStaScheduleDetailJSON?id=" + QString::number(id) + "&int_fk_scheduleid="
            + QString::number(scheduleId) + "&name=" + name
            + "&sectionid=" + QString::number(sectionId) + "&starttime=" + starttime + "&endtime=" + endtime
            + "&type=" + QString::number(type) + "&state=1" +  Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheduleDetailDB);
//    pHandler->run();
}

void ScheduleDetailDB::find()
{
    m_curRequestCode = Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findStaScheduleDetailByScheduleIdJSON?int_fk_scheduleid=" + QString::number(scheduleId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ScheduleDetailDB);
//    pHandler->run();
}

void ScheduleDetailDB::replyArrived(const QJsonObject &json)
{
    switch(m_curRequestCode) {
    case Add:{
         emit addFinish(json);
    }
        break;

    case Delete: {
        emit delFinish(json);
    }
        break;

    case Update:{
        emit updateFinish(json);
    }
        break;

    case Find: {
        emit findFinish(json);
    }
        break;

    default:
        break;
    }
}

//void ScheduleDetailDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void ScheduleDetailDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}

//void ScheduleDetailDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}

//void ScheduleDetailDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}
