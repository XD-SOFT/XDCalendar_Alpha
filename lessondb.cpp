#include "lessondb.h"
#include "config.h"
#include <QDebug>

LessonDB::LessonDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pRequest = Handler::getInstance();
    connect(pRequest, &Handler::lessonDBReplyArrived, this, &LessonDB::replyArrived);
//    connect(pRequest, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pRequest, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
    //    connect(pRequest, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
}

int LessonDB::getScheduleID() const
{
    return scheduleId;
}

void LessonDB::add()
{
#if 0
    m_curRequestCode = Add;
//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pRequest = Handler::getInstance();
    pRequest->requestByUrlAndType(sBaseUrl, Handler::LessonDB);
//    pRequest->run();
#endif

    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addTeaLessonJSON?TLfkUseridNO=" + QString::number(userId) + "&TLfkScheduleidNO="
            + QString::number(scheduleId) + "&TLstate=" + "1" + "&TLtype=" + "" + Arg::tokenStr;

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonDB);

}

void LessonDB::find()
{
    m_curRequestCode = Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findTeaLessonJSON?TLfkUseridNO=" + QString::number(userId) + "&TLfkScheduleidNO="
            + QString::number(scheduleId) + Arg::tokenStr;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
    Handler *pRequest = Handler::getInstance();
//    pRequest->setRequestUrl(baseUrl, Handler::LessonDB);
    pRequest->requestByUrlAndType(url, Handler::LessonDB);
//    pRequest->run();
}

void LessonDB::del()
{
    m_curRequestCode = Delete;

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pRequest = Handler::getInstance();
    pRequest->requestByUrlAndType(sBaseUrl, Handler::LessonDB);
    //    pRequest->run();
}

void LessonDB::replyArrived(const QJsonObject &jsonObj)
{
    switch(m_curRequestCode) {
    case Add: {
        emit addFinish(jsonObj);
    }
        break;

    case Delete: {
        emit delFinish(jsonObj);
    }
        break;

    case Find: {
        emit findFinish(jsonObj);
    }
        break;

    default:
        break;
    }
}

//void LessonDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void LessonDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void LessonDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}
