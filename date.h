#ifndef DATE_H
#define DATE_H

#include <QObject>

class Date : public QObject
{
    Q_OBJECT



public:
    explicit Date(QObject *parent = 0);

signals:

public slots:

public:
    static quint16 getSpringFestivalDate(int year);

    static int getFirstDayOfWeek(int year, int month);
    static int getTotalMonthDays(int year, int month);
    static int getMonthDays(int year, int month);
    static bool isLoopYaer(int year);
    static QString getLunarDate(int year, int month, int day);
    static QString getLunarMonAndDay(int year, int month, int day);
    // 计算今年是什么年如 ：甲子年
    static QString getLunarTime(int year);
private:
    static QString holiday(int month, int day);
    static QString solarTerms(int year, int month, int day);
    static QString lunarFestival(int month, int day);
};

#endif // DATE_H
