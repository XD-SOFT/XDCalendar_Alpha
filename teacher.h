#ifndef TEACHER_H
#define TEACHER_H
#include "term.h"
#include "semester.h"
#include "schedule.h"
#include <QObject>
#include <QPixmap>
#include <QFileInfo>
class QJsonObject;
class Upgradable
{
public:
    Upgradable () = default;
    Upgradable (int exp, int maxExp, int level): mExp (exp), mMaxExp (maxExp), mLevel (level) {}
    Upgradable (const QJsonObject& json) {load (json);}
    Upgradable (const Upgradable&) = default;
    Upgradable (Upgradable&&) = default;
    Upgradable& operator = (const Upgradable&) = default;
    Upgradable& operator = (Upgradable&&) = default;
    ~Upgradable () = default;

    void load (const QJsonObject& json);
    QJsonObject save () const;
    int exp () const {return mExp;}
    int maxExp () const {return mMaxExp;}
    int level () const {return mLevel;}
protected:
    friend class Teacher;
    int mExp = 0;
    int mMaxExp = 0;
    int mLevel = 0;
};

class Person
{
public:
    Person () = default;
    Person (const QString& name, const QString& password, const QFileInfo& iconFile, const QString& location, const QString& school, const QString& email, const QString& cellphone);
    Person (const QJsonObject&);
    Person (const Person& rhs):mName (rhs.mName), mPassword (rhs.mPassword), mIconFile (rhs.mIconFile), mLocation (rhs.mLocation), mSchool (rhs.mSchool), mEmail (rhs.mEmail), mCellPhone (rhs.mCellPhone) {}
    Person& operator = (const Person& rhs)
    {
        mName = rhs.mName;
        mPassword = rhs.mPassword;
        mIconFile = rhs.mIconFile;
        mLocation = rhs.mLocation;
        mSchool = rhs.mSchool;
        mEmail = rhs.mEmail;
        mCellPhone = rhs.mCellPhone;
        return *this;
    }

    Person (Person&& rhs): mName (std::move(rhs.mName)), mPassword (std::move(rhs.mPassword)), mIconFile (std::move(rhs.mIconFile)), mLocation (std::move(rhs.mLocation)), mSchool (std::move(rhs.mSchool)), mEmail (std::move(rhs.mEmail)), mCellPhone (std::move(rhs.mCellPhone)) {}

    Person& operator = (Person&& rhs)
    {
        mName = std::move(rhs.mName);
        mPassword = std::move(rhs.mPassword);
        mIconFile = std::move(rhs.mIconFile);
        mLocation = std::move(rhs.mLocation);
        mSchool = std::move(rhs.mSchool);
        mEmail = std::move(rhs.mEmail);
        mCellPhone = std::move(rhs.mCellPhone);
        return *this;
    }

    ~Person () = default;
    void load (const QJsonObject&);
    QJsonObject save () const;
    QPixmap openIcon () const
    {
        return QPixmap (mIconFile.absoluteFilePath ());
    }
protected:
    void clear ()
    {
        mName.clear ();
        mPassword.clear ();
        mIconFile = QFileInfo ();
        mLocation.clear ();
        mSchool.clear ();
        mEmail.clear ();
        mCellPhone.clear ();
    }

    //There is no validator on any member data
    QString mName;
    QString mPassword;
    QFileInfo mIconFile;
    QString mLocation;
    QString mSchool;
    QString mEmail;
    QString mCellPhone;
};

class Term;
class Semester;
class Teacher : public QObject, public Person, public Upgradable
{
    Q_OBJECT
public:
    explicit Teacher(const Person&, const Upgradable&, const QString& subject, const QVector<Term*>& terms,const QVector<Semester *> &semesters, QObject *parent = 0);
    Teacher (int exp, int maxExp, int level, const QString& name, const QString& password, const QFileInfo& iconFile, const QString& location, const QString& school, const QString& email, const QString& cellphone, const QString& subject, const QVector<Term*>& terms, const QVector<Semester *> &semesters, QObject *parent = 0);
    Teacher (const QJsonObject& , QObject* parent = 0);
    Teacher (const Teacher&) = delete;
    Teacher& operator = (const Teacher&) = delete;
    ~Teacher ();
    void load (const QJsonObject&);
    QJsonObject save () const;
    void clear ()
    {
        Person::clear ();
        mSubject.clear ();
        for (auto& t: mTerms)
            delete t;
        mTerms.clear ();
        for (auto& s: mSemesters)
            delete s;
        mSemesters.clear();
        for (auto& s: mSchedules)
            delete s;
        mSchedules.clear();
    }
    QString getName () const;
    QString getPassword () const {return mPassword;}
    QString getSubject () const {return mSubject;}
    QString getLocation () const {return mLocation;}
    QString getSchool () const {return mSchool;}
    QString getEmail () const {return mEmail;}
    QString getCellphone () const {return mCellPhone;}
    QFileInfo getIconFile () const {return mIconFile;}
    const QVector<Term*>& terms () {return mTerms;}
    QVector<Semester*>& semesters() {return mSemesters;}
    QVector<Schedule*>& schedules() {return mSchedules;}

    void setTerms(const QVector<Term*> &termsVec);

    void setSemesters(const QVector<Semester*> &semestersVec);


    void notify ()
    {
        emit nameChanged (mName);
        emit passwordChanged (mPassword);
        emit iconChanged (mIconFile);
        emit locationChanged (mLocation);
        emit schoolChanged (mSchool);
        emit emailChanged (mEmail);
        emit cellphoneChanged (mCellPhone);
        emit subjectChanged (mSubject);
        emit levelChanged (*this);
    }

signals:
    void subjectChanged (const QString& subject);
    void nameChanged (const QString& name);
    void passwordChanged (const QString& password);
    void iconChanged (const QFileInfo& file);
    void locationChanged (const QString& location);
    void schoolChanged (const QString& school);
    void emailChanged (const QString& email);
    void cellphoneChanged (const QString& cellphone);
    void levelChanged (Upgradable);
public slots:
    void setName (const QString& name)
    {
        mName = name;
        emit nameChanged (name);
    }

    void setPassword (const QString& password)
    {
        mPassword = password;
        emit passwordChanged (password);
    }

    void setIcon (const QFileInfo& file)
    {
        mIconFile = file;
        openIcon ();
        emit iconChanged (file);
    }

    void setLocation (const QString& location)
    {
        mLocation = location;
        emit locationChanged (location);
    }

    void setSchool (const QString& school)
    {
        mSchool = school;
        emit schoolChanged (school);
    }

    void setEmail (const QString& email)
    {
        mEmail = email;
        emit emailChanged (email);
    }

    void setCellphone (const QString& cellphone)
    {
        mCellPhone = cellphone;
        emit cellphoneChanged (cellphone);
    }

    void setSubject (const QString& subject)
    {
        mSubject = subject;
        emit subjectChanged (subject);
    }

    void setLevel (const Upgradable& rhs)
    {
        mExp = rhs.mExp;
        mMaxExp = rhs.mMaxExp;
        mLevel = rhs.mLevel;
        emit levelChanged (rhs);
    }
private:
    QString mSubject;
    QVector<Term*> mTerms;
    QVector<Semester*> mSemesters;
    QVector<Schedule*> mSchedules;
};

#endif // TEACHER_H
