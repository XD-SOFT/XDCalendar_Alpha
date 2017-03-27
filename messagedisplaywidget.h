/**
 * XDCalendar Project 2017-03-18 version
  * 风格化的信息提示框，替代QMessageBox.
  * 这个类使用必须自己管理释放.因为阴影不能设置父窗口对象.
  * @example MessageDisplayWidget::showMessage;
  *
  * @author BiXiaoxia
  * @version 2017.03.18.
  * @author Changmanyi
  * @version 2017.03.24.
*/
#ifndef XDSF_MESSAGEDISPLAYWIDGET_H
#define XDSF_MESSAGEDISPLAYWIDGET_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class MessageDisplayWidget;
}

class QEventLoop;
enum class MessageDisplayButtonType : quint8 {
    Ok = 0,
    Cancel = 1,
    About,
    NoButton = 255
};
//上面每添加一个新的按键,必须添加对应的名称 ；按键的值=对应当前数据组的下标索引
static QString MessageDisplayButtonName[]={
    "Ok",
    "Cancel",
    "About"
};
class MessageDisplayWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    ~MessageDisplayWidget();

    static int showMessage(QWidget *parent ,const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);
    static int information(QWidget *parent ,const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);
    static int about(QWidget *parent ,const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);


private slots:
    void on_pOkBtn_clicked();
    void on_pBtn_clicked();

private:
    explicit MessageDisplayWidget(QWidget *parent = 0);
    MessageDisplayWidget(const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn,
                           MessageDisplayButtonType btn, int type,QWidget *parent = Q_NULLPTR );
    static int creatMessage(QWidget *parent ,const QString &sTitle, const QString &sInfo, int type, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                            MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);

private:
    Ui::MessageDisplayWidget *ui;
    static QEventLoop *m_pEvLoop;
};

#endif // XDSF_MESSAGEDISPLAYWIDGET_H
