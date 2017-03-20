#include "inputtextwidget.h"
#include "ui_inputtextwidget.h"
#include <QEventLoop>

QEventLoop *InputTextWidget::m_pEvLoop = Q_NULLPTR;

InputTextWidget::InputTextWidget(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::InputTextWidget)
{
    ui->setupUi(this);

    setShadowWidth(30);

    connect (ui->pTitleBarWgt, &TitleBar::tbClose, [this] () {
         m_pEvLoop->exit(-1);
    });
}

InputTextWidget::InputTextWidget(const QString &sTitle, const QString &sDefaultText, QWidget *parent):
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::InputTextWidget)
{
    ui->setupUi(this);

    ui->pTitleBarWgt->setTitle(sTitle);
    ui->pTextLineEdit->setText(sDefaultText);

    setShadowWidth(30);

    connect (ui->pTitleBarWgt, &TitleBar::tbClose, [this] () {
        if(m_pEvLoop != Q_NULLPTR) {
            m_pEvLoop->exit(-1);
        }
        else {
            close();
        }
    });
}

InputTextWidget::~InputTextWidget()
{
    delete ui;
}

QString InputTextWidget::getText(const QString &sTitle, const QString &sDefaultText, QWidget *parent)
{
    InputTextWidget *pInputTextWgt = new InputTextWidget(sTitle, sDefaultText, parent);
    pInputTextWgt->show();

    if(m_pEvLoop == Q_NULLPTR) {
        m_pEvLoop = new QEventLoop();
    }

    int nResult = m_pEvLoop->exec();
    QString sText = pInputTextWgt->getText();

    if(nResult != 0) {
        sText.clear();
    }

    pInputTextWgt->setParent(Q_NULLPTR);
    delete pInputTextWgt;
    pInputTextWgt = Q_NULLPTR;

    return sText;
}

QString InputTextWidget::getText() const
{
    return ui->pTextLineEdit->text();
}

void InputTextWidget::on_pOKBtn_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(0);
    }
    else {
        close();
    }
}

void InputTextWidget::on_pCancelBtn_clicked()
{
    if(m_pEvLoop != Q_NULLPTR) {
        m_pEvLoop->exit(-1);
    }
    else {
        ui->pTextLineEdit->clear();
        close();
    }
}
