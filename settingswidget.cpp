//#ifdef Q_OS_WIN
//#define _WIN32_WINNT 0x0501    //这个必须放在所有include的前面??.
//#endif

#include "settingswidget.h"
#include "ui_settingswidget.h"
#include "titlebar.h"
#include "config.h"
#include "handler.h"
#include "userdb.h"
#include "dataclassinstancemanage.h"
#include "messagedisplaywidget.h"
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

//#ifdef Q_OS_WIN
//#include <windows.h>  //这个要必须放在所有inclde最后面???.
//#endif

SettingsWidget *SettingsWidget::m_pInstance = Q_NULLPTR;

SettingsWidget *SettingsWidget::getInstance()
{
    if(m_pInstance == Q_NULLPTR) {
        m_pInstance = new SettingsWidget;
    }

    return m_pInstance;
}

void SettingsWidget::destroyInstance()
{
    if(m_pInstance != Q_NULLPTR) {
        delete m_pInstance;
        m_pInstance = Q_NULLPTR;
    }
}

SettingsWidget::SettingsWidget() :
    FramelessModalMovableShadowWidget(),
    ui(new Ui::SettingsWidget)
{
    ui->setupUi(this);
    m_pTestNetWorkMgr = Q_NULLPTR;
//    m_pUserdb = Q_NULLPTR;
    m_bServerTestPassed = false;

    setWindowTitle(tr("教师客户端"));

    TitleBar *pTitleBar = new TitleBar(tr("设置"), this);
    pTitleBar->setFixedHeight(40);
    ui->verticalLayout_2->insertWidget(0, pTitleBar);
    connect (pTitleBar, &TitleBar::tbClose, [this] ()
    {
        close();
    });

    QButtonGroup *pAutoStartBtnGrp = new QButtonGroup(this);
    pAutoStartBtnGrp->addButton(ui->pAutoStartRadioBtn);
    pAutoStartBtnGrp->addButton(ui->pDisableStartRadioBtn);
    pAutoStartBtnGrp->setExclusive(true);

    QButtonGroup *pModeBtnGrp = new QButtonGroup(this);
    pModeBtnGrp->addButton(ui->pStandartModeBtn);
    pModeBtnGrp->addButton(ui->pSigleDayModeBtn);
    pModeBtnGrp->addButton(ui->pInClassModeBtn);
    pModeBtnGrp->setExclusive(true);

    QRegExp regExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator *pValidator = new QRegExpValidator(regExp, this);
    ui->pIPAddressLineEdit->setValidator(pValidator);
//    ui->pIPAddressLineEdit->setInputMask("000.000.000.000;0");
    ui->pIPAddressLineEdit->setCursorPosition(0);
//    ui->pIPAddressLineEdit->setText("101.200.176.087");
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::load()
{
    ///Mark，由于原来设计限制，这些配置没做管理，重构需要设计管理配置类.
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);

    //read auto start.
    QString sKey = Arg::username + "autostart";
    bool bAutoStart = configSettings.value(sKey, false).toBool();
    ui->pAutoStartRadioBtn->setChecked(bAutoStart);
    ui->pDisableStartRadioBtn->setChecked(!bAutoStart);

    //read View Mode.
    sKey = Arg::username + "displayMode";
    ViewMode mode = static_cast<ViewMode>(configSettings.value(sKey, static_cast<quint8>(ViewMode::Standard)).toInt());
    if(mode == ViewMode::Standard) {
        ui->pStandartModeBtn->setChecked(true);
    }
    else if(mode == ViewMode::Day) {
        ui->pSigleDayModeBtn->setChecked(true);
    }
    else {
        ui->pInClassModeBtn->setChecked(true);
    }

    //read server config.
    QString serverIp = configSettings.value("ServerIP", QString()).toString();
    ui->pIPAddressLineEdit->setText(serverIp);
    int port = configSettings.value("ServerPort", 8080).toInt();
    ui->pPortSpinBox->setValue(port);

    ///TODO,读取保存路径.
    sKey = Arg::username + "savefilepath";
    QString sPath = configSettings.value(sKey, QString()).toString();

    if(sPath.isEmpty()) {
        sPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + "XDcalendar";
        QDir configDir = QDir(sPath);

        if (!configDir.exists ()) {
            configDir.mkpath(configDir.absolutePath());
        }

    }

    ui->pSaveFilePathLineEdit->setText(sPath);

    //如果没有用户登录，则禁用密码修改.
    ui->pPasswordLineEdit->clear();
    bool bEnableModified = Arg::token.isEmpty() ? false : true;
    ui->pModifiedGrpBox->setEnabled(bEnableModified);
}

void SettingsWidget::on_pOkBtn_clicked()
{
    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);

    //随机启动.
    QString sAppPath = QApplication::applicationFilePath().replace("/", "\\");

#ifdef Q_OS_WIN
    QString sKey = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings reg(sKey, QSettings::NativeFormat);

    //开机自动运行
    if(ui->pAutoStartRadioBtn->isChecked())
    {
        reg.setValue("MoonCalendar", sAppPath);
    }
    else
    {
        reg.remove("MoonCalendar");
    }
#endif
    sKey = Arg::username + "autostart";
    configSettings.setValue(sKey, ui->pAutoStartRadioBtn->isChecked());

    ViewMode mode;

    //显示模式.
    if(ui->pStandartModeBtn->isChecked()) {
        mode = ViewMode::Standard;
    }
    else if(ui->pSigleDayModeBtn->isChecked()) {
        mode = ViewMode::Day;
    }
    else {
        mode = ViewMode::InClass;
    }

    sKey = Arg::username + "displayMode";
    configSettings.setValue(sKey, static_cast<quint8>(mode));

    QString sCurIPServer = ui->pIPAddressLineEdit->text().trimmed();
    QString serverIp = configSettings.value("ServerIP", QString()).toString();
    int nServerPort = configSettings.value("ServerPort", -1).toInt();
    int nCurServerPort = ui->pPortSpinBox->value();

    if(!sCurIPServer.isEmpty() && nCurServerPort >= 0) {
        if((sCurIPServer != serverIp) || (nCurServerPort != nServerPort)) {
            if(m_bServerTestPassed) {
                Arg *pArg = Arg::getInstance();
                pArg->setServerIpAndAddress(ui->pIPAddressLineEdit->text(), ui->pPortSpinBox->value());

                ///服务器设置没必要用用户名去匹配.
                configSettings.setValue("ServerIP", sCurIPServer);
                configSettings.setValue("ServerPort", ui->pPortSpinBox->value());

                m_bServerTestPassed = false;
            }
            else {
#ifdef USER_QT_MESSAGEBOX
                QMessageBox::information(this, tr("教师客户端"), tr("服务器配置未测试通过!"));
#else
                MessageDisplayWidget::showMessage(tr("教师客户端"), tr("服务器配置未测试通过!")) ;
#endif
            }
        }
    }
    else {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("服务器地址不能为空！"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("服务器地址不能为空！"));
#endif

        return;
    }

    ///TODO.下面保存路径.
    sKey = Arg::username + "savefilepath";
    QString sSavePath = ui->pSaveFilePathLineEdit->text();
    configSettings.setValue(sKey, sSavePath);
    Arg *pArg = Arg::getInstance();
    pArg->configDir = QDir(sSavePath);

    QString sPassword = ui->pPasswordLineEdit->text().trimmed();
    if(!sPassword.isEmpty()) {
        UserDB *pUserdb = DataClassInstanceManage::getInstance()->getUserDBPtr();
        pUserdb->modifiedPasswordByUserName(Arg::username, sPassword);
    }

    close();
}

void SettingsWidget::on_pTestNetWorkBtn_clicked()
{
    if(m_pTestNetWorkMgr == Q_NULLPTR) {
        m_pTestNetWorkMgr = new QNetworkAccessManager;
        connect(m_pTestNetWorkMgr, &QNetworkAccessManager::finished, this, &SettingsWidget::testReplyArrived);
    }

    QString sHostName = ui->pIPAddressLineEdit->text();
    m_pTestNetWorkMgr->connectToHost(sHostName, ui->pPortSpinBox->value());
}

void SettingsWidget::testReplyArrived(QNetworkReply *reply)
{
    if(reply->error() == QNetworkReply::NoError) {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("测试通过，服务器设置可用"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("测试通过，服务器设置可用"));
#endif
        m_bServerTestPassed = true;
//        Arg *pArg = Arg::getInstance();
//        pArg->setServerIpAndAddress(ui->pIPAddressLineEdit->text(), ui->pPortSpinBox->value());

        return;
    }
#ifdef USER_QT_MESSAGEBOX
    QMessageBox::information(this, tr("教师客户端"), tr("测试未通过，服务器设置不可用"));
#else
    MessageDisplayWidget::showMessage(tr("教师客户端"), tr("测试未通过，服务器设置不可用"));
#endif
    m_bServerTestPassed = false;
}

void SettingsWidget::on_pChangeSavePathBtn_clicked()
{
    QString sGetFilePath = QFileDialog::getExistingDirectory(this, tr("路径选择"), "C:\\Users\\Administrator\\AppData\\Local\\XDcalendar");
    sGetFilePath.replace("/", "\\");

    ui->pSaveFilePathLineEdit->setText(sGetFilePath);
}
