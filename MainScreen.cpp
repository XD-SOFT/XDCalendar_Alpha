#include "MainScreen.h"
#include "sidebar.h"
#include "setupdialog.h"
#include "ccu.h"
#include "config.h"
#include "database.h"
#include "gridviewweek.h"
#include "termwidget.h"
#include "userinfoview.h"
#include "userlistwidget.h"
#include "livewidget.h"
#include "filelistwidget.h"
#include "coursecellwidget.h"
#include "lookandfeelconfigdialog.h"
#include "feedbackdialog.h"
#include "semester.h"
#include "semesterdialog.h"
#include "schedule.h"
#include "scheduledialog.h"
#include "operationbutton.h"
#include "dlgconfig.h"
#include "logindialog.h"
#include "infoandsettingsstackwidget.h"
#include <batchadddialog.h>
#include <QPainter>
#include <QPaintEvent>
#include <thread>
#include "editrightwidget.h"
#include "userinfowidget.h"
#include "daywidget.h"
#include "mybutton.h"
#include "config.h"
#include "dragbox.h"
#include "gridviewweek.h"
#include "filelistwidget.h"
#include "termwidget.h"
#include "settingswidget.h"

MainScreen* MainScreen::mainScreen = Q_NULLPTR;
int MainScreen::window_long = 0;
//#define TEST_BACKGROUND

MonthChoose::MonthChoose(QWidget *parent):
    QWidget(parent)
{
    //构造组件
    LeftButton = new TransparentButton(QIcon(":/Icon/1190912.png"),"",this);
    LeftButton->setObjectName(QStringLiteral("MonthChooseLeftButton"));
    LeftButton->setFixedSize(QSize(Const::MyButtonSize*2,Const::MyButtonSize*2));
    connect(LeftButton,SIGNAL(clicked(bool)),this,SLOT(TurnLeft()));
    RightButton = new TransparentButton(QIcon(":/Icon/1190967.png"),"",this);
    RightButton->setObjectName(QStringLiteral("MonthChooseRightButton"));
    RightButton->setFixedSize(QSize(Const::MyButtonSize*2,Const::MyButtonSize*2));
    connect(RightButton,SIGNAL(clicked(bool)),this,SLOT(TurnRight()));
    YearButton = new TransparentButton(this);
    YearButton->setObjectName(QStringLiteral("MonthChooseYearButton"));
    MonthButton = new TransparentButton(this);
    MonthButton->setObjectName(QStringLiteral("MonthChooseMonthButton"));
    //构造布局
    MainLayout = new QHBoxLayout(this);
    MainLayout->addWidget(LeftButton);
    MainLayout->addWidget(YearButton);
    MainLayout->addWidget(MonthButton);
    MainLayout->addWidget(RightButton);
    MainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    MainLayout->setMargin(0);
    MainLayout->setSpacing(0);
    setMonth(QDateTime::currentDateTime().date());
    setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
}

void MonthChoose::setLabel()
{
    YearButton->setText(QString::number(date.year())+tr("年"));
    if(date.month()<10)
        MonthButton->setText("  "+QString::number(date.month())+tr("月"));
    else
        MonthButton->setText(QString::number(date.month())+tr("月"));
}

void MonthChoose::setMonth(const QDate &date_data)
{
    if(date_data.year()!=date.year() || date_data.month()!=date.month())
    {
        date=date_data;
        setLabel();
        emit dateChange(date);
    }
}


MainScreen::MainScreen(/*QWidget *parent*/):
BaseShadowWidget()
{
    ///这个提到最前面，避免影响后面不知道的一些样式设置.
    QString sStyle = styleSheet() +  QString("QScrollBar:vertical {"
                                                       "margin:0px 0px 0px 0px;"
                                                       "border: 1px solid #dadbde;"
                                                       "border-radius: 5px;"
                                                       "background-color:#f9f9f9;"
                                                       "width:10px;"
                                                       "}"

                                                       "QScrollBar::handle:vertical {"
                                                       "border: 0px;"
                                                       "background-color:#d9d9d9;"
                                                       "margin:0px 0px 0px 0px;"
                                                       "border-radius: 5px;"
                                                       "min-height: 60px;"
                                                       "}"

                                                    "QScrollBar::handle:vertical:hover{"
                                                    "border: 0px;"
                                                    "background-color:#bfbfbf;"
                                                    "margin:0px 0px 0px 0px;"
                                                    "border-radius: 5px;"
                                                    "min-height: 60px;"
                                                    "}"
                                                 "QScrollBar::add-line:vertical {"
                                                       "border: 0px solid grey;"
                                                       "height: 0px;"
                                                       "subcontrol-position: bottom;"
                                                       "subcontrol-origin: margin;"
                                                   "}"

                                                   "QScrollBar::sub-line:vertical {"
                                                       "border: 0px solid grey;"
                                                       "height: 0px;"
                                                       "subcontrol-position: top;"
                                                       "subcontrol-origin: margin;"
                                                   "}"

                                                  ".QPushButton{background:#1250b5; \
                                                            border:1px solid #1250b5;\
                                                            color:white;}");


    setStyleSheet(sStyle); ///样式表最后要整合为一份放到源代码中管理.

    mainScreen = this;
    ///Edit by BiXiaoxia 2016.12.20.
//    setShadowEnabled(false); ///Mark 2016.12.21 by BiXiaoxia, 因为好多部件尺寸写死，暂时没法启用阴影.
//    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Window);//隐藏边框/菜单图标
    //    setSizePolicy (QSizePolicy::Minimum, QSizePolicy::Minimum);
    //    setSizePolicy (QSizePolicy::Maximum, QSizePolicy::Maximum);
#ifdef TEST_COURSE_CELL //The TEST can't run directly. Some simple changed is required.
    CourseCellWidget* ccw = new CourseCellWidget (this);
    ccw->link (Arg::user->terms ().at (0)->lessonWeek (0).front ());
    ccw->show ();
#endif
#ifdef TEST_COURSE_GRID
    CourseGrid* cg = new CourseGrid (this);
    cg->link (Arg::user->terms ().at (0), 0);
    cg->show ();
#endif

    if(QFile(QCoreApplication::applicationDirPath()+Const::defaultSavePath).exists())
    {
        loadAll(QCoreApplication::applicationDirPath()+Const::defaultSavePath);
    }
    setupUi ();
//    setAutoFillBackground (true);
//    setAttribute (Qt::WA_TranslucentBackground);
//    show();
//    lower();
//    reset();
    onedayMode (false);

#ifdef TEST_BACKGROUND
    setBackgroundImage (QImage (":/Icon/images.jpg"));
    //    setBackgroundOpacity (0.5);
    setForegroundOpacity (0.6);
#endif
//    setBackgroundOpacity (mBackgroundOpacity);
//    gridView->setBackground (mForegroundOpacity);
//    setForegroundOpacity (mForegroundOpacity);
//    connect (mLookAndFeel, SIGNAL (backgroundOpacityChanged(double)), this, SLOT (setBackgroundOpacity (double)));
//    connect (mLookAndFeel, SIGNAL (foregroundOpacityChanged(double)), this, SLOT (setForegroundOpacity (double)));
//    connect (mLookAndFeel, SIGNAL (backgroundImageChanged(QImage)), this, SLOT (setBackgroundImage (QImage)));
    showSideBar (false);

    setWindowTitle(tr("教师桌面"));
    setWindowIcon(QIcon(":/Icon/MoonCalendar.ico"));

//    setShadowEnabled(BaseShadowWidget::ShadowSide::Left, false);

    installEventFilter(this);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setFixedSize(sizeHint());
}

MainScreen::~MainScreen()
{
}

void MainScreen::getCurretTimeLesson(Lesson *&pLesson, int &nLeftTime)
{
    gridView->getCurretTimeLesson(pLesson, nLeftTime);
}

void MainScreen::reloadLesson()
{
    gridView->reloadLessons();
}


void MainScreen::setupUi()
{
    /// Mark by BiXiaoxia 2016.12.20,临时性解决方式解决切换重新构造bug.
    MainScreen::mainScreen = this;
    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainScreen"));

    setArg();
    setMouseTracking(true);

    //    this->setAttribute(Qt::WA_TranslucentBackground);//隐藏背景
    //    this->setStyleSheet("");

    ///<标准模式.
    if(!m_bStandardUIInitialized) {
//        centralWidget = new QWidget(this);
//        centralWidget->setAttribute (Qt::WA_TranslucentBackground);
//        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        //    centralWidget->setStyleSheet(QString(centralWidget->metaObject()->className())+"#"+centralWidget->objectName()+" { background-color: rgba(0,0,0,0.3) ; border: 1px solid black} ");

        ///Modified by BiXiaoxia 2016.12.21.
        MainGridLayout = new QGridLayout(this);
        MainGridLayout->setSpacing(0);
        MainGridLayout->setObjectName(QStringLiteral("MainGridLayout"));
//        setShadowEnabled(BaseShadowWidget::ShadowSide::Left, false);
        int nShadowMargin = getShadowMargin();
        MainGridLayout->setContentsMargins(nShadowMargin, nShadowMargin, nShadowMargin, nShadowMargin);

        TopLine = new QWidget(this);
        TopLine->setObjectName(QStringLiteral("TopLine"));
        TopLine->installEventFilter(this);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(TopLine->sizePolicy().hasHeightForWidth());
        TopLine->setSizePolicy(sizePolicy);
        TopLineLayout = new QHBoxLayout(TopLine);
        TopLineLayout->setSpacing(0);
        TopLineLayout->setObjectName(QStringLiteral("TopMenuLayout"));
        TopLineLayout->setContentsMargins(5, 5, 5, 5);
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        //        auto todayLabel = new QLabel (TopLine);
        //        todayLabel->setPixmap (QPixmap::fromImage (QImage(QString(":Icon/month/%1.png").arg (QDate::currentDate ().month ()))));

        //    TopSetup = new TransparentButton(QIcon(":/Icon/1177844.png"),"",TopLine);
        //    TopSetup->setToolTip("设置");
        //    TopSetup->setObjectName(QStringLiteral("TopSetup"));
        ////    TopSetup->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
        //    connect(TopSetup,SIGNAL(clicked(bool)),SLOT(setupConfig()));
        mFixButton = new TransparentButton(/*QIcon(":/Icon/Standard/01_14.png"), "", */TopLine);
        mFixButton->setStyleSheet("QPushButton{border-image: url(:/Icon/Standard/01_14.png)} \
                                  QPushButton:checked{border-image: url(:/Icon/Standard/01_15.png)}");


        connect (mFixButton, SIGNAL(toggled(bool)), this, SLOT(changeLockStatus()));
        mFixButton->setCheckable(true);
        mFixButton->setToolTip("固定");
        mFixButton->installEventFilter(this);

        mConfigLookAndFeel = new TransparentButton(QIcon(":/Icon/Standard/01_16.png"),"",TopLine);
        mConfigLookAndFeel->setObjectName(QStringLiteral("lookAndFeel"));
        mConfigLookAndFeel->setToolTip("皮肤");
        mConfigLookAndFeel->installEventFilter(this);
        //    AddEventListButton->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
        connect(mConfigLookAndFeel,SIGNAL(clicked(bool)),SLOT(configLookAndFeel()));

        //    ResizeButton = new TransparentButton(QIcon(":/Icon/4077.png"),"",TopLine);
        //    ResizeButton->setObjectName(QStringLiteral("ResizeButton"));
        //    ResizeButton->setToolTip("调整大小");
        ////    ResizeButton->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
        //    connect(ResizeButton,SIGNAL(clicked(bool)),this,SLOT(StartResize()));

        TopMore = new TransparentButton(QIcon(":/Icon/Standard/01_18.png"),"",TopLine);
        TopMore->setObjectName(QStringLiteral("TopMore"));
        TopMore->setToolTip("更多");
        TopMore->installEventFilter(this);
        connect(TopMore,SIGNAL(clicked(bool)),SLOT(showMainMenu()));
//        mBlockButton = new TransparentButton(QIcon(":/Icon/Standard/01_20.png"), "", TopLine);
        //    mFixButton->setToolTip("固定");
        mMinButton = new TransparentButton(QIcon(":/Icon/Standard/01_22.png"), "", TopLine);
        mMinButton->setToolTip("最小化");
        mMinButton->installEventFilter(this);
        connect(mMinButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
        mCloseButton = new TransparentButton(QIcon(":/Icon/Standard/01_24.png"), "", TopLine);
        mCloseButton->setToolTip("关闭");
        mCloseButton->installEventFilter(this);
        mCloseButton->setHoverdColor(QColor("#d74631"));
        connect(mCloseButton, SIGNAL(clicked(bool)), CCU::ccu, SLOT(close()));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(TopMore->sizePolicy().hasHeightForWidth());
        TopMore->setSizePolicy(sizePolicy1);
        //    TopMore->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));

//        userInfoView = new UserInfoView (this);
        userInfoView = new UserInfoWidget(this);
        userInfoView->setFixedHeight(45);
        userInfoView->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Fixed);

#ifdef SEMESTER_SETTING_ON_MAINWINDOW
        //编辑模式需要的widget在这里创建，隐藏.
        m_pSemesterChooserWgt = new  QWidget(this);
        TopLineLayout->addWidget(m_pSemesterChooserWgt);

        QHBoxLayout* semesterChooserLayout = new QHBoxLayout();
//        qDebug()<<"============current view mode is edit mode=================="<<endl;
        //---------------------学期设置按钮------------------------//
        if(Arg::getInstance()->currentSemester() != nullptr)
        {
            setSemester = new QPushButton (Arg::getInstance()->currentSemester()->getTitle(), TopLine);
            connect(Arg::getInstance(), &Arg::currentSemesterChanged, [&](Semester *s){
                setSemester->setText(s->getTitle());
                setSemester->update();
            });
        }
        else
        {
            setSemester = new QPushButton (tr("学期设置"), TopLine);
        }

        connect (setSemester, &QPushButton::clicked, [this] ()
        {
            auto semeD = new SemesterDialog (this);
            semeD->show();
        });

        setSemester->setObjectName ("SetSemester");

        //---------------------作息时间设置按钮------------------------//
        if(Arg::getInstance()->currentSchedule() != nullptr)
        {
            setSchedule = new QPushButton (Arg::getInstance()->currentSchedule()->getName(), TopLine);
            connect(Arg::getInstance(), &Arg::currentScheduleChanged, [&](Schedule *s){
                setSchedule->setText(s->getName());
                setSchedule->update();
            });
        }
        else
        {
            setSchedule = new QPushButton (tr("作息时间设置"), TopLine);
        }

        connect (setSchedule, &QPushButton::clicked, [this] ()
        {
            auto scheD = new ScheduleDialog (this);
            scheD->show ();
        });
        setSchedule->setObjectName ("SetSchedule");

        /*
        //2016/12/1修改
        testButton = new QPushButton (tr("测试"), TopLine);
        connect (testButton, &QPushButton::clicked, [this] ()
        {
            qDebug()<<"loginDialog"<<endl;
            auto login = new loginDialog (this);
            login->show ();
        });
        */

        semesterChooserLayout->addWidget (setSemester);
        semesterChooserLayout->addWidget (setSchedule);
        //semesterChooserLayout->addWidget (testButton);
        semesterChooserLayout->setContentsMargins(0, 0, 0, 0);
        m_pSemesterChooserWgt->setLayout(semesterChooserLayout);

        if(Arg::viewMode == ViewMode::Edit)
        {
            ///Replace by BiXiaoxia 2016.12.20.
            m_pSemesterChooserWgt->show();
    //        TopLineLayout->addWidget(m_pSemesterChooserWgt);
    //        TopLineLayout->addLayout(semesterChooserLayout, 2);
        }
        else {
            m_pSemesterChooserWgt->hide();
        }
#endif

        TopLineLayout->setSpacing (2);
        TopLineLayout->addWidget(userInfoView);
        TopLineLayout->addStretch (8);
        /*
        TopLineLayout->addLayout (semesterChooserLayout, 10);
        TopLineLayout->addStretch (30);
         */

//        auto todayLabel = new QLabel (TopLine);
//        todayLabel->setPixmap (QPixmap::fromImage (QImage(QString(":Icon/month/%1.png").arg (QDate::currentDate ().month ()))));
//        TopLineLayout->addWidget(todayLabel, 5);

        TopLineLayout->addStretch (20);
        QWidget* operationWidget = new QWidget (this);
        auto opLayout = new QHBoxLayout(operationWidget);
        opLayout->setSpacing (0);
        opLayout->setMargin (0);

        mFixButton->setObjectName("FixButton");
        mConfigLookAndFeel->setObjectName("ConfigLookAndFeel");
        TopMore->setObjectName("TopMore");
//        mBlockButton->setObjectName("BlockButton");
        mMinButton->setObjectName("MinButton");
        mCloseButton->setObjectName("CloseButton");

        opLayout->addWidget (mFixButton, 1);
        opLayout->addWidget(mConfigLookAndFeel, 1);
        opLayout->addWidget(TopMore, 1);
//        opLayout->addWidget(mBlockButton, 1);
        opLayout->addWidget(mMinButton, 1);
        opLayout->addWidget(mCloseButton, 1);


        mFixButton->setIconSize({24, 24});
        mConfigLookAndFeel->setIconSize({24, 24});
        TopMore->setIconSize({24, 24});
//        mBlockButton->setIconSize({24, 24});
        mMinButton->setIconSize({24, 24});
        mCloseButton->setIconSize({24, 24});
        /*
        mFixButton->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #2260c5; margin-left: 3px; margin-right: 3px;");
        mConfigLookAndFeel->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #2260c5; margin-left: 3px; margin-right: 3px;");
        TopMore->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #2260c5; margin-left: 3px; margin-right: 3px;");
        mBlockButton->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #2260c5; margin-left: 3px; margin-right: 3px;");
        mMinButton->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #2260c5; margin-left: 3px; margin-right: 3px;");
        mCloseButton->setStyleSheet("min-width: 28px; min-height: 28px; background-color: #d74635; margin-left: 3px; margin-right: 3px;");
    */
        TopLineLayout->addWidget (operationWidget, 3);
        //    TopLineLayout->addWidget(ResizeButton, 1);
        //    TopLineLayout->addWidget(TopSetup, 1);

        ///Delete by BiXiaoxia 2016.12.21.
//        setCentralWidget(centralWidget);

        retranslateUi();

        QMetaObject::connectSlotsByName(this);

        gridView = new TermWidget (this);
        gridView->link (Arg::user);
        connect(gridView, &TermWidget::requestShowLessons, this, &MainScreen::showLinkedLessons);
        connect(gridView, &TermWidget::requestResizeMainWindow, this, &MainScreen::resizeWindow);



        m_pInfoSettingsStackWgt = new InfoAndSettingsStackWidget;
        m_pInfoSettingsStackWgt->setMinimumWidth(0);

//        tabWidget = new Sidebar(this);
//        tabWidget->setMinimumWidth (0);
//        fileListWidget = new FileListWidget (this);
//        Arg::fileListWidget = fileListWidget;
//        tabWidget->addTab(fileListWidget, "");
//        //tabWidget->addTab (fileListWidget, /*"QIcon(":/Icon/Slide/Resources_default.png")"*/ "Resources");
////        tabWidget->addTab (new UserListWidget (this), /* QIcon(":/Icon/Slide/Friends_default.png")*/ "Friends");
////        tabWidget->addTab (new LiveWidget (this), /* QIcon(":/Icon/Slide/circle_default.png") */ "Circle");

//        tabWidget = new Sidebar(this);
//        tabWidget->setMinimumWidth (0);
//        fileListWidget = new FileListWidget (this);
//        Arg::fileListWidget = fileListWidget;
//        EditRightWidget *editRightWidget = new EditRightWidget(this);


//        tabWidget->addTab (editRightWidget, /*"QIcon(":/Icon/Slide/Resources_default.png")"*/ "Resources");
//        tabWidget->addTab (new UserListWidget (this), /* QIcon(":/Icon/Slide/Friends_default.png")*/ "Friends");
//        tabWidget->addTab (new LiveWidget (this), /* QIcon(":/Icon/Slide/circle_default.png") */ "Circle");




        ///< Mark 2016.12.20 by BiXiaoxia 右侧显示隐藏按钮.
        mHideTabButton = new QPushButton (this);
        QPixmap icon(":/Icon/sidecontrolbuttonbg.png");
        mHideTabButton->setStyleSheet("QPushButton{background-color:transparent;"
                                      /*"border-image:url(:/Icon/sidecontrolbuttonbg.png);}"
                                      "QPushButton:hover{background-color:transparent;"
                                      "border-image:url(:/Icon/sidecontrolbuttonbghover.png)}"
                                      "QPushButton:checked{background-color:transparent;"
                                      "border-image:url(:/Icon/sidecontrolbuttonbg_checked.png)};"
                                     "QPushButton:checked:hover{background-color:transparent;"
                                      "border-image:url(:/Icon/sidecontrolbuttonbg_checkedhover.png)}"*/
                                      "QToolTip {background-color:white}"); ///QPushbutton的checked:hover有bug，所以一直不起作用,写其中有给会导致qi'ta其它的不太正常.
        mHideTabButton->setMask(icon.createHeuristicMask(true));
        mHideTabButton->setIconSize(QSize(icon.width(), icon.height()));
        mHideTabButton->setFixedSize(icon.width(), icon.height());
        mHideTabButton->setIcon(icon);
        mHideTabButton->setObjectName (QStringLiteral ("hideSideBar"));
        mHideTabButton->setCheckable (true);
        mHideTabButton->setChecked (false);
        mHideTabButton->setToolTip(tr("显示侧边栏"));
        mHideTabButton->installEventFilter(this);
        //    mHideTabButton->setAutoFillBackground (true);
//        ///Add by BiXiaoxia 2016.12.20.
//        QGraphicsOpacityEffect *pOpcityEft = new QGraphicsOpacityEffect;
//        mHideTabButton->setGraphicsEffect(pOpcityEft);
//        pOpcityEft->setOpacity(0.7);
//        ///Add End.
        connect(mHideTabButton, SIGNAL (clicked (bool)), this, SLOT (showSideBar (bool)));

        composing();

        this->move((QApplication::desktop()->width()-width())*4/5,(QApplication::desktop()->height()-height())/4);

        system_tray = new QSystemTrayIcon(this);
        system_tray->setToolTip(tr("云师翼教师桌面"));
        system_tray->setIcon(QIcon(":/Icon/MoonCalendar.ico"));
        connect(system_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(activated(QSystemTrayIcon::ActivationReason)));
        system_tray->show();

        ///Add by BiXiaoxia 2016.12.20,加载设置的皮肤,这里暂时这么用，重构时候需要单独建立一个settings模块，管理用户及其它设置类.
//        qDebug() << "begin load and paint skin" << QTime::currentTime();
        QString sPixmap = QApplication::applicationDirPath() + "/background/default/default.jpg";
        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        QString sImageFilePathName = configSettings.value("Skin", sPixmap).toString();
        mBackgroundImage.load(sImageFilePathName);
//        qDebug() << "end load and paint skin" << QTime::currentTime();
        mBackgroundOpacity = configSettings.value("BackGroundOpacity", 1.0f).toReal();
        mForegroundOpacity = configSettings.value("ForeGroundOpacity", 0.5f).toReal();

        gridView->setBackground (mForegroundOpacity);

        adjustSize();
//        setFixedHeight(sizeHint().height());

        m_bStandardUIInitialized = true;
    }
    else if(isSideBarShown){ //如果side bar是显示的，要重新刷新一下右侧信息栏，使其同步.
        showSideBar(false);
        showSideBar(true);
    }

    /*
     * Top Semester Select Bar
     * TODO:
     * Connect Semester Dialog
     * Left / Right function
     */

    /*
        QHBoxLayout* semesterChooserLayout = new QHBoxLayout ();

        lastSemester = new QPushButton (TopLine);
        lastSemester->setIcon (QIcon(QPixmap(":/Icon/Standard/01_10.png").scaled(24, 24)));
        lastSemester->setIconSize(QSize{24, 24});
        //lastSemester->setFixedSize(24, 24);
        lastSemester->setObjectName ("SemesterChooser");
        //lastSemester->setStyleSheet("margin: 0; padding:0; border: 0px solid black;");
        connect (lastSemester, &QPushButton::clicked, [] () {
            if (Arg::currentWeekIndex () > 0)
                Arg::setCurrentWeekIndex (Arg::currentWeekIndex () - 1);
        });

        nextSemester = new QPushButton (TopLine);
        nextSemester->setIcon (QIcon(":/Icon/Standard/01_12.png"));
        nextSemester->setIconSize(QSize{24, 24});
        nextSemester->setObjectName ("SemesterChooser");
        connect (nextSemester, &QPushButton::clicked, [] () {
            if (Arg::currentWeekIndex () < Arg::currentTerm ()->weekNumber () - 1)
                Arg::setCurrentWeekIndex (Arg::currentWeekIndex () + 1);
        });
        */

    //Modified by wangqin 12-12

//    qDebug()<<"current view mode is:"<<endl;

//    if(/*(*/Arg::viewMode == ViewMode::Edit/*) && (!m_bEditUIInitialized)*/)
//    {
//        QHBoxLayout* semesterChooserLayout = new QHBoxLayout();
//        qDebug()<<"============current view mode is edit mode=================="<<endl;
//        //---------------------学期设置按钮------------------------//
//        if(Arg::getInstance()->currentSemester() != nullptr)
//        {
//            setSemester = new QPushButton (Arg::getInstance()->currentSemester()->getTitle(), TopLine);
//            connect(Arg::getInstance(), &Arg::currentSemesterChanged, [&](Semester *s){
//                setSemester->setText(s->getTitle());
//                setSemester->update();
//            });
//        }
//        else
//        {
//            setSemester = new QPushButton (tr("学期设置"), TopLine);
//        }

//        connect (setSemester, &QPushButton::clicked, [this] ()
//        {
//            auto semeD = new SemesterDialog (this);
//            semeD->show ();
//        });

//        setSemester->setObjectName ("SetSemester");

//        //---------------------作息时间设置按钮------------------------//
//        if(Arg::getInstance()->currentSchedule() != nullptr)
//        {
//            setSchedule = new QPushButton (Arg::getInstance()->currentSchedule()->getName(), TopLine);
//            connect(Arg::getInstance(), &Arg::currentScheduleChanged, [&](Schedule *s){
//                setSchedule->setText(s->getName());
//                setSchedule->update();
//            });
//        }
//        else
//        {
//            setSchedule = new QPushButton (tr("作息时间设置"), TopLine);
//        }

//        connect (setSchedule, &QPushButton::clicked, [this] ()
//        {
//            auto scheD = new ScheduleDialog (this);
//            scheD->show ();
//        });
//        setSchedule->setObjectName ("SetSchedule");

//        /*
//        //2016/12/1修改
//        testButton = new QPushButton (tr("测试"), TopLine);
//        connect (testButton, &QPushButton::clicked, [this] ()
//        {
//            qDebug()<<"loginDialog"<<endl;
//            auto login = new loginDialog (this);
//            login->show ();
//        });
//        */

//        semesterChooserLayout->addWidget (setSemester);
//        semesterChooserLayout->addWidget (setSchedule);
//        //semesterChooserLayout->addWidget (testButton);
//        semesterChooserLayout->setContentsMargins(0, 0, 0, 0);
//        m_pSemesterChooserWgt->setLayout(semesterChooserLayout);

//        m_bEditUIInitialized = true;
//    }


    //--------------For login test-----------------------//
    /*
    testButton = new QPushButton (tr("测试"), TopLine);
    connect (testButton, &QPushButton::clicked, [this] ()
    {
        auto login = new loginDialog (this);
        login->show ();
    });
    TopLineLayout->addWidget (testButton, 2);
*/
    //----------------------------------------------------//

    /*
        semesterChooserLayout->addWidget (lastSemester);
        semesterChooserLayout->addWidget (setSemester);
        semesterChooserLayout->addWidget (nextSemester);
        semesterChooserLayout->addWidget (setSchedule);
        semesterChooserLayout->addWidget (testButton);
        */

//    if(Arg::viewMode == ViewMode::Edit)
//    {
//        ///Replace by BiXiaoxia 2016.12.20.
//        m_pSemesterChooserWgt->show();
////        TopLineLayout->addWidget(m_pSemesterChooserWgt);
////        TopLineLayout->addLayout(semesterChooserLayout, 2);
//    }
//    else {
//        m_pSemesterChooserWgt->hide();
//    }

//    if(!m_bStandardUIInitialized) {

//        ///End.
//    }

//    if(!m_bStandardUIInitialized) {
//         m_bStandardUIInitialized = true;
//    }
//    else if(isSideBarShown){ //如果side bar是显示的，要重新刷新一下右侧信息栏，使其同步.
//        showSideBar(false);
//        showSideBar(true);
//    }

    setFixedSize(sizeHint());

} // setupUi


void MainScreen::changeDaySchme()
{

}

void MainScreen::activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        show();
    }
    else if (reason == QSystemTrayIcon::Context) {
        showMainMenu();
    }
}

void MainScreen::showMainMenu()
{
    QMenu *menu = new QMenu(0);

    /*QAction *drag_menu = menu->addAction(tr("允许拖拽文件"));
    drag_menu->setCheckable(true);
    drag_menu->setChecked(accept_drag_files);
    connect(drag_menu, SIGNAL(triggered(bool)), SLOT(changeDragFiles()));*/
    /*
    QAction *MousePenetration_action = menu->addAction(tr("桌面锁定"));
    MousePenetration_action->setIcon(QIcon(":/Icon/Icon/1197100.png"));
    connect(MousePenetration_action, SIGNAL(triggered(bool)), SLOT(changeLockAndStayTop()));

    QAction *save_action = menu->addAction(tr("保存信息"));
    save_action->setIcon(QIcon(":/Icon/1178562.png"));
    connect(save_action, SIGNAL(triggered(bool)), SLOT(saveSlot()));

    QAction *load_action = menu->addAction(tr("读取信息"));
    load_action->setIcon(QIcon(":/Icon/1178562.png"));
    connect(load_action, SIGNAL(triggered(bool)), SLOT(loadSlot()));
*/
    QAction *standardMode_action = menu->addAction (tr("标准模式"));
    standardMode_action->setIcon (QIcon(":/Icon/Standard/01_38.png"));
    connect (standardMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Standard);});
    QAction *inclassMode_action = menu->addAction (tr("上课模式"));
    inclassMode_action->setIcon (QIcon(":/Icon/Standard/01_50.png"));
    connect (inclassMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::InClass);});
    QAction *currentDayMode_action = menu->addAction (tr("单日模式"));
    currentDayMode_action->setIcon (QIcon(":/Icon/Standard/01_56.png"));
    connect (currentDayMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Day);});
    QAction *editDayMode_action = menu->addAction (tr("编辑模式"));
    editDayMode_action->setIcon (QIcon(":/Icon/Standard/01_58.png"));
    connect (editDayMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Edit);});
    menu->addSeparator ();

    QAction *setupConfig_action = menu->addAction(tr("设置"));
    setupConfig_action->setIcon (QIcon(":/Icon/Standard/01_60.png"));
    connect(setupConfig_action, SIGNAL(triggered(bool)), SLOT(setupConfig()));
    menu->addSeparator ();  

#if 0
    QAction *upgradeAction = menu->addAction (tr("检查更新"));
    upgradeAction->setIcon (QIcon(":/Icon/Standard/01_62.png"));

    menu->addSeparator ();
    QAction *feedbackAction = menu->addAction (tr("用户反馈"));
    feedbackAction->setIcon (QIcon(":/Icon/Standard/01_64.png"));
    connect (feedbackAction, &QAction::triggered, [this] ()
    {
        auto feedback = new FeedBackDialog (this);
        feedback->show ();
    });
    QAction *aboutAction = menu->addAction (tr("关于我们"));
    aboutAction->setIcon (QIcon(":/Icon/Standard/01_66.png"));
#endif

    QAction *homeAction = menu->addAction (tr("官方网站"));
    homeAction->setIcon (QIcon(":/Icon/Standard/01_68.png"));
    connect(homeAction, &QAction::triggered, [this]() {QDesktopServices::openUrl(QUrl("http://www.xidiankeji.com"));});
    //    connect (feedbackAction, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Edit);});
    /*
    QMenu *lan_menu = menu->addMenu(tr("切换语言"));
    lan_menu->setIcon(QIcon(":/Icon/1201732.png"));
    connect(lan_menu->addAction("中文"), SIGNAL(triggered(bool)), CCU::ccu, SLOT(changeToChinese()));
    connect(lan_menu->addAction("English"), SIGNAL(triggered(bool)), CCU::ccu, SLOT(changeToEnglish()));
    menu->addSeparator();
    */
    menu->addSeparator ();
    QAction *aboutAction = menu->addAction (tr("关于MoonCalendar..."));
    aboutAction->setIcon (QIcon(":/Icon/Standard/01_66.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAboutWindow));

    menu->addSeparator ();
    QAction *pLogOffAct = menu->addAction (tr("注销"));
    pLogOffAct->setIcon (QIcon(":/Icon/Standard/01_66.png"));
    connect(pLogOffAct, SIGNAL(triggered()), this, SLOT(logOff()));

    QAction *exit_action = menu->addAction(tr("退出"));
    exit_action->setIcon(QIcon(":/Icon/Standard/exit.png"));
    connect(exit_action, SIGNAL(triggered(bool)), CCU::ccu, SLOT(close()));
    menu->exec(QCursor::pos());
}


void MainScreen::close()
{
    //在此处保存一些配置文件

    ///Mark，这个不关掉，会延迟退出.
    delete system_tray;
    system_tray = Q_NULLPTR;

//    saveAll(QCoreApplication::applicationDirPath()+Const::defaultSavePath);
    QWidget::close();
}



void MainScreen::configLookAndFeel()
{
    if (mLookAndFeel == nullptr)
    {
        mConfigLookAndFeel->setCursor(Qt::WaitCursor);

        mLookAndFeel = new LookAndFeelConfigDialog (mBackgroundOpacity, mForegroundOpacity);
        ///Add by BiXiaoxia 2016.12.20.
        connect (mLookAndFeel, SIGNAL(backgroundImageChanged(const QString&)), this, SLOT(setBackgroundImage(const QString&)));
        connect (mLookAndFeel, SIGNAL (backgroundOpacityChanged(double)), this, SLOT (setBackgroundOpacity (double)));
        connect (mLookAndFeel, SIGNAL (foregroundOpacityChanged(double)), this, SLOT (setForegroundOpacity (double)));
        connect(mLookAndFeel, &LookAndFeelConfigDialog::restoreSkin, this, &MainScreen::restoreSkin);

        mConfigLookAndFeel->unsetCursor();
    }

    mLookAndFeel->show();
}

void MainScreen::restoreSkin()
{
    QString sPixmap = QApplication::applicationDirPath() + "/background/default/default.jpg";
    setBackgroundImage(sPixmap);
    setBackgroundOpacity(1.0f);
    setForegroundOpacity(0.5f);
}

void MainScreen::logOff()
{
    emit systemLogOff();
}

void MainScreen::showAboutWindow()
{

}

void MainScreen::unlockAndStayTop()
{
    setAttribute (Qt::WA_TransparentForMouseEvents, false);
    show();
    raise();
    qApp->processEvents();
#ifdef WIN
    if(window_long)SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, window_long);
#endif
}

void MainScreen::reset()
{
    if(Arg::isMousePenetration == true)changeLockStatus();
    else unlockAndStayTop();
    lower();
    gridView->reset();
    lower();
}

void MainScreen::changeLockStatus()
{
    if(!Arg::isLocked()) {
        m_unLockPt = pos();
    }

    Arg::setLock (!Arg::isLocked());

    QPoint pt;

    QDesktopWidget *pWgt = QApplication::desktop();
    QRect availableRect = pWgt->availableGeometry();

    if(Arg::isLocked()) {
        int nShadowWidth = getShadowMargin();
        pt.setX(availableRect.width() - width() + nShadowWidth);
        pt.setY(-nShadowWidth);
    }
    else {
        pt = m_unLockPt;
    }

    move(pt);
//    setAttribute (Qt::WA_TransparentForMouseEvents, true);
//    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnBottomHint);
//    setWindowOpacity (0.6);
//    show();
//    lower();
//    qApp->processEvents();
//#ifdef WIN
//    window_long = GetWindowLong((HWND)this->winId(), GWL_EXSTYLE);
//    SetWindowLong((HWND)this->winId(), GWL_EXSTYLE, window_long | WS_EX_TRANSPARENT | WS_EX_LAYERED |WS_EX_TOOLWINDOW);
    //#endif
}

void MainScreen::changeLockAndStayTop()
{
    Arg::isMousePenetration^=1;
    reset();
}

void MainScreen::showLinkedLessons()
{
    if(!isSideBarShown) {
        showSideBar(true);
    }
}

void MainScreen::resizeWindow()
{
    setFixedSize(sizeHint());
}

void MainScreen::setArg()
{
    DefaultSize=QSize(820,650);
}

void MainScreen::saveAll(QString s)
{
    QFile file(s);
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_7);

        out<<this->pos()<<this->size();
        out<<(*DataBase::getDataBase());

        file.close();
    }
    else QMessageBox::warning(0, tr("教师客户端"), tr("无法打开文件:%1").arg(s));
}
void MainScreen::loadAll(QString s)
{
    QFile file(s);
    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_7);

        QPoint p;QSize s;
        in>>p>>s;
        this->move(p);
        this->resize(s);
        in>>(*DataBase::getDataBase());

        file.close();
    }
    else QMessageBox::warning(0,"教师客户端","文件不存在或数据格式错误！");

}

void MainScreen::StartResize()
{
    resizeDragBoudingBox = new DraggableBox(0,this);
    resizeDragBoudingBox->exec();
    delete resizeDragBoudingBox;
}
void MainScreen::saveSlot()
{
    QString s = QFileDialog::getSaveFileName( this,"保存配置",QCoreApplication::applicationDirPath(),"MoonData(*.mdd)");
    if(s!="")
    {
        saveAll(s);
    }
}
void MainScreen::loadSlot()
{
    QString s = QFileDialog::getOpenFileName( this,"读取配置",QCoreApplication::applicationDirPath(),"MoonData(*.mdd)");
    if(s!="")
    {
        loadAll(s);
        reset();
    }
}

void MainScreen::setupConfig()
{
#if 0
    if(m_pConfigDialog == Q_NULLPTR) {
        m_pConfigDialog = new dlgConfig(this);
        connect(m_pConfigDialog, &dlgConfig::settingChanged, this, &MainScreen::reset);
    }

    m_pConfigDialog->show();
#endif

//    if(m_pSettingsWgt == Q_NULLPTR) {
//        m_pSettingsWgt = new SettingsWidget(this);
//    }
    SettingsWidget *pSettingsWgt = SettingsWidget::getInstance();
    pSettingsWgt->load();
    pSettingsWgt->show();

    ///Delete by BiXiaoxia 2016.12.23，样式表移到配置类里一次加载，样式表加载效率比较低，这里如果不是换了样式，不要每次加载.
//    dlgConfig dialog;
//    QFile stylesheet{":/config.qss"};
//    stylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
//    QString str = QString(stylesheet.readAll());
//    dialog.setStyleSheet(str);

//    dialog.exec();
//    reset();
}

void MainScreen::composing ()
{
    MainGridLayout->setSpacing (0);
    MainGridLayout->addWidget(TopLine, 0, 0, 1, 3);
    //MainGridLayout->addWidget(monthchoose, 1, 0, 1, 1);
    MainGridLayout->addWidget(gridView, 1, 0, 1, 1);
    MainGridLayout->addWidget(mHideTabButton, 1, 1, 1, 1);
//    MainGridLayout->addWidget(m_pInfoSettingsStackWgt, 1, 2, 1, 1);
    MainGridLayout->addWidget(m_pInfoSettingsStackWgt, 1, 2, 1, 1);
//    MainGridLayout->setRowMinimumHeight(1, height() - TopLine->height());
//    MainGridLayout->addWidget (tabWidget, 1, 2, 1, 1);
//    MainGridLayout->setColumnStretch (0, 10000);
//    MainGridLayout->setColumnStretch (1, 1);
//    MainGridLayout->setColumnStretch (2, 3750);

    //    Term
}

void MainScreen::onedayMode (bool on )
{
    if (!gridView) return;

    m_bOneDayMode = on;

    if (on)
    {
//        ///Mark,这里必须要放在前面，适应利用布局控制size的gridView.
//        resize(sizeHint());
//        setFixedSize(sizeHint());
//        ///Mark End.

        ///Mark，这里有时有时无的bug，暂时替换掉.
        //gridView->showOneDay (/*Arg::currentWeekday ()*/);
        ///Replace end.
        gridView->showOneDay(QDate::currentDate().dayOfWeek() - 1);
        userInfoView->hide ();
//        lastSemester->hide ();
//        setSemester->hide ();
//        nextSemester->hide ();
//        mBlockButton->hide ();
        mCloseButton->hide ();
        mConfigLookAndFeel->hide ();
        mMinButton->hide ();       
    }
    else
    {
        gridView->showAllDay ();
        userInfoView->show ();
//        lastSemester->show ();
//        setSemester->show ();
//        nextSemester->show();
//        mBlockButton->show ();
        mCloseButton->show ();
        mConfigLookAndFeel->show ();
        mMinButton->show ();
    }

    setFixedSize(sizeHint());
    update();

    //    setMinimumHeight (gridView->sizeHint ().height () + 50);
    //    setFixedSize (gridView->sizeHint ().width ()+ tabWidget->width () + mHideTabButton->width (), height ());
//    update();
//    updateGeometry ();
//    setMinimumSize(sizeHint ());
//    resize (sizeHint());


}

void MainScreen::retranslateUi()
{
    setWindowTitle(QApplication::translate("MainScreen", "Widget", 0));
}

void MainScreen::setBackgroundImage (const QString& sImageFilePathName)
{
    mBackgroundImage.load(sImageFilePathName);

    //    qDebug () << tabWidget->x () << this->width () << image.width ();
    //    tabWidget->setBackground (image.copy (static_cast<int>(static_cast<double>(tabWidget->x ()) / width () * image.width ()), static_cast<int>(static_cast<double>(tabWidget->y ()) / height () * height ()), static_cast<int>(static_cast<double>(tabWidget->width ()) / this->width () * image.width ()), static_cast<int>(static_cast<double>(tabWidget->height ()) / height () * image.height ())));
    repaint();


    ///Mark，暂时这么处理，以后重构需要重新规划设置类.
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    configSettings.setValue("Skin", sImageFilePathName);
}

void MainScreen::setBackgroundOpacity (double opacity)
{
    mBackgroundOpacity = opacity;
    update ();

    ///Mark 2017.01.05 by BiXiaoxia，暂时这么记录透明度,可以上课模式共用.
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    configSettings.setValue("BackGroundOpacity", opacity);
}
void MainScreen::setForegroundOpacity (double opacity)
{
    mForegroundOpacity = opacity;
    gridView->setBackground (opacity);
//    auto palette = tabWidget->palette ();
//    palette.setBrush (tabWidget->backgroundRole (), QBrush (QColor (255, 255, 255, static_cast<int>(opacity * 255))));
//    tabWidget->setPalette (palette);
    //    static_cast<Sidebar*>(tabWidget)->setOpacity (opacity);
    ///Mark 2017.01.05 by BiXiaoxia，暂时这么记录透明度,可以上课模式共用.
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    configSettings.setValue("ForeGroundOpacity", opacity);
}

void MainScreen::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev)

    painter->save ();
    painter->setOpacity (mBackgroundOpacity);
    if (mBackgroundImage.isNull ())
    {
        int nShadowWidth = getShadowMargin();
        painter->fillRect (QRect(nShadowWidth, nShadowWidth, width() - 2 * nShadowWidth, height() - 2 * nShadowWidth), Qt::white);
    }
    else
    {
        ///Modeified by BiXiaoxia 2016.12.21.
        int nShadowWidth = 0;

        if(isShaodwEnabled()) {
            nShadowWidth = getShadowMargin();
        }

        QRect imgRect;

        ///Mark,要考虑减掉阴影处理，目前只有三边阴影需求，基类也这么实现的，所以暂时不考虑top.
        //保持左侧背景图片不变，即永远只绘制左侧尺寸的image.
        if(!isSideBarShown) {
            imgRect.setRect(nShadowWidth, nShadowWidth, width() - 2 * nShadowWidth, height() - 2 * nShadowWidth);
        }
        else {
            imgRect.setRect(nShadowWidth, nShadowWidth, width() - 2 * nShadowWidth - m_pInfoSettingsStackWgt->width(),
                            height() - 2 * nShadowWidth);
        }

        ///单日模式也需要考虑.
        if(!m_bOneDayMode) {
            painter->drawImage(imgRect, mBackgroundImage);
        }
        else {
            ///Mark,切记一定要排除阴影部分.
            painter->drawImage(nShadowWidth, nShadowWidth, mBackgroundImage, 0, 0, imgRect.width(), imgRect.height());
        }
    }

    painter->restore ();
}

//void MainScreen::paintEvent (QPaintEvent* ev)
//{


//    ///Modified by BiXiaoxia 2016.12.21.
//    BaseShadowWidget::paintEvent (ev);
//}


void MainScreen::showSideBar (bool on)
{
    ///Modified by BiXiaoxia 2016.12.23,重新规划右侧信息显示与编辑窗口.
    isSideBarShown = on;
//    MainGridLayout->setRowFixedHeight(1, height() - TopLine->height());

    m_pInfoSettingsStackWgt->setFixedWidth (on ? m_pInfoSettingsStackWgt->sizeHint ().width (): 0);
    setFixedSize(sizeHint());

    if(on) {
        QPixmap icon(":/Icon/sidecontrolbuttonbg_checked.png");
        mHideTabButton->setIcon(icon);

        mHideTabButton->setToolTip(tr("关闭侧边栏"));
    }
    else {
        QPixmap icon(":/Icon/sidecontrolbuttonbg.png");
        mHideTabButton->setIcon(icon);

        mHideTabButton->setToolTip(tr("显示侧边栏"));
    }

      ///Mark delete 2017.03.03,暂时注释掉，编辑状态页面暂时不显示.
//    InfoAndSettingsStackWidget::InfoSettingsType type;
//    if(Arg::viewMode == ViewMode::Edit) {
//        type = InfoAndSettingsStackWidget::EditMode;
//    }
//    else if(Arg::viewMode == ViewMode::Standard){
//        type = InfoAndSettingsStackWidget::StandardMode;
//    }

//    m_pInfoSettingsStackWgt->switchInfoAndSettingsWidget(type);
    ///Mark End.

    ///REMOVE by BiXiaoxia 2012.12.20.
//    update ();
//    updateGeometry ();
//     setMinimumSize (sizeHint ());
    ///REMOVE END.
//    emit mainScreenResized(sizeHint());

//    resize(sizeHint());
//    setFixedSize(sizeHint());

//    if (on)
//        mHideTabButton->setIcon (QPixmap::fromImage (QImage(":/Icon/Slide/sidehide.png").scaled (mHideTabButton->size ())));
//    else
//        mHideTabButton->setIcon (QPixmap::fromImage (QImage(":/Icon/Slide/sideshow.png").scaled (mHideTabButton->size ())));

    //    auto anchor = gridView->geometry ();
    //    mHideTabButton->move ((anchor.right () - (on? 0 : mHideTabButton->width ())), (anchor.top () + anchor.bottom ()) >> 1);
    //    qDebug () << QPoint ((anchor.right () - (on? 0 : mHideTabButton->width ())), (anchor.top () + anchor.bottom ()) >> 1);
}

void MainScreen::mousePressEvent (QMouseEvent* ev)
{
//    if(TopLine->rect().contains(ev->pos())) {
//        mPressed = true;
//        mLastMousePos = ev->globalPos ();
//        mLastWindowPos = this->pos ();
//    }

    QWidget::mousePressEvent (ev);
}

void MainScreen::mouseMoveEvent (QMouseEvent* ev)
{
    if (mPressed &&  !Arg::isLocked())
    {
        QDesktopWidget *pDeskTopWgt = QApplication::desktop();
        QRect availRect = pDeskTopWgt->availableGeometry();
        QPoint pt = mLastWindowPos + ev->globalPos () - mLastMousePos;

        int nShadowWidth = getShadowMargin();
//        QPoint nextRightTopPt = rect().topRight() + pt;

        if((pt.x() > -nShadowWidth)
                && (pt.x() < availRect.width() - width() + nShadowWidth)
                && (pt.y() > /*- TopLine->height() / 2*/ - nShadowWidth)
                && (pt.y() < availRect.height() - TopLine->height() - nShadowWidth)) {
            this->move(pt);
        }
    }

    QWidget::mouseMoveEvent (ev);
//    mHideTabButton->setIcon (QPixmap::fromImage (QImage(":/Icon/Slide/sideshow.png")));
}

void MainScreen::mouseReleaseEvent (QMouseEvent* ev)
{
    mPressed = false;
    QWidget::mouseReleaseEvent (ev);
}

QSize MainScreen::sizeHint() const
{
//    if(!isVisible()) {
//        return QSize(0, 0);
//    }

#define USE_EXPANDING_GRIDVIEW
#ifdef USE_EXPANDING_GRIDVIEW
    auto gs = gridView->sizeHint();
    auto ss = isSideBarShown ? m_pInfoSettingsStackWgt->sizeHint () : QSize(0, m_pInfoSettingsStackWgt->sizeHint().height());

    int nShadowWidth = getShadowMargin();
    int nWidth = gs.width () + ss.width () + mHideTabButton->width() + nShadowWidth + nShadowWidth;
    int nHeight = TopLine->height() + gs.height() + 2 * nShadowWidth /*+ layout()->spacing() +
            layout()->contentsMargins().bottom()*/;

#if 0
    if(m_bFirstVisible) { ///加入这个为了调整第一次尺寸较小，后面会变大情况, 这些及gridView关联的类里相关的调整都是临时手段.
        nHeight = gs.height() + nShadowWidth + nShadowWidth;
        nHeight += TopLine->height();
        nHeight -= 50;
    }
    else {
        nHeight = m_firstSize.height();
    }
#endif

    QSize size(nWidth, nHeight);

    qDebug() << "the mainscreen size is:" << size << m_firstSize << nHeight;

    return size;
#else
    QSize leftSize(620, 530);

    if(Arg::viewMode == ViewMode::Day) {
        leftSize.setWidth(200);
    }

    auto ss = isSideBarShown ? m_pInfoSettingsStackWgt->sizeHint ():QSize(0, 0);

    int nShadowWidth = 2 * getShadowMargin();
    return QSize(leftSize.width() + ss.width() + nShadowWidth, leftSize.height() + nShadowWidth);
#endif
}


void MainScreen::enterEvent (QEvent* ev)
{
    QWidget::enterEvent (ev);

#if USE_FIXED_OPECITY
    if (Arg::isLocked ())
    {
        setWindowOpacity (1.0);
        setFixedSize(sizeHint());
        TopLine->show ();
        int nTopLinzeHeight = TopLine->height();
        move(pos().x(), pos().y() - nTopLinzeHeight);
    }
#else
    if (Arg::isLocked())
    {
        setWindowOpacity (1.0);
    }
#endif
}

void MainScreen::leaveEvent (QEvent* ev)
{
    QWidget::leaveEvent (ev);

#if USE_FIXED_OPECITY
    if (Arg::isLocked ())
    {
        setWindowOpacity (0.6);
        int nTopLinzeHeight = TopLine->height();
        TopLine->hide ();
        setFixedSize(sizeHint().width(), sizeHint().height() - TopLine->height());
        move(pos().x(), pos().y() + nTopLinzeHeight);
    }
#else
    if(Arg::isLocked()) {
        setWindowOpacity(0.5);
    }
#endif
}

void MainScreen::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if(m_bFirstVisible) {
        ///保存第一次尺寸，现在第一次加了topLine的，为了处理后面加topLine会变大情况.
        m_firstSize.setWidth(sizeHint().width());
        m_firstSize.setHeight(sizeHint().height());

        setFixedHeight(m_firstSize.height());

        m_bFirstVisible = false;
    }
}

bool MainScreen::eventFilter(QObject *watched, QEvent *event)
{
    switch(event->type()) {
    case QEvent::MouseButtonPress: {
        gridView->hideCalendarSelect();

        if(watched == TopLine) {
            mPressed = true;
            QMouseEvent *pMouseEv = dynamic_cast<QMouseEvent*>(event);
            mLastMousePos = pMouseEv->globalPos();
            mLastWindowPos = this->pos();

            return true;
        }
    }
        break;

    case QEvent::MouseButtonDblClick: {
        if(watched == TopLine) {
            static int loopControl = 0;
            int loopIndex = loopControl % 2;
            ViewMode vwMode;

            if(loopIndex == 0) {
                vwMode = ViewMode::Day;

            }
            else if(loopIndex == 1){
                vwMode = ViewMode::InClass;
            }

            CCU::ccu->changeViewMode(vwMode);
            ++loopControl;

            return true;
        }
        break;

    case QEvent::HoverEnter:{
            if(watched == mHideTabButton) {
                if(mHideTabButton->isChecked()) {
                    QPixmap icon(":/Icon/sidecontrolbuttonbg_checkedhover.png");
//                    mHideTabButton->setMask(icon.createHeuristicMask(true));
                    mHideTabButton->setIcon(icon);
                }
                else {
                    QPixmap icon(":/Icon/sidecontrolbuttonbghover.png");
                    mHideTabButton->setIcon(icon);
                }

                return true;
            }
        }
            break;

        case QEvent::HoverLeave:{
            if(watched == mHideTabButton) {
                if(mHideTabButton->isChecked()) {
                    QPixmap icon(":/Icon/sidecontrolbuttonbg_checked.png");
//                    mHideTabButton->setMask(icon.createHeuristicMask(true));
                    mHideTabButton->setIcon(icon);
                }
                else {
                    QPixmap icon(":/Icon/sidecontrolbuttonbg.png");
                    mHideTabButton->setIcon(icon);
                }

                return true;
            }
        }
            break;

    default:
            break;
    }
        break;
    }


    return QWidget::eventFilter(watched, event);
}


