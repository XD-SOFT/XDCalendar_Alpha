#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include "framelessmodalmovableshadowwidget.h"

class QTextEdit;
class QLineEdit;
class QPushButton;

class FeedBackDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit FeedBackDialog(QWidget *parent = 0);
signals:
public slots:
    void submit ();
private:
    QTextEdit* mFeedBackEdit = nullptr;
    QLineEdit* mContactEdit = nullptr;
    QPushButton* mSubmitButton = nullptr;
};

#endif // FEEDBACKDIALOG_H
