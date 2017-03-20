#include "keyworddb.h"
#include "config.h"
#include <QDebug>

//void KeywordDB::findState(QJsonObject json)
//{
//    qDebug()<<"come in findState"<<endl;
//    emit findFinish(json);
//}

KeywordDB::KeywordDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = "http://101.200.176.87:8080/Desktop/urlFindAll.action";

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::keywordDBReplyArrived, this, &KeywordDB::requestReply);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void KeywordDB::find()
{
//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
    //https://api.douban.com/v2/book/1220562
    //http://101.200.176.87:8080/Desktop/urlDelResKeyword.action?id=5
//    Handler* handler = new Handler("http://101.200.176.87:8080/Desktop/urlFindAll.action");
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::KeywordDB);
//    pHandler->run();
}

void KeywordDB::requestReply(const QJsonObject &jsonObj)
{
    qDebug()<<"come in findState"<<endl;
    emit findFinish(jsonObj);
}
