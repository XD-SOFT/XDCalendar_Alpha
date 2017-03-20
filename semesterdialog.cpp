#include "semesterdialog.h"
#include "titlebar.h"
#include "config.h"
#include "editsem.h"
#include "semester.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

SemesterDialog::SemesterDialog(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent)
{
    qDebug()<<"come in semesterDialog"<<endl;
//    setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    setWindowModality (Qt::WindowModal);
//    setAttribute (Qt::WA_DeleteOnClose);

    //this->setStyle("cleanlooks");

    int nShadowWidth = getShadowMargin();
    auto layout = new QVBoxLayout (this);
    layout->setContentsMargins (nShadowWidth, nShadowWidth, nShadowWidth, nShadowWidth);

    auto titleBar = new TitleBar (QStringLiteral ("学期设置"), this);
    connect (titleBar, SIGNAL(tbClose()), this, SLOT(closeSemesterDialog()));
    layout->addWidget (titleBar);

    contentLayout = new QVBoxLayout ();
    contentLayout->setContentsMargins (9, 6, 6, 6);
    {
        contentLayout->addWidget(new QLabel(QStringLiteral("<font pointsize=10 color=#666666>当前学期:</font>"),this),5);
        if(Arg::currentSemester() != nullptr)
        {
            contentLayout->addWidget(new QLabel(Arg::currentSemester()->getTitle(),this),5);
        }
        else
        {
            contentLayout->addWidget(new QLabel(tr("未设置"),this),5);
        }

        contentLayout->addSpacing(10);
        auto label1 = new QLabel(QStringLiteral ("<font pointsize=10 color=#666666>我保存的学期:</font>"), this);
        contentLayout->addWidget (label1, 5);

        contentLayout->addStretch();

        semList = new QListWidget (this);
        contentLayout->addWidget(semList);

        if(Arg::semesters() != nullptr)
        {
            if(!Arg::semesters()->isEmpty())
            {
                reloadSemester();
            }
        }
        qDebug()<<"here?"<<endl;

        qDebug()<<"88888"<<endl;
        auto buttonLayout = new QHBoxLayout();
        {
            buttonLayout->addStretch(1);
            ButtonAdd = new QPushButton(QObject::tr("新增"));
            ButtonAdd->setMinimumHeight(20);
            ButtonAdd->setFixedWidth(80);
            connect (ButtonAdd, SIGNAL(clicked(bool)), this, SLOT(addSemester()));
//            ButtonAdd->setStyleSheet("QPushButton{background:#1250b5; \
//                                     border:1px solid #1250b5;\
//                                     color:white}");//设置按钮背景、边框颜色.

            ButtonEdit = new QPushButton(QObject::tr("编辑"));
            ButtonEdit->setMinimumHeight(20);
            ButtonEdit->setFixedWidth(80);
            connect (ButtonEdit, SIGNAL(clicked(bool)), this, SLOT(editSemester()));
//            ButtonEdit->setStyleSheet("QPushButton{background:#1250b5; \
//                                      border:1px solid #1250b5;\
//                                      color:white}");//设置按钮背景、边框颜色.

            buttonLayout->addWidget(ButtonAdd);
            buttonLayout->addSpacing(10);
            buttonLayout->addWidget(ButtonEdit);
            buttonLayout->addStretch(1);
        }

        contentLayout->addLayout(buttonLayout);
    }
    qDebug()<<"hereeeee"<<endl;
    layout->addLayout (contentLayout);

    setFixedSize(400, 300);
}

void SemesterDialog::reloadSemester()
{
    semList->clear();
    semMap.clear();
    for(Semester *s : *(Arg::semesters())){
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setSizeHint(QSize(semList->width(), 25));
        QString sTitle = s->getTitle();
        pItem->setText(sTitle);
        semList->addItem(pItem);
        semMap[sTitle] = s;
    }
}


void SemesterDialog::editSemester()
{
    QListWidgetItem* currentSem;
    if(semList->currentItem() != nullptr)
    {
        currentSem = semList->currentItem();
    }
    else
    {
        currentSem = semList->item(0);
    }
    Arg::setCurrentSemester(semMap[currentSem->text()]);
    qDebug()<<"current item: "<<currentSem->text()<<endl;
    qDebug()<<"sem name: "<<semMap[currentSem->text()]->getTitle()<<endl;
    editSem = new EditSem(this, semMap[currentSem->text()]);
    connect(editSem, SIGNAL(updateParent()), this, SLOT(reloadSemester()));
    editSem->show();
}

void SemesterDialog::addSemester()
{
    editSem = new EditSem(this, nullptr);
    connect(editSem, SIGNAL(updateParent()), this, SLOT(reloadSemester()));
    editSem->show();
}

void SemesterDialog::closeSemesterDialog()
{
    this->close ();
}


SemesterDialog::~SemesterDialog()
{
}
