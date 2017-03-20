#ifndef TIMEDIFF_H
#define TIMEDIFF_H
#include <QString>
#include <QDateTime>

class TimeDiff
{
public:
    static QString Between(const QDateTime& from, const QDateTime& to);
    static void Test();
};

#endif // TIMETO_H
