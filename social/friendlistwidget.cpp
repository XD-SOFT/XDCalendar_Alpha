#include "friendlistwidget.h"
#include "friendcardwidget.h"
#include "friend.h"
#include "searchbox.h"
#include <QVector>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>

FriendListWidget::FriendListWidget(const QVector <Friend *> &friends, QWidget *parent) : QWidget(parent)
{
    //a scroll area for friend list
    auto wrapper = new QVBoxLayout(this);
    wrapper->setMargin(0);
    wrapper->setSpacing(0);

    auto area = new QScrollArea();
    area->setWidgetResizable(true);
    wrapper->addWidget(area);

    auto widget = new QWidget();
    widget->setAutoFillBackground(false);
    area->setWidget(widget);

    mLayout = new QVBoxLayout (widget);
    mLayout->setSpacing(0);
    mLayout->setMargin(0);

    //add components
    mLayout->addWidget(new SearchBox());

    mLayout->addStretch(5);

    for(auto ptr : friends){
        addFriend(ptr);
    }
}


void FriendListWidget::removeWidget(QWidget *wg)
{
    mLayout->removeWidget(wg);
    delete wg;
}

void FriendListWidget::addItem(QWidget *wg)
{
    mLayout->insertWidget(mLayout->count()-1, wg);
}

void FriendListWidget::addFriend(Friend *fr)
{
    auto newFriendWidget = new FriendCardWidget(fr);
    connect(newFriendWidget, &FriendCardWidget::removed, this, &FriendListWidget::removeWidget);
    connect(newFriendWidget, &FriendCardWidget::clicked, [this, fr](){
       emit enterDetail(fr);
    });
    addItem(newFriendWidget);
}

FriendListWidget::~FriendListWidget()
{
//    qDebug()<<"deleteing friendlistwidget"<<endl;
}
