#include "friendcardwidget.h"
#include "friend.h"
#include "maskunclickablelabel.h"
#include "frienddetaildialog.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QCursor>
#include <QEvent>
#include <QDebug>
#include <QIcon>

FriendCardWidget::FriendCardWidget(Friend *fr, QWidget *parent) : QPushButton(parent)
{
    mFr = fr;

    setFlat(true);
    setFixedHeight(60);

    auto hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);

    //left avatar
    auto avatar = new MaskUnclickableLabel(0.95, this);
    avatar->setFixedSize(40, 40);
    try{
    avatar->setPixmap(QPixmap(fr->getIconPath()).scaled(avatar->size(),Qt::KeepAspectRatioByExpanding));
        hLayout->addWidget(avatar,1);
    }
    catch(std::exception e){
        qDebug()<<"avatar load failure: "<<e.what()<<endl;
    }

    //right text labels
    auto rLayout = new QVBoxLayout();
    {
        //name label(editable)
        mNameLabel = new QLabel();
        rLayout->addWidget(mNameLabel);

        //name label's edit input box
        mNameEditWidget = new QWidget();
        mNameEditWidget->hide();
        auto nameEditLayout = new QHBoxLayout(mNameEditWidget);
        nameEditLayout->setMargin(0);
        nameEditLayout->setSpacing(0);
        {
            //input box
            mNameLineEdit = new QLineEdit();
            nameEditLayout->addWidget(mNameLineEdit);

            //ok button
            auto doneButton = new QPushButton();
            doneButton->setIcon(QIcon(":/Icon/success.png"));
            doneButton->setFlat(true);
            doneButton->setMinimumWidth(20);
            nameEditLayout->addWidget(doneButton);

            //if lost focus, enter pressed or ok clicked
            connect(doneButton, &QPushButton::clicked, this, &FriendCardWidget::afterNickNameEditingFinish);
            connect(mNameLineEdit, &QLineEdit::editingFinished, this, &FriendCardWidget::afterNickNameEditingFinish);
        }
        rLayout->addWidget(mNameEditWidget);

        mInstituteLabel = new QLabel();
        mInstituteLabel->setObjectName("instituteLabel");
        rLayout->addWidget(mInstituteLabel);
    }
    hLayout->addLayout(rLayout,10);

    afterDataChanged(mFr);

    //ui order
    connect (this, &FriendCardWidget::detail, this, &FriendCardWidget::showDetailDialog);
    connect (this, &FriendCardWidget::rename, this, &FriendCardWidget::setNickNameEdit);
    //to model
    connect (this, &FriendCardWidget::toggleLive, mFr, &Friend::changeLiveStatusAlter);
    connect (this, &FriendCardWidget::editNickNameFinished, mFr, &Friend::changeNickName);
    //from model
    connect (mFr, &Friend::liveStatusChanged, this, &FriendCardWidget::afterLiveStatusChanged);
    connect (mFr, &Friend::dataChanged, this, &FriendCardWidget::afterDataChanged);
}

void FriendCardWidget::showDetailDialog()
{
    auto dialog = new FriendDetailDialog(mFr);
    dialog->show();
    dialog->setFocus();
}

void FriendCardWidget::afterDataChanged(Friend *fr)
{
    if(fr != mFr)
        qDebug()<<"FriendCardWidget data changed: object doesn't match!"<<endl;
    mNameLabel->setText(mFr->getDisplayName());
    mNameLineEdit->setText(mFr->getNickName());
    mInstituteLabel->setText(mFr->getInstituteName());
}

void FriendCardWidget::setNickNameEdit()
{
    mNameLabel->hide();
    mNameEditWidget->show();
    mNameLineEdit->setFocus();
    mNameLineEdit->selectAll();

//    emit setNickNameEditFinished();
}

void FriendCardWidget::afterLiveStatusChanged()
{
    mToggleLiveAction->setText(mFr->getLiveStatusToggleString());
}

void FriendCardWidget::afterNickNameEditingFinish(){
    emit editNickNameFinished(mNameLineEdit->text());

    mNameEditWidget->hide();
    mNameLabel->show();
}

void FriendCardWidget::contextMenuEvent(QContextMenuEvent *event)
{
    auto menu = new QMenu();

    auto DetailAction = menu->addAction (tr("查看详情"));
    connect (DetailAction, &QAction::triggered, [this](){
       emit detail();
    });

    auto RenameAction = menu->addAction (tr("修改备注"));
    connect (RenameAction, &QAction::triggered, [this] ()
    {
        emit rename();
    });

    mToggleLiveAction = menu->addAction(mFr->getLiveStatusToggleString());
    connect (mToggleLiveAction, &QAction::triggered, [this](){
       emit toggleLive();
    });

    menu->addSeparator();

    auto RemoveAction = menu->addAction (tr("删除好友"));
    connect (RemoveAction, &QAction::triggered, [this]()
    {
        emit removed(this);
    });

    menu->exec(QCursor::pos());
}
