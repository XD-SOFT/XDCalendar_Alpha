#include "config.h"
#include "teacher.h"
#include "lesson.h"
#include "term.h"
#include "semester.h"
#include "getdatanw.h"
#include "messagedisplaywidget.h"
#include <QStandardPaths>
#include <stdexcept>
#ifdef TEST
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#endif
#include <QMessageBox>
#include <QSettings>

Arg* Arg::instance = Arg::getInstance ();
int Arg::sUpLoadFileCount = 0;
int Arg::sDownLoadFileCount = 0;

int Arg::isMousePenetration=false;
int Arg::lockDrag=false;
int Arg::showChineseCalendar=true;
int Arg::mergeLoad=false;
int Arg::GetFocusMode=0;
int Arg::systemtrayPressType=0;
int Arg::weekBeginDay=0;
int Arg::WeekDayIsHoliday[7]={0,0,0,0,0,1,1};//从0开始
int Arg::showNowTime=true;
int Arg::todayLight=true;
int Arg::showWeekLabel=true;
int Arg::mCurrentWeekday=0;
int Arg::mCurrentWeekIndex=0;
bool Arg::mIsLocked = false;
QString Arg::label_text[7]={QObject::tr("星期一"),QObject::tr("星期二"),QObject::tr("星期三"),
                            QObject::tr("星期四"),QObject::tr("星期五"),QObject::tr("星期六"),QObject::tr("星期日")};

ViewMode Arg::viewMode = ViewMode::Standard;
FileListWidget* Arg::fileListWidget = nullptr;
Lesson* Arg::mCurrentLesson = nullptr;
Term* Arg::term = nullptr;
Semester* Arg::mCurrentSemester = nullptr;
QVector <Semester *> * Arg::mSemesters = nullptr;
Schedule* Arg::mCurrentSchedule = nullptr;
QVector <Schedule *> * Arg::mSchedules = nullptr;

QJsonObject Arg::settings;

//http://101.200.176.87:8080/Desktop/urlLogin?username=admina&password=admin


//QString Arg::tokenStr = "&username=admin&token=111111";
QString Arg::tokenStr = "";
QString Arg::tokenStr2 = "?username=admin&token=111111";
QString Arg::token = "";

//QString Arg::resUrl/* = "ftp://101.200.176.87:2121/"*/;


QString Arg::IP = "101.200.176.87";

//QString Arg::sNetRequestHostUrl = "http://101.200.176.87:8080/Desktop/";

//QString Arg::username = "admin";
QString Arg::username = "";
QString Arg::sNickName = "";
int Arg::lessonId = 1;
int Arg::userId = 2;

QMap<QString, int> Arg::subjectMap;
QMap<QPair<int, QString>, QString> Arg::noteMap;
QMap<QPair<int, QString>, QMap<QString, int> > Arg::fileIdMap;

GetDataNW* Arg::nw = new GetDataNW();
//CCU* Arg::ccu = nullptr;


static auto testModel = [] () -> Teacher*
{
        try{
        QFile file ("in.json");
        file.open (QIODevice::ReadOnly);

        QByteArray arr = file.readAll ();
        qDebug()<<"load data: "<<arr<<endl;
        QJsonObject jo = QJsonDocument::fromBinaryData (arr).object ();
        qDebug() << "jo: " << jo["terms"].toArray()<<endl;

        /*
        QFile semeF ("samples/semesters.json");
        semeF.open (QIODevice::ReadOnly);
        QByteArray semeBA = semeF.readAll ();
        jo.insert("semesters", QJsonValue(QJsonDocument::fromJson(semeBA).array()));
        qDebug()<<"semesters------------->"<<semeBA<<endl;

        QFile scheF ("samples/schedule.json");
        scheF.open (QIODevice::ReadOnly);
        QByteArray scheBA = scheF.readAll ();
        qDebug()<<"scheBA: "<<scheBA<<endl;
        //QJsonObject scheJO = QJsonDocument::fromJson(scheBA).object();
        jo.insert("schedules", QJsonValue(QJsonDocument::fromJson(scheBA).array()));
        qDebug()<<"schedule-------------->"<<scheBA<<endl;
        */




        qDebug()<< "------------Current Json-----------" << endl;
        qDebug()<<jo<<endl;
        qDebug()<<"------------------------------------" << endl;

        Arg::settings = jo;
        Arg::setCurrentWeekday (QDate::currentDate().dayOfWeek() - 1);
        qDebug()<< "current day number: " << QDate::currentDate().dayOfWeek() - 1;

        ///Mark 2017.02.17，这里修改掉.
        auto t = new Teacher (jo, nullptr);

        /*
        Arg::setCurrentSemester(Arg::findCurrentSemester(t->semesters()));
        Arg::setSemesters(&(t->semesters()));

        Arg::setCurrentSchedule(Arg::findCurrentSchedule(t->schedules()));
        Arg::setSchedules(&(t->schedules()));
        */

        //load data from network



        return t;
        }
        catch(std::exception& e)
        {
            qDebug()<<e.what()<<endl;
        }

};

void Arg::loadConfigDir()
{
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "savefilepath";
    QString sPath = configSettings.value(sKey, QString()).toString();

    if(sPath.isEmpty()) {
        QString path = QStandardPaths::writableLocation (QStandardPaths::ConfigLocation);
        configDir = QDir(path + "/" + Const::applicationName);

        if (!configDir.exists ()) {
            configDir.mkpath (configDir.absolutePath());
        }
    }
    else {
        configDir = QDir(sPath);
    }
}

void Arg::getSaveDir(QDir &dir)
{
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "savefilepath";
    QString sPath = configSettings.value(sKey, QString()).toString();

    if(sPath.isEmpty()) {
        QString path = QStandardPaths::writableLocation (QStandardPaths::ConfigLocation);
        configDir = QDir(path + "/" + Const::applicationName);

        if (!configDir.exists ()) {
            configDir.mkpath (configDir.absolutePath());
        }
    }
    else {
        configDir = QDir(sPath);
    }

    dir = configDir;
}

//static auto standardConfigDir = [] () -> const QDir
//{
//        QString path = QStandardPaths::writableLocation (QStandardPaths::ConfigLocation);
//        auto dir = QDir (path + "/" + Const::applicationName);
//        qDebug() << dir.absolutePath ();
//        if (!dir.exists ())
//        dir.mkpath (dir.absolutePath ());
//        return dir;
//        };
//const QDir Arg::configDir = standardConfigDir ();
Teacher* Arg::user = testModel ();

Arg* Arg::getInstance ()
{
    static bool first = true;
    if (instance == nullptr || first)
    {
        instance = new Arg ();
        first = false;
        return instance;
    }
    else
        return instance;
}

void Arg::setCurrentLesson (Lesson* lesson)
{
//  qDebug()<<"lesson: "<<lesson<<" mCurrentLesson: "<<mCurrentLesson<<endl;
    bool same = lesson == mCurrentLesson;
    mCurrentLesson = lesson;
    if (lesson != nullptr)
    {
        mCurrentLesson = lesson;
        setCurrentWeekday (static_cast<int>(lesson->day ()) - 1);
        if (term == nullptr)
            throw std::logic_error ("Access current lesson while current term is nullptr");
        qDebug()<<"lesson date: "<<lesson->getDate().toString("yyyy-M-d")<<endl;
        qDebug()<<term->getDateWeekNumber(lesson->getDate ())<<endl;
        qDebug()<<"lesson weekNum: "<<lesson->getWeekId()<<endl;
        //--------------BUG HERE NOT FIXED------------------//
        //setCurrentWeekIndex (term->weekNumber (lesson->getDate ()) - 1);
        //--------------------------------------------------//
        qDebug()<<"bug fixed!!!"<<endl;
    }
    qDebug()<<"same: "<<same<<endl;
    if (!same)
    {
        emit instance->currentLessonChanged (lesson);
    }
}

void Arg::setCurrentWeekday (int weekday)
{
    qDebug() << "set current weekday is:" << weekday;
    bool same = weekday == mCurrentWeekday;
    if (weekday >= 0 && weekday < 7)
    {
        mCurrentWeekday = weekday;
    }
    if (!same)
        emit instance->currentWeekdayChanged (weekday);
}

void Arg::setLock (bool lock)
{
    bool same = lock == mIsLocked;
    mIsLocked = lock;
    if (!same)
        emit instance->lockChanged (lock);
}

void Arg::dumpUserData()
{
    QFile file ("in.json");
    file.open (QIODevice::WriteOnly);

    QJsonObject userDate = instance->getUserData();
    QJsonDocument document (userDate);
    file.write (document.toBinaryData ());
    file.close();
}

void Arg::setCurrentWeekIndex (int weeknumber)
{
    qDebug()<<"weeknumber: "<<weeknumber<<" mCurrentWeekIndex: "<<mCurrentWeekIndex<<endl;
    bool same = weeknumber == mCurrentWeekIndex;
    qDebug()<<"term weekNumber: "<<term->getTermWeeksNumber()<<endl;
    if (weeknumber >= 0 && (term == nullptr || weeknumber < term->getTermWeeksNumber()))
    {
        mCurrentWeekIndex = weeknumber;
    }
    if (!same)
    {
        qDebug()<<"currentWeekNumChanged"<<endl;
        emit instance->currentWeekNumberChanged (weeknumber);
    }
}

void Arg::setCurrentTerm (Term* t)
{
    bool same = t == term;
    if (t != nullptr)
    {
        term = t;
    }

    if (!same) {
//        term = t;

//        emit instance->currentTermChanged(term);
    }
}

void Arg::setCurrentSemester(Semester* s)
{
    bool same = s == mCurrentSemester;
    if (s != nullptr)
    {
        mCurrentSemester = s;
    }
    if (!same){
        emit instance->currentSemesterChanged (s);
        qDebug()<<"semester chg to"<<s->getTitle()<<endl;
    }
}

/*
 * Current semester setting strategy
 */
Semester * Arg::findCurrentSemester(QVector<Semester *> &sV)
{
//    for(const Semester *s : sV){
//        s->getStartTime() <= curr
//    }

    return sV[0];
}

void Arg::setSemesters(QVector<Semester *> *sV)
{
    bool same = sV == mSemesters;
    if (sV != nullptr)
    {
        mSemesters = sV;
    }
    if (!same){
        emit instance->semestersChanged (sV);
        qDebug()<<"semesters chged"<<endl;
    }
}


void Arg::setCurrentSchedule(Schedule* s)
{
    bool same = s == mCurrentSchedule;
    if (s != nullptr)
    {
        mCurrentSchedule = s;
    }
    if (!same){
        ///Mark,数据完成再同步.
//        emit instance->currentScheduleChanged (s);
        qDebug()<<"schedule change to"<<s->getName()<<endl;
    }
}

/*
 * Current schedule setting strategy
 */
Schedule * Arg::findCurrentSchedule(QVector<Schedule *> &sV)
{
//    for(const Schedule *s : sV){
//        s->getStartTime() <= curr
//    }

    return sV[0];
}

void Arg::setSchedules(QVector<Schedule *> *sV)
{
    bool same = sV == mSchedules;
    if (sV != nullptr)
    {
        mSchedules = sV;
    }
    if (!same){
        emit instance->schedulesChanged (sV);
        qDebug()<<"schedules chaged"<<endl;
    }
}

QJsonObject Arg::getUserData()
{
    return user->save();
}

void Arg::deleteLessonFinished(int nWeekDay, int nSection, int nLessonDetailID, const QDate &endDate)
{
    if(term != Q_NULLPTR) {
       term->deleteBaseLesson(nWeekDay, nSection, nLessonDetailID, endDate);
    }
}

void Arg::updateLessonFinished(int nPreviousDetailID, Lesson *pLesson)
{
    if(term != Q_NULLPTR) {
        int nWeekDay = pLesson->getWeekId();
        int nSection = pLesson->getSection();
//        term->updateBaseLesson(nWeekDay, nSection, pLesson);
        //删除是特殊处理，删除日期以前的需要保持以前数据，所以需要做以前数据删除处理，加新课程添加处理.
        QDate newStartDate = pLesson->getStartDate();
        QDate modifiedEndDate = newStartDate.addDays(-7);
        //因为更新牵涉了对当前lesson设置为删除标志与添加新的课程操作，所以对已经存在的pLesson拷贝构造.
        Lesson *pNewLesson = new Lesson(*pLesson);
        term->deleteBaseLesson(nWeekDay, nSection, nPreviousDetailID, modifiedEndDate);
        term->addBaseLesson(nWeekDay, nSection, pNewLesson);
    }
}

void Arg::addLessonFinished(Lesson *pLesson)
{
    if(term != Q_NULLPTR) {
        int nWeekDay = pLesson->getWeekId();
        int nSection = pLesson->getSection();
        term->addBaseLesson(nWeekDay, nSection, pLesson);

        qDebug() << "the add lesson weekday and section is:" << nWeekDay << nSection;
    }
}

/*
void Arg::dumpUserData(){
    QFile file ("in.json");
    file.open (QIODevice::WriteOnly);
    QTextStream in(&file);
    QJsonObject userDate = user->save();
    QJsonDocument document;
    document.setObject(userDate);
    QByteArray byte_array = document.toJson(QJsonDocument::Compact);
    QString json_str(byte_array);

    in<<json_str;
    file.close();
}
*/

void Arg::setServerIpAndAddress(const QString &sHostAddress, int port)
{
    sNetHostUrl = QString("http://%1:%2/Desktop/").arg(sHostAddress).arg(port);
    qDebug() << "the server base url is:" << sNetHostUrl;

     QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
     configSettings.setValue("ServerIP", sHostAddress);
     configSettings.setValue("ServerPort", port);

     IP = sHostAddress;
}

bool Arg::getNetReusetHostUrl(QString &sNetRequestUrl)
{
    if(sNetHostUrl.isEmpty()) {
        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        IP =  configSettings.value("ServerIP", QString()).toString();

        if(IP.isEmpty()) {
#ifdef USER_QT_MESSAGEBOX
            QMessageBox::warning(0, tr("教师客户端"), tr("请配置服务器"));
#else
           MessageDisplayWidget::showMessage(tr("教师客户端"), tr("请配置服务器")) ;
#endif

            return false;
        }

        int port = configSettings.value("ServerPort", 8080).toInt();
        sNetHostUrl = QString("http://%1:%2/Desktop/").arg(IP).arg(port);
    }

    sNetRequestUrl = sNetHostUrl;

    return true;
}
