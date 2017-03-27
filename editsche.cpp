#include "editsche.h"
#include "titlebar.h"
#include "config.h"
#include "scheduledb.h"
#include "semester.h"
#include "editsectime.h"
#include "messagedisplaywidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "schedule.h"
#include <QDateEdit>
#include <QJsonObject>


EditSche::EditSche(QWidget *parent, Schedule* sche) :
    FramelessModalMovableShadowWidget(parent),
    schedule(sche)
{
    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    auto titleBar = new TitleBar (tr("作息时间编辑"), this);
    connect (titleBar, SIGNAL(tbClose()), this, SLOT(closeEditScheDialog()));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int nShadowMargin = getShadowMargin();
    mainLayout->setContentsMargins(nShadowMargin, nShadowMargin, nShadowMargin, nShadowMargin + 9);
//    mainLayout->setMargin (0);
    mainLayout->setSpacing (0);   

    scheName = new QLabel(tr("作息时间名称"));
//    scheName->setFixedHeight(50);
//    scheName->setFixedWidth(50);

    scheEdit = new QLineEdit();

//    QHBoxLayout* scheLayout = new QHBoxLayout;
//    scheLayout->addSpacing(20);
//    scheLayout->addWidget(scheName);
//    scheLayout->addSpacing(10);
//    scheLayout->addWidget(scheEdit);
//    scheLayout->addSpacing(20);


    //开始时间设置.
    startName = new QLabel(tr("开始时间"));
//    startName->setFixedHeight(50);
//    startName->setFixedWidth(50);

    startEdit = new QDateEdit();
    startEdit->setCalendarPopup(true);

//    QHBoxLayout* startLayout = new QHBoxLayout;
//    startLayout->addSpacing(20);
//    startLayout->addWidget(startName);
//    startLayout->addSpacing(10);
//    startLayout->addWidget(startEdit);
//    startLayout->addSpacing(20);

    //结束时间.
    endName = new QLabel(tr("结束时间"));
//    endName->setFixedHeight(50);
//    endName->setFixedWidth(50);

    endEdit = new QDateEdit();
    endEdit->setCalendarPopup(true);

//    QHBoxLayout* endLayout = new QHBoxLayout;
//    endLayout->addSpacing(20);
//    endLayout->addWidget(endName);
//    endLayout->addSpacing(10);
//    endLayout->addWidget(endEdit);
//    endLayout->addSpacing(20);

    //课节数.
    sectionName = new QLabel(tr("课节数"));
//    sectionName->setFixedHeight(50);
//    sectionName->setFixedWidth(50);

    sectionEdit = new QComboBox();
    //sectionEdit->addItem(tr("5"));
    //sectionEdit->addItem(tr("6"));
    sectionEdit->addItem(tr("7"));
    sectionEdit->addItem(tr("8"));
    sectionEdit->addItem(tr("9"));

//    QHBoxLayout* sectionLayout = new QHBoxLayout;
//    sectionLayout->addSpacing(20);
//    sectionLayout->addWidget(sectionName);
//    sectionLayout->addSpacing(12);
//    sectionLayout->addWidget(sectionEdit);
//    sectionLayout->addSpacing(20);

    //适用年级.
    gradeName = new QLabel(tr("适用年级(用逗号隔开: 1,2,3)"));
//    gradeName->setFixedHeight(50);
//    gradeName->setFixedWidth(250);

    gradeEdit = new QLineEdit();

//    QHBoxLayout* gradeLayout = new QHBoxLayout;
//    gradeLayout->addSpacing(20);
//    gradeLayout->addWidget(gradeName);
//    gradeLayout->addSpacing(10);
//    gradeLayout->addWidget(gradeEdit);
//    gradeLayout->addSpacing(20);

    //
    sureButton = new QPushButton(tr("确定"), this);
    sureButton->setFixedWidth(80);
    sureButton->setMinimumHeight(20);
    connect(sureButton, SIGNAL(clicked(bool)), this, SLOT(sureButtonClicked()));
//    sureButton->setStyleSheet("QPushButton{background:#1250b5; \
//                             border:1px solid #1250b5;\
//                             color:white}");//设置按钮背景、边框颜色.

    resetButton = new QPushButton(tr("重置"), this);
    resetButton->setFixedSize(80, 20);
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButtonClicked()));
//    resetButton->setStyleSheet("QPushButton{background:#1250b5; \
//                             border:1px solid #1250b5;\
//                             color:white}");//设置按钮背景、边框颜色.

    //load date if necessary
    if(schedule != nullptr)
    {
        qDebug()<<"edit schedule is: "<<schedule->getName()<<endl;
        scheEdit->setText(schedule->getName());
        startEdit->setDate(schedule->getStartTime());
        endEdit->setDate(schedule->getEndTime());
        sectionEdit->setCurrentText(QString::number(schedule->getSection()));
        gradeEdit->setText(schedule->gradesStr());
    }

    QHBoxLayout* setLayout = new QHBoxLayout;
    setLayout->addStretch(1);
    setLayout->addWidget(sureButton);
    setLayout->addSpacing(10);
    setLayout->addWidget(resetButton);
    setLayout->addStretch(1);

    QGridLayout *pGridLayout = new QGridLayout;
    //作息时间相关加入.
    pGridLayout->addWidget(scheName, 0, 0);
    pGridLayout->addWidget(scheEdit, 0, 1);

    //开始时间加入.
    pGridLayout->addWidget(startName , 1, 0);
    pGridLayout->addWidget(startEdit , 1, 1);

    //结束时间加入.
    pGridLayout->addWidget(endName , 2, 0);
    pGridLayout->addWidget(endEdit , 2, 1);

    //课节数加入.
    pGridLayout->addWidget(sectionName , 3, 0);
    pGridLayout->addWidget(sectionEdit , 3, 1);

    //适用年级加入.
    pGridLayout->addWidget(gradeName, 4, 0);
    pGridLayout->addWidget(gradeEdit, 4, 1);
    pGridLayout->setContentsMargins(20, 5, 20, 5);
    pGridLayout->setHorizontalSpacing(10);
    pGridLayout->setVerticalSpacing(6);

    mainLayout->addWidget (titleBar, 0);
    mainLayout->addLayout(pGridLayout, 5);
//    mainLayout->addLayout(startLayout, 5);
//    mainLayout->addLayout(endLayout, 5);
//    mainLayout->addLayout(sectionLayout, 5);
//    mainLayout->addLayout(gradeLayout, 5);
    mainLayout->addStretch(1);
    mainLayout->addLayout(setLayout, 0);

//    setFixedSize (this->sizeHint ());

    add = false;

    setFixedSize(400, 300);
}

void EditSche::closeEditScheDialog()
{
    qDebug()<<"close edit sche"<<endl;
    this->close ();
}

bool EditSche::valid()
{
     if(!scheEdit->text().isEmpty() && !gradeEdit->text().isEmpty())
     {
         return true;
     }
     else
     {
         qDebug()<<"input invalid"<<endl;
         return false;
     }
}

void EditSche::editState(QJsonObject json)
{
    qDebug()<<"shedule editState: "<<json<<endl;
    if(add)
    {
        int scheID = json["id"].toInt();
        schedule->setId(scheID);
    }

}

void EditSche::sureButtonClicked()
{
    if(schedule == nullptr)
    {
        schedule = new Schedule();
        add = true;
    }

    if(valid() && schedule != nullptr)
    {
        //对于本地json而言，操作EditSche里面的schedule指针即可
        schedule->setName(scheEdit->text());
        schedule->setStartTime(startEdit->date());
        schedule->setEndTime(endEdit->date());
        schedule->setSection(sectionEdit->currentText().toInt());
        schedule->setGrades(gradeEdit->text());

        ScheduleDB* schedb = new ScheduleDB();
        schedb->setTitle(schedule->getName());
        schedb->setSection(schedule->getSection());
        schedb->setStartDate(schedule->getStartTime());
        schedb->setEndDate(schedule->getEndTime());
        schedb->setGradesStr(schedule->gradesStr());
        schedb->setTermId(Arg::currentSemester()->getId());

        if(add) //an add action
        {
            if(Arg::schedules() == nullptr)
            {
                Arg::setSchedules(new QVector<Schedule*>);
            }
            else
            {
                Arg::schedules()->append(schedule);
            }
            //invoke add api for network
            connect(schedb, SIGNAL(addFinish(QJsonObject)), this, SLOT(editState(QJsonObject)) );
            schedb->add();
        }
        else //update
        {
            connect(schedb, SIGNAL(updateFinish(QJsonObject)), this, SLOT(editState(QJsonObject)) );
            schedb->updateUserSche();
        }
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("修改成功"));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("修改成功"));
#endif
        emit updateParent(schedule, add);
        this->close();
        //对于服务器端数据库，需要构造scheduleDB并且调用该类中相应的API
    }
    else
    {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("输入不合法"));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("输入不合法"));
#endif

    }

}

void EditSche::resetButtonClicked()
{

}
