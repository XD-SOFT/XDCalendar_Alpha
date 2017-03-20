#ifndef LOGINTHREAD_H
#define LOGINTHREAD_H

#include <QThread>

#include <QString>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QUrl>
#include <QDebug>
#include <QObject>

#include "httpop.h"

#define POST "POST"
#define GET "GET"

class loginThread : public QThread
{
    Q_OBJECT
public:
    loginThread(QWidget* parent, QString username, QString password);
    void Login();

protected slots:
    void LoginFinish(QNetworkReply *reply);

private:
    void run();
    QWidget* my_parent;
    QString username;
    QString password;

    //HttpOp* loginHO;
    //QNetworkAccessManager* manager;
    //QNetworkRequest *request;






};

#endif // LOGINTHREAD_H
