#ifndef MODIFYPASSWORDDIALOG_H
#define MODIFYPASSWORDDIALOG_H

#include "framelessmodalmovableshadowwidget.h"
class QLabel;
class QPushButton;
class QLineEdit;
class QLayout;
class QTimer;
class QFrame;

class ModifyPasswordDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    enum class State {Validating, Modifying};
    explicit ModifyPasswordDialog(QWidget *parent = 0);
    virtual ~ModifyPasswordDialog () = default;

protected:
    void drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev);

protected slots:
    void onSubmit ();
    void submitValidator ();
    void submitPassword ();
    void applyForValidator ();
    void validatePassword ();
private:
    State mState = State::Validating;
    QWidget *mValidateWidget, *mModifyWidget, *mSuccessWidget;
    QLabel* titleLabel;
    QFrame* line;
    QLineEdit *mValidatorEdit, *mPasswordEdit, *mConfirmPasswordEdit;
    QPushButton *mSendValidatorBtn, *mSubmitBtn;
    int mTimeCount = 0;
    QTimer* mTimer;
};

#endif // MODIFYPASSWORDDIALOG_H
