#ifndef EDITSCHE_H
#define EDITSCHE_H

#include "framelessmodalmovableshadowwidget.h"
//#include <QWidget>
//#include <QComboBox>
//#include <QLabel>
//#include <QLineEdit>
//#include <QPushButton>
//#include "schedule.h"
//#include <QDateEdit>
#include <QJsonObject>
//#include "editsectime.h"

class Schedule;
class QLabel;
class QLineEdit;
class QComboBox;
class QDateEdit;
class EditSectime;
class QPushButton;

class EditSche : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit EditSche(QWidget *parent = 0, Schedule* sche = nullptr);

    bool valid(); //检查用户输入数据的合法性
    //void setSchedule(Schedule* schedule) { this->schedule = schedule; }


    //QDateEdit date = new QDateEdit();
    //date->setCalendarPopup(true);
private:
    QLabel *scheName, *startName, *endName, *sectionName, *gradeName;
    QLineEdit *scheEdit, *gradeEdit;
    QComboBox *sectionEdit;
    QDateEdit *startEdit, *endEdit;
    QPushButton *sureButton, *resetButton;

    Schedule* schedule;
    EditSectime* sectimeEdit;
    bool add; //used to record if an add action

signals:
    void updateParent(Schedule*, bool);
public slots:
    void editState(QJsonObject);
    void closeEditScheDialog();
    void sureButtonClicked();
    void resetButtonClicked();
};

#endif // EDITSCHE_H
