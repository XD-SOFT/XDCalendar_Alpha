#include "userinfowidget.h"
#include "ui_userinfowidget.h"
#include "teacher.h"
#include "config.h"
#include "messagedisplaywidget.h"
#include <QPainter>
#include <QResizeEvent>
#include <QSignalMapper>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "ui_iconselectwidget.h"
#include "titlebar.h"

UserInfoWidget::UserInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoWidget)
{
    ui->setupUi(this);

//    ui->pNameLb->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->pNameBtn->setStyleSheet("font-size: 12pt; color: white");

    connect(ui->pIconLb, SIGNAL(clicked()), this, SLOT(showHeaderIconSelectWidget()));

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    ///Mark,这些key的稳定字符串值重构需要提出来做处理.
    QString sKey = Arg::username + "userSelectIcon";
    QVariant var = configSettings.value(sKey, QPixmap());
    QPixmap iconPix;
    if(var.canConvert(QMetaType::QPixmap)) {
        iconPix = var.value<QPixmap>();
    }

    if(iconPix.isNull()) {
        iconPix.load(":/Icon/Settings/Head1.png");
    }

    ui->pIconLb->setPixmap(iconPix);

    link(Arg::user);
}

UserInfoWidget::~UserInfoWidget()
{
    delete ui;
}

void UserInfoWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UserInfoWidget::resizeEvent(QResizeEvent *event)
{
    int nAdjustSize = event->size().width() > event->size().height() ? event->size().height() : event->size().width();
    ui->pIconLb->setFixedSize(nAdjustSize, nAdjustSize);
}

void UserInfoWidget::reload()
{
//    QString sUserName = mLinkedUser->getName().trimmed();
//    if(sUserName.isEmpty()) {
//        sUserName.append(tr("刘红业"));
//    }

    QString sText = Arg::sNickName;
    ui->pNameBtn->setText(Arg::sNickName);

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "userSelectIcon";
    QVariant var = configSettings.value(sKey, QPixmap());
    QPixmap iconPix;

    if(var.canConvert(QMetaType::QPixmap)) {
        iconPix = var.value<QPixmap>();
    }


    if(iconPix.isNull()) {
        iconPix.load(":/Icon/Settings/Head1.png");
    }

    ui->pIconLb->setPixmap(iconPix);

//    QPixmap iconPix = mLinkedUser->openIcon();
//    if(!iconPix.isNull()) {
//        ui->pIconLb->setPixmap(iconPix.scaled (ui->pIconLb->size (), Qt::KeepAspectRatioByExpanding));
//    }
//    else {
//        QPixmap pixmap;
//        pixmap.load(":/Icon/images.jpg");
//        ui->pIconLb->setPixmap(pixmap);
//    }
}

void UserInfoWidget::link(const Teacher *user)
{
    if (mLinkedUser != nullptr)
    {
        disconnect (mLinkedUser, SIGNAL (levelChanged (Upgradable)), this, SLOT (reload ()));
        disconnect (mLinkedUser, SIGNAL (iconChanged (QFileInfo)), this, SLOT (reload ()));
    }

    mLinkedUser = user;

    if (mLinkedUser != nullptr)
    {
        connect (mLinkedUser, SIGNAL (levelChanged (Upgradable)), this, SLOT (reload ()));
        connect (mLinkedUser, SIGNAL (iconChanged (QFileInfo)), this, SLOT (reload ()));
    }

    reload();
}

void UserInfoWidget::unlink()
{
    link();
}

void UserInfoWidget::showHeaderIconSelectWidget()
{
    if(m_pIconSelectWgt == Q_NULLPTR) {
        m_pIconSelectWgt = new IconSelectWidget(this);
        connect(m_pIconSelectWgt, &IconSelectWidget::iconSelectComplete, this, &UserInfoWidget::setHeaderIcon);
    }

    m_pIconSelectWgt->show();
}

void UserInfoWidget::setHeaderIcon(const QPixmap &pix)
{
//    QPixmap pix = icon.pixmap(ui->pIconLb->pixmap()->size());
    ui->pIconLb->setPixmap(pix);
    ui->pIconLb->repaint();

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "userSelectIcon";
    configSettings.setValue(sKey, pix);
}

IconSelectWidget::IconSelectWidget(QWidget *parent):
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::IconSelectWidget)
{
    ui->setupUi(this);

    TitleBar *pTitleBar = new TitleBar(tr("头像选择"), this);
    pTitleBar->setFixedHeight(40);
    ui->verticalLayout->insertWidget(0, pTitleBar);
    connect (pTitleBar, &TitleBar::tbClose, [this] ()
    {
        close();
    });

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "userUploadIcon";
    QString iconPathName = configSettings.value(sKey, QString()).toString();

    if(iconPathName.isEmpty()) {
        iconPathName = ":/Icon/Settings/92.png";
    }

    ui->pUploadIconLb->setPixmap(QPixmap(iconPathName));


    ui->pIconBtn1->setStyleSheet("background:white; border:0px");
    ui->pIconBtn2->setStyleSheet("background:white; border:0px");
    ui->pIconBtn3->setStyleSheet("background:white; border:0px");
    ui->pIconBtn4->setStyleSheet("background:white; border:0px");

    m_pBtnGrp = new QButtonGroup(this);
    m_pBtnGrp->addButton(ui->pIconBtn1, 0);
    m_pBtnGrp->addButton(ui->pIconBtn2, 1);
    m_pBtnGrp->addButton(ui->pIconBtn3, 2);
    m_pBtnGrp->addButton(ui->pIconBtn4, 3);

    sKey = Arg::username + "userSelectIconBtnIndex";
    int selIconBtnIndex = configSettings.value(sKey, 0).toInt();
    QPushButton *pBtn = dynamic_cast<QPushButton*>(m_pBtnGrp->button(selIconBtnIndex));
    pBtn->click();

    connect(m_pBtnGrp, SIGNAL(buttonClicked(int)), this, SLOT(iconBtnClicked(int)));

    connect(ui->pUploadIconLb, SIGNAL(clicked()), this, SLOT(selectUploadImg()));
}

IconSelectWidget::~IconSelectWidget()
{
//    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
//    configSettings.setValue("userIcon", icon);
}

void IconSelectWidget::iconBtnClicked(int index)
{
    QPushButton *pBtn = dynamic_cast<QPushButton*>(m_pBtnGrp->button(index));
    QIcon icon = pBtn->icon();
    QPixmap pix = icon.pixmap(pBtn->size());

    emit iconSelectComplete(pix);

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "userSelectIconBtnIndex";
    configSettings.setValue(sKey, index);
}

void IconSelectWidget::upLoadIcon()
{

}

void IconSelectWidget::on_pUploadBtn_clicked()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.jpg)"));
    dialog.setViewMode(QFileDialog::Detail);

    if (dialog.exec())
    {
        QString filename = dialog.selectedFiles()[0];
        QImage img (filename);
        if(img.isNull()) {
#ifdef USER_QT_MESSAGEBOX
            QMessageBox::warning(this, tr("Error"), tr("Cannot load image file"));
#else
            MessageDisplayWidget::showMessage(this, tr("Error"), tr("Cannot load image file"));
#endif
            return;
        }

        QPixmap pixmap = QPixmap::fromImage(img);
        ui->pUploadIconLb->setPixmap(pixmap);

        QFileInfo fiInfo(filename);
        QString sImgFileName = fiInfo.fileName();
        QString sNewFile = "./UserConfig/icon/" + sImgFileName;

        bool bCopyStatus = QFile::copy(filename, sNewFile);

        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        QString sKey = Arg::username + "userUploadIcon";

        if(bCopyStatus) {
            configSettings.setValue(sKey, sNewFile);
        }
        else {
            configSettings.setValue(sKey, filename);
        }
    }
}

void IconSelectWidget::selectUploadImg()
{
    const QPixmap *pix = ui->pUploadIconLb->pixmap();
    emit iconSelectComplete(*pix);

//    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
//    QString sKey = Arg::username + "userSelectIconBtnIndex";
//    configSettings.setValue(sKey, -1);
}
