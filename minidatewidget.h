#ifndef DATEWIDGET_H
#define DATEWIDGET_H

#include <QWidget>
#include <QDate>
#include <QTimer>
class QLabel;
namespace MiniMode {
class MiniDateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MiniDateWidget(QWidget *parent = 0);
    void setInterval (int msec);// automatically stop the timer
    void setTimer (bool on);
signals:

public slots:
    void updateRemaintimeLabel ();
    void updateTodayLabel ();
protected:
    void setup ();
    void paintEvent (QPaintEvent*);

private:
    QLabel* mRemainTimeLabel = nullptr;
    QLabel* mTodayLabel = nullptr;

    QDate mToday = QDate::currentDate ();
    QTimer mRemainTime;
    QTimer mUpdateTimer;
};
}

#endif // DATEWIDGET_H
