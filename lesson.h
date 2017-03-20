#ifndef LESSON_H
#define LESSON_H
//#include "config.h"
#include "folder.h"
#include <QObject>
#include <QColor>
#include <QDate>
//#include "lessonremarkdb.h"
//#include "filenumwidget.h"

class FileNumWidget;
class QJsonObject;

///Mark 2017.02.22.重构时候，Lesson有基准课程，每周课程，考虑用原型模式.
class Lesson : public QObject
{
    Q_OBJECT
public:
    //Coresponding signals is emitted
    explicit Lesson(int section, const QString& subject, const QString& location, const QString& unit, const QString& note, Folder* source, QColor color, const QDate& date, QObject *parent = 0);
    explicit Lesson (const QJsonObject& json, QObject* parent = nullptr);
    Lesson()
    {
        mRootFolder = new Folder("课程课件", this);
    }

    Lesson(const Lesson &otherLesson)/* = delete*/;

    Lesson& operator = (const Lesson&) = delete;
    ~Lesson ();

    /*!
     * \brief setStartTime, 上课开始时间.
     * \param sStartTime.
     */
    void setStartTime(const QString &sStartTime);

    void getStartTime(QString &sStartTime);

    /*!
     * \brief setEndTime,下课时间.
     * \param sEndTime
     */
    void setEndTime(const QString &sEndTime);

    void getEndTime(QString &sEndTime);

    /*!
     * \brief getResourceCount,返回课程资源数量.
     * \return
     */
    int getResourceCount() const;

    QMap<QString, File*> name2File;
    FileNumWidget* lesFileNumWidget;

    //Getters and Setters. Coresponding signals is emitted
    WeekDay day () const {return static_cast<WeekDay>(mDate.dayOfWeek ());}
    int section () const {return mSectionNumber;}
    void setSection (int section) {mSectionNumber = section; emit sectionNumberChanged (section);}
    QString subject () const {return mSubject;}
    void setSubject (const QString& subject) {mSubject = subject; emit subjectChanged (subject);}
    QString location () const {return mLocation;}
    void setLocation (const QString& location) {mLocation = location; emit locationChanged (location);}
    QString unit () const {return mUnit;}
    void setUnit (const QString& unit) {mUnit = unit; emit unitChanged (unit);}
    QString note () const {return mNote;}

    bool noteLoaded() const;

    void setNote(const QString& note);

    Folder* rootFolder ();

    void setRootFolder(Folder* f);

    QColor getColor() const;

    void setColor(const QColor& color);

    QDate getDate() const;

    /*!
     * \brief setDate,设置该节课上课日期,ex.2017.2.1.
     * \param date
     */
    void setDate(const QDate& date);

    int getLessonDetailId() const;

    void setLessonDetailId(int lessonDetailId);

    void setWeekId(int weekId);

    int getWeekId() const;

    int getSection() const;

    void setLessonId(int lessonid);

    int getLessonId() const;

    void setClassId(int classid);

    int getClassId() const;

    void setRoomId(int roomid);

    int getRoomId() const;

    void setStartDate(const QDate &startdate);

    QDate getStartDate() const;

    void setEndDate(const QDate &enddate);

    QDate getEndDate() const;

    void setRepeat(int repeat);

    int getRepeat() const;

    void clear();


    //Coresponding signals is emitted
    void load (const QJsonObject& json);
    //Save
    QJsonObject save () const;

    //Emit all signals
    void notify();

signals:
    void sectionNumberChanged (int sn);
    void subjectChanged (QString subject);
    void locationChanged (QString location);
    void unitChanged (QString unit);
    void noteChanged (QString note);
    void colorChanged (QColor color);
    void dateChanged (const QDate& date);
public slots:

private:
    int lessonid; //主课程表id
    int classid;
    int roomid;

    int lessonDetailId; //used for network
    int mSectionNumber;
    int weekId = -1;
    QString mSubject;
    QString mLocation;
    QString mUnit;
    QString mNote;
    Folder* mRootFolder = nullptr;
    QColor mColor ;//the color of this lesson's cell
    QDate mDate = QDate::currentDate ();

    QString m_sStartTime; ///< \brief 上课时间.

    QString m_sEndTime; ///< \brief 下课时间.

    /*!
     * \brief startDate，课程开始日期.
     */
    QDate startDate;

    /*!
     * \brief endDate,课程结束日期.
     */
    QDate endDate;

    /*!
     * \brief m_bNoteLoaded, 课程备注是否已经加载.
     */

    /*!
     * \brief m_nRepeat,课节重复方式.
     */
    int m_nRepeat;

    bool m_bNoteLoaded;
};

#endif // LESSON_H
