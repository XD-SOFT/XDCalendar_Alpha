#include "httpop.h"

HttpOp::HttpOp()
{
    qDebug() << "HttpOp construct" <<endl;
    this->mode(GET);
    this->init();
}

// set send data for class member
HttpOp* HttpOp::data(QString sdata)
{
    qDebug() << "HttpOp data" <<endl;
    this->mdata = sdata;
    this->mQdataArray.append(sdata);
    this->mNetRequest.setHeader(QNetworkRequest::ContentLengthHeader, this->mQdataArray.length());
    return this;
}

// set send mode (post or get)
HttpOp* HttpOp::mode(QString smode)
{
    qDebug() << "HttpOp mode" <<endl;
    this->mmode = smode;
    return this;
}

void HttpOp::init()
{
    qDebug() << "HttpOp init" <<endl;
    mNetManager = new QNetworkAccessManager();
    //this->mNetRequest.setRawHeader("Accept","*/*");
    //this->mNetRequest.setRawHeader("Accept-Language","zh-CN,zh;q=0.8,gl;q=0.6,zh-TW;q=0.4");
    //this->mNetRequest.setRawHeader("Connection","keep-alive");
    //this->mNetRequest.setRawHeader("Host","music.163.com");
    this->mNetRequest.setRawHeader
        ("Referer","http://127.0.0.1:8000/site/thirdPartAccount?username=student&password=123");
    this->mNetRequest.setHeader(QNetworkRequest::UserAgentHeader,"Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/33.0.1750.152 Safari/537.36");
    this->mNetRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    //this->mNetRequest.setHeader(QNetworkRequest::CookieHeader,"appver=1.5.2");
}

HttpOp* HttpOp::url(QString surl)
{
    qDebug() << "HttpOp url" <<endl;
    this->mUrl = surl;
    this->mNetRequest.setUrl(QUrl(surl));
    return this;
}

HttpOp::~HttpOp()
{
    delete this->mNetManager;
}

// send data
QNetworkAccessManager* HttpOp::send(QNetworkReply* reply)
{
    qDebug() << "HttpOp send" <<endl;
    if(mmode == POST) reply = mNetManager->post(mNetRequest,mQdataArray);
    else{
        qDebug()<<"Using Get Method"<<endl;
        qDebug()<<"http request: "<<mNetRequest.url()<<endl;
        //mNetRequest.setUrl(QUrl(QString(mUrl)));
        reply = mNetManager->get(mNetRequest);
        qDebug()<<reply->error()<<" "<<reply->readAll()<<endl;
    }
    qDebug() << "http reply: " << reply->isFinished() << " " << reply->isRunning() << reply->isReadable() << endl;
    QString result = QString(reply->readAll());
    qDebug()<<"result: "<<result<<" "<<endl;
    return mNetManager;
}

