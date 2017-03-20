#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include <QDate>
//#include <QGridLayout>
//#include <QVBoxLayout>
//#include <QHBoxLayout>
//#include "mybutton.h"
//#include "daywidget.h"
//#include "term.h"
//#include "config.h"
#include "BaseShadowWidget.h"

class QGridLayout;
class QHBoxLayout;
class QVBoxLayout;
class QCalendarWidget;
class QEventLoop;
class CalenderSelectWidget;
class CourseCellWidget;
class TransparentButton;
class DayWidget;
class Term;
class Lesson;
class ResFilesDB;
class GetDataNW;
class QLabel;

//class CourseGridLayout : public QGridLayout
//{
//     Q_OBJECT

//  public:
//    void setCellFixedSize(int width, int height);
//    QSize sizeHint()const;

//private:
//    int m_nCellWidth;
//    int m_nCellHeight;
//};

class GridViewWeek : public QWidget//show a grid listed by weeks in main screen
{
    Q_OBJECT
public:
    static constexpr std::size_t daysPerWeek = 7;
    explicit GridViewWeek(QWidget *parent = 0);
    GridViewWeek(QWidget* parent, int dayNum, bool today);
    virtual ~GridViewWeek () = default;
signals:

public slots:
    virtual void reset() = 0;
    void rearrange(const QDate &curDate = QDate::currentDate());

    void rearrange(int i);

    void setBackground (const QImage& image);

    virtual void setBackground (double opacity);

protected:
    QGridLayout *grid = nullptr;
    int dayNum;
    bool currentDaySchem;
    TransparentButton *weekDayBtns[7];
    void paintEvent (QPaintEvent* event);
private:
    QHBoxLayout *weekLayout;
    TransparentButton *currentDay;
    QVBoxLayout *mainLayout;
    QImage mBackgroundImage;
    double mOpacity = 1.0;
};

//N行M列日期表格
//view days in a month
// listed by week
class DayGrid : public GridViewWeek
{
    Q_OBJECT
public:
    DayWidget *Days[6][7];

    explicit DayGrid(QWidget *parent = 0);
    ~DayGrid () = default;
public slots:
    void setMonth(const QDate &);
    void reset();
private:
    QDate date;
    int x_len,y_len;
    int x_maxlen,y_maxlen;
};

//view courses in a week
//In this Grid, every cell widget is required to be a CourseCellwidget
class CourseGrid :  public GridViewWeek
{
    Q_OBJECT
public:
    explicit CourseGrid (QWidget* parent = nullptr);
    ~CourseGrid();

    void link (Term* term, int weekNumber);

    void unlink();

    void showOneDay(int weekday);

    void showAllDay();

    virtual void setBackground(double opacity);

    QSize sizeHint () const;

    void mapWeekLessons(const QDate &date);

    void hideCalendarSelect();

    void getCurrentTimeLesson(Lesson *&pLesson, int &nLeftTime);

    void reloadLessons();

public slots:
    void reset ();

signals:
    void requestShowLessons();

    void requestResizeMainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    /*!
     * \brief addCourseCell,添加课程单元.
     * \param nSectionID, 课程section.
     * \param nRow, 课程添加的进grid的行数.
     * \param nWeek, 课程的周日期，1到7.
     * \param nStartTime, 课程开始上课时间.
     * \param nEndTime, 下课时间.
     * \return
     * \note 注意目前设计的week周日期，课程section都是从1开始.
     */
    CourseCellWidget *addCourseCell(int nSectionID, int nRow, int nWeek, int nStartTime,
                                    int nEndTime);

private:
    void setup ();

    void initializeDefaultClassGrid();

    void getTermByDate(const QDate &date);

private slots:
    void termLoadFinished();

    void setCurrentTerm(Term *pTerm);

    void showDateWidget();

    void changeToEdit();

    void changeToStandardMode();

    void queryTermByDateFinishied(const QJsonObject &jsonObj);

    //add by woody 2017-02-22
    //modified by bixiaoxia 2017-02-24.
    //modified by woody 2017-02-24 直接解析返回的json即可，里面包含了映射课程的所有所需信息
    void getFilesState(const QJsonObject&);  //获取每一个课程格子所需的文件

    /*!
     * \brief setLessonRemark,设置一周中detailid为nDetailID的课程备注为sRemark.
     * \param nDetailID
     * \param sRemark
     */
    void setLessonRemark(int nDetailID, const QString &sRemark);

private:
    ///@brief 当前请求显示课程的日期.
    QDate m_curDisplayDate;
//    QVBoxLayout* headerLayout;
    QLayout *weekHeaderLayout/*, *courseGridLayout*/;

    TransparentButton *placeHolderButton;

    Term* mLinkedTerm = Q_NULLPTR;
    Lesson* currentPointedLesson = nullptr;
    int mWeekNumber;

    //CourseCellWidget对象二维数组，第一维为section，代表课程日期表中的section课程，第二维度为weekday.
    QVector<QVector<CourseCellWidget*> > cellWidgets;

    //课间间隔保存.
    QList<QWidget*> m_breakWidgetList;

    //一周课程数组，数组从0到6代表周一到周日.QMap<int, Lesson^*>int为section.
    QVector<QMap<int, Lesson*> > m_weekLessonVec;

    //课节及break文本显示部件list.
    QList<QWidget*> m_textDisplayWgtList;

    //日期选择部件.
    CalenderSelectWidget *m_pCalendarWgt = Q_NULLPTR;

    int m_nBreakNum;

    int showingDay = -1;

    double m_fOpacity = 1.0f;

    int m_nCurDisplayCourseDateWeekNum = -1;

    ///@brief 事件循环，为了请求网络数据时候暂时阻塞ui.
    QEventLoop *m_pEvLoop = Q_NULLPTR;

    //add by woody 2017-02-22
//    GetDataNW* getNWDate = nullptr;
//    ResFilesDB* res_file = nullptr;

    QHash<int, CourseCellWidget*> m_resDetailIDCourseCellWgtHash; //请求资源的课程detailID号与对应的widgethash表格.

    bool m_bMustReload = false;
};

class BreakWidget : public QWidget
{
    Q_OBJECT

public:
    BreakWidget(const QString &sText, QWidget *parent = Q_NULLPTR);

    void setBackgroundAlpha(qreal alpha);

protected:
    void paintEvent(QPaintEvent *pEv);

private:
    qreal m_fAlapha;
    QLabel *m_pTextLb;
};

class CalenderSelectWidget : public BaseShadowWidget
{
    Q_OBJECT

public:
    CalenderSelectWidget(QWidget *parent = Q_NULLPTR);

    ~CalenderSelectWidget();

    void setCurrentDate(int year, int month);

protected:
    void leaveEvent(QEvent *pEv);

signals:
    void dateSelected(const QDate &date);

private:
    QCalendarWidget *m_pCalendarWgt;
};

#endif // GRIDVIEW_H
