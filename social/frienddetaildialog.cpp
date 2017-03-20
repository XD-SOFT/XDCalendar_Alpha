#include "frienddetaildialog.h"
#include "friend.h"
#include "maskunclickablelabel.h"
#include "timediff.h"
#include <QDate>
#include <QDateTime>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QWidget>
#include <QDebug>

FriendDetailDialog::FriendDetailDialog(Friend *fr, QWidget *parent) : QWidget(parent){
    setWindowFlags(Qt::Dialog);

    auto layout = new QGridLayout(this);

    //avatar
    auto avatar = new MaskUnclickableLabel(0.95, this);
    avatar->setFixedSize(100, 100);
    try{
    avatar->setPixmap(QPixmap(fr->getIconPath()).scaled(avatar->size(),Qt::KeepAspectRatioByExpanding));
        layout->addWidget(avatar,1,1,1,2, Qt::AlignCenter);
    }
    catch(std::exception e){
        qDebug()<<"avatar load failure: "<<e.what()<<endl;
    }


    //details
    int tot = layout->count() + 1;
    auto add = [layout, &tot](const QString &left, const QString &right){
        layout->addWidget(new GrayTextLabel(left), tot, 1, 1, 1, Qt::AlignRight);
        layout->addWidget(new QLabel(right), tot++, 2, 1, 1, Qt::AlignLeft);
    };

    add(tr("姓名"), fr->getName());
    add(tr("备注"), fr->getNickName());
    add(tr("性别"), fr->getGenderString());
    add(tr("单位"), fr->getInstituteName());
    add(tr("注册时间"), TimeDiff::Between(QDateTime(fr->getRegDate().addDays(-1)), QDateTime(QDate::currentDate())));
    add(tr("地区"), fr->getLocation());
    add(tr("E-Mail"), fr->getEmail());
    add(tr("电话号码"), fr->getPhoneNum());
    add(tr("生日"), fr->getBirthday().toString());
}

//obsoleted
QString &FriendDetailDialog::getDetailString(Friend *fr)
{
    return *new QString(
        tr("姓名: ") +  + "\r\n" +
        tr("备注: ") + fr->getNickName() + "\r\n" +
        tr("性别: ") + fr->getGenderString() + "\r\n" +
        tr("学校: ") + fr->getInstituteName() + "\r\n" +
        tr("注册日期: ") + fr->getRegDate().toString()
    );
}
