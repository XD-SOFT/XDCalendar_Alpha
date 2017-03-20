#ifndef EDITSECTIME_H
#define EDITSECTIME_H

#include "framelessmodalmovableshadowwidget.h"
//#include <QWidget>
//#include <QLabel>
//#include <QTimeEdit>
//#include <QTime>
//#include <QPushButton>
#include <QJsonObject>
//#include "schedule.h"
//#include "scheduledetaildb.h"

class Schedule;
class QLabel;
class QTimeEdit;
class ScheduleDetailDB;
class QPushButton;

class EditSectime : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit EditSectime(QWidget *parent = 0, Schedule* sche = nullptr, bool = true);
    void setDetail(QJsonObject detailJson)
    {
        this->detailJson = detailJson;
    }
    QJsonObject getDetail()
    {
        return detailJson;
    }
    void setupUI();
    void dbOperation(); //database operation

private:
    QJsonObject detailJson;
    bool add;
    int sectionNum;
    Schedule* schedule;
    QVector<QLabel*> labels;
    QVector<QString> names;
    QVector<QTimeEdit*> timeEditBegin;
    QVector<QTimeEdit*> timeEditEnd;

    QVector<ScheduleDetailDB*> scheDBList;
    //QVector<QString> timeBeginStr; //before edit
    //QVector<QString> timeEndStr; //before edit

    QVector<int> secId;
    QPushButton *sureButton, *resetButton;


signals:

public slots:
    void getSecTimeState(QJsonObject);
    void NWResult(QJsonObject);
    void closeEditTimeDialog();
    void sureButtonClicked();
    void resetButtonClicked();

};

#endif // EDITSECTIME_H
