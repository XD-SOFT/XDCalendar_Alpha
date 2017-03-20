#include "miniuserinfowidget.h"
#include "minimode.h"
#include "lesson.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
using namespace MiniMode;
InfoView::InfoView(QWidget *parent) : QWidget(parent)
{
    setup ();

    QVBoxLayout* mainLayout = new QVBoxLayout (this);
    mainLayout->setSpacing (0);
    mainLayout->setMargin (0);
    mainLayout->addWidget (mSubjectLabel);
    mainLayout->addWidget (mLocationLabel);
    mainLayout->addWidget (mClassLabel);
}

void InfoView::setup ()
{
    mSubjectLabel = new QLabel (this);
    mSubjectLabel->setObjectName (QStringLiteral ("Subject"));
    mSubjectLabel->setAlignment(Qt::AlignCenter);

    mLocationLabel = new QLabel (this);
    mLocationLabel->setObjectName (QStringLiteral ("Location"));
    mLocationLabel->setAlignment(Qt::AlignCenter);

    mClassLabel = new QLabel (this);
    mClassLabel->setObjectName (QStringLiteral ("Class"));
    mClassLabel->setAlignment(Qt::AlignCenter);
}

void InfoView::setSubject (const QString& subject)
{
    mSubjectLabel->setText (subject);
}

void InfoView::setLocation (const QString& location)
{
    mLocationLabel->setText (location);
}
void InfoView::setClass (const QString& cl)
{
    mClassLabel->setText (cl);
}

void InfoView::link (Lesson* lesson)
{
    if (mLinkedLesson != nullptr)
    {
        disconnect (lesson, SIGNAL (subjectChanged (const QString&)), this, SLOT (setSubject (const QString&)));
        disconnect (lesson, SIGNAL (locationChanged (const QString&)), this, SLOT (setLocation (const QString&)));
        disconnect (lesson, SIGNAL (unitChanged (const QString&)), this, SLOT (setClass (const QString)));
    }
    mLinkedLesson = lesson;
    if (mLinkedLesson != nullptr)
    {
        connect (lesson, SIGNAL (subjectChanged (const QString&)), this, SLOT (setSubject (const QString&)));
        connect (lesson, SIGNAL (locationChanged (const QString&)), this, SLOT (setLocation (const QString&)));
        connect (lesson, SIGNAL (unitChanged (const QString&)), this, SLOT (setClass (const QString)));

        reload();
    }
}

void InfoView::reload ()
{
    if (mLinkedLesson != nullptr)
    {
        setSubject (mLinkedLesson->subject ());
        setLocation (mLinkedLesson->location ());
        setClass (mLinkedLesson->unit ());
    }
    else
    {
        setSubject ("NO");
        setLocation ("CLASS");
        setClass ("NOW");
    }
}

void InfoView::paintEvent (QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
