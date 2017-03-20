#include "resfilesdb.h"
#include "config.h"
#include <QDebug>

ResFilesDB::ResFilesDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::resFileDBReplayArrived, this, &ResFilesDB::replyArrived);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void ResFilesDB::add()
{
    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //urlAddResFiles.action?fkLessondetail=1&remark=鲁迅&updatetime=2016-10-14&state=2&usedatetime=2016-10-14&url=www.aa.com&filename=清明上河图
    QString url = sBaseUrl + "urlAddResFiles.action?fkLessondetail=" + QString::number(lessonDetailId) + "&remark=" + remark + "&updatetime="
                + updatetime.toString("yyyy-MM-dd") + "&state=" + QString::number(state) + "&usedatetime=" + usedatatime.toString("yyyy-M-d") + "&url=" + fileurl + "&filename="
                + filename + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ResFileDB);
//    pHandler->run();
}

void ResFilesDB::del()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //urlDelResFiles.action?id=9
    QString url = sBaseUrl + "urlDelResFiles.action?id=" + QString::number(id) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ResFileDB);
//    pHandler->run();
}

void ResFilesDB::update()
{
    m_curRequestCode = Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //urlUpdateResFiles.action?id=10&state=1&remark=鲁迅&userid=1&filename=清明上河图&fkLessondetail=1&updatetime=2016-10-14&usedatetime=2016-10-14&url=www.aa.com
    //&filecode=dsdsdsfa11d&filepic=aaa&fileico=bbb&keyword=3,4,5
    QString url = sBaseUrl + "urlUpdateResFiles.action?fkLessondetail=" + QString::number(lessonDetailId) + "&remark=" + remark + "&updatetime="
            + updatetime.toString("yyyy-MM-dd") + "&state=" + QString::number(state) + "&usedatetime=" + usedatatime.toString("yyyy-M-d") + "&url=" + fileurl + "&filename="
            + filename + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ResFileDB);
//    pHandler->run();
}

void ResFilesDB::find()
{
    m_curRequestCode = Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "urlSearchByfkLessondetail.action?fkLessondetail=" + QString::number(lessonDetailId)
            + "&usedatetime=" + usedatatime.toString("yyyy-M-d") + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ResFileDB);
//    pHandler->run();
}

void ResFilesDB::replyArrived(const QJsonObject &jsonObj)
{
    switch (m_curRequestCode) {
    case Add:{
        emit addFinish(jsonObj);
    }
        break;

    case Delete:{
        emit delFinish(jsonObj);
    }
        break;

    case Update:{
        emit updateFinish(jsonObj);
    }
        break;

    case Find:{
        emit findFinish(jsonObj);
    }
        break;

    default:
        break;
    }
}

//void ResFilesDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void ResFilesDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void ResFilesDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}

//void ResFilesDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}
