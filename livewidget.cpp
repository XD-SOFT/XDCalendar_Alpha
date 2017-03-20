#include "livewidget.h"
#include "social/livecardwidget.h"
#include "social/friend.h"
#include "social/timediff.h"
#include "searchbox.h"
#include "social/livemessage.h"
#include "social/livelistwidget.h"
#include <QVBoxLayout>
#include <QDateTime>
#include <QDebug>
#include <QScrollArea>


LiveWidget::LiveWidget(QWidget* parent): QWidget (parent)
{
    auto wrapper = new QVBoxLayout(this);
    wrapper->setSpacing(0);
    wrapper->setMargin(0);

    //fetch data (temporary)
    QVector <LiveMessage *> test;
    test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)),"特朗普的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)),"希拉里的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)),"特朗普的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)),"希拉里的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "奥巴马","白宫",":/Icon/1201322.png", QDate(2016,11,2)),"奥巴马的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Donald Trump","Trump Tower",":/Icon/11666.png", QDate(2016,11,2)),"特朗普的消息",QDateTime(QDate(2016,11,4))));
    test.push_back(new LiveMessage(new Friend(1, "Hillary Clinton","In Jail",":/Icon/1185785.png", QDate(2016,11,2)),"希拉里的消息",QDateTime(QDate(2016,11,4))));

    //add view
    wrapper->addWidget(new LiveListWidget(test));

    mBackButton = new QPushButton(tr("< 返回"), this);
    wrapper->addWidget(mBackButton);
    mBackButton->hide();
}
