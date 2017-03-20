#include "userlistwidget.h"
#include "social/animationstackedwidget.h"
#include "social/friend.h"
#include "social/friendcardwidget.h"
#include "social/friendlistwidget.h"
#include "social/livelistwidget.h"
#include "social/livemessage.h"
#include "social/animationstackedwidget.h"
#include "searchbox.h"
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QScrollArea>
#include <QDebug>
#include <QScrollBar>

UserListWidget::UserListWidget(QWidget *parent) : QWidget(parent)
{   
    auto wrapper = new QVBoxLayout(this);
    wrapper->setSpacing(0);
    wrapper->setMargin(0);

    //set stacked widget
    mStackedWidget = new AnimationStackedWidget();
    mStackedWidget->setDuration(500);
    wrapper->addWidget(mStackedWidget);

    //back button for friend sharing interface
    mBackButton = new QPushButton(tr("< 返回"), this);
    mBackButton->hide();
    connect(mBackButton, &QPushButton::clicked, this, &UserListWidget::changeToFriendList);
    wrapper->addWidget(mBackButton);

    //temporary, fetch friend data
    QVector <Friend *> test;
    test.push_back(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)));
    test.last()->setGender(2);
    test.push_back(new Friend(1, "Bernie Sanders", "nowhere", ":/Icon/1163593.png", QDate(2016,11,4)));
    test.push_back(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Bernie Sanders", "nowhere", ":/Icon/1163593.png", QDate(2016,11,4)));
    test.push_back(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)));
    test.last()->setGender(2);
    test.push_back(new Friend(1, "Bernie Sanders", "nowhere", ":/Icon/1163593.png", QDate(2016,11,4)));
    test.push_back(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)));
    test.push_back(new Friend(1, "Bernie Sanders", "nowhere", ":/Icon/1163593.png", QDate(2016,11,4)));

    //add view
    auto widget = new FriendListWidget(test);
    connect(widget, &FriendListWidget::enterDetail, this, &UserListWidget::changeToFriend);
    mStackedWidget->addWidget(widget);
    mStackedWidget->setCurrentIndex(0);
}

//show a friend's sharings
void UserListWidget::changeToFriend(Friend *fr)
{
    //temporary, fetch sharings of a friend
    if(fr->getUid() == 1){
        QVector<LiveMessage *> test;

        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
        test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));

        //delete last friend's sharing view
        if(mWidget != nullptr){
            mStackedWidget->removeWidget(mWidget);
            delete mWidget;
        }

        //slide to friend's sharing
        mWidget = new LiveListWidget(test);
        mBackButton->show();
        mStackedWidget->addWidget(mWidget);
        mStackedWidget->setLength(200, AnimationStackedWidget::AnimationType::RightToLeft);
        mStackedWidget->start(1);
        //mArea->verticalScrollBar()->setValue(0);
    }
}

void UserListWidget::changeToFriendList()
{
    //slide to friend list
    mBackButton->hide();
    mStackedWidget->setLength(200, AnimationStackedWidget::AnimationType::LeftToRight);
    mStackedWidget->start(0);
    //mArea->verticalScrollBar()->setValue(0);

    //mStackedWidget->removeWidget(mWidget);
}
