#include "editwindow.h"
#include "titlebar.h"
#include "lessondetaildb.h"
#include "semester.h"
#include "lesson.h"
#include "term.h"
#include "coursecellwidget.h"
#include "dataclassinstancemanage.h"
#include "messagedisplaywidget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QMessageBox>

EditWindow::EditWindow(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent)
{
    m_bEditStatus = false;

    auto titleBar = new TitleBar (tr("课程编辑"), this);
    titleBar->setFixedHeight(40);
    connect (titleBar, &TitleBar::tbClose, [this] ()
    {
        this->close ();
    });

    if(parent != nullptr)
        this->cell = static_cast<CourseCellWidget*>(parent);

    ///Modified by BiXiaoxia 2016.12.22.
    int nShadowWidth = getShadowMargin();
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(nShadowWidth, nShadowWidth-10, nShadowWidth, nShadowWidth );
    ///
//    mainLayout->setSpacing (0);
    QHBoxLayout* courseLayout = new QHBoxLayout;
    courseLayout->setContentsMargins(30, 0, 30, 0);
    QHBoxLayout* locationLayout = new QHBoxLayout;
    locationLayout->setContentsMargins(30, 0, 30, 0);
    QHBoxLayout* classLayout = new QHBoxLayout;
    classLayout->setContentsMargins(30, 0, 30, 0);
    QHBoxLayout* loopLayout = new QHBoxLayout;
    loopLayout->setContentsMargins(30, 0, 30, 0);
    QHBoxLayout* setLayout = new QHBoxLayout;
    setLayout->setContentsMargins(30, 0, 30, 0);

    courseName = new QLabel("课程*");
//    courseName->setFixedHeight(50);
    courseName->setFixedWidth(60);

    locationName = new QLabel("地点*");
//    locationName->setFixedHeight(50);
    locationName->setFixedWidth(60);

    className = new QLabel("班级*");
//    className->setFixedHeight(50);
    className->setFixedWidth(60);

    QLabel *pStartDateLb = new QLabel;
    pStartDateLb->setText(tr("起始时间*"));
    pStartDateLb->setFixedWidth(60);
    m_pStartDateEdit = new QDateEdit(QDate::currentDate());
    m_pStartDateEdit->setStyleSheet("QDateEdit{border-width:2px;}");
    QHBoxLayout *pStartDateLayout = new QHBoxLayout;
    pStartDateLayout->setContentsMargins(30, 0, 30, 0);
    pStartDateLayout->addWidget(pStartDateLb);
    pStartDateLayout->addWidget(m_pStartDateEdit);

    QLabel *pEndDateLb = new QLabel;
    pEndDateLb->setText(tr("结束时间*"));
    pEndDateLb->setFixedWidth(60);
    m_pEndDateEdit = new QDateEdit(QDate::currentDate());
    m_pEndDateEdit->setStyleSheet("QDateEdit{border-width:2px;}");
    QHBoxLayout *pEndDateLayout = new QHBoxLayout;
    pEndDateLayout->setContentsMargins(30, 0, 30, 0);
    pEndDateLayout->addWidget(pEndDateLb);
    pEndDateLayout->addWidget(m_pEndDateEdit);

    loopName = new QLabel("几周循环*");
//    loopName->setFixedHeight(50);
    loopName->setFixedWidth(60);

    courseEdit = new QComboBox();
    loopEdit = new QComboBox();

    locationEdit = new QLineEdit();
    classEdit = new QLineEdit();
    sureButton = new QPushButton(tr("确定"), this);
    sureButton->setMinimumWidth(20);
    sureButton->setFixedWidth(80);
    sureButton->setFixedHeight(24);

    resetButton = new QPushButton(tr("重置"), this);
    resetButton->setMinimumWidth(20);
    resetButton->setFixedWidth(80);
    resetButton->setFixedHeight(24);
    connect(sureButton, SIGNAL(clicked(bool)), this, SLOT(sureButtonClicked()));
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButtonClicked()));

    QMap<QString, int>::iterator it;
    for (it = Arg::subjectMap.begin(); it != Arg::subjectMap.end(); it++)
    {
        courseEdit->addItem(it.key(), it.value());
    }

    /*
    courseEdit->addItem(tr("语文"));
    courseEdit->addItem(tr("数学"));
    courseEdit->addItem(tr("英语"));
    courseEdit->addItem(tr("美术"));
    courseEdit->addItem(tr("体育"));
    */

    //设置循环ui表示及代表data值.
    loopEdit->addItem(tr("每周循环"), 1);
    loopEdit->addItem(tr("2周循环"), 2);
    loopEdit->addItem(tr("3周循环"), 3);
    loopEdit->addItem(tr("4周循环"), 4);

    /*
    mainLayout->addWidget(className);
    mainLayout->addWidget(courseName);
    */


    //mainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    //mainLayout->setMargin(0);
    //mainLayout->setSpacing(0);

    courseLayout->addWidget(courseName);
    courseLayout->addWidget(courseEdit);

    locationLayout->addWidget(locationName);
    locationLayout->addWidget(locationEdit);

    classLayout->addWidget(className);
    classLayout->addWidget(classEdit);

    loopLayout->addWidget(loopName);
    loopLayout->addWidget(loopEdit);

    setLayout->addStretch(1);
    setLayout->addWidget(sureButton);
    setLayout->addSpacing(55);
    setLayout->addWidget(resetButton);
    //setLayout->addStretch(1);

    mainLayout->setSpacing(3);

    mainLayout->addWidget (titleBar);
   // mainLayout->addStretch(1);
    mainLayout->addLayout(courseLayout);
   // mainLayout->addStretch(1);
    mainLayout->addLayout(locationLayout);
   // mainLayout->addStretch(1);
    mainLayout->addLayout(classLayout);
   // mainLayout->addStretch(1);
    mainLayout->addLayout(loopLayout);

   // mainLayout->addStrut(1);
    mainLayout->addLayout(pStartDateLayout);
   // mainLayout->addStretch(1);
    mainLayout->addLayout(pEndDateLayout);
  //  mainLayout->addSpacing(20);
    mainLayout->addLayout(setLayout);
    mainLayout->addSpacing(8);

    ///Mark，暂时这么处理, 保持代码稳定先.
    setFixedSize (400, 350);
    //courseLayout->setMargin(0);
    //mainLayout->setMargin (5);


    //mainLayout->addWidget(className);
    //mainLayout->addWidget(courseName);

    //setLayout(mainLayout);
    LessonDetailDB *pLessonDetailDBPtr = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
    connect(pLessonDetailDBPtr, &LessonDetailDB::addFinish, this, &EditWindow::addLessonFinished, Qt::UniqueConnection);
    ///Mark,信号暂时这么处理.
//    Arg *pArg = Arg::getInstance();
//    connect(pLessonDetailDBPtr, &LessonDetailDB::addFinish, this, &Arg::addLessonFinished);
    connect(pLessonDetailDBPtr, &LessonDetailDB::updateFinish, this, &EditWindow::updateLessonFinished, Qt::UniqueConnection);
}

EditWindow::~EditWindow()
{

}

void EditWindow::reset()
{
    m_bEditStatus = false;
    Lesson *pLesson = cell->getLesson();

    if(pLesson != Q_NULLPTR) {
        m_pStartDateEdit->setDate(pLesson->getStartDate());
        m_pEndDateEdit->setDate(pLesson->getEndDate());
        loopEdit->setCurrentIndex(pLesson->getRepeat() - 1);
        courseEdit->setCurrentText(pLesson->subject());
        classEdit->setText(pLesson->unit());
        locationEdit->setText(pLesson->location());
//        courseEdit->setCurrentIndex(courseEdit->findData(pLesson->getClassId()));
    }
    else {
        QDate date = QDate::currentDate();
        int todayWeekDay = date.dayOfWeek();
        int lessonWeekDay = cell->getDayIndex();
        QDate startDate = date.addDays(lessonWeekDay - todayWeekDay);
        Term *pCurTerm = Arg::currentTerm();
//        QDate termStartDate = pCurTerm->getStart();
        QDate endDate = pCurTerm->getEnd();
        m_pStartDateEdit->setDate(startDate);
        m_pEndDateEdit->setDate(endDate);

        loopEdit->setCurrentIndex(0);
        courseEdit->setCurrentIndex(0);
        classEdit->setText("");
        locationEdit->setText("");
    }
}

void EditWindow::sureButtonClicked()
{
    if(courseEdit->currentText() != "" && locationEdit->text() != "" && classEdit->text() != "")
    {

        //for network use
//        LessonDetailDB* lessonDB = new LessonDetailDB();
//        LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
//        pLessonDetailDB->setStartDate(Arg::currentSemester()->getStartTime());
//        pLessonDetailDB->setEndDate(Arg::currentSemester()->getEndTime());
        Lesson *pLesson = cell->getLesson();

        if(pLesson != nullptr) //update
        {
//            qDebug()<<"current lesson is not null"<<endl;
            QDate preStartDate = pLesson->getStartDate();
            QDate preEndDate = pLesson->getEndDate();
            int nPreRepeat = pLesson->getRepeat();
            int nCurRepeat = loopEdit->currentData().toInt();

            if((nPreRepeat == nCurRepeat)
                    && (pLesson->subject() == courseEdit->currentText())
                    && (pLesson->location() == locationEdit->text())
                    && (pLesson->unit() == classEdit->text())
                    && (preStartDate == m_pStartDateEdit->date())
                    && (preEndDate == m_pEndDateEdit->date())){
                this->close();

                return;
            }

            //for ui use
//            int nReapt = loopEdit->currentData().toInt();
            pLesson->setSubject(courseEdit->currentText());
            pLesson->setLocation(locationEdit->text());
            pLesson->setUnit(classEdit->text());
            pLesson->setRepeat(nCurRepeat);
            pLesson->setStartDate(m_pStartDateEdit->date());
            pLesson->setEndDate(m_pEndDateEdit->date());
           //            pLesson->

            //for network use, update a lessonDetail
            LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
            pLessonDetailDB->setClassName(pLesson->unit());
            pLessonDetailDB->setSubject(pLesson->subject());
            pLessonDetailDB->setRoom(pLesson->location());
            pLessonDetailDB->setId(pLesson->getLessonDetailId());
            pLessonDetailDB->setLessonId(pLesson->getLessonId());
            pLessonDetailDB->setClassId(pLesson->getClassId());
            pLessonDetailDB->setRoomId(pLesson->getRoomId());
            pLessonDetailDB->setWeekId(cell->getDayIndex());
            pLessonDetailDB->setSectionId(cell->getSecIndex());
            pLessonDetailDB->setRepeat(nCurRepeat);
            pLessonDetailDB->setStartDate(m_pStartDateEdit->date());
            pLessonDetailDB->setEndDate(m_pEndDateEdit->date());

//            qDebug()<<"cell detail id: "<<lessonDB->getId()<<endl;
//            qDebug()<<"lesson detail id: "<<lessonDB->getId()<<endl;
//            connect(lessonDB, SIGNAL(updateFinish(QJsonObject)), this, SLOT(getDBState(QJsonObject)));

            m_pCurEditLesson = pLesson;
            m_bEditStatus = true;

            pLessonDetailDB->update();
        }
        else  //Add.
        {
            Arg *pArg = Arg::getInstance();
            QDate semsterStartDate = pArg->currentSemester()->getStartTime();
            QDate semesterEndDate  =pArg->currentSemester()->getEndTime();
            QDate startDate = m_pStartDateEdit->date();
            QDate endDate = m_pEndDateEdit->date();

            if(!((startDate >= semsterStartDate) && (endDate <= semesterEndDate))) {
#ifdef USER_QT_MESSAGEBOX
                QMessageBox::information(this, tr("教师客户端"), tr("请重新选择日期"));
#else
               MessageDisplayWidget::showMessage(tr("教师客户端"), tr("请重新选择日期"));
#endif

                return;
            }

            if(startDate > endDate) {

#ifdef USER_QT_MESSAGEBOX
                QMessageBox::information(this, tr("教师客户端"), tr("结束日期不能早于开始日期！"));
#else
            MessageDisplayWidget::showMessage(tr("教师客户端"), tr("结束日期不能早于开始日期！")) ;
#endif

                return;
            }

            int secIndex = cell->getSecIndex();
            int dayIndex = cell->getDayIndex();
            int weekDay = cell->getDayIndex();
            int nRepeat = loopEdit->currentData().toInt();

            QMap<QString, QList<int> > conflictMap;
            Term *pCurTerm = pArg->currentTerm();
            bool bConflict = pCurTerm->checkConflictLessons(startDate, endDate, weekDay, secIndex, nRepeat, conflictMap);
            if(bConflict) {
                QString sText = QString("与");

                for(auto itor = conflictMap.begin(); itor != conflictMap.end(); ++itor) {
                    sText.append(itor.key());
                    sText.append("在");
                    QList<int> weekList = conflictMap.value(itor.key());

                    for(int index = 0; index < weekList.size(); ++index) {
                        sText.append(QString::number(weekList.at(index)));
                        sText.append(" ");
                    }

                    sText.append("周");
                }

                sText.append("时间设置冲突");
                MessageDisplayWidget::showMessage(tr("教师客户端"), sText);

                return;
            }

            qDebug()<<"current lesson is null"<<endl;
            pLesson = new Lesson();
            Folder* folder = new Folder(tr("文件列表"), pLesson);

            //这个sectionIndex怎么可能从cell->getSecIndex里加入？.

//            QDate startDate = m_pStartDateEdit->date()/*Arg::currentTerm()->getStart()*/;
            int currentWeekIndex = Arg::currentWeekIndex();
            QDate date = semsterStartDate.addDays(currentWeekIndex * 7 + dayIndex);
            //QDate date(1997,7,17);

            ///Mark,这是base课程，所以设置date并没有多大意义.
            pLesson->setLessonId(Arg::lessonId);
            pLesson->setRootFolder(folder);
            pLesson->setSubject(courseEdit->currentText());
            pLesson->setLocation(locationEdit->text());
            pLesson->setUnit(classEdit->text());
            pLesson->setSection(secIndex);
            pLesson->setDate(date);
            pLesson->setColor(QColor(100,100,100));
            pLesson->setStartDate(startDate);
            pLesson->setEndDate(endDate);
            pLesson->setRepeat(nRepeat);
            pLesson->setWeekId(weekDay);

//            qDebug()<<"lesson: "<< "tartDate:" << semsterStartDate << "endDate:"<< endDate <<pLesson->location()<<endl;
            cell->link(pLesson);

            //for network use, add a new lessonDetail
            LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
            pLessonDetailDB->setStartDate(startDate);
            pLessonDetailDB->setEndDate(endDate);
            pLessonDetailDB->setClassName(classEdit->text());
            pLessonDetailDB->setSubject(courseEdit->currentText());
            pLessonDetailDB->setRoom(locationEdit->text());
            pLessonDetailDB->setLessonId(pLesson->getLessonId());
            pLessonDetailDB->setWeekId(weekDay);
            pLessonDetailDB->setSectionId(secIndex);
            pLessonDetailDB->setRepeat(nRepeat);
            pLessonDetailDB->setLessonId(Arg::lessonId);

            m_pCurEditLesson = pLesson;
            m_bEditStatus = true;

//            connect(lessonDB, SIGNAL(addFinish(const QJsonObject&)), this, SLOT(addState(const QJsonObject&)));
            pLessonDetailDB->add();

//            if(m_pEditEvLoop == Q_NULLPTR) {
//                m_pEditEvLoop = new QEventLoop;
//            }

//            m_pEditEvLoop->exec();
        }

        this->close();
    }
    else
    {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("课程参数不能为空"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("课程参数不能为空"));
#endif
    }

}

void EditWindow::addLessonFinished(const QJsonObject &json)
{
    qDebug()<<"add course result: "<<json<<endl;
    if(json["status"] == "false") {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("添加课程失败"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("添加课程失败"));
#endif

        return;
    }
    //    else
    //    {
    //不是该窗口编辑.
    if(!m_bEditStatus) {
        return;
    }

    m_bEditStatus = false;

    int lessonDetId = json["TLDid"].toInt();
    qDebug()<<"lessonDetId: "<<lessonDetId<<endl;
    if(m_pCurEditLesson != Q_NULLPTR) {
        m_pCurEditLesson->setLessonDetailId(lessonDetId);

        LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
        pLessonDetailDB->setId(lessonDetId);
        //    }

        Arg *pArg = Arg::getInstance();
        pArg->addLessonFinished(m_pCurEditLesson);

        m_pCurEditLesson = Q_NULLPTR;
    }
}

void EditWindow::updateLessonFinished(const QJsonObject &json)
{
    qDebug()<<"update course result: "<<json<<endl;
    if(json["status"] == "false"){
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("更新课程失败"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("更新课程失败"));
#endif
        return;
    }

    //不是该窗口编辑.
    if(!m_bEditStatus) {
        return;
    }

    m_bEditStatus = false;
    int lessonDetId = json["TLDid"].toInt();

    if(m_pCurEditLesson != Q_NULLPTR) {
        int nPreviousID = m_pCurEditLesson->getLessonDetailId();
        m_pCurEditLesson->setLessonDetailId(lessonDetId);
        LessonDetailDB *pLessonDetailDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
        pLessonDetailDB->setId(lessonDetId);

        Arg *pArg = Arg::getInstance();
        pArg->updateLessonFinished(nPreviousID, m_pCurEditLesson);

        m_pCurEditLesson = Q_NULLPTR;
    }
}

void EditWindow::getDBState(QJsonObject json)
{
    qDebug()<<"DB result: "<<json<<endl;

}

void EditWindow::resetButtonClicked()
{
    courseEdit->setCurrentIndex(0);
    classEdit->clear();
    locationEdit->clear();
}









