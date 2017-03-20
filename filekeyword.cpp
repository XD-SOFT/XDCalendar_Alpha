#include "filekeyword.h"
#include "handler.h"
#include "config.h"
#include <QDebug>

FileKeyword::FileKeyword(QObject *parent):
    QObject(parent)
{
//    baseUrl = "http://101.200.176.87:8080/Desktop/urlFindAll.action";

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::fileKeyWordDBReplyArrived, this, &FileKeyword::handleReply);

//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void FileKeyword::add()
{
    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
    //https://api.douban.com/v2/book/1220562
    //http://101.200.176.87:8080/Desktop/urlDelResKeyword.action?id=5
    //    Handler* handler = new Handler("http://101.200.176.87:8080/Desktop/urlFindAll.action");
    //    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
    //    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::FileKeywordDB);
//    pHandler->run();
}

void FileKeyword::del()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
    //https://api.douban.com/v2/book/1220562
    //http://101.200.176.87:8080/Desktop/urlDelResKeyword.action?id=5
//    Handler* handler = new Handler("http://101.200.176.87:8080/Desktop/urlFindAll.action");
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::FileKeywordDB);
//    pHandler->run();
}

void FileKeyword::find()
{
    m_curRequestCode = Find;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
    //https://api.douban.com/v2/book/1220562
    //http://101.200.176.87:8080/Desktop/urlDelResKeyword.action?id=5
//    Handler* handler = new Handler("http://101.200.176.87:8080/Desktop/urlFindAll.action");
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::FileKeywordDB);
//    pHandler->run();
}

void FileKeyword::handleReply(const QJsonObject &jsonObj)
{
    switch(m_curRequestCode) {
    case Add: {
        emit addFileKeywordFinished(jsonObj);
    }
        break;

    case Delete: {
        emit deleteFileKeywordFinished(jsonObj);
    }
        break;

    case Find: {
        emit findKeywordFinished(jsonObj);
    }
        break;

    default:
        break;
    }
}


//void FileKeyword::addState(QJsonObject json)
//{
//    qDebug()<<"come in addState"<<endl;
//    emit addFinish(json);
//}

//void FileKeyword::delState(QJsonObject json)
//{
//    qDebug()<<"come in delState"<<endl;
//    emit delFinish(json);
//}

//void FileKeyword::findState(QJsonObject json)
//{
//    qDebug()<<"come in findState"<<endl;
//    emit findFinish(json);
//}
