#include "ipaddresswidget.h"
#include "ui_ipaddresswidget.h"

IPAddressWidget::IPAddressWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IPAddressWidget)
{
    ui->setupUi(this);
}

IPAddressWidget::~IPAddressWidget()
{
    delete ui;
}
