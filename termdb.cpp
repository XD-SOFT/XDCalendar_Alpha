#include "termdb.h"
#include "config.h"

//void TermDB::addState(QJsonObject json)
//{
//    qDebug()<<"come in addState"<<endl;
//    emit addFinish(json);
//}

//void TermDB::delState(QJsonObject json)
//{
//    qDebug()<<"come in delState"<<endl;
//    emit delFinish(json);
//}

//void TermDB::updateState(QJsonObject json)
//{
//    qDebug()<<"come in updateState"<<endl;
//    emit updateFinish(json);
//}

//void TermDB::queryAllState(QJsonObject json)
//{
//    qDebug()<<"come in queryAllState"<<endl;
//    emit queryAllFinish(json);
//}

//void TermDB::querySchoolState(QJsonObject json)
//{
//    qDebug()<<"come in querySchoolState"<<endl;
//    emit querySchoolFinish(json);
//}

//void TermDB::queryUserState(QJsonObject json)
//{
//    qDebug()<<"come in queryUserState"<<endl;
//    emit queryUserFinish(json);
//}


void TermDB::queryAll()
{
    m_curRequestCode = RequestCode::GetAll;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "getStaTermJsonByUserId?ownership=" + QString::number(Arg::userId) + Arg::tokenStr;
    //QString url = baseUrl + "getStaTermJSON" + Arg::tokenStr2;
    qDebug()<<"url: "<<url<< endl;
    request(url);
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryAllState(QJsonObject)));
//    handler->run();

}

void TermDB::querySchool()
{
    m_curRequestCode = RequestCode::AllBySchoolCreate;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "getStaTermbyOwnershipJSON?ownership=0" + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
    request(url);
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(querySchoolState(QJsonObject)));
//    handler->run();
}

void TermDB::queryUser()
{
    m_curRequestCode = RequestCode::AllByUserCreate;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "getStaTermbyOwnershipJSON?ownership=" + QString::number(Arg::userId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
    request(url);
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryUserState(QJsonObject)));
    //    handler->run();
}

void TermDB::queryTermByDate(const QDate &date)
{
    m_curRequestCode = RequestCode::GetByDate;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString sRequestUrl = sBaseUrl + "findStaTermByTime?time=" + date.toString("yyyy-MM-dd")
            + Arg::tokenStr;

    request(sRequestUrl);
}

void TermDB::request(const QString &sUrl)
{
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(sUrl, Handler::TermDB);
//    pHandler->run();
}

void TermDB::handleReply(const QJsonObject &jsonObj)
{
    switch(m_curRequestCode){
    case RequestCode::Add: {
        emit addTermFinished(jsonObj);
    }
        break;

    case RequestCode::Delete: {
        emit deleteTermFinished(jsonObj);
    }
        break;

    case RequestCode::Update: {
        emit updateTermFinished(jsonObj);
    }
    break;

    case RequestCode::GetAll: {
        emit queryAllTermFinished(jsonObj);
    }
        break;

    case RequestCode::AllBySchoolCreate: {
        emit querySchoolTermFinished(jsonObj);
    }
        break;

    case RequestCode::AllByUserCreate: {
        emit queryUserTermFinished(jsonObj);
    }
        break;

    case RequestCode::GetByDate:{
        emit queryTermByDateFinished(jsonObj);
    }
        break;

    }
}

TermDB::TermDB(QObject *parent):
    QObject(parent)
{
    qDebug()<<"come in terdb"<<endl;
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;
//    qDebug()<<"termdb baseurl: "<<baseUrl<<endl;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, SIGNAL(termDBReplyArrived(const QJsonObject&)), this, SLOT(handleReply(const QJsonObject&)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryUserState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(querySchoolState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(queryAllState(QJsonObject)));
}

void TermDB::add()
{ 
    m_curRequestCode = RequestCode::Add;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addStaTermJSON?title=" + title + "&yearstart="
            + yearstart + "&ownership=" + QString::number(ownership) + "&starttime=" + starttime.toString("yyyy-MM-dd")
            + "&endtime=" + endtime.toString("yyyy-MM-dd") + "&term=" + term + Arg::tokenStr;

    qDebug()<<"term add url: "<<url<< endl;
//    Handler* handler = new Handler(url);
    request(url);
}

void TermDB::delUserTerm()
{
    m_curRequestCode = RequestCode::Delete;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "delStaTermJSON?id=" + id + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);

    request(url);
}

void TermDB::updateUserTerm()
{
    m_curRequestCode = RequestCode::Update;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "updateStaTermJSON?id=" + QString::number(id) + "&title=" +  title + "&yearstart="
            + yearstart + "&ownership=" + QString::number(ownership) + "&starttime=" + starttime.toString("yyyy-MM-dd")
            + "&endtime=" + endtime.toString("yyyy-MM-dd") + "&term=" + term + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);

    request(url);
}
