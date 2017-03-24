#include "getdatanw.h"
#include "config.h"
#include "semester.h"
#include "schedule.h"
#include "ccu.h"
#include "lesson.h"
#include "scheduledetaildb.h"
#include "term.h"
#include "downloadfile.h"
#include "handler.h"
#include "lessondb.h"
#include "lessondetaildb.h"
#include "lessonremarkdb.h"
#include "resfilesdb.h"
#include "scheduledb.h"
#include "termdb.h"
#include "schedule.h"
#include "dataclassinstancemanage.h"
#include "teacher.h"
#include <QJsonArray>
#include <QHash>
#include <QMessageBox>
#include <QDebug>

using Section = QPair<int, int>;
using Break = QPair<int, QString>;
class downloadFile;

void GetDataNW::getBaseDataFinish(const QJsonObject &json)
{
    qDebug()<<"base data: "<<json<<endl;
    qDebug()<<"subject result: "<<json["result"]<<endl;
    QJsonArray resultAry = json["result"].toArray();
    for (auto res : resultAry)
    {
        int subjectId =  res.toObject()["id"].toInt();
        QString subjectName = res.toObject()["name"].toString();
        Arg::subjectMap.insert(subjectName, subjectId);
    }
    qDebug()<<"subjectMap size: "<<Arg::subjectMap.size()<<endl;
    getSemesterDate();
}

GetDataNW::GetDataNW(QObject *parent):
    QObject(parent)
{
    courseCount = 0;
    courseNum = 0;

    Handler *pHandler = Handler::getInstance();
    connect(pHandler, SIGNAL(dataNWReplyArrived(const QJsonObject &)), this, SLOT(getBaseDataFinish(const QJsonObject &)));
}

GetDataNW::~GetDataNW()
{
    //        if (handler != nullptr) delete handler;
    Handler::destroyInstance();
    if (termdb != Q_NULLPTR) delete termdb;
    if (schedb != Q_NULLPTR) delete schedb;
//    if (lesson_detail != Q_NULLPTR) delete lesson_detail;
    if (lessondb != Q_NULLPTR) delete lessondb;
    if (scheDetDB != Q_NULLPTR) delete scheDetDB;
}

void GetDataNW::getBaseData()
{
    qDebug()<<"get base data from database"<<endl;
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

//    QString baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;
    QString url = sBaseUrl + "subjectInfoJSON" + Arg::tokenStr2;
    qDebug()<<"url: "<<url<< endl;
//    handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(getBaseDataFinish(QJsonObject)));
//    handler->run();

    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::DataNW);
//    pHandler->run();
}


void GetDataNW::getSemesterDate()
{
     m_bLessonDetailLoaded = false;
     m_bScheduleDetailLoaded = false;

    qDebug()<<"get semester data from database"<<endl;
    if(termdb == Q_NULLPTR) {
//        termdb = new TermDB();
//        connect(termdb, SIGNAL(queryAllFinish(QJsonObject)), this, SLOT(getSemState(QJsonObject)));
        termdb = DataClassInstanceManage::getInstance()->getTemDBPtr();
        connect(termdb, &TermDB::queryAllTermFinished, this, GetDataNW::getSemState);
    }

    termdb->queryAll();
    //termdb->querySchool();
}

void GetDataNW::getScheduleData()
{
    qDebug()<<"get schedule data from database"<<endl;
    if(schedb == Q_NULLPTR) {
        schedb = new ScheduleDB();
        connect(schedb, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(getScheState(const QJsonObject&)));
    }

    schedb->setTermId(Arg::currentTerm()->getTermID());
    schedb->findByTerm();

    //TermDB* termdb = new TermDB();

    //connect(termdb, SIGNAL(queryUserFinish(QJsonObject)), this, SLOT(getSemState(QJsonObject)));
    //termdb->querySchool();
    //termdb->setOwnership(5);
    //termdb->queryUser();

    //connect(schedb, SIGNAL(queryByTermFinish(QJsonObject)), this, SLOT(getScheState(QJsonObject)));
    //schedb->setTermId(1);
    //schedb->queryByTerm();

}

//获取到作息时间表信息.
void GetDataNW::getScheState(const QJsonObject &json)
{
    qDebug()<<"schedule result: "<<json["result"]<<endl;
    //otmTeaLessonDetails
    QJsonArray resultAry = json["result"].toArray();
    Arg::setSchedules(new QVector<Schedule*>);

    for (auto res : resultAry)
    {
        Schedule* sche = new Schedule();

        int sid =  res.toObject()["id"].toInt();
        int section = res.toObject()["section"].toInt();
        QString name = res.toObject()["name"].toString();
        QString grades = res.toObject()["grades"].toString();

        QString createtimeStr = res.toObject()["createtime"].toString();
        QStringList strlist = createtimeStr.split("T");
        QDate createtime = QDate::fromString (strlist[0], "yyyy-MM-dd");

        QDate starttime = QDate::fromString (res.toObject()["startdate"].toString(), "yyyy-M-d");
        QDate endtime = QDate::fromString (res.toObject()["enddate"].toString(), "yyyy-M-d");

        sche->setId(sid);
        sche->setSection(section);
        sche->setName(name);
        sche->setGrades(grades);
        sche->setCreateTime(createtime);
        sche->setStartTime(starttime);
        sche->setEndTime(endtime);

        Arg::schedules()->append(sche);

        qDebug()<<"sid: "<<sid<<" section: "<<section<<" name: "<<name<<" starttime: "<<starttime.toString("yyyy-M-d")
               <<endl;

        /*
        QJsonArray lessons = res.toObject()["teaLesson"].toArray ();
        qDebug()<<"lesson array: "<<lessons<<endl;

        for (auto les : lessons)
        {
            QJsonArray courses = les.toObject()["otmTeaLessonDetails"].toArray();
            for (auto cou : courses)
            {
                QJsonObject couObj = cou.toObject();
                qDebug()<<"className: "<<couObj["classname"]<<" roomno: "<<couObj["roomno"]<<" subject: "<<couObj["fkSubjectId"].toObject()["name"];
            }
            qDebug()<<endl;
        }
        */
    }

    //Q_ASSERT(!Arg::schedules()->isEmpty());
    if(Arg::schedules()->isEmpty())
    {
        qDebug()<<"Schedules in database is Empty!!!!!"<<endl;
        return;
        //exit(-1);
    }

    QVector<Schedule*> *scheduleVec = Arg::schedules();
    Schedule *pSchedule = scheduleVec->first();
    Arg::setCurrentSchedule(pSchedule);
    Arg::nw->getLessonsByUserID(Arg::userId, pSchedule->getId());
}

void GetDataNW::getSemState(const QJsonObject &json)
{
    qDebug()<<"semester result: "<<json<<endl;
    QJsonArray terms = json["result"].toArray();

    Arg::setSemesters(new QVector<Semester*>);

    QDate newlyEndDate = QDate::currentDate();
    Semester *newlySem = Q_NULLPTR;

    for(auto term : terms)
    {
        Semester* sem = new Semester();
        /*
        id = idV.toInt();
        mTitle = titleV.toString();
        mTerm = termV.toString();
        mCreateTime = QDate::fromString (createTimeV.toString (), Qt::ISODate);
        mUpdateTime = QDate::fromString (updateTimeV.toString (), Qt::ISODate);
        mYearStart = yearStartV.toInt();
        mOwnership = ownershipV.toInt();
        mStartTime = QDate::fromString (starttimeV.toString (), Qt::ISODate);
        mEndTime = QDate::fromString (endtimeV.toString (), Qt::ISODate);
        */

        int id = static_cast<int>(term.toObject()["id"].toDouble());
        int ownership = static_cast<int>(term.toObject()["ownership"].toDouble());
        QString termName = term.toObject()["term"].toString();
        QString title = term.toObject()["title"].toString();
        int yearstart = term.toObject()["yearstart"].toString().toInt();
        QString starttimeStr = term.toObject()["starttime"].toString();
        //qDebug()<< "starttimeStr: "<<starttimeStr<<endl;
        QStringList strlist = starttimeStr.split(" ");
        //qDebug()<<"strlist: "<<strlist[0]<<endl;
        QDate starttime = QDate::fromString (strlist[0], "yyyy-M-d");

        starttimeStr = term.toObject()["endtime"].toString();
        strlist = starttimeStr.split(" ");
        QDate endtime = QDate::fromString (strlist[0], "yyyy-M-d");

        starttimeStr = term.toObject()["createtime"].toString();
        strlist = starttimeStr.split(" ");
        QDate createtime = QDate::fromString (strlist[0], "yyyy-M-d");

        starttimeStr = term.toObject()["updatetime"].toString();
        strlist = starttimeStr.split(" ");
        QDate updatetime = QDate::fromString (strlist[0], "yyyy-M-d");

        qDebug()<<"ownership: "<<ownership<<" yearstart: "<<yearstart<<" starttime: "<<starttime
                <<" endtime: "<<endtime<<" createtime: "<<createtime<<" updatetime: "<<updatetime<<endl;

        sem->setId(id);
        sem->setOwnership(ownership);
        sem->setTerm(termName);
        sem->setTitle(title);
        sem->setYearStart(yearstart);
        sem->setStartTime(starttime);
        sem->setEndTime(endtime);
        sem->setCreateTime(createtime);
        sem->setUpdateTime(updatetime);

        Arg::semesters()->append(sem);

        if(newlyEndDate < endtime) {
            newlyEndDate = endtime;
            newlySem = sem;
        }
    }

    if(!Arg::semesters()->isEmpty())
    {
        if(newlySem == Q_NULLPTR) {
            newlySem = Arg::semesters()->first();
        }

        Arg::setCurrentSemester(newlySem);

        Term *pTerm = new Term(newlySem->getId(), newlySem->getStartTime(), newlySem->getEndTime(), this);
        Arg::setCurrentTerm(pTerm);

//        Arg::currentTerm()->setStart(Arg::currentSemester()->getStartTime());
//        Arg::currentTerm()->setEnd(Arg::currentSemester()->getEndTime());
    }
    ///TODO 2017.02.20.
    else { //如果不存在学期信息，则直接建立一个学期，默认.
//        Term *pTerm = new Term(pCurSem->getId(), pCurSem->getStartTime(), pCurSem->getEndTime(), this);
//        Arg::setCurrentTerm(pTerm);
    }

    //schedule data is base on semester data
    getScheduleData();
}

void GetDataNW::changeTerm(const QJsonObject &jsonObj)
{
    m_bLessonDetailLoaded = false;
    m_bScheduleDetailLoaded = false;

    QJsonArray jsonArray = jsonObj["list"].toArray();

    int index = 0;
    Semester* pNeedSem = Q_NULLPTR;

    for(auto value : jsonArray) {
        Semester* sem = new Semester();
        QString sCreateTime = value.toObject()["createtime"].toString();
        QStringList strlist = sCreateTime.split(" ");
        QDate createTime = QDate::fromString (strlist[0], "yyyy-M-d");

        QString sEndTime = value.toObject()["endtime"].toString();
        strlist = sEndTime.split(" ");
        QDate endTime = QDate::fromString(strlist[0], "yyyy-M-d");

        int id = static_cast<int>(value.toObject()["id"].toDouble());

        int ownership = static_cast<int>(value.toObject()["ownership"].toDouble());

        QString sStartTime = value.toObject()["starttime"].toString();
        strlist = sStartTime.split(" ");
        QDate startTime = QDate::fromString(strlist[0], "yyyy-M-d");

        int nState = static_cast<int>(value.toObject()["state"].toDouble());

        QString sTitle = value.toObject()["title"].toString();

        QString sUpdateTime = value.toObject()["updatetime"].toString();
        strlist = sUpdateTime.split(" ");
        QDate updatetime = QDate::fromString(strlist[0], "yyyy-M-d");

        int yearstart = value.toObject()["yearstart"].toString().toInt();

        sem->setId(id);
        sem->setOwnership(ownership);
//        sem->setTerm(termName);
        sem->setTitle(sTitle);
        sem->setYearStart(yearstart);
        sem->setStartTime(startTime);
        sem->setEndTime(endTime);
        sem->setCreateTime(createTime);
        sem->setUpdateTime(updatetime);

        Arg::semesters()->append(sem);

        if(index == 0) {
            pNeedSem = sem;
        }

        ++index;
    }


    if(pNeedSem != Q_NULLPTR) {
        Arg::setCurrentSemester(pNeedSem);
        Term *pTerm = new Term(pNeedSem->getId(), pNeedSem->getStartTime(), pNeedSem->getEndTime(), this);
        Arg::setCurrentTerm(pTerm);

        getScheduleData();
    }
}

void GetDataNW::reset()
{
    lessons.clear();
    fileMap.clear();
    Arg::fileIdMap.clear();

    courseNum = 0;
    courseCount = 0;

    m_nFileResQueryCount = 0;

    m_bLessonDetailLoaded = false;

    m_bScheduleDetailLoaded = false;

    m_bFirstLoaded = true;
}

Lesson* GetDataNW::getFilesState(const QJsonObject &json, QVector<QMap<int, Lesson*> > &pRepyLesson) //获取每个课程格子的资源文件
{
#if 1
//    if(json["status"].toString() == "false"){
//        qDebug()<<"api status is false"<<endl;
//        return nullptr;
//    }

//    Arg::fileIdMap.clear();
    //    ///记录数据延迟显示.
    //    static int nQueryCount = 0;
    //    ++nQueryCount;
    //    ///End.

    qDebug()<<"####################GetDateNW lessonFiles: #######################"<<endl;
    //    qDebug()<<json<<endl;
    //    qDebug()<<"########################################################"<<endl;
    QJsonArray filesA = json["result"].toArray ();

    //----记录数据库返回的课程信息，用于匹配参数weekLessons中的某一节课程
    QString lesDate;
    int lesDetailId;
    QMap<QString, int> id2name; //文件名与ID Map.
    fileMap.clear();
    QList<QString> filelist;

    for(auto file : filesA)
    {
        int id = file.toObject()["id"].toInt();
        int detId = file.toObject()["fkLessondetail"].toInt();
        QString date = file.toObject()["usedatetime"].toString("yyyy-M-d").split(" ").at(0);

        lesDate = date;
        lesDetailId = detId;

        //qDebug()<<"--------Date: "<<date<<"-------------"<<endl;
        QString filename = file.toObject()["filename"].toString();

//        QPair<int, QString> course;
//        course.first = detId;
//        course.second = date;


//        if(fileMap.find(course) == fileMap.end()) //文件所属课程尚未构造map
//        {
            //qDebug()<<"course has no map: "<<course.first<<" "<<course.second<<endl;

        if(!filelist.contains(filename)) {
            filelist.append(filename);
        }

        if(!id2name.contains(filename)) {
            id2name.insert(filename, id);
        }
//        fileMap.insert(course, filelist);
//        fileMap[course] = filelist;
            //            Arg::fileIdMap[course] = id2name;

//        }
//        else //文件所属课程已经在map里
//        {
//            filelist = fileMap[course];
//            id2name = Arg::fileIdMap[course];

//            if(!filelist.contains(filename)) //文件尚未被加入到该课程中
//            {
//                //qDebug()<<"add file: "<<filename<<" to course: "<<course.first<<" "<<course.second<<endl;
//                filelist.append(filename);
//                id2name.insert(filename, id);
//                fileMap[course] = filelist;
//                //                Arg::fileIdMap[course] = id2name;
//            }

//        }
    }


    QPair<int, QString> course;
    course.first = lesDetailId;
    course.second = lesDate;

    fileMap.insert(course, filelist);


    //courseCount++;
    qDebug()<<"fileMap size: "<<fileMap.size()<<endl;

    //qDebug()<<"courseNum: "<<courseNum<<" courseCount: "<<courseCount<<endl;

    //-----找到返回的数据对应的Lesson指针，传给fetchFiles去ftp获取文件--------//
    Lesson* les = nullptr;
    for (int i = 0; i < pRepyLesson.size(); i++)
    {
        QMap<int, Lesson*>::const_iterator lesson_mapIt;
        for( lesson_mapIt=pRepyLesson[i].constBegin(); lesson_mapIt!=pRepyLesson[i].constEnd(); ++lesson_mapIt)
        {
            if(lesson_mapIt.value()->getDate().toString("yyyy-M-d") == lesDate &&
                    lesson_mapIt.value()->getLessonDetailId() == lesDetailId){
                qDebug()<<"find the mother of the resource files "<<lesDetailId<<" "<<lesDate<<endl;
                les = lesson_mapIt.value();

                break; //找到中断循环.
            }
        }

        if(les != nullptr) {
            break;
        }
    }


    //if(courseCount == courseNum) //获取文件列表完毕
    //{
    qDebug()<<"Strar to download, courseCount: "<<endl;
    //        Arg::fileIdMap = courseFileMap;
    if((les != nullptr) && (!id2name.isEmpty())) {
        fetchFiles(les, id2name);
    }

    return les;
    //}

    //qDebug()<<"files array: "<<filesA<<endl;
    //QDir root = Arg::configDir;
    //qDebug() << "root: " << root << endl;

    //    ///Add by 2017.02.13,最后通知ui显示.

    //    if(nQueryCount == m_nFileResQueryCount) {
    //        //emit showMainWindow();
    //        return;
    //    }

#endif

}

void GetDataNW::getRemarkState(const QJsonObject &json) //获取每个课程格子的备注
{
    qDebug()<<"lessonRemark: "<<json<<endl;
    //QJsonValue remarkV = json;
    //qDebug()<<"remakV: "<<remarkV<<endl;
    QString statue = json["status"].toString();
    if (statue == "true")
    {
        QString remark = json["result"].toObject()["remark"].toString();
        int detailId = json["TLDid"].toInt();
        QString date = json["Teachingdate"].toString().split(" ").at(0);
        QPair<int, QString> course;
        course.first = detailId;
        course.second = date;
        Arg::noteMap.insert(course, remark);
    }
    qDebug()<<"noteMap size: "<<Arg::noteMap.size()<<endl;

    //int lessondetail = static_cast<int>(remarkV.toObject()["fk_lessondetail"].toDouble());
    //qDebug()<<"remark: "<<remark;
    //remarks.insert(lessondetail,remark);
    //qDebug()<<"map size: "<<remark.size()<<endl;
}

//--------------------从ftp下载文件------------------------------//
void GetDataNW::fetchFiles(Lesson* les, const QMap<QString, int> &fileNameIDMap)
{

    if(les == nullptr){
        qDebug()<<"Null lesson pointer in fetchFiles"<<endl;
        return;
    }
    /*
    for (int i = 0; i < Arg::currentTerm()->getLessons().size(); i++)
    {
        QList<Lesson*> oneWeekLessons = Arg::currentTerm()->getLessons().at(i);
        for (int j = 0; j < oneWeekLessons.size(); j++)
        {
     */

    /*
    for (int i = 0; i < weekLessons.size(); i++)
    {
        QMap<int, Lesson*>::const_iterator lesson_mapIt;
        for( lesson_mapIt=weekLessons[i].constBegin(); lesson_mapIt!=weekLessons[i].constEnd(); ++lesson_mapIt)
        {
     */
            //Lesson* les = lesson_mapIt.value();
#if 0
            downloadFile* dtTool = new downloadFile();
            QMap<QString, QString> args;
            args["date"] = les->getDate().toString("yyyy-M-d");
            args["section"] = QString::number(les->getSection());
            args["detailID"] = QString::number(les->getLessonDetailId());
            args["folderName"] = args["date"] + "-" + args["section"];
#endif

            /*
            QString filename = QString::number(Arg::userId) + QString::number(les->getLessonDetailId())
                    + les->getDate().toString("yyyy-M-d");
            */

            QPair<int, QString> courseDate;
            courseDate.first = les->getLessonDetailId();
            courseDate.second = les->getDate().toString("yyyy-M-d");


            QList<QString> fn = fileMap[courseDate];

            qDebug()<<"first: "<<courseDate.first<<" second: "<<courseDate.second<<endl;
            for (int k = 0; k < fn.size(); k++)
            {
                //QString eachFile = filename + fn[k];
                qDebug()<<"K: "<<k<<endl;
                QString eachFile = fn[k];
                QString fp = "./SaveFile/" + eachFile;
                qDebug()<<"fp--->"<<fp<<endl;
                File* file = new File(fp, les->rootFolder());
                les->rootFolder()->add(file);
//                les->notify();

                quint32 nID = fileNameIDMap.value(file->name());
                file->setFileID(nID);

                les->name2File.insert(file->name(), file);

                ///Mark,暂时让用户自己选择下载.
#if 0

                qDebug()<<"fileName in getNW: "<<eachFile<<endl;
                args["fileName"] = eachFile;
                dtTool->download(args, les);
#endif
            }

            QMap<QPair<int, QString>, QString>::iterator it;
            it = Arg::noteMap.find(courseDate);
            if( it != Arg::noteMap.end())
            {
                les->setNote(it.value());
            }

#if 0
            ///Add by BiXiaoxia.
            delete dtTool;
            dtTool = Q_NULLPTR;
#endif
            //return;

//        }
//    }

}

void GetDataNW::mapLessonToTerm()
{
    //计算当前周数.
    Term *pTerm = Arg::currentTerm();
    int nWeeksNum = pTerm->getTermWeeksNumber();

    Q_ASSERT_X(nWeeksNum > 0, "mapLessonToTerm", "Term weeks number equals to 0");

    QDate term_start = pTerm->getStart();
//    QDate term_end = pTerm->getEnd();

//    int weekNum = term_end.weekNumber() - term_start.weekNumber() + 1;

//    if(weekNum < 0) //跨年
//    {
//        weekNum = 52 - term_start.weekNumber() + 1 + term_end.weekNumber();
//        if(weekNum < 0)
//        {
//            qDebug()<<"跨年周数计算错误"<<endl;
//            return;
//        }
//    }

//    qDebug()<<"begin weekNum: "<<term_start.weekNumber()<<" end weekNum: "<<term_end.weekNumber()<<endl;
//    qDebug()<<"term_start: "<<term_start<<" term_end: "<<term_end<<" weekNum: "<<weekNum<<endl;

    QVector<QList<Lesson*> > lessons_ui;

    //因为查询出的课程为周几某时间段课信息，所以需要将课程对应到整个学期.
    for (int nWeek = 0; nWeek < nWeeksNum; ++nWeek) //for each week
    {
        qDebug()<<"week: "<< nWeek <<endl;
        //当前周课程.
        QList<Lesson*> lessonList;

        for (int j = 0; j < lessons.size(); j++) //for each lesson, decide add or not
        {
            Lesson *les = lessons.at(j);
//            LessonDetailDB* tempLes = lessons[j]; //lesson for network
//            Lesson* les = new Lesson(); //lesson for ui
//            les->setLessonId(tempLes->getLessonId());
//            les->setLessonDetailId(tempLes->getId());
//            les->setClassId(tempLes->getClassId());
//            les->setRoomId(tempLes->getRoomId());
//            les->setLocation(tempLes->getRoom());
//            les->setSection(tempLes->getSectionId());
//            les->setSubject(tempLes->getSubject());
//            les->setUnit(tempLes->getClassName());
//            les->setWeekId(tempLes->getWeekId());
//            les->setStartDate(tempLes->getStartDate());
//            les->setEndDate(tempLes->getEndDate());
//            les->setRepeat(tempLes->getRepeat());
            les->setDate(term_start.addDays((nWeek * 7) + les->getWeekId()));

            //            qDebug()<<"lesson: "<<j<<" lessonDetailID: "<<les->getLessonDetailId()<<endl;

            int repeat = les->getRepeat();

            if (nWeek == 0 || repeat == 1)
            {
                lessonList.append(les);
            }
            else
            {
                if(nWeek >= repeat && repeat % nWeek == 0)
                {
                    lessonList.append(les);
                }
            }
        }

        lessons_ui.append(lessonList);

        ///MarkTodo 2017.02.15,这里考虑怎么处理.
//        ///Mark,这里需要修改，只需要加载当前周的课程的文件资源即可，其余的显示哪周再去加载.
//        courseNum = courseNum + lessonList.size();
    }

    ///Mark Todo,这里需要考虑如果课程对应为空的提示.
    int week = QDate::currentDate().weekNumber();

//    qDebug() << "the weeknum is:" << weekNum << "the current week index is:" << Arg::currentWeekIndex()
//             << term_start.weekNumber() << term_start << "   " << term_end.weekNumber() << term_end;
//    qDebug()<<"=======Total course number: "<<courseNum<<"=========="<<endl;

    Arg::currentTerm()->setLessons(lessons_ui);
    qDebug()<< "network lesson ui size: "<< Arg::currentTerm()->getLessons().size()<<endl;
}

//--------------------把通过API获取的后台数据转化成前端需要的格式显示出来-----------------------------------//
///Mark2017.02.15，感觉这种所有读取出来的方式有点问题.分周后课程好像也不正常???.
void GetDataNW::mapNetwork2LocalUI()
{
//    ///计算当前周数.
//    QDate term_start = Arg::currentTerm()->getStart();
//    QDate term_end = Arg::currentTerm()->getEnd();

//    int weekNum = term_end.weekNumber() - term_start.weekNumber() + 1;

//    if(weekNum < 0) //跨年
//    {
//        weekNum = 52 - term_start.weekNumber() + 1 + term_end.weekNumber();
//        if(weekNum < 0)
//        {
//            qDebug()<<"跨年周数计算错误"<<endl;
//            return;
//        }
//    }

//    qDebug()<<"begin weekNum: "<<term_start.weekNumber()<<" end weekNum: "<<term_end.weekNumber()<<endl;
//    qDebug()<<"term_start: "<<term_start<<" term_end: "<<term_end<<" weekNum: "<<weekNum<<endl;

//    QVector<QList<Lesson*> > lessons_ui;

////    qDebug()<<"local lesson ui size: "<<Arg::currentTerm()->getLessons().size()<<endl;
////    for (int i = 0; i < Arg::currentTerm()->getLessons().size(); i++)
////    {
////        for (int j = 0; j < Arg::currentTerm()->getLessons()[i].size(); j++)
////        {
////            qDebug()<<"i: "<<i<<" j: "<<Arg::currentTerm()->lessonWeek(i)[j]->location()<<endl;
////        }
////    }

//    //因为查询出的课程为周几某时间段课信息，所以需要将课程对应到整个学期.
//    for (int nCurWeek = 0; nCurWeek < weekNum; ++nCurWeek) //for each week
//    {
//        qDebug()<<"week: "<< nCurWeek <<endl;
//        //当前周课程.
//        QList<Lesson*> lessonList;

//        for (int j = 0; j < lessons.size(); j++) //for each lesson, decide add or not
//        {
//            LessonDetailDB* tempLes = lessons[j]; //lesson for network
//            Lesson* les = new Lesson(); //lesson for ui
//            les->setLessonId(tempLes->getLessonId());
//            les->setLessonDetailId(tempLes->getId());
//            les->setClassId(tempLes->getClassId());
//            les->setRoomId(tempLes->getRoomId());
//            les->setLocation(tempLes->getRoom());
//            les->setSection(tempLes->getSectionId());
//            les->setSubject(tempLes->getSubject());
//            les->setUnit(tempLes->getClassName());
//            les->setWeekId(tempLes->getWeekId());
//            les->setStartDate(tempLes->getStartDate());
//            les->setEndDate(tempLes->getEndDate());
//            les->setRepeat(tempLes->getRepeat());
//            les->setDate(term_start.addDays((nCurWeek * 7) + les->getWeekId()));

////            qDebug()<<"lesson: "<<j<<" lessonDetailID: "<<les->getLessonDetailId()<<endl;

//            int repeat = tempLes->getRepeat();
//            if (nCurWeek == 0 || repeat == 1)
//            {
//                lessonList.append(les);
//            }
//            else
//            {
//                if(nCurWeek >= repeat && repeat % nCurWeek == 0)
//                {
//                    lessonList.append(les);
//                }
//            }
//        }

//        ///Mark,这里需要修改，只需要加载当前周的课程的文件资源即可，其余的显示哪周再去加载.
//        courseNum = courseNum + lessonList.size();
//        lessons_ui.append(lessonList);
//    }

//    qDebug()<<"=======Total course number: "<<courseNum<<"=========="<<endl;

//    Arg::currentTerm()->setLessons(lessons_ui);
//    qDebug()<<"network lesson ui size: "<<Arg::currentTerm()->getLessons().size()<<endl;

    //-------------------------使用ftp获取需要下载的文件列表到本地-------------------------------------------//
  ///Mark 2017.02.16,这里暂时不放开，先调试别的，做的时候找到当前周只加载当前周的即可, 而且请求资源要队列管理请求的课程，否则会混乱.
  #if 0
    for (int i = 0; i < Arg::currentTerm()->getLessons().size(); i++)
    {
        for (int j = 0; j < Arg::currentTerm()->getLessons()[i].size(); j++)
        {
            ++m_nFileResQueryCount;

            qDebug()<<"i: "<<i<<" j: "<<Arg::currentTerm()->lessonWeek(i)[j]->location()<<endl;
            //--------------Get remak and files of a course cell------------------//

            Lesson* les = Arg::currentTerm()->lessonWeek(i)[j];
//            LessonRemarkDB* lesson_remark = new LessonRemarkDB();
            ResFilesDB* res_file = new ResFilesDB();

            ///Mark Delete 20176.02,14,这块不要，等课节信息框出现再获取.
//            LessonRemarkDB* lesson_remark = new LessonRemarkDB();
//            lesson_remark->setLessonDetailId(les->getLessonDetailId());
//            lesson_remark->setTeachingDate(les->getDate());
//            connect(lesson_remark, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(getRemarkState(const QJsonObject&)));
//            lesson_remark->find();

            res_file->setLessonDetailId(les->getLessonDetailId());
            res_file->setUseDataTime(les->getDate());
            connect(res_file, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(getFilesState(const QJsonObject&)));
            res_file->find();
            //--------------------------------------------------------------------//

        }
    }

#endif
}

void GetDataNW::getLessonDetailState(const QJsonObject &json) //获取所有课程
{
    qDebug()<<"lessonDetail: "<<json<<endl;
    QJsonArray lessonsA = json["result"].toArray ();
    //qDebug()<<"lesson array: "<<lessonsA<<endl;
    //return;

//    int index = 0;
//    QVector<QList<Lesson*> > lessonsListVec;

    QMultiHash<int, Lesson*> multiHash;

    for (QJsonValue weekLesson: lessonsA)
    {
        //int lesson_id = static_cast<int>(week.toObject()["id"].toDouble());
        qDebug()<<"===================Lesson Id: "<<Arg::lessonId<<"===================="<<endl;

        QJsonObject object = weekLesson.toObject();
        int classid = object["classid"].toInt();
        int roomid  = object["roomid"].toInt();
        int id      = object["id"].toInt();
        int section = object["sectionid"].toInt();
        int repeatperiod = object["repeatperiod"].toInt();
        int weekId = object["weekid"].toInt();
        QString subject = object["infoSubject"].toObject()["name"].toString();
        QString classname = object["classname"].toString();
        QString location = object["roomno"].toString();

        QString starttimeStr = object["startdate"].toString();
        QStringList strlist = starttimeStr.split(" ");
        QDate startdate = QDate::fromString (strlist[0], "yyyy-M-d");

        starttimeStr = object["enddate"].toString();
        strlist = starttimeStr.split(" ");
        QDate enddate = QDate::fromString (strlist[0], "yyyy-M-d");

        qDebug()<<"stratdate: "<<startdate.toString("yyyy-M-d")<<" enddate: "<<enddate.toString("yyyy-M-d")
               <<" classname: "<<classname<<" section: "<<section<<" subject: "<<subject
              << "the lesson repeat:"<< repeatperiod <<endl;

        //--------------These sections are breaks-------------------//
        ///Mark2017.02.16，这里为什么把section == 4的去掉？.
//        if(section == 4 || section == 7)continue;
#if 1
        Lesson* les = new Lesson(); //lesson for ui
        les->setLessonId(Arg::lessonId);
        les->setLessonDetailId(id);
        les->setClassId(classid);
        les->setRoomId(roomid);
        les->setLocation(location);
        les->setSection(section);
        les->setSubject(subject);
        les->setUnit(classname);
        les->setWeekId(weekId);
        les->setStartDate(startdate);
        les->setEndDate(enddate);
        les->setRepeat(repeatperiod);

//    lessons.append(les);
        multiHash.insertMulti(weekId, les);
#else

        //For network
        LessonDetailDB* lessonDet = new LessonDetailDB();
        lessonDet->setId(id);
        lessonDet->setRoomId(roomid);
        lessonDet->setClassId(classid);
        lessonDet->setStartDate(startdate);
        lessonDet->setEndDate(enddate);
        lessonDet->setRoom(location);
        lessonDet->setClassName(classname);
        lessonDet->setSubject(subject);
        lessonDet->setLessonId(Arg::lessonId);
        lessonDet->setWeekId(weekId);
        lessonDet->setSectionId(section);
        lessonDet->setRepeat(repeatperiod);

        lessons.append(lessonDet);
#endif
    }

    QMap<int, QMultiMap<int, Lesson *> > baseLessonMap;
    QList<int> weekIDList = multiHash.keys();

    for(int nPos = 0; nPos < weekIDList.size(); ++nPos) {
        QMultiMap<int, Lesson*> lessonMap;
        int nWeekDay = weekIDList.at(nPos);
        QList<Lesson*> lessList = multiHash.values(nWeekDay);

        for(int lesPos = 0; lesPos < lessList.size(); ++lesPos) {
            Lesson *pLess = lessList.at(lesPos);
            int nSection = pLess->getSection();
            lessonMap.insertMulti(nSection, pLess);
        }

        baseLessonMap.insert(nWeekDay, lessonMap);
    }

    if(!baseLessonMap.isEmpty()) {
        Arg::currentTerm()->setTermBaseLessons(baseLessonMap);
    }

    ///Mark,暂时去掉，对应放进各自周去做, 内存不存放这么多东西.
//    mapLessonToTerm();

//    mapNetwork2LocalUI();
    m_bLessonDetailLoaded = true;

    ///Mark 2017.02.16,暂时放这里，等资源加载完成再去掉.
    ///

    if((m_bLessonDetailLoaded) && (m_bScheduleDetailLoaded)) {
        if(m_bFirstLoaded) {
            emit showMainWindow();
        }
        else {
            Arg *pArg = Arg::getInstance();
            emit pArg->termLoadFinished();
        }

        m_bFirstLoaded = false;
    }

//    ///
//    delete lesson_detail;
//    lesson_detail = Q_NULLPTR;
}

void GetDataNW::addLessonTableFinished(const QJsonObject &json)
{
//    getLessonState(json);

    lessondb->setUserId(Arg::userId);
    int scheID = Arg::currentSchedule()->getId();
    lessondb->setScheduleId(scheID);
    lessondb->find();
}

//课程表信息.
void GetDataNW::getLessonState(const QJsonObject &json)
{
    qDebug()<<"lseesondb result: "<<json<<endl;
    QJsonArray lessons = json["result"].toArray();
    //qDebug()<<"result: "<<lessons<<endl;

    ///Mark 2017.02.17，这里服务器数据异常，返回lessons数组为空.
    //Q_ASSERT(lessons.size() == 1);
    if(lessons.size() != 1)
    {
        ///Mark 2017.03.06,这里先临时替数据库创建主课程表.
        qDebug()<< tr("课程主表数据库异常")<<endl;
//        QMessageBox::information(Q_NULLPTR, tr("课表数据异常"), tr("请建立课表数据"));

//        emit showMainWindow();

        lessondb->add();
//        Desktop/addTeaLessonJSON?TLfkUseridNO=2&TLfkScheduleidNO=1&TLstate=1&TLtype=1
        ///Mark End.

        return;
    }

    ///沟通过一次只能有一张课表，所以暂时这么处理.
    QJsonValue lesson = lessons.at(0);
    //for(auto lesson : lessons)
    //{
        QJsonValue lessonId = lesson.toObject()["id"];
        int lesson_id = static_cast<int>(lessonId.toDouble());
        qDebug()<<"lessonId: "<<lesson_id;
        Arg::lessonId = lesson_id;

//        if(lesson_detail == Q_NULLPTR) {
//        lesson_detail = new LessonDetailDB();
        LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
        connect(pLessonDetailDB, SIGNAL(findAllFinish(const QJsonObject&)), this, SLOT(getLessonDetailState(const QJsonObject&)));
//        }

        pLessonDetailDB->setLessonId(lesson_id);
        pLessonDetailDB->findAll();
    //}

        qDebug()<<"getLessonState"<<endl;

    //qDebug()<<"value id: "<<lessonId.t<<endl;
    //return;

        if(lessondb != Q_NULLPTR) {
            delete lessondb;
            lessondb = Q_NULLPTR;
        }
}

//详细作息时间表.
void GetDataNW::detailFinish(const QJsonObject &json) //获取课节详细信息，总课节数，每节课起始时间
{
    qDebug()<<"-----------Detail Section Info-----------------"<<endl;
    qDebug()<<json<<endl;
    qDebug()<<"-----------------------------------------------"<<endl;

    QJsonArray sections = json["result"].toArray();
    int secNum = sections.size();
    qDebug()<<"secNum: "<<secNum<<endl;

    if(secNum == 0)
    {
        qDebug()<<"作息时间详细表异常"<<endl;
        return;
    }

//    QVector<Section> ui_sections;
    QVector<Break> ui_break;
//    QString timeStr;
    int num = 0;

    //QMap有自动排序行为，默认升序，所以可以来控制课节顺序.
    QMap<int, QMap<int, Section> > typeSecMap;
//    QMap<int, Section> sortSecHash;
//    QList<int> secTypeList;

    for (QJsonValue sec : sections)
    {
        QJsonObject jsonObj = sec.toObject();
        qDebug() << "the jsonObject is:" << jsonObj;
        int secId = jsonObj["sectionid"].toInt();
        int id = jsonObj["id"].toInt();
        QString sType = jsonObj["type"].toString();
        int type = sType.toInt();//上下午标志.
        int scheId = jsonObj["fkScheduleid"].toInt();

        QMap<int, Section> secMap;
        if(typeSecMap.contains(type)) {
            secMap = typeSecMap.value(type);
        }

        QString startTime = jsonObj["starttime"].toString();
        QStringList timeList = startTime.split(":");

        if(timeList.size() < 2)
        {
            qDebug()<<"课节时间数据异常："<<timeList.size()<<endl;
            return;
        }

        int sec_begin = timeList.at(0).toInt() * 3600 + timeList.at(1).toInt() * 60;

        QString endTime = jsonObj["endtime"].toString();
        timeList = endTime.split(":");
        int sec_end = timeList.at(0).toInt() * 3600 + timeList.at(1).toInt() * 60;

        Section ui_sec;
        ui_sec.first = sec_begin;
        ui_sec.second = sec_end;

        secMap.insert(secId, ui_sec);

        typeSecMap.remove(type);
        typeSecMap.insert(type, secMap);

//        sortSecHash.insert(ui_sec.first, ui_sec);

//        ui_sections.append(ui_sec);


        num++;

        qDebug()<<"num: "<<num<<endl;


//        int secId = sec.toObject()["sectionid"].toInt();
//        int id = sec.toObject()["id"].toInt();
//        int type  = sec.toObject()["type"].toInt();
//        int scheId = sec.toObject()["fkScheduleid"].toInt();
//        QString startTime = sec.toObject()["starttime"].toString();
//        QString endTime = sec.toObject()["endtime"].toString();

        ///Mark 2017.02.16,这块没必要留着.
//        ScheduleDetailDB* secDet = new ScheduleDetailDB();
//        secDet->setId(id);
//        secDet->setSectionId(secId);
//        secDet->setScheduleId(scheId);
//        secDet->setType(type);
//        secDet->setStartTime(startTime);
//        secDet->setEndTime(endTime);

//        my_sections.append(secDet);
//        qDebug()<<"sec: "<<secId<<endl;
    }

//    QMap<int, Section>::const_iterator hashItor = sortSecHash.constBegin();
//    for(; hashItor != sortSecHash.constEnd(); ++hashItor) {
//        ui_sections.append(hashItor.value());
//    }

//    //Convert from network date to local ui data
//    QVector<Section> ui_sections;
//    QVector<Break> ui_break;
//    QString timeStr;
//    QStringList timeList;
//    int num = 0;
//    for (auto sec : my_sections)
//    {
//        timeStr = sec->getStartTime();
//        timeList = timeStr.split(":");
//        //Q_ASSERT(timeList.size() == 3);
//        if(timeList.size() < 2)
//        {
//            qDebug()<<"课节时间数据异常："<<timeList.size()<<endl;
//            return;
//        }
//        int sec_begin = timeList.at(0).toInt() * 3600 + timeList.at(1).toInt() * 60;
//                //+ timeList.at(2).toInt();

//        timeStr = sec->getEndTime();
//        timeList = timeStr.split(":");
//        int sec_end = timeList.at(0).toInt() * 3600 + timeList.at(1).toInt() * 60;
                //+ timeList.at(2).toInt();

//        Section ui_sec;
//        ui_sec.first = sec_begin;
//        ui_sec.second = sec_end;

//        ui_sections.append(ui_sec);
/*
        if(num == 4) //add a noon break from 12:00 to 13:00
        {
            Section noon_sec;
            noon_sec.first = 12 * 3600;
            noon_sec.second = 13 * 3600;
            ui_sections.append(noon_sec);

            Break noon_break;
            noon_break.first = num;
            noon_break.second = tr("午休");
            ui_break.append(noon_break);
        }

        if(num == 7) //add a afternoon break from 16:30 to 17:00
        {
            Section afternoon_sec;
            afternoon_sec.first = 16 * 3600 + 30 * 60;
            afternoon_sec.second = 17 * 3600;
            ui_sections.append(afternoon_sec);

            Break afternoon_break;
            afternoon_break.first = num;
            afternoon_break.second = tr("活动");
            ui_break.append(afternoon_break);
        }
*/
//        num++;

//        qDebug()<<"num: "<<num<<endl;
//    }

    ///Mark 2017.02.16, not allowed class count larger than 6?.
//    if(ui_sections.size() <= 6)
//    {
//        qDebug()<<"作息时间详细表异常"<<endl;
//        return;
//    }

    Arg::currentTerm()->setSections(typeSecMap);
    Arg::currentTerm()->setBreaks(ui_break); ///Mark，现在这个ui_break为空.

    m_bScheduleDetailLoaded = true;

    ///Mark 切换学期必须要加载完相关数据，所以前面给Arg设置操作，都不要发送信号，最后一次发送刷新.

    if((m_bLessonDetailLoaded) && (m_bScheduleDetailLoaded)) {
        if(m_bFirstLoaded) {
            emit showMainWindow();
        }
        else {
            Arg *pArg = Arg::getInstance();
            emit pArg->termLoadFinished();
        }

        m_bFirstLoaded = false;
    }

//    qDebug()<<"detailFinished"<<endl;
}

void GetDataNW::getLessonsByUserID(int userID, int scheID) //获取课程主表id
{
    ///Mark 2017.02.13，因为信号使用比较混乱，所以这里暂时不要做判断.
//    if(lessondb == Q_NULLPTR) {
        lessondb = new LessonDB();
        connect(lessondb, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(getLessonState(const QJsonObject&)));
        connect(lessondb, SIGNAL(addFinish(const QJsonObject&)), this, SLOT(addLessonTableFinished(const QJsonObject&)));
//    }

    lessondb->setUserId(userID);
    lessondb->setScheduleId(scheID);
    lessondb->find();

    if(scheDetDB == nullptr) {
        scheDetDB = new ScheduleDetailDB();
        connect(scheDetDB, SIGNAL(findFinish(const QJsonObject&)), this, SLOT(detailFinish(const QJsonObject&)));
    }

    scheDetDB->setScheduleId(scheID);
    scheDetDB->find();
    ///End.
}
