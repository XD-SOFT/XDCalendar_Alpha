#include "dialog.h"
#include "messagedisplaywidget.h"
#include<QtDebug>
#include <QTextCodec>
#include <QMap>
#include <QJsonDocument>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    download_Btn=new QPushButton("download");
    progressBar=new QProgressBar;
    label = new QLabel;
    layout=new QVBoxLayout();
    layout->addWidget(progressBar);
    layout->addWidget(download_Btn);
    setLayout(layout);

    connect(download_Btn,SIGNAL(clicked()),this,SLOT(startOp()));

}

void Dialog::startOp()
{
    testMap.insert("detailID","2");
    testMap.insert("folderName","周一 7月 21 1997-4");
    testMap.insert("fileName","1.txt");

    if(down == Q_NULLPTR) {
        down  = new downloadFile();
//        connect(this, SIGNAL(localFinish(QMap<QString, QString>)), down, SLOT(download(QMap<QString, QString>)));
    }

//    emit localFinish(testMap);


//    down->download(testMap);
}


void Dialog::uiState(QJsonObject jo)
{
     this->close();
#ifdef USER_QT_MESSAGEBOX
     QMessageBox::information(this, tr("教师客户端"), QString(QJsonDocument(jo).toJson()));
#else
    MessageDisplayWidget::showMessage(tr("教师客户端"), QString(QJsonDocument(jo).toJson()));
#endif
}

Dialog::~Dialog()
{
    if(down != Q_NULLPTR) {
        delete down;
        down = Q_NULLPTR;
    }
}

void Dialog::onDownloadProgress(qint64 bytesSent, qint64 bytesTotal){
    progressBar->setMaximum(bytesTotal);
    progressBar->setValue(bytesSent);

}




