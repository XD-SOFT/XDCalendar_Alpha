#include "handler.h"
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMutex>

Handler* Handler::m_pInstance = Q_NULLPTR;
/*
Handler::Handler(QObject *parent) : QObject(parent)
{

}
*/

Handler::Handler(QObject *parent):
    QObject(parent)
{
    m_pManager = new QNetworkAccessManager(this);
//    m_pRequest = new QNetworkRequest();
    connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Finished(QNetworkReply*)));
}

//Handler::Handler(const QString &arguments, QObject *parent):
//    QObject(parent)
//{
//    this->arguments = arguments;

//    m_pManager = new QNetworkAccessManager(this);
//    m_pRequest = new QNetworkRequest();
//    connect(m_pManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(Finished(QNetworkReply*)));
//}

Handler::~Handler()
{
    delete m_pRequest;
    m_pRequest = Q_NULLPTR;
}

Handler *Handler::getInstance()
{
    if (m_pInstance == Q_NULLPTR)
    {
        QMutex mutex;
        mutex.lock();

        Handler *tempInstance = m_pInstance;

        if(m_pInstance == NULL)
        {
            tempInstance = new Handler;
            m_pInstance = tempInstance;
        }

        mutex.unlock();
    }

    return m_pInstance;
}

void Handler::requestByUrlAndType(const QString &arguments, RequestType type)
{
    //如果请求还在，没有处理，那么避免重复请求.
    if(!requestUrlTypeHash.contains(arguments)) {
        requestUrlTypeHash.insert(arguments, type);
//        m_requestTypeVec.append(type);
        m_reqArguments.clear();
        m_reqArguments.append(arguments);
    }
    else {
        RequestType existType = requestUrlTypeHash.value(arguments);

        Q_ASSERT_X(existType == type, "Handler::setRequestUrl", "Exist the same request but different request type");
    }

    run();
}

void Handler::destroyInstance()
{
    qDebug() << "deleting~~~~~~" << m_pInstance;
    if(m_pInstance != NULL)
    {
        static QMutex mutex;
        mutex.lock();

        if(m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }

        mutex.unlock();
    }
}

void Handler::reset()
{
    m_reqArguments.clear();
    requestUrlTypeHash.clear();
    m_requestTypeVec.clear();

    for(int index = 0; index < m_replyList.size(); ++index) {
        QNetworkReply *pReply = m_replyList.at(index);
        pReply->abort();
    }

    m_replyList.clear();
}

void Handler::Finished(QNetworkReply *reply)
{
//    QNetworkRequest request = reply->request();
//    QString sRequestUrl = request.url().toString();

    QNetworkRequest request = reply->request();
    QString sRequestUrl = request.url().toString();
    RequestType type = requestUrlTypeHash.value(sRequestUrl, Default);
    qDebug() << "the reply url is:" << sRequestUrl;



    ///Mark 2017.02.13,重构时候这里设计时候考虑修改为表驱动.
    //qDebug()<<"handler Finished"<<endl;
    if (reply->error() == QNetworkReply::NoError)
    {
        ///必须添加请求与应答是否一致判断, 不一致要再次做处理.

        //不存在这个类型的请求.
        if(type == Default) {
           ///Mark一般不会执行到这里，执行到这里说明错误了.
//            m_requestTypeVec.removeOne(type);

            return;
        }

        //        if(m_requestTypeVec.isEmpty()) {
//            requestUrlTypeHash.clear();
//        }

        QByteArray bytes = reply->readAll();
        //qDebug()<<bytes;
        //QString res = QString::fromUtf8(bytes);
        //qDebug()<<"report: "<<res<<endl;
        QJsonObject jo = QJsonDocument::fromJson(bytes).object();
        //qDebug()<<jo<<endl;
        //qDebug() << "jo size: " <<jo.length()<<endl;
        //qDebug() << "jo: " << jo["summary\\"].toArray()<<endl;
        //出队一个类型.
//        RequestType type = requestTypeQueue.dequeue();
        qDebug() << "the request reply code is:" << type;

        if(jo["status"] != "tokenFalseRedirect")
        {
            switch(type) {
            case RequestType::Login: {
                emit loginStateChanged(jo);
            }
                break;

            case RequestType::TermDB: {
                emit termDBReplyArrived(jo);
            }
                break;

            case RequestType::Schedule: {
                emit scheduleReplyArrived(jo);
            }
                break;

            case RequestType::ScheduleDetailDB: {
                emit scheduleDetailDBReplyArrived(jo);
            }
                break;

            case RequestType::LessonDB: {
                emit lessonDBReplyArrived(jo);
            }
                break;

            case RequestType::LessonDetailDB : {
                emit lessonDetailDBReplyArrived(jo);
            }
                break;

            case RequestType::LessonRemarkDB: {
                emit lessonRemarkDBReplyArrived(jo);
            }
                break;

            case RequestType::ResFileDB: {
                emit resFileDBReplayArrived(jo);
            }
                break;

            case RequestType::SubjectInfoDB: {
                emit subjectInfoDBReplyArrived(jo);
            }
                break;

            case RequestType::GradeInfoDB: {
                emit gradeInfoDBReplyArrived(jo);
            }
                break;

            case RequestType::ClientConfigDB: {
                emit clientConfigDBReplyArrived(jo);
            }
                break;

            case RequestType::KeywordDB: {
                emit keywordDBReplyArrived(jo);
            }
                break;

            case RequestType::FileKeywordDB:{
                emit fileKeyWordDBReplyArrived(jo);
            }
                break;

            case RequestType::DataNW:{
                emit dataNWReplyArrived(jo);
            }
                break;

            case RequestType::ScheDetDB:{
                emit scheDetDBReplyArrived(jo);
            }
                break;

            case RequestType::UserInfomation:{
                emit getUserInfomationFinished(jo);
            }
                break;

            case RequestType::ModifiedPassword:{
                emit modifiedPasswordFinished(jo);
            }
                break;

            case RequestType::DownloadUrlRequest: {
                emit downloadUrlRequestFinished(jo);
            }
                break;

            default: {
//                emit updateUi(jo);
            }
                break;
            }
        }
        else
        {
            qDebug()<<"false token"<<endl;
            exit(-1);
            //return;
            //QMessageBox::information(this, "提示", "用户名或密码错误");
        }
    }
    else
    {
        reconnect++;
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
//        qDebug(qPrintable(reply->errorString()));
        /*
                         if(reconnect<=10)
                         {
                             run();
                         }
                         */
        //QMessageBox::information(this, "提示", "服务器连接错误");

//        requestUrlTypeHash.remove(sRequestUrl);

        QString sError = qPrintable(reply->errorString());
        if(sError.contains(":")) {
            QStringList sList = sError.split(":");
            sError= sList.last();
        }

        switch(type) {
        case RequestType::Login: {
            emit loginRequestError(sError);
        }
            break;

            ///TODO.其它错误处理暂时没加入.
        default:

            break;

        }
    }

    requestUrlTypeHash.remove(sRequestUrl);
    m_replyList.removeOne(reply);
    reply->deleteLater();
}

void Handler::run()
{
    QNetworkRequest request;
    request.setUrl(QUrl(m_reqArguments));
    QNetworkReply *pReply = m_pManager->get(request);
    m_replyList.append(pReply);
    qDebug()<<"request" << m_reqArguments << "finished" <<endl;
}
