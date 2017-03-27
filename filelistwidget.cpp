#include "filelistwidget.h"
#include "myfileiconprovider.h"
#include "searchbox.h"
#include "config.h"
#include "folder.h"
#include "teacher.h"
#include "term.h"
#include "lesson.h"
#include "filenumwidget.h"
#include "messagedisplaywidget.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QProcess>
#include <QStyleOption>
#include <QPainter>
#include <QHeaderView>
#include <QFileIconProvider>
#include <qgraphicseffect.h>
#include <QStandardItem>
#include <qbitmap.h>
#include <qlabel.h>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressBar>
#include "coursetreewidget.h"
#include "classinfowidget.h"
#include "treedisplaydelegate.h"
#include "baseshadowwidget.h"
#include "resfilesdb.h"
#include "downloadfile.h"
#include "dataclassinstancemanage.h"
#include "folder.h"
#include "uploadfiles2ftp.h"

//#pragma execution_character_set("utf-8")

const int RESOURCE_ABSOLUTELY_PATH_ROLE = Qt::UserRole + 1000;
//const int RESOURCE_FILE_ROLE = Qt::UserRole + 1001;

FileListWidget::FileListWidget(QWidget *parent) : QWidget(parent)
{
    setup ();

    QVBoxLayout *mainLayout = new QVBoxLayout (this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QLabel *pTitleLb = new QLabel;
    pTitleLb->setFixedHeight(26);
//    QFont font = qApp->font();
//    font.setPointSize(14);
//    pTitleLb->setFont(font);
    pTitleLb->setText(tr("<p style = \"margin:5px;\">课程信息</p>"));
    pTitleLb->setStyleSheet("background-color:#f9f9f9;\
                             font-family: \"Microsoft YaHei\";\
                             font-size:10pt;");
    mainLayout->addWidget(pTitleLb);


    //右侧显示课程信息的文本框
    //标题栏.

    //ClassInfoWidget *m_classInfoWidget = new ClassInfoWidget(this);
    contentTextEdit = new QTextEdit;
//    pTitleWgtLayout->addWidget(contentTextEdit);
//    font.setPointSize(12);
    contentTextEdit->setStyleSheet("font-family: \"Microsoft YaHei\";\
                                    font-size:11pt;");
    contentTextEdit->setStyleSheet("color:#959595");
    contentTextEdit->setAlignment(Qt::AlignLeft | Qt::AlignHCenter);
    //去掉边框
    contentTextEdit->setFrameStyle(QFrame::NoFrame);
    contentTextEdit->setReadOnly(true);
    contentTextEdit->setFixedHeight(70);
    //使编辑框内的文字无法被选中
//    contentTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);

//    contentTextEdit->setAttribute (Qt::WA_TranslucentBackground);

//    //设置阴影
//    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect(this);
//    //阴影偏移
//    shadow_effect->setOffset(0, 5);
//    //阴影颜色 silver gainsboro ghostwhite
//    shadow_effect->setColor(QColor("#E8E8E8"));
//    //阴影模糊度
//    shadow_effect->setBlurRadius(10);
//    contentTextEdit->setGraphicsEffect(shadow_effect);

//    contentTextEdit->setFixedHeight(80);
    //设置QTextEdit控件的透明度
    //    QGraphicsOpacityEffect *e3 = new QGraphicsOpacityEffect(this);
    //    e3->setOpacity(0.7);
    //    contentTextEdit->setGraphicsEffect(e3);


    //mainLayout->addWidget(m_classInfoWidget);
    mainLayout->addWidget (contentTextEdit);
    mainLayout->addSpacing(0);

    QFrame *pHLine = new QFrame;
    pHLine->setFrameShape(QFrame::HLine);
    pHLine->setFrameShadow(QFrame::Plain);
    pHLine->setFixedHeight(1);
    pHLine->setStyleSheet("border: 1px solid #e9e9e9");
    mainLayout->addWidget(pHLine);
    mainLayout->addSpacing(0);

    m_pResIconLb = new QLabel;
    m_pResIconLb->setPixmap(QPixmap(":/image/0001.png"));
//    pPathIconLb->setStyleSheet("background-color:#f9f9f9");
    m_pResIconLb->setFixedSize(30, 30);
    m_pResIconLb->installEventFilter(this);

    m_pResTextLb = new QLabel;
    m_pResTextLb->setFixedHeight(30);
    m_pResTextLb->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_pResTextLb->setStyleSheet("font-family: \"Microsoft YaHei\";\
                        font-size:10pt;");
    m_pResTextLb->setText(tr("课程资源"));
    m_pResTextLb->installEventFilter(this);

    QHBoxLayout *pPathLayout = new QHBoxLayout;
    pPathLayout->setContentsMargins(5, 5, 5, 5);
    pPathLayout->setSpacing(5);
    pPathLayout->addWidget(m_pResIconLb);
    pPathLayout->addWidget(m_pResTextLb);
    mainLayout->addLayout(pPathLayout);

    QFrame *pHLine1 = new QFrame;
    pHLine1->setFrameShape(QFrame::HLine);
    pHLine1->setFrameShadow(QFrame::Plain);
    pHLine1->setFixedHeight(1);
    pHLine1->setStyleSheet("border: 1px solid #e9e9e9");
    mainLayout->addWidget(pHLine1);
    mainLayout->addSpacing(0);

//    QFrame *pHLine = new QFrame;
//    pHLine->setFrameShape(QFrame::HLine);
//    pHLine->setFixedHeight(1);
//    pHLine->setFrameShadow(QFrame::Sunken);
//    mainLayout->addSpacing(0);
//    mainLayout->addWidget(pHLine);
//    mainLayout->addSpacing(20);
    //搜索栏
    //mainLayout->addWidget (searchBox, 1);
    //文件列表
    mainLayout->addWidget(fileTreeWidget, 10);
//    mainLayout->setSpacing(20);

    QFrame *pHLine2 = new QFrame;
    pHLine2->setFrameShape(QFrame::HLine);
    pHLine2->setFrameShadow(QFrame::Plain);
    pHLine2->setFixedHeight(1);
    pHLine2->setStyleSheet("border: 1px solid #e9e9e9");
    mainLayout->addWidget(pHLine2);
    mainLayout->addSpacing(0);

    m_pProgressBar = new QProgressBar(this);
    m_pProgressBar->setMinimum(0);
    m_pProgressBar->setMaximum(0);
    m_pProgressBar->setFixedHeight(30);
    m_pProgressBar->hide();
    mainLayout->addWidget(m_pProgressBar);
    mainLayout->addSpacing(5);

    QHBoxLayout *pUpLayout = new QHBoxLayout;
    pUpLayout->setContentsMargins(5, 0, 0, 0);
    QLabel *pUpTextLb = new QLabel;
    pUpTextLb->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pUpTextLb->setText(tr("正在上传文件个数："));
    m_pUpCountLb = new QLabel;
    m_pUpCountLb->setText(QString::number(0));
    pUpLayout->addWidget(pUpTextLb);
    pUpLayout->addSpacing(5);
    pUpLayout->addWidget(m_pUpCountLb);

    mainLayout->addLayout(pUpLayout);
    mainLayout->addSpacing(5);

    QHBoxLayout *pDownLayout = new QHBoxLayout;
    pDownLayout->setContentsMargins(5, 0, 0, 0);
    QLabel *pDownTextLb = new QLabel;
    pDownTextLb->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    pDownTextLb->setText(tr("正在下载文件个数："));
    pDownLayout->addWidget(pDownTextLb);
    pDownLayout->addSpacing(5);
    m_pDownCountLb = new QLabel;
    m_pDownCountLb->setText(QString::number(0));
    pDownLayout->addWidget(m_pDownCountLb);
    mainLayout->addLayout(pDownLayout);
    mainLayout->addSpacing(5);

    TreeDisplayDelegate *pTreeItemDelegate = new TreeDisplayDelegate;
    fileTreeWidget->setItemDelegate(pTreeItemDelegate);
    //保证不被绘制上的部分透明
    fileTreeWidget->setAttribute (Qt::WA_TranslucentBackground);
    //设置表头
    //    QStringList header;
    //    header<<"ECJTU"<<"CERT";
    //    fileTreeWidget->setHeaderLabels(header);


    //设置根节点子节点之间的缩进距离
    fileTreeWidget->setIndentation(0);

    //去掉QTreeWidget被选中后会出现的虚线框
    fileTreeWidget->setFocusPolicy(Qt::NoFocus);

    //fileTreeWidget->setSelectionMode(QAbstractItemView::NoSelection);

    //设置fileTreeWidget  每一项的高度 鼠标扫过 按下的颜色 去掉虚线  "QTreeWidget::item{height:40px;} " "​​QTreeWidget{background-image:url(./image/treelistbk.jpg);} " "QTreeWidget::branch {image:none;}"
    fileTreeWidget->setRootIsDecorated( false ); //去掉虚线边框
    fileTreeWidget->setFrameStyle(QFrame::NoFrame); //去掉边框
    fileTreeWidget->setStyleSheet(
                "QTreeWidget::branch {image:none;} \
                QTreeWidget::item:hover{background-color:rgb(241,241,241)} \
                QTreeWidget::item:selected{background-color:rgb(241,241,241)} \
                QTreeWidget::item:pressed{backgro‌​und-color:rgb(241,241,241)} \
               ");

    fileTreeWidget->header ()->close ();

    //双击文件打开相应文件
    connect(fileTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this,SLOT(openFile(QTreeWidgetItem* ,int)));

    //单击展开关闭文件列表
    connect(fileTreeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)), this,SLOT(spreadList(QTreeWidgetItem* ,int)));

    //connect(upload_button,SIGNAL(clicked()), this,SLOT(uploadFile()));
    //connect(fileTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) , this,SLOT(a( QTreeWidgetItem * , QTreeWidgetItem *  )));


    setAttribute (Qt::WA_TranslucentBackground);   

    QFont titleLbFont = pTitleLb->font();
    qDebug() << "the font point size is:" << titleLbFont.pointSize()
             << "the font family is:" << titleLbFont.family();

    ResFilesDB *pResFileDB = DataClassInstanceManage::getInstance()->getResFilesDBPtr();
    connect(pResFileDB, SIGNAL(delFinish(const QJsonObject&)), this, SLOT(nwState(const QJsonObject&)));
}

FileListWidget::~FileListWidget()
{
     if(upLoad != Q_NULLPTR) {
         delete upLoad;
         upLoad = Q_NULLPTR;
     }

     if(dtTool != Q_NULLPTR) {
         delete dtTool;
         dtTool = Q_NULLPTR;
     }
}

void FileListWidget::openFile(QTreeWidgetItem* treeItem, int num)
{
    qDebug() << "openFile: " << treeItem->text(0) << " num: " << num;
    //QString path = fileMap[treeItem->text(0)];
    //QString path = nodeMap[treeItem]->fileInfo().absoluteFilePath();

//    QString path;
//    if(treeItem->text(0) != ""){
//        path = nodeMap[treeItem];
//    }
//    else{
//        qDebug()<<"ERROR in FileListWidget openFile"<<endl;
//        return;
//    }

//    QString path = nodeMap[treeItem];
    QString path = treeItem->data(num, RESOURCE_ABSOLUTELY_PATH_ROLE).toString();

    //QString date = Arg::currentLesson()->getDate().toString("yyyy-M-d");
    Lesson *pCurLesson = Arg::currentLesson();
    QString date = pCurLesson->getDate().toString("yyyy-M-d");
    QString section = QString::number(Arg::currentLesson()->getSection());
    QString courseFolder = date + "-" + section;

    qDebug() << "the open file path name is:" << date << section << courseFolder << pCurLesson->subject();
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);

    QString sFile;
    if(treeItem->text(0) == "课程课件")
    {
        sFile = dir.absolutePath() + "/SaveFile/" + courseFolder;
    }
    else
    {
        sFile = dir.absolutePath() + "/SaveFile/" + courseFolder + "/" + path.split("/").last();
    }


    sFile.replace("/","\\");
//    qDebug() << "file path: " << sFile << endl;

    QFile file(sFile);
    if(!file.exists()) {

#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("请先下载该文件"));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("请先下载该文件"));
#endif

        return;
    }

    QProcess::startDetached("explorer " + sFile);
    //QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(path).absoluteFilePath()));
}

//单击展开关闭文件夹
void FileListWidget::spreadList(QTreeWidgetItem* treeItem,int num)
{
    //        if(treeItem->isSelected()){
    //             treeItem->setBackground(0,QBrush(Qt::lightGray));
    //        }
    //        else{
    //            treeItem->setBackground(0,QBrush(Qt::white));
    //        }
    //    treeItem->setIcon(0,QIcon("./image/0002.png"));

    if(treeItem->isExpanded()){
        treeItem->setExpanded(false);
    }
    else{
        treeItem->setExpanded(true);
    }
}

void FileListWidget::updateContent(Folder* root, bool bMust)
{
    if((root == mLinkedFolder) && (!bMust)) {
        return;
    }

    qDebug() << "update content: " << root->size() << endl;

    link(root);

    if(root->size() > 0) {
        update ();
        fileTreeWidget->expandAll();
    }
}

void FileListWidget::handleUporDownloadFile(Lesson *pLesson)
{
    if(pLesson == Q_NULLPTR) {
        return;
    }

    //上传文件是否有.
    if(upLoad != Q_NULLPTR) {
        upLoad->handleUploadFiles(pLesson);
    }

    //下载文件是否有.
    if(dtTool != Q_NULLPTR) {
        dtTool->handleDownloadFiles(pLesson);
    }
}

void FileListWidget::showUpdateProgress(bool bVisible)
{
    ///Mark,为了尽快实现功能，这个函数就这么暂时实现.
    m_pUpCountLb->setText(QString::number(Arg::sUpLoadFileCount));

    if(bVisible) {
        if((Arg::sUpLoadFileCount > 0) || (Arg::sDownLoadFileCount > 0)) {
            m_pProgressBar->show();
        }
    }
    else {
        if((Arg::sDownLoadFileCount == 0) && (Arg::sUpLoadFileCount == 0)) {
            m_pProgressBar->hide();
        }
    }

}

//FileListWidget::~FileListWidget () = default;

void FileListWidget::setup ()
{
    searchBox = new SearchBox (this);
    connect (searchBox, SIGNAL (search (QString)), this, SLOT (search (QString)));

    fileTreeWidget = new QTreeWidget (this);

}

void FileListWidget::search (const QString& keyword)
{
    ///Mark 2017.03.24 by BiXiaoxia，这种就是多余，以后换成自己实现MODEL VIEW处理.
//    if (mLinkedFolder == nullptr)
//        return;
//    static auto curIt = name2Item.begin ();
//    if (lastSearchTarget.isEmpty () || keyword.isEmpty () || keyword != lastSearchTarget)
//    {
//        lastSearchTarget = keyword;
//        curIt = name2Item.begin ();
//    }
//    auto itBegin = curIt;
//    while (true)
//    {
//        bool found = false;
//        if (curIt.key ().contains (keyword))
//        {
//            fileTreeWidget->setCurrentItem (curIt.value ());
//            found = true;
//        }
//        ++curIt;
//        if (curIt == name2Item.end ())
//            curIt = name2Item.begin ();
//        if (found)
//            break;
//        if (curIt == itBegin)
//        {
//            curIt = name2Item.begin ();
//            break;
//        }
//    }
}

void FileListWidget::link (/*const*/ Folder* folder)
{
    mLinkedFolder = folder;

//    ///为了避免这里disconnect的崩溃，暂时这么处理.
//    if(m_allRootFolderList.contains(folder)) {
//        return;
//    }

//    m_curResPath.clear();

    if(folder != nullptr) {
        connect (folder, SIGNAL (changed ()), this, SLOT (update ()), Qt::UniqueConnection);
        connect(folder, SIGNAL(addFileComplete(File*)), this, SLOT(addFileComplete(File*)), Qt::UniqueConnection);
//        m_allRootFolderList.append(folder);
    }


    update();
//    if (mLinkedFolder != nullptr)
//        disconnect (mLinkedFolder, SIGNAL (changed ()), this, SLOT (update ()));
////    mLinkedFolder = folder;
//    if (folder != nullptr) {
//        connect (folder, SIGNAL (changed ()), this, SLOT (update ()));
//    }

//    mLinkedFolder = folder;
}

void FileListWidget::clearResourceDisplay()
{
//    ///Mark,暂时这么处理，重构不能每个课程带一个Folder，改为模型视图.
//    int nCount = fileTreeWidget->topLevelItemCount();

//    if(nCount > 0) {
//        QTreeWidgetItem *pParentItem = fileTreeWidget->topLevelItem(0);
//        int nChildCount = pParentItem->childCount();

//        for(int index = 0; index < nChildCount; ++index) {
//            pParentItem->removeChild(pParentItem->child(index));
//        }
//    }

    fileTreeWidget->clear();
//    m_curResPath.clear();
    mLinkedFolder = Q_NULLPTR;
}

void FileListWidget::update ()
{
    if(m_bUsedReset) {
        fileTreeWidget->clear ();
        name2Item.clear ();
        lastSearchTarget.clear ();
        m_curResPath.clear();
        //    fileTreeWidget->addTopLevelItem (getItemFromFolder (mLinkedFolder));
        //fileTreeWidget->addTopLevelItem (getItemFromFolder (mLinkedLesson->rootFolder()));
        getItemFromFolder(mLinkedFolder);
    }
}

//2016/12/14  修改
/*QTreeWidgetItem**/ void FileListWidget::getItemFromFolder (const Folder* folder)
{
    if(folder == Q_NULLPTR) {
        return;
    }

    if(m_provider == Q_NULLPTR) {
        m_provider = new MyFileIconProvider;
    }

//    QTreeWidgetItem* ret = new QTreeWidgetItem ({folder->name ()});
//    ///Mark,暂时这么处理吧.
//    ret->setSizeHint(0, QSize(10, 0));
//    ret->setIcon(0,QIcon("./image/0001.png"));
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);
    m_curResPath = /*Arg::configDir*/dir.absolutePath() + "/SaveFile/" + folder->name();
//    nodeMap.insert(ret, folderPath);
//    m_curResPath.clear();
//    m_curResPath.append(folderPath);
//    name2Item.insert (folder->name (), ret);
    int row = -1;
    for (const auto& f: folder->files ())
    {
        ++row;
        //QTreeWidgetItem* node = new QTreeWidgetItem (ret, {f->name ()});
        QTreeWidgetItem* node = new QTreeWidgetItem /*(ret)*/;
        QString fileName = {f->name()};

        //新建一个封装好的widget  包含一个label 两个button
        CourseTreeWidget* nodeWidget = new CourseTreeWidget(row,fileName, this);

        //设置子节点的高度
        node->setSizeHint(0,QSize(10, 29));

        //设置文件图标
        QIcon icon = m_provider->icon (f->fileInfo ());
        if(icon.isNull()) {
            icon.addFile(":/Icon/file.png");
        }

        node->setIcon(0, icon);
        fileTreeWidget->addTopLevelItem(node);
        nodeWidget->layout()->setContentsMargins(20, 0, 5, 0);
        fileTreeWidget->setItemWidget(node, 0, nodeWidget);
        //创建下载、删除按钮对应的槽函数
        connect(nodeWidget, &CourseTreeWidget::downloadOK, this, &FileListWidget::GF_downloadFile);
        connect(nodeWidget, &CourseTreeWidget::deleteOK, this, &FileListWidget::deleteFile);
        connect(nodeWidget, &CourseTreeWidget::uploadFile, this, &FileListWidget::uploadFile);
        QFileInfo fiInfo = f->fileInfo();
        QString sFilePathName = fiInfo.absoluteFilePath();
        node->setData(0, RESOURCE_ABSOLUTELY_PATH_ROLE, sFilePathName);

//        nodeMap.insert(node, f->fileInfo().absoluteFilePath());
//        ret->addChild (node);
//        name2Item.insert (f->name (), node);
    }

    ///Mark2017.02.27，这里暂时不要做这个处理了，这里跟设计上本来就是出问题了,重构再加入.
//    for (const auto& f: folder->subfolders ())
//    {
//        ret->addChild (getItemFromFolder (f));
//    }

//    return ret;
}

void FileListWidget::uploadFile(int nRow)
{
//    if(m_pProgressBar == Q_NULLPTR) {
//        m_pProgressBar = new QProgressBar(this);
//        m_pProgressBar->setMinimum(0);
//        m_pProgressBar->setMaximum(0);
//    }


//    ///Mark 2017.02.28，目前设计只有一层文件夹，所以暂时用topLevelItem，添加里也是topLevelItem.
//    QTreeWidgetItem *pItem = fileTreeWidget->topLevelItem(nRow);
//    QRect rect = fileTreeWidget->visualItemRect(pItem);
//    m_pProgressBar->resize(rect.width(), rect.height());
//    m_pProgressBar->move(fileTreeWidget->pos() + rect.topLeft());
//    m_pProgressBar->show();

    QTreeWidgetItem *pItem = fileTreeWidget->topLevelItem(nRow);
    qDebug()<<nRow<<endl;

 //   QString path = nodeMap[fileTreeWidget->currentItem()];
    QString path = pItem->data(0, RESOURCE_ABSOLUTELY_PATH_ROLE).toString();
    qDebug()<< path <<endl;

    if(upLoad == Q_NULLPTR) {
        upLoad = new UploadFiles2Ftp();
        connect(upLoad, SIGNAL(uploadFinished()), this, SLOT(uploadFinished()));
    }

    upLoad->setLesson(Arg::currentLesson());

    QFileInfo fiInfo(path);
    QString sDate = Arg::currentLesson()->getDate().toString("yyyy-M-d") +"-" + QString::number(Arg::currentLesson()->getSection());
    QString sUploadName = sDate + "/" + fiInfo.fileName();
    QString sFilePathName = fiInfo.absolutePath();

    if(!sFilePathName.contains(sDate)) {

         sFilePathName = sFilePathName + "/" + sUploadName;
    }
    else {
        sFilePathName = sFilePathName + "/" + fiInfo.fileName();
    }

    QFile upLoadFile(sFilePathName);
    if(!upLoadFile.exists()) {
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("本地文件不存在，请先下载该文件！"));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("本地文件不存在，请先下载该文件！"));
#endif

        return;
    }

    QMap<QString, QString> sourcefileMap;
    sourcefileMap.insert("fileName", sUploadName);
    sourcefileMap.insert("localFilePath",  sFilePathName);
    sourcefileMap.insert("detailID", QString::number(Arg::currentLesson()->getLessonDetailId()));
    sourcefileMap.insert("date", Arg::currentLesson()->getDate().toString("yyyy-M-d"));

    upLoad->up2ftp(sourcefileMap, false);

    showUpdateProgress(true);
}

void FileListWidget::uploadFinished()
{
    showUpdateProgress(false);
}

void FileListWidget::GF_downloadFile(int nRow)
{
//   if(m_pProgressBar == Q_NULLPTR) {
//       m_pProgressBar = new QProgressBar(this);
//       m_pProgressBar->setMinimum(0);
//       m_pProgressBar->setMaximum(0);
////       m_pProgressBar->setFixedSize(80, 60);
//   }

   ///Mark 2017.02.28，目前设计只有一层文件夹，所以暂时用topLevelItem，添加里也是topLevelItem.
   QTreeWidgetItem *pItem = fileTreeWidget->topLevelItem(nRow);
//   QRect rect = fileTreeWidget->visualItemRect(pItem);
//   m_pProgressBar->resize(rect.width(), rect.height());
//   m_pProgressBar->move(fileTreeWidget->pos() + rect.topLeft());
   m_pProgressBar->show();

   qDebug()<<nRow<<endl;

//   QString path = nodeMap[fileTreeWidget->currentItem()];
   QString path = pItem->data(0, RESOURCE_ABSOLUTELY_PATH_ROLE).toString();
   qDebug()<<path<<endl;

   if(dtTool == Q_NULLPTR) {
       dtTool = new downloadFile();
       connect(dtTool, SIGNAL(dowLoadComplete(const QString&)), this, SLOT(downLoadComplete(const QString&)));
   }

   QMap<QString, QString> args;
   Lesson *pLesson = Arg::currentLesson();
   args["date"] = pLesson->getDate().toString("yyyy-M-d");
   args["section"] = QString::number(Arg::currentLesson()->getSection());
   args["detailID"] = QString::number(Arg::currentLesson()->getLessonDetailId());
   args["folderName"] = args["date"] + "-" + args["section"];
   args["fileName"] = path.split("/").last();

//   ++Arg::sDownLoadFileCount;
   dtTool->download(args, pLesson);

   m_pDownCountLb->setText(QString::number(Arg::sDownLoadFileCount));

   qDebug() << "the download args is:" << args;
}

void FileListWidget::downLoadComplete(const QString &sError)
{
//    --Arg::sDownLoadFileCount;

    if((Arg::sDownLoadFileCount == 0) && (Arg::sUpLoadFileCount == 0)) {
        m_pProgressBar->hide();
    }

    m_pDownCountLb->setText(QString::number(Arg::sDownLoadFileCount));

    if(!sError.isEmpty()) {
        QString sText = sError;
        if(sError.contains(":")) {
            sText= sError.split(":").last();
        }
#ifdef USER_QT_MESSAGEBOX
        QMessageBox::information(this, tr("教师客户端"), tr("下载错误：%1").arg(sText));
#else
        MessageDisplayWidget::information(this, tr("教师客户端"), tr("下载错误：%1").arg(sText));
#endif
    }
}

void FileListWidget::deleteFile(int nRow)
{
    ///Mark 这个过程要改变.
    qDebug()<< "the related row" << nRow<<endl;
    //QString path = nodeMap[fileTreeWidget->itemAt(0, nRow)];
//    QString path = nodeMap[fileTreeWidget->currentItem()];
    QTreeWidgetItem *pCurItem = fileTreeWidget->topLevelItem(nRow);
    QWidget *pWgt = fileTreeWidget->itemWidget(pCurItem, 0);
    QString path = pCurItem->data(0, RESOURCE_ABSOLUTELY_PATH_ROLE).toString();
//    File *pFile = pCurItem->data(0, RESOURCE_FILE_ROLE).toInt();

//  qDebug()<<path<<endl;

    QString date = Arg::currentLesson()->getDate().toString("yyyy-M-d");
    //QString date = mLinkedLesson->getDate().toString("yyyy-M-d");
    QString section = QString::number(Arg::currentLesson()->getSection());
    QString courseFolder = date + "-" + section;

    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);
    QString file = /*Arg::configDir*/dir.absolutePath() + "/SaveFile/" + courseFolder + "/" + path.split("/").last();
    file.replace("/","\\");
    qDebug()<<"del file: "<<file<<endl;

    QFile::remove(file);
//    if(!bRemoveStatus) {
//        QMessageBox::information(this, tr("删除文件"), tr("删除失败，请重试"));

//        return;
//    }

    //先调整后面的itemWidget的index.
    int nRowCount = fileTreeWidget->topLevelItemCount();
    for(int nIndex = nRow + 1; nIndex < nRowCount; ++nIndex) {
        QTreeWidgetItem *pItem = fileTreeWidget->topLevelItem(nIndex);
        CourseTreeWidget *pCourseTreeWgt = dynamic_cast<CourseTreeWidget*>(fileTreeWidget->itemWidget(pItem, 0));
        pCourseTreeWgt->setRelatedRow(nIndex - 1);

        qDebug() << "move the " << nIndex << "item" << pItem;
    }


    QString fp = "./SaveFile/" + courseFolder + "/" + path.split("/").last();
    qDebug()<<"fp: "<<fp<<endl;
    //mLinkedFolder->remove(fp);


    //-------------------删除资源文件数据库记录-----------------------//
    ///Delete 2017.03.24.
//    QPair<int, QString> course;
//    course.first = Arg::currentLesson()->getLessonDetailId();
//    course.second = date;
//    QString sCoureName = path.split("/").last();
//    int fileid = Arg::fileIdMap[course][sCoureName];
    ///End.
//    ResFilesDB resdb;
//    resdb.setId(fileid);
//    connect(&resdb, SIGNAL(delFinish(QJsonObject)), this, SLOT(nwState(QJsonObject)));
//    resdb.del();
    File* uifile = mLinkedLesson->name2File[path.split("/").last()];

    if(uifile != Q_NULLPTR) {
        ResFilesDB *pResFileDB = DataClassInstanceManage::getInstance()->getResFilesDBPtr();
        pResFileDB->setId(uifile->getFileID());
        pResFileDB->del();
    }


//    qDebug()<<"del filid: "<< nFileID <<endl;

    //---------------------删除界面元素------------------------//
    //File* uifile = new File(fp, mLinkedLesson->rootFolder());
//    File* uifile = mLinkedLesson->name2File[path.split("/").last()];
//    qDebug()<<"uifile pointer in remmove: "<<uifile<<endl;
//    qDebug()<<"linked lesson before remove: "<<mLinkedLesson->rootFolder()->size(false)<<endl;

    if(uifile != nullptr)
    {
        m_bUsedReset = false;

        mLinkedLesson->rootFolder()->remove(uifile);

//        qDebug()<<"linked lesson after remove: "<<mLinkedLesson->rootFolder()->size(false)<<endl;

        //qDebug()<<"linked folder before remove: "<<mLinkedFolder->size(false)<<endl;
        //mLinkedFolder->remove(uifile);
        //qDebug()<<"linked folder after remove: "<<mLinkedFolder->size(false)<<endl;

        if(mLinkedLesson->lesFileNumWidget != nullptr)
        {
            mLinkedLesson->lesFileNumWidget->delFile();

            if(mLinkedLesson->lesFileNumWidget->getFileNum() == 0) {
                mLinkedLesson->lesFileNumWidget->setDisplayFileNumEnabled(false);
            }

            mLinkedLesson->lesFileNumWidget->update();
        }

//        QWidget *pWgt = fileTreeWidget->itemWidget(pCurItem, 0);
        qDebug() << "the widget of take item00" << pWgt;
        delete pWgt;
        pWgt = Q_NULLPTR;
        ///Mark，暂时这么处理，都是topLevel层级的.
        fileTreeWidget->takeTopLevelItem(nRow);

        delete pCurItem;
        pCurItem = Q_NULLPTR;

//        qDebug() << "the widget of take item" << pWgt;
//        update();


        //---------------------删除Map元素------------------------//
        mLinkedLesson->name2File.remove(path.split("/").last());
//        delete uifile;

        m_bUsedReset = true;
    }
}

void FileListWidget::nwState(const QJsonObject &json)
{
    qDebug()<<"network result: "<<json<<endl;
}

void FileListWidget::addFileComplete(File *file)
{
    m_bUsedReset = false;

    if(m_provider == Q_NULLPTR) {
        m_provider = new MyFileIconProvider;
    }

//    QString folderPath = Arg::configDir.absolutePath() + "/SaveFile/" + folder->name();
//    m_curResPath.clear();
//    m_curResPath.append(folderPath);

    QTreeWidgetItem* node = new QTreeWidgetItem /*(ret)*/;
    QString fileName = {file->name()};

    //新建一个封装好的widget  包含一个label 两个button
    CourseTreeWidget* nodeWidget = new CourseTreeWidget(fileTreeWidget->topLevelItemCount(), fileName, this);

    //设置子节点的高度
    node->setSizeHint(0,QSize(10, 29));

    QFileInfo fiInfo = file->fileInfo();

    //设置文件图标
    QIcon icon = m_provider->icon(fiInfo);
    if(icon.isNull()) {
        icon.addFile(":/Icon/file.png");
    }

    node->setIcon(0, icon);
    fileTreeWidget->addTopLevelItem(node);
    nodeWidget->layout()->setContentsMargins(20, 0, 5, 0);
    fileTreeWidget->setItemWidget(node, 0, nodeWidget);
    //创建下载、删除按钮对应的槽函数
    connect(nodeWidget, &CourseTreeWidget::downloadOK, this, &FileListWidget::GF_downloadFile);
    connect(nodeWidget, &CourseTreeWidget::deleteOK, this, &FileListWidget::deleteFile);
    connect(nodeWidget, &CourseTreeWidget::uploadFile, this, &FileListWidget::uploadFile);
    QString sFilePathName = fiInfo.absoluteFilePath();
    node->setData(0, RESOURCE_ABSOLUTELY_PATH_ROLE, sFilePathName);

//    name2Item.insert(file->name (), node);
    m_bUsedReset = true;

    fileTreeWidget->expandItem(node);
}

//修改右侧显示课程信息文本框的内容
void FileListWidget::updatecontentTextEdit(const QMap<QString, QString> &lessonMap)
{
    //ClassInfoWidget *m_classInfoWidget = new ClassInfoWidget(this);
    //mainLayout->addWidget(m_classInfoWidget);
    contentTextEdit->clear();

    contentTextEdit->setText(" ");
    QString sLessDate = lessonMap["date"] /* + "  " + lessonMap["subject"]*/;
    QString sLessUnit = lessonMap["unit"];
    QString sLessName = lessonMap["subject"];

//    contentTextEdit->append("   "+lessonMap["date"]+"  "+lessonMap["subject"]);
//    contentTextEdit->append("   "+lessonMap["unit"]+"  "+lessonMap["subject"]);
    ///Replace.
    QString sHtmlText = QString("<p style = \"margin:5px; line-height:30px;\" ><font color=\"#959595\">%1 </font></p>\
                                <p style = \"margin:5px;\"><font color=\"#959595\">%2 </font> <font color=\"#959595\">&nbsp;&nbsp;&nbsp;%3 </font></p>")
            .arg(sLessDate).arg(sLessUnit).arg(sLessName);
//    QString sHtmlText = QString("%1 /n %2  %3").arg(sLessDate).arg(sLessUnit).arg(sLessName);
    contentTextEdit->setText(sHtmlText);

    //contentTextEdit->setFontPointSize(20);//设置字号
    //contentTextEdit->setTextColor(Qt::red);//设置字体颜色


//    contentTextEdit->insertPlainText(lessonMap["date"]);
//    contentTextEdit->insertPlainText("  ");
//    contentTextEdit->insertPlainText(lessonMap["subject"]);
//    //换行
//    QString temp="";
//    contentTextEdit->insertPlainText(temp+='\n');

//    contentTextEdit->insertPlainText(lessonMap["unit"]);
//    contentTextEdit->insertPlainText("  ");
//    contentTextEdit->insertPlainText(lessonMap["location"]);



}

void FileListWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

bool FileListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if((event->type() == QEvent::MouseButtonDblClick) && ((watched == m_pResIconLb) || (watched == m_pResTextLb))) {
        if(mLinkedLesson != nullptr) {
            if(!m_curResPath.isEmpty()) {
                Lesson *pCurLesson = Arg::currentLesson();
                QString date = pCurLesson->getDate().toString("yyyy-M-d");
                QString section = QString::number(Arg::currentLesson()->getSection());
                QString courseFolder = date + "-" + section;

                Arg *pArg = Arg::getInstance();
                QDir dir;
                pArg->getSaveDir(dir);
                QString file = /*Arg::configDir*/dir.absolutePath() + "/SaveFile/" + courseFolder;

                file.replace("/","\\");
                qDebug() << "file path: " << file << endl;

                QDir folderDir(file);
                if(!folderDir.exists()) {
                   folderDir.mkpath(file);
                }

                QProcess::startDetached("explorer " + file);
            }

            return true;
        }
    }

    return QWidget::eventFilter(watched, event);
}
