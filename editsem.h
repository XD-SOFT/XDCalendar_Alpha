#ifndef EDITSEM_H
#define EDITSEM_H

#include "framelessmodalmovableshadowwidget.h"
//#include <QWidget>
//#include <QComboBox>
//#include <QLabel>
//#include <QLineEdit>
//#include <QPushButton>
//#include "semester.h"
//#include <QDateEdit>
#include <QJsonObject>

class QLabel;
class QLineEdit;
class QComboBox;
class QDateEdit;
class QPushButton;
class Semester;

class EditSem : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit EditSem(QWidget *parent = 0, Semester* sem = nullptr);
    bool valid(); //检查用户输入数据的合法性

private:
    QLabel *semName, *startName, *endName, *yearName, *termName;
    QLineEdit *semEdit;
    QComboBox *yearEdit, *termEdit;
    QDateEdit *startEdit, *endEdit;
    QPushButton *sureButton, *resetButton;

    Semester* semester;

    bool add; //record if an add action

signals:
    void updateParent();

public slots:
    void editState(const QJsonObject &json);
    void closeEditSemDialog();
    void sureButtonClicked();
    void resetButtonClicked();

};

#endif // EDITSEM_H
