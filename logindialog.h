#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "framelessmodalmovableshadowwidget.h"
#include <QVector>
#include <QJsonObject>

//namespace Ui {
//class loginDialog;
//}

class QPushButton;
class MaskLabel;
class QLabel;
class QCheckBox;
class QLineEdit;
class QListWidget;
class QComboBox;
class UserDB;
class CCU;
class QMovie;
class LoginStatusWidget;

class loginDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    explicit loginDialog(QWidget *parent = 0, CCU* ccu = nullptr);
    ~loginDialog();

    QWidget *getMainwindow();

    void enableChangeAcounntLogin(bool bEnabled);

public slots:
    void onAppStartMessageReceived(const QString&);

private slots:
    void loginState(const QJsonObject &);

    void on_pushButton_2_clicked();

    void on_loginButton_clicked();

    void accountChanged(int nIndex);

    void showMainWindow();

    void currentAccountChanged(const QString &sText);

    void modifiedPasswordFinished(const QString &sUserName, const QString &sPassword);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

    void showEvent(QShowEvent *event);

    void timerEvent(QTimerEvent *pTmEv);

    void drawExcludeShadowPart(QPainter *painter, QPaintEvent* ev);

private:
//    Ui::loginDialog *ui;
    QVector<QString> applist;

    QPushButton* loginButton;
    MaskLabel *photoLabel;
    QLabel *regLabel, *forgetLabel,/*, *recordLabel, *autoLabel, *schmeLabel,*/ *topLabel;
    QCheckBox *recordBox, *autoBox/*, *schmeBox*/;
    QLineEdit *usernameInput, *passwordInput;

    QComboBox *pLoginAccountComboBox;
    QJsonObject *jsonFile = nullptr;
    QListWidget *list_widget;
    QMovie *m_pHeaderMovie;

    UserDB* userdb = Q_NULLPTR;
    CCU* ccu;

    LoginStatusWidget *m_pLoginStatusWidget = Q_NULLPTR;

    int m_nAutoLoginTimerID;

    bool m_bChangeAccountLogin = false; //是否为切换账号登录，即注销登录.

private:
    void keyPressEvent(QKeyEvent * event);
    void readLocalUserJsonFile(const QString &filename);
    void writeLocalUserJsonFile(const QString &filename);
    void initializeUIValue();

signals:
    void start();

public slots:
    void showAccount(const QString &account);
    void removeAccount(const QString &account);
};

#endif // LOGINDIALOG_H
