#ifndef LIVELISTWIDGET_H
#define LIVELISTWIDGET_H

#include <QScrollArea>
#include <QVector>
#include <QPushButton>

class LiveMessage;

class LiveListWidget : public QWidget
{
    Q_OBJECT
public:
    LiveListWidget(QVector <LiveMessage *> &messages, QWidget *parent = nullptr);

};

#endif // LIVELISTWIDGET_H
