#include "lookandfeelconfigdialog.h"
#include "titlebar.h"
#include "config.h"
#include <QSlider>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <random>
LookAndFeelConfigDialog::LookAndFeelConfigDialog(double bo, double fo, QWidget *parent) : FramelessModalMovableShadowWidget(parent)
{
//    setWindowFlags (Qt::FramelessWindowHint);
    mBackgroundOpacitySlider = new QSlider (Qt::Horizontal, this);
    mBackgroundOpacitySlider->setRange (0, 100);
    mBackgroundOpacitySlider->setValue (static_cast<int>(bo * 100));
    mBackgroundOpacitySlider->setSingleStep (1);
    connect (mBackgroundOpacitySlider, static_cast<void(QSlider::*)(int)>(&QSlider::valueChanged), [this] (int value) {
        emit this->backgroundOpacityChanged (static_cast<double>(value) / 100.0);
    });
    auto boLabel = new QLabel ("背景透明度:");
    boLabel->setBuddy (mBackgroundOpacitySlider);

    mForegroundOpacitySlider = new QSlider (Qt::Horizontal, this);
    mForegroundOpacitySlider->setRange (0, 100);
    mForegroundOpacitySlider->setValue (static_cast<int>(fo * 100));
    mForegroundOpacitySlider->setSingleStep (1);
    connect (mForegroundOpacitySlider, static_cast<void(QSlider::*)(int)>(&QSlider::valueChanged), [this] (int value) {
        emit this->foregroundOpacityChanged (static_cast<double>(value) / 100.0);
    });
    auto foreboLabel = new QLabel ("前景透明度:");
    foreboLabel->setBuddy (mForegroundOpacitySlider);
    auto opacityLayout = new QHBoxLayout ();
    opacityLayout->setContentsMargins(10, 0, 10, 6);
    opacityLayout->addSpacing(20);
    opacityLayout->addWidget (boLabel);
    opacityLayout->addSpacing(5);
    opacityLayout->addWidget (mBackgroundOpacitySlider);
    opacityLayout->addSpacing(20);

    opacityLayout->addWidget (foreboLabel);
    opacityLayout->addSpacing(5);
    opacityLayout->addWidget (mForegroundOpacitySlider);
    opacityLayout->addSpacing(40);
    auto titleBar = new TitleBar ("皮肤", this);
    connect (titleBar, &TitleBar::tbClose, [this] ()
    {
        this->close ();
    });

    QPushButton *pRestoreDefaultBtn = new QPushButton;
    pRestoreDefaultBtn->setText(tr("恢复默认"));
    connect(pRestoreDefaultBtn, SIGNAL(clicked()), this, SLOT(restoreDefault()));
    opacityLayout->addWidget(pRestoreDefaultBtn);
    opacityLayout->addSpacing(20);

    mImageCandidatesLayout = new QVBoxLayout ();
    mImageCandidatesLayout->setContentsMargins(5, 5, 5, 5);
    mImageCandidatesLayout->setSpacing(5);

    mButtonGroup = new QButtonGroup (this);


    int nShadowMargin = getShadowMargin();
    auto layout = new QVBoxLayout (this);
    layout->setContentsMargins(nShadowMargin, nShadowMargin, nShadowMargin, nShadowMargin);
    layout->setSpacing(0);
    layout->addWidget(titleBar);
    layout->addLayout(mImageCandidatesLayout);
    layout->addLayout(opacityLayout);


#ifdef USED_STANDARD_WRITE_LOCAL
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);
    QDir skinDir = QDir(/*Arg::configDir*/dir.absolutePath () + "/background");
#else
    QDir skinDir = QDir(QApplication::applicationDirPath() + "./background");
    qDebug() << "the applicaiton dir path" << QApplication::applicationDirPath() << "filepath" << QApplication::applicationFilePath();
#endif

    loadBackgroundImageCandidates(skinDir);

//    QWidget *pMainScreen = Q_NULLPTR;
//    QWidgetList widgetLst = qApp->topLevelWidgets();
//    for (QWidgetList::ConstIterator it = widgetLst.constBegin(), cend = widgetLst.constEnd(); it != cend; ++it) {
//        QWidget *w = *it;
//        const QMetaObject *metaObj = w->metaObject();
//        QString sClassName = metaObj->className(); //必须经过这一步，否则判断不到.
//        if(sClassName == "MainScreen") {
//            pMainScreen = w;

//            break;
//        }
//    }

//    int nMinSize = pMainScreen->width() > pMainScreen->height() ? pMainScreen->height() : pMainScreen->width();
//    setFixedSize(nMinSize, nMinSize);

    setWindowTitle(tr("皮肤设置"));

    setStyleSheet("QDialog > QPushButton{background:#1250b5; \
                  border:1px solid #1250b5;\
                  color:white;}");
}

void LookAndFeelConfigDialog::loadBackgroundImageCandidates (const QDir& dir)
{    
    ///< \Mark,2016.12.20 by BiXiaoxia.
    QFileInfoList imgFileList = dir.entryInfoList (QDir::Files);
    int imgFileCount = 0;

//    //save and chage cursor.
//    QCursor savedCursor = cursor();
//    setCursor(Qt::WaitCursor);

    for (auto entry : imgFileList)
    {
        if (entry.isFile ()) {
            QLayout* layout;
            if (imgFileCount % LookAndFeelConfigDialog::candidatesColumn == 0)
            {
                layout = new QHBoxLayout;
                layout->setSpacing(5);
                mImageCandidatesLayout->addItem(layout);
            }
            else
            {
                layout = static_cast<QLayout*>(mImageCandidatesLayout->itemAt (mImageCandidatesLayout->count () - 1));
            }

            QString sIconPathName = entry.absoluteFilePath();
            QImage image(sIconPathName);
            image = image.scaled (this->width () / LookAndFeelConfigDialog::candidatesColumn, this->width () / LookAndFeelConfigDialog::candidatesColumn);
            ///< \Mark,2016.12.20 by BiXiaoxia,暂时用button这么处理，以后这个要换成model view处理.
            auto btn = new QPushButton (this);
            ///临时处理方式，暂时用objectName来存储背景图片路径.
            btn->setObjectName(sIconPathName);
            btn->setIconSize(image.size());
            btn->setFixedSize(image.size());
            btn->setIcon(QPixmap::fromImage(image));
            layout->addWidget(btn);
            mButtonGroup->addButton (btn);
            btn->setChecked(false);
            btn->setCheckable(true);
            btn->setStyleSheet("QPushButton:pressed{background-color: blue}");

            connect(btn, &QPushButton::clicked, this, &LookAndFeelConfigDialog::skinButtonChecked);

            ++imgFileCount;
        }
    }

//    setCursor(savedCursor);

    updateGeometry ();
}

void LookAndFeelConfigDialog::pickOne ()
{
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    if (mBackgroundImageCandidates.empty ())
        return;
    std::uniform_int_distribution<> dis (0, mBackgroundImageCandidates.size () - 1);
    //    emit backgroundImageChanged (mBackgroundImageCandidates.at (dis(gen)));
}

void LookAndFeelConfigDialog::skinButtonChecked(bool bChecked)
{
    QPushButton *pBtn = qobject_cast<QPushButton*>(sender());
    QString sIconPathName;

    if (bChecked)
    {
        sIconPathName.append(pBtn->objectName());
    }

    emit backgroundImageChanged(sIconPathName);
}

void LookAndFeelConfigDialog::restoreDefault()
{
    emit restoreSkin();
}

void LookAndFeelConfigDialog::loadBackgroundImageCandidates (const QFileInfo& file)
{
    if (file.exists ())
    {
//        loadBackgroundImageCandidates (QImage (file.absoluteFilePath ()));
        ///< Replace by BiXiaoxia 2016.12.20.
        loadBackgroundImageCandidates (file.absoluteFilePath());
    }
}

///MARK, 废弃.
void LookAndFeelConfigDialog::loadBackgroundImageCandidates (const QString &sIconPathName)
{
    QLayout* layout;
    if (mBackgroundImageCandidates.size () % LookAndFeelConfigDialog::candidatesColumn == 0)
    {
        layout = new QHBoxLayout;
        layout->setSpacing(6);
        mImageCandidatesLayout->addItem(layout);
    }
    else
    {
        layout = static_cast<QLayout*>(mImageCandidatesLayout->itemAt (mImageCandidatesLayout->count () - 1));
    }

//    mBackgroundImageCandidates.push_back (image.scaled (this->width () / LookAndFeelConfigDialog::candidatesColumn, this->width () / LookAndFeelConfigDialog::candidatesColumn));

    ///< \Mark,2016.12.20 by BiXiaoxia,暂时这么处理，以后这个要换成model view处理.
    QImage image(sIconPathName);
    image = image.scaled (this->width () / LookAndFeelConfigDialog::candidatesColumn, this->width () / LookAndFeelConfigDialog::candidatesColumn);
    auto btn = new QPushButton (this);
    ///临时处理方式，暂时用objectName来存储背景图片路径.
    btn->setObjectName(sIconPathName);
    btn->setIconSize(image.size());
    btn->setFixedSize(image.size());
    btn->setIcon(QPixmap::fromImage(image));
    layout->addWidget(btn);
    mButtonGroup->addButton (btn);
    btn->setChecked (false);
    btn->setCheckable (true);

    connect (btn, static_cast<void(QPushButton::*)(bool)>(&QPushButton::clicked), [this, sIconPathName] (bool checked)
    {
        if (checked)
            emit this->backgroundImageChanged (sIconPathName);
        else
            emit this->backgroundImageChanged (QString());
    });

    updateGeometry ();
    update ();
}

void LookAndFeelConfigDialog::mousePressEvent (QMouseEvent* ev)
{
//    QWidget::mousePressEvent (ev);
    mPressed = true;
    mPreWindowPos = pos ();
    mPreCursorPos = ev->globalPos ();
}

void LookAndFeelConfigDialog::mouseMoveEvent (QMouseEvent* ev)
{
    if (mPressed)
    {
        move (ev->globalPos () - mPreCursorPos + mPreWindowPos);
    }
//    QDialog::mouseMoveEvent (ev);
}


void LookAndFeelConfigDialog::mouseReleaseEvent (QMouseEvent* ev)
{
    mPressed = false;
}
