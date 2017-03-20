#ifndef UPLOADFILES2FTP_H
#define UPLOADFILES2FTP_H

#include <QObject>
#include "filetransfer.h"
#include "lesson.h"

class UploadFiles2Ftp : public QObject
{
    Q_OBJECT
public:
    explicit UploadFiles2Ftp();

    void setLesson(Lesson* les)
    {
        this->lesson = les;
    }

    Lesson* getLesson()
    {
        return lesson;
    }

    void handleUploadFiles(Lesson *pLesson);

//    void updateFileToFtp(const QMap<QString, QString> &filePath);

private:
    FileTransfer* transfer = Q_NULLPTR;
    Lesson* lesson;

signals:
    void start();
    void send_arguments(QMap<QString, QString>);

    void uploadFinished();

    void fileUploadFinished(const QString &fileName);

    void uploadError(const QString &sFilePathName, const QString &fileError);

public slots:
    void up2ftp(const QMap<QString, QString> &, bool bCreatUpload = true);
    void uiState(QJsonObject jo);
};

#endif // UPLOADFILES2FTP_H
