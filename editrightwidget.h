#ifndef EDITRIGHTWIDGET_H
#define EDITRIGHTWIDGET_H

#include <QWidget>

namespace Ui {
class EditRightWidget;
}

class SemesterDialog;
class ScheduleDialog;

class EditRightWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditRightWidget(QWidget *parent = 0);
    ~EditRightWidget();

private slots:
    void on_semester_button_clicked();

    void on_schedule_button_clicked();

    void on_edit_confirm_button_clicked();

private:
    Ui::EditRightWidget *ui;
    SemesterDialog *m_pSemeWgt;
    ScheduleDialog *m_pScheduleDlg;
};

#endif // EDITRIGHTWIDGET_H
