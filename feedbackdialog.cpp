#include "feedbackdialog.h"
#include "titlebar.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRegExpValidator>
#include <QRegExp>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
FeedBackDialog::FeedBackDialog(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent)
{
//    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
//    setWindowModality (Qt::WindowModal);
    setAttribute (Qt::WA_DeleteOnClose);

    auto layout = new QVBoxLayout (this);
    int nShadowWidth = getShadowMargin();
    layout->setContentsMargins(nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);
    auto contentLayout = new QVBoxLayout ();
    contentLayout->setContentsMargins (20, 20, 20, 20);
    {
        auto titleBar = new TitleBar (QStringLiteral ("用户反馈"), this);
        connect (titleBar, SIGNAL(tbClose ()), this, SLOT(submit ()));
        layout->addWidget (titleBar, 1);
    }

    {
        auto label1 = new QLabel(QStringLiteral ("您的支持是我们改进产品的动力，请留下您的宝贵意见，我们会认真聆听"), this);
        contentLayout->addWidget (label1, 5);

        mFeedBackEdit = new QTextEdit (this);
        mFeedBackEdit->setPlaceholderText (QStringLiteral ("最多可输入300字"));
        contentLayout->addWidget (mFeedBackEdit, 50);

        auto label2 = new QLabel (QStringLiteral ("您的联系方式，方便我们帮您解决问题"), this);
        contentLayout->addWidget (label2, 5);

        mContactEdit = new QLineEdit (this);
        mContactEdit->setPlaceholderText (QStringLiteral ("手机号/邮箱"));
        QRegExp contactRE ("(\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b)|(.{11})");
        mContactEdit->setValidator (new QRegExpValidator(contactRE));
        contentLayout->addWidget (mContactEdit, 5);


        auto submitLayout = new QHBoxLayout;
        submitLayout->addSpacing (100);
        mSubmitButton = new QPushButton ("提交反馈", this);
        submitLayout->setAlignment (Qt::AlignRight);
        mSubmitButton->setObjectName ("submit");
        mSubmitButton->setFixedSize (100, 40);
        submitLayout->addWidget (mSubmitButton);
        connect (mSubmitButton, SIGNAL(clicked()), this, SLOT(submit ()));

        contentLayout->addLayout (submitLayout, 8);
    }
    layout->addLayout (contentLayout);
}

void FeedBackDialog::submit ()
{
    this->close ();
}
