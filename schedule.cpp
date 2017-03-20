#include "schedule.h"
#include <stdexcept>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDebug>

Schedule::Schedule (const QJsonObject& json, QObject* parent): QObject (parent)
{
    qDebug()<<"cccc"<<endl;
    load (json);
}

static QString LoadErrMsgHdr = "Unable to load a Schedule from this QJsonObject: ";
void Schedule::load (const QJsonObject& json)
{

    QJsonValue idV = json["id"];
    if (idV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "idV is not Double").toStdString ());

    QJsonValue termidV = json["fk_termid"];
    if (termidV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "termidV is not Double").toStdString ());

    QJsonValue nameV = json["name"];
    if (nameV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "name is not String").toStdString ());

    QJsonValue createTimeV = json["createtime"];
    if (createTimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "createtime is not String").toStdString ());

    QJsonValue updateTimeV = json["updatetime"];
    if (updateTimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "updatetime is not String").toStdString ());

    QJsonValue sectionV = json["section"];
    if (sectionV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "section is not String").toStdString ());

    QJsonValue gradesV = json["Grades"];
    if (gradesV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "grades is not String").toStdString ());
    QJsonValue starttimeV = json["Startdate"];
    if (starttimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "starttime is not String").toStdString ());
    QJsonValue endtimeV = json["Enddate"];
    if (endtimeV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "endtime is not String").toStdString ());

    id = idV.toInt();
    fk_termid = termidV.toInt();
    name = nameV.toString();
    mCreateTime = QDate::fromString (createTimeV.toString (), Qt::ISODate);
    mUpdateTime = QDate::fromString (updateTimeV.toString (), Qt::ISODate);
    section = sectionV.toInt();
    mGrades = gradesInt(gradesV.toString());
    mStartDate = QDate::fromString (starttimeV.toString (), Qt::ISODate);
    mEndDate = QDate::fromString (endtimeV.toString (), Qt::ISODate);

    qDebug()<<"schedule record:"<<name<<' '<<mCreateTime<<' '<<mUpdateTime<<' '<<section<<' '<<mStartDate<<' '<<mEndDate<<endl;

    update();
}

QJsonObject Schedule::save ()
{
    //QString gStr = this->gradesStr();
    return QJsonObject {{"id", QString::number(id)}, {"fk_termid", QString::number(fk_termid)},
      {"name", name}, {"createtime", mCreateTime.toString (Qt::ISODate)},
      {"updatetime", mUpdateTime.toString (Qt::ISODate)},
      {"section", QString::number(section)}, {"Grades", gradesStr()},
      {"Startdate", mStartDate.toString (Qt::ISODate)},{"Enddate", mEndDate.toString (Qt::ISODate)}};
}
