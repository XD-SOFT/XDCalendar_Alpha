/** XDCalendar Project 2017-01-12 version
 * 耗时操作状态提示类.
 * @class LoginStautsWidget.
 * @brief 耗时状态过渡提示类.
 * @author BiXiaoxia.
 * @version 2017.02.07
*/
#ifndef XDSOFT_LOGINSTATUSWIDGET_H
#define XDSOFT_LOGINSTATUSWIDGET_H

#include "baseshadowwidget.h"
#include <QFont>

class LoginStatusWidgetPrivate;

class LoginStatusWidget : public BaseShadowWidget
{
    Q_OBJECT

public:
    LoginStatusWidget(QWidget *parent = Q_NULLPTR);
    ~LoginStatusWidget();

    /*!
      * @brief 启用背景图片，如果没有调用setStartImage设置背景图片，则使用默认背景图片.
      * @param bEnabled
    */
    void setStartImageEnabled(bool bEnabled);

    /*!
      * @brief 设置背景图片.
      * @param sPixPathName, 图片的路径名.
    */
    void setStartImage(const QString &sPixPathName);

    /*!
      * @brief 设置透明度.
      * @param fOpacity, 透明度值.
    */
    void setOpacity(qreal fOpacity);

    /*!
      * @brief 设置状态文本字体.
      * @param font, 字体.
    */
    void setFont(const QFont &font);

    /*!
      * @brief 设置状态文本是否启用.
      * @param bEnabled,
    */
    void setStatusTextEnabled(bool bEnabled);

    /*!
      * @brief 设置状态文本.
      * @param sText,状态文本.
    */
    void setStatusText(const QString &sText);

    /*!
      * @brief 设置状态文本是否自动改变,一般没有状态过来的时候用，但是如果耗时操作阻塞住了未必好用.
      * @param bEnabled.
    */
    void setAutoChangeStatusText(bool bEnabled);

protected:
    void drawExcludeShadowPart(QPainter *painter, QPaintEvent* ev);

    void showEvent(QShowEvent *pEv);

    void closeEvent(QCloseEvent *pEv);

    void timerEvent(QTimerEvent *pEv);

private:
    LoginStatusWidgetPrivate *m_pPrivate;
};

#endif // XDSOFT_LOGINSTATUSWIDGET_H
