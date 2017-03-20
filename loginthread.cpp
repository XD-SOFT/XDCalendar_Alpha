#include "loginthread.h"
#include <QDebug>
#include <QJsonParseError>
#include <QMessageBox>
#include <QJsonObject>

loginThread::loginThread(QWidget* parent, QString username, QString password)
{
    my_parent = parent;
    this->username = username;
    this->password = password;
}

// netease music login
void loginThread::Login()
{

    qDebug() << "loginThread login" <<endl;
    // make password to md5
    QByteArray byte_array;
    byte_array.append(password);
    //QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    //QString md5 = hash_byte_array.toHex();

    // send login data
    //qDebug()<<"here?"<<endl;

    //qDebug()<<"you are right"<<endl;
    //request->setUrl(QUrl("https://api.douban.com/v2/book/1220562"));
    //qDebug()<<"herere00000"<<endl;
    //manager->get(*request);
    //qDebug()<<"finish"<<endl;
    //qDebug() << "loginHO new" <<endl;
    //loginHO = new HttpOp();
    //QNetworkReply *reply;
    //loginHO->url(QString("http://127.0.0.1:8000/site/thirdPartAccount?username="+username+"&password="+password));
    //loginHO->url("www.baidu.com");
    //loginHO->data(QString("username="+username+"&password="+md5+"&rememberLogin=true"));
    //loginHO->data(QString("username="+username+"&password="+password));
    //loginHO->mode(POST);
    //loginHO->mode(GET);
    //QNetworkAccessManager* QNetworkManager = loginHO->send(reply);
    //QObject::connect(QNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoginFinish(QNetworkReply*)));
}


void loginThread::LoginFinish(QNetworkReply *reply)
{
    qDebug() << "loginThread loginFinish" <<endl;
    // when reply is no error
    if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray bytes = reply->readAll();
            qDebug()<<bytes;
            // decode json string
             QJsonParseError jsonEr;
             QJsonObject jsonOb = QJsonObject(QJsonDocument::fromJson(reply->readAll(),&jsonEr).object());

             if(jsonEr.error == QJsonParseError::NoError)
             {
                 QJsonObject account = jsonOb.take("account").toObject();
                 if(account.isEmpty()){
                    QMessageBox::information(NULL,"Information","Login error: return Json data is Empty ",QMessageBox::Ok);
                    return;
                 }
                 // get userID
                 QString fuid = QString::number(account.take("id").toInt(),10);
                 qDebug() << "fuid: " << fuid << endl;
                 //this->uid = fuid;
                 //emit senduid(fuid);
                 //this->GetPlaylist(fuid);
             }
             else
             {
                 QMessageBox::information(NULL,"Information","Login error: return data isn't JsonObject",QMessageBox::Ok);
             }
        }
        else {
            QMessageBox::information(NULL,"Information","Login error: Request is fail",QMessageBox::Ok);
            // 出现错误，打印出来，方便调试解决
            //QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
            //qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
            //qDebug(qPrintable(reply->errorString()));
        }
    reply->deleteLater();
    //delete loginHO;
    //loginHO = NULL;
}


void loginThread::run()
{
    qDebug()<< "This is login thread" << endl;
    QNetworkAccessManager* manager = new QNetworkAccessManager();
    connect(manager,SIGNAL(finished(QNetworkReply*)),  //关联信号和槽
                this,SLOT(LoginFinish(QNetworkReply*)));
    QNetworkRequest* request = new QNetworkRequest();

    QByteArray byte_array;
    byte_array.append(password);
    //QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    //QString md5 = hash_byte_array.toHex();

    // send login data
    //qDebug()<<"here?"<<endl;

    //qDebug()<<"you are right"<<endl;
    request->setUrl(QUrl("https://api.douban.com/v2/book/1220562"));
    //qDebug()<<"herere00000"<<endl;
    /*QNetworkReply *reply = */ manager->get(*request);
    qDebug()<<"finish: "<<endl;

    //Login();

}

