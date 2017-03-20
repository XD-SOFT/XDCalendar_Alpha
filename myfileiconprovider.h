#ifndef MYFILEICONPROVIDER_H
#define MYFILEICONPROVIDER_H
#include <QFileIconProvider>
#include <QIcon>
class QFileInfo;
class MyFileIconProvider: public QFileIconProvider
{
public:
    MyFileIconProvider();
    QIcon icon (const QFileInfo& fileinfo) const;
};

#endif // MYFILEICONPROVIDER_H
