#include "lessonremarkdb.h"
#include "config.h"
#include <QDebug>

LessonRemarkDB::LessonRemarkDB(QObject *parent) :
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, &Handler::lessonRemarkDBReplyArrived, this, &LessonRemarkDB::replyArrived);
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
    //    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
}

void LessonRemarkDB::setId(int id)
{
    this->id = id;
}

int LessonRemarkDB::getId()
{
    return id;
}

void LessonRemarkDB::setLessonDetailId(int lessonDetailId)
{
    this->lessonDetailId = lessonDetailId;
}

int LessonRemarkDB::getLessonDetailId()
{
    return lessonDetailId;
}

void LessonRemarkDB::setRemark(const QString &remark)
{
    this->remark = remark;
}

QString LessonRemarkDB::getRemark()
{
    return remark;
}

void LessonRemarkDB::setTeachingDate(const QDate &teachingDate)
{
    this->teachingDate = teachingDate;
}

QDate LessonRemarkDB::getTeachingDate()
{
    return teachingDate;
}

void LessonRemarkDB::setState(int state)
{
    this->state = state;
}

int LessonRemarkDB::getState()
{
    return state;
}

//void LessonRemarkDB::setBaseUrl(const QString &url)
//{
//    this->baseUrl = url;
//}

//QString LessonRemarkDB::getBaseUrl()
//{
//    return baseUrl;
//}

void LessonRemarkDB::add()
{
    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "addTeaLessonRemarkJSON?TLDid=" + QString::number(lessonDetailId)
            + "&teachingdate=" + teachingDate.toString("yyyy-MM-dd") + "&TLRremark="
            + remark + "&TLRstate=1" + Arg::tokenStr;
//    QString url = baseUrl + "addTeaLessonRemarkJSON?" + "&teachingdate=" + teachingDate.toString("yyyy-MM-dd") + "&TLRremark="
//            + remark + "&TLRstate=1" + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonRemarkDB);
//    pHandler->run();
}

void LessonRemarkDB::del()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "delTeaLessonRemarkJSON?TLDid=" + QString::number(lessonDetailId)
            + "&teachingdate=" + teachingDate.toString("yyyy-MM-dd") + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;

//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonRemarkDB);
//    pHandler->run();
}

void LessonRemarkDB::update()
{
    m_curRequestCode = Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "updateTeaLessonRemarkJSON?TLDid=" + QString::number(lessonDetailId)
            + "&TLRremark=" + remark + "&teachingdate=" + teachingDate.toString("yyyy-MM-dd")
            + "&TLRstate=1" + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonRemarkDB);
//    pHandler->run();
}

void LessonRemarkDB::find()
{
    m_curRequestCode = Find;

    qDebug() << "the teachingDate is:" << teachingDate;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findTeaLessonRemarkJSON?TLDid=" + QString::number(lessonDetailId)
            + "&teachingdate=" + teachingDate.toString("yyyy-MM-dd") + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonRemarkDB);
//    pHandler->run();
}

void LessonRemarkDB::replyArrived(const QJsonObject &jsonObj)
{
    qDebug() << "the LessonRemarkDB replyArrived jsonObj is:" << jsonObj;

    switch (m_curRequestCode) {
    case Add:{
        emit addFinish(jsonObj);
    }
        break;

    case Find:{
        QString sRemark;
        int nDetailID = jsonObj["TLDid"].toInt();
        QString statue = jsonObj["status"].toString();

        if (statue == "true")
        {
            QJsonObject remarkObj = jsonObj["result"].toObject();
            sRemark.append(remarkObj["remark"].toString());
            //            int detailId = json["TLDid"].toInt();
//            QString date = json["Teachingdate"].toString().split(" ").at(0);
//            QPair<int, QString> course;
//            course.first = detailId;
//            course.second = date;
//            Arg::noteMap.insert(course, remark);
        }

        remark.clear();
        remark.append(sRemark);

        emit findFinish(nDetailID, sRemark);
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

    default:
        break;
    }
}

//void LessonRemarkDB::addState(QJsonObject json)
//{
//    emit addFinish(json);
//}

//void LessonRemarkDB::findState(QJsonObject json)
//{
//    emit findFinish(json);
//}

//void LessonRemarkDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}

//void LessonRemarkDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}
