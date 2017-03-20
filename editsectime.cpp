#include "editsectime.h"
#include "titlebar.h"
#include "schedule.h"
#include "scheduledetaildb.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QTimeEdit>
#include <QTime>
#include <QPushButton>
#include <QJsonArray>
#include <QDebug>

EditSectime::EditSectime(QWidget *parent, Schedule* sche, bool flag) :
    FramelessModalMovableShadowWidget(parent),
    schedule(sche),
    add(flag)
{

    qDebug()<<"come in editsectime"<<endl;
    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);

    sectionNum = schedule->getSection();    

    setupUI();

    ScheduleDetailDB* schedb = new ScheduleDetailDB();

//    schedb = new ScheduleDetailDB();
    schedb->setScheduleId(schedule->getId());
    connect(schedb, SIGNAL(findFinish(QJsonObject)), this, SLOT(getSecTimeState(QJsonObject)));
    schedb->find();

}

void EditSectime::setupUI()
{
    auto titleBar = new TitleBar (tr("课节时间编辑"), this);
    connect (titleBar, SIGNAL(tbClose()), this, SLOT(closeEditTimeDialog()));

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int nShadowMargin = getShadowMargin();
    mainLayout->setContentsMargins(nShadowMargin, nShadowMargin, nShadowMargin, nShadowMargin + 9);
    mainLayout->setSpacing (6);

    for(int i = 0; i < sectionNum; i++)
    {
        QString beginStr = QString(tr("第"));
        QString middleStr = QString::number(i+1);
        QString endStr = QString(tr("节课"));
        QString nameStr = beginStr + middleStr + endStr;
        qDebug()<<"i: "<<i<<" name: "<<nameStr<<endl;
        names.append(nameStr);

        QLabel* label = new QLabel(beginStr + middleStr + endStr);
        label->setFixedWidth(150);
        label->setFixedHeight(50);
        labels.append(label);

        QTime* time1 = new QTime(7,30);
        QTime* time2 = new QTime(8,15);
        QTimeEdit* timeEdit1 = new QTimeEdit(time1->addSecs(i*60*45 + i*15*60), this);
        QTimeEdit* timeEdit2 = new QTimeEdit(time2->addSecs(i*60*45 + i*15*60), this);

        timeEdit1->setFixedHeight(50);
        timeEdit1->setFixedWidth(150);

        timeEdit2->setFixedHeight(50);
        timeEdit2->setFixedWidth(150);

        timeEditBegin.append(timeEdit1);
        timeEditEnd.append(timeEdit2);
        secId.append(i);
    }

    mainLayout->addWidget(titleBar, 1);

    for(int i = 0; i < labels.size(); i++)
    {
        QHBoxLayout* secLayout = new QHBoxLayout;
        secLayout->addWidget(labels[i]);
        secLayout->addWidget(timeEditBegin[i]);
        secLayout->addWidget(timeEditEnd[i]);

        mainLayout->addLayout(secLayout, 10);
    }

    QHBoxLayout* buttonLayout = new QHBoxLayout;

    sureButton = new QPushButton(tr("确定"), this);
    resetButton = new QPushButton(tr("重置"), this);
    connect(sureButton, SIGNAL(clicked(bool)), this, SLOT(sureButtonClicked()));
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButtonClicked()));
    buttonLayout->addWidget(sureButton);
    buttonLayout->addWidget(resetButton);
    mainLayout->addLayout(buttonLayout);

    setFixedSize (this->sizeHint ());
    qDebug()<<"setupui editsectime"<<endl;

}

void EditSectime::resetButtonClicked()
{

}

void EditSectime::sureButtonClicked()
{
    QTime noon(12,0);
    QTime evening(17,0);
    for (int i = 0; i < timeEditBegin.size(); i++)
    {
        qDebug() <<"i: " << i <<" " <<timeEditBegin[i]->time()<<endl;

        ScheduleDetailDB* schedb = new ScheduleDetailDB();
        schedb->setScheduleId(schedule->getId());
        schedb->setName(names[i]);

        ///Mark 2017.02.16,一律改为从0开始.
        schedb->setSectionId(i);
//        schedb->setSectionId(i+1);
        schedb->setStartTime(timeEditBegin[i]->time().toString("hh:mm"));
        schedb->setEndTime(timeEditEnd[i]->time().toString("hh:mm"));
        if(timeEditBegin[i]->time() < noon) //上午课程
        {
            schedb->setType(1);
        }
        else if (timeEditBegin[i]->time() > noon && timeEditBegin[i]->time() < evening) //下午课程
        {
            schedb->setType(2);
        }
        else //晚上课程
        {
            schedb->setType(3);
        }
        scheDBList.append(schedb);
    } 

    dbOperation();
    this->close();

}

void EditSectime::getSecTimeState(QJsonObject json)
{
    qDebug()<<"------------------"<<endl;
    qDebug()<<json<<endl;
    qDebug()<<"sectime: "<<json["status"]<<endl;
    if(json["status"] == "false") return;
    QJsonArray secArray = json["result"].toArray();

    QVector<int> idV;
    QVector<QString> nameV;
    QVector<QString> startV;
    QVector<QString> endV;

    qDebug()<<"array size: "<<secArray.size()<<endl;
    if(secArray.size() == 0)
    {
        return;
    }

    for (auto sec : secArray)
    {
        int id = sec.toObject()["id"].toInt();
        QString starttime = sec.toObject()["starttime"].toString();
        QString endtime = sec.toObject()["endtime"].toString();
        QString name = sec.toObject()["name"].toString();

        idV.append(id);
        nameV.append(name);
        startV.append(starttime);
        endV.append(endtime);

        qDebug()<<"secArray: "<<id<<endl;
    }

    if(secArray.size() != sectionNum && !add)
    {
        qDebug()<<"locad data conflict with internet data"<<endl;
        return;
    }
    else //reload local data
    {
        qDebug()<<"secId size: "<<secId.size()<<" timeEditBegin size: "<<timeEditBegin.size()<<" timeEditEnd: "<<timeEditEnd<<endl;

        if(secId.size() != names.size() != timeEditBegin.size() != timeEditEnd.size() != idV.size() != nameV.size())
        {
            qDebug()<<"locad data conflict with internet data"<<endl;
            return;
        }
        for (int i = 0; i < secId.size(); i++)
        {
            qDebug()<<"reload i:"<<i<<endl;
            //timeBeginStr[i] = timeEditBegin[i]->text();
            //timeEndStr[i] = timeEditBegin[i]->text();

            secId[i] = idV[i];
            names[i] = nameV[i];
            timeEditBegin[i]->setTime(QTime::fromString(startV[i], "hh:mm:ss"));
            timeEditEnd[i]->setTime(QTime::fromString(endV[i], "hh:mm:ss"));
        }

    }
}

void EditSectime::dbOperation()
{
    qDebug()<<"===========dbOperation==============="<<endl;

    if(add)
    {
        qDebug()<<"add detail schedule section"<<endl;
        for (int i = 0; i < scheDBList.size(); i++)
        {
            scheDBList[i]->add();
            connect(scheDBList[i], SIGNAL(addFinish(QJsonObject)), this, SLOT(NWResult(QJsonObject)));
        }
    }
    else
    {
        qDebug()<<"---------update schedetail db---------"<<endl;
        ScheduleDetailDB* schedb = new ScheduleDetailDB();
        schedb->setScheduleId(schedule->getId());
        connect(schedb, SIGNAL(delFinish(QJsonObject)), this, SLOT(NWResult(QJsonObject)));
        schedb->del();


        for (int i = 0; i < scheDBList.size(); i++)
        {
            scheDBList[i]->add();
            connect(scheDBList[i], SIGNAL(addFinish(QJsonObject)), this, SLOT(NWResult(QJsonObject)));
        }

    }


}

void EditSectime::NWResult(QJsonObject json)
{
    qDebug()<<"network result: "<<json<<endl;
}

void EditSectime::closeEditTimeDialog()
{
    qDebug()<<"close edit sectime"<<endl;
    this->close ();
}

