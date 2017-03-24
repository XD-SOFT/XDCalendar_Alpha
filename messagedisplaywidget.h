/**
 * XDCalendar Project 2017-03-18 version
  * 风格化的信息提示框，替代QMessageBox.
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

enum class MessageDisplayButtonType : quint8 {
    Ok = 0,
    Cacel = 1,
    NoButton = 255
};

class MessageDisplayWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    ~MessageDisplayWidget();

    static void showMessage(const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = MessageDisplayButtonType::NoButton);

private:
    explicit MessageDisplayWidget(QWidget *parent = 0);

private:
    Ui::MessageDisplayWidget *ui;
};

#endif // XDSF_MESSAGEDISPLAYWIDGET_H
