#ifndef GETDATANW_H
#define GETDATANW_H

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QVector>
//#include "lessondb.h"
//#include "lessondetaildb.h"
//#include "lessonremarkdb.h"
//#include "resfilesdb.h"
//#include "scheduledb.h"
//#include "scheduledetaildb.h"
//#include "termdb.h"
//#include "handler.h"
//#include "schedule.h"

class LessonDetailDB;
class ScheduleDetailDB;
class Handler;
class TermDB;
class ScheduleDB;
class LessonDB;
class Lesson;

class GetDataNW : public QObject
{
    Q_OBJECT
public:

    explicit GetDataNW(QObject *parent = Q_NULLPTR);

    ~GetDataNW();

    /*!
     * \brief getBaseData,获取学校学科等基础数据.
     */
    void getBaseData();

    void getLessonsByUserID(int userID, int scheID); //通过用户ID获取他的所有课程

    /*!
     * \brief getSemesterDate, 获取数据库中所有学期列表.
     */
    void getSemesterDate();

    /*!
     * \brief getScheduleData, 获取数据库中所有作息时间列表.
     */
    void getScheduleData();

    void mapNetwork2LocalUI();

    void fetchFiles(Lesson*); //下载每个课程格子所需的文件

    Lesson* getFilesState(const QJsonObject&, QVector<QMap<int, Lesson*> >&);  //获取每一个课程格子所需的文件

    void changeTerm(const QJsonObject &jsonObj);

    void reset();

private:
    //QMap<int, QString> remarks;
    ///<\brief lessons, 数据库请求获得的课程表，周几第几节课这种形式，需要对应到整个学期.
    QVector<Lesson*> lessons;

//    QVector<ScheduleDetailDB*> my_sections;
    QMap< QPair<int, QString>, QList<QString> > fileMap; //用于存储每节课的文件列表
    QMap< QPair<int, QString>, QMap<QString, int> > courseFileMap; //用于存储某节课的文件ID和文件名映射
    int courseNum; //记录总共有多少节课
    int courseCount; //getFileState里增加,用于判断是否获取完毕

    Handler* handler = nullptr;
    TermDB* termdb = Q_NULLPTR;
    ScheduleDB* schedb = Q_NULLPTR;
    LessonDetailDB* lesson_detail = Q_NULLPTR;
    LessonDB* lessondb = Q_NULLPTR;
    ScheduleDetailDB* scheDetDB = nullptr;

    int m_nFileResQueryCount = 0;

    bool m_bLessonDetailLoaded;

    bool m_bScheduleDetailLoaded;

    bool m_bFirstLoaded = true; //是否为第一次登录加载.

    /*!
     * \brief mapLessonToTerm, 将课程对应到整个学期.
     */
    void mapLessonToTerm();

signals:
    void showMainWindow();

private slots:
    /*!
     * \brief getBaseDataFinish，获取标准学科信息返回.
     */
    void getBaseDataFinish(const QJsonObject &);
    void detailFinish(const QJsonObject &);
    void getScheState(const QJsonObject &);
    void getSemState(const QJsonObject &);
    void getLessonState(const QJsonObject &);
    void getLessonDetailState(const QJsonObject &);
    void getRemarkState(const QJsonObject &); //获取每一个课程格子的备注

    void addLessonTableFinished(const QJsonObject &json);
};

#endif // GETDATANW_H
