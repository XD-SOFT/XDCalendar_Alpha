#ifndef USERLISTWIDGET_H
#define USERLISTWIDGET_H

#include "social/friend.h"
#include "social/animationstackedwidget.h"
#include <QWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QLayout>
#include <QStackedWidget>

class Friend;

class UserListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserListWidget(QWidget *parent = 0);

private:
    QScrollArea *mArea;
    QVBoxLayout *mLayout;
    QWidget *mDialog;
    QWidget *mWidget = nullptr;
    QPushButton *mBackButton; //a back button for friend's sharings interface
    AnimationStackedWidget *mStackedWidget;
    QWidget *mFriendOnly;
signals:
public slots:
    void changeToFriend(Friend *fr);
    void changeToFriendList();
};

#endif // USERLISTWIDGET_H
