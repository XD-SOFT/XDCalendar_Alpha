#include "uploadfiles2ftp.h"
#include "resfilesdb.h"
#include <qthread.h>
#include <QDebug>
#include <QMessageBox>
#include <QJsonObject>
#include "config.h"
#include <QJsonDocument>
#include "lesson.h"
//#include "dataclassinstancemanage.h"

using namespace std;
UploadFiles2Ftp::UploadFiles2Ftp()
{
    transfer = new FileTransfer(this);
    connect(transfer, SIGNAL(uploadFileFinished()), this, SIGNAL(uploadFinished()));
    connect(transfer, &FileTransfer::fileUploadFinished, this, &UploadFiles2Ftp::fileUploadFinished);
    connect(transfer, &FileTransfer::uploadFileError, this, &UploadFiles2Ftp::uploadError);
}

void UploadFiles2Ftp::handleUploadFiles(Lesson *pLesson)
{
    if(transfer != Q_NULLPTR) {
        transfer->handleUploadFiles(pLesson);
    }
}

//void UploadFiles2Ftp::updateFileToFtp(const QMap<QString, QString> &filePath)
//{
//    qDebug()<<"come in up2ftp"<<endl;

//    //向ftp服务器上传文件
//    QMap<QString, QString> arguments;

//    qDebug()<<filePath["fileName"]<<endl;
//    QList<QString> strlist=filePath["fileName"].split("/");
//    QString detailID = filePath["detailID"]; //从前端获得detailID
//    QString date = filePath["date"]; //从前端获取文件所属课程的上课时间

//    //拼装ftp上存储文件的新名字
//    QString fileNewName = "";
//    QFileInfo fileinfo = QFileInfo(filePath["localFilePath"]);
//    QList<QString> filenameList=strlist.takeAt(1).split(".");
//    fileNewName = QString::number(Arg::userId) + detailID + date;
//    fileNewName.append(filenameList.takeAt(0));
//    fileNewName.append(".");
//    fileNewName.append(fileinfo.suffix());

//    qDebug()<<fileNewName<<endl;

//    //***********2016/12/8 修改  等待接口
//    arguments.insert("username", "user3");
//    arguments.insert("password", "123456");
//    arguments.insert("host", Arg::IP);
//    arguments.insert("path",fileNewName);
//    arguments.insert("filePath",filePath["localFilePath"]);

//    ///Mark，这里直接调用就行.
//    transfer->ftpUpload(arguments);
//}

void UploadFiles2Ftp::up2ftp(const QMap<QString, QString> &filePath, bool bCreatUpload)
{
    qDebug()<<"come in up2ftp"<<endl;

//    //向ftp服务器上传文件
//    QMap<QString, QString> arguments;

//    qDebug()<<filePath["fileName"]<<endl;
//    QList<QString> strlist=filePath["fileName"].split("/");
//    QString detailID = filePath["detailID"]; //从前端获得detailID
//    QString date = filePath["date"]; //从前端获取文件所属课程的上课时间

//    //拼装ftp上存储文件的新名字
//    QString fileNewName = "";
//    QFileInfo fileinfo = QFileInfo(filePath["localFilePath"]);
//    QList<QString> filenameList=strlist.takeAt(1).split(".");
//    fileNewName = QString::number(Arg::userId) + detailID + date;
//    fileNewName.append(filenameList.takeAt(0));
//    fileNewName.append(".");
//    fileNewName.append(fileinfo.suffix());

//    qDebug()<<fileNewName<<endl;

//    //***********2016/12/8 修改  等待接口
//    arguments.insert("username", "user3");
//    arguments.insert("password", "123456");
//    arguments.insert("host", Arg::IP);
//    arguments.insert("path",fileNewName);
//    arguments.insert("filePath",filePath["localFilePath"]);

//    ///Mark todo

//    if(bCreatUpload) {
//        //向数据库资源表内插入记录
////        ResFilesDB *pResFileDB = DataClassInstanceManage::getInstance()->getResFilesDBPtr();
//        ResFilesDB* resfile = new ResFilesDB(this);
////        connect(pResFileDB, SIGNAL(addFinish(QJsonObject)), this, SLOT(uiState(QJsonObject)), Qt::UniqueConnection);
//        resfile->setLessonDetailId(detailID.toInt());
//        resfile->setRemark(tr(""));
//        resfile->setUpdateTime(QDate::currentDate());
//        resfile->setUseDataTime(QDate::fromString(date, "yyyy-M-d"));
//        resfile->setState(1);
//        //url的拼装
//        resfile->setFileUrl(Arg::ftpStr+fileNewName);
//        resfile->setFileName(filePath["fileName"].split("/").takeAt(1));
////        resfile->add();
//    }

    //connect(transfer, SIGNAL(updateUi(QJsonObject)), this, SLOT(uiState(QJsonObject)));
    //QThread *pThread = new QThread(this);
    //transfer->moveToThread(pThread);
    //connect(this, SIGNAL(start()), filetransfer, SLOT(ftpDownload()));
    //connect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( getArguments(QMap<QString, QString>) ));
    //semit send_arguments(arguments);

    ///Mark，这里直接调用就行.
    transfer->ftpUpload(filePath, lesson, bCreatUpload);
//    connect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( ftpUpload(QMap<QString, QString>) ));
//    //pThread->start();
//    emit send_arguments(arguments);
//    disconnect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( ftpUpload(QMap<QString, QString>) ));
    ///Mark End.
    /*
    for (it = filePath.begin(); it != filePath.end(); it++)
    {
        arguments.clear();

        QList<QString> strlist=it.key().split("/");
        QString detailID = filePath["detailID"]; //从前端获得detailID
        //拼装ftp上存储文件的新名字
        QString fileNewName = "";
        QFileInfo fileinfo = QFileInfo(it.value());
        QList<QString> filenameList=strlist.takeAt(1).split(".");
        fileNewName = detailID ;
        fileNewName.append(filenameList.takeAt(0));
        fileNewName.append(".");
        fileNewName.append(fileinfo.suffix());


        //***********2016/12/8 修改  等待接口
        arguments.insert("username", filePath["username"]);
        arguments.insert("password", filePath["password"]);
        arguments.insert("host", "localhost");
        arguments.insert("path",fileNewName);
        arguments.insert("filePath",it.value());

        //向数据库资源表内插入记录
            ResFilesDB* resfile = new ResFilesDB();
            //connect(resfile, SIGNAL(addFinish(QJsonObject)), this, SLOT(loginState(QJsonObject)) );
            resfile->setLessonDetailId(detailID.toInt());
            resfile->setRemark(tr(""));
            resfile->setUpdateTime(QDate::currentDate());
            resfile->setUseDataTime(QDate::currentDate());
            resfile->setState(1);
            //url的拼装
            resfile->setFileUrl("ftp://localhost:2121/user3/"+fileNewName);
            resfile->setFileName(it.key().split("/").takeAt(1));
            resfile->add();


        connect(transfer, SIGNAL(updateUi(QJsonObject)), this, SLOT(uiState(QJsonObject)));
        //QThread *pThread = new QThread(this);
        //transfer->moveToThread(pThread);
        //connect(this, SIGNAL(start()), filetransfer, SLOT(ftpDownload()));
        //connect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( getArguments(QMap<QString, QString>) ));
        //semit send_arguments(arguments);

        connect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( ftpUpload(QMap<QString, QString>) ));
        //pThread->start();
        emit send_arguments(arguments);
        disconnect(this, SIGNAL(send_arguments(QMap<QString, QString>)), transfer, SLOT( ftpUpload(QMap<QString, QString>) ));
    }*/
}

void UploadFiles2Ftp::uiState(QJsonObject jo)
{

    qDebug()<<"upload result: "<<jo<<endl;
    QString filename = jo["filename"].toString();
    int fileid = jo["id"].toInt();
    qDebug()<<"filename: "<<filename<<" fileid: "<<fileid<<endl;

    if(lesson == nullptr)
    {
        qDebug()<<"lesson is null in uploadFiles2Ftp"<<endl;
        return;
    }
    else
    {
        QPair<int, QString> course;
        course.first = lesson->getLessonDetailId();
        course.second = lesson->getDate().toString("yyyy-M-d");

//        qDebug()<<"first: "<<course.first<<" second: "<<course.second<<endl;
        ///Replace by following code.
//        Arg::fileIdMap[course].insert(filename, fileid);
        QMap<QString, int> fileIDMap;
        fileIDMap.insert(filename, fileid);
        Arg::fileIdMap.insert(course, fileIDMap);
        ///
    }

    //QMessageBox::information(this, tr("提示"), QString(QJsonDocument(jo).toJson()));
}
