#include "dlgconfig.h"
#include "ui_dlgconfig.h"
#include "flowlayout.h"
#include "appconfig.h"
#include "titlebar.h"
#include "modifypassworddialog.h"
#include "messagedisplaywidget.h"

#include <QStatusBar>
#include <QToolBar>
#include <QStackedWidget>
#include <QStackedLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsDropShadowEffect>
#include <QListWidget>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QRadioButton>
#include <QLineEdit>
#include <QComboBox>
#include <QJsonArray>
#include <QFileDialog>
#include <QMessageBox>
#include <QBuffer>
#include <QDialogButtonBox>
#include <QJsonDocument>

QString imgUpload_base64 {
    "iVBORw0KGgoAAAANSUhEUgAAAEsAAABLCAYAAAA4TnrqAAAABmJLR0QA/wD/AP+gvaeTAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH4AwFDCYBlX3mxwAAD1pJREFUeNrVXDtsG0ca/mdmd8nlQ1yKT4uiLUtCCl+q+AL4ugBJkSbp0iQp0iVIuhg4N04VNzYQdwmS7gpfmnS5xsUZcHcu7FS54C4IIMvm2YxpkdwVn7s7M1eIsxgOl+SuRCsJAUEURe7OfP/7+/8hgt/gwTnf9X3/4ng8fnk8Hr/k+/5Z13WrlNI8YywNAAQAKEKor+t6B2Pc1DTtUSKR+DmZTP6o6/oDjPEvp71udBo3oZRmKKVvD4fDNx3HeW0wGNQppYAQAs45cM6PFoOmlyP+ZowBQggQQsHzVCr1OJPJ3E2lUrdN0/weIdT7Q4NFKX3Ltu33Dw8P3xkMBogxBhjjKYAwxkLbZM2bAU5+TfwWnyGEcMuyvrMs61YikfjHHwqs8Xj8ycHBwceO41xgjAUblAFRX5PBUd8XeTMIgWmaP62vr3+1trb25e8aLNd1P3r+/PmVbre7JTYtNCnEb81ozzwAloEn7iFMFADANM2HhULh+tra2te/K7AYY68fHBxca7Val2SAomx0BcFirmYihCCbzd6rVCpXDcO485uD1e/3v3j69Omn4/F4oZ95YX5EEYh6T845EEKgUCjcLJVKl38TsDjnr3Q6nW+azeaf1YXKi3/RYMmAyUDJAUQElnQ6fX9jY+NDTdN+ODWwfN9/98mTJ387PDzUVUDk8H6qOdAEMDnNUIWHEAJN07x6vf6BaZrfvnCwXNf96/7+/nXXdadyHzkFOE3AhI9UNVt+jjGeApMQAtVq9YplWTfi3AvHefNoNPp8b2/vuu/7M8mkeK4uNO7GFyWpiyKh+AxjbApAYYbidUII+L4PzWbzeqfT+fyFgDUajT7f39+/Okk2QzUpTMpRzGfZa/NAn5e7ifWo2b8sSMYYPHny5Gq32/18pWYoTE9oVBSzWFXKEAbIovJIdfSq4NT/IYRgY2PjSi6Xu3FisHzff3d/f//v4/F4Cgy1dBGvySWJWFiYJkZNMZaZZFh9qTr1RYITa9/a2npvmdNfCBbl7JXmk6f3bNvWwyQdtknZBFQA1c0RQgJTka8XljupZiYLaVFgUUGTnb68D03TvO3t7Uu6rs9NK7RFYNmd7jcCqDA1lhcsO2f1vRhjUFkGSin0ej0YDAbg+z54ngee54FcS4rPapoGiUQCTNOEZDIJuq5PXVeALmtwWEqjVB1qyqE3Go1vAODV2Jo17A++2Nt/9CkAm4k+qlmFMQHqwgghwBiDfr8Ptm1DrxefURHgG4YBuVwOcrkc6LoeRDtN04BSCvOEK2v6vAS6UCjcLJfLlyODxSl7fe/x/j/d4QAomy5U5ZAstEXdTJipDgYDaLVaMBqNpgCM6lvk+4gNT8oYyOfzC/2Z/FvsQTZXWZgIIdjZ2XkjkUjciQRWt23/q9FsXMKAIke5sIWJ38+ePYN2uz2T4Yf5DhmYRXWfXM6k02k4c+bMFD8mAIkSSFT/l06n7507d+4vS/Ms3/c/evaseQkDAq7BTASUQVBvJp7L0ms0GtBut4NrCDORs2rVeYdpaphwBPCO40Cj0Zhag7jPshxOXEf+cRznkm3bHy3VrGazudfpdLaAU+ALctZ5+Y9wvIwxePr0Kdi2PZNizCu6oyavYRGOMQapVArq9XqsMissxZn4xYc7Ozvn52oWY+yT5117CyECgJcnn/NKD9/3wbZtcBxnitsSmiWnCnGcu3DkqkYLjR8Oh3BwcBCqpcv2IfyWVLFsOY7zyVyw2getjzEwoJ4LCPTYxayQDGMMms3mlN8QG9M0LTT/WkAsBpvHGAeSV4OE+H+n0wHXdadcQxyhyz613W5/HAoWY+ytrm1fQJQD0TWgfnRVxhiD7/ugaUdO7vnz50AIAc/zpqQlciHZB0a5tqpZKlelaRowxsD3fUAIQafTiay5apSV3UWv17swGo3emgELIfS+61NgwIExAA5jQJhGtnmhWb7vg+M4QCkN8h41jZB/VtRFAkIIEEKAUgrdbncqzYlqGWo0xhhDu91+fwosynim3++/A4wCIhOSDBD4kIycKIob9Hq9GXDEouUcTM3RojrjRb1FWZP6/X4sn6gKUazv8PDwHUppJgALA3278eR/6MheARjzgRMDNDaOxVBOVHfK7mX6RvZBqnmeoFky008UYMUBSk6LlKCChsPh2wFYw5H7JnN9AG4ABgSAJxuMQHfJNk4phX6/P8NIzNOuVdA48xiMfr8f2QwXXZdzDt1u980ALNNMvIYIBgrukdSBA0J8afoQljLITnle1Fl1e0xlLcRaPM9byfVHo9FrAACYAewC4DrnHDAgQByA8Ym0WPQmKOccxuNxYJIvApQoEVP2l0J4J32Mx+O653m7GCF0cRkXHrXJqUryNNtgqkaLdGZVj9FodFEDgJej8OJRc615bajTAG1eWbSK7tFoNHoZc85fWsUiRcQ7bROcx66uqhUnrjkej1/CAHB2FRcLM9vTACzs/urvk/pDxhh4nncWI4SqUQvlKFompwinaX5qxn6StGEOFVTFAJCP2s9bBqooak9jIGSRrxLPdV1fmZlTSvOYc54+iWbJEtR1faUmEEdYwlxkfsswjJVcf1KepTFCiByxgEcZO0MTBhMxwEAiL1YtcU57MEREYk3TAgZkHlt6TM0lmAGnHHxgoAHmDBD4wOmEFgYaGXlB7wr/sYqFxjETTdOCRFTXdaCUQqPRWKW5U4w47nMggDkDQAwQJI7KHEYAAYvsADHGMBgMpv4+LZ8lQJKbtowxcF13ZfchhPQx4rSDJsUzcAwcPKCUw9Gz6GYoWNCwieTTeKjBZdUCQwh1sN1tN//7n5+O2ukIAecINEyAcgANopN/AADpdHoq6z0tvyW7Arn/twoHLwWyJsZa8hHnE8ILMWCEAiAfACEAxCItVGiSaZqBo1fJuBcJlNrPFELK5XIru4dhGI9wwjB+RogD5xQ4R0C4DoB0QIwDXzwKMUXPIIQgl8sFTYUoprgKzZNpbbkRgjGGbDa7MqEkk8mfsWFoPwqGGXEAoJNchQAgGp11wBgDISSQpmgkhAGzao2T2VgBnmVZK2FjhcANw/gRA8CDZDJ1dEM4IvyCrgtGsRw8YwyKxeJU80AuR46b+C7TQPWoiq7rUCwWI3WPolgOIQQMw3iAEUK/pNPpx+rgRJyNyBFoMtwKABBoVlirPg5Yy4bR1DSmXC4fO8CE0dS6rj9OJBK/4EkUuyvsXqhu3M6I2kbP5/MB+RY28LHK0C6ztfl8HjKZzLG5tDDwk8nk3SNHdeS8boscKc6ksJo6yI62UCgEqcS86b+oZhI2+6XOO4hpmmKxCJxz8Dwv9sxDmFZxziGbzd4OwMIYf59Op7nsrOOaSNg0YK1Wg2QyOQXkSZgNFTSZ9EskEnDmzJkg14qjtYvmKDDGfG1t7fsALIRQz7Ks747DGMgTKGqTkhAC9XodMplMKIsat72uaq/Y0NraGtRqtanAEseBy/5NbbZms9nvxMHPAP5UKnXLMIyZefaomxHvl5uosoYVi8WpRckDuFFCt5omCNMuFApQq9Wm7i/8YRRhhLkeWbD5fP5WcG/5g61W698HBwcLD1Qu8ieqeagM5ng8hlarBb1eLxgciSKUsBwtm81CqVQKhnHl+apVEJCTFOSn3d3dPwVaKL9hbW3tK5V5XLYZ2daFSqu+S/zWNA02Nzdha2srGD2KU/dhjMGyLNje3oZarRbQMoJ1EGuWJ5jjuBJVe4vF4ldTJiv/kUgkvsxmsw/lNCCsWyM/Vw8RqeefxbUEIccYA9M0Y3WLxXUMw4BqtTrFboj/CZZWnVqOCpTcnZoEjIeWZX05FywAgHK5fF04yXkz5WEdYNn01PaYqmFiJChOKUMIgeFwCIPBIBCQ7LvkhmrYkEeU2lIk1ZN87fqM0NQXNE37ulwu3wuLVmrpEvfUlthEp9OJlTTKHXLbtmdoINllqAMoUbkwWRNTqdS9QqHw9VKwJhHgaiKRmDs9HKfdpW6q1+vBeDyONWwmb95xHHBdN+Ct5HXJApEL67BcTX4IHyfWU61Wr4YKfs7C7tRqtZvyOZyT8ONydOp0OjOzWlG0U96k4zihn1VdQdhXIcz7nDDp9fX1m6Zp3okMFgCAaZqXi8XiffVbO+SpvShphSx9z/Og3+8fK6TL17FtO1RrltV4YQSh/FoqlbpfqVTmHjpfaAfr6+sfZjIZTySQ8sRx1GgjF822bR+r/lQ3KUBXyyj5urKrCDs7rboUjLG3sbHx4UINX6L+P2xsbHyQTCanyhnRbopTogiNkEuJ45RVQstE203V3jAiUPaN6mkRIfR6vf6BYRg/HBusSXT8tl6vXxHgiJup9Is6waKGZtd1g9xKru+Ow1qKg1NiEjrsQGhY6iJn+iJXI4RAuVy+kslklp7Gj7RawzBunD179ppaAqkFaFjIFlGv2+0urPniNihc14XhcBjK2IalLjJNJLuRYrF4rVgsRjqFH1m0pml+du7cuWtyiJVp43nRRoArTw8fd0JHTYwdx5kxeTWRlgtrAZBUhF8rlUqfRa4k4iw4lUp9dv78+Svi5mHnCsOctmyCUQv0ZXw/xjiYdVenDNVGq8qkIISgUqlcqVQqn8W5f2xe1zCMG7u7u++ZpunJPkPlq2RNGw6HS8P2cWhkuUU/j00VkVxy6t7m5uZ7hULhRtz7HosE1zTt21qtdqlUKt2XO8Bhs1KEkGAGQs6u45QjYbNX8rz7MuaVUgqUUkilUvd3dnYuZbPZb4+z72N3DHRd/6Farb5arVZvitJIDd/ib6FZQrpqm+wklcFgMAiNwPL9McZQKpVunj9//tVl6cELAUs8LMu6vLW19UYul7unHvcFgNBT9XGHR8JGAeTZ+zDtE/4pk8nc297efmPeIfFYFrWKVpSmaXcA4M5gMPjo4ODgiuM4W8JXuK4bcExqoasGibCid940sniv7/tAKQ1O4Yv3aZr2sFKpXM/lcr+vb2ZTH/1+/5N2u/3x4eHhhU6nA7/++uuM1FcxkiSA2dzcDJoipmn+ZFnWV/l8/vf9nX/qw/O8txqNxvt7e3vvOI6Doha/qv9bdB57Ahbf3Nz8zrKsW6lU6o/1bZIhIGS63e7brVbrzVar9Vq73a7LpzHkE/lhQMqgCd+Xy+Uel0qlu+Vy+XalUvnjf0/pvIfv+7uO41x0HOdlx3FeGo1GZ4fDYdXzvDylNM0YI4QQqmlaX9O0jq7rzWQy+SidTv9sWdaPlmU9MAzj1L8B9/8O4ZKQaH9lUAAAAABJRU5ErkJggg=="
};

inline void resetLayout(QLayout* src)
{
    src->setContentsMargins(0, 0, 0, 0);
    src->setSpacing(0);
}

inline void loadImage(QPushButton* btn, const QString& base64)
{
    /*
    QGraphicsScene* scene = new QGraphicsScene;
    qgv->setScene(scene);
    QGraphicsPixmapItem* pitem = new QGraphicsPixmapItem(QPixmap(src));
    pitem->setPos(0, 0);
    scene->addItem(pitem);
    */

    QImage x = QImage::fromData(QByteArray::fromBase64(base64.toLatin1()));
    QPixmap y = QPixmap::fromImage(x);
    btn->setIcon(QIcon(y));
    btn->setIconSize(QSize{75, 75});
}

void dlgConfig::addTag(const QString &tagname)
{
    QWidget* iwrapper = new QWidget;
    QHBoxLayout* ilayout = new QHBoxLayout;
    QPushButton* itagname = new QPushButton;
    QToolButton* itagdel = new QToolButton;

    itagname->setText(tagname);
    itagname->setStyleSheet("tagbody");
    itagdel->setStyleSheet("tagdel");

    ilayout->addWidget(itagname);
    ilayout->addWidget(itagdel);
    resetLayout(ilayout);

    iwrapper->setLayout(ilayout);

    ui->frmTags->layout()->addWidget(iwrapper);
}

void dlgConfig::dbgActAddTag(const QString &src)
{
    addTag(src);
}
/*
void loadData()
{iwrapper

}
*/
void dlgConfig::refreshConfig()
{
#ifndef hlp_set
#define hlp_set
#define hlp_set_bool(a, b) glbAppConfig[a].toObject()[b].toBool()
#define hlp_set_str(a, b) glbAppConfig[a].toObject()[b].toString()
#define hlp_set_int(a, b) glbAppConfig[a].toObject()[b].toInt()
#define hlp_contain_str(a, b, c) glbAppConfig[a].toObject()[b].toArray().contains(c)
#endif
    ui->rdo_start_on_boot_yes->setChecked(hlp_set_bool("basic", "start_on_boot"));
    ui->rdo_start_on_boot_no->setChecked(!hlp_set_bool("basic", "start_on_boot"));

    ui->rdoStandard->setChecked(hlp_set_str("basic", "initial_mode") == "standard");
    ui->rdoToday->setChecked(hlp_set_str("basic", "initial_mode") == "single_day");
    ui->rdoClass->setChecked(hlp_set_str("basic", "initial_mode") == "in_class");

    ui->rdoInternet->setChecked(hlp_set_str("basic", "link_mode") == "internet");
    ui->rdoLocal->setChecked(hlp_set_str("basic", "link_mode") == "local");
    ui->rdoOffline->setChecked(hlp_set_str("basic", "link_mode") == "offline");

    ui->txtServerAddress->setText(hlp_set_str("basic", "server_addr"));
    ui->txtInterface->setText(hlp_set_str("basic", "server_interface"));

    ui->txtLocalDir->setText(hlp_set_str("basic", "local_storage"));
    // glbAppConfig["basic"].toObject()["start_on_boot"].toBool()
/*
    ui->lblSel1->setText(hlp_set_int("user", "avatar") == 1 ? "Selected" : "");
    ui->lblSel2->setText(hlp_set_int("user", "avatar") == 2 ? "Selected" : "");
    ui->lblSel3->setText(hlp_set_int("user", "avatar") == 3 ? "Selected" : "");
    ui->lblSel4->setText(hlp_set_int("user", "avatar") == 4 ? "Selected" : "");

    if(!glbAppConfig["user"].toObject()["custom_avatar"].isNull())
    {
        QImage x = QImage::fromData(QByteArray::fromBase64(hlp_set_str("user", "custom_avatar").toLatin1()));
        QPixmap y = QPixmap::fromImage(x);
        ui->btnSelect5->setIcon(QIcon(y));
        ui->btnSelect5->setIconSize(QSize{75, 75});
    }
*/
    loadImage(ui->btnSelect1, hlp_set_str("user", "avatar1"));
    loadImage(ui->btnSelect2, hlp_set_str("user", "avatar2"));
    loadImage(ui->btnSelect3, hlp_set_str("user", "avatar3"));
    loadImage(ui->btnSelect4, hlp_set_str("user", "avatar4"));
    loadImage(ui->btnSelect5, imgUpload_base64);

    ui->cmbProvince->addItem(hlp_set_str("user", "province"));
    ui->cmbCity->addItem(hlp_set_str("user", "city"));
    ui->cmbDistrict->addItem(hlp_set_str("user", "district"));
    ui->cmbSchl->addItem(hlp_set_str("user", "school"));
    ui->cmbSubject->addItem(hlp_set_str("user", "subject"));
    ui->cmbClass->addItem(hlp_set_str("user", "class"));

    ui->rdo_sync_yes->setChecked(hlp_set_bool("schedule", "sync_schl_sche"));
    ui->rdo_sync_no->setChecked(!hlp_set_bool("schedule", "sync_schl_sche"));
    ui->rdo_override_yes->setChecked(hlp_set_bool("schedule", "override_local"));
    ui->rdo_override_no->setChecked(!hlp_set_bool("schedule", "override_local"));
    ui->chkLocation->setChecked(hlp_contain_str("schedule", "default_cell", "location"));
    ui->chkSubject->setChecked(hlp_contain_str("schedule", "default_cell", "subject"));
    ui->chkClass->setChecked(hlp_contain_str("schedule", "default_cell", "class"));

    ui->chkAfterClass->setChecked(hlp_contain_str("schedule", "default_resource", "bfcls"));
    ui->chkBeforeClass->setChecked(hlp_contain_str("schedule", "default_resource", "incls"));
    ui->chkInClass->setChecked(hlp_contain_str("schedule", "default_resource", "aftcls"));
    ui->chkRawMaterial->setChecked(hlp_contain_str("schedule", "default_resource", "raw"));

    ui->rdo_read_yes->setChecked(hlp_set_bool("schedule", "read_schl_sche"));
    ui->rdo_read_no->setChecked(!hlp_set_bool("schedule", "read_schl_sche"));

    for (const auto& item: glbAppConfig["schedule"].toObject()["tags"].toArray())
    {
        addTag(item.toString());
    }
}

inline void exchangeImage(QPushButton* a, QPushButton* b) {
    QIcon tmp = a->icon();
    a->setIcon(b->icon());
    b->setIcon(tmp);
}

void dlgConfig::shiftImage() {
    ui->btnSelect4->setIcon(ui->btnSelect3->icon());
    ui->btnSelect3->setIcon(ui->btnSelect2->icon());
    ui->btnSelect2->setIcon(ui->btnSelect1->icon());
}

void dlgConfig::uploadImage() {
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setNameFilter(tr("Images (*.png *.jpg *.gif)"));
    dialog.setViewMode(QFileDialog::Detail);
    if (dialog.exec())
    {
        QString filename = dialog.selectedFiles()[0];
        QImage img (filename);
        if(img.isNull()) {
#ifdef USER_QT_MESSAGEBOX
            QMessageBox::warning(this, tr("教师客户端"), tr("加载文件失败！"));
#else
            MessageDisplayWidget::showMessage(tr("教师客户端"), tr("加载文件失败！"));
#endif
            return;
        }
        QPixmap pixmap = QPixmap::fromImage(img);

        shiftImage();

        ui->btnSelect1->setIcon(QIcon(pixmap));
        ui->btnSelect1->setIconSize(QSize{75, 75});
    }
}

dlgConfig::dlgConfig(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::dlgConfig)
{
    // Settings
    glbAppConfig = AppConfig::defaultConfig();

    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    int nShadowWidth = getShadowMargin();
    ui->mainLayout->setContentsMargins(nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);

    //添加标题栏.
    auto titleBar = new TitleBar (tr("设置"), this);
    connect (titleBar, &TitleBar::tbClose, [this] ()
    {
        this->close ();
    });

    ui->mainLayout->insertWidget(0, titleBar);

//    // Resetting Explicit Layout
//    resetLayout(ui->horizontalLayout);
////    resetLayout(ui->mainLayout);
//    resetLayout(ui->horizontalLayout_3);
//    resetLayout(ui->horizontalLayout_4);
//    resetLayout(ui->horizontalLayout_5);
//    resetLayout(ui->horizontalLayout_6);
//    resetLayout(ui->horizontalLayout_7);
//    resetLayout(ui->horizontalLayout_8);
//    resetLayout(ui->horizontalLayout_9);
//    resetLayout(ui->horizontalLayout_10);
//    resetLayout(ui->horizontalLayout_11);
//    resetLayout(ui->horizontalLayout_12);
//    resetLayout(ui->horizontalLayout_13);
//    resetLayout(ui->horizontalLayout_14);
//    resetLayout(ui->horizontalLayout_15);
//    resetLayout(ui->horizontalLayout_16);
//    resetLayout(ui->horizontalLayout_17);
//    resetLayout(ui->horizontalLayout_18);
//    resetLayout(ui->horizontalLayout_19);

//    resetLayout(ui->verticalLayout);
//    resetLayout(ui->verticalLayout_2);
//    resetLayout(ui->verticalLayout_3);
//    resetLayout(ui->verticalLayout_4);
//    resetLayout(ui->verticalLayout_5);
//    resetLayout(ui->verticalLayout_6);
//    resetLayout(ui->verticalLayout_7);
//    resetLayout(ui->verticalLayout_8);
//    resetLayout(ui->verticalLayout_9);
//    resetLayout(ui->verticalLayout_10);
//    resetLayout(ui->verticalLayout_11);
//    resetLayout(ui->verticalLayout_12);
//    resetLayout(ui->verticalLayout_13);
//    resetLayout(ui->verticalLayout_14);

//    resetLayout(ui->gridLayout_2);

    ui->lstNav->item(0)->setTextAlignment(Qt::AlignCenter);
    ui->lstNav->item(1)->setTextAlignment(Qt::AlignCenter);
    ui->lstNav->item(2)->setTextAlignment(Qt::AlignCenter);
    ui->lstNav->item(3)->setTextAlignment(Qt::AlignCenter);


    /*
    ui->label_5->setAlignment(Qt::AlignCenter);
    ui->label_6->setAlignment(Qt::AlignCenter);
    ui->label_8->setAlignment(Qt::AlignCenter);
    ui->label_9->setAlignment(Qt::AlignCenter);
    */

    ui->lblSel1->setAlignment(Qt::AlignCenter);
    ui->lblSel2->setAlignment(Qt::AlignCenter);
    ui->lblSel3->setAlignment(Qt::AlignCenter);
    ui->lblSel4->setAlignment(Qt::AlignCenter);


    FlowLayout* flowlayout = new FlowLayout(-1, -1, -1);
    resetLayout(flowlayout);

//    resetLayout(ui->buttonBox->layout());
//    resetLayout(ui->buttonBox_2->layout());
//    resetLayout(ui->buttonBox_3->layout());

    ui->frmTags->setLayout(flowlayout);

//    setFixedSize(600, 440);

    //dbgReload* dbgwin = new dbgReload(this);
    //dbgwin->show();

    //connect(dbgwin, &dbgReload::sigAddTag, this, &MainWindow::dbgActAddTag);

    connect(ui->lstNav, &QListWidget::currentRowChanged, ui->stkBody, &QStackedWidget::setCurrentIndex);
    connect(ui->btnBrowse, &QPushButton::clicked, this, &dlgConfig::uploadImage);
    connect(ui->btnSelect5, &QPushButton::clicked, this, &dlgConfig::uploadImage);
    connect(ui->btnSelect2, &QPushButton::clicked, [this](){
        exchangeImage(ui->btnSelect1, ui->btnSelect2);
    });
    connect(ui->btnSelect3, &QPushButton::clicked, [this](){
        exchangeImage(ui->btnSelect1, ui->btnSelect3);
    });
    connect(ui->btnSelect4, &QPushButton::clicked, [this](){
        exchangeImage(ui->btnSelect1, ui->btnSelect4);
    });

//    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &dlgConfig::saveData);
//    connect(ui->buttonBox_2, &QDialogButtonBox::accepted, this, &dlgConfig::saveData);
//    connect(ui->buttonBox_3, &QDialogButtonBox::accepted, this, &dlgConfig::saveData);

    ui->lstNav->setCurrentRow(0);

    refreshConfig();"彩蛋";

    ///Add by BiXiaoxia 2016.12.23,样式文件放进这里加载,一次创建对象一次加载.
    QFile stylesheet{":/config.qss"};
    stylesheet.open(QIODevice::ReadOnly | QIODevice::Text);
    QString str = QString(stylesheet.readAll());
    setStyleSheet(str);
}

inline QString loadImageData(QPushButton* src)
{
    QPixmap pm = src->icon().pixmap(75, 75);
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    pm.save(&buffer, "PNG");
    return QString::fromLatin1(bytes.toBase64());
}

void dlgConfig::saveData()
{
#ifndef hlp_set
#define hlp_set
#define hlp_set_bool(a, b) glbAppConfig[a].toObject()[b].toBool()
#define hlp_set_str(a, b) glbAppConfig[a].toObject()[b].toString()
#define hlp_set_int(a, b) glbAppConfig[a].toObject()[b].toInt()
#define hlp_contain_str(a, b, c) glbAppConfig[a].toObject()[b].toArray().contains(c)
#endif
    QJsonObject basic;
    basic["start_on_boot"] = ui->rdo_start_on_boot_yes->isChecked();
    basic["initial_mode"] =
            ui->rdoStandard->isChecked() ? "standard" :
            (ui->rdoToday->isChecked() ? "single_day" : "in_class");
    basic["link_mode"] =
            ui->rdoInternet->isChecked() ? "internet" :
            (ui->rdoLocal->isChecked() ? "local" : "offline");
    basic["server_addr"] = ui->txtServerAddress->text();
    basic["server_interface"] = ui->txtInterface->text();
    basic["local_storage"] = ui->txtLocalDir->text();

    QJsonObject user;
    user["avatar1"] = loadImageData(ui->btnSelect1);
    user["avatar2"] = loadImageData(ui->btnSelect2);
    user["avatar3"] = loadImageData(ui->btnSelect3);
    user["avatar4"] = loadImageData(ui->btnSelect4);

    QJsonObject schedule;
    schedule["sync_schl_sche"] = ui->rdo_sync_yes->isChecked();
    schedule["override_local"] = ui->rdo_override_yes->isChecked();
    QJsonArray defcell;
    if(ui->chkLocation->isChecked()) defcell.append("location");
    if(ui->chkSubject->isChecked()) defcell.append("subject");
    if(ui->chkClass->isChecked()) defcell.append("class");
    QJsonArray defres;
    if(ui->chkBeforeClass->isChecked()) defres.append("bfcls");
    if(ui->chkInClass->isChecked()) defres.append("incls");
    if(ui->chkAfterClass->isChecked()) defres.append("aftcls");
    if(ui->chkRawMaterial->isChecked()) defres.append("raw");
    schedule["default_cell"] = defcell;
    schedule["default_resource"] = defres;
    schedule["read_schl_sche"] = ui->rdo_read_yes->isChecked();
    QJsonArray tags;
    bool first = true;
    for( const auto& kk: ui->frmTags->children())
    {
        if(first) {
            first = false;
            continue;
        }
        tags.append(((QPushButton*)(((QWidget*)kk)->children()[1]))->text());
        //tags.append(((QPushButton*)(((QFrame*)kk)->layout()->children()[0]))->text());
    }
    schedule["tags"] = tags;

    QJsonObject newconf;
    newconf["basic"] = basic;
    newconf["user"] = user;
    newconf["schedule"] = schedule;

    // Round
    newconf["user"].toObject()["province"] = glbAppConfig["user"].toObject()["province"];
    newconf["user"].toObject()["city"] = glbAppConfig["user"].toObject()["city"];
    newconf["user"].toObject()["district"] = glbAppConfig["user"].toObject()["district"];
    newconf["user"].toObject()["school"] = glbAppConfig["user"].toObject()["school"];
    newconf["user"].toObject()["class"] = glbAppConfig["user"].toObject()["class"];
    newconf["user"].toObject()["subject"] = glbAppConfig["user"].toObject()["subject"];

    QList<QPair<QString, QString>> stat_new_conf;

    for(const auto& cat: newconf.keys())
    {
        for(const auto& item: newconf[cat].toObject().keys())
        {
            if( newconf[cat].toObject()[item] != glbAppConfig[cat].toObject()[item] )
            {
                stat_new_conf.push_back({cat, item});
            }
        }
    }

    glbAppConfig = newconf;

    for(const auto& new_item: stat_new_conf)
    {
        emit settingChanged(new_item.first, new_item.second);
    }
}

dlgConfig::~dlgConfig()
{
    delete ui;
}

void dlgConfig::on_pModifiedPasswordBtn_clicked()
{
    ModifyPasswordDialog *pModifyPasswordDlg = new ModifyPasswordDialog(this);
    pModifyPasswordDlg->show();
}

void dlgConfig::on_pSaveDataBtn_clicked()
{
    saveData();
}
