#include "friend.h"
#include <QString>
#include <QDate>
#include <QDebug>

void Friend::changeNickName(QString name)
{
    this->setNickName(name);
    emit dataChanged(this);
}

QString Friend::getThirdPersonPronoun(){
    int &&res = getGender();
    if(res == 2)
        return tr("她");
    else if(res == 0x10000000)
        return tr("虵");
    else
        return tr("他");
}

QString Friend::getLiveStatusToggleString(){
    if(!getLiveStatus())
        return tr("看") + getThirdPersonPronoun() + tr("的分享");
    else
        return tr("不看") + getThirdPersonPronoun() + tr("的分享");
}

QString Friend::getStatusString(){
    int tmp = getStatus();
    switch(tmp){
        case 1:
            return tr("在线");
        default:
            return tr("离线");
    }
}

void Friend::changeLiveStatusAlter()
{
    qDebug()<<getDisplayName()<<' '<<getLiveStatus()<<endl;
    setLiveStatus(!getLiveStatus());
    emit liveStatusChanged();
}
