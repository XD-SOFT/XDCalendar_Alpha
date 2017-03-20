#ifndef SEMESTERDIALOG_H
#define SEMESTERDIALOG_H

#include "framelessmodalmovableshadowwidget.h"
#include <QMap>

class QVBoxLayout;
class QPushButton;
class EditSem;
class QListWidget;
class Semester;

class SemesterDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    explicit SemesterDialog(QWidget *parent = 0);
    ~SemesterDialog();  

signals:

public slots:
    void reloadSemester();
    void closeSemesterDialog();
    void editSemester();
    void addSemester();

private:
    QVBoxLayout* contentLayout;
    QPushButton *ButtonAdd;
    QPushButton *ButtonEdit;
    EditSem* editSem;
    QListWidget* semList;
    QMap<QString, Semester*> semMap;
};

#endif // SEMESTERDIALOG_H
