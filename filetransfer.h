#ifndef FILETRANSFER_H
#define FILETRANSFER_H

#include <QObject>
//#include <QNetworkAccessManager>
//#include <QFile>
#include <QNetworkReply>
//#include <QNetworkRequest>
//#include <QIODevice>
//#include <QFileInfo>
#include <QMap>
#include <QJsonObject>

class ResFilesDB;
class QDialog;
class QFileInfo;
class QFile;
class Lesson;
class QTimer;
class QNetworkReply;

class FileTransfer : public QObject
{
    Q_OBJECT
public:
    FileTransfer(QObject *parent);

    ~FileTransfer();


    FileTransfer(const QMap<QString, QString> &arguments, QDialog* dialog);


    FileTransfer(const QMap<QString, QString> &arguments, QObject *parent);

    void ftpUpload(const QMap<QString, QString> &filePath, Lesson *pLesson, bool bCreateUpload = true);

    void ftpDownload();

    bool setArguments(const QMap<QString, QString> &arguments, Lesson *pLesson);

    ///原来架构设计所致，只能暂时分开处理，上传下载，内部数据结构不分.
    void handleDwonloadFiles(Lesson *pLesson);

    void handleUploadFiles(Lesson *pLesson);

signals:
    void ftpDownloadError(const QString &sError);
    void transferPercent(int id, QString percent, QMap<QString, QString> fileInfo );

protected slots:

//    void ftpUpload(const QMap<QString, QString> &arguments);

    void abortTransfer();

    void httpDownload();

    void slotError(QNetworkReply::NetworkError code);
    void ftpUploadReplyFinished(QNetworkReply *reply);
    void ftpDownloadReplyFinished(QNetworkReply *reply);
    void donwLoadError(QNetworkReply::NetworkError code);


    void httpReplyFinished(QNetworkReply*reply);

    void onReadyRead();
    void getArguments(QMap<QString, QString> argument);

    void slotAuthenticationRequired(QNetworkReply*, QAuthenticator *authenticator);

    void readyRead();

    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);

    ///@brief 下载http格式的文件.
    void downloadFileByHttp(const QJsonObject &jsonObj);

private:
    QFile *file;
//    QFileInfo fileInfo;
    QNetworkAccessManager *accessManager = Q_NULLPTR;
    QNetworkAccessManager* manager = Q_NULLPTR;
    QNetworkRequest request;
    QNetworkReply *reply = Q_NULLPTR;
    bool httpRequestAbort; //请求停止的标志位,false表示允许请求
    QMap<QString, QString> arguments;
    QMap<QString, QString> filearguments;

    ///当前上传的文件.
    QFile *m_uploadFile = Q_NULLPTR;

    QDialog* dialog;

    QList<QNetworkReply*> replyList;

//    QFile m_downLoadFile; //当前下载文件.

//    QUrl m_url;

    QMap<QNetworkReply*, ResFilesDB*> m_replyResDBMap;

    QMap<ResFilesDB*, Lesson*> m_resDBAndLessonMap; //资源DB与Lesson课程对象指针map.

    QMap<QNetworkReply*, QFile*> m_uploadReplyFileMap;

    //课程上传或者下载文件哈希表.
    QMultiHash<Lesson*, QMap<QString, QString> > m_upOrDownloadFileHash;

    QHash<QNetworkReply*, QMap<QString, QString> > m_replyArgsHash;

    QHash<QTimer*, QNetworkReply*> m_timerReplyHash;

    QMap<QNetworkReply*, QString > m_downloadReplayMap;

    bool m_bAbort = false;

signals:
//    void updateUi(QJsonObject);
    void downloadFinished();

//    void downloadFileError(const QString &sError);

    void myDownloadProgress(qint64,qint64);

    void uploadFileFinished();

    void uploadFileError(const QString &sFilePathName, const QString &sError);

    void fileUploadFinished(const QString &fileName);

private slots:
    void addFinished(const QJsonObject&);

    ///Mark,下面两个函数以后重构合成一个,暂时受框架影响.
    void downloadTimeOut();

    void uploadTimeOut();

    void onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

private slots:
    finished();
};

#endif // FILETRANSFER_H
