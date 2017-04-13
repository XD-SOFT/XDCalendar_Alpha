#include "logindialog.h"
#include "dialog.h"
#include "titlebar.h"
//#include "ui_logindialog.h"
#include "handler.h"
#include "userdb.h"
#include "resfilesdb.h"
#include "config.h"
#include "ccu.h"
#include "masklabel.h"
#include "accountitem.h"
#include "loginstatuswidget.h"
#include "messagedisplaywidget.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QListWidget>
#include "loginthread.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QVector>
#include <QComboBox>
#include <QJsonObject>
#include <QTimerEvent>
#include <QEvent>
#include <QMovie>
#include <QPainter>
#include <QJsonArray>
#include <QSettings>
#include "settingswidget.h"
#include "dataclassinstancemanage.h"

const int PASSWORD_ROLE = Qt::UserRole + 1000;
const int RECORD_PASSWORD_ROLE = Qt::UserRole + 1001;
const int AUTO_LOGIN_ROLE = Qt::UserRole + 1002;
//const int USERID_ROLE = Qt::UserRole + 1003;
//const int INCLASS_ROLE = Qt::UserRole + 1003;

loginDialog::loginDialog(QWidget *parent, CCU* c) :
    FramelessModalMovableShadowWidget(parent),
//    ui(new Ui::loginDialog),
    ccu(c)
{
    connect(ccu, &CCU::logOffFinished, [this](){enableChangeAcounntLogin(true); show();});
    setWindowTitle(tr("教师桌面"));
//    setWindowIcon(QIcon(":/Icon/tray.png"));
//    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    //ui->setupUi(this);
    //ui->passwordEdit->setEchoMode(QLineEdit::Password);
//    readLocalUserJsonFile("login.json");
//    QVBoxLayout* mainLayout = new QVBoxLayout(this);
//    int nShadowWidth = getShadowWidth();
//    mainLayout->setContentsMargins(nShadowWidth, 0, nShadowWidth, nShadowWidth + 20);
//    mainLayout->setSpacing(6);

//    QVBoxLayout* accountLayout = new QVBoxLayout;
//    QVBoxLayout* photoLayout = new QVBoxLayout;
//    QVBoxLayout* labelLayout = new QVBoxLayout;
//    QHBoxLayout* checkBoxLayout = new QHBoxLayout;

//    QHBoxLayout* topLayout = new QHBoxLayout;
//    topLayout->setMargin(0);

//    QHBoxLayout* centerLayout = new QHBoxLayout;
//    QHBoxLayout* bottomLayout = new QHBoxLayout;

//    auto titleBar = new TitleBar ("", this);
//    connect (titleBar, &TitleBar::tbClose, [this] ()
//    {
//        this->close ();
//    });

//    qDebug()<<"come in login dialog"<<endl;

    readLocalUserJsonFile("login.json");

    //顶部动画及login.
    topLabel = new QLabel(this);
//    QPixmap loginImg;
//    loginImg.load(":/Icon/login.png");
//    topLabel->setFixedWidth(width () - 2 * nShadowWidth);
    topLabel->setFixedHeight(80);
//    topLabel->setStyleSheet("QLabel{border-image: url(:/Icon/05_03.png);}");
//    topLabel->setAlignment(Qt::AlignCenter);
//    topLabel->setPixmap(loginImg);
    QFileInfo gifFileInfo("./gif/bg.gif");
    Q_ASSERT_X(gifFileInfo.exists(), "loginDialog", "loginDialog login gif file not exists.");

    m_pHeaderMovie = new QMovie("./gif/bg.gif");
    topLabel->setMovie(m_pHeaderMovie);
    connect(m_pHeaderMovie, SIGNAL(finished()), m_pHeaderMovie, SLOT(start()));

    QLabel *pLoginLb = new QLabel;
    QPixmap loginImg;
    loginImg.load(":/Icon/login.png");
    pLoginLb->setPixmap(loginImg);

    QHBoxLayout *pMovieLbLayout = new QHBoxLayout;
    pMovieLbLayout->addStretch(1);
    pMovieLbLayout->addWidget(pLoginLb);
    pMovieLbLayout->addStretch(1);
    topLabel->setLayout(pMovieLbLayout);

//    QHBoxLayout* topLayout = new QHBoxLayout;
//    topLayout->setMargin(0);
//    topLayout->addWidget(topLabel);

//    recordBox = new QCheckBox();
//    recordBox->setText(tr("记住密码"));
//    autoBox = new QCheckBox();
//    autoBox->setText(tr("自动登录"));
//    schmeBox = new QCheckBox();
//    schmeBox->setText(tr("上课模式"));
#define ENBALE_REGISTER_ACCOUT
#ifdef ENBALE_REGISTER_ACCOUT  //暂时不用账号注册于忘记密码功能.
    regLabel = new QLabel(/*tr("注册账号")*/);
    regLabel->setEnabled(false);
    regLabel->installEventFilter(this);
    regLabel->setFixedHeight(30);
    regLabel->setFixedWidth(70);
//    regLabel->setStyleSheet("color:#6c98ef; font-size:12px");

    forgetLabel = new QLabel(/*tr("忘记密码?")*/);
    forgetLabel->setEnabled(false);
    forgetLabel->installEventFilter(this);
    forgetLabel->setFixedHeight(30);
    forgetLabel->setFixedWidth(70);
//    forgetLabel->setStyleSheet("color:#6c98ef; font-size:12px");
#endif
    //forgetLabel->setStyleSheet();
//    recordLabel = new QLabel(tr("记住密码"));
//    recordLabel->setFixedHeight(30);
//    recordLabel->setFixedWidth(70);

//    autoLabel = new QLabel(tr("自动登录"));
//    autoLabel->setFixedHeight(30);
//    autoLabel->setFixedWidth(70);

//    schmeLabel = new QLabel(tr("上课模式"));
//    schmeLabel->setFixedHeight(30);
//    schmeLabel->setFixedWidth(70);

    photoLabel = new MaskLabel(this);
    photoLabel->setFixedWidth(70);
    photoLabel->setFixedHeight(70);

//    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
//    QString sKey = /*Arg::username + */"userSelectIcon" + QString::number(Arg::userId);
//    QVariant var = configSettings.value(sKey, QPixmap());
//    QPixmap iconPix;

//    if(var.canConvert(QMetaType::QPixmap)) {
//        iconPix = var.value<QPixmap>();
//    }

//    if(iconPix.isNull()) {
    QPixmap iconPix(":/Icon/Settings/Head1.png");
//    }

    photoLabel->setPixmap(iconPix);

    ///Mark,暂时用这个来实现账号跟密码ui重叠.
    QWidget *pAccoutPasswordWgt = new QWidget;
    pAccoutPasswordWgt->setFixedSize(217, 62);

    pLoginAccountComboBox = new QComboBox(pAccoutPasswordWgt);
    pLoginAccountComboBox->setFixedHeight(31);
    pLoginAccountComboBox->setFixedWidth(217);
    pLoginAccountComboBox->setGeometry(0, 0, 31, 217);
    pLoginAccountComboBox->setEditable(true);
    connect(pLoginAccountComboBox->lineEdit(), SIGNAL(textChanged(const QString&)), this, SLOT(currentAccountChanged(const QString&)));
    connect(pLoginAccountComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(accountChanged(int)));
    pLoginAccountComboBox->setStyleSheet("QComboBox {border-left: 1px solid lightGray; \
                                          border-top:1px solid lightGray;\
                                          border-right:1px solid lightGray; \
                                          border-bottom: 0px; \
                                          border-top-left-radius: 3px; \
                                          border-top-right-radius: 3px;} \
                                          QComboBox::drop-down { \
                                          width:30px; \
                                          background:transparent; \
                                          border-width: 0px } \
                                          QComboBox::down-arrow { \
                                          image: url(:/image/downarrow-normal.png);} \
                                          QComboBox::down-arrow:pressed { \
                                          image :url(:image/downarrow-pressed.png); }");
//    qDebug()<<"intiBox"<<endl;
//    initComboBox();
//    qDebug()<<"after initBox"<<endl;

    //usernameInput = new QLineEdit();
    //usernameInput->setPlaceholderText(tr("手机号/邮箱"));
    passwordInput = new QLineEdit(pAccoutPasswordWgt);
    passwordInput->setStyleSheet("QLineEdit{border: 1px solid lightGray; \
                                 border-bottom-left-radius: 3px; \
                                 border-bottom-right-radius: 3px;}");
    passwordInput->setPlaceholderText(tr("密码"));
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setFixedHeight(31);
    passwordInput->setFixedWidth(217);
    passwordInput->setGeometry(0, 31, 31, 217);

    loginButton = new QPushButton();
    loginButton->setText(tr("登录"));
    loginButton->setFixedWidth(217);
    loginButton->setFixedHeight(30);
    loginButton->setAutoDefault(true);
    loginButton->setShortcut(QKeySequence("Enter"));
    loginButton->setStyleSheet("QPushButton{border:0px; \
                                background-color: rgb(92, 190, 255); \
                                color: white; \
                                border-radius: 3px} \
                                QPushButton:hover{border:0px; \
                                background-color: #09a3dc; \
                                color: white; \
                                border-radius: 3px} \
                                    QPushButton:pressed{border:0px; \
                                    background-color: #09a3dc; \
                                    color: white; \
                                    border-radius: 3px} ");


    connect(loginButton, SIGNAL(clicked()), this, SLOT(on_loginButton_clicked()));

//    photoLayout->addWidget(photoLabel);
//    photoLayout->setMargin(10);

    QGridLayout *pGridLayout = new QGridLayout;
    pGridLayout->setVerticalSpacing(0);
    pGridLayout->addWidget(photoLabel, 0, 0, 2, 1);
    pGridLayout->addWidget(pAccoutPasswordWgt, 0, 1, 2, 1);
//    pGridLayout->addWidget(pLoginAccountComboBox, 0, 1, 1, 1);
//    pGridLayout->addWidget(passwordInput, 1, 1, 1, 1);

#ifdef ENBALE_REGISTER_ACCOUT
    pGridLayout->addWidget(regLabel, 0, 2, 1, 1); // row = 0 col = 1
    pGridLayout->addWidget(forgetLabel, 1, 2, 1, 1); //row = 1 col = 2
#endif
    pGridLayout->setContentsMargins(20, 5, 20, 0);

    //accountLayout->addWidget(usernameInput);
//    accountLayout->addWidget(pLoginAccountComboBox);
//    //---------Need to fixed-----------//
//    QLabel* emptyLabel = new QLabel(this);
//    emptyLabel->setFixedHeight(20);
//    accountLayout->addWidget(emptyLabel);
//    //---------------------------------//
//    accountLayout->addWidget(passwordInput);
//    accountLayout->setMargin(10);

//    labelLayout->addWidget(regLabel);
//    labelLayout->addWidget(forgetLabel);
//    labelLayout->setMargin(10);

    recordBox = new QCheckBox();
    recordBox->setText(tr("记住密码"));
//    recordBox->setCheckable(true);

    autoBox = new QCheckBox();
    autoBox->setText(tr("自动登录"));
//    autoBox->setCheckable(true);
    connect(autoBox, &QCheckBox::toggled, [this](bool checked){if(checked){recordBox->setChecked(checked);}});

    QPushButton *pSettingsBtn = new QPushButton;
    pSettingsBtn->setText(tr("设置"));
    pSettingsBtn->setFlat(true);
    connect(pSettingsBtn, &QPushButton::clicked, [this]() {SettingsWidget *pSettingsWgt = SettingsWidget::getInstance();
        pSettingsWgt->load();
        pSettingsWgt->show();
        loginButton->setEnabled(true);});

//    schmeBox = new QCheckBox();
//    schmeBox->setText(tr("上课模式"));

    QHBoxLayout* checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->setSpacing(0);
    checkBoxLayout->addWidget(recordBox);
    checkBoxLayout->addSpacing(10);
//    checkBoxLayout->addWidget(recordLabel);
    checkBoxLayout->addWidget(autoBox);
    checkBoxLayout->addSpacing(1);
//    checkBoxLayout->addWidget(autoLabel);
    checkBoxLayout->addWidget(pSettingsBtn);
//    checkBoxLayout->addWidget(schmeLabel);
    checkBoxLayout->setContentsMargins(90, 0, 50, 0);

//    centerLayout->addLayout(photoLayout);
//    centerLayout->addLayout(accountLayout);
//    centerLayout->addLayout(labelLayout);

    QHBoxLayout* bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch(1);
    bottomLayout->addWidget(loginButton);
    bottomLayout->addStretch(1);

    //qDebug()<<"load login.json"<<endl;
    //readLocalUserJsonFile("login.json");
    //qDebug()<<"after load login.json"<<endl;
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    int nShadowWidth = getShadowMargin();
    mainLayout->setContentsMargins(nShadowWidth, nShadowWidth - 5, nShadowWidth, nShadowWidth + 10);
    mainLayout->setSpacing(6);

//    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(topLabel);
    mainLayout->addLayout(pGridLayout);
//    mainLayout->addLayout(centerLayout);
    mainLayout->addLayout(checkBoxLayout);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(bottomLayout);

//    this->setFixedWidth(390 + 2 * nShadowWidth);
//    this->setFixedHeight(265 + 2 * nShadowWidth);
//    this->setStyleSheet("background-color:white;");
    //setWindowFlags(Qt::FramelessWindowHint);

    setFixedSize(400 + 2 * nShadowWidth, 261 + 2 * nShadowWidth);

    auto titleBar = new TitleBar ("", this);
    connect (titleBar, &TitleBar::tbClose, [this] ()
    {
        this->close ();
        qApp->quit();
    });

    titleBar->setAttribute (Qt::WA_TranslucentBackground);
    titleBar->setStyleSheet (QStringLiteral ("background-color: transparent;"));
    titleBar->setFixedWidth (this->width () - 2 * nShadowWidth);
    titleBar->setFixedHeight (80);
    titleBar->raise ();
    titleBar->move (nShadowWidth, 0);

    m_nAutoLoginTimerID = -1;

//    qDebug() << "logindialog initilized end!";

    initializeUIValue();

    if(pLoginAccountComboBox->count() > 0) {
        accountChanged(0);
    }

//    qDebug() << "logindialog ui value end.";
}

loginDialog::~loginDialog()
{
//    delete ui;

    delete jsonFile;
    jsonFile = nullptr;

    delete m_pHeaderMovie;
    m_pHeaderMovie = Q_NULLPTR;
}

QWidget *loginDialog::getMainwindow()
{
    ///Mark,暂时这么处理，因为主窗口管理设计有问题.
    return ccu->getMainWindow();
}

void loginDialog::enableChangeAcounntLogin(bool bEnabled)
{
    m_bChangeAccountLogin = bEnabled;
}

void loginDialog::onAppStartMessageReceived(const QString &)
{
    QWidget *pWgt = ccu->getMainWindow();

    if(pWgt == Q_NULLPTR) {
        pWgt = this;
    }

    Qt::WindowStates winStatus = Qt::WindowNoState;
    if (pWgt->windowState() & Qt::WindowMaximized)
    {
        winStatus = Qt::WindowMaximized;
    }

    pWgt->setWindowState(Qt::WindowMinimized);
    pWgt->setWindowState(Qt::WindowActive | winStatus);
    pWgt->show();
    pWgt->raise();

//    QMessageBox::information(this, tr("test"), tr("test"));
}

void loginDialog::loginState(const QJsonObject &jo)
{
    qDebug() << "login's jsondata"<<endl;
    //QMessageBox::information(this, "提示", QString(QJsonDocument(jo).toJson()));
    QJsonValue statusValue = jo.value("status");
    if(statusValue.isString())
    {
//        qDebug() << "the user info jo is:" << jo;

        QString status = statusValue.toString();
        if(status.compare("true") == 0)
        {
            QString userState = pLoginAccountComboBox->currentText().trimmed();
            QString passwordState = passwordInput->text();
            bool recordState = (recordBox->checkState() == Qt::Checked);
            bool autoState = (autoBox->checkState() == Qt::Checked);
//            bool schmeState = (schmeBox->checkState() == Qt::Checked);

            QString token = jo.value("token").toString();
            //设置用户id，获取课程主表时候使用.
            Arg::userId = jo.value("userid").toInt();

            QByteArray ba;
            ba.append(userState);
            QByteArray nameHashBa = QCryptographicHash::hash(ba, QCryptographicHash::Md5);

            ba.clear();
            ba.append(passwordState);
            QByteArray passwordBa = ba.toHex();

            QString sEncryPassword(nameHashBa);
            sEncryPassword.append(passwordBa);

            QJsonArray userArray = jsonFile->value("users").toArray();
            //二分查找
            bool find = false;
            for(int i = 0; i < userArray.size(); i++)
            {
                QJsonObject user = userArray.at(i).toObject(); ///Mark 2017.01.19.返回的是副本，修改并不能生效.
                if(userState.compare(user.value("user").toString()) == 0)
                {
                    user["user"] = userState;
                    user["password"] = sEncryPassword;
                    user["record"] = recordState;
                    user["auto"] = autoState;
                    user["userid"] = Arg::userId;
//                    user["schme"] = schmeState;

                    find = true;

                    ///Mark 2017.01.19. 前面toObject返回的是副本，修改并不能生效，两种选择生效.
                    userArray.removeAt(i);
                    userArray.insert(0, user);

                    break;
                }
            }

            if(!find){
                QJsonObject tmp;
                tmp.insert("user", userState);
                tmp.insert("password", sEncryPassword);
                tmp.insert("record", recordState);
                tmp.insert("auto", autoState);
                tmp.insert("userid", Arg::userId);
//                tmp.insert("schme", schmeState);
                userArray.insert(0,tmp);               
//                pLoginAccountComboBox->setItemData(nCurrentIndex, user.value("userid"), USERID_ROLE);
                //jsonFile["users"] = QJsonObject(userArray);
                //jsonFile->value("users") = QJsonObject(userArray);
            }

            jsonFile->remove("users");
            jsonFile->insert("users", userArray);

            writeLocalUserJsonFile("login.json");

            int nCurrentIndex = pLoginAccountComboBox->currentIndex();
            pLoginAccountComboBox->setItemData(nCurrentIndex, passwordState);
            pLoginAccountComboBox->setItemData(nCurrentIndex, recordState);
            pLoginAccountComboBox->setItemData(nCurrentIndex, autoState);

//            qDebug() << status + " " + token <<endl;
            //QString Arg::tokenStr = "&username=admina&token=111111";
            //QString Arg::tokenStr2 = "?username=admina&token=111111";
            QString tokenStr = "&username=" + userdb->getUsername() + "&token=" + token;
            QString tokenStr2 = "?username=" + userdb->getUsername() + "&token=" + token;
            Arg::tokenStr = tokenStr;
            Arg::tokenStr2 = tokenStr2;
            Arg::token = token;

            Arg::username = userState;

            close();

            if(m_pLoginStatusWidget == Q_NULLPTR) {
                m_pLoginStatusWidget = new LoginStatusWidget;
            }

            m_pLoginStatusWidget->setOpacity(0.0f);
            m_pLoginStatusWidget->resize(540, 370);
            m_pLoginStatusWidget->move(qApp->desktop()->width() / 2 - m_pLoginStatusWidget->width() / 2, qApp->desktop()->height() / 2 - m_pLoginStatusWidget->height() / 2);
            m_pLoginStatusWidget->show();
            qApp->processEvents(); //确保进度提示框显示再进行后面的任务.

            connect(Arg::nw, SIGNAL(showMainWindow()), this, SLOT(showMainWindow()), Qt::UniqueConnection);

            Arg::nw->getBaseData();
            ccu->startWork();

//            loginStatusWidget.close();

//            m_pHeaderMovie->stop();
        }
        else {
            //qDebug() << status <<endl;
#ifdef USER_QT_MESSAGEBOX
            QMessageBox::information(this, tr("教师客户端"), tr("登录失败，请检查用户名跟密码！"));
#else
            MessageDisplayWidget::showMessage(tr("教师客户端"), tr("登录失败，请检查用户名跟密码！"));
#endif
        }
//        writeLocalUserJsonFile("login.json");
    }
    else {
        //qDebug() << status <<endl;
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("登录失败,请检查网络设置！"));
#else
        MessageDisplayWidget::showMessage(tr("教师客户端"), tr("登录失败,请检查网络设置！"));
#endif
    }

    loginButton->setEnabled(true);
}

void loginDialog::on_pushButton_2_clicked()
{

}

void loginDialog::on_loginButton_clicked()
{
    if(!loginButton->isEnabled()) {
        return;
    }

    //其实在这一个地方发现服务器地址为空，拦截住即可.
    Arg *pArg = Arg::getInstance();
    QString sBaseUrl;
    bool bStatus = pArg->getNetReusetHostUrl(sBaseUrl);
    if(!bStatus) {
        return;
    }

    loginButton->setEnabled(false);

    if(m_nAutoLoginTimerID != -1) {
        killTimer(m_nAutoLoginTimerID);
    }

    int index = pLoginAccountComboBox->findText(pLoginAccountComboBox->lineEdit()->text());
    if(index == -1) {
        pLoginAccountComboBox->addItem(pLoginAccountComboBox->lineEdit()->text());
        pLoginAccountComboBox->setCurrentText(pLoginAccountComboBox->lineEdit()->text());
    }

    if(pLoginAccountComboBox->currentText() != NULL && passwordInput->text() != NULL)
    {
        qDebug() << "login button push" <<endl;
//        userdb = new UserDB();
//        connect(userdb, SIGNAL(updateLoginUI(QJsonObject)), this, SLOT(loginState(QJsonObject)) );
        ///MARK,上面两句代码会多次创建，多次连接信号，修改为如下.
        if(userdb == Q_NULLPTR) {
//            userdb = new UserDB();
            userdb = DataClassInstanceManage::getInstance()->getUserDBPtr();
            connect(userdb, SIGNAL(updateLoginUI(const QJsonObject&)), this, SLOT(loginState(const QJsonObject&)) );
            connect(userdb, &UserDB::modifiedPasswordByUserNameFinished, this, &loginDialog::modifiedPasswordFinished);
            connect(userdb, &UserDB::loginRequestError, [this](const QString &sError){loginButton->setEnabled(true);
#ifdef USER_QT_MESSAGEBOX
                QMessageBox::information(this, tr("教师客户端"), tr("登录错误：%1").arg(sError));});
#else
                MessageDisplayWidget::showMessage(tr("教师客户端"), tr("登录错误：%1").arg(sError));});
#endif
        }

        userdb->setUsername(pLoginAccountComboBox->lineEdit()->text());
        userdb->setPassword(passwordInput->text());
        userdb->query();
    }
    else
    {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::about(NULL, "教师客户端", "用户名密码不能为空");
#else
        MessageDisplayWidget::showMessage("教师客户端", "用户名密码不能为空", MessageDisplayButtonType::Ok);
#endif

        if(pLoginAccountComboBox->currentText().isEmpty()) {
            pLoginAccountComboBox->setFocus();
        }
        else {
            passwordInput->setFocus();
        }

        loginButton->setEnabled(true);
    }

    SettingsWidget *pSettingsWgt = SettingsWidget::getInstance();
    if(pSettingsWgt->isVisible()) {
        pSettingsWgt->close();
    }
}

void loginDialog::currentAccountChanged(const QString &sText)
{
    loginButton->setEnabled(true);

    ///Mark，暂时这么简单处理.整个处理逻辑暂时存在问题，侧面暂时添加.
    Arg::username = sText;
    Arg *pArg = Arg::getInstance();
    pArg->loadConfigDir();

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

    photoLabel->setPixmap(iconPix);

    int index = pLoginAccountComboBox->findText(sText);
    if(index != -1) {
        pLoginAccountComboBox->setCurrentIndex(index);
    }
}

void loginDialog::modifiedPasswordFinished(const QString &sUserName, const QString &sPassword)
{
    qDebug() << "modified password end:" << sUserName;
}

void loginDialog::accountChanged(int nIndex)
{
    loginButton->setEnabled(true);

    if(m_nAutoLoginTimerID != -1) {
        killTimer(m_nAutoLoginTimerID);
        m_nAutoLoginTimerID = -1;
    }

    bool bRecordPassword = pLoginAccountComboBox->itemData(nIndex, RECORD_PASSWORD_ROLE).toBool();
    recordBox->setChecked(bRecordPassword);
    QString sPassword = bRecordPassword ? pLoginAccountComboBox->itemData(nIndex, PASSWORD_ROLE).toString() : QString();
    passwordInput->setText(sPassword);

    autoBox->setChecked(pLoginAccountComboBox->itemData(nIndex, AUTO_LOGIN_ROLE).toBool());
//    bool bGetUserIDStatus = false;
//    Arg::userId = pLoginAccountComboBox->itemData(nIndex, USERID_ROLE).toInt(&bGetUserIDStatus);

    Arg::username = pLoginAccountComboBox->currentText().trimmed();
    Arg *pArg = Arg::getInstance();
    pArg->loadConfigDir();


//    schmeBox->setChecked(pLoginAccountComboBox->itemData(nIndex, INCLASS_ROLE).toBool());
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

    photoLabel->setPixmap(iconPix);

    m_nAutoLoginTimerID = startTimer(5000);
}

///Mark,这个逻辑重构移走，登录窗口不应该管理这么多功能逻辑.
void loginDialog::showMainWindow()
{
    ///Mark，这些逻辑以后一定不能绑定到登录窗口里.
    ccu->showMainWindow();

    QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
    QString sKey = Arg::username + "displayMode";
    ViewMode mode = static_cast<ViewMode>(configSettings.value(sKey, static_cast<quint8>(ViewMode::Standard)).toInt());

    if(mode != Arg::viewMode) {
        ccu->changeViewMode(mode);
    }

    m_pLoginStatusWidget->close();
    delete m_pLoginStatusWidget;
    m_pLoginStatusWidget = Q_NULLPTR;

    m_pHeaderMovie->stop();
}

bool loginDialog::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == regLabel) {

    }
}

void loginDialog::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    if(!m_bChangeAccountLogin) {
        m_nAutoLoginTimerID = startTimer(5000); //5s定时自动登录计时.
    }
    else {
        passwordInput->clear();
        autoBox->setChecked(false);
    }

    m_pHeaderMovie->start();
}

void loginDialog::timerEvent(QTimerEvent *pTmEv)
{
    if(pTmEv->timerId() == m_nAutoLoginTimerID) {
        killTimer(m_nAutoLoginTimerID);
        m_nAutoLoginTimerID = -1;
        //自动登录逻辑.
        if((autoBox->isChecked()) && (!passwordInput->text().trimmed().isEmpty())) {
            on_loginButton_clicked();
        }
    }
}

void loginDialog::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev);

    QPainterPath path;
    int nShadowWidth = getShadowMargin();
    path.addRoundedRect(nShadowWidth, nShadowWidth, width() - 2 * nShadowWidth, height() - 2 * nShadowWidth, 5.0f, 5.0f);
    painter->fillPath(path, QBrush(Qt::white));
}

void loginDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
        on_loginButton_clicked();
}

void loginDialog::readLocalUserJsonFile(const QString &filename)
{
//    qDebug()<<"load json"<<endl;
    if(jsonFile != nullptr)
        delete jsonFile;
    else
    {
        jsonFile = new QJsonObject();
    }
    QFile file(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString all = file.readAll();
    file.close();
    QJsonDocument docu = QJsonDocument::fromJson(all.toUtf8());
    *jsonFile = docu.object();
//    qDebug() << QString(QJsonDocument(*jsonFile).toJson())<<endl;
    //QJsonValue usersValue = jsonFile->value("users");
    //QJsonArray userArray = usersValue.toArray();
    //qDebug() << userArray.size()<<endl;
}

void loginDialog::writeLocalUserJsonFile(const QString &filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    QByteArray sJson = QJsonDocument(*jsonFile).toJson();
//    qDebug() << "the config json value is:" << QString(sJson);
    file.write(sJson);
    file.close();

//    delete jsonFile;
//    jsonFile = nullptr;
}

void loginDialog::initializeUIValue()
{
    QJsonArray userArray = jsonFile->value("users").toArray();
    QString sTmp;

    for(int i = 0; i < userArray.size(); i++)
    {
        QJsonObject user = userArray.at(i).toObject();
        sTmp.clear();
        sTmp.append(user.value("user").toString());
        pLoginAccountComboBox->insertItem(i, sTmp);

        QByteArray ba;
        ba.append(sTmp);
        QByteArray nameHashBa = QCryptographicHash::hash(ba, QCryptographicHash::Md5);

        sTmp.clear();
        sTmp = user.value("password").toString();
        sTmp.remove(QString(nameHashBa));

        ba.clear();
        ba.append(sTmp);
        ba = QByteArray::fromHex(ba);
        sTmp.clear();
        sTmp.append(ba);
//        ba.clear();
//        ba.append(passwordState);
//        QByteArray passwordBa = ba.toHex();

//        QString sEncryPassword(nameHashBa);
//        sEncryPassword.append(passwordBa);

        pLoginAccountComboBox->setItemData(i, sTmp, PASSWORD_ROLE);
        pLoginAccountComboBox->setItemData(i, user.value("record"), RECORD_PASSWORD_ROLE);
        pLoginAccountComboBox->setItemData(i, user.value("auto"), AUTO_LOGIN_ROLE);
//        pLoginAccountComboBox->setItemData(i, user.value("userid"), USERID_ROLE);
    }

    if(pLoginAccountComboBox->count() > 0) {
        pLoginAccountComboBox->setCurrentIndex(0);

        bool bRecordPassword = pLoginAccountComboBox->currentData(RECORD_PASSWORD_ROLE).toBool();
        recordBox->setChecked(bRecordPassword);
        QString sPassword = bRecordPassword ? pLoginAccountComboBox->currentData(PASSWORD_ROLE).toString() : QString();
        passwordInput->setText(sPassword);

        autoBox->setChecked(pLoginAccountComboBox->currentData(AUTO_LOGIN_ROLE).toBool());

//        schmeBox->setChecked(pLoginAccountComboBox->currentData(INCLASS_ROLE).toBool());
    }
}

//void loginDialog::initComboBox()
//{
//    //pLoginAccountComboBox->addItem("item",0);
//    //pLoginAccountComboBox->setItemText(0,tr("text"));
////    list_widget = new QListWidget();
////    pLoginAccountComboBox->setModel(list_widget->model());
////    pLoginAccountComboBox->setView(list_widget);
////    pLoginAccountComboBox->setEditable(true);
//    /*for(int i=0; i<3; i++)
//    {
//        AccountItem *account_item = new AccountItem();
//        account_item->setAccountNumber(QString("safe_") + QString::number(i, 10) + QString("@sina.com"));
//        connect(account_item, SIGNAL(showAccount(QString)), this, SLOT(showAccount(QString)));
//        connect(account_item, SIGNAL(removeAccount(QString)), this, SLOT(removeAccount(QString)));
//        QListWidgetItem *list_item = new QListWidgetItem(list_widget);
//        list_widget->setItemWidget(list_item, account_item);
//    }*/
//    QJsonArray userArray = jsonFile->value("users").toArray();
//    for(int i = 0; i < userArray.size(); i++)
//    {
//        ///Mark by BiXiaoxia 2017.01.16,这块是没用的.
////        QJsonObject user = userArray.at(i).toObject();
////        AccountItem *account_item = new AccountItem();
////        account_item->setAccountNumber(user.value("user").toString());
////        connect(account_item, SIGNAL(showAccount(QString)), this, SLOT(showAccount(QString)));
////        connect(account_item, SIGNAL(removeAccount(QString)), this, SLOT(removeAccount(QString)));
////        QListWidgetItem *list_item = new QListWidgetItem(list_widget);
////        list_widget->setItemWidget(list_item, account_item);
//        QJsonObject user = userArray.at(i).toObject();
//        pLoginAccountComboBox->insertItem(i, user.value("user").toString());
//        pLoginAccountComboBox->setItemData(i, user.value("password"), RECORD_PASSWORD_ROLE);
//        pLoginAccountComboBox->setItemData(i, user.value("auto"), AUTO_LOGIN_ROLE);
//        pLoginAccountComboBox->setItemData(i, user.value("schme"), INCLASS_ROLE);


//    }
//}

//将选项文本显示在QpLoginAccountComboBox当中
void loginDialog::showAccount(const QString &account)
{
        pLoginAccountComboBox->setEditText(account);
        QJsonArray userArray = jsonFile->value("users").toArray();
        int list_count = list_widget->count();
        for(int i=0; i<list_count; i++)
        {
            QListWidgetItem *item = list_widget->item(i);
            AccountItem *account_item = (AccountItem *)(list_widget->itemWidget(item));
            QString account_number = account_item->getAccountNumber();
            if(account == account_number)
            {
                QJsonObject choose = userArray.at(i).toObject();
                recordBox->setChecked(choose["record"].toBool());
                if(choose["record"].toBool())
                    passwordInput->setText(choose["password"].toString());
                autoBox->setChecked(choose["auto"].toBool());
//                schmeBox->setChecked(choose["schme"].toBool());
                delete item;
                break;
            }
        }
        pLoginAccountComboBox->hidePopup();
}

//删除帐号时，弹出提示框，与用户进行交互，告知是否确定要删除此帐号的所有信息！
void loginDialog::removeAccount(const QString &account)
{
    pLoginAccountComboBox->hidePopup();

    //if(msg_box->exec() == QDialog::Accepted)
    //{
        QJsonArray userArray = jsonFile->value("users").toArray();
        int list_count = list_widget->count();
        for(int i=0; i<list_count; i++)
        {
            QListWidgetItem *item = list_widget->item(i);
            AccountItem *account_item = (AccountItem *)(list_widget->itemWidget(item));
            QString account_number = account_item->getAccountNumber();
            if(account == account_number)
            {
                list_widget->takeItem(i);
                userArray.takeAt(i);
                jsonFile->insert("users", userArray);
                delete item;
                break;
            }
        }
    //}
}

