#include "titlebar.h"
#include "mybutton.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
TitleBar::TitleBar(QWidget *parent)
{
    mTitle = new QLabel (this);
    mTitle->setObjectName ("title");

    mClose = new TransparentButton (this);
    mClose->setIcon (QIcon(":/Icon/1168703.png"));
    mClose->setIconSize({24, 24});
    mClose->setObjectName ("CloseButton");
    mClose->setHoverdColor(QColor("#d74631"));
//    connect (mClose, SIGNAL (clicked()), this, SIGNAL (tbClose ()));
    connect (mClose, &QPushButton::clicked, [this] ()
    {
        emit this->tbClose ();
    });

    auto layout = new QHBoxLayout (this);
    layout->addWidget (mTitle, 5);
    layout->addStretch (1);
    layout->addWidget (mClose);
    layout->addSpacing(1);

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
}

TitleBar::TitleBar(const QString& title, QWidget *parent) : QWidget(parent)
{
    mTitle = new QLabel (title, this);
    mTitle->setObjectName ("title");

    mClose = new TransparentButton (this);
    mClose->setIcon (QIcon(":/Icon/1168703.png"));
    mClose->setIconSize({24, 24});
    mClose->setObjectName ("CloseButton");
    mClose->setHoverdColor(QColor("#d74631"));
//    connect (mClose, SIGNAL (clicked()), this, SIGNAL (tbClose ()));
    connect (mClose, &QPushButton::clicked, [this] ()
    {
        emit this->tbClose ();
    });

    auto layout = new QHBoxLayout (this);
    layout->addWidget (mTitle, 5);
    layout->addStretch (1);
    layout->addWidget (mClose);
    layout->addSpacing(1);

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Maximum);
}

void TitleBar::setTitle(const QString &sTitle)
{
    mTitle->setText(sTitle);
}

void TitleBar::paintEvent (QPaintEvent* )
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
