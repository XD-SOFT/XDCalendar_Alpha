#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QVBoxLayout>
#include <QIODevice>
#include <QFileInfo>
#include <QLabel>
#include <QProgressBar>
#include <QThread>
#include "filetransfer.h"
#include "downloadfile.h"

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    Dialog(QMap<QString, QString> arguments, QWidget *parent = 0)
    {
        this->arguments = arguments;
        //this->dialog = dialog;
    }
    ~Dialog();
protected slots:

    void onDownloadProgress(qint64 bytesSent,qint64 bytesTotal);

    void startOp();

    void uiState(QJsonObject jo);

signals:
    void start();
    void localFinish(QMap<QString, QString>);

private:
    QLabel* label;
    QPushButton *download_Btn;
    QProgressBar *progressBar;
    QVBoxLayout *layout;
    FileTransfer* filetransfer;
    QMap<QString, QString> arguments;
    QMap <QString, QString> testMap;//filename and source path
    downloadFile* down;


};




#endif // DIALOG_H
