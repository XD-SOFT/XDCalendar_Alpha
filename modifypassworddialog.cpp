#include "modifypassworddialog.h"
#include "teacher.h"
#include <QLabel>
#include "config.h"
#include <QPushButton>
#include <QLineEdit>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <mutex>
#include <thread>
#include <QTimer>

ModifyPasswordDialog::ModifyPasswordDialog(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent)
{
//    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    setAttribute (Qt::WA_DeleteOnClose);

    auto layout = new QVBoxLayout (this);
    int nShadowWidth = getShadowMargin();
    layout->setContentsMargins(nShadowWidth + 5, nShadowWidth + 5, nShadowWidth + 5, nShadowWidth + 5);

    titleLabel = new QLabel (this);
    titleLabel->setText ("修改密码");
    titleLabel->setFixedHeight(30);
    layout->addWidget (titleLabel);

    line = new QFrame (this);
    line->setFrameShape (QFrame::HLine);
    line->setFrameShadow (QFrame::Sunken);
    layout->addWidget (line);

    mValidateWidget = new QWidget (this);
    auto mValidateLayout = new QVBoxLayout(mValidateWidget);
    layout->addWidget (mValidateWidget);

    {
        auto validateMethodLabel = new QLabel (this);
        validateMethodLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        validateMethodLabel->setText ("验证方式");
        auto validateMethodDetail = new QLabel (this);
        validateMethodDetail->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        validateMethodDetail->setText (QString("绑定手机:%1").arg ("13680010416"));
        QHBoxLayout *methodLayout = new QHBoxLayout;
        methodLayout->addWidget (validateMethodLabel);
        methodLayout->addWidget (validateMethodDetail);
        mValidateLayout->addLayout (methodLayout);
    }
    {
        auto validatorLabel = new QLabel ("验证码", this);
        mValidatorEdit = new QLineEdit (this);
        mValidatorEdit->setPlaceholderText ("请输入验证码");
        mSendValidatorBtn = new QPushButton ("免费获取验证码", this);
        connect (mSendValidatorBtn, SIGNAL(clicked()), this, SLOT(applyForValidator ()));
        QHBoxLayout *validateLayout = new QHBoxLayout;
        validateLayout->addWidget (validatorLabel);
        validateLayout->addWidget (mValidatorEdit);
        validateLayout->addWidget (mSendValidatorBtn);
        mValidateLayout->addLayout (validateLayout);
    }

    mModifyWidget = new QWidget (this);
    mModifyWidget->hide ();
    layout->addWidget (mModifyWidget);
    auto mModifyLayout = new QGridLayout (mModifyWidget);

    auto pwLabel = new QLabel ("新密码", this);
    auto confirmLabel = new QLabel ("确认密码", this);
    mPasswordEdit = new QLineEdit (this);
    mPasswordEdit->setEchoMode (QLineEdit::Password);
    mConfirmPasswordEdit = new QLineEdit (this);
    mConfirmPasswordEdit->setEchoMode (QLineEdit::Password);
    static_cast<QGridLayout*>(mModifyLayout)->addWidget (pwLabel, 0, 0);
    static_cast<QGridLayout*>(mModifyLayout)->addWidget (confirmLabel, 1, 0);
    static_cast<QGridLayout*>(mModifyLayout)->addWidget (mPasswordEdit, 0, 1);
    static_cast<QGridLayout*>(mModifyLayout)->addWidget (mConfirmPasswordEdit, 1, 1);


    mSubmitBtn = new QPushButton ("提交", this);
    mSubmitBtn->setObjectName ("submit");
    QHBoxLayout *pSubmitBtnLayout = new QHBoxLayout;
    pSubmitBtnLayout->addStretch(1);
    pSubmitBtnLayout->addWidget(mSubmitBtn);
    pSubmitBtnLayout->addStretch(1);
    connect (mSubmitBtn, SIGNAL (clicked ()), this, SLOT(onSubmit ()));

    mSuccessWidget = new QWidget (this);
    mSuccessWidget->hide ();
    auto mSuccessLayout = new QVBoxLayout(mSuccessWidget);
    {
        mSuccessLayout->setAlignment (Qt::AlignHCenter);
        auto l0 = new QLabel (this);
        l0->setAlignment (Qt::AlignHCenter);
        l0->setPixmap (QPixmap::fromImage (QImage (":/Icon/success.png")));
        l0->setFixedSize (100, 100);
        l0->setScaledContents (true);
        auto l1 = new QLabel ("密码修改成功", this);
        auto l2 = new QLabel ("请用新密码登录教师桌面应用", this);
        mSuccessLayout->addWidget (l0);
        mSuccessLayout->addWidget (l1);
        mSuccessLayout->addWidget (l2);
    }
    layout->addWidget (mSuccessWidget);

    layout->addLayout(pSubmitBtnLayout);
    layout->addSpacing(20);

    setFixedSize(300, 255);
}

void ModifyPasswordDialog::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    painter->save();
    painter->setPen (QPen (QColor(17, 81, 181)));

    auto bias = getShadowMargin();
    painter->fillRect(bias, bias, width() - bias * 2, height() - 2 * bias, Qt::white);
    painter->drawRect (bias + 5, bias + 5, width () - bias * 2 - 10, height () - 2 * bias - 10);
    painter->restore();
}

void ModifyPasswordDialog::onSubmit ()
{
    if (mState == State::Validating)
    {
        submitValidator ();
    }
    else if (mState == State::Modifying)
    {
        submitPassword ();
    }
    else
        return;
}

void ModifyPasswordDialog::submitPassword ()
{
    //if success
    if (mPasswordEdit->text () != mConfirmPasswordEdit->text ())
        return;
    Arg::user->setPassword (mPasswordEdit->text ());
    mModifyWidget->hide ();
    mSubmitBtn->hide ();
    titleLabel->hide ();
    line->hide ();
    mSuccessWidget->show ();
    QTimer* timer = new QTimer (this);
    timer->setSingleShot (true);
    timer->setInterval (1000);
    timer->start ();
    connect (timer, &QTimer::timeout, [this] ()
    {
        this->close ();
    });
}

void ModifyPasswordDialog::submitValidator ()
{
    //if success
    mState = State::Modifying;
    mValidateWidget->hide ();
    mModifyWidget->show ();
    validatePassword ();
    connect (mPasswordEdit, SIGNAL(textChanged (QString)), this, SLOT (validatePassword ()));
    connect (mConfirmPasswordEdit, SIGNAL(textChanged (QString)), this, SLOT (validatePassword ()));
}

void ModifyPasswordDialog::validatePassword ()
{
    qDebug () << mPasswordEdit->text () << mConfirmPasswordEdit->text ();
    if (mPasswordEdit->text () == mConfirmPasswordEdit->text ())
        mSubmitBtn->setEnabled (true);
    else
        mSubmitBtn->setEnabled (false);
}
void ModifyPasswordDialog::applyForValidator ()
{
    static bool initialized = false;
    static auto updater = [this] () {
            --this->mTimeCount;
            if (this->mTimeCount > 0)
            {
                this->mSendValidatorBtn->setText (QString("%1秒后重新获取").arg (this->mTimeCount));
            }
            else
            {
                this->mSendValidatorBtn->setText ("免费获取验证码");
                this->mSendValidatorBtn->setEnabled (true);
                this->mTimer->stop ();
            }
        };
    if (!initialized)
    {
        mTimer = new QTimer;
        mTimer->setSingleShot (false);
        mTimer->setInterval (2048);
        initialized = true;
        connect (mTimer, &QTimer::timeout, updater);
    }
    if (mTimeCount <= 0)
    {
        mTimeCount = 11;
        mTimer->start ();
        updater ();
        this->mSendValidatorBtn->setEnabled (false);
    }
    else
        return;
}

//void ModifyPasswordDialog::paintEvent (QPaintEvent* ev)
//{
//    QPainter painter (this);
//    painter.setPen (QPen (QColor(17, 81, 181)));

//    auto bias = 5;
//    painter.drawRect (bias, bias, this->width () - bias * 2, this->height () - bias * 2);

//    QWidget::paintEvent (ev);
//}
