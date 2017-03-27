#include "filetransfer.h"
#include "messagedisplaywidget.h"
#include<QtDebug>
#include <QTextCodec>
#include <QAuthenticator>
#include "config.h"
#include <QProgressDialog>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QIODevice>
#include <QFileInfo>
#include <QFile>
#include <QTimer>
#include <QMessageBox>
#include "resfilesdb.h"
#include "lesson.h"

//#define FTP_USE

#include "ccu.h"


void FileTransfer::slotError(QNetworkReply::NetworkError code)
{
    //    --Arg::sDownLoadFileCount;
    //    emit uploadFileError("code.errorString()");
}

void FileTransfer::ftpUploadReplyFinished(QNetworkReply *reply)
{
    --Arg::sUpLoadFileCount;

    qDebug()<<"ftp upload reply"<<endl;
    QByteArray bytes = reply->readAll();  //获取字节
    QString result(bytes);  //转化为字符串
    qDebug()<<result;

//    if(m_uploadReplyFileMap.isEmpty()) {
//        ///Mark,一般执行到这里，用户关闭程序，取消上传，结果上传完了，这时候不再继续做处理.
//        return;
//    }

    QFile *pFile = m_uploadReplyFileMap.value(reply);
    QString sFilePathName;

    if(pFile != Q_NULLPTR) {

        sFilePathName = pFile->fileName();

        pFile->close();
        delete pFile;
        pFile = Q_NULLPTR;
    }

    //因为还要写上传文件资源id，所以这块不放进这里处理.
//    if(m_replyArgsHash.contains(reply)) {
//        QMap<QString, QString> replyArgs = m_replyArgsHash.value(reply);
//        QList<Lesson*> lessKeyList = m_upOrDownloadFileHash.keys();

//        for(auto itor = lessKeyList.begin(); itor != lessKeyList.end(); ++itor) {
//            Lesson *pLesson = *itor;

//            QList<QMap<QString, QString> > lessonFilesArgList = m_upOrDownloadFileHash.values(pLesson);

//            for(auto argItor = lessonFilesArgList.begin(); argItor != lessonFilesArgList.end(); ++argItor) {
//                if(replyArgs == *argItor) {
//                    m_upOrDownloadFileHash.remove(pLesson, replyArgs);

//                    break;
//                }
//            }
//        }
//    }


    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        qDebug()<<result;

        if(m_replyResDBMap.contains(reply)) {
            ResFilesDB *pResFilesDB = m_replyResDBMap.value(reply);
            if(pResFilesDB != Q_NULLPTR) {
                pResFilesDB->add();
            }

//            m_replyResDBMap.remove(reply);
        }

        //        if(bCreatUpload) {
        //            //向数据库资源表内插入记录
        //            ResFilesDB *pResFileDB = DataClassInstanceManage::getInstance()->getResFilesDBPtr();
        //    //        ResFilesDB* resfile = new ResFilesDB(this);
        //            connect(pResFileDB, SIGNAL(addFinish(QJsonObject)), this, SLOT(uiState(QJsonObject)), Qt::UniqueConnection);
        //            resfile->setLessonDetailId(detailID.toInt());
        //            resfile->setRemark(tr(""));
        //            resfile->setUpdateTime(QDate::currentDate());
        //            resfile->setUseDataTime(QDate::fromString(date, "yyyy-M-d"));
        //            resfile->setState(1);
        //            //url的拼装
        //            resfile->setFileUrl(Arg::ftpStr+fileNewName);
        //            resfile->setFileName(filePath["fileName"].split("/").takeAt(1));
        //            resfile->add();
        //        }

        emit fileUploadFinished(sFilePathName);
    }
    else
    {
        if(!m_bAbort) {
            //处理错误
            qDebug()<<"handle errors here";
            QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
            qDebug(qPrintable(reply->errorString()));

            emit uploadFileError(sFilePathName, qPrintable(reply->errorString()));

            reply->deleteLater();
        }
    }

    emit uploadFileFinished();

//    m_replyArgsHash.remove(reply);
    //    replyList.removeAll(reply);

//    reply->deleteLater();
}

void FileTransfer::ftpDownloadReplyFinished(QNetworkReply *reply)
{
    --Arg::sDownLoadFileCount;

    if(Arg::sDownLoadFileCount < 0) {
        Arg::sDownLoadFileCount = 0;
    }
    qDebug()<<"ftp download reply end"<<endl;

    if (reply->error() == QNetworkReply::NoError)
    {
        Arg *pArg = Arg::getInstance();
        QDir dir;
        pArg->getSaveDir(dir);
        QString filePath = /*Arg::configDir*/dir.absolutePath() + "/SaveFile/" + filearguments["folderName"]+"/"+filearguments["fileName"];

        qDebug()<<"ftp download file is:" << filePath;

        ///Mark2017.03.03，这里做过处理，如果文件存在，则删除掉.
        if(QFile(filePath).exists()){
            bool bRemoveStatus = QFile::remove(filePath);
            qDebug()<<"*********File Exits and delete**********"<< bRemoveStatus << endl;
        }

        //        // 重命名临时文件
        //        QFileInfo fileInfo(filePath);
        //        QFileInfo newFileInfo = fileInfo.absolutePath() + filearguments["fileName"];
        //        QDir dir1;
        //        if (dir1.exists(fileInfo.absolutePath()))
        //        {
        //            if (newFileInfo.exists())
        //                newFileInfo.dir().remove(newFileInfo.fileName());
        //            QFile::rename(filePath, newFileInfo.absoluteFilePath());
        //        }
        /*
        QDir *createfile = new QDir;
        bool exist = createfile->exists(filePath);
        if (exist){
            return;
        }
        else
        {

        }
*/
        if(m_replyArgsHash.contains(reply)) {
            QMap<QString, QString> replyArgs = m_replyArgsHash.value(reply);
            QList<Lesson*> lessKeyList = m_upOrDownloadFileHash.keys();

            for(auto itor = lessKeyList.begin(); itor != lessKeyList.end(); ++itor) {
                Lesson *pLesson = *itor;

                QList<QMap<QString, QString> > lessonFilesArgList = m_upOrDownloadFileHash.values(pLesson);

                for(auto argItor = lessonFilesArgList.begin(); argItor != lessonFilesArgList.end(); ++argItor) {
                    if(replyArgs == *argItor) {
                        m_upOrDownloadFileHash.remove(pLesson, replyArgs);

                        qDebug() << "remove the download complete file" << replyArgs;

                        break;
                    }
                }
            }

            QTimer *pTimer = qobject_cast<QTimer*>(m_timerReplyHash.key(reply));

            if(pTimer != Q_NULLPTR) {
                m_timerReplyHash.remove(pTimer);

                delete pTimer;
                pTimer = Q_NULLPTR;
            }
        }

        QFile* file = new QFile(filePath);

        //        if(file->exists())
        //        {
        //            qDebug()<<"file exists when ftp download"<<endl;

        //            return;
        //        }
        //        else
        //        {


        qDebug()<<"create file for a course in specific folder"<<endl;
        file->open(QIODevice::WriteOnly);//只读方式打开文件
        file->write(reply->readAll());
        file->close();
        //        }


        QJsonObject jo;
        qDebug() << "jo size: " <<jo.length()<<endl;
        //        emit updateUi(jo);
        emit downloadFinished();
    }
    else
    {
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
        qDebug()<<"-------------httpErrorString:"<<reply->errorString();
        if(!m_bAbort) {
            qDebug()<<"handle errors here";
            QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
            qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
            //        qDebug();

            emit ftpDownloadError(qPrintable(reply->errorString()));
        }
    }



    //    replyList.removeAll(reply);
    m_replyArgsHash.remove(reply);

    reply->deleteLater();

    if(Arg::sDownLoadFileCount == 0) {
        delete accessManager;
        accessManager = Q_NULLPTR;
    }
}

void FileTransfer::ftpDownload()
{
    qDebug()<<"ftpDownload"<<endl;

 #ifdef FTP_USE
    QUrl url;

    url.setScheme("ftp");
    //        url.setHost(arguments["host"]);//设置主机地址
    //        url.setPath(arguments["path"]);//设置URL路径
    //        url.setPort(2121);//设置URL的端口。
    //        url.setPassword(arguments["password"]); //设置ftp用户密码
    //        url.setUserName(arguments["username"]);//设置ftp用户名

    QString filename = QString::number(Arg::userId) + filearguments["detailID"] + filearguments["date"]
            + filearguments["fileName"];


    //    filearguments["host"] = Arg::IP;
    url.setPath("/" + filename);//设置URL路径
    url.setHost(/*filearguments["host"]*/Arg::IP);//设置主机地址
    url.setPort(2121);//设置URL的端口.
    url.setPassword("123456"); //设置ftp用户密码
    url.setUserName("user3");//设置ftp用户名

    //    filearguments["host"] = Arg::IP;
    //    url.setHost(filearguments["host"]);//设置主机地址
    //    url.setPath("/" + filename);//设置URL路径
    //    url.setPort(2121);//设置URL的端口.
    //    url.setPassword("123456"); //设置ftp用户密码
    //    url.setUserName("user3");//设置ftp用户名

    qDebug()<< "upload file args:" << filearguments["detailID"]<<","<<filearguments["fileName"]<<endl;

    qDebug()<<"---downLoad url："<<url.toString();
#else
    //http://101.200.176.87:8080/Desktop/clientDownloadFile&token=111111&username=lilu&fileName162872017-3-22test.txt
    QString filename = QString::number(Arg::userId) + filearguments["detailID"] + filearguments["date"]
            + filearguments["fileName"];
    QString uPath ="\/Desktop\/clientDownloadFile";
    QString uQuery =QString("\?token=111111")+
            QString("\&username=") + Arg::username+
            QString("\&fileName=") + filename;
    QString address =QString("http\:\/\/\%1:8080\%2\%3").arg(Arg::IP).arg(uPath).arg(uQuery);
    qDebug()<<"--------url:"<<address;
    QUrl url(address);

#endif
    QNetworkRequest request(url);
    //    request.setUrl(m_url);
    //    QNetworkAccessManager* manager = new QNetworkAccessManager;
    if(accessManager == Q_NULLPTR) {
        accessManager = new QNetworkAccessManager;
        connect(accessManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slotAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ftpDownloadReplyFinished(QNetworkReply*)));
    }

    //    if(reply != Q_NULLPTR) {
    //        disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)));
    //    }

    ++Arg::sDownLoadFileCount;

    QTimer *pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &FileTransfer::downloadTimeOut);

    reply = accessManager->get(request);
    m_replyArgsHash.insert(reply, filearguments);
    m_timerReplyHash.insert(pTimer, reply);
    connect(reply, SIGNAL(finished()), this, SLOT(finished()), Qt::UniqueConnection);
    //    qDebug()<<"***adsdsd"<<endl;
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onDownloadProgress(qint64, qint64)));
    //    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)), Qt::UniqueConnection);
    //    connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));

    pTimer->start(300000);
    //    replyList.append(reply);
}

void FileTransfer::downloadTimeOut()
{
    QTimer *pTimer = qobject_cast<QTimer*>(sender());
    QNetworkReply *pReply = m_timerReplyHash.value(pTimer);

    m_timerReplyHash.remove(pTimer);

    if(m_replyArgsHash.contains(reply)) {
        QMap<QString, QString> replyArgs = m_replyArgsHash.value(reply);
        QList<Lesson*> lessKeyList = m_upOrDownloadFileHash.keys();

        for(auto itor = lessKeyList.begin(); itor != lessKeyList.end(); ++itor) {
            Lesson *pLesson = *itor;

            QList<QMap<QString, QString> > lessonFilesArgList = m_upOrDownloadFileHash.values(pLesson);

            for(auto argItor = lessonFilesArgList.begin(); argItor != lessonFilesArgList.end(); ++argItor) {
                if(replyArgs == *argItor) {
                    m_upOrDownloadFileHash.remove(pLesson, replyArgs);

                    break;
                }
            }
        }

        m_replyArgsHash.remove(reply);
    }


    emit ftpDownloadError(tr("time out"));

    pTimer->deleteLater();

    pReply->abort();
}

void FileTransfer::uploadTimeOut()
{
    QTimer *pTimer = qobject_cast<QTimer*>(sender());
    QNetworkReply *pReply = m_timerReplyHash.value(pTimer);
    m_timerReplyHash.remove(pTimer);

    if(m_replyArgsHash.contains(pReply)) {
        QMap<QString, QString> replyArgs = m_replyArgsHash.value(pReply);
        QList<Lesson*> lessKeyList = m_upOrDownloadFileHash.keys();

        for(auto itor = lessKeyList.begin(); itor != lessKeyList.end(); ++itor) {
            Lesson *pLesson = *itor;

            QList<QMap<QString, QString> > lessonFilesArgList = m_upOrDownloadFileHash.values(pLesson);

            for(auto argItor = lessonFilesArgList.begin(); argItor != lessonFilesArgList.end(); ++argItor) {
                if(replyArgs == *argItor) {
                    m_upOrDownloadFileHash.remove(pLesson, replyArgs);

                    break;
                }
            }
        }

        m_replyArgsHash.remove(pReply);
    }

    QFile *pFile = m_uploadReplyFileMap.value(pReply);
    QString sFilePathName;

    if(pFile != Q_NULLPTR) {

        sFilePathName = pFile->fileName();

        pFile->close();
        delete pFile;
        pFile = Q_NULLPTR;
    }

    emit uploadFileError(sFilePathName, tr("time out"));

    pTimer->deleteLater();

    //必须放后面.
    pReply->abort();
}

void FileTransfer::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QTimer *pTimer = qobject_cast<QTimer*>(m_timerReplyHash.key(reply));

    if(pTimer != Q_NULLPTR) {
        m_timerReplyHash.remove(pTimer);

        //        qDebug() << "down load time out";

        if(pTimer != Q_NULLPTR) {
            delete pTimer;
            pTimer = Q_NULLPTR;
        }
    }
}

bool FileTransfer::setArguments(const QMap<QString, QString> &arg, Lesson *pLesson)
{
    if(m_upOrDownloadFileHash.count(pLesson, arg) > 0) {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(0, tr("教师客户端"), tr("该文件已在下载队列中."));
#else
        MessageDisplayWidget::information(0, tr("教师客户端"), tr("该文件已在下载队列中."));
#endif

        return false;
    }

    filearguments = arg;
    m_upOrDownloadFileHash.insertMulti(pLesson, arg);

    qDebug() << "the file argments is:" << filearguments;

    return true;
}

void FileTransfer::handleDwonloadFiles(Lesson *pLesson)
{
    if(m_upOrDownloadFileHash.contains(pLesson)) {
        QList<QMap<QString, QString> > argsMapList = m_upOrDownloadFileHash.values(pLesson);
        for(auto argsItor = argsMapList.begin(); argsItor != argsMapList.end(); ++argsItor) {
            QMap<QString, QString> argsMap = *argsItor;
            QNetworkReply *reply = m_replyArgsHash.key(argsMap);
            reply->abort();

            m_upOrDownloadFileHash.remove(pLesson);

            //            --Arg::sDownLoadFileCount;
            //            emit downloadFinished();
        }
    }
}

void FileTransfer::handleUploadFiles(Lesson *pLesson)
{
    if(m_upOrDownloadFileHash.contains(pLesson)) {
        QList<QMap<QString, QString> > argsMapList = m_upOrDownloadFileHash.values(pLesson);
        for(auto argsItor = argsMapList.begin(); argsItor != argsMapList.end(); ++argsItor) {
            QMap<QString, QString> argsMap = *argsItor;
            QNetworkReply *reply = m_replyArgsHash.key(argsMap);
            reply->abort();

            m_upOrDownloadFileHash.remove(pLesson);

            //            emit uploadFileFinished();
        }
    }
}

void FileTransfer::abortTransfer()
{
//    if(!m_replyArgsHash.isEmpty()) {
//        QList<QNetworkReply*> replyList = m_replyArgsHash.keys();

//        for(auto itor = replyList.begin(); itor != replyList.end(); ++itor) {
//            (*itor)->abort();
//        }
//    }
}

FileTransfer::finished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());

    pReply->deleteLater();
}

FileTransfer::FileTransfer(QObject *parent) :
    QObject(parent)
{
//    connect(CCU::ccu, &CCU::transferFileAbort, this, &FileTransfer::abortTransfer);
}

FileTransfer::~FileTransfer()
{
//    m_upOrDownloadFileHash.clear();
//    m_replyArgsHash.clear();
//    m_replyResDBMap.clear();

//    QList<QTimer*> timerList = m_timerReplyHash.keys();
//    qDeleteAll(timerList);
//    m_timerReplyHash.clear();

    if(!m_replyArgsHash.isEmpty()) {
        QList<QNetworkReply*> replyList = m_replyArgsHash.keys();
        m_replyArgsHash.clear();
        for(int index = 0; index < replyList.size(); ++index) {
            QNetworkReply *pReply = replyList.at(index);
            pReply->abort();

            m_bAbort = true;
        }
    }


//    if(accessManager != Q_NULLPTR) {
//        delete accessManager;
//        accessManager = Q_NULLPTR;
//    }

//    if(manager != Q_NULLPTR) {
//        delete manager;
//        manager = Q_NULLPTR;
//    }

    manager->deleteLater();
    accessManager->deleteLater();
}

FileTransfer::FileTransfer(const QMap<QString, QString> &arguments, QDialog *dialog):
    QObject()
{
    this->filearguments = arguments;
    this->dialog = dialog;

//    connect(CCU::ccu, &CCU::transferFileAbort, this, &FileTransfer::abortTransfer);
}

FileTransfer::FileTransfer(const QMap<QString, QString> &arguments, QObject *parent):
    QObject(parent)
{
    this->filearguments = arguments;
//    connect(CCU::ccu, &CCU::transferFileAbort, this, &FileTransfer::abortTransfer);
}

//高峰修改  2016/12/1
void FileTransfer::ftpUpload(const QMap<QString, QString> &filePath, Lesson *pLesson, bool bCreateUpload)
{
    if(m_upOrDownloadFileHash.count(pLesson, filePath) > 0) {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(0, tr("教师客户端"), tr("该文件已在上传队列中."));
#else
        MessageDisplayWidget::information(0, tr("教师客户端"), tr("该文件已在上传队列中."));
#endif
        return;
    }

    //向ftp服务器上传文件
    QMap<QString, QString> arguments;

    qDebug()<<filePath["fileName"]<<endl;
    QList<QString> strlist=filePath["fileName"].split("/");
    QString detailID = filePath["detailID"]; //从前端获得detailID
    QString date = filePath["date"]; //从前端获取文件所属课程的上课时间

    //拼装ftp上存储文件的新名字
    QString fileNewName = "";
    QFileInfo fileinfo = QFileInfo(filePath["localFilePath"]);
    QList<QString> filenameList=strlist.takeAt(1).split(".");
    fileNewName = QString::number(Arg::userId) + detailID + date;
    fileNewName.append(filenameList.takeAt(0));
    fileNewName.append(".");
    fileNewName.append(fileinfo.suffix());

    qDebug()<<fileNewName<<endl;

    //***********2016/12/8 修改  等待接口
    arguments.insert("username", "user3");
    arguments.insert("password", "123456");
    arguments.insert("host", Arg::IP);
    arguments.insert("path",fileNewName);
    arguments.insert("filePath",filePath["localFilePath"]);

    if(manager == Q_NULLPTR) {
        manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ftpUploadReplyFinished(QNetworkReply*)));
        connect(manager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
                this, SLOT(slotAuthenticationRequired(QNetworkReply*, QAuthenticator*)));
    }

    qDebug()<<"filePath: "<<arguments["filePath"]<<endl;
    QFile *pLoadFile = new QFile(arguments["filePath"]);

    if(pLoadFile->open(QIODevice::ReadOnly)) {

        ///Mark todo
        ResFilesDB* resfile = Q_NULLPTR;

        if(bCreateUpload) {
            //向数据库资源表内插入记录
            //        ResFilesDB *pResFileDB = DataClassInstanceManage::getInstance()->getResFilesDBPtr();
            resfile = new ResFilesDB(this);
            connect(resfile, SIGNAL(addFinish(const QJsonObject&)), this, SLOT(addFinished(const QJsonObject&)), Qt::UniqueConnection);
            resfile->setLessonDetailId(detailID.toInt());
            resfile->setRemark(tr(""));
            resfile->setUpdateTime(QDate::currentDate());
            resfile->setUseDataTime(QDate::fromString(date, "yyyy-M-d"));
            resfile->setState(1);
            //url的拼装
            QString sResUrl = QString("%1/%2").arg(Arg::username).arg(fileNewName);
            resfile->setFileUrl(sResUrl);
            resfile->setFileName(filePath["fileName"].split("/").takeAt(1));
            //        resfile->add();
        }
        //        QByteArray data = file.readAll();


#ifdef FTP_USE
        QUrl url;
        url.setScheme("ftp");
        url.setPath(arguments["path"]);
        url.setHost(arguments["host"]);
        url.setUserName(arguments["username"]);
        url.setPassword(arguments["password"]);
        url.setPort(2121);//设置URL的端口.
#else
        //http://101.200.176.87:8080/Desktop/clientUploadFile?filePath=/c/v/b/&userId=16&detailID=253&date=2017-3-23&token=111111&usernamelilu
        QString uPath ="clientUploadFile";
        QString uFilePath ="?filePath="+QString("/")+QString("upload")+QString("/")+Arg::username+QString("/");
        QString uQuery =QString("&userId=") + QString::number(Arg::userId) +
                QString("&detailID=") + detailID +
                QString("&date=") + date +
                QString("&token=111111")+
                QString("&username=") + Arg::username+
                QString("&fileName=") + arguments["path"];

        QString sServerConfig;
        Arg *pArg = Arg::getInstance();
        pArg->getNetReusetHostUrl(sServerConfig);

        QString address =QString("%1\%2\%3\%4").arg(sServerConfig).arg(uPath).arg(uFilePath).arg(uQuery);
        QUrl url(address);
        qDebug()<<"--------url:"<<address;

#endif
        QTimer *pTimer = new QTimer(this);
        connect(pTimer, &QTimer::timeout, this, &FileTransfer::uploadTimeOut);

        qDebug()<<"test: "<<url.path()<<endl;
        ++Arg::sUpLoadFileCount;

        qDebug()<<"---------sUpLoadFileCount++="<<Arg::sUpLoadFileCount;
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
        reply = manager->post(request, pLoadFile);

        m_timerReplyHash.insert(pTimer, reply);
        pTimer->start(300000);
        m_upOrDownloadFileHash.insertMulti(pLesson, filePath);
        m_replyArgsHash.insert(reply, filePath);

        if(resfile !=  Q_NULLPTR) {
            m_replyResDBMap.insert(reply, resfile);
            m_resDBAndLessonMap.insert(resfile, pLesson);
        }

        m_uploadReplyFileMap.insert(reply, pLoadFile);

        connect(reply, SIGNAL(uploadProgress(qint64, qint64)), this, SLOT(uploadProgress(qint64,qint64)));
        //        connect(reply, SIGNAL(finished()), this, SLOT(uploadFinished()));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(slotError(QNetworkReply::NetworkError)), Qt::UniqueConnection);
    }
    else {
        delete m_uploadFile;
        m_uploadFile = Q_NULLPTR;
    }
}

void FileTransfer::uploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    QTimer *pTimer = qobject_cast<QTimer*>(m_timerReplyHash.key(reply));

    if(pTimer != Q_NULLPTR) {
        m_timerReplyHash.remove(pTimer);

        if(pTimer != Q_NULLPTR) {
            delete pTimer;
            pTimer = Q_NULLPTR;
        }
    }
    //    qreal fValue = static_cast<qreal>(bytesSent) / static_cast<qreal>(bytesTotal);
    if(bytesSent == bytesTotal) {

//        emit uploadFileFinished();
        ///TODO,进度显示相关.
#ifndef FTP_USE //FTP是单线程传输，跟http不一样，http可能多次进入这个函数.

#else

#endif

        qDebug() << "upload complete";
    }
}

void FileTransfer::addFinished(const QJsonObject &jo)
{
    ResFilesDB *pSender = dynamic_cast<ResFilesDB*>(sender());
    if(!m_resDBAndLessonMap.contains(pSender)) {
        delete pSender;
        pSender = Q_NULLPTR;
        return;
    }

    Lesson *pLesson = m_resDBAndLessonMap.value(pSender);
    if(pLesson == Q_NULLPTR) {
        return;
    }

    QString filename = jo["filename"].toString();
    int fileid = jo["id"].toInt();
    qDebug()<<"filename: "<<filename<<" fileid: "<<fileid<<endl;

    QPair<int, QString> course;
    course.first = pLesson->getLessonDetailId();
    course.second = pLesson->getDate().toString("yyyy-M-d");

    if(pLesson->name2File.contains(filename)) {
        File *pFile = pLesson->name2File.value(filename);
        pFile->setFileID(fileid);
    }

    QNetworkReply *reply = m_replyResDBMap.key(pSender);
    if(m_replyArgsHash.contains(reply)) {
        QMap<QString, QString> replyArgs = m_replyArgsHash.value(reply);
        QList<Lesson*> lessKeyList = m_upOrDownloadFileHash.keys();

        for(auto itor = lessKeyList.begin(); itor != lessKeyList.end(); ++itor) {
            Lesson *pLesson = *itor;

            QList<QMap<QString, QString> > lessonFilesArgList = m_upOrDownloadFileHash.values(pLesson);

            for(auto argItor = lessonFilesArgList.begin(); argItor != lessonFilesArgList.end(); ++argItor) {
                if(replyArgs == *argItor) {
                    m_upOrDownloadFileHash.remove(pLesson, replyArgs);

                    break;
                }
            }
        }
    }

    m_replyResDBMap.remove(reply);
    m_replyArgsHash.remove(reply);
//    reply->deleteLater();

    //        qDebug()<<"first: "<<course.first<<" second: "<<course.second<<endl;
    ///Replace by following code.
    //        Arg::fileIdMap[course].insert(filename, fileid);
//    QMap<QString, int> fileIDMap;
//    fileIDMap.insert(filename, fileid);
//    Arg::fileIdMap.insert(course, fileIDMap);
//    ///


    //删除resfiledb.
    pSender->deleteLater();
}


//2016/12/2 高峰创建
void FileTransfer::getArguments(QMap<QString, QString> argument)
{
    qDebug()<<"come in getArguments"<<endl;

    arguments=argument;
}

void FileTransfer::slotAuthenticationRequired(QNetworkReply *reply1, QAuthenticator *authenticator)
{
    if(reply1 == reply) {
        authenticator->setUser("user3");
        authenticator->setPassword("123456");
    }
}

void FileTransfer::readyRead()
{
    qDebug() << "ready read";

}

void FileTransfer::httpDownload(){

    QUrl url("http://imgstore.cdn.sogou.com/app/a/100540002/503008.png");
    QFileInfo fileInfo= url.path();
    file= new QFile(fileInfo.fileName());
    file->open(QIODevice::WriteOnly);//只读方式打开文件
    accessManager=new QNetworkAccessManager(this);
    request.setUrl(url);

    reply=accessManager->get(request);//通过发送数据，返回值保存在reply指针里.
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(httpReplyFinished(QNetworkReply*)));//finish为manager自带的信号，replyFinished是自定义的
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), dialog, SLOT( onDownloadProgress(qint64 ,qint64 )));//download文件时进度
    connect((QObject *)reply, SIGNAL(readyRead()),this, SLOT(onReadyRead()));
}


void FileTransfer::httpReplyFinished(QNetworkReply *reply){
    qDebug()<<"come in http reply finished"<<endl;
    QByteArray bytes = reply->readAll();  //获取字节
    QString result(bytes);  //转化为字符串
    qDebug()<<result;

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();  //获取字节
        QString result(bytes);  //转化为字符串
        qDebug()<<result;
    }
    else
    {
        //处理错误
    }
    reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;
}


void FileTransfer::onReadyRead(){
    file->write(reply->readAll());
    qDebug()<<"file size: "<<file->size()<<endl;
    //file->close();
}
