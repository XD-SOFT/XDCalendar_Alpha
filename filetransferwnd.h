#ifndef FILETRANSFERWND_H
#define FILETRANSFERWND_H

#include "framelessmodalmovableshadowwidget.h"
#include <QMap>

class itemWidget :public QWidget
{
    Q_OBJECT
public:
    explicit itemWidget(QWidget* parent = Q_NULLPTR);
    ~itemWidget();
    void setFileName(QString fileName);
    void setPrograss(int value);
    int  prograssValue();
};

namespace Ui {
class fileTransferWnd;
}

class fileTransferWnd : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    explicit fileTransferWnd(QWidget *parent = 0);
    ~fileTransferWnd();

public slots:
    /*!
     * \brief id 唯一标识.
     * \param percent  百分比.
     * \param fileInfo  文件信息（名称、上传或下载）.
     * \example transferPercent(1, "20", QMap<QString,QString> fileInfo);
     */
    void  transferPercent(int id, QString percent, QMap<QString,QString> fileInfo);

private :
    void addTransferItem(QString fileName, int id, bool upload = true);

private:
    Ui::fileTransferWnd *ui;
    QMap<int, itemWidget*> m_transList;
};

#endif // FILETRANSFERWND_H
