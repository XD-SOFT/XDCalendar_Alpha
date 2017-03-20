#include "userdb.h"
#include "handler.h"
#include "config.h"
//#include <QThread>
#include <QJsonArray>
#include <QDebug>

void UserDB::visitState(const QJsonObject &json)
{
    qDebug()<<"come in visitState: "<<endl << json;
    emit updateLoginUI(json);

    QJsonValue statusValue = json.value("status");
    if(statusValue.isString())
    {
        QString status = statusValue.toString();
        if(status.compare("true") == 0)
        {
            Arg::userId = json.value("userid").toInt();
            Arg *pArg = Arg::getInstance();
            QString sBaseUrl;
            bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
            if(!bStatus) {
                return;
            }

            QString url = sBaseUrl + "Desktop/findTUserJson?id=" + QString::number(Arg::userId) + Arg::tokenStr;
            Handler* pHandler = Handler::getInstance();
            pHandler->requestByUrlAndType(url, Handler::UserInfomation);
        }
    }
}


void UserDB::setUserInfomation(const QJsonObject &jsonObj)
{
//    qDebug() << "GET THE USER IFNOMATION END:" << jsonObj;

    QJsonValue statusValue = jsonObj.value("status");
    QString status = statusValue.toString();
    if(status.compare("1") == 0)
    {
        QJsonArray jsonAry = jsonObj["id"].toArray();
        for (QJsonValue res : jsonAry)
        {
            QString sDBUserName = res.toObject()["name"].toString();

            Arg::sNickName = sDBUserName.isEmpty() ? Arg::username : sDBUserName;

            qDebug() << "the user name is:" << Arg::sNickName;
        }
    }
    else {
        Arg::sNickName = Arg::username;
    }
}

void UserDB::modifiedPasswordFinished(const QJsonObject &jsonObj)
{
    QJsonValue statusValue = jsonObj.value("status");
    bool bStatus = statusValue.toBool();
    if(bStatus) {
        if((!m_sPassword.isEmpty()) && (!m_sUserName.isEmpty())) {
            emit modifiedPasswordByUserNameFinished(m_sUserName, m_sPassword);
        }
    }
}

UserDB::UserDB(QObject *parent):
    QObject(parent)
{
//    baseUrl = Arg::sNetRequestHostUrl/*"http://101.200.176.87:8080/Desktop/"*/;

    Handler* pHandler = Handler::getInstance();
    connect(pHandler, SIGNAL(loginStateChanged(const QJsonObject&)), this, SLOT(visitState(const QJsonObject&)));
    connect(pHandler, &Handler::loginRequestError, this, &UserDB::loginRequestError);
    connect(pHandler, SIGNAL(getUserInfomationFinished(const QJsonObject&)), this, SLOT(setUserInfomation(const QJsonObject&)));

    connect(pHandler, SIGNAL(modifiedPasswordFinished(const QJsonObject&)), this, SLOT(modifiedPasswordFinished(const QJsonObject&)));
}

void UserDB::modifiedPasswordByUserName(const QString &sUserName, const QString &sPassword)
{
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    m_sPassword.append(sPassword);
    m_sUserName.append(sUserName);

    QString url = sBaseUrl + "changePasswordJSON?username=" + sUserName +"&newPassword2=" + sPassword + "&token=" + Arg::token;
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::ModifiedPassword);
}

void UserDB::query()
{
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    QString url = sBaseUrl + "urlLogin?username=" + username + "&password=" + password;
    qDebug()<<"url: "<<url<< endl;
//    Handler* handler = new Handler(url);
//    connect(handler, SIGNAL(updateUi(QJsonObject)), this, SLOT(visitState(QJsonObject)));
//    handler->run();
    Handler *pHandler = Handler::getInstance();
    pHandler->requestByUrlAndType(url, Handler::Login);
//    pHandler->run();
}
