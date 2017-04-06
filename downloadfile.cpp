#include "downloadfile.h"
#include <qthread.h>
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include "config.h"

using namespace std;
downloadFile::downloadFile():
    QObject()
{

}

void downloadFile::handleDownloadFiles(Lesson *pLesson)
{
    transfer->handleDwonloadFiles(pLesson);
}

bool downloadFile::download(const QMap<QString, QString> &filePath, Lesson *pLesson)
{
    qDebug()<<"come in downloadFile"<<endl;

    QString dateString = filePath["date"] + QString("-") + filePath["section"];

    Arg *pArg = Arg::getInstance();
    QDir saveDir;
    pArg->getSaveDir(saveDir);
    QString currentApp = /*Arg::configDir*/saveDir.absolutePath();
    qDebug()<<"currentApp: "<<currentApp<<endl;

    QString DirPath = currentApp + "/SaveFile";
    QDir dir;

    if(!dir.exists(DirPath))
        qDebug()<<dir.mkdir(DirPath);
    DirPath+="/"+dateString;
    if(!dir.exists(DirPath))
        qDebug()<<dir.mkdir(DirPath);

    /*
    QString NewPath = DirPath+"/"+url.fileName();

    QString fileName = dateString+"/"+url.fileName();
    */


    QString sourcePath  = DirPath + "/" + filePath["fileName"];
    qDebug()<<"source file: "<<sourcePath<<endl;

//    ///Mark2017.03.03，这里做过处理，如果文件存在，则删除掉.
//    if(QFile(sourcePath).exists()){
//        bool bRemoveStatus = QFile::remove(sourcePath);
//        qDebug()<<"*********File Exits and delete**********"<< bRemoveStatus << endl;
//    }
//    }else{
    //从ftp服务器下载文件
    qDebug()<<"go to ftp"<<endl;
    if(transfer == Q_NULLPTR) {
        transfer = new FileTransfer(filePath, this);
        connect(transfer, SIGNAL(downloadFinished()), this, SIGNAL(dowLoadComplete()));
        connect(transfer, SIGNAL(ftpDownloadError(const QString&)), this, SLOT(downLoadError(const QString&)));
        //            connect(this, SIGNAL(start()), transfer, SLOT( ftpDownload() ));
        connect(transfer, SIGNAL(transferPercent(int,QString,QMap<QString,QString>)),
                this, SIGNAL(transferPercent(int,QString,QMap<QString,QString>)));
    }

    qDebug() << "the filePath is:" << filePath;

    bool bStatus = transfer->setArguments(filePath, pLesson);
    if(bStatus) {
        transfer->ftpDownload();
    }

    return bStatus;
    //        emit start();
//        disconnect(this, SIGNAL(start()), transfer, SLOT( ftpDownload() ));
//    }


}

void downloadFile::downLoadError(const QString &sError)
{
//    --Arg::sDownLoadFileCount;
    emit dowLoadComplete(sError);
}

void downloadFile::uiState(QJsonObject jo)
{
    //QMessageBox::information(this, tr("提示"), QString(QJsonDocument(jo).toJson()));
//    --Arg::sDownLoadFileCount;

    emit dowLoadComplete();
}
