#include "messagedisplaywidget.h"
#include "ui_messagedisplaywidget.h"
#include <QEventLoop>
#include <QDebug>
QEventLoop *MessageDisplayWidget::m_pEvLoop = Q_NULLPTR;
MessageDisplayWidget::MessageDisplayWidget(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::MessageDisplayWidget)
{
    ui->setupUi(this);

    setShadowWidth(30);

    connect (ui->pTitleBarWgt, &TitleBar::tbClose, [this] () {
        m_pEvLoop->exit(-1);
    });
}

MessageDisplayWidget::MessageDisplayWidget(const QString &sTitle, const QString &sInfo, MessageDisplayButtonType okBtn, MessageDisplayButtonType btn, QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::MessageDisplayWidget)
{
    ui->setupUi(this);

    ui->pTitleBarWgt->setTitle(sTitle);
    ui->lText->setText(sInfo);

    setShadowWidth(30);

    //设置OK文本
    ui->pOkBtn->setText(MessageDisplayButtonName[(int)okBtn][0]);

    //显示或隐藏按键
    (okBtn == MessageDisplayButtonType::NoButton) ? ui->pOkBtn->hide() : ui->pOkBtn->show();
    (btn == MessageDisplayButtonType::NoButton) ? ui->pBtn->hide() : ui->pBtn->show();

    //当只有确定按键是使用确定按钮图标，否则使用另一个按键图标
    QString iconSyle = "QLabel#lIcon{border-image:url(%1);}";
    if(btn != MessageDisplayButtonType::NoButton)
    {
        //放在这里是为了防止btn==NoButton
        ui->pBtn->setText(MessageDisplayButtonName[(int)btn][0]);
        ui->lIcon->setStyleSheet(iconSyle.arg(MessageDisplayButtonName[(int)btn][1]) );
    }else{
        //当有一个按键居中
        ui->horizontalSpacer->changeSize(0,20);
        ui->lIcon->setStyleSheet(iconSyle.arg(MessageDisplayButtonName[(int)okBtn][1]) );
    }

    connect (ui->pTitleBarWgt, &TitleBar::tbClose, [this] () {
        if(m_pEvLoop != Q_NULLPTR) {
            m_pEvLoop->exit(-1);
        }
        else {
            close();
        }
    });
}

MessageDisplayWidget::~MessageDisplayWidget()
{
    delete ui;
}

int MessageDisplayWidget::showMessage(QWidget *parent ,const QString &sTitle, const QString &sInfo,
                                       MessageDisplayButtonType okBtn,
                                       MessageDisplayButtonType btn)
{
    ///Mark,使用QEventLoop.参照InputTextWidget ==》"inputtextwidget.h"实现.
    MessageDisplayWidget *pMessageDisplayWgt = new MessageDisplayWidget(sTitle, sInfo, okBtn, btn, parent);
    pMessageDisplayWgt->show();

    if(m_pEvLoop == Q_NULLPTR) {
        m_pEvLoop = new QEventLoop();
    }

    int nResult = m_pEvLoop->exec();

    pMessageDisplayWgt->setParent(Q_NULLPTR);
    delete pMessageDisplayWgt;
    pMessageDisplayWgt = Q_NULLPTR;

    return nResult;
}

void MessageDisplayWidget::on_pOkBtn_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(0);
    }
    else {
        close();
    }
}

void MessageDisplayWidget::on_pBtn_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(-1);
    }
    else {
        close();
    }
}
