#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QWidget>
class QLabel;
class Lesson;
namespace MiniMode {
class InfoView : public QWidget
{
    Q_OBJECT
public:
    explicit InfoView(QWidget *parent = 0);

    void link (Lesson* lesson = nullptr);
    void unlink () { link (nullptr);}
public slots:
    void reload ();

signals:

protected:
    void setup ();
    void paintEvent (QPaintEvent*);
public slots:
    void setSubject (const QString& subject);
    void setLocation (const QString& process);
    void setClass (const QString& cl);
private:

    //label is updated automatically
    QLabel* mSubjectLabel;
    QLabel* mLocationLabel;
    QLabel* mClassLabel;

    Lesson* mLinkedLesson = nullptr;
};
}

#endif // USERINFOWIDGET_H
