#ifndef CONST_H
#define CONST_H
#include <QDate>
#include <QDir>
#include <QDebug>
#include "filelistwidget.h"
#include "getdatanw.h"
//#include "ccu.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>
#include <QTextCodec>

#define TEST
//全局常量
enum class ViewMode:quint8 {Standard, InClass, Day, Edit};
enum class WeekDay:int {Monday = 1, Tuesday = 2, Wednesday = 3, Thursday = 4, Friday = 5, Saturday = 6, Sunday = 7};
namespace Const
{
    const int MyButtonSize=20;
    const QDate MinimumDate(1900,1,1);
    const QDate MaximumDate(2099,2,31);
    const bool deleteEverytime = false;
    const QString defaultSavePath("/data.mdd");//程序运行文件夹内
    const QString applicationName("XDcalendar");
    namespace SetupDialog {
        constexpr int avatarSize = 81;
        constexpr int avatarInfoHeight = 20;
    }
    namespace Searchbox {
        constexpr int kernelHeight = 25;
    }
    namespace MainScreen {
        constexpr int breakHeight = 15;
    }
}
class Teacher;
class Lesson;
class Term;
class Semester;
class Schedule;

class Arg: public QObject
{
    Q_OBJECT
public:
    static int sUpLoadFileCount; //正在上传的文件数目.
    static int sDownLoadFileCount; //正在下载的文件数目.

    static int isMousePenetration;//是否鼠标穿透 0 1
    static int lockDrag;//拖拽文件锁定 0 1
    static int showChineseCalendar;//显示农历
    static int mergeLoad;//是否在读取文件时合并事件
    static int GetFocusMode;//窗口获取焦点的方式(0:点击;1:移入)
    static int systemtrayPressType;//右下角托盘打开的方式(0:鼠标右键;1:鼠标左键)
    static int weekBeginDay;
    static int WeekDayIsHoliday[7];//从0开始
    static int showNowTime;//显示月份
    static int todayLight;//今日高亮
    static int showWeekLabel;//显示星期几
    static ViewMode viewMode;
    static Teacher* user;
    /*static const*/ QDir configDir;
    static FileListWidget* fileListWidget;
    static QJsonObject settings;
    static QString label_text[7];

    static QString tokenStr;
    static QString tokenStr2;
    static QString token;
    static QString ftpStr;
    static QString IP;
    static int lessonId; //课程主表ID
    static QString username;
    static QString sNickName; ///昵称,暂时这样.
    static int userId;
    static GetDataNW* nw;
    static QMap<QString, int> subjectMap;
    //QPair里第一个参数为detailID 第二个参数为上课日期，通过这两个值唯一锁定ui的一个课程
    static QMap<QPair<int, QString>, QString> noteMap;
    static QMap< QPair<int, QString>, QMap<QString, int> > fileIdMap; //用于存储某节课的文件ID和文件名映射
    static bool mIsLocked;
    //static CCU* ccu;
    //static loginDialog* login;

public:
    static Arg* getInstance ();


    /*!
     * \brief setServerIpAndAddress,设置服务器ip地址与端口号.exam.ip"192.168.125.107"
     * \param sHostAddress
     * \param port
     */
    void setServerIpAndAddress(const QString &sHostAddress, int port);

    /*!
     * \brief getNetReusetHostUrl,获取服务器设置格式化后的请求url.
     * \param sNetRequestUrl
     * \return true,获取url成功，false，失败.
     */
    bool getNetReusetHostUrl(QString &sNetRequestUrl);

    static Lesson* currentLesson () {return mCurrentLesson;}
    static void setCurrentLesson (Lesson* lesson);
    static int currentWeekday () {return mCurrentWeekday;}
    static void setCurrentWeekday (int);
    static int currentWeekIndex () {return mCurrentWeekIndex;}
    static void setCurrentWeekIndex (int);
    static Term* currentTerm () {return term;}
    static void setCurrentTerm (Term*);
    static Semester* currentSemester () {return mCurrentSemester;}
    static void setCurrentSemester(Semester*);
    static Semester * findCurrentSemester(QVector<Semester *> &sV);
    static QVector<Semester*> * semesters() {return mSemesters;}

    static void setSemesters(QVector<Semester *> *sV);
    static Schedule* currentSchedule () {return mCurrentSchedule;}
    static void setCurrentSchedule(Schedule*);
    static Schedule * findCurrentSchedule(QVector<Schedule *> &sV);
    static QVector<Schedule*> * schedules() {return mSchedules;}
    static void setSchedules(QVector<Schedule *> *sV);

    static bool isLocked() {return mIsLocked;}
    static void setLock(bool lock);

    static void dumpUserData();

    static QJsonObject getUserData();

public:

    /*!
     * \brief deleteLessonFinished,删除课程完成处理槽函数，暂时放到这里.
     */
    void deleteLessonFinished(int nWeekDay, int nSection, int nLessonDetailID, const QDate &endDate);

    /*!
     * \brief updateLessonFinished
     */
    void updateLessonFinished(Lesson *pLesson);

    /*!
     * \brief addLessonFinished
     */
    void addLessonFinished(Lesson *pLesson);

    void loadConfigDir();

    void getSaveDir(QDir &dir);

signals:
    void currentLessonChanged (Lesson*);
    void currentWeekdayChanged (int);
    void currentWeekNumberChanged (int);
    void currentTermChanged (Term*);
    void currentSemesterChanged (Semester *);
    void semestersChanged(const QVector<Semester *> *);
    void currentScheduleChanged (Schedule *);
    void schedulesChanged(const QVector<Schedule *> *);
    void lockChanged (bool isLocked);

    ///<@brief 学期加载完成信号.
    void termLoadFinished();

private:
    static int mCurrentWeekday;
    static int mCurrentWeekIndex;
    static Lesson* mCurrentLesson;
    static Term* term;
    static Arg* instance;
    static Semester* mCurrentSemester;
    static QVector<Semester *> * mSemesters;
    static Schedule* mCurrentSchedule;
    static QVector<Schedule *> *mSchedules;

    QString sNetHostUrl; //暂时放在这里.

    Arg () {}
    virtual ~Arg () = default;
};

enum class SaveFormat:char {Json, BinaryQtJson};
#endif // CONST_H
