#ifndef DAYWIDGET_H
#define DAYWIDGET_H

#include <QDate>
#include <QtWidgets>
#include "mybutton.h"
#include "database.h"
#include "config.h"

//日期单元格中显示事件的部分
class TextShowWindow : public QFrame
{
    Q_OBJECT
public:
    friend class DayWidget;
    TextShowWindow(QWidget *parent);
    void reset(DailyData *data);
    QWidget *MainWidget;//所有的事件方块都放入此QWidget,与主界面不建立布局关系，手动布局遮罩


protected:
    void resizeEvent(QResizeEvent *event);
private:
    DailyData *data;//从父对象DayWidget继承
    QSlider *verticalSlider;

    QVBoxLayout *MainLayout;//MainWidget内部对每个事件方块的布局
    EventTextButton* addEvent(EventData *e);//根据事件信息构造按钮块
    void clear(){}
public slots:
    void NewEvent();
    void resetSize()
    {
        MainWidget->resize(this->width(),data->text.size()*Const::MyButtonSize);
    }
};

//日期单元格
class DayWidget : public QFrame
{
    Q_OBJECT

private:
    QGridLayout *MainGridLayout;
    QWidget *DownWidget;//下部长条
    TextShowWindow *EventWidget;//事件窗(中部最大的彩色窗口)
    QHBoxLayout *DownHLayout;//下部长条布局
    DragMenu *FileMenu;
    void reset();//根据当前日期重置格子内部元件
    bool MainWidgetHideOrShow(QKeyEvent *event, bool pd = false);
public:
    TransparentButton *SetColorButton,*FileButton,*PlusButton;
    QDate date;
    DailyData *data;
    QLabel *dateLabel;
    QGridLayout *gridlayout;
    explicit DayWidget(QWidget *parent = 0);

    //根据内置变量重置样式表
    void resetStyleSheet();
    void setDate(const QDate &);
    void setBackgroundColor(const QColor &);
    void setBackgroundAlpha(double);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject *watched, QEvent *event);

signals:

public slots:
    void setColor();
};

#endif // DAYWIDGET_H
