#include "aboutwlg.h"
#include "ui_aboutwlg.h"
#include <QFile>
#include <QTextStream>
aboutWlg::aboutWlg(QString fileName, QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::aboutWlg)
{
    ui->setupUi(this);

    //设置消息框为模态窗口
    this->setAttribute(Qt::WA_ShowModal, true);
    setShadowWidth(30);
    setWindowOpacity(0.9);

    ui->pTitleBarWgt->setTitle(tr("关于MoonCalendar"));
    ui->lIcon->setStyleSheet("QLabel#lIcon{border-image:url(:/Icon/MoonCalendar.ico)}");

    connect(ui->pOkBtn, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->pOkBtn, SIGNAL(clicked(bool)), this, SIGNAL(closeWnd()));
    connect (ui->pTitleBarWgt, SIGNAL(tbClose()), this, SLOT(close()));

    QFile file(fileName);
    if (file.open(QFile::ReadOnly))
    {
        QTextStream out(&file);
        ui->lText->setText(out.readAll());
        file.close();
    }

}

aboutWlg::~aboutWlg()
{
    delete ui;
}
