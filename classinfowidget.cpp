#include "classinfowidget.h"
#include "ui_classinfowidget.h"

ClassInfoWidget::ClassInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClassInfoWidget)
{
    ui->setupUi(this);
}

ClassInfoWidget::~ClassInfoWidget()
{
    delete ui;
}
