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
    explicit MessageDisplayWidget(QWidget *parent = 0);
    ~MessageDisplayWidget();

    static void showMessage(QWidget *parent, const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn = MessageDisplayButtonType::Ok,
                       MessageDisplayButtonType btn = NoButton);
private:
    Ui::MessageDisplayWidget *ui;
};

#endif // XDSF_MESSAGEDISPLAYWIDGET_H
