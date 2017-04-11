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
    NoButton = 255
};

class MessageDisplayWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    ~MessageDisplayWidget();

    /*!
     * \brief sTitle 窗口标题.
     * \param sInfo  消息框信息.
     * \param okBtn  确定按钮.
     * \param parent 文本输入部件父窗口.
     * \return btn   取消按钮.
     * \example MessageDisplayWidget::showMessage（"education", "确认提交信息！", MessageDisplayButtonType::Ok, MessageDisplayButtonType::Cancel）
     */

    static MessageDisplayButtonType showMessage(const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);

private slots:
    void on_OkButton_clicked();
    void on_CandelButton_clicked();

private:
    explicit MessageDisplayWidget(QWidget *parent = 0);
    MessageDisplayWidget(const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn,
                           MessageDisplayButtonType btn, QWidget *parent = 0);

private:
    Ui::MessageDisplayWidget *ui;
    static QEventLoop *m_pEvLoop;
};

#endif // XDSF_MESSAGEDISPLAYWIDGET_H
