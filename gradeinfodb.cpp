#include "gradeinfodb.h"
#include "handler.h"
#include "config.h"
#include <QDebug>


//void GradeInfoDB::visitState(QJsonObject json)
//{
//    qDebug()<<"come in visitState"<<endl;
//    emit updateUI(json);
//}


GradeInfoDB::GradeInfoDB(QObject *parent):
    QObject(parent)
{
    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::gradeInfoDBReplyArrived, this, &GradeInfoDB::requestReply);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(visitState(QJsonObject)));
}

void GradeInfoDB::query()
{
//    QString url = baseUrl;
//    qDebug()<<"url: "<<url<< endl;
    //https://api.douban.com/v2/book/1220562
    //http://101.200.176.87:8080/Desktop/urlDelResKeyword.action?id=5
//    Handler* handler = new Handler("http://101.200.176.87:8080/Desktop/urlFindAll.action");
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(visitState(QJsonObject)));
//    handler->run();
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sBaseUrl, Handler::GradeInfoDB);
//    pHandler->run();
}

void GradeInfoDB::requestReply(const QJsonObject &jsonObj)
{
    qDebug()<<"come in visitState"<<endl;
    emit updateUI(jsonObj);
}
