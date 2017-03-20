#ifndef SCHEDULEDIALOG_H
#define SCHEDULEDIALOG_H

#include "framelessmodalmovableshadowwidget.h"
#include <QMap>

class QPushButton;
class EditSche;
class QListWidget;
class EditSectime;
class Schedule;

class ScheduleDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit ScheduleDialog(QWidget *parent = 0);
    ~ScheduleDialog();

private:
    QPushButton *ButtonAdd;
    QPushButton *ButtonEdit;
    EditSche* editSche;
    EditSectime* sectimeEdit;
    QListWidget* scheList;
    QMap<QString, Schedule*> scheMap;

signals:


public slots:
    void detailFinish(const QJsonObject &);
    void reloadSchedule(Schedule*, bool);
    void closeScheduleDialog();
    void editSchedule();
    void addSchedule();
};

#endif // SCHEDULEDIALOG_H
