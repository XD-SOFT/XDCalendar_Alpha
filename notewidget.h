#ifndef NOTEWIDGET_H
#define NOTEWIDGET_H

#include "baseshadowwidget.h"
#include <QPoint>
#include <QList>
#include <QQUeue>
//#include <QDebug>
//#include "lessonremarkdb.h"
class Lesson;
class QTextEdit;
class QPushButton;
class QLabel;

/*!
* \Mark, edit by BiXiaoxia 2016.12.21,改变基类，变为有阴影的效果.
*/
class NoteWidget : public BaseShadowWidget
{
    Q_OBJECT
public:
    //make a notewidget, linked to lesson, show at position. When mouse is out of it's geometry and region, the notewidget is closed;
    static NoteWidget* makeInstance (Lesson* lesson, const QRegion& region, const QPoint& position = QPoint ());
    //simply access to the notewidget singleton
    static NoteWidget* getInstance ()
    {
        return instance;
    }
    //remove the notewidget
    static void cleanInstance ()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }

    void handleLessonRemarkDB();

    void setLabel (const QString& string);

signals:
    void noteWidgetClosed();

    /*!
     * \brief lessonResourceAddComplete
     * \param pLinkedLesson
     * \param sFileName
     * \param sFilePathName
     */
    void lessonResourceAddComplete(Lesson *pLinkedLesson, const QString &sFileName, const QString &sFilePathName);

private:
    void resetUI();

protected slots:
    void reload ();

private slots:
//    void setLessonRemark(const QString &sRemark);

protected:
    void link (Lesson* lesson = nullptr);
//    void closeEvent (QCloseEvent* )
//    {
//        instance = nullptr;
//    }
    Lesson* lesson () {return mLinkedLesson;}
//    void enterEvent(QEvent *event);
    void leaveEvent (QEvent* );
    void focusOutEvent (QFocusEvent*);
//    void mouseMoveEvent (QMouseEvent*);
    void mousePressEvent (QMouseEvent*);
//    void paintEvent (QPaintEvent* ev);

    void drawExcludeShadowPart(QPainter *painter, QPaintEvent* ev) Q_DECL_FINAL;

private slots:
    void createPdf();

    void createPPT();

    void createDoc();

    void recordClass();

    void cancelRemark();
private:
    static NoteWidget* instance;
    explicit NoteWidget(const QRegion& region, QWidget *parent = 0);
    ~NoteWidget();

//    QString lastNote;
//    LessonRemarkDB* lesDB = Q_NULLPTR;
    Lesson* mLinkedLesson = nullptr;

//    QQueue<Lesson*> m_reqNoteLessonQueue;  //请求note的课程队列.

    QLabel* mLabel = nullptr;
    QTextEdit* mEdit = nullptr;
    QPushButton* mCancel = nullptr;
    QList<QPushButton*> mTools;
    QRegion mExternRegion;
    QMetaObject::Connection noteChanged, noteCancel;
    static int m_nTopMargin;

    ///< \brief 避免重复对remark进行操作.
    bool m_bRemarkHandled;
};
//class MousePressEater: public QObject
//{
//    Q_OBJECT
//public:
//    MousePressEater (QObject* target) : mTarget (target) {}
//protected:
//    bool eventFilter (QObject* obj, QEvent* event);
//    QObject* mTarget = nullptr;
//};

#endif // NOTEWIDGET_H
