#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include "filetransfer.h"

///Mark, 这个类多余的,重构去掉.
class Lesson;
class downloadFile : public QObject
{
    Q_OBJECT
public:
    explicit downloadFile();


    void handleDownloadFiles(Lesson *pLesson);

private:
    FileTransfer* transfer = Q_NULLPTR;

signals:
    void start();
    void send_arguments(QMap<QString, QString>);

    void dowLoadComplete(const QString &sError = QString());

public slots:
    ///pLesson == Q_NULLPTR的时候为启动时候下载.
    void download(const QMap<QString, QString> &, Lesson *pLesson);
    void uiState(QJsonObject jo);

private slots:
    void downLoadError(const QString &sError);
};

#endif // DOWNLOADFILE_H
