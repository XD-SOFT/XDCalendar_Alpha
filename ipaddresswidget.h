#ifndef IPADDRESSWIDGET_H
#define IPADDRESSWIDGET_H

#include <QWidget>

namespace Ui {
class IPAddressWidget;
}

class IPAddressWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IPAddressWidget(QWidget *parent = 0);
    ~IPAddressWidget();

private:
    Ui::IPAddressWidget *ui;
};

#endif // IPADDRESSWIDGET_H
