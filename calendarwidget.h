#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H
#include <QCalendarWidget>
#include <QDate>
class QPainter;

class CalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = Q_NULLPTR);

private:
    void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private slots:
    void dateClick(const QDate &date);
    void currentPageChanged(int year, int month);
    void comeToday();
};

#endif // CALENDARWIDGET_H
