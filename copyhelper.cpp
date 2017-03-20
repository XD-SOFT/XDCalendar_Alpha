#include "copyhelper.h"
#include "qtfilecopier.h"
#include "wavewidget.h"
#include <cstdlib>
#include <QDebug>
#include <QDir>
#include <ctime>
#include <QFile>
#include "wavewidget.h"

CopyHelper::CopyHelper(QObject* parent) : QObject(parent)
{

}

QString CopyHelper::store(const QString& source, const QString& fileName)
{
    //QDir::current().mkdir("files");
    //QString dest = QString("%1/files/%2_%3").arg(QDir::currentPath(), randStr(), fileName);
    QString dest = fileName;
    QtFileCopier* copier = new QtFileCopier();
    copier->setProgressInterval(1);

//    WaveWidget* wave = new WaveWidget;
//    wave->show();

//    qreal length = QFile(source).size();

//    connect(copier, &QtFileCopier::dataTransferProgress, [wave, length](long id, qint64 pro){
//        wave->setProgress(pro/length);
//    });

//    connect(copier, &QtFileCopier::done, [copier, wave]{
//        wave->close();
//        wave->deleteLater();
//        copier->deleteLater();
//    });

    copier->copy(source, dest);

    return dest;
}

void CopyHelper::get(const QString& dest, const QString& from)
{
    QtFileCopier* copier = new QtFileCopier();
    copier->setProgressInterval(1);

    WaveWidget* wave = new WaveWidget;
    wave->show();

    qreal length = QFile(from).size();

    connect(copier, &QtFileCopier::dataTransferProgress, [wave, length](long id, qint64 pro){
        wave->setProgress(pro/length);
    });

    connect(copier, &QtFileCopier::done, [copier, wave]{
        wave->close();
        wave->deleteLater();
        copier->deleteLater();
    });

    copier->copy(from, dest);
}

QString CopyHelper::randStr(int digits)
{
    srand(time(NULL));
    QString str;
    for (int i = 0; i < digits; i++) {
        str.append(QChar('A' + rand()%26));
    }
    return str;
}
