#include "ccu.h"
#include "minimainscreen.h"
#include <QSettings>
#include "handler.h"

CCU* CCU::ccu = nullptr;

void CCU::startWork()
{
    Arg::viewMode = ViewMode::Standard;
    dayScreen = nullptr;
    CCU::ccu=this;
    qApp->setQuitOnLastWindowClosed(false);//

    ///Mark移到main函数里，因为对调用这个之前出现的窗口样式表不起作用.
//        //设置qss默认样式表
//        QFile f(":/concise.qss");
//        if(f.open(QIODevice::ReadOnly | QIODevice::Text))
//        {
//            QTextStream txtInput(&f);
//            QString qss = txtInput.readAll();
//            qApp->setStyleSheet(qss);
//        }

    configTranslator.load("./lang/settings_zh_CN.qm");
    qApp->installTranslator(&configTranslator);

//    qDebug() << "load language end!";

//        MainScreen::mainScreen = new MainScreen();
//        MainScreen::mainScreen->hide();
//        MainScreen::mainScreen->move(qApp->desktop()->width() / 2 - MainScreen::mainScreen->width() / 2,
//                                     qApp->desktop()->height() / 2 - MainScreen::mainScreen->height() / 2);
}

void CCU::showMainWindow()
{
    if(MainScreen::mainScreen == Q_NULLPTR) {
        MainScreen::mainScreen = new MainScreen();
        connect(MainScreen::mainScreen, SIGNAL(systemLogOff()), this, SLOT(logOff()));
    }
//        MainScreen::mainScreen->hide();

    MainScreen::mainScreen->show();
    MainScreen::mainScreen->move(qApp->desktop()->width() / 2 - MainScreen::mainScreen->width() / 2,
                                 qApp->desktop()->height() / 2 - MainScreen::mainScreen->height() / 2);
}

QWidget *CCU::getMainWindow()
{
//    if(MainScreen::mainScreen == Q_NULLPTR) {
//        MainScreen::mainScreen = new MainScreen();
//        connect(MainScreen::mainScreen, SIGNAL(systemLogOff()), this, SLOT(logOff()));
//        MainScreen::mainScreen->hide();
//    }

    return MainScreen::mainScreen;
}

void CCU::clearDayScreen()
{
    if (dayScreen != nullptr)delete dayScreen;
}

void CCU::changeViewMode (ViewMode target)
{
    if (Arg::viewMode == target)
    {
//        qDebug()<<"return"<<endl;
        return;
    }
    switch (Arg::viewMode) {
    case ViewMode::Standard:
//        qDebug()<<"view in standard mode"<<endl;
        if(target != ViewMode::Edit)MainScreen::mainScreen->hide ();
        break;
    case ViewMode::InClass:
//        qDebug()<<"view in inclass mode"<<endl;
        MiniMode::MainScreen::clearInstance ();
        break;
    case ViewMode::Day:
//        qDebug()<<"view in day mode"<<endl;
        MainScreen::mainScreen->hide();
        break;
    case ViewMode::Edit:
//        qDebug()<<"view in edit mode"<<endl;
        ///Mark2017.03.07,编辑完成重新加载下，目前这么一体放，只能暂时这么处理.
        MainScreen::mainScreen->onedayMode (false);
        MainScreen::mainScreen->hide ();
        MainScreen::mainScreen->reloadLesson();
        Arg::viewMode = target; //special here because edit mode needs this valve
//        emit changeEdit();
        break;
    default:
//        qDebug()<<"default"<<endl;
        break;
    }

    Arg::viewMode = target;

    switch (target) {
    case ViewMode::Standard:
    {
        //-----------wangqin change 12-12------------------//
//        qDebug()<<"target is standard"<<endl;
//        Arg::viewMode = target;
        MainScreen::mainScreen->onedayMode (false);
        MainScreen::mainScreen->setupUi();
        MainScreen::mainScreen->show ();

        emit changeToStandardMode();

    }
        break;

    case ViewMode::InClass:
    {
        MiniMode::MainScreen* ms = MiniMode::MainScreen::makeInstance (nullptr);
        ///Mark 2017.02.26，这里要求的不是点击后的当前课程，要求的是切换到这个模式当前时间点所在的课程.
//        ms->link (Arg::currentLesson ());
        Lesson *pLesson = Q_NULLPTR;
        int nLeftTime = 0;
        MainScreen::mainScreen->getCurretTimeLesson(pLesson, nLeftTime);
        ms->link(pLesson, nLeftTime);

        ///Mark，在link里已经执行了reload,外面多次执行导致多次加载bug.
//        ms->reload ();
        ///Mark 2017.01.05 by BiXiaoxia，暂时这么记录背景透明度,可以上课模式共用.
        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        qreal fOpacity = configSettings.value("BackGroundOpacity", 1.0).toReal();
        ms->setBackgroundOpacity(fOpacity);
//        fOpacity = configSettings.value("ForeGroundOpacity", 0.5).toReal();
//        ms->setForegroundOpacity(fOpacity);
        ms->show ();
        auto ag = QApplication::desktop ()->availableGeometry ();
        ms->move (ag.width () - ms->width () - 50, 50);
        break;
    }
    case ViewMode::Day:
    {
        emit changeToStandardMode();

//        qDebug()<<"target is day mode"<<endl;
        MainScreen::mainScreen->onedayMode (true);
        MainScreen::mainScreen->show ();
//        dayScreen = new MainScreen (true);
        //MainScreen::mainScreen->hide();
//        dayScreen->show();
//        qDebug()<<"day schme"<<endl;
        break;
    }
    case ViewMode::Edit:
    {
        //-----------wangqin change 12-12------------------//
        qDebug()<<"target is edit mode"<<endl;
//        Arg::viewMode = target; //special here because edit mode needs this valve

        MainScreen::mainScreen->onedayMode (false);
        MainScreen::mainScreen->setupUi();
        MainScreen::mainScreen->show ();

        emit changeEdit();
        break;
    }
     default:
        break;
    }
    //    Arg::viewMode = target;
}

void CCU::close()
{
    if((Arg::sUpLoadFileCount > 0) || (Arg::sDownLoadFileCount > 0)) {
        QMessageBox::StandardButton btn = QMessageBox::information(MainScreen::mainScreen, tr("教师客户端"), tr("存在上传或者下载的文件，确定关闭？"), QMessageBox::Ok, QMessageBox::Cancel);
        if(btn == QMessageBox::Cancel) {
            ///Mark Todo,做相关文件方面处理.
            return;
        }

//        emit transferFileAbort();
        ///Mark Todo，文件取消操作处理.
    }

//    qDebug()<<"close the MainScreen"<<endl;
    Arg::dumpUserData();
    //        MainScreen::mainScreen->close();

    delete MainScreen::mainScreen;
    MainScreen::mainScreen = Q_NULLPTR;

    ///因为http传输牵涉多线程，所以用qApp->exit或者quit都不能终止.
    exit(0);
//    qApp->quit();
}

void CCU::logOff()
{
    if((Arg::sUpLoadFileCount > 0) || (Arg::sDownLoadFileCount > 0)) {
        QMessageBox::StandardButton btn = QMessageBox::information(MainScreen::mainScreen, tr("教师客户端"),
                                                                   tr("存在上传或者下载的文件，确定注销？"),
                                                                   QMessageBox::Ok, QMessageBox::Cancel);
        if(btn == QMessageBox::Cancel) {
            ///Mark Todo, 取消上传做相关处理....

            return;
        }

//        emit transferFileAbort();
    }

//    qDebug()<<"close the MainScreen"<<endl;
    Arg::dumpUserData();
    //        MainScreen::mainScreen->close();

    delete MainScreen::mainScreen;
    MainScreen::mainScreen = Q_NULLPTR;

    ///Mark Todo，文件取消操作处理.
    reset();

    emit logOffFinished();
}

void CCU::reset()
{
    Arg::sUpLoadFileCount = 0;
    Arg::sDownLoadFileCount = 0;
    Arg::token.clear();
    Arg::tokenStr.clear();
    Arg::viewMode = ViewMode::Standard;
    Arg::username.clear();
    Arg::sNickName.clear();
    Arg::userId = -1;
    Arg::subjectMap.clear();
    Arg::noteMap.clear();
    Arg::fileIdMap.clear();
    Arg::mIsLocked = false;

    Arg::nw->reset();

    Handler *pHandler = Handler::getInstance();
    pHandler->reset();
}
