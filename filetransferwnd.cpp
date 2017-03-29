#include "filetransferwnd.h"
#include "ui_filetransferwnd.h"
#include <QTreeWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
itemWidget::itemWidget(QWidget *parent ):QWidget(parent)
{
    QLabel *fileName = new QLabel(fileName);
    fileName->setFixedWidth(60);
    fileName->setAlignment(Qt::AlignCenter);
    fileName->setObjectName("fileName");
    QProgressBar * prograss= new QProgressBar();
    prograss->setObjectName("prograss");
    prograss->setRange(0, 1000);
    QHBoxLayout *itemHLayout= new QHBoxLayout;
    itemHLayout->addSpacing(10);
    itemHLayout->addWidget(fileName);
    itemHLayout->addSpacing(15);
    itemHLayout->addWidget(prograss);
    itemHLayout->addSpacing(20);
    setLayout(itemHLayout);
    itemHLayout->setSpacing(0);
    itemHLayout->setMargin(0);
    prograss->setStyleSheet("color:#1151B5;");
}

void itemWidget::setFileName(QString fileName)
{
    QLabel *fileLable  = NULL;
    foreach (QObject *o, this->children()) {
        if(o->objectName() == "fileName")
            fileLable = (QLabel*) o;
    }
    if(NULL != fileLable)
        fileLable->setText(fileName);
}

void itemWidget::setPrograss(int value)
{
    //查找进度条
    QProgressBar *progressBar  = NULL;
    foreach (QObject *o, this->children()) {
        if(o->objectName() == "prograss")
            progressBar = (QProgressBar*) o;
    }
    //添加值
    if(NULL != progressBar)
        if(value != -1)
        {
            progressBar->setValue(value);
        }else{
            progressBar->setStyleSheet("background-color:red");
        }
}

int itemWidget::prograssValue()
{
    int value = -1;
    QProgressBar *progressBar  = NULL;
    foreach (QObject *o, this->children()) {
        if(o->objectName() == "prograss")
            progressBar = (QProgressBar*) o;
    }
    if(NULL != progressBar)
        value =  progressBar->value();

    return value;
}

itemWidget::~itemWidget()
{
    foreach (QObject *o, this->children()) {
        if(o->objectName() == "prograss")
        {
            QProgressBar *progressBar  =(QProgressBar *)o;
            delete progressBar;
        }else if(o->objectName() == "fileName")
        {
            QLabel *fileName  =(QLabel *)o;
            delete fileName;
        }
    }

}


fileTransferWnd::fileTransferWnd(QWidget *parent) :
    FramelessModalMovableShadowWidget(parent),
    ui(new Ui::fileTransferWnd)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
    connect(ui->pTitleBarWgt, SIGNAL(tbClose()), this, SLOT(close()) );
    ui->listWidget->setStyleSheet("border:0px;");
}

fileTransferWnd::~fileTransferWnd()
{
    //删除
    for(int row = ui->listWidget->count(); row >0; row--)
    {
        QListWidgetItem *childItem  = ui->listWidget->item(row);
        int id = childItem->data(Qt::UserRole+1).toInt();
        m_transList.remove(id);
        ui->listWidget->removeItemWidget(childItem);
        delete childItem;
        delete m_transList.value(id);
    }

    delete ui;
}

void fileTransferWnd::addTransferItem(QString fileName, int id, bool upload)
{
    //删除一项
    for(int row = 0; row < ui->listWidget->count(); row++)
    {
        QListWidgetItem *childItem  = ui->listWidget->item(row);
        int id = childItem->data(Qt::UserRole+1).toInt();
        if(m_transList.value(id)->prograssValue() == 1000)
        {
            m_transList.remove(id);
            ui->listWidget->removeItemWidget(childItem);
            delete childItem;
            delete m_transList.value(id);
        }
    }

    //创建新的列表项
    QListWidgetItem *item = new QListWidgetItem();
    itemWidget *w = new itemWidget;
    w->setObjectName("TransferBar");
    w->setFileName(fileName);
    w->setPrograss(1000);
    ui->listWidget->addItem(item);

    //切换图标
    if(upload)
    {
        QIcon icon(":/Icon/file.png");
        item->setIcon(icon);
    }else{
        QIcon icon(":/Icon/file.png");
        item->setIcon(icon);
    }
    ui->listWidget->setItemWidget(item, w);
    w->show();
    m_transList.insert(id, w);
    //保存用户id
    item->setData(Qt::UserRole+1, QVariant(id));
}


void fileTransferWnd::transferPercent(int id, QString percent, QMap<QString, QString> fileInfo)
{
    //如果找不到id就创建新的文件传输项
    if(!m_transList.keys().contains(id)){
        addTransferItem(fileInfo.value("fileName"), id);
    }

    itemWidget *item = Q_NULLPTR;
    item = m_transList.value(id);

    //进度操作
    if(item != Q_NULLPTR)
    {
        item->setPrograss(percent.toDouble());
    }
}
