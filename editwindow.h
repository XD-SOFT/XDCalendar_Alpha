#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include "framelessmodalmovableshadowwidget.h"
#include <QJsonObject>

class CourseCellWidget;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QDateEdit;
class Lesson;

/// < \version BiXiaoxia 2016.12.22,重新编辑排版，带阴影可拖动效果.
class EditWindow : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    EditWindow(QWidget *parent = 0);

    void reset();

private:
    QLabel *courseName, *locationName, *className, *loopName;
    QLineEdit *classEdit, *locationEdit;
    QComboBox *courseEdit, *loopEdit;
    QPushButton *sureButton, *resetButton;
    CourseCellWidget* cell;

    QDateEdit *m_pStartDateEdit;

    QDateEdit *m_pEndDateEdit;

    Lesson* m_pCurEditLesson;

private slots:
    void addLessonFinished(const QJsonObject &json);

    void updateLessonFinished(const QJsonObject &json);

    void getDBState(QJsonObject);

    void sureButtonClicked();

    void resetButtonClicked();
};

#endif // EDITWINDOW_H
