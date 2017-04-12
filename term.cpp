#include "term.h"
#include "lesson.h"
#include <stdexcept>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <algorithm>
//#include <QDebug>
Term::Term(int nSemsID, const QDate &startDate, const QDate &endDate, QObject *parent):
    QObject (parent),
    m_nSemsID(nSemsID),
    mStart(startDate),
    mEnd(endDate)
{

}

Term::Term (QDate start, QDate end, const QMap<int, QMap<int, Section> >& section, QObject* parent):
    QObject (parent),
    mStart (start),
    mEnd (end),
    mSectionsKeyValeMap(section)
{
    if (start > end)
    {
        throw std::invalid_argument ("Start date is later than end date in a term");
    }

    ///MarkTODO 2017.02.20,这个要存储所有周数的Lessons的设置随着这次改造完成要去掉.
    mLessons.resize (getTermWeeksNumber());
    notify ();
}

Term::Term (const QJsonObject& json, QObject* parent): QObject (parent)
{
#if USE_DEFAULT_TEST_DATA
    load (json);
#endif
}

int Term::getTermID() const
{
    return m_nSemsID;
}

bool Term::checkConflictLessons(const QDate &startDate, const QDate &endDate, int nWeekDay, int nSection, int nLoop,
                                QMap<QString, QList<int>> &conflictMap)
{
    conflictMap.clear();

    //待判断课程上课周数.
    QList<int> checkLesWeekList = getLessonOnClassWeeks(startDate, endDate, nLoop);

    QMultiMap<int, Lesson*> weekLessonMap = mBaseLessonsMap.value(nWeekDay);
    QList<Lesson*> secLessonList = weekLessonMap.values(nSection);


    for(int index = 0; index < secLessonList.size(); ++index) {
        Lesson *pLesson = secLessonList.at(index);
        QDate lesStartDate = pLesson->getStartDate();
        QDate lesEndDate = pLesson->getEndDate();
        int nLesLoop = pLesson->getRepeat();

        QList<int> lesWeekList = getLessonOnClassWeeks(lesStartDate, lesEndDate, nLesLoop);
        QList<int> conflictList;

        for(auto itor = lesWeekList.begin(); itor != lesWeekList.end(); ++itor) {
            if(checkLesWeekList.contains(*itor)) {
                conflictList.append(*itor);
            }
        }

        if(!conflictList.isEmpty()) {
            conflictMap.insert(pLesson->subject(), conflictList);
        }
    }

    if(!conflictMap.isEmpty()) {
        return true;
    }

    return false;
}

QList<int> Term::getLessonOnClassWeeks(const QDate &startDate, const QDate &endDate, int loop)
{
    //获取weekDate在学期中的周数.
    int nStartWeekNum = getDateWeekNumber(startDate);
    int nEndWeekNum = getDateWeekNumber(endDate);

    QList<int> lessonWeekList;
    int week = nStartWeekNum;
    int nLoopCtrl = 0;
    while(week <= nEndWeekNum) {
        week += loop * nLoopCtrl;

        if(week <= nEndWeekNum) {
            lessonWeekList.append(week);
        }

       ++nLoopCtrl;
    }

    return lessonWeekList;
}

Term::~Term () = default;

int Term::getTermWeeksNumber() const
{
    int weekNum = mEnd.weekNumber () - mStart.weekNumber () + 1;
    if(weekNum < 0) //跨年
    {
        weekNum = 52 - mStart.weekNumber() + 1 + mEnd.weekNumber();
        if(weekNum < 0)
        {
            qDebug()<<"跨年周数计算错误"<<endl;
            return 0;
        }
    }

    return weekNum;
}

int Term::getDateWeekNumber(const QDate& date) const
{
    //return date.weekNumber () - mStart.weekNumber () + 1;
    /*先判断日期周数是否在学期中*/
    bool bBigger = date > mEnd;
    bool bSmaller = date < mStart;

    if((bBigger) || (bSmaller)) {
        qDebug() << tr("日期不在学期中");
        return 0;
    }

    int weekNum = date.weekNumber () - mStart.weekNumber () + 1;
    if(weekNum < 0) //跨年
    {
        weekNum = 52 - mStart.weekNumber() + 1 + date.weekNumber();
        if(weekNum < 0)
        {
            qDebug()<<"跨年周数计算错误"<<endl;
            return 0;
        }
    }

    return weekNum;

}

QList<Lesson*>& Term::lessonWeek (int weekNumber)
{
    qDebug() << "the weekNumber and mLesson size is:" << weekNumber << mLessons.size();

//    if (weekNumber < 0 || weekNumber >= mLessons.size ())
//        throw std::invalid_argument ("Invalid week number when query lessons.");
    Q_ASSERT_X((weekNumber >= 0 && weekNumber < mLessons.size ()), "Term::lessonWeek", "weekNumer is error");

    return mLessons[weekNumber];
}
void Term::clear ()
{
    mStart = mEnd = QDate::currentDate ();
    for (auto& list: mLessons)
        for (auto& p : list)
            delete p;
    mLessons.clear ();
    mSectionsKeyValeMap.clear ();
}

void Term::getTermBaseLessons(QMap<int, QMultiMap<int, Lesson *> > &baseLessons)
{
    baseLessons = mBaseLessonsMap;
}

void Term::setTermBaseLessons(const QMap<int, QMultiMap<int, Lesson *> > &baseLessons)
{
    mBaseLessonsMap = baseLessons;
}

void Term::addBaseLesson(int nWeekDay, int nSection, Lesson *pLesson)
{
    QMultiMap<int, Lesson*> secLessonMap;

    if(mBaseLessonsMap.contains(nWeekDay)) {
        secLessonMap = mBaseLessonsMap.value(nWeekDay);
        secLessonMap.insertMulti(nSection, pLesson);
    }
    else {
        secLessonMap.insertMulti(nSection, pLesson);
//        mBaseLessonsMap.insert(nWeekDay, secLessonMap);
    }

    mBaseLessonsMap.remove(nWeekDay);
    mBaseLessonsMap.insert(nWeekDay, secLessonMap);
}

void Term::updateBaseLesson(int nWeekDay, int nSection, Lesson *pLesson)
{
    QMultiMap<int, Lesson*> secLessonMap = mBaseLessonsMap.value(nWeekDay);

    if(!secLessonMap.isEmpty()) {
        int nDetailID = pLesson->getLessonDetailId();
        QList<Lesson*> baseLessonList = secLessonMap.values(nSection);

        for(int index = 0; index < baseLessonList.size(); ++index) {
            Lesson *pBaseLesson = baseLessonList.at(index);

            if(nDetailID == pBaseLesson->getLessonDetailId()) {
                pBaseLesson->setRepeat(pLesson->getRepeat());
                pBaseLesson->setSubject(pLesson->subject());
                pBaseLesson->setLocation(pLesson->location());
                pBaseLesson->setUnit(pLesson->unit());
                pBaseLesson->setStartDate(pLesson->getStartDate());
                pBaseLesson->setEndDate(pLesson->getEndDate());

                break;
            }
        }
    }

    mBaseLessonsMap.remove(nWeekDay);
    mBaseLessonsMap.insert(nWeekDay, secLessonMap);
}

void Term::deleteBaseLesson(int nWeekDay, int nSection, int nLessonDetailID, const QDate &endDate)
{
    QMultiMap<int, Lesson*> secLessonMap = mBaseLessonsMap.value(nWeekDay);

    if(!secLessonMap.isEmpty()) {
        QList<Lesson*> baseLessonList = secLessonMap.values(nSection);
//        QDate delDate = QDate::currentDate();
        //结束日期至少要比删除日期少一天，才会不再在删除当天显示课程.

        for(int index = 0; index < baseLessonList.size(); ++index) {
            Lesson *pBaseLesson = baseLessonList.at(index);

            if(nLessonDetailID == pBaseLesson->getLessonDetailId()) {
                pBaseLesson->setEndDate(endDate);

                break;
            }
        }
    }

    mBaseLessonsMap.remove(nWeekDay);
    mBaseLessonsMap.insert(nWeekDay, secLessonMap);
}

static QString LoadErrMsgHdr = "Unable to load a Term from this QJsonObject: ";

#if USE_DEFAULT_TEST_DATA
void Term::load (const QJsonObject& json)
{
    QJsonValue startV = json["start"];
    if (startV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "start is not String").toStdString ());
    QJsonValue endV = json["end"];
    if (endV.type () != QJsonValue::String)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "end is not String").toStdString ());
    QJsonValue scV = json["section"];
    if (scV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "section is not Array").toStdString ());
    QJsonArray scA = scV.toArray ();
    for (auto list: scA)
    {
        if (list.type () != QJsonValue::Array)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "section is not all Array").toStdString ());
        QJsonArray la = list.toArray ();
        if (la.size () != 2 || std::end (la) != std::find_if (std::begin (la), std::end (la), [] (const QJsonValue& v) {return v.type () != QJsonValue::Double;}))
           throw std::invalid_argument (QString(LoadErrMsgHdr + "section is not all Array of 2 Double's Array").toStdString ());
    }
    QJsonValue lessonsV = json["lessons"];
    if (lessonsV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString(LoadErrMsgHdr + "lessons is not Array").toStdString ());
    QJsonArray lessonsA = lessonsV.toArray ();
    for (auto list: lessonsA)
    {
        if (list.type () != QJsonValue::Array)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "lessons is not all Array").toStdString ());
        QJsonArray la = list.toArray ();
        if (std::end (la) != std::find_if (std::begin (la), std::end (la), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
           throw std::invalid_argument (QString(LoadErrMsgHdr + "lessons is not all Array of Objects").toStdString ());
    }
    QJsonValue breakV = json["breaks"];
    if (breakV.type () != QJsonValue::Array)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "breaks is not a Array").toStdString ());
    QJsonArray breakA = breakV.toArray ();
    for (auto b: breakA)
    {
        if (b.type () != QJsonValue::Array)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "breaks is not all Array").toStdString ());
        QJsonArray ba = b.toArray ();
        if (ba.size () != 2)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "breaks is not all Array of 2").toStdString ());
        if (ba[0].type () != QJsonValue::Double || ba[1].type () != QJsonValue::String)
           throw std::invalid_argument (QString(LoadErrMsgHdr + "breaks is not all Array of {int, string}").toStdString ());
    }
    mBreaks.resize (1);
    mBreaks[0].first = 5;
    mBreaks[0].second = "午休";
//    mBreaks[1].first = 7;
//    mBreaks[1].second = "活动";
//    std::sort (std::begin (mBreaks), std::end (mBreaks), [] (const Break& a, const Break& b) {return a.first >= b.first;});
    clear ();
    mStart = QDate::fromString (startV.toString (), Qt::ISODate);
    mEnd = QDate::fromString (endV.toString (), Qt::ISODate);
    for (auto pairV: scA)
    {
        auto pair = pairV.toArray ();
        mSections.push_back ({static_cast<int>(pair[0].toDouble ()), static_cast<int>(pair[1].toDouble ())});
    }
    for (auto week: lessonsA)
    {
        QList<Lesson*> list;
        for (auto l: week.toArray ())
            list.push_back (new Lesson (l.toObject (), this));
        mLessons.push_back (list);
    }

    /*
    for (auto b: breakA)
    {
        auto ba = b.toArray ();
        mBreaks.push_back (Break{ba[0].toDouble (), ba[1].toString ()});
    }
    */
    notify ();
}
#endif

QJsonObject Term::save () const
{
#if USE_DEFAULT_TEST_DATA
    QJsonArray lessonArrayArray;
    for (const auto& list: mLessons)
    {
        QJsonArray lessonArray;
        for (const auto& ptr: list)
            lessonArray.push_back (ptr == nullptr? QJsonValue (): ptr->save ());
        lessonArrayArray.push_back (lessonArray);
    }
    QJsonArray sectionArray;
    for (const auto& pair: mSections)
    {
        sectionArray.push_back (QJsonArray {pair.first, pair.second});
    }
    QJsonArray breakArray;
    for (const auto& b: mBreaks)
    {
        breakArray.push_back (QJsonArray{b.first, b.second});
    }
    return QJsonObject {{"start", mStart.toString (Qt::ISODate)}, {"end", mEnd.toString (Qt::ISODate)}, {"section", sectionArray}, {"lessons", lessonArrayArray}, {"breaks", breakArray}};
#endif

    return QJsonObject();
}


QDate Term::getStart() const
{
    return mStart;
}

QDate Term::getEnd() const
{
    return mEnd;
}

QDate Term::getDate (int weekindex, int day)
{
    QDateTime ret (this->mStart);
    return ret.addDays (weekindex * 7) \
            .addDays (day - this->mStart.dayOfWeek ()) \
            .date ();
}

void Term::getLessonDate(int weekDayID, const QDate &inClassDate, QDate &date)
{
    //获取weekDate在学期中的周数.
    int nTermWeekNum = getDateWeekNumber(inClassDate);
    if(nTermWeekNum == 0) { //日期不在学期中.
        return;
    }

    //请求日期在一周中的周天数标志.
    int nWeekDateNum = inClassDate.dayOfWeek(); //[1..7];
    int nAdjustNum = nWeekDateNum - 1;
    //请求课程的周的第一天日期.
    QDate weekStartDate = inClassDate.addDays(-nAdjustNum);
      nAdjustNum = weekDayID - 1;
    date = weekStartDate.addDays(nAdjustNum);
}

bool Term::getWeekLessonsByDate(QVector<QMap<int, Lesson*>> &weekLessons, const QDate &weekDate)
{
    //获取weekDate在学期中的周数.
    int nTermWeekNum = getDateWeekNumber(weekDate);
    if(nTermWeekNum == 0) { //日期不在学期中.
        return false;
    }

    //请求日期在一周中的周天数标志.
    int nWeekDateNum = weekDate.dayOfWeek(); //[1..7];
    int nAdjustNum = nWeekDateNum - 1;
    //请求课程的周的第一天日期.
    QDate weekStartDate = weekDate.addDays(-nAdjustNum);

//    int lessonStartWeekNum = getDateWeekNumber(mStart);
    int nStartWeekNum = getDateWeekNumber(mStart);
    //将数组一维尺寸置为一周天数.
//    weekLessons.resize(7);
    QList<int> weekDayKeyList = mBaseLessonsMap.keys();

    for(int nKeyPos = 0; nKeyPos < weekDayKeyList.size(); ++nKeyPos) {
        //周数服务器返回是1到7，所以存储的位置要减一处理.
        int nWeekDay = weekDayKeyList.at(nKeyPos);

        //周日期不对，排除.
        if((nWeekDay < 0 ) || (nWeekDay > 7)) {
            continue;
        }

        QMultiMap<int, Lesson*> baseWeekDayLessonsMap = mBaseLessonsMap.value(nWeekDay);
//        QMultiMap<int, Lesson*>::const_iterator cItor = baseWeekDayLessonsMap.constBegin();
        QMap<int, Lesson*> secLessonMap;
        QList<int> secKeys = baseWeekDayLessonsMap.keys();

        //一天的课程对应.
        for(int nPos = 0; nPos < secKeys.size(); ++nPos) {
            int nSec = secKeys.at(nPos);
            QList<Lesson*> baseLessonList = baseWeekDayLessonsMap.values(nSec);

            for(int index = 0; index < baseLessonList.size(); ++index) {
                Lesson *pBaseLesson = baseLessonList.at(index);

                //判断要用是不是在一周来判断.

                //如果课程结束周数比提供的时间早，那么该课程不在本周显示.
                QDate endDate = pBaseLesson->getEndDate();
                int nEndWeekNum = getDateWeekNumber(endDate);
                if(nEndWeekNum < nTermWeekNum) {
                    qDebug() << "the end week and request week is:" << nEndWeekNum << nTermWeekNum;
                    continue;
                }

                //如果课程添加周数比提供的晚，则课程不在本周显示.
                QDate startDate = pBaseLesson->getStartDate();
                nStartWeekNum = getDateWeekNumber(startDate);

                if(nStartWeekNum > nTermWeekNum) {
                    qDebug() << "the start week and request week is:" << nStartWeekNum << nTermWeekNum;
                    continue;
                }

                bool bRepeatDisplay = false;
                int repeat = pBaseLesson->getRepeat();

                if(repeat == 0) {
                    continue;
                }

                if (repeat == 1)
                {
                    bRepeatDisplay = true;
                }
                else
                {
                    int weekDiff = nTermWeekNum - nStartWeekNum;

                    if(weekDiff >= repeat && weekDiff % repeat == 0)
                    {
                        bRepeatDisplay = true;
                    }
                }

                Lesson *pLesson = new Lesson(*pBaseLesson);

                if(bRepeatDisplay) {
                    ///Mark,由于测试数据经常初始日期不对，计算错误，所以改一下方式.
    //                pLesson->setDate(startDate.addDays(((nTermWeekNum - lessonStartWeekNum) * 7) + pLesson->getWeekId()));

                    nAdjustNum = pLesson->getWeekId() - 1;
                    QDate lessonDate = weekStartDate.addDays(nAdjustNum);

                    if(endDate < lessonDate) {
                        continue;
                    }

                    pLesson->setDate(lessonDate);

                    secLessonMap.insert(nSec, pLesson);
                }
            }

        }

        weekLessons[nWeekDay - 1] = secLessonMap;
    }

    return true;
}

int Term::getSectionTypeNumber() const
{
    return mSectionsKeyValeMap.keys().size();
}
//int Term::getSectionNumber() const
//{
//    return mSections.size ();
//}

void Term::getSections(QMap<int, QMap<int, QPair<int, int> > > &sectionMap) const
{
    sectionMap = mSectionsKeyValeMap;
}

QVector<QPair<int, QString> > Term::getBreaks() const
{
    return mBreaks;
}

void Term::setStart(const QDate &start)
{
    mStart = start;
    emit startChanged (start);
    emit changed ();
}

void Term::setEnd(const QDate &end)
{
    mEnd = end;
    emit endChanged (end);
    emit changed ();
}

void Term::setSections(const QMap<int, QMap<int, QPair<int, int> > > &section)
{
    mSectionsKeyValeMap = section;

//    emit sectionChanged (section);
//    emit changed ();
}

void Term::setBreaks(const QVector<Term::Break> &breaks)
{
    mBreaks = breaks;
//    notify();
}

void Term::notify()
{
//    qDebug()<<"come in notify"<<endl;
    emit startChanged (mStart);
    emit endChanged (mEnd);
    emit sectionChanged (mSectionsKeyValeMap);
    emit changed ();
    emit lessonChanged ();
}

QVector<QList<Lesson *> > Term::getLessons()
{
    return mLessons;
}

void Term::setLessons(const QVector<QList<Lesson *> > &mLessons)
{
    qDebug()<<"come in setLessons: "<<mLessons.size()<<" origin size: "<<this->mLessons.size()<<endl;
    this->mLessons.resize(mLessons.size());
    qDebug()<<"after resize: "<<this->mLessons.size()<<endl;
    this->mLessons = mLessons;
    qDebug()<<"here??"<<endl;
    emit lessonChanged ();
    qDebug()<<"here?7777?"<<endl;
    emit changed ();
    qDebug()<<"here??????"<<endl;
}
