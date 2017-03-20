#ifndef COURSECELLWIDGET_H
#define COURSECELLWIDGET_H

#include <QFrame>
#include "config.h"
//#include <QDate>
//#include <QWidget>
//#include "mybutton.h"
//#include "database.h"
//#include "filenumwidget.h"
//#include "lesson.h"
//#include "teacher.h"
//#include "term.h"
//#include "colorbutton.h"
//#include "editbutton.h"
//#include "editwindow.h"
//#include "uploadfiles2ftp.h"
//#include "downloadfile.h"

class QLabel;
class Folder;
class EditWindow;
class QGraphicsEffect;
class EventData;
class QSlider;
class QVBoxLayout;
class EventTextButton;
class FileNumWidget;
class ColorButton;
class EditButton;
class UploadFiles2Ftp;
class DailyData;
class QGraphicsDropShadowEffect;
class CourseColorPickerWidget;

//日期单元格中显示事件的部分
class TextShowWindowXD : public QFrame
{
    Q_OBJECT
public:
    friend class CourseCellWidget;
    TextShowWindowXD(QWidget *parent);
    void reset(QList<EventData*> text);
    void reset(DailyData *data);
    QWidget *MainWidget;//所有的事件方块都放入此QWidget,与主界面不建立布局关系，手动布局遮罩

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QList<EventData*>text;

    //DailyData *data;//从父对象DayWidget继承
    QSlider *verticalSlider;

    QVBoxLayout *MainLayout;//MainWidget内部对每个事件方块的布局
    EventTextButton* addEvent(EventData *e);//根据事件信息构造按钮块
    void clear(){}
public slots:
    void NewEvent();
    void resetSize()
    {
        //MainWidget->resize(this->width(),data->text.size()*Const::MyButtonSize);
        MainWidget->resize(this->width(), text.size()*Const::MyButtonSize);
    }
};



class CourseCellWidget : public QWidget  /* public QFrame */
{
    Q_OBJECT
public:
    explicit CourseCellWidget(QWidget *parent = 0);
    ~CourseCellWidget();

    bool isEditStatus() const;

    QFileInfoList getFileList(QString path);
    void fileCopy(QString &fromDir, QString &toDir, QString &fileName);
    bool copyFileToPath(QString sourceDir ,QString toDir, QString fileName, bool coverFileIfExist);
    bool copyDirectoryFiles(QString dateString, Folder* folder, QString fromDir, QString toDir, bool coverFileIfExist);

    Lesson* getLesson();
    FileNumWidget* getFileNumWidget() { return fileNumWidget; }
    //根据内置变量重置样式表
    void resetStyleSheet();
    //void setDate(const QDate &);
    void setBackgroundAlpha(double);

    void link (Lesson* lesson = nullptr);
    void unlink ();
    QSize sizeHint () const;

    int getSecIndex(){ return secIndex; }
    int getDayIndex(){ return dayIndex; }
    void setSecIndex(int i){ secIndex = i; }
    void setDayIndex(int i){ dayIndex = i; }
    void setOpacity (double o);

    void setStartTime(int nSeconds);

    int getStartTime() const;

    void setEndTime(int nSeconds);

    int getEndTime() const;

    void setStartTime(const QString &sStartTime);

    void getStartTime(QString &sStartTime);

    void setEndTime(const QString &sEndTime);

    void getEndTime(QString &sEndTime);

signals:
    void localFinish(QMap<QString, QString>);

    /*!
     * \brief requestShowLessons,课程存在，请求显示课程展示部件信号，这个以后用专门的信号控制类，避免每个CourseCellWidget对象都要连接.
     */
    void requestShowLessons();

public slots:
    void delCourse();
//    void getDBState(const QJsonObject &);
    void reset();//根据当前lesson重置格子内部元件s
    void setColor();
    void showEditWindow();
    void setEdit(bool bEditEnabled);

private slots:
    void lostActiveted();

    void fileUploadFinished(const QString &fileName);

private slots:
    /*!
    @ \brief 设置LinkedLesson颜色.
    @ \param color,要设置的颜色.
    */
    void setLinkedLessonColor(const QColor &color);

    /*!
    @ \brief 撤销linkedLesson颜色设置,恢复为上一个颜色.
    */
    void cancelSetLinkedLessonColor();

    /*!
     * \brief addLessonResource
     * \param pLinkedLesson, 这个参数只能暂时先放这里这么处理，这些需要重构，做一个总体管理.
     * \param sFileName
     * \param sFileDir
     */
    void addLessonResource(Lesson *pLinkedLesson, const QString &sFileName, const QString &sFileDir);


    void deleteLessonFinished(int nDetailID, const QJsonObject &jsonObj);

    void upLoadFinished();

    void uploadFileError(const QString &sFilePathName, const QString &sError);

protected:

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    void mouseMoveEvent (QMouseEvent* event);
    void enterEvent (QEvent* ev);
    void leaveEvent (QEvent* ev);
    bool eventFilter(QObject *watched, QEvent *event);
    void paintEvent(QPaintEvent *event);
    void setup ();
    void resizeEvent (QResizeEvent* ev);

private:
    ///< \brief 创建weekday索引.
    void createWeekDay();

private:
    int secIndex = -1, dayIndex = -1;
    Lesson* mLinkedLesson = Q_NULLPTR;
//    LessonDetailDB* lesDB = nullptr;

    QMap <QString, QString> fileMap; //filename and path
    QMap <QString, QString> lessonMap; //右侧课程信息
    QMap <QString, QString> sourcefileMap;//filename and source path
    QMap <QString, QString> testMap;//filename and source path

    FileNumWidget* fileNumWidget;
    ColorButton* colorButton;
    EditButton* editButton;
    //EditButton* delButton;

    QAction *delAction;
    QLabel *courseLabel, *locationLabel, *classLabel;
    EditWindow *editWindow;

    double mBackgroundAlpha;
    QList<EventData*>my_text;
    QList<QUrl>my_urls;

    QString m_sStartTime; ///< \brief 上课时间.

    QString m_sEndTime; ///< \brief 下课时间.

    UploadFiles2Ftp* upload;
    downloadFile* down = Q_NULLPTR;
    QGraphicsDropShadowEffect *m_pShadowEffect;
    CourseColorPickerWidget *m_pColorPicker;
    QColor m_linkedLessonCurColor;
    double mOpacity;

    int m_nStartTimeSeconds;

    int m_nEndTimeSeconds;

    ///< \brief，星期几查询.
    /// < \param int, index
    /// < \param QString,星期几.
    QMap<int, QString> m_weekDayMap;

    ///Mark,暂时这么用着，重构不允许这样，上传下载要移到线程里去.
    class QProgressBar *m_pProgressBar = Q_NULLPTR;

    ///< \brief 是否已经记录linkedLesson设置之前的color.
    bool m_bNeedRecordColor;

    /// < \brief 是否四个角保持同样弧度.
    bool m_bKeepSameRadius;


    bool mActivated = false;

    bool m_bEditEnabled = false; //是否处在编辑状态.

    bool m_bCreateNewFile = false;

    //signals:
//    void newEvent(const QDate& date, QString file = QString(), QString fileName=QString());
};

#endif // COURSECELLWIDGET_H
