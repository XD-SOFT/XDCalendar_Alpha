#ifndef CCU_H
#define CCU_H
#include "config.h"
#include "MainScreen.h"
#include "minimainscreen.h"
#include <QFile>
#include <QTranslator>
#include <QObject>
#include <QApplication>
#include <QSize>
#include <QDeskTopWidget>
#include <QMessageBox>

class CCU : public QObject// central control unit
{
    Q_OBJECT
public:
    QTranslator translator;//translate language
    QTranslator configTranslator;
    MainScreen* dayScreen;

    void startWork();

    void showMainWindow();

    QWidget *getMainWindow();

    void clearDayScreen();

    static CCU* ccu;
public slots:
//    void changeToChinese()
//    {
//        configTranslator.load(":/settings_zh_CN.qm");
//        qApp->removeTranslator(&translator);
//        qApp->installTranslator(&configTranslator);
//        MainScreen::mainScreen->close();
//        delete MainScreen::mainScreen;
//        MainScreen::mainScreen = new MainScreen ();
//    }
//    void changeToEnglish()
//    {
//        qApp->installTranslator(&translator);
//        qApp->removeTranslator(&configTranslator);
//        MainScreen::mainScreen->close();
//        delete MainScreen::mainScreen;
//        MainScreen::mainScreen = new MainScreen ();
//    }

    void changeViewMode (ViewMode target);

    void close();

    void logOff();

private:
    void reset();

signals:
    void changeEdit();

    void changeToStandardMode();

    void logOffFinished();

    void transferFileAbort();
};
#endif // CCU_H
