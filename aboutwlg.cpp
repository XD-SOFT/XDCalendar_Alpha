#include "aboutwlg.h"
#include "ui_aboutwlg.h"
#include <QFile>
#include <QTextStream>
aboutWlg::aboutWlg(QString fileName, QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::aboutWlg)
{
    ui->setupUi(this);
    setShadowWidth(30);
    connect (ui->pTitleBarWgt, SIGNAL(tbClose()), this, SLOT(close()));
    ui->pTitleBarWgt->setTitle(tr("关于MoonCalendar"));
    ui->lIcon->setStyleSheet("QLabel#lIcon{border-image:url(:/Icon/MoonCalendar.ico)}");
    connect(ui->pOkBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pOkBtn, SIGNAL(clicked(bool)), this, SIGNAL(closeWnd()));
    QFile file(fileName);
    if(file.open(QFile::ReadOnly))
    {
        QTextStream out(&file);
        ui->lText->setText(out.readAll());
        file.close();
    }
    this->setWindowOpacity(0.9);

}

aboutWlg::~aboutWlg()
{
    delete ui;
}
