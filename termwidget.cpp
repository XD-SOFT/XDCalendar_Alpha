#include "termwidget.h"
#include "coursecellwidget.h"
#include "gridviewweek.h"
#include "term.h"
#include "config.h"
#include "teacher.h"
#include <iostream>
#include <QHBoxLayout>

TermWidget::TermWidget(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout (this);
    layout->setContentsMargins (10, 10, 0, 10);
    setSizePolicy (QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    connect (Arg::getInstance (), static_cast<void(Arg::*)(Term*)>(&Arg::currentTermChanged), [this] (Term* term) {
        this->selectTerm (term);
    });
    connect (Arg::getInstance (), static_cast<void(Arg::*)(int)>(&Arg::currentWeekNumberChanged), [this] (int week) {
        this->selectWeek (week);
    });

    if(currentCourseGrid == Q_NULLPTR) {
        currentCourseGrid = new CourseGrid;
        connect(currentCourseGrid, &CourseGrid::requestShowLessons, this, &TermWidget::requestShowLessons);
        connect(currentCourseGrid, &CourseGrid::requestResizeMainWindow, this, &TermWidget::requestResizeMainWindow);
        connect(currentCourseGrid, &CourseGrid::termWeekChanged, this, &TermWidget::termWeekChanged);
        layout->addWidget(currentCourseGrid);
    }

//    setShadowWidth(10);
//    setShadowEnabled(ShadowSide::Left, false);
//    setShadowEnabled(ShadowSide::Top, false);
//    setShadowEnabled(ShadowSide::Bottom, false);
//    setShadowEnabled(ShadowSide::Right, false);

//    setShadowPixmapBySide(ShadowSide::Right, "./image/top_sd.png");
}

void TermWidget::link (Teacher* user)
{
    if (mLinkedUser != nullptr)
    {
//        for (const auto& list: termCourseLists)
//            for (const auto& cg: list)
//                delete  cg;
//        termCourseLists.clear ();
////        currentCourseGrid = nullptr;
//        term2CourseList.clear ();
    }
    mLinkedUser = user;
    if (mLinkedUser != nullptr)
    {
        ///Mark 2017.02.17,暂时没法同用户连接，暂时这个逻辑用Arg替换.
        ///
//        if(mLinkedUser->terms().size() > 0) {
//             selectTerm(0);
//             currentCourseGrid->link();
//        }
        Term *pTerm = Arg::currentTerm();
        if(pTerm != nullptr) {
            ///Mark to delete 2017.02.20, 去掉这个逻辑，感觉不太对劲.
//            currentCourseGrid->link(pTerm, pTerm->getTermWeeksNumber());
            ///Mark End.
        }

//        for (const auto& term: mLinkedUser->terms ())
//        {
//            QVector<CourseGrid*> cgList;
//            try
//            {
//                for (int i = 0; i < term->weekNumber (); ++i)
//                {
//                    CourseGrid* cg = new CourseGrid (this);
//                    connect(cg, &CourseGrid::requestShowLessons, this, &TermWidget::requestShowLessons);
//                    cg->hide ();
//                    cg->link (term, i);
//                    cgList.append (cg);
//                }
//                termCourseLists.append (cgList);
//                term2CourseList[term] = cgList;
//            }
//            catch (std::exception& e)
//            {
//                std::cerr << e.what () << std::endl;
//            }
//        }
//        selectTerm (0);
    }
}

void TermWidget::reset ()
{

}

void TermWidget::selectWeek (int index)
{
//    if (Arg::currentTerm () != nullptr && term2CourseList.contains (Arg::currentTerm ()) && index >= 0 && index < Arg::currentTerm ()->weekNumber ())
//    {
//        if (currentCourseGrid != nullptr)
//        {
//            currentCourseGrid->hide ();
//            layout->takeAt (0);
//        }
//        currentCourseGrid = term2CourseList[Arg::currentTerm ()].at (index);
//        layout->addWidget (currentCourseGrid);
//        currentCourseGrid->show ();
//        currentCourseGrid->setBackground (mOpacity);
//        Arg::setCurrentWeekIndex (index);
//    }
}

void TermWidget::selectTerm (int index)
{
    if (mLinkedUser == nullptr)
        return;
    if (index < 0 || index >= mLinkedUser->terms ().size ())
        return;
    auto term = mLinkedUser->terms ().at (index);
    Arg::setCurrentTerm (term);
    selectWeek (0);
    //course grid will be set by selectWeek
}

void TermWidget::selectTerm (Term* term)
{
    if (term == nullptr)
        return;
    if (!mLinkedUser->terms ().contains (term))
        return;
    Arg::setCurrentTerm (term);
}

void TermWidget::showOneDay (int weekday)
{
//    qDebug() << "one day the current weekday is:" << weekday;

    if (currentCourseGrid == nullptr)
        return;
    if (weekday < 0 || weekday >= 7)
        currentCourseGrid->showAllDay ();
    else
        currentCourseGrid->showOneDay (weekday);
}

void TermWidget::setBackground (double opacity)
{
    mOpacity = opacity;
    currentCourseGrid->setBackground (opacity);
    update ();
}

QSize TermWidget::sizeHint() const
{
    return currentCourseGrid ? (currentCourseGrid->sizeHint() + QSize(20, 20)) : QSize(20, 20);
}

void TermWidget::hideCalendarSelect()
{
    currentCourseGrid->hideCalendarSelect();
}

void TermWidget::getCurretTimeLesson(Lesson *&pLesson, int &nLeftTime)
{
    currentCourseGrid->getCurrentTimeLesson(pLesson, nLeftTime);
}

void TermWidget::reloadLessons()
{
    currentCourseGrid->reloadLessons();
}

//void TermWidget::paintEvent(QPaintEvent *ev)
//{
//    QPainter painter(this);
//    painter.fillRect(0, 0, width() - 10, height(), QColor(255, 255, 255, 0));
//}
