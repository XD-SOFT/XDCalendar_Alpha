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

MessageDisplayWidget::MessageDisplayWidget(const QString &sTitle,
                                           const QString &sInfo,
                                           MessageDisplayButtonType okBtn,
                                           MessageDisplayButtonType btn, QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::MessageDisplayWidget)
{
    ui->setupUi(this);

    //Qt::WindowStaysOnTopHint使弹出窗口呆在最顶层，Qt::FramelessWindowHint设置无边框，不加这个会显示边框
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    //设置消息框为模态窗口
    this->setAttribute(Qt::WA_ShowModal, true);

    ui->pTitleBarWgt->setTitle(sTitle);
    ui->lText->setText(sInfo);

    setShadowWidth(30);

    //防止弹出框太长，分行显示
    ui->lText->setWordWrap(true);
    ui->lText->setContentsMargins(20,0,20,0);

    //显示或隐藏按键
    (okBtn == MessageDisplayButtonType::NoButton) ? ui->pOkBtn->hide() : ui->pOkBtn->show();
    (btn == MessageDisplayButtonType::NoButton) ? ui->pCandelBtn->hide() : ui->pCandelBtn->show();

    //当有一个按键居中
    if(btn == MessageDisplayButtonType::NoButton)
    {
        ui->horizontalSpacer->changeSize(0,20);
    }

    connect(ui->pOkBtn, SIGNAL(clicked(bool)), this, SLOT(on_OkButton_clicked()));
    connect(ui->pCandelBtn, SIGNAL(clicked(bool)), this, SLOT(on_CandelButton_clicked()));

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

MessageDisplayButtonType MessageDisplayWidget::showMessage(const QString &sTitle, const QString &sInfo,
                                                           MessageDisplayButtonType okBtn,
                                                           MessageDisplayButtonType btn)
{
    int nResult  = 0;

   //安全处理，当有一个执行时，再次调用崩溃
   if(m_pEvLoop != NULL) return MessageDisplayButtonType::NoButton;

    ///Mark,使用QEventLoop.参照InputTextWidget ==》"inputtextwidget.h"实现.
    MessageDisplayWidget *pMessageDisplayWgt = new MessageDisplayWidget(sTitle, sInfo, okBtn, btn);
    pMessageDisplayWgt->show();

    if(m_pEvLoop == Q_NULLPTR) {
        m_pEvLoop = new QEventLoop();
    }

    nResult = m_pEvLoop->exec();

    delete pMessageDisplayWgt;
    delete m_pEvLoop;
    m_pEvLoop = Q_NULLPTR;

    return nResult == 0 ? MessageDisplayButtonType::Ok : MessageDisplayButtonType::Cancel;
}

void MessageDisplayWidget::on_OkButton_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(0);
    }
    else {
        close();
    }
}

void MessageDisplayWidget::on_CandelButton_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(-1);
    }
    else {
        close();
    }
}
