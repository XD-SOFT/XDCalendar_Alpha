#include "lesson.h"
#include "lessonremarkdb.h"
#include "filenumwidget.h"
#include "config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <stdexcept>
#include <algorithm>
#include <QColor>
#include <QDebug>

using namespace Const;

Lesson::Lesson(int section, const QString& subject, const QString& location, const QString& unit, const QString& note, Folder* source, QColor color, const QDate& date, QObject *parent):
    QObject(parent),
    mSectionNumber (section),
    mSubject (subject),
    mLocation (location),
    mUnit (unit),
    mNote (note),
    mRootFolder (source),
    mColor (color),
    mDate (date)
{
    mRootFolder->setParent (this);
    notify ();

    m_bNoteLoaded = false;
}

Lesson::~Lesson () = default;

Lesson::Lesson (const QJsonObject& json, QObject* parent) : QObject (parent)
{
    load (json);

    m_bNoteLoaded = false;
}

Lesson::Lesson(const Lesson &otherLesson)
{
    lessonid = otherLesson.getLessonId();
    classid = otherLesson.getClassId();
    roomid = otherLesson.getRoomId();
    lessonDetailId = otherLesson.getLessonDetailId();
    mSectionNumber = otherLesson.getSection();
    weekId = otherLesson.getWeekId();
    mSubject = otherLesson.subject();
    mLocation = otherLesson.location();
    mUnit = otherLesson.unit();
    mNote = otherLesson.note();

    mRootFolder = new Folder("课程课件", this);

    mColor = otherLesson.getColor();

    mDate = otherLesson.getDate();

    startDate = otherLesson.getStartDate();

    endDate = otherLesson.getEndDate();

    m_bNoteLoaded = false;

    m_nRepeat = otherLesson.getRepeat();
}

void Lesson::setStartTime(const QString &sStartTime)
{
    m_sStartTime.clear();
    m_sStartTime.append(sStartTime);
}

void Lesson::getStartTime(QString &sStartTime)
{
    sStartTime = m_sStartTime;
}

void Lesson::setEndTime(const QString &sEndTime)
{
    m_sEndTime.clear();
    m_sEndTime.append(sEndTime);
}

void Lesson::getEndTime(QString &sEndTime)
{
    sEndTime = m_sEndTime;
}

int Lesson::getResourceCount() const
{
    return lesFileNumWidget->getFileNum();
}

bool Lesson::noteLoaded() const
{
    return m_bNoteLoaded;
}

void Lesson::setNote(const QString &note)
{
    /*
            if(mNote != note && note != " ")
            {
                bool update = false;
                if(mNote != " ") update = true;
                //写数据库
                qDebug()<<"remark: "<<mNote<<endl;
                LessonRemarkDB* lesDB = new LessonRemarkDB();
                lesDB->setLessonDetailId(lessonDetailId);
                lesDB->setTeachingDate(mDate);
                lesDB->setRemark(mNote);

                if(update)
                {
                    lesDB->update();
                }
                else
                {
                    lesDB->add();
                }
            }
            */


    mNote = note;
    m_bNoteLoaded = true;
    //            emit noteChanged (note);
}

Folder *Lesson::rootFolder()
{
    return mRootFolder;
}

void Lesson::setRootFolder(Folder *f)
{
    mRootFolder = f;
}

QColor Lesson::getColor() const
{
    return mColor;
}

void Lesson::setColor(const QColor &color)
{
    mColor = color;
    emit colorChanged (color);
}

QDate Lesson::getDate() const
{
    return mDate;
}

void Lesson::setDate(const QDate &date)
{
    mDate = date;
//    emit dateChanged (date);
}

int Lesson::getLessonDetailId() const
{
    return lessonDetailId;
}

void Lesson::setLessonDetailId(int lessonDetailId)
{
    this->lessonDetailId = lessonDetailId;
}

void Lesson::setWeekId(int weekId)
{
    this->weekId = weekId;
}

int Lesson::getWeekId() const
{
    return weekId;
}

int Lesson::getSection() const
{
    return mSectionNumber;
}

void Lesson::setLessonId(int lessonid)
{
    this->lessonid = lessonid;
}

int Lesson::getLessonId() const
{
    return lessonid;
}

void Lesson::setClassId(int classid)
{
    this->classid = classid;
}

int Lesson::getClassId() const
{
    return classid;
}

void Lesson::setRoomId(int roomid)
{
    this->roomid = roomid;
}

int Lesson::getRoomId() const
{
    return roomid;
}

void Lesson::setStartDate(const QDate &startdate)
{
    this->startDate = startdate;
}

QDate Lesson::getStartDate() const
{
    return startDate;
}

void Lesson::setEndDate(const QDate &enddate)
{
    this->endDate = enddate;
}

QDate Lesson::getEndDate() const
{
    return endDate;
}

void Lesson::setRepeat(int repeat)
{
    m_nRepeat = repeat;
}

int Lesson::getRepeat() const
{
    return m_nRepeat;
}

void Lesson::clear()
{
    mSubject.clear ();
    mLocation.clear ();
    mUnit.clear ();
    mNote.clear ();
    if (mRootFolder != nullptr)
        delete mRootFolder;
    mRootFolder  = nullptr;
}

static QString LoadErrMsgHdr = "Unable to load a Lesson from this QJsonObject: ";
void Lesson::load (const QJsonObject& json)
{
    QJsonValue snV = json["section number"];
    if (snV.type () != QJsonValue::Double)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "section number is not a Double").toStdString ());
    QJsonValue subjectV = json["subject"];
    if (subjectV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "subject is not a String").toStdString ());
    QJsonValue locationV = json["location"];
    if (locationV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "location is not a String").toStdString ());
    QJsonValue unitV = json["unit"];
    if (unitV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "unit is not a String").toStdString ());
    QJsonValue noteV = json["note"];
    if (noteV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "note is not a String").toStdString ());
    QJsonValue folderV = json["folder"];
    if (folderV.type () != QJsonValue::Object)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "folder is not a Object").toStdString ());

    QJsonValue colorV = json["color"];
    if (colorV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "color is not a Array").toStdString ());
    QJsonArray colorA = colorV.toArray ();
    if (colorA.count () != 4)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "color's length is not 4(r, g, b, a)").toStdString ());
    if (std::end (colorA) != std::find_if (std::begin (colorA), std::end (colorA), [] (const QJsonValue& v) {return v.type () != QJsonValue::Double;}))
        throw std::invalid_argument (QString(LoadErrMsgHdr + "color is not all Double").toStdString ());
    QJsonValue dateV = json["date"];

    if (dateV.type () != QJsonValue::String)
    {
        throw std::invalid_argument (QString(LoadErrMsgHdr + "date is not a String").toStdString ());
    }

    clear ();
    mSectionNumber = static_cast<int>(snV.toDouble ());
    mSubject = subjectV.toString ();
    mLocation = locationV.toString ();
    mUnit = unitV.toString ();
    mNote = noteV.toString ();
    mDate = QDate::fromString (dateV.toString (), Qt::ISODate);
    mRootFolder = new Folder (folderV.toObject (), this);
    mColor = QColor::fromRgb (static_cast<ushort>(colorA[0].toDouble ()), static_cast<ushort>(colorA[1].toDouble ()), static_cast<ushort>(colorA[2].toDouble ()), static_cast<ushort>(colorA[3].toDouble ()));
    notify ();
}

QJsonObject Lesson::save () const
{
    QJsonObject folderObject = mRootFolder == nullptr? QJsonObject () : mRootFolder->save ();
    QJsonArray colorArray = {mColor.red (), mColor.green (), mColor.blue (), mColor.alpha ()};
    return QJsonObject {{"section number", mSectionNumber}, {"subject", mSubject}, {"location", mLocation}, {"unit", mUnit}, {"note", mNote}, {"folder", folderObject}, {"color", colorArray}, {"date", mDate.toString (Qt::ISODate)}};
}

void Lesson::notify()
{
    emit sectionNumberChanged (mSectionNumber);
    emit subjectChanged (mSubject);
    emit locationChanged (mLocation);
    emit unitChanged (mUnit);
    emit noteChanged (mNote);
    emit colorChanged (mColor);
    emit dateChanged (mDate);
}
