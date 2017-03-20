#ifndef WIDGET_H
#define WIDGET_H

//#include <QtWidgets>
#include "baseshadowwidget.h"
#include <array>
#include <QSystemTrayIcon>
#include <QDate>
//#include "daywidget.h"
//#include "mybutton.h"
//#include "config.h"
//#include "dragbox.h"
//#include "gridviewweek.h"
//#include "filelistwidget.h"
//#include "termwidget.h"
//顶部的月份选择
class LookAndFeelConfigDialog;
class Sidebar;
class dlgConfig;
class InfoAndSettingsStackWidget;
class UserInfoWidget;
class TransparentButton;
class QHBoxLayout;
class FileListWidget;
class QGridLayout;
class QSpacerItem;
class TermWidget;
class QPushButton;
class DraggableBox;
class Lesson;

class MonthChoose : public QWidget
{
    Q_OBJECT
public:
    explicit MonthChoose(QWidget *parent = 0);
    ~MonthChoose(){}
private:
    QDate date;
    QHBoxLayout *MainLayout;
    TransparentButton *LeftButton,*RightButton;
    TransparentButton *YearButton,*MonthButton;

    void setLabel();

signals:
    void dateChange(QDate);

public slots:
    void setMonth(const QDate &date_data);

    void TurnLeft()
    {
        setMonth(date.addMonths(-1));
    }
    void TurnRight()
    {
        setMonth(date.addMonths(1));
    }
};

class GridViewWeek;
class UserInfoView;
class SettingsWidget;

//主窗口
class MainScreen : public BaseShadowWidget
{
    Q_OBJECT

public:
    friend class CCU;
    FileListWidget* fileListWidget;

//    QWidget *centralWidget;//all compositions is in this widget

    QGridLayout *MainGridLayout = nullptr;

    QWidget *TopLine;//
    QHBoxLayout *TopLineLayout;
    QSpacerItem *horizontalSpacer;
    TransparentButton *TopSetup,*TopMore,*ResizeButton,*mConfigLookAndFeel, *TopExit;
    TransparentButton *mFixButton, *mBlockButton, *mCloseButton, *mMinButton;

    QWidget* semesterChooser;
//    QPushButton *lastSemester, *nextSemester;
    QPushButton *setSemester, *setSchedule , *testButton;

    TermWidget *gridView;
    GridViewWeek *dayView;

    UserInfoWidget *userInfoView;


//    Sidebar* tabWidget;
//    MonthChoose *monthchoose;

    DraggableBox *resizeDragBoudingBox;
    QSystemTrayIcon *system_tray;
    static MainScreen *mainScreen;
    //static MainScreen *dayScreen;
    void setupUi();
    QSize sizeHint() const;
    void onedayMode (bool on);
    void retranslateUi();
    // retranslateUi

    //对建好的元件进行排版
    void composing();
    explicit MainScreen(/*QWidget *parent = 0 */ );
    ~MainScreen();

    void getCurretTimeLesson(Lesson *&pLesson, int &nLeftTime);

    void reloadLesson();

//signals:
//    void mainScreenResized(const QSize &size);

signals:
    void systemLogOff();

private slots:
    void activated(QSystemTrayIcon::ActivationReason reason);
    void close();
    void setupConfig();
    void showMainMenu();
    void changeLockStatus();
    void changeLockAndStayTop();

    void showLinkedLessons();

    void resizeWindow();

    void restoreSkin();

    void logOff();

    void showAboutWindow();

private:
    QSize DefaultSize;
    bool daySchme;
    void setArg();//获得参数(可以通过默认设置或文件读取的方式)

    static int window_long;
    void unlockAndStayTop();
    void reset();//根据参数响应重绘

    void saveAll(QString);
    void loadAll(QString);
public slots:
    void StartResize();

    void showSideBar (bool on);
    void configLookAndFeel();
    void saveSlot();
    void loadSlot();
    void changeDaySchme();
    ///Replace by BiXiaoxia 2016.12.20.
//    void setBackgroundImage (const QImage& image);
    void setBackgroundImage(const QString &sImageFilePathName);

    void setBackgroundOpacity (double opacity);
    void setForegroundOpacity (double opacity);
protected:
    /*!
    @ \brief 绘制背景图片等.
    @ \author BiXiaxia
    */
    void drawExcludeShadowPart(QPainter *painter, QPaintEvent* ev);
//    void paintEvent (QPaintEvent* ev);
    void mousePressEvent (QMouseEvent* ev);
    void mouseMoveEvent (QMouseEvent* ev);
    void mouseReleaseEvent (QMouseEvent* ev);
    void enterEvent (QEvent*);
    void leaveEvent (QEvent*);

    void showEvent(QShowEvent *event);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    double mBackgroundOpacity = 1.0;
    double mForegroundOpacity= 0.67;
    QImage mBackgroundImage;
    LookAndFeelConfigDialog* mLookAndFeel = nullptr;
    QPushButton* mHideTabButton;

#ifdef SEMESTER_SETTING_ON_MAINWINDOW ///Mark，原来编辑模式需要用到一些额外的部件，都加入这个里面管理了，目前不需要用这些，所以预处理掉.
    /// < \variable m_pSemesterChooserWgt, 编辑模式ui依附的widget,临时性解决方法, 变量太多，暂时定义就在这里赋空值.
    QWidget *m_pSemesterChooserWgt = Q_NULLPTR;
#endif

#if 0
    /// < \variable m_dialog,设置类对象指针.
    dlgConfig *m_pConfigDialog = Q_NULLPTR;
#endif

    /// < \variable m_pInfoSettingsStackWgt,信息显示及设置管理类对象.
    InfoAndSettingsStackWidget *m_pInfoSettingsStackWgt = Q_NULLPTR;

    bool mPressed = false;
    QPoint mLastWindowPos, mLastMousePos;

    QPoint m_unLockPt; //没锁定前位置.

    /// < \variable 第一次显示时候的尺寸，因为用了布局控制课程网格，导致变形，暂时只能用尺寸协调手段协调.
    QSize m_firstSize;

    bool isSideBarShown = false;

    /// < \variable m_bStandardUIInitialized, 标准ui对象是否已经初始化完成，这个仅仅是临时结局方法，重构要改变.
    bool m_bStandardUIInitialized = false;

    bool m_bFirstVisible = true;

    /// < \brief 是否是单日模式.
    bool m_bOneDayMode = false;

//    /// < \variable m_bEditUIInitialized, 编辑模式ui对象是否已经初始化完成，备注m_bStandardUIInitialized.
//    bool m_bEditUIInitialized = false;
};


#endif // WIDGET_H
