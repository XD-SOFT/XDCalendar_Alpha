#include "editsem.h"
#include "titlebar.h"
#include "config.h"
#include "termdb.h"
#include "semester.h"
#include "dataclassinstancemanage.h"
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
#include <QDateEdit>

EditSem::EditSem(QWidget *parent, Semester* sem) :
    FramelessModalMovableShadowWidget(parent),
    semester(sem)
{

    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    auto titleBar = new TitleBar (tr("学期编辑"), this);
    connect (titleBar, SIGNAL(tbClose()), this, SLOT(closeEditSemDialog()));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int nShadowMargin = getShadowMargin();
    mainLayout->setContentsMargins(nShadowMargin, nShadowMargin, nShadowMargin, nShadowMargin + 10);
    mainLayout->setSpacing (0);
    QHBoxLayout* semLayout = new QHBoxLayout;
    semLayout->setContentsMargins(20, 5, 20, 5);
    QHBoxLayout* termLayout = new QHBoxLayout;
    termLayout->setContentsMargins(20, 5, 20, 5);
    QHBoxLayout* startLayout = new QHBoxLayout;
    startLayout->setContentsMargins(20, 5, 20, 5);
    QHBoxLayout* endLayout = new QHBoxLayout;
    endLayout->setContentsMargins(20, 5, 20, 5);
    QHBoxLayout* yearLayout = new QHBoxLayout;
    yearLayout->setContentsMargins(20, 5, 20, 5);
    QHBoxLayout* setLayout = new QHBoxLayout;
    setLayout->setContentsMargins(20, 5, 20, 0);

    semName = new QLabel("学期名称");
//    semName->setFixedHeight(50);
    semName->setFixedWidth(60);

    startName = new QLabel("开始时间");
//    startName->setFixedHeight(50);
    startName->setFixedWidth(60);

    endName = new QLabel("结束时间");
//    endName->setFixedHeight(50);
    endName->setFixedWidth(60);

    yearName = new QLabel("所属学年");
//    yearName->setFixedHeight(50);
    yearName->setFixedWidth(60);

    termName = new QLabel("所属学期");
//    termName->setFixedHeight(50);
    termName->setFixedWidth(60);

    startEdit = new QDateEdit();
    startEdit->setCalendarPopup(true);
    endEdit = new QDateEdit();
    endEdit->setCalendarPopup(true);

    yearEdit = new QComboBox();
    termEdit = new QComboBox();

    semEdit = new QLineEdit();


    sureButton = new QPushButton(tr("确定"), this);
    sureButton->setFixedSize(80, 20);
    resetButton = new QPushButton(tr("重置"), this);
    resetButton->setFixedSize(80, 20);
    connect(sureButton, SIGNAL(clicked(bool)), this, SLOT(sureButtonClicked()));
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButtonClicked()));

    yearEdit->addItem(tr("2017"));
    yearEdit->addItem(tr("2018"));
    yearEdit->addItem(tr("2019"));
    yearEdit->addItem(tr("2020"));
    yearEdit->addItem(tr("2021"));

    termEdit->addItem(tr("第一学期"));
    termEdit->addItem(tr("第二学期"));

    //load date if necessary
    if(semester != nullptr)
    {
        qDebug()<<"edit semester is: "<<semester->getTitle()<<" starttime: "<<semester->getStartTime().toString("yyyy-mm-dd")<<endl;
        semEdit->setText(semester->getTitle());
        startEdit->setDate(semester->getStartTime());
        endEdit->setDate(semester->getEndTime());
        yearEdit->setCurrentText(QString::number(semester->getYearStart()));
        termEdit->setCurrentText(semester->getTerm());
    }


    semLayout->addWidget(semName);
    semLayout->addWidget(semEdit);

    termLayout->addWidget(termName);
    termLayout->addWidget(termEdit);

    startLayout->addWidget(startName);
    startLayout->addWidget(startEdit);

    endLayout->addWidget(endName);
    endLayout->addWidget(endEdit);

    yearLayout->addWidget(yearName);
    yearLayout->addWidget(yearEdit);

    setLayout->addStretch(1);
    setLayout->addWidget(sureButton);
    setLayout->addStretch(1);
    setLayout->addWidget(resetButton);
    setLayout->addStretch(1);

    mainLayout->addWidget (titleBar, 1);
    mainLayout->addLayout(semLayout, 1);
    mainLayout->addLayout(termLayout, 1);
    mainLayout->addLayout(startLayout, 1);
    mainLayout->addLayout(endLayout, 1);
    mainLayout->addLayout(yearLayout, 1);
    mainLayout->addStretch(1);
    mainLayout->addLayout(setLayout, 1);

    setFixedSize (400, 300);

    add = false;


    //initilize termdb.
    DataClassInstanceManage *pInstance = DataClassInstanceManage::getInstance();
    TermDB *pTermDB = pInstance->getTemDBPtr();
    connect(pTermDB, &TermDB::addTermFinished, this, &EditSem::editState);
    connect(pTermDB, &TermDB::updateTermFinished, this, &EditSem::editState);
}

void EditSem::closeEditSemDialog()
{
    qDebug()<<"close edit sem"<<endl;
    this->close ();
}

bool EditSem::valid()
{
     if(!semEdit->text().isEmpty())
     {
         return true;
     }
     else
     {
         qDebug()<<"input invalid"<<endl;
         return false;
     }
}

void EditSem::editState(const QJsonObject &json)
{
    qDebug()<<"semester editState: "<<json<<endl;
}

void EditSem::sureButtonClicked()
{
    if(semester == nullptr)
    {
        semester = new Semester();
        add = true;
    }

    if(valid() && semester != nullptr)
    {
        //对于本地json而言，操作EditSem里面的semester指针即可
        semester->setTitle(semEdit->text());
        semester->setTerm(termEdit->currentText());
        semester->setStartTime(startEdit->date());
        semester->setEndTime(endEdit->date());
        semester->setYearStart(yearEdit->currentText().toInt());      

//        TermDB* termdb = new TermDB();
//        connect(termdb, SIGNAL(addFinish(QJsonObject)), this, SLOT(editState(QJsonObject)) );
//        connect(termdb, SIGNAL(updateFinish(QJsonObject)), this, SLOT(editState(QJsonObject)) );
        TermDB *pTermDB = DataClassInstanceManage::getInstance()->getTemDBPtr();
        pTermDB->setTitle(semester->getTitle());
        pTermDB->setTerm(semester->getTerm());
        pTermDB->setStartTime(semester->getStartTime());
        pTermDB->setEndTime(semester->getEndTime());
        pTermDB->setYearStrat(QString::number(semester->getYearStart()));
        pTermDB->setOwnership(Arg::userId); //here is the userid

        if(add)
        {
            if(Arg::semesters() == nullptr)
            {
                Arg::setSemesters(new QVector<Semester*>);
            }
            else
            {
                Arg::semesters()->append(semester);
            }
            //invoke add api
            pTermDB->add();
        }
        else
        {
            pTermDB->setId(semester->getId());
            pTermDB->updateUserTerm();
        }

#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("修改成功"));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("修改成功"));
#endif
        updateParent();
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

void EditSem::resetButtonClicked()
{

}
