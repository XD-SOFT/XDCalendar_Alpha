#ifndef FRIENDDETAILDIALOG_H
#define FRIENDDETAILDIALOG_H

#include "friend.h"
#include <QWidget>
#include <QLabel>

class GrayTextLabel : public QLabel{
Q_OBJECT
public:
    GrayTextLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(text, parent, f){}
};

class FriendDetailDialog : public QWidget
{
Q_OBJECT
public:
    FriendDetailDialog(Friend *fr, QWidget *parent = NULL);
private:
    //obsolete
    QString &getDetailString(Friend *fr);

    QLabel *mLabel;
};

#endif // FRIENDDETAILDIALOG_H
