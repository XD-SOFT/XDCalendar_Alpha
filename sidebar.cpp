#include "sidebar.h"
#include <QPainter>
#include <QDebug>
#include <QImage>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QStyleOption>
#include "filelistwidget.h"
#include "config.h"
#include "userlistwidget.h"
#include "livewidget.h"

Sidebar::Sidebar(QWidget* parent): QWidget (parent)
{
    mPages = new QStackedWidget (this);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

#ifdef USED_USERLIST
    mTabBar = new QTabBar (this);
    connect (mTabBar, SIGNAL(currentChanged(int)), mPages, SLOT(setCurrentIndex(int)));
    mTabBar->setExpanding (true);
    layout->addWidget (mTabBar);
#endif

    layout->addWidget (mPages);
//    setAttribute (Qt::WA_TranslucentBackground);
    setMinimumWidth (0);

    initialize();
}

void Sidebar::addTab (QWidget* parent, const QString& name)
{
#ifdef USED_USERLIST
    mTabBar->addTab ("");
#endif

    mPages->addWidget (parent);
}

void Sidebar::resizeEvent (QResizeEvent* ev)
{
#ifdef USED_USERLIST
    if(mTabBar->count() > 0) {
        setStyleSheet (QString("QTabBar::tab {width: %1px; height: 56px;}").arg (mTabBar->width () / mTabBar->count ()));
    }
#endif

    QWidget::resizeEvent (ev);
}

void Sidebar::initialize()
{
    FileListWidget *pFileListWidget = new FileListWidget (this);
    Arg::fileListWidget = pFileListWidget;
//    addTab(pFileListWidget, "");
    addTab (pFileListWidget, /*"QIcon(":/Icon/Slide/Resources_default.png")"*/ "Resources");

#ifdef USED_USERLIST
    addTab (new UserListWidget (this), /* QIcon(":/Icon/Slide/Friends_default.png")*/ "Friends");
    addTab (new LiveWidget (this), /* QIcon(":/Icon/Slide/circle_default.png") */ "Circle");
#endif
}

void Sidebar::paintEvent (QPaintEvent* ev)
{
//    QWidget::paintEvent (ev);
    QPainter p (this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    p.setCompositionMode (QPainter::CompositionMode_Multiply);
//    if (background.isNull ())
//        p.fillRect (QRect(0, 0, width (), height ()), QBrush (Qt::transparent));
//    else
//    {
//        QImage rawImage = background.scaled (this->size ());
//        QImage image (rawImage.width (), rawImage.height (), rawImage.format ());
//        for (int i = 0; i < rawImage.width () - 1; ++i)
//        {
//            for (int j = 0; j < rawImage.height () - 1; ++j)
//            {
//                QColor color, c1 = rawImage.pixelColor (i, j), c2 = rawImage.pixelColor (i + 1, j), c3 = rawImage.pixelColor (i, j + 1), c4 = rawImage.pixelColor (i + 1, j + 1);
//                color.setRed ((c1.red () + c2.red () + c3.red () + c4.red ()) >> 2);
//                color.setGreen ((c1.green () + c2.green () + c3.green () + c4.green ()) >> 2);
//                color.setBlue ((c1.blue () + c2.blue () + c3.blue () + c4.blue ()) >> 2);
//                image.setPixelColor (i, j, color);
//            }
//        }
//        p.drawImage (0, 0, image);
//    }
    p.fillRect (0, 0, this->width (), this->height (), QBrush (QColor (255, 255, 255, static_cast<int>(255))));
//    QWidget::paintEvent (ev);
}

void Sidebar::setBackground (const QImage& image)
{
    background = image;
    update ();
}
