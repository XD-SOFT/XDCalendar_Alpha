/**
 * XDCalendar Project 2016-12-23 version
  * 不同模式信息显示或编辑设置类管理窗口类.
  * 目前只有标准模式与编辑模式相关设置，其它模式需要添加在此类扩展.
  * @example
  * 1.在InfoSettingsType中添加新的类型.
  * 2.构造函数中添加新的widget.
  * 3.switchInfoAndSettingsWidget(InfoSettingsType)中添加新的切换逻辑.
  *
  * @author BiXiaoxia
  * @version 2016.12.23.
*/
#ifndef XDKJSOFT_INFOANDSETTINGSSTACKWIDGET_H
#define XDKJSOFT_INFOANDSETTINGSSTACKWIDGET_H

#include <QStackedWidget>


class InfoAndSettingsStackWidgetPrivate;

class InfoAndSettingsStackWidget : public QStackedWidget
{
    Q_OBJECT

public:
    Q_ENUMS(InfoSettingsType)
    enum InfoSettingsType{StandardMode, EditMode};
    explicit InfoAndSettingsStackWidget(QWidget *parent = 0);
    ~InfoAndSettingsStackWidget();

    /*!
     @ \brief 切换到type类型页面显示.
     @ \param type,设置或信息页面类型.
    */
    void switchInfoAndSettingsWidget(InfoSettingsType type);

protected:
    void paintEvent(QPaintEvent *pEvent);

private:
    InfoAndSettingsStackWidgetPrivate *m_pPrivate;
};

#endif // XDKJSOFT_INFOANDSETTINGSSTACKWIDGET_H
