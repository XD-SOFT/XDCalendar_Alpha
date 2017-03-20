#ifndef LIVECARDWIDGET_H
#define LIVECARDWIDGET_H

#include "friend.h"
#include <QTime>
#include <QDateTime>
#include <QLabel>
#include <QWidget>
#include <QPushButton>

class LiveCardWidget : public QPushButton
{
Q_OBJECT
public:
    LiveCardWidget(Friend *fr, QLabel *contentBox, QDateTime dt, QWidget *parent=0);
};

#endif // LIVECARDWIDGET_H
