#ifndef FRIENDCARDWIDGET_H
#define FRIENDCARDWIDGET_H

#include "friend.h"
#include <QLabel>
#include <QString>
#include <QAction>
#include <QPushButton>
#include <QVector>
#include <QLayout>
#include <QLineEdit>

class FriendDetailDialog;

class FriendCardWidget : public QPushButton
{
    Q_OBJECT
public:
    FriendCardWidget(Friend *fr, QWidget *parent=0);

private:
    void contextMenuEvent(QContextMenuEvent *event);

    Friend *mFr;
    QVector <Friend *> mList;

    QWidget *mWrapper;

    QWidget *mNameEditWidget;
    QLabel *mNameLabel;
    QLineEdit *mNameLineEdit;
    QLabel *mInstituteLabel;

    QAction *mToggleLiveAction;
    FriendDetailDialog *mDialog;
protected:

public slots:
    //ui action
    void showDetailDialog();
    void setNickNameEdit();

    // ui result receive
    void afterNickNameEditingFinish();

    //dat update
    void afterLiveStatusChanged();
    void afterDataChanged(Friend *fr);

signals:
    //activate action
    void detail();
    void rename();
    void toggleLive();

    //action finished
    void editNickNameFinished(QString);
    void removed(FriendCardWidget *);
};

#endif // FRIENDCARDWIDGET_H
