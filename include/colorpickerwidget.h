/**
 * BiXiaoxia Test Project 2016-08-09 version
  * 颜色拾取器，颜色拾取分两部分，第一步先选择基础颜色，根据基础颜色拾取器会绘制出颜色HSV图，再通过鼠标从HSV图中选取最终使用颜色.
  * 可以调整最终选取颜色的alpha值.
  * @author BiXiaoxia
  * @version 2016.08.09.
*/
#ifndef BASEUI_COLORPICKERDIALOG_H
#define BASEUI_COLORPICKERWIDGET_H

#include <QWidget>
#include "baseui_global.h"

namespace Ui {
class ColorPickerWidget;
}

class BASEUISHARED_EXPORT ColorPickerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorPickerWidget(QWidget *parent = 0);
    ~ColorPickerWidget();

    /*!
    @ \brief 获取当前选定的颜色，该颜色alpha值为用户调整后的.
    @ \param QColor &color, 选定的颜色值.
    */
    void getCurrentColor(QColor &color);

signals:
    /*!
    @ \brief 颜色拾取完成,此信号与鼠标选取同步，alpha值为255.
    */
    void colorSelected(const QColor &color);

private slots:
    void showSelectedColor(const QColor &selColor);

private:
    Ui::ColorPickerWidget *ui;
};

#endif // BASEUI_COLORPICKERWIDGET_H
