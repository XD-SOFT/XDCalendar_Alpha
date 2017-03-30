#include "setupdialog.h"
#include "config.h"
#include "teacher.h"
#include "mybutton.h"
#include "titlebar.h"
#include "modifypassworddialog.h"
#include "masklabel.h"
#include <QDebug>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QGridLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QLabel>
#include <QWidget>
#include <QMap>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QDockWidget>
#include <QFileDialog>
#include <QButtonGroup>
#include <utility>
#include <QStackedWidget>
#include <QListWidget>
#include <QBitmap>
#include <QSignalMapper>
#include <QRadioButton>
SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent)
{
    setup ();
    connect (mTitleBar, SIGNAL (tbClose ()), this, SLOT (reject ()));
    auto* layout = new QVBoxLayout (this);
    layout->setSpacing (0);
    layout->setMargin (0);
    layout->addWidget (mTitleBar);
    auto settingLayout = new QHBoxLayout ();
    settingLayout->addWidget (mTabbar, 2);
    settingLayout->addWidget (mSettingWidgets, 10);
    layout->addLayout (settingLayout);
    connect (mTabbar, SIGNAL (currentRowChanged (int)), mSettingWidgets, SLOT (setCurrentIndex(int)));
    setWindowFlags (Qt::FramelessWindowHint);

}


void SetupDialog::setup ()
{
    mAvatarLabelSelectMapper = new QSignalMapper (this);
    connect (mAvatarLabelSelectMapper, SIGNAL (mapped (int)), this, SLOT (selectAvatarLabel (int)));

    mTitleBar = new TitleBar ("SETTINGS", this);

    mUserSettings = new QWidget (this);
    setupUserSettings ();

    mGeneralSettings = new QWidget (this);
    setupGeneralSettings ();

    mCalendarSettings = new QWidget (this);
    setupCalendarSettings ();

    mSettingWidgets = new QStackedWidget (this);
    mSettingWidgets->addWidget (mUserSettings);
    mSettingWidgets->addWidget (mGeneralSettings);
    mSettingWidgets->addWidget (mCalendarSettings);

    mTabbar = new QListWidget (this);
    mTabbar->addItem (tr("General"));
    mTabbar->addItem (tr("User"));
    mTabbar->addItem (tr("Application"));
    mTabbar->addItem (tr("Calendar"));
    for (int i = 0; i < mTabbar->count (); ++i)
    {
        mTabbar->item (i)->setSizeHint (QSize (120, 50));
    }
}

void SetupDialog::setupGeneralSettings ()
{
    QGridLayout* layout = new QGridLayout (mGeneralSettings);

    int row = 0;
    auto addBinaryArgSetter = [&] (int& arg, const QString& title)
    {
        QCheckBox* checkbox = new QCheckBox (title, mGeneralSettings);
        connect (checkbox, &QCheckBox::stateChanged, [&] (int state)
        {
            arg = (state == Qt::Unchecked)? false: true;
        });
        layout->addWidget (checkbox, row++, 0);
    };
    auto addComboArgSetter = [&] (int& arg, const QString& title, const QMap<QString, int>& map)
    {
        QComboBox* combobox = new QComboBox (mGeneralSettings);
        QLabel* label = new QLabel (title, mGeneralSettings);
        label->setBuddy (combobox);
        for (auto it = map.begin (); it != map.end (); ++it)
            combobox->addItem (it.key ());
        connect (combobox, &QComboBox::currentTextChanged, [&] (const QString& text) {arg = map[text];});
        layout->addWidget (label, row, 0);
        layout->addWidget (combobox, row, 1);
        ++row;

    };

    addBinaryArgSetter (Arg::isMousePenetration, "Mouse Transparent");
    addBinaryArgSetter (Arg::lockDrag, "Lock Drag");
    addBinaryArgSetter (Arg::showChineseCalendar, "Show Chinese Calendar");
    addBinaryArgSetter (Arg::mergeLoad, "Merge on load");
    addBinaryArgSetter (Arg::todayLight, "Highlight current day");
    addComboArgSetter (Arg::GetFocusMode, "Focus Mode", {{"Click", 0}, {"Move In", 1}});
    addComboArgSetter (Arg::systemtrayPressType, "Tray Response Type", {{"Right Click", 0}, {"Left CLick", 1}});
}

void SetupDialog::setupCalendarSettings()
{
    QVBoxLayout* layout = new QVBoxLayout{mCalendarSettings};
    // Creating Horizontal Layout
    QGroupBox* grp_sync_school = new QGroupBox{"Hello"};
    layout->addWidget(grp_sync_school);
    QHBoxLayout* b_sync_school = new QHBoxLayout{grp_sync_school};

    b_sync_school->addWidget(new QLabel("Synchronize School Calendar"));
    auto b_sync_school_yes = new QRadioButton("Yes");
    auto b_sync_school_no = new QRadioButton("No");
    b_sync_school->addWidget(b_sync_school_yes);
    b_sync_school->addWidget(b_sync_school_no);
    QButtonGroup* g_sync_school = new QButtonGroup(this);
    g_sync_school->addButton(b_sync_school_yes);
    g_sync_school->addButton(b_sync_school_no);

}

void SetupDialog::setupUserSettings ()
{
    QVBoxLayout* layout = new QVBoxLayout (mUserSettings);

    //set Icon area
    QGroupBox* mIconGroupBox = new QGroupBox (mUserSettings);
    mIconGroupBox->setTitle ("AVATAR");
    QHBoxLayout* iconGBLayout = new QHBoxLayout (mIconGroupBox);
    QButtonGroup* mIconGroup = new QButtonGroup (mIconGroupBox);
    QPushButton* addBtn = new QPushButton ("ADD", mIconGroupBox);
    QLabel* addLabel = new MaskLabel (this);
    addLabel->setPixmap (QPixmap::fromImage (QImage(":/Icon/face.png")));
    addLabel->setScaledContents (true);
    addLabel->setMaximumSize (Const::SetupDialog::avatarSize, Const::SetupDialog::avatarSize);
    iconGBLayout->addWidget (addBtn);
    addBtn->setFixedSize (Const::SetupDialog::avatarSize, Const::SetupDialog::avatarInfoHeight);
    connect (addBtn, &QPushButton::clicked, [this, mIconGroup, iconGBLayout, mIconGroupBox] ()
    {
        QString name = QFileDialog::getOpenFileName (this, "Open Icon", "~/", "Images (*.jpg *.png *.bmp *.svg *.ico)");
        if (name.isNull () || name.isEmpty ())
            return;
        QFile file (name);
        QFileInfo fileinfo = QFileInfo (name);

        Arg *pArg = Arg::getInstance();
        QDir dir;
        pArg->getSaveDir(dir);
        QString newPath = dir.absolutePath () + '/' + QFileInfo (name).fileName ();
        newPath.replace("//", "/");
        newPath.replace("/", "\\");
        file.copy (newPath);

        QPixmap openedIcon = QPixmap::fromImage (QImage (newPath));

        this->addIcon (fileinfo, openedIcon);
    });
    iconGBLayout->addLayout (mAvatarsLayout = new QGridLayout);
    mAvatarsLayout->setRowStretch (0, 100);
    mAvatarsLayout->setRowStretch (1, 5);
    auto addLayout = new QVBoxLayout;
    addLayout->addWidget (addLabel);
    addLayout->addWidget (addBtn);
    iconGBLayout->addLayout (addLayout);

    if (Arg::user->getIconFile ().exists ())
        addIcon (Arg::user->getIconFile (), Arg::user->openIcon (), true);

    //normal info area
    QGroupBox* infoGroupBox = new QGroupBox (mUserSettings);
    QGridLayout* infoGBLayout = new QGridLayout (infoGroupBox);
    auto addInfoEdit = [&] (const QString& title, const QString& init, auto listener, int row)
    {
        QLabel* label = new QLabel (title, infoGroupBox);
        QLineEdit* edit = new QLineEdit (infoGroupBox);
        edit->setText (init);
        label->setBuddy (edit);
        connect (edit, &QLineEdit::textChanged, listener);
        infoGBLayout->addWidget (label, row, 0);
        infoGBLayout->addWidget (edit, row, 1);
    };
    int rowInInfoGB = 0;
    addInfoEdit ("LOCATION", Arg::user->getLocation (), [] (const QString& text) {Arg::user->setLocation (text);}, rowInInfoGB++);
    addInfoEdit ("SCHOOL", Arg::user->getSchool (), [] (const QString& text) {Arg::user->setSchool (text);}, rowInInfoGB++);
    addInfoEdit ("SUBJECT", Arg::user->getSubject (), [] (const QString& text) {Arg::user->setSubject (text);}, rowInInfoGB++);

    //password area
    QGroupBox* passwordGroupBox = new QGroupBox (mUserSettings);
    QHBoxLayout* passwordGBLayout = new QHBoxLayout (passwordGroupBox);
    passwordGBLayout->addWidget (new QLabel ("PASSWORD", passwordGroupBox));
    QPushButton* passwordDialogBtn (new QPushButton ("Edit", passwordGroupBox));
    connect (passwordDialogBtn, &QPushButton::clicked, [this] ()
    {
        auto dialog = new ModifyPasswordDialog (this);
        dialog->setWindowModality (Qt::WindowModal);
        dialog->show ();
    });
    passwordDialogBtn->setAttribute (Qt::WA_TranslucentBackground);
    passwordGBLayout->addWidget (passwordDialogBtn);

    //=========
    layout->addWidget (mIconGroupBox);
    layout->addWidget (infoGroupBox);
    layout->addWidget (passwordGroupBox);
}

void SetupDialog::updateIconGroupBox ()
{
    static QLabel* selectedLabel = new QLabel ("SELECTED", this);
    while (mAvatarsLayout->count ())
        mAvatarsLayout->takeAt (0);
    int cnt = 0;
    for (const auto& avatar: mAvatars)
    {
        mAvatarsLayout->addWidget (std::get<AvatarTupleElement::Label>(avatar), 0, cnt);
        if (std::get<AvatarTupleElement::Activated>(avatar))
            mAvatarsLayout->addWidget (selectedLabel, 1, cnt);
        ++cnt;
    }
}

void SetupDialog::addIcon (const QFileInfo& fileinfo, const QPixmap& pixmap, bool activated)
{
    QLabel* label = new MaskLabel (this);
    label->setPixmap (pixmap);
    label->setScaledContents (true);
    label->setMaximumSize (Const::SetupDialog::avatarSize, Const::SetupDialog::avatarSize);
    connect (label, SIGNAL (clicked ()), mAvatarLabelSelectMapper, SLOT (map ()));
    mAvatarLabelSelectMapper->setMapping (label, mAvatars.size ());
    mAvatars.push_back (std::make_tuple(label, fileinfo, pixmap, activated));
    updateIconGroupBox ();
}

void SetupDialog::selectAvatarLabel (int index)
{
    if (index < 0 || index >= mAvatars.size ())
        return;
    for (auto& avatar: mAvatars)
    {
        std::get<AvatarTupleElement::Activated>(avatar) = false;
    }
    std::get<AvatarTupleElement::Activated>(mAvatars[index]) = true;

    Arg::user->setIcon (std::get<AvatarTupleElement::File>(mAvatars[index]));

    updateIconGroupBox ();
}
