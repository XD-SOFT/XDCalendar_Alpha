#include "messagedisplaywidget.h"
#include "ui_messagedisplaywidget.h"

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

void MessageDisplayWidget::showMessage(QWidget *parent, const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn, MessageDisplayButtonType btn)
{

}
