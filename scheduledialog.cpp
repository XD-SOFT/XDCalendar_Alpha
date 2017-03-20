#include "scheduledialog.h"
#include "titlebar.h"
#include "config.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>

#include "semester.h"
#include "schedule.h"
#include "scheduledetaildb.h"
#include "editsche.h"
#include "editsectime.h"

ScheduleDialog::ScheduleDialog(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent)
{

    qDebug()<<"come in scheduleDialog"<<endl;
    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    setWindowModality (Qt::WindowModal);
//    setAttribute (Qt::WA_DeleteOnClose);

    //this->setStyle("cleanlooks");
    int nShadowWidth = getShadowMargin();
    auto layout = new QVBoxLayout (this);
    layout->setContentsMargins (nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);

    auto titleBar = new TitleBar (QStringLiteral ("作息时间设置"), this);
    connect (titleBar, SIGNAL(tbClose()), this, SLOT(closeScheduleDialog()));
    layout->addWidget (titleBar, 1);

    auto contentLayout = new QVBoxLayout ();
    contentLayout->setContentsMargins (20, 10, 20, 9);
    {
        contentLayout->addWidget(new QLabel(QStringLiteral("<font pointsize=10 color=#666666>当前作息时间:</font>"),this),5);
        if(Arg::currentSchedule() != nullptr)
        {
            contentLayout->addWidget(new QLabel(Arg::currentSchedule()->getName(),this),5);
        }
        else
        {
            contentLayout->addWidget(new QLabel(tr("未设置"),this),5);
        }

        contentLayout->addSpacing(10);
        auto label1 = new QLabel(QStringLiteral ("<font pointsize=10 color=#666666>我保存的作息时间:</font>"), this);
        contentLayout->addWidget (label1, 5);

        scheList = new QListWidget (this);
        contentLayout->addWidget(scheList);

        if(Arg::schedules() != nullptr)
        {
            if(!Arg::schedules()->isEmpty())
            {
                for(Schedule *s : *(Arg::schedules())){
                    QListWidgetItem *pItem = new QListWidgetItem;
                    pItem->setSizeHint(QSize(scheList->width(), 25));
                    QString sName = s->getName();
                    scheList->addItem(sName);
                    scheMap[sName] = s;
                }
            }
        }

        auto buttonLayout = new QHBoxLayout();
        {
            buttonLayout->addStretch(1);
            ButtonAdd = new QPushButton(QObject::tr("新增"));
            ButtonAdd->setMinimumHeight(20);
            ButtonAdd->setFixedWidth(80);
            connect (ButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addSchedule()));
            ButtonAdd->setStyleSheet("QPushButton{background:#1250b5; \
                                     border:1px solid #1250b5;\
                                     color:white}");//设置按钮背景、边框颜色.

            ButtonEdit = new QPushButton(QObject::tr("编辑"));
            ButtonEdit->setMinimumHeight(20);
            ButtonEdit->setFixedWidth(80);
            connect (ButtonEdit, SIGNAL(clicked(bool)), this, SLOT(editSchedule()));
            ButtonEdit->setStyleSheet("QPushButton{background:#1250b5; \
                                      border:1px solid #1250b5;\
                                      color:white}");//设置按钮背景、边框颜色.

            buttonLayout->addWidget(ButtonAdd);
            buttonLayout->addSpacing(10);
            buttonLayout->addWidget(ButtonEdit);
            buttonLayout->addStretch(1);
        }
        contentLayout->addLayout(buttonLayout);
    }
    layout->addLayout (contentLayout);

    setFixedSize(400, 300);
}

void ScheduleDialog::editSchedule()
{

    QListWidgetItem* currentSche;
    if(scheList->currentItem() != nullptr)
    {
        currentSche = scheList->currentItem();
    }
    else
    {
        currentSche = scheList->item(0);
    }
    Arg::setCurrentSchedule(scheMap[currentSche->text()]);
    qDebug()<<"current item: "<<currentSche->text()<<endl;
    qDebug()<<"sche name: "<<scheMap[currentSche->text()]->getName()<<endl;
    editSche = new EditSche(this, scheMap[currentSche->text()]);
    connect(editSche, SIGNAL(updateParent(Schedule*, bool)), this, SLOT(reloadSchedule(Schedule*, bool)));
    editSche->show();
}

void ScheduleDialog::reloadSchedule(Schedule* sche, bool add)
{
    qDebug()<<"reload schedule"<<endl;
    scheList->clear();
    scheMap.clear();
    for(Schedule *s : *(Arg::schedules())){
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setSizeHint(QSize(scheList->width(), 25));
        QString sName = s->getName();
        scheList->addItem(sName);
        scheMap[sName] = s;
    }

    //弹出作息时间详细表dialog
    sectimeEdit = new EditSectime(this, sche, add);
    if(add)
    {
        sectimeEdit->show();
    }
    else
    {
        ScheduleDetailDB* schedb = new ScheduleDetailDB();
        schedb->setScheduleId(sche->getId());
        connect(schedb, SIGNAL(findFinish(QJsonObject)), this, SLOT(detailFinish(QJsonObject)));
        schedb->find();
    }

}

void ScheduleDialog::detailFinish(const QJsonObject &json)
{
    qDebug()<<"Detail result: "<<json<<endl;
    sectimeEdit->setDetail(json);
    sectimeEdit->show();
}

void ScheduleDialog::addSchedule()
{
    editSche = new EditSche(this, nullptr);
    connect(editSche, SIGNAL(updateParent(Schedule*, bool)), this, SLOT(reloadSchedule(Schedule*, bool)));
    editSche->show();
}

void ScheduleDialog::closeScheduleDialog()
{
    qDebug()<<"close sche dialog"<<endl;
    this->close ();
}


ScheduleDialog::~ScheduleDialog()
{
}
