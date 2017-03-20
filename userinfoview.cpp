#include "userinfoview.h"
#include "teacher.h"
#include "config.h"
#include "masklabel.h"
#include <QIcon>
#include <QString>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBitmap>
#include <QResizeEvent>
#include <cmath>
#include <QDebug>

UserInfoView::UserInfoView(QWidget *parent) : QWidget(parent)
{
    setup ();

//    QVBoxLayout* rightLayout = new QVBoxLayout ();
//    rightLayout->setSpacing (5);
//    rightLayout->setMargin (5);
//    rightLayout->addWidget (mNameLabel);
//    rightLayout->addWidget (mExpBar);

    QHBoxLayout* mainLayout = new QHBoxLayout (this);
    mainLayout->setContentsMargins(20, 0, 0, 0);
    mainLayout->addWidget (mIconLabel);
    mainLayout->addSpacing(0);
    mainLayout->addWidget(mNameLabel);

//    setLayout(mainLayout);

    link (Arg::user);
    reload ();

    setStyleSheet("background-color: white");
}
void UserInfoView::setup ()
{
    //    mExpBar = new QProgressBar (this);
//    mExpBar->setFormat ("%v/%m");
//    mExpBar->setAlignment (Qt::AlignRight);

    //mIconLabel = new QLabel (this);
    mIconLabel = new MaskLabel;
    connect(mIconLabel, SIGNAL(clicked()), this, SLOT(showHeaderIconSelectWidget()));
    mIconLabel->setFixedWidth(50);
    mIconLabel->setFixedHeight(50);
    mIconLabel->setScaledContents (true);
    mIconLabel->setStyleSheet("background-color: green");
    QPixmap pixmap;
    pixmap.load(":/Icon/images.jpg");
    mIconLabel->setPixmap(pixmap);
    mIconLabel->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Preferred);
    //mIconLabel->setObjectName (QStringLiteral ("Icon"));

    mNameLabel = new QLabel;
    mNameLabel->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Preferred);
    mNameLabel->setObjectName (QStringLiteral ("Name"));
    mNameLabel->setStyleSheet("background-color: red");
}

UserInfoView::~UserInfoView () = default;

void UserInfoView::reload ()
{
    mNameLabel->setText (mLinkedUser->getName ());
//    mNameLabel->setText(tr("刘红业"));
    mNameLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    QPixmap iconPix = mLinkedUser->openIcon ();
    if(!iconPix.isNull()) {
        mIconLabel->setPixmap (iconPix.scaled (mIconLabel->size (), Qt::KeepAspectRatioByExpanding));
    }


//    mExpBar->setValue (mLinkedUser->exp ());
//    mNameLabel->setText(tr("刘红业"));
    //mNameLabel->setText (mLinkedUser->getName ());
    //------------2017-01-04 王钦注释 为显示出头像------------------------//
    //mIconLabel->setPixmap (mLinkedUser->openIcon ().scaled (mIconLabel->size (), Qt::KeepAspectRatioByExpanding));
    //----------------------------------------------------------------//

    //    mExpBar->setValue (mLinkedUser->exp ());
    //    mExpBar->setMaximum (mLinkedUser->maxExp ());
}

void UserInfoView::link (const Teacher* user)
{
    if (mLinkedUser != nullptr)
    {
        disconnect (mLinkedUser, SIGNAL (levelChanged (Upgradable)), this, SLOT (reload ()));
        disconnect (mLinkedUser, SIGNAL (iconChanged (QFileInfo)), this, SLOT (reload ()));
    }
    mLinkedUser = user;
    if (mLinkedUser != nullptr)
    {
        connect (mLinkedUser, SIGNAL (levelChanged (Upgradable)), this, SLOT (reload ()));
        connect (mLinkedUser, SIGNAL (iconChanged (QFileInfo)), this, SLOT (reload ()));
    }
}

void UserInfoView::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UserInfoView::resizeEvent(QResizeEvent *event)
{
    int nAdjustSize = event->size().width() > event->size().height() ? event->size().height() : event->size().width();
    mIconLabel->setFixedSize(nAdjustSize, nAdjustSize);
//    QPixmap iconPix = mLinkedUser->openIcon ();

//    if(!iconPix.isNull()) {
//        mIconLabel->setPixmap (iconPix.scaled (mIconLabel->size (), Qt::KeepAspectRatioByExpanding));
//    }
//    else {
//        QPixmap pixmap;
//        pixmap.load(":/Icon/images.jpg");
//        mIconLabel->setPixmap(pixmap.scaled(mIconLabel->size (), Qt::KeepAspectRatioByExpanding));
//    }
}
