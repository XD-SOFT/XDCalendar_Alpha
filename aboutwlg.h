#ifndef ABOUTWLG_H
#define ABOUTWLG_H

#include <QWidget>
#include "framelessmodalmovableshadowwidget.h"
namespace Ui {
class aboutWlg;
}

class aboutWlg : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    explicit aboutWlg(QString fileName, QWidget *parent = Q_NULLPTR);
    ~aboutWlg();

signals:
    void closeWnd();

private:
    Ui::aboutWlg *ui;
};

#endif // ABOUTWLG_H
