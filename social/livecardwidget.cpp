#include "livecardwidget.h"
#include "friend.h"
#include "timediff.h"
#include "maskunclickablelabel.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QTime>
#include <QDate>
#include <QDateTime>
#include <QWidget>
#include <QDebug>

LiveCardWidget::LiveCardWidget (Friend *fr, QLabel *contentBox, QDateTime dt, QWidget *parent) : QPushButton(parent)
{
    setFlat(true);
    setFixedHeight(100);

    auto layout = new QVBoxLayout(this);

    //upper layout, the sharing's message
    auto hLayout = new QHBoxLayout();
    hLayout->setMargin(0);

    //left avatar
    auto lLayout = new QVBoxLayout();
    {
        auto avatar = new MaskUnclickableLabel(0.95, this);
        avatar->setFixedSize(30, 30);
        try{
        avatar->setPixmap(QPixmap(fr->getIconPath()).scaled(avatar->size(),Qt::KeepAspectRatioByExpanding));
        lLayout->addWidget(avatar);
        }
        catch(std::exception e){
            qDebug()<<"avatar load failure: "<<e.what()<<endl;
        }
        lLayout->addStretch(5);
    }
    hLayout->addLayout(lLayout,1);

    //right details
    auto rLayout = new QVBoxLayout();
    {
        auto nickNameLabel = new QLabel(fr->getDisplayName(),this);
        nickNameLabel->setObjectName("NickNameLabel");
        rLayout->addWidget(nickNameLabel,1);

        contentBox->setStyleSheet("background-color: #F5F5F5;padding: 10px;");
        rLayout->addWidget(contentBox,10);

        connect(fr, &Friend::dataChanged, [this, nickNameLabel](Friend *fr){
            nickNameLabel->setText(fr->getDisplayName());
        });
    }
    hLayout->addLayout(rLayout,10);
    layout->addLayout(hLayout, 10);

    //bottom layout, left: institute  right: the time from now
    auto bottomLayout = new QHBoxLayout();
    {
        bottomLayout->addWidget(new QLabel(fr->getInstituteName()));
        bottomLayout->addStretch(5);
        bottomLayout->addWidget(new QLabel(TimeDiff::Between(dt, QDateTime::currentDateTime()) + QObject::tr("前")));
    }
    layout->addLayout(bottomLayout,1);
}
