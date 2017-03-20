#include "teacher.h"
#include "term.h"
#include "semester.h"
#include "config.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonArray>
void Upgradable::load (const QJsonObject& json)
{
    auto getInt = [&] (const QString& key) -> int
    {
        QJsonValue value = json[key];
        if (value.type () != QJsonValue::Double)
            throw std::invalid_argument (QString("Unable to load a Level from QJsonObject: " + key + "is not Double").toStdString ());
        return static_cast<int>(value.toDouble());
    };
    int exp = getInt ("exp");
    int maxExp = getInt ("max exp");
    int level = getInt ("level");

    mExp = exp;
    mMaxExp = maxExp;
    mLevel = level;
}
QJsonObject Upgradable::save () const
{
    return QJsonObject {{"exp", mExp}, {"max exp", mMaxExp}, {"level", mLevel}};
}

//=======================================================
Person::Person (const QString& name, const QString& password, const QFileInfo& iconFile, const QString& location, const QString& school, const QString& email, const QString& cellphone): mName (name), mPassword (password), mIconFile (iconFile), mLocation (location), mSchool (school), mEmail (email), mCellPhone (cellphone)
{
}

Person::Person (const QJsonObject& json)
{
    load (json);
}

void Person::load (const QJsonObject& json)
{
    auto getString = [&] (const QString& key) -> QString
    {
        QJsonValue value = json[key];
        if (value.type () != QJsonValue::String)
            throw std::invalid_argument (QString("Unable to load a Person from QJsonObject: " + key + "is not String").toStdString ());
        return value.toString ();
    };
    QString name = getString ("name");
    QString password = getString ("password");
    QString iconPath = getString ("icon path");
    QString location = getString ("location");
    QString school = getString ("school");
    QString email = getString ("email");
    QString cellphone = getString ("cellphone");

    clear ();

    mName = name;
    mPassword = password;

    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);
    mIconFile.setFile (/*Arg::configDir*/dir, iconPath);
    qDebug() << mIconFile.absoluteFilePath ();
    mLocation = location;
    mSchool = school;
    mEmail = email;
    mCellPhone = cellphone;
}

QJsonObject Person::save () const
{
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);

    return QJsonObject {{"name", mName}, {"password", mPassword},
        {"icon path", /*Arg::configDir*/dir.relativeFilePath (mIconFile.absoluteFilePath ())}, {"location", mLocation}, {"school", mSchool}, {"email", mEmail}, {"cellphone", mCellPhone}};
}


Teacher::Teacher (const Person& person, const Upgradable& up, const QString& subject, const QVector<Term*>& terms,  const QVector<Semester*>& semesters, QObject* parent): QObject (parent), Person (person), Upgradable (up), mSubject (subject), mTerms (terms), mSemesters(semesters)
{
    notify ();
}

Teacher::Teacher (int exp, int maxExp, int level, const QString& name, const QString& password, const QFileInfo& iconFile, const QString& location, const QString& school, const QString& email, const QString& cellphone, const QString& subject, const QVector<Term*>& terms, const QVector<Semester*>& semesters, QObject* parent): QObject (parent), Person (name, password, iconFile, location, school, email, cellphone), Upgradable (exp, maxExp, level), mSubject (subject), mTerms (terms), mSemesters(semesters)
{
    notify ();
}

Teacher::Teacher (const QJsonObject& json, QObject* parent): QObject (parent)
{
//    load (json);
}

static QString LoadTeacherErrMsg = "Unable to load a Teacher from this QJsonObject:";
void Teacher::load (const QJsonObject& json)
{
    QJsonValue subjectV = json["subject"];
    if (subjectV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "subject is not String").toStdString ());
    QJsonValue termV = json["terms"];
    if (termV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "terms is not Array").toStdString ());
    QJsonArray termA = termV.toArray ();
    if (std::end (termA) != std::find_if (std::begin (termA), std::end (termA), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
    {
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "terms is not all Object").toStdString ());
    }
    QJsonValue levelV = json["level"];
    if (levelV.type () != QJsonValue::Object)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "level is not Object").toStdString ());
    QJsonValue personV = json["person"];
    if (personV.type () != QJsonValue::Object)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "person is not Object").toStdString ());

    /*
    QJsonValue semestersV = json["semesters"];
    if (semestersV.isUndefined()){

    }
    if (semestersV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "semesters is not Array").toStdString ());
    QJsonArray semestersA = semestersV.toArray ();
    if (std::end (semestersA) != std::find_if (std::begin (semestersA), std::end (semestersA), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
    {
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "semesters is not all Object").toStdString ());
    }


    QJsonValue schedulesV = json["schedules"];
    if (schedulesV.isUndefined()){

    }
    if (schedulesV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "schedules is not Array").toStdString ());
    QJsonArray schedulesA = schedulesV.toArray ();
    if (std::end (schedulesA) != std::find_if (std::begin (schedulesA), std::end (schedulesA), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
    {
        throw std::invalid_argument (QString(LoadTeacherErrMsg + "schedules is not all Object").toStdString ());
    }
    */

    clear ();
    Person::load (personV.toObject ());
    Upgradable::load (levelV.toObject ());
    mSubject = subjectV.toString ();
    for (auto t: termA)
    {
        mTerms.push_back (new Term (t.toObject (), this));
    }

    /*
    for (auto s: semestersA)
    {
        mSemesters.push_back (new Semester (s.toObject (), this));
    }
    for (auto sche: schedulesA)
    {
        mSchedules.push_back (new Schedule (sche.toObject (), this));
    }
    */

    notify ();
}

QJsonObject Teacher::save () const
{
    QJsonArray termA;
    //QJsonArray semestersA;
    for (const auto& t: mTerms)
    {
        termA.push_back (t->save ());
    }

    /*
    for (Semester * const &s: mSemesters)
    {
        semestersA.push_back (s->save ());
    }
    */

    return QJsonObject {{"person", Person::save ()}, {"level", Upgradable::save ()}, {"subject", mSubject}, {"terms", termA} /*, {"semesters", semestersA}*/ };
}

QString Teacher::getName() const
{
    return Arg::username;
}

void Teacher::setTerms(const QVector<Term *> &termsVec)
{
    mTerms = termsVec;
}

void Teacher::setSemesters(const QVector<Semester *> &semestersVec)
{
    mSemesters = semestersVec;
}


Teacher::~Teacher () = default;
