#ifndef COPYHELPER_H
#define COPYHELPER_H

#include <QString>
#include <QObject>
//#include "calendarevent.h"

class CopyHelper : public QObject
{
    Q_OBJECT
public:
    CopyHelper(QObject* parent = 0);
    QString static store(const QString& source, const QString& fileName);
    void static get(const QString& dest, const QString& from);

private:
    static QString randStr(int digits = 6);
};

#endif // COPYHELPER_H
