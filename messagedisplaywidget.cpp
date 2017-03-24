#include "messagedisplaywidget.h"
#include "ui_messagedisplaywidget.h"
#include <QEventLoop>

MessageDisplayWidget::MessageDisplayWidget(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::MessageDisplayWidget)
{
    ui->setupUi(this);
}

MessageDisplayWidget::~MessageDisplayWidget()
{
    delete ui;
}

void MessageDisplayWidget::showMessage(QWidget *parent, const QString &sTitle, const QString &sInfo,
                                       MessageDisplayButtonType okBtn,
                                       MessageDisplayButtonType btn)
{
    ///Mark,使用QEventLoop.参照InputTextWidget ==》"inputtextwidget.h"实现.

}
