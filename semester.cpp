#include "semester.h"
#include <stdexcept>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

Semester::Semester (const QJsonObject& json, QObject* parent): QObject (parent)
{
    //qDebug()<<"cccc"<<endl;
    load (json);
}

static QString LoadErrMsgHdr = "Unable to load a Semester from this QJsonObject: ";
void Semester::load (const QJsonObject& json)
{

    QJsonValue idV = json["id"];
    if (idV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "idV is not Double").toStdString ());
    QJsonValue titleV = json["title"];
    if (titleV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "title is not String").toStdString ());
    QJsonValue termV = json["term"];
    if (termV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "term is not String").toStdString ());
    QJsonValue createTimeV = json["createtime"];
    if (createTimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "createtime is not String").toStdString ());
    QJsonValue updateTimeV = json["updatetime"];
    if (updateTimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "updatetime is not String").toStdString ());
    QJsonValue yearStartV = json["yearstart"];
    if (yearStartV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "yearstart is not String").toStdString ());
    QJsonValue ownershipV = json["ownership"];
    if (ownershipV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "ownership is not String").toStdString ());
    QJsonValue starttimeV = json["starttime"];
    if (starttimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "starttime is not String").toStdString ());
    QJsonValue endtimeV = json["endtime"];
    if (endtimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "endtime is not String").toStdString ());

    id = idV.toInt();
    mTitle = titleV.toString();
    mTerm = termV.toString();
    mCreateTime = QDate::fromString (createTimeV.toString (), Qt::ISODate);
    mUpdateTime = QDate::fromString (updateTimeV.toString (), Qt::ISODate);
    mYearStart = yearStartV.toInt();
    mOwnership = ownershipV.toInt();
    mStartTime = QDate::fromString (starttimeV.toString (), Qt::ISODate);
    mEndTime = QDate::fromString (endtimeV.toString (), Qt::ISODate);

    qDebug()<<"semester record:"<<mTitle<<' '<<mCreateTime<<' '<<mUpdateTime<<' '<<mYearStart<<' '<<mOwnership<<' '<<mStartTime<<' '<<mEndTime<<endl;

    update();
}

QJsonObject Semester::save () const
{
    return QJsonObject {{"id", QString::number(id)}, {"title", mTitle}, {"term", mTerm}, {"createtime", mCreateTime.toString (Qt::ISODate)}, {"updatetime", mUpdateTime.toString (Qt::ISODate)}, {"yearstart", mYearStart}, {"ownership", mOwnership}, {"starttime", mStartTime.toString (Qt::ISODate)}, {"endtime", mEndTime.toString (Qt::ISODate)}};
}
