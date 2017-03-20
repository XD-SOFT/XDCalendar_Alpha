#ifndef FRIENDLISTWIDGET_H
#define FRIENDLISTWIDGET_H

#include "friend.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QVector>

class FriendListWidget : public QWidget
{
    Q_OBJECT
public:
    FriendListWidget(const QVector <Friend *> &friends, QWidget *parent = nullptr);

    virtual ~FriendListWidget();
private:
    void addItem(QWidget *wg);
    void addFriend(Friend *fr);

    QVBoxLayout *mLayout;
signals:
    void enterDetail(Friend *fr);  //switch to sharings of a friend
public slots:
    void removeWidget(QWidget *wg);
};

#endif // FRIENDLISTWIDGET_H
