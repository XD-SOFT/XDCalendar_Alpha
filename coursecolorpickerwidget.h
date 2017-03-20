/** XDCalendar Project 2016-12-28 version
  * 颜色选取窗口.
  *
  * @author BiXiaoxia
  * @version 2016.12.28.
*/
#ifndef XDSOFT_COURSECOLORPICKERWIDGET_H
#define XDSOFT_COURSECOLORPICKERWIDGET_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class CourseColorPickerWidget;
}

class CourseColorPickerWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    explicit CourseColorPickerWidget(QWidget *parent = 0);
    ~CourseColorPickerWidget();

signals:
    void colorSelectComplete(const QColor &color);

    void cancelSelectedColor();

private slots:
    void on_pConfirmBtn_clicked();

    void on_pCancelBtn_clicked();

private:
    Ui::CourseColorPickerWidget *ui;
};

#endif // XDSOFT_COURSECOLORPICKERWIDGET_H
