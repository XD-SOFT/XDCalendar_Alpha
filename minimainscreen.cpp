#include "minimainscreen.h"
#include "minimode.h"
#include "ccu.h"
#include "lesson.h"
#include "config.h"
#include "miniuserinfowidget.h"
#include "minidatewidget.h"
#include "minifilelistwidget.h"
#include "mybutton.h"
#include "term.h"
#include <utility>
#include <QPoint>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>
#include <string>
#include <stdexcept>
#include <QGridLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QPainter>
#include <QSettings>
#include <QGraphicsOpacityEffect>
#include <QStyleOption>
#include <QEvent>

MiniMode::MainScreen* MiniMode::MainScreen::instance = nullptr;
MiniMode::MainScreen::MainScreen(QWidget *parent)
    : BaseShadowWidget(parent),
      m_pModeCtlMenu(Q_NULLPTR)
{

    setupSubWidgets();
    QHBoxLayout* layout = new QHBoxLayout (this);
    layout->setSpacing (0);
    layout->setMargin (6);
    QVBoxLayout* mainLayout = new QVBoxLayout ();
    layout->addLayout (mainLayout, 10);
    mainLayout->setSpacing (6);
    mainLayout->setMargin (0);

    topLine = new QWidget;
    topLine->installEventFilter(this);
    QHBoxLayout* topLayout = new QHBoxLayout (topLine);
    topLayout->setSpacing (0);
    topLayout->setMargin (0);
    moreBtn = new TransparentButton(QIcon(":/Icon/Standard/01_18.png"),"",topLine);
    moreBtn->setObjectName(QStringLiteral("TopLine"));
    moreBtn->setToolTip("更多");
    auto mCloseButton = new TransparentButton(QIcon(":/Icon/Standard/01_24.png"), "", topLine);
    mCloseButton->setObjectName(QStringLiteral("TopLine"));
    mCloseButton->setToolTip("关闭");
    connect(mCloseButton, SIGNAL(clicked(bool)), CCU::ccu, SLOT(close()));
    connect (moreBtn, SIGNAL (clicked ()), this, SLOT (showMainMenu ()));

    topLayout->addStretch (10);
    topLayout->addWidget (moreBtn, 1);
    topLayout->addWidget (mCloseButton, 1);
    mainLayout->addWidget (topLine, 1);

    QWidget* centralWidget = new QWidget (this);
//    m_pOpacityEffect = new QGraphicsOpacityEffect;
//    m_pOpacityEffect->setOpacity(1.0);
//    centralWidget->setGraphicsEffect(m_pOpacityEffect);

    auto centralLayout = new QGridLayout (centralWidget);
    centralLayout->addWidget (mUserInfoWidget, 0, 0, 2, 1);
    centralLayout->setSpacing (0);
    centralLayout->setMargin (0);

    QWidget* operatorWidget = new QWidget (this);
    operatorWidget->setObjectName ("Operation");
    QHBoxLayout *operationLayout = new QHBoxLayout(operatorWidget);
    operatorWidget->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
    operationLayout->setContentsMargins(0, 0, 0, 0);
    operationLayout->setSpacing (0);
    operationLayout->addWidget(mRecordBtn/*, 5*/);

    QFrame *pRecordVLine = new QFrame;
    pRecordVLine->setFrameShape(QFrame::VLine);
    pRecordVLine->setFrameShadow(QFrame::Plain);
    pRecordVLine->setFixedWidth(1);
    pRecordVLine->setStyleSheet("border: 1px solid #e9e9e9");
    operationLayout->addWidget(pRecordVLine);

    operationLayout->addWidget(mDrawingPadBtn/*, 5*/);

    QFrame *pDrawingVLine = new QFrame;
    pDrawingVLine->setFrameShape(QFrame::VLine);
    pDrawingVLine->setFrameShadow(QFrame::Plain);
    pDrawingVLine->setFixedWidth(1);
    pDrawingVLine->setStyleSheet("border: 1px solid #e9e9e9");
    operationLayout->addWidget(pDrawingVLine);

    operationLayout->addWidget(mFileListWidget);

    centralLayout->addWidget (mDateWidget, 0, 1);
    centralLayout->addWidget(operatorWidget, 1, 1);
    centralLayout->setColumnStretch (0, 1);
    centralLayout->setColumnStretch (1, 3);


    mainLayout->addWidget (centralWidget, 10);
//    setWindowFlags(Qt::FramelessWindowHint);
//    setAttribute (Qt::WA_TranslucentBackground);

    int nShadowWidth = getShadowMargin();
    mainLayout->setContentsMargins(nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);

    setFixedSize (MiniMode::width + 2 * nShadowWidth, MiniMode::height + 2 * nShadowWidth);

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sImageFilePathName = configSettings.value("Skin", QString()).toString();
    mBackgroundImage.load(sImageFilePathName);
}

void MiniMode::MainScreen::mousePressEvent (QMouseEvent* ev)
{
    mPressed = true;
    mOldWindowPosition = this->pos ();
    mOldCursorPosition = ev->globalPos ();
    QWidget::mousePressEvent (ev);
}

void MiniMode::MainScreen::mouseReleaseEvent (QMouseEvent* ev)
{
    mPressed = false;
    QWidget::mouseReleaseEvent (ev);
}

void MiniMode::MainScreen::mouseMoveEvent (QMouseEvent* ev)
{
    if (mPressed)
    {
        this->move (mOldWindowPosition + ev->globalPos () -mOldCursorPosition);
    }
    QWidget::mouseMoveEvent (ev);
}

void MiniMode::MainScreen::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    QStyleOption opt;
    opt.init(this);
//    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, painter, this);
//    painter->setCompositionMode (QPainter::CompositionMode_Multiply);
    painter->save();
    painter->setOpacity(m_fBackgroundOpacity);

    if(!mBackgroundImage.isNull()) {
        painter->drawPixmap(0, 0, width(), height(), mBackgroundImage, 0, 0, width(), height());
    }
    else {
        painter->fillRect (0, 0, this->width (), this->height (), QBrush (QColor (255, 255, 255, 50)));
    }

    painter->restore();
}

bool MiniMode::MainScreen::eventFilter(QObject *watched, QEvent *event)
{
    if((topLine != Q_NULLPTR) && (watched == topLine)) {
        if(event->type() == QEvent::MouseButtonDblClick) {
            CCU::ccu->changeViewMode(ViewMode::Standard);

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}

void MiniMode::MainScreen::showMainMenu ()
{
    if(m_pModeCtlMenu == Q_NULLPTR) {
        m_pModeCtlMenu = new QMenu(0);

        QAction *standardMode_action = m_pModeCtlMenu->addAction (tr("标准模式"));
        standardMode_action->setIcon (QIcon(":/Icon/Standard/01_38.png"));
        connect (standardMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Standard);});
        QAction *inclassMode_action = m_pModeCtlMenu->addAction (tr("上课模式"));
        inclassMode_action->setIcon (QIcon(":/Icon/Standard/01_50.png"));
        connect (inclassMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::InClass);});
        QAction *currentDayMode_action = m_pModeCtlMenu->addAction (tr("单日模式"));
        currentDayMode_action->setIcon (QIcon(":/Icon/Standard/01_56.png"));
        connect (currentDayMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Day);});
        QAction *editDayMode_action = m_pModeCtlMenu->addAction (tr("编辑模式"));
        editDayMode_action->setIcon (QIcon(":/Icon/Standard/01_58.png"));
        connect (editDayMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Edit);});
        //    QAction *standardMode_action = menu->addAction (tr("STANDARD"));
        //    connect (standardMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Standard);});
        //    QAction *inclassMode_action = menu->addAction (tr("IN CLASS"));
        //    connect (inclassMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::InClass);});
        //    QAction *dayMode_action = menu->addAction (tr("DAY MODE"));
        //    connect (dayMode_action, &QAction::triggered, [] () {CCU::ccu->changeViewMode (ViewMode::Day);});
        QAction *exit_action = m_pModeCtlMenu->addAction(tr("退出"));
        exit_action->setIcon(QIcon(":/Icon/Standard/01_24.png"));
        connect(exit_action, SIGNAL(triggered(bool)), CCU::ccu, SLOT(close()));

        m_pModeCtlMenu->addSeparator ();
    }

    m_pModeCtlMenu->exec (QCursor::pos ());
}

void MiniMode::MainScreen::setupSubWidgets() {
    mUserInfoWidget = new InfoView(this);
    mDateWidget = new MiniDateWidget(this);

    mRecordBtn = new QPushButton(this);
    mRecordBtn->setFlat(true);
    mRecordBtn->setStyleSheet("QPushButton::pressed{"
                              "background-color: #e9e9e9}"
                              "QPushButton{border: 0px 0px 0px 0px solid}"
                              "QPushButton::hover{background: #e9e9e9}");
    //    mRecordBtn->setText("R");
    QIcon recordIcon(":/Icon/Mini/01_41.png");
    mRecordBtn->setFixedWidth(30);
    mRecordBtn->setIconSize(mRecordBtn->size());
    mRecordBtn->setIcon(recordIcon);
    mRecordBtn->setObjectName (QStringLiteral ("Operation"));
    mRecordBtn->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);

    mDrawingPadBtn = new QPushButton(this);
    mDrawingPadBtn->setFlat(true);
    mDrawingPadBtn->setStyleSheet("QPushButton::pressed{"
                                  "background-color: #e9e9e9}"
                                  "QPushButton{border: 0px 0px 1px 0px solid}"
                                  "QPushButton::hover{background: #e9e9e9}");
    mDrawingPadBtn->setFixedWidth(30);
    mDrawingPadBtn->setIconSize(mDrawingPadBtn->size());
//    mDrawingPadBtn->setText("D");
    mDrawingPadBtn->setIcon (QIcon(":/Icon/Mini/01_43.png"));
    mDrawingPadBtn->setObjectName (QStringLiteral ("Operation"));
    mDrawingPadBtn->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);

    mFileListWidget = new FilelistView(this);
}

MiniMode::MainScreen::~MainScreen() = default;

void MiniMode::MainScreen::link (Lesson* lesson, int nLeftSeconds)
{
    mLinkedLesson = lesson;
    mUserInfoWidget->link (mLinkedLesson);
    if (lesson != nullptr)
        mFileListWidget->link (mLinkedLesson->rootFolder ());
    else
        mFileListWidget->link (nullptr);
    try {
        if (lesson != nullptr)
        {
            ///Mark TODO 2017.02.17,上课模式处理,暂时不处理，等所有服务器读取流程弄完，再弄.
//            Term::Section sec = Arg::currentTerm ()->getSections ().at (lesson->section () - 1);
//            mDateWidget->setInterval ((sec.second - sec.first) * 1000);
//            mDateWidget->setTimer (true);
            mDateWidget->setInterval(nLeftSeconds * 1000);
            mDateWidget->setTimer(true);
        }
        else {
            mUserInfoWidget->setSubject(tr("暂无课程"));
        }
    }
    catch (std::exception& e)
    {
        throw std::out_of_range (std::string("Lesson's section number is out of current term's sections.") + e.what ());
    }
}

void MiniMode::MainScreen::setForegroundOpacity(qreal fOpacity)
{
    m_fPregroundOpacity = fOpacity;

//    if(m_pOpacityEffect != Q_NULLPTR) {
//        m_pOpacityEffect->setOpacity(m_fPregroundOpacity);
//    }
}

void MiniMode::MainScreen::setBackgroundOpacity(qreal fOpacity)
{
    m_fBackgroundOpacity = fOpacity;
}

void MiniMode::MainScreen::reload ()
{
    mUserInfoWidget->reload ();
    mFileListWidget->reload ();
}

void MiniMode::MainScreen::enterEvent (QEvent*)
{
    qDebug () << "enter";
}
void MiniMode::MainScreen::leaveEvent (QEvent*)
{

    qDebug () << "leave";
}

//void MiniMode::MainScreen::paintEvent (QPaintEvent* ev)
//{
//    QStyleOption opt;
//    opt.init(this);
//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    p.setCompositionMode (QPainter::CompositionMode_Multiply);
//    p.fillRect (0, 0, this->width (), this->height (), QBrush (QColor (255, 255, 255, 50)));
//    QWidget::paintEvent (ev);
//}
