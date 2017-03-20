#ifndef TERMWIDGET_H
#define TERMWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMap>

class Term;
class Teacher;
class CourseGrid;
class Lesson;

class TermWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TermWidget (QWidget* parent);
    ~TermWidget () = default;
    void link (Teacher* user = nullptr);
    void unlink () {link (nullptr);}
    void selectTerm (int index);
    void selectTerm (Term*);
    void selectWeek (int index);
    void showOneDay (int weekDay);
    void showAllDay () {showOneDay (-1);}
    void setBackground (double opacity);
    QSize sizeHint () const;

    void hideCalendarSelect();

    void getCurretTimeLesson(Lesson *&pLesson, int &nLeftTime);

    void reloadLessons();
protected:
//    void paintEvent(QPaintEvent *ev);

signals:
    void requestShowLessons();

    void requestResizeMainWindow();

public slots:
    void reset ();
private:
    //terms and weekNums have the same length, and map to each other
    Teacher* mLinkedUser = nullptr;

    ///Mark to delete 2017.02.20,不能ui跟数据多少绑定.
//    QVector<QVector<CourseGrid*> > termCourseLists; //all of the courses for all terms
    QMap<Term*, QVector<CourseGrid*> > term2CourseList;

    CourseGrid* currentCourseGrid = Q_NULLPTR;
    QLayout* layout;
    double mOpacity = 1.0;
};



#endif // TERMWIDGET_H
