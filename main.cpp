//#pragma execution_character_set("utf-8")
#include <QApplication>
#include "config.h"
#include "ccu.h"
#include "logindialog.h"
#include "singleobjectmanager.h"
#include <QTextCodec>
#include <QSettings>
#include <iostream>
#include <qtsingleapplication.h>
#include <QProcess>
//class Arg;

//#define USE_RUNNING_LOG
#ifdef USE_RUNNING_LOG
QFile g_logFile("./runlog/MoonCalendar.log");
#endif

void LogManagement(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
#define USE_DEBUG_INFORMATION
#ifdef USE_DEBUG_INFORMATION
    QByteArray localMsg = msg.toLocal8Bit();
    QString sLogStr;
    switch (type) {
    case QtDebugMsg:
        //        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sLogStr.append("Debug:");

        break;
        //    case QtWarningMsg:
        ////        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        //        sLogStr.append("Warning:");
        break;
    case QtCriticalMsg:
        //        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sLogStr.append("Critical:");
        break;
    case QtFatalMsg:
        //        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
        sLogStr.append("Fatal:");
        abort();

    default:
        break;
    }

    sLogStr.append(QDateTime::currentDateTime().toString());
    sLogStr.append(":");
    sLogStr.append(localMsg.constData());
    sLogStr.append(",");
    sLogStr.append(context.file);
    sLogStr.append(",");
    sLogStr.append(context.line);
    sLogStr.append(",");
    sLogStr.append(context.function);
    sLogStr.append("\r\n");

    //#define USE_RUNNING_LOG
#ifdef USE_RUNNING_LOG
    QTextStream logStream(&g_logFile);
    logStream.setAutoDetectUnicode(true);
    logStream << sLogStr;
#endif
    //#else
    //   fprintf(stderr, "%s", sLogStr.toLocal8Bit());
    //#endif

#endif
}


int main(int argc, char *argv[])
{
    //    QApplication a(argc, argv);
    QtSingleApplication a(argc, argv);
    if(a.isRunning()) {
        //        QMessageBox::information(0, QObject::tr("教师客户端"), QObject::tr("已有一个程序实例在运行!"));
        a.sendMessage("");
        return 0;
    }

#ifdef USE_RUNNING_LOG
    if(g_logFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        //        g_logStream.setDevice(&g_logFile);

        qInstallMessageHandler(LogManagement);
    }
#endif

    QFont font;
    font.setFamily("Microsoft YaHei");
    font.setPointSize(10);
    a.setFont(font);

    //    qDebug() << "font settings end!";

    //设置qss默认样式表
    ///Mark 2017.01.17,样式表重构时候考虑放进源码中用一个文件管理，比单独编辑txt文件方便.
    QFile f(":/concise.qss");
    if(f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream txtInput(&f);
        QString qss = txtInput.readAll();
        qApp->setStyleSheet(qss);
    }

    //    qDebug() << "load style sheet end.";

#ifdef USE_TEST
    //    Arg::nw->getBaseData();
    //    CCU c;
    //    c.startWork();
    //创建单一实例管理类.
    //    ///MARK,这个重构时候要重新设计.
    //    SingleObjectManager::getInstance();
#else
    //-------Login Dialog-------//
    CCU* c = new CCU();


    QString appPath = qApp->applicationDirPath();

    //如果更新程序存,执行更新程序
    if (QFile::exists(appPath + QDir::separator() + "mcdudp.exe"))
    {
        //如过不是更新程序启动当前应用程序（没传入参数），执行更新，否则已经是最新，直接启动
        if(argc  == 1)
        {
            QString serverAddress;
            Arg *pArg =  Arg::getInstance();
            pArg->getNetReusetHostUrl(serverAddress);

            QProcess process;

            QStringList arguments;
            arguments <<serverAddress;
            qDebug()<<"--appPath" <<appPath + QDir::separator() + "mcdudp.exe";
            qDebug()<<"serverAddress"<<serverAddress;

            process.startDetached(appPath + QDir::separator() + "mcdudp.exe", arguments, appPath);
            qDebug()<< "--is start" <<process.waitForStarted();

            return 0;
        }else {

            QProcess process;
            process.execute("taskkill /im mcdudp.exe /f");
            process.waitForFinished();

        }
    }else {

        qDebug()<<"updater.exe is not find!";
    }

    loginDialog* login = new loginDialog(nullptr, c);
    //    qDebug() << "loginDialog initialized end!";
    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), login, SLOT(onAppStartMessageReceived(const QString&)));
    login->show();

    //--------------------------//
    //    qDebug() << "login dialog show!";
#endif

    int nResult =  a.exec();

#ifndef USE_TEST
    delete c;
    c = Q_NULLPTR;

    delete login;
    login = Q_NULLPTR;
#endif


    //    //销毁.
    //    SingleObjectManager::destroyInstance();

    return nResult;
}
