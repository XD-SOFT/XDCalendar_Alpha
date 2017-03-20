#include "subjectinfodb.h"
#include "handler.h"
#include "config.h"

//void SubjectInfoDB::visitState(QJsonObject json)
//{
//    qDebug()<<"come in visitState"<<endl;
//    emit updateUI(json);
//}


SubjectInfoDB::SubjectInfoDB(QObject *parent):
    QObject(parent)
{
//     baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

     Handler *pHandler = Handler::getInstance();
     connect(pHandler, &Handler::subjectInfoDBReplyArrived, this, &SubjectInfoDB::replyArrived);
//     connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(visitState(QJsonObject)));
}

void SubjectInfoDB::query()
{
//    qDebug()<<"baseUrl: "<<baseUrl<<endl;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }
//    baseUrl = "http://101.200.176.87:8080/Desktop/";
    QString url = sBaseUrl + "subjectInfoJSON?" + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(visitState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::SubjectInfoDB);
//    pHandler->run();
}

void SubjectInfoDB::replyArrived(const QJsonObject &jsonObj)
{
    qDebug()<<"come in visitState"<<endl;
    emit updateUI(jsonObj);
}
