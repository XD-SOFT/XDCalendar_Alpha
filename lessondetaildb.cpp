#include "lessondetaildb.h"
#include "config.h"
#include <QDebug>

LessonDetailDB::LessonDetailDB(QObject *parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler* pHandler = Handler::getInstance();
    connect(pHandler, &Handler::lessonDetailDBReplyArrived, this, &LessonDetailDB::replyArrived);

//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
    //    connect(pHandler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findAllState(QJsonObject)));
}

void LessonDetailDB::setRepeat(int repeat)
{
    this->repeat = repeat;
}

void LessonDetailDB::add()
{
    m_curRequestCode = Add;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }
    //addTeaLessonDetailJSON?TLid=1&TLDclassid=1&TLDclassname=1&TLDroomid=1&TLDroomno=1&TLDweekid=1&TLDsectionid=1&TLDstartdate=2016-10-10&TLDenddate=2016-10-11&TLDstate=1&TLDrepeatperoid=1
    QString url = sBaseUrl + "addTeaLessonDetailJSON?TLid=" + QString::number(lessonId) +
            "&TLDclassid=1" + "&TLDclassname=" + className + "&TLDroomid=1" + "&TLDroomno=" + room + "&infoSubjectId="
            + QString::number(Arg::subjectMap[subject])
            + "&TLDweekid=" + QString::number(weekId) + "&TLDsectionid=" + QString::number(sectionId)
            + "&TLDstartdate=" + startDate.toString("yyyy-MM-dd") + "&TLDenddate=" + endDate.toString("yyyy-MM-dd")
            + "&TLDstate=1&TLDrepeatperiod=" + QString::number(repeat) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;

    //return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(addState(QJsonObject)));
//    handler->run();

     Handler* pHandler = Handler::getInstance();
     pHandler->requestByUrlAndType(url, Handler::LessonDetailDB);
//     pHandler->run();
}

void LessonDetailDB::del()
{
    m_curRequestCode = Delete;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "delTeaLessonDetailJSON?TLDid=" + QString::number(id) + "&time="
            + endDate.toString("yyyy-MM-dd") + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;

    //return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(delState(QJsonObject)));
//    handler->run();
    Handler* pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonDetailDB);
//    pHandler->run();
}

void LessonDetailDB::update()
{
    m_curRequestCode = Update;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    //updateTeaLessonDetailJSON?TLDid=1&TLid=1&TLDclassid=1&TLDclassname=1&TLDroomid=1&TLDroomno=1&TLDweekid=1&TLDsectionid=1&TLDstartdate=2016-10-10&TLDenddate=2016-10-11&TLDstate=1&TLDrepeatperoid=1
    QString url = sBaseUrl + "updateTeaLessonDetailJSON?TLDid=" + QString::number(id) + "&TLid=" + QString::number(lessonId) +
            "&infoSubjectId=" + QString::number(Arg::subjectMap[subject]) +
            "&TLDclassid=1&TLDroomid=1&TLDclassname=" + className + "&TLDroomno=" + room
            + "&TLDweekid=" + QString::number(weekId) + "&TLDsectionid=" + QString::number(sectionId)
            + "&TLDstartdate=" + startDate.toString("yyyy-MM-dd") + "&TLDenddate=" + endDate.toString("yyyy-MM-dd")
            + "&TLDstate=1&TLDrepeatperiod=" + QString::number(repeat) + Arg::tokenStr;

    qDebug()<<"url: "<<url<< endl;

    //return;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(updateState(QJsonObject)));
//    handler->run();
    Handler* pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonDetailDB);
//    pHandler->run();
}

void LessonDetailDB::findAll()
{
    m_curRequestCode = FindAll;

    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "findTeaLessonDetailJSON?TLid=" + QString::number(lessonId) + Arg::tokenStr;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(findAllState(QJsonObject)));
//    handler->run();
    Handler* pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::LessonDetailDB);
//    pHandler->run();
}

void LessonDetailDB::replyArrived(const QJsonObject &jsonObj)
{
    switch (m_curRequestCode) {
    case Add:{
        emit addFinish(jsonObj);
    }
        break;

    case Delete:{
        emit delFinish(id, jsonObj);
    }
        break;

    case Update:{
        emit updateFinish(jsonObj);
    }
        break;

    case FindAll: {
        emit findAllFinish(jsonObj);
    }
        break;

    default:
        break;
    }

    qDebug() << "Lesson detail db jsonObj" << m_curRequestCode << jsonObj;
}

//void LessonDetailDB::addState(QJsonObject json)
//{
//    qDebug()<<"addState: "<<json<<endl;
//    emit addFinish(json);
//}

//void LessonDetailDB::findAllState(QJsonObject json)
//{
//    emit findAllFinish(json);
//}

//void LessonDetailDB::delState(QJsonObject json)
//{
//    emit delFinish(json);
//}

//void LessonDetailDB::updateState(QJsonObject json)
//{
//    emit updateFinish(json);
//}
