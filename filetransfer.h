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
#include <QRunnable>
#include <QQueue>

class ResFilesDB;
class QDialog;
class QFileInfo;
class QFile;
class Lesson;
class QTimer;
class QNetworkReply;
class QEventLoop;

typedef QMap<QString, QString> InvokableQMap;
Q_DECLARE_METATYPE(InvokableQMap)

class HttpDownloadRunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:
    /*!
     *\brief 设置反射对象,用这个通过Q_INVOKABLE来反射得到转换后的资源参数.
     *\param pInvokableObjPtr.
    */
    HttpDownloadRunnable(const QMap<QString, QString> &arguments, QObject *pInvokableObj, QObject *parent = Q_NULLPTR);
    ~HttpDownloadRunnable();

    void setDownloadUrl(const QString &url);

protected:
    /*!
     *\brief 在实现类中实现对不同类型的资源的转换.
    */
    void run() final;

private slots:
    void authenticationNetworkAcessManager(QNetworkReply *reply1, QAuthenticator *authenticator);

    void downloadFinished(QNetworkReply *reply);

    void processDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    void downloadTimeOut();

    void slotError(QNetworkReply::NetworkError error);

    void writeData();

private:
    QObject *m_pInvokableObj;
    QString m_sDownloadUrl;
    QNetworkAccessManager *m_pNetworkAccessMgr;
    QMap<QString, QString> m_arguments;

    ///上一次收到的所有bytes.
    qint64 m_nLastTotalReceivedBytes;

//    QTimer *m_pTimer;

    QEventLoop *m_pEvLoop;

    QFile *m_pDownloadFile;
};

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

    Q_INVOKABLE void httpDownloadError(const InvokableQMap &arguments, const QString &sError);

    ///Todo,这个有待确定.
    Q_INVOKABLE void httpDownloadProgress(const InvokableQMap &arguments, qint64 bytesReceived, qint64 bytesTotal);

    Q_INVOKABLE void httpDownLoadFinished(const InvokableQMap &arguments);

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

    //课程上传文件哈希表.
    QMultiHash<Lesson*, QMap<QString, QString> > m_uploadFileHash;

    //课程下载课程与文件哈希表.
    QMultiHash<Lesson*, QMap<QString, QString> > m_downloadFileHash;

    QHash<QNetworkReply*, QMap<QString, QString> > m_replyArgsHash;

    QHash<QTimer*, QNetworkReply*> m_timerReplyHash;

//    ///http下载参数list,http下载改为多线程，暂时这么处理.
//    /// 上传的暂时不改变.
//    QList<QMap<QString, QString> > m_dwonloadArgList;

    QMap<QNetworkReply*, QString > m_downloadReplayMap;

    //下载文件参数队列.
    QQueue<QMap<QString, QString> > m_downloadArgsQueue;

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
