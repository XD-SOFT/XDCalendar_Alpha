#include "clientconfigdb.h"
#include "config.h"
#include <QDebug>

ClientConfigDB::ClientConfigDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::clientConfigDBReplyArrived, this, &ClientConfigDB::handleReply);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void ClientConfigDB::add()
{
    m_curRequestType = Add;

//    QString url = Arg::sNetRequestHostUrl;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::ClientConfigDB);
//    pHandler->run();
}

void ClientConfigDB::update()
{
    m_curRequestType = Update;

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::ClientConfigDB);
//    pHandler->run();
}

void ClientConfigDB::find()
{
    m_curRequestType = Find;

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::ClientConfigDB);
//    pHandler->run();
}

void ClientConfigDB::handleReply(const QJsonObject &jsonObj)
{
    switch (m_curRequestType) {
    case Add:{
        emit addClientConfigFinished(jsonObj);
    }
        break;

    case Update: {
        emit updateClientConfigFinished(jsonObj);
    }
        break;

    case Find: {
        emit findClientConfigFinished(jsonObj);
    }
        break;
    default:
        break;
    }
}

//void ClientConfigDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void ClientConfigDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void ClientConfigDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}
