#ifndef TERM_H
#define TERM_H
#include <QObject>
#include <QVector>
#include <QList>
#include <QDate>
#include <QPair>
#include <QDebug>

class Lesson;
class QJsonObject;
class Term : public QObject
{
    Q_OBJECT
public:
    using Section = QPair<int, int>;

    using Break = QPair<int, QString>;

    explicit Term(int nSemsID, const QDate &startDate, const QDate &endDate, QObject *parent);

    Term(QDate start, QDate end, const QMap<int, QMap<int, Section> > &section, QObject *parent = 0);

    explicit Term (const QJsonObject& json, QObject* parent = 0);

    Term (const Term&) = delete;

    Term& operator= (const Term&) = delete;

    ~Term ();

    int getTermID() const;

    /// @brief 检查是否存在课程冲突.
    bool checkConflictLessons(const QDate &startDate, const QDate &endDate, int nWeekDay, int nSection,
                              int nLoop, QMap<QString, QList<int> > &conflictMap);

    /*!
     * \brief getTermWeeksNumber,获取学期所含的周数.
     * \return int, 为0学期无效，大于0，学期周数.
     */
    int getTermWeeksNumber() const;

    /*!
     * \brief getDateWeekNumber,获取提供日期的在学期中的周数.
     * \param 获取周数的日期.
     * \return int, 如果返回为0，则周数不在学期中， 大于0，在学期中的周数.
     */
    int getDateWeekNumber(const QDate &date) const;

    void clear ();

    void getTermBaseLessons(QMap<int, QMultiMap<int, Lesson *> > &baseLessons);

    /*!
     * \brief setTermBaseLessons, 设置学期基准课程.QHash的key值为周几, QMap里的key值为section.
     * \param baseLessons
     */
    void setTermBaseLessons(const QMap<int, QMultiMap<int, Lesson *> > &baseLessons);

    /*!
     * \brief addBaseLesson,添加一节基础数据.
     * \param nWeekDay,周几.
     * \param nSection, section.
     * \param pLesson, 课程.
     * \note 新作软件设计的时候，上面直接针对外面数据结构操作的两个接口setTermBaseLessons与getTermBaseLessons去掉.
     *       直接换为这种接口，现在为了保持原来设计直接设计上界面接口.
     */
    void addBaseLesson(int nWeekDay, int nSection, Lesson *pLesson);

    /*!
     * \brief updateBaseLesson，更新已存在的基准课程.
     * \param nWeekDay， 课程所在的日期天数.
     * \param nSection, 课程的section.
     * \param pLesson
     */
    void updateBaseLesson(int nWeekDay, int nSection, Lesson *pLesson);

    /*!
     * \brief deleteBaseLesson, 删除周天数是nWeekDay[1..7], 课程section为nSection[1...)的课程.
     * \param nWeekDay
     * \param nSection
     * \param delDate,删除日期.
     */
    void deleteBaseLesson(int nWeekDay, int nSection, int nLessonDetailID, const QDate &endDate);

    QList<Lesson*>& lessonWeek (int weekNumber);

#if USE_DEFAULT_TEST_DATA ///这里的数据是写死的，所以暂时注释掉.
    void load (const QJsonObject& json);
#endif

    QJsonObject save () const;

    QDate getStart () const;

    QDate getEnd () const;

    QDate getDate (int weekindex, int day);

//    int getSectionNumber () const;
    /*!
     * \brief getWeekLessonsByDate,通过日期获取一周的课程,vector里0到6代表周一到周日，QMap的key代表.
     * \param weekLessons,引用获得的一周课程.
     * \param weekDate,日期.
     * \return 返回false，获取不成功.
     */
    bool getWeekLessonsByDate(QVector<QMap<int, Lesson*>> &weekLessons, const QDate &weekDate);

    void getSections(QMap<int, QMap<int, QPair<int, int> > > &sectionMap) const;

    QVector<QPair<int, QString> > getBreaks () const;

    void setStart(const QDate& start);

    void setEnd(const QDate& end);

    ///Mark 2017.02.17，这里暂时跟以前不考虑服务器break,要考虑section类型来.
    /// section 0,早读，1上午， 2下午， 3 晚自习.
    void setSections (const QMap<int, QMap<int, QPair<int, int> > >& section);

    void setBreaks(const QVector<Break>& breaks);

    void notify ();

    //for network
    QVector<QList<Lesson*> > getLessons();

    void setLessons(const QVector<QList<Lesson*> > &mLessons);

    int getSectionTypeNumber() const;

    /*!
     * \brief getLessonDate,通过给定的时间与课程在一周中的天数index，获取上课日期.
     * \param weekDayID
     * \param inClassDate
     * \param date
     */
    void getLessonDate(int weekDayID, const QDate &inClassDate, QDate &date);

signals:
    void startChanged (QDate start);
    void endChanged (QDate end);
    void sectionChanged (const QMap<int, QMap<int, Section> > &);
    void lessonChanged ();
    void changed ();

private:
    /// @brief 获取一节课在一个学期中所有上课周数.
    QList<int> getLessonOnClassWeeks(const QDate &startDate, const QDate &endDate, int loop);

public slots:
private:
    int m_nSemsID; //学期数据库对应ID.
    QDate mStart;
    QDate mEnd;

    QMap<int, QMap<int, QPair<int, int> > > mSectionsKeyValeMap;//section类型与时间段键值对,int为section类型,利用map是为了顺序性.
                                                      //int 0，早读，1上午课程， 2，下午课程，3，晚自习.
                                                      //QMap<int, Section>section的属性Map,int 为sectionID.
                                                      //属于第几节，Section为起始终止时间.


    /*!
      @ brief 学期基础课程二维数组.第一个维度代表一周的日期坐标，第二个Lesson section坐标,用QMap是为了保持顺序, QMultiMap,是同一个时间段可以有几种情况，单双周上课等.
    */
    QMap<int, QMultiMap<int, Lesson*> > mBaseLessonsMap;

    QVector<QList<Lesson*> > mLessons;
    QVector<Break> mBreaks;
};

#endif // TERM_H
