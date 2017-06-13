#include "notewidget.h"
#include "lesson.h"
#include "term.h"
#include "inputtextwidget.h"
#include "dataclassinstancemanage.h"
#include "lessonremarkdb.h"
#include "messagedisplaywidget.h"
#include <QTextEdit>
#include <QPushButton>
#include <QPoint>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSizePolicy>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QProcess>
#include <QPolygon>
#include <QStyleOption>
#include <QGraphicsDropShadowEffect>
#include <QPrinter>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QApplication>
#include <QProcess>

extern void drawShadow(QPainter &_painter, qint16 _margin, qreal _radius, QColor _start, QColor _end, qreal _startPosition, qreal _endPosition0, qreal _endPosition1, qreal _width, qreal _height);
NoteWidget* NoteWidget::instance = nullptr;
int NoteWidget::m_nTopMargin = 0;

NoteWidget::NoteWidget(const QRegion& region, QWidget *parent) : BaseShadowWidget(parent), mExternRegion (region)
{
    setShadowWidth(30);
    mLabel = new QLabel (this);
    mLabel->setText("test");
    mCancel = new QPushButton (tr("取消备注"));
    mCancel->setObjectName (QStringLiteral ("Cancel"));
    mEdit = new QTextEdit (this);
    mEdit->setPlaceholderText (tr("点击输入备注"));
    mEdit->setFrameShape(QFrame::StyledPanel);

    QPushButton *pCreatePdfBtn = new QPushButton (this);
    pCreatePdfBtn->setFixedSize(22, 22);
    pCreatePdfBtn->clearFocus();
    pCreatePdfBtn->setFlat(true);
    pCreatePdfBtn->setStyleSheet("QPushButton{border-image: url(:/Icon/ppt1)} \
                                  QPushButton:pressed{border-image: url(:/Icon/ppt2)} \
                                  QPushButton:hover{border-image: url(:/Icon/ppt2)}");
//    pCreatePdfBtn->setIcon(QIcon(":/Icon/ppt1"));
//    connect(pCreatePdfBtn, &QPushButton::clicked, this, &NoteWidget::createPdf);
    connect(pCreatePdfBtn, &QPushButton::clicked, this, &NoteWidget::createPPT);
    mTools.push_back (pCreatePdfBtn);

    QPushButton *pCreateDocBtn = new QPushButton (this);
    pCreateDocBtn->setFixedSize(22, 22);
    pCreateDocBtn->clearFocus();
    pCreateDocBtn->setFlat(true);
    pCreateDocBtn->setStyleSheet("QPushButton{border-image: url(:/Icon/word1)} \
                                  QPushButton:pressed{border-image: url(:/Icon/word2)} \
                                  QPushButton:hover{border-image: url(:/Icon/word2)}");
    connect(pCreateDocBtn, &QPushButton::clicked, this, &NoteWidget::createDoc);
    mTools.push_back (pCreateDocBtn);
//    connect (drawingPad, &QPushButton::clicked, [this] () {
//        QProcess* process = new QProcess (this);
//        process->start ("./utility/Pencil2D.exe");
//    });
//    pCreateDocBtn->setIcon (QIcon(":/Icon/word1"));

    QPushButton *pRecordClassBtn = new QPushButton(this);
    pRecordClassBtn->setFixedSize(22, 22);
    pRecordClassBtn->clearFocus();
    pRecordClassBtn->setFlat(true);
    pRecordClassBtn->setStyleSheet("QPushButton{border-image: url(:/Icon/yb1)} \
                                   QPushButton:pressed{border-image: url(:/Icon/yb2)} \
                                   QPushButton:hover{border-image: url(:/Icon/yb2)}");

    connect(pRecordClassBtn, &QPushButton::clicked, this, &NoteWidget::recordClass);
    mTools.push_back (pRecordClassBtn);
    //    auto p = new QPushButton (this);
//    p->setIcon (QIcon(":/Icon/Standard/01_74.png"));
//    mTools.push_back (p);
//    for (const auto& t: mTools)
//    {
//        t->setObjectName (QStringLiteral ("Tool"));
//    }

    QHBoxLayout* topLine = new QHBoxLayout ();
    topLine->setContentsMargins(5, 0, 0, 0);
    topLine->addWidget (mLabel);
    topLine->addWidget (mCancel);
    QHBoxLayout* bottomLine = new QHBoxLayout ();
    bottomLine->setContentsMargins(0, 0, 0, 5);
    bottomLine->addStretch (100);
    for (const auto& w: mTools) {
        bottomLine->addWidget(w);
        bottomLine->addSpacing(6);
    }

    QVBoxLayout* layout = new QVBoxLayout (this);
//    layout->addSpacing (8);
    layout->setContentsMargins(5, 0, 0, 5);
    layout->setSpacing (0);
    layout->addLayout (topLine);
    layout->addWidget (mEdit);
    layout->addLayout (bottomLine);

    setWindowFlags (Qt::Tool | Qt::FramelessWindowHint);
    setAttribute (Qt::WA_TranslucentBackground);
//    setAttribute (Qt::WA_DeleteOnClose);
    setFocus ();
    setMouseTracking (true);

    ///Edit by BiXiaoxia 2016.12.21,适应阴影.
    setFixedSize (310, 210);
    m_nTopMargin = getShadowMargin();
    layout->setContentsMargins(m_nTopMargin, m_nTopMargin, m_nTopMargin, m_nTopMargin);
    setLayout(layout);

    connect(mCancel, &QPushButton::clicked, this, &NoteWidget::cancelRemark);

//    //连接备注数据库.
//    LessonRemarkDB *pLesDB = DataClassInstanceManage::getInstance()->getLessonRemarkDBPtr();
//    connect(pLesDB, &LessonRemarkDB::findFinish, this, &NoteWidget::setLessonRemark);
}

NoteWidget::~NoteWidget()
{
//    if(lesDB != Q_NULLPTR) {
//        delete lesDB;
//        lesDB = Q_NULLPTR;
//    }
}

void NoteWidget::cancelRemark()
{
    mLinkedLesson->setNote ("");
    mEdit->clear();

    LessonRemarkDB *pLesDB = DataClassInstanceManage::getInstance()->getLessonRemarkDBPtr();
    pLesDB->del();
}

void NoteWidget::link (Lesson* lesson)
{
    mLinkedLesson = lesson;

    if (mLinkedLesson == nullptr)
        return;

//    if(!mLinkedLesson->noteLoaded()) {
//        LessonRemarkDB *pLesDB = DataClassInstanceManage::getInstance()->getLessonRemarkDBPtr();
//        pLesDB->setLessonDetailId(mLinkedLesson->getLessonDetailId());
//        pLesDB->setTeachingDate(mLinkedLesson->getDate());
//        m_reqNoteLessonQueue.enqueue(mLinkedLesson);
//        pLesDB->find();

//        qDebug() << "the lesson tecahing date is:" << mLinkedLesson->getDate();
//    }
//    else {
    mEdit->setText(mLinkedLesson->note());
    LessonRemarkDB *pLesDB = DataClassInstanceManage::getInstance()->getLessonRemarkDBPtr();
    pLesDB->setLessonDetailId(mLinkedLesson->getLessonDetailId());
    pLesDB->setTeachingDate(mLinkedLesson->getDate());
//    }

    reload();
//    auto listener = [&] () {mLinkedLesson->setNote (mEdit->toPlainText ());};
//    auto cancel = [&] () {mLinkedLesson->setNote (""); /*this->close ();*/
//    mEdit->clear();};

//    if (mLinkedLesson != nullptr)
//    {
//        disconnect (noteChanged);
//        disconnect (noteCancel);
//    }
//    mLinkedLesson = lesson;
//    if (mLinkedLesson != nullptr)
//    {
//        noteChanged = connect (mEdit, &QTextEdit::textChanged, listener);
//        noteCancel = connect (mCancel, &QPushButton::clicked, cancel);
//    }
}

//void NoteWidget::enterEvent(QEvent *event)
//{

//    QWidget::enterEvent(event);
//}

void NoteWidget::reload ()
{
//    lastNote = "";
    if (mLinkedLesson == nullptr)
        return;

//    mEdit->setText (mLinkedLesson->note ());
//    lastNote = mLinkedLesson->note();
    ///TODO.添加时间段.
    /// 感觉前后不一致，添加lesson section是从0开始，这边成了-1.
//    Term::Section sec = Arg::currentTerm()->getSections().at (mLinkedLesson->section ());
//    Term::Section sec = Arg::currentTerm()->getSections().at (mLinkedLesson->section () - 1);
    //课程开始时间.
//    int nStartHours = sec.first / 3600;
//    int nRemainTime = sec.first % 3600;
//    int nStartMinutes = nRemainTime / 60;
////    int nStartSeconds = nRemainTime % 60; //上课一般不用秒，所以去掉处理.

//    //课程结束时间.
//    int nEndHours =  sec.second / 3600;
//    nRemainTime = sec.second % 3600;
//    int nEndMinutes = nRemainTime / 60;
////    int nEndSeconds = nRemainTime % 60;
    QString sStartTime;
    mLinkedLesson->getStartTime(sStartTime);
    QString sEndTime;
    mLinkedLesson->getEndTime(sEndTime);

    QString sTime = QString("%1 - %2").arg(sStartTime).arg(sEndTime);
    mLabel->setText(sTime);
}

//void NoteWidget::setLessonRemark(const QString &sRemark)
//{
//    Lesson *lesson = m_reqNoteLessonQueue.dequeue();

//    //延迟返回的前面的请求，给设置到原来的lesson上，避免乱序.
//    if(lesson != mLinkedLesson) {
//        lesson->setNote(sRemark);

//        return;
//    }

//    //为当前请求的备注.
//    mLinkedLesson->setNote(sRemark);
//    mEdit->clear();
//    mEdit->setText(sRemark);
//}

NoteWidget* NoteWidget::makeInstance (Lesson* lesson, const QRegion& region, const QPoint& pos)
{
    ///Mark by BiXiaoxia 2017.01.12, 这里有问题，通过leaveEvent跟focusOutEvent去销毁并不是所有销毁情况，比如这个这个实例显示了，但是光标并没有到这个实例上来，会出现多个实例并存，有的不会再销毁.
    if (instance == nullptr) {
        instance = new NoteWidget (region, nullptr);
    }

    instance->m_bRemarkHandled = false;

    if (lesson != instance->lesson ())
    {
        instance->link (lesson);
//        instance->reload ();
        qDebug() << "the current lesson id is:" << lesson->getClassId() << lesson->getLessonDetailId();
    }

    //微调x值，使尖头对准中间.
    if (pos != instance->pos ()) {
         int nCenterX = pos.x();

         //减去阴影部分及尖头绘制像素30阴影， 尖头绘制.
         QPoint newPos(nCenterX - m_nTopMargin, pos.y() - m_nTopMargin + 2);
         instance->move(newPos);
    }

    if (!instance->isVisible ())
    {
        instance->show ();
    }

    return instance;
}

void NoteWidget::setLabel (const QString& str)
{
    mLabel->setText (str);
//    update ();
}

void NoteWidget::leaveEvent (QEvent* ev)
{
    if (mExternRegion.contains (QCursor::pos ()))
    {
        ev->ignore ();
        return;
    }

    handleLessonRemarkDB();

    /// Mark 2017.01.12,前面makeInstance做了说明，这里要改造，暂时改造使用,让其关闭.
//    instance = nullptr;
//    destroy ();
    emit noteWidgetClosed();
    close();
}


void NoteWidget::mousePressEvent (QMouseEvent* ev)
{
    QWidget::mousePressEvent (ev);
}

void NoteWidget::focusOutEvent (QFocusEvent* fv)
{
    if (fv->reason () == Qt::MouseFocusReason)
    {
        fv->ignore ();
        return;
    }
//    qDebug() << fv->reason ();

    handleLessonRemarkDB();

    /// Mark 2017.01.12,前面makeInstance做了说明，这里要改造，暂时改造使用，让其关闭.
//    instance = nullptr;
//    destroy ();

    emit noteWidgetClosed();
    close();
}

void NoteWidget::handleLessonRemarkDB()
{
//    if(lesDB == Q_NULLPTR) {
//        lesDB = new LessonRemarkDB();
//    }
    if((mLinkedLesson == nullptr) || m_bRemarkHandled) {
        return;
    }

    QString sLastRemark = mLinkedLesson->note();
    LessonRemarkDB *pLesDB = DataClassInstanceManage::getInstance()->getLessonRemarkDBPtr();
//    pLesDB->setLessonDetailId(mLinkedLesson->getLessonDetailId());
//    pLesDB->setTeachingDate(mLinkedLesson->getDate());
//    lesDB->setRemark(mEdit->toPlainText());

    QString sCurRemark = mEdit->toPlainText().trimmed();

    if(sLastRemark.compare(sCurRemark) == 0)
    {
        //do nothing
//        qDebug()<<"doing nothing for remark"<<endl;
    }
    else
    {
        //如果以前备注为空，则添加备注，否则更新备注.
        if(sLastRemark.compare(QString("")) == 0)
        {
            qDebug()<<"add a remark"<<endl;
            pLesDB->setRemark(sCurRemark);
            pLesDB->add();
        }
        else
        {
            qDebug()<<"update a remark"<<endl;
            pLesDB->setRemark(sCurRemark);
            pLesDB->update();
        }
    }

    mLinkedLesson->setNote(sCurRemark);
    m_bRemarkHandled = true;

    resetUI();
}

void NoteWidget::resetUI()
{
    mEdit->clear();
    mLabel->clear();
    mLinkedLesson = nullptr;
}

//void NoteWidget::paintEvent (QPaintEvent* ev)
//{

////    drawShadow(painter, 20, 1.0, QColor(120, 120, 120, 32), QColor(255, 255, 255, 0), 0.0, 1.0, 0.6, width(), height());
////    BaseShadowWidget::paintEvent (ev);
//}

void NoteWidget::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev)

    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, painter, this);
    painter->setRenderHint (QPainter::Antialiasing);
    painter->setRenderHint (QPainter::HighQualityAntialiasing);
//    painter.setBrush (this->palette ().background ());
    int nShadowWidth = getShadowMargin();
#ifdef USE_TRIANGLE_ARROW //使用三角箭头形式的多边形方式不.
    QPolygon poly (QVector<QPoint>{{nShadowWidth, nShadowWidth /*+ 7*/}, {nShadowWidth + 10, nShadowWidth /*+ 7*/}, {nShadowWidth + 15, nShadowWidth - 7}, {nShadowWidth + 20, nShadowWidth /*+ 7*/},
                                   {width() - nShadowWidth/* - 20*/, nShadowWidth /*+ 7*/}, {width () - nShadowWidth/* -20*/, height ()/* - 20*/},
                                   {0, height () /*- 20*/ - nShadowWidth}});
    QPainterPath path;
    path.addPolygon (poly);
    painter->fillRect (QRect(nShadowWidth, nShadowWidth, width () - 2 * nShadowWidth, height () - 2 * nShadowWidth), QBrush(QColor (255, 255, 255, 1)));
    painter->fillPath (path, QBrush(Qt::white));
#else
    QPainterPath path;
    path.addRoundedRect(nShadowWidth, nShadowWidth, width() - 2 * nShadowWidth, height() - 2 * nShadowWidth, 5.0f, 5.0f);
    painter->fillPath(path, QBrush(Qt::white));
#endif
}

void NoteWidget::createPdf()
{
    QString dateString = QString(mLinkedLesson->getDate().toString("yyyy-M-d")) + QString("-") + QString::number(mLinkedLesson->section());

    Arg *pArg = Arg::getInstance();
    QDir saveDir;
    pArg->getSaveDir(saveDir);
    QString currentApp = /*Arg::configDir*/saveDir.absolutePath();
    QString DirPath = currentApp + "/SaveFile";
    QDir dir;
    if(!dir.exists(DirPath) && mLinkedLesson) {
        bool bMkdirStatus = dir.mkdir(DirPath);
        Q_ASSERT_X(bMkdirStatus, "NoteWidget::createPdf", "mkdir savefile failed!");
    }

    DirPath+="/"+dateString;
    if(!dir.exists(DirPath) && mLinkedLesson) {
        bool bMkdirStatus = dir.mkdir(DirPath);
        Q_ASSERT_X(bMkdirStatus, "NoteWidget::createPdf", "mkdir DirPath failed!");
    }

    QString sDefaultFileName = /*DirPath + */"Default" +  QDateTime::currentDateTime().toString("yyyymmddhhmm");
    QString sFileName = InputTextWidget::getText(tr("请输入文件名"), sDefaultFileName, this);

    if(sFileName.trimmed().isEmpty()) {
        return;
    }

    if(!sFileName.contains(".pdf")) {
        sFileName.append(".pdf");
    }

    QString sFilePathName = DirPath + "/" + sFileName;
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(sFilePathName);
    QTextDocument textDocument;
    textDocument.print(&printer);

    QString sDateFileName = dateString + "/" + sFileName; ///Mark，为了区分每天每节课的资源..

    emit lessonResourceAddComplete(mLinkedLesson, sDateFileName, sFilePathName);
}

void NoteWidget::createPPT()
{
//    QPushButton *pSender = dynamic_cast<QPushButton*>(sender());
//    pSender->setIcon(QIcon(":/Icon/ppt2"));
    ///Mark,由于可能跨平台使用，不使用COM来创建word文档，直接采用复制空白文件方式.
    #ifdef TRUNK_VERSION
        QString sResourceFile = QApplication::applicationDirPath() + "/../docs";
    #else
        QString sResourceFile = QApplication::applicationDirPath() + "/docs";
    #endif

    #ifdef Q_OS_WIN
        sResourceFile.append("/Default.pptx");
        qDebug() << "now run Q_OS_WIN";
    #elif defined(Q_OS_MAC)
        //MAC下文件后缀可能得修改.
        sResourceFile.append("/Default.pptx");
        qDebug() << "now run Q_OS_MAC";
    #endif

        qDebug() << "the create ppt path is:" << sResourceFile;
        QString sSuffix(".pptx");

        //如果PPTX不存在，则ppt.
        QFile file(sResourceFile);
        if(!file.exists()) {
            sResourceFile.remove("Default.pptx");
            sResourceFile.append("Default.ppt");
            sSuffix.clear();
            sSuffix.append(".ppt");
        }

        file.setFileName(sResourceFile);
        if(file.exists()) {
            QString dateString = QString(mLinkedLesson->getDate().toString("yyyy-M-d")) + QString("-") + QString::number(mLinkedLesson->section());

            Arg *pArg = Arg::getInstance();
            QDir saveDir;
            pArg->getSaveDir(saveDir);
            QString currentApp = /*Arg::configDir*/saveDir.absolutePath();
            QString DirPath = currentApp + "/SaveFile";

            QDir dir;
            if(!dir.exists(DirPath) && mLinkedLesson) {
                bool bMkdirStatus = dir.mkdir(DirPath);
                Q_ASSERT_X(bMkdirStatus, "NoteWidget::createppt", "mkdir savefile failed!");
            }

            DirPath+="/"+dateString;
            if(!dir.exists(DirPath) && mLinkedLesson) {
                bool bMkdirStatus = dir.mkdir(DirPath);
                Q_ASSERT_X(bMkdirStatus, "NoteWidget::createppt", "mkdir DirPath failed!");
            }

            Lesson *pEditLesson = mLinkedLesson;
            //换焦点，mLinkedLesson将置空.
//            QDateTime curTime = QDateTime::currentDateTime();
            int index = pEditLesson->getResourceCount();
            QString sDefaultFileName = /*DirPath + */"Default" +  QString::number(index);
            QString sFileName = InputTextWidget::getText(tr("请输入文件名"), sDefaultFileName, this);

            if(sFileName.trimmed().isEmpty()) {
//                pSender->setIcon(QIcon(":/Icon/ppt1"));
                return;
            }


    #ifdef Q_OS_WIN
            if(!sFileName.contains(sSuffix)) {
                sFileName.append(sSuffix);
            }
    #elif defined(Q_OS_MAC)
            ///TODO,MAC下后缀再考虑.
    #endif

            QString sFilePathName = DirPath + "/" + sFileName;

            QFile file(sFilePathName);
            if(file.exists()) {
               bool bExist = pEditLesson->rootFolder()->checkName(sFileName);

               if(bExist) {
#ifdef USER_QT_MESSAGEBOX
                  QMessageBox::StandardButton btn = QMessageBox::information(this, tr("文件同名"), tr("存在同名文件，请重命名文件."));
#else
                   MessageDisplayButtonType btn = MessageDisplayWidget::showMessage(tr("文件同名"), tr("存在同名文件，请重命名文件."));
#endif

                  return;
               }

               QFile::remove(sFilePathName);
//                , QMessageBox::Ok, QMessageBox::Cancel);
//                if(btn == QMessageBox::Ok) {
//                    QFile::remove(sFilePathName);
//                }
//                else {
//                    return;
//                }
            }

            bool bStatus = QFile::copy(sResourceFile, sFilePathName);

            if(bStatus) {
                QString sDateFileName = dateString + "/" + sFileName; ///Mark，为什么这样不清楚,为了适应以前设计，重构考虑以前设计给改掉.
                file.close();

                emit lessonResourceAddComplete(pEditLesson, sDateFileName, sFilePathName);
            }
            else {
#ifdef USER_QT_MESSAGEBOX
                QMessageBox::information(this, tr("教师客户端"), tr("创建PPT文件失败!"));
#else
                MessageDisplayWidget::showMessage(tr("教师客户端"), tr("创建PPT文件失败!"));
#endif
            }
        }

//        pSender->setIcon(QIcon(":/Icon/ppt1"));
}

///Mark，这些重构要移出这个类.
void NoteWidget::createDoc()
{
//    QPushButton *pSender = dynamic_cast<QPushButton*>(sender());
//    pSender->setIcon(QIcon(":/Icon/word2"));

///Mark,由于可能跨平台使用，不使用COM来创建word文档，直接采用复制空白文件方式.
#ifdef TRUNK_VERSION
    QString sResourceFile = QApplication::applicationDirPath() + "/../docs";
#else
    QString sResourceFile = QApplication::applicationDirPath() + "/docs";
#endif

#ifdef Q_OS_WIN
    sResourceFile.append("/Default.docx");
    qDebug() << "now run Q_OS_WIN";
#elif defined(Q_OS_MAC)
    //MAC下文件后缀可能得修改.
    sResourceFile.append("/Default.docx");
    qDebug() << "now run Q_OS_MAC";
#endif

    qDebug() << "the create docx path is:" << sResourceFile;

    QFile file(sResourceFile);
    if(file.exists()) {
        QString dateString = QString(mLinkedLesson->getDate().toString("yyyy-M-d")) + QString("-") + QString::number(mLinkedLesson->section());

        Arg *pArg = Arg::getInstance();
        QDir saveDir;
        pArg->getSaveDir(saveDir);
        QString currentApp = /*Arg::configDir*/saveDir.absolutePath();
        QString DirPath = currentApp + "/SaveFile";

        qDebug() << "the current create doc path is:" << DirPath;

        QDir dir;
        if(!dir.exists(DirPath) && mLinkedLesson) {
            bool bMkdirStatus = dir.mkdir(DirPath);
            Q_ASSERT_X(bMkdirStatus, "NoteWidget::createPdf", "mkdir savefile failed!");
        }

        DirPath+="/"+dateString;
        if(!dir.exists(DirPath) && mLinkedLesson) {
            bool bMkdirStatus = dir.mkdir(DirPath);
            Q_ASSERT_X(bMkdirStatus, "NoteWidget::createPdf", "mkdir DirPath failed!");
        }

        Lesson *pEditLesson = mLinkedLesson;
//        QDateTime curTime = QDateTime::currentDateTime();
//        int index = curTime.offsetFromUtc();
        int index = pEditLesson->getResourceCount();
        QString sDefaultFileName = /*DirPath + */"Default" +  QString::number(index);
//        QString sDefaultFileName = /*DirPath + */"Default" +  QDateTime::currentDateTime().toString("yyyymmddhhmm");
        QString sFileName = InputTextWidget::getText(tr("请输入文件名"), sDefaultFileName, this);

        if(sFileName.trimmed().isEmpty()) {
//            pSender->setIcon(QIcon(":/Icon/word1"));
            return;
        }


#ifdef Q_OS_WIN
        if(!sFileName.contains(".docx")) {
            sFileName.append(".docx");
        }
#elif defined(Q_OS_MAC)
        ///TODO,MAC下后缀再考虑.
#endif

        QString sFilePathName = DirPath + "/" + sFileName;

        QFile file(sFilePathName);
        if(file.exists()) {
            bool bExist = pEditLesson->rootFolder()->checkName(sFileName);

            if(bExist) {
#ifdef USER_QT_MESSAGEBOX
               QMessageBox::StandardButton btn = QMessageBox::information(this, tr("教师客户端"), tr("存在同名文件，请重命名文件."));
#else
             MessageDisplayButtonType btn =   MessageDisplayWidget::showMessage(tr("教师客户端"), tr("存在同名文件，请重命名文件."));
#endif

               return;
            }

            QFile::remove(sFilePathName);
        }

        bool bStatus = QFile::copy(sResourceFile, sFilePathName);
        qDebug() << "the resource file and new file is:" << sResourceFile
                 << " " << sFilePathName;

        if(bStatus) {
            QString sDateFileName = dateString + "/" + sFileName; ///Mark，为什么这样不清楚,为了适应以前设计，重构考虑以前设计给改掉.
            file.close();

            emit lessonResourceAddComplete(pEditLesson, sDateFileName, sFilePathName);
        }
        else {
#ifdef USER_QT_MESSAGEBOX
            QMessageBox::information(this, tr("教师客户端"), tr("创建Doc文件失败!"));
#else
            MessageDisplayWidget::showMessage(tr("教师客户端"), tr("创建Doc文件失败!"));
#endif
        }
    }

    //    pSender->setIcon(QIcon(":/Icon/word1"));
}

void NoteWidget::recordClass()
{
    QString dateString = QString(mLinkedLesson->getDate().toString("yyyy-M-d")) + QString("-") + QString::number(mLinkedLesson->section());

    Arg *pArg = Arg::getInstance();
    QDir rootDir;
    pArg->getSaveDir(rootDir);
    QString currentApp = /*Arg::configDir*/rootDir.absolutePath();
    QString sDirPath = currentApp + "/SaveFile" + "/" + dateString;

    QDir saveDir(sDirPath);
    if(!saveDir.exists()) {
        saveDir.mkpath(sDirPath);
    }

//    sDirPath = QDir::toNativeSeparators(sDirPath);

    Lesson *pEditLesson = mLinkedLesson;
//        QDateTime curTime = QDateTime::currentDateTime();
//        int index = curTime.offsetFromUtc();
    int index = pEditLesson->getResourceCount();
    QString sDefaultFileName = "DefaultRecord" +  QString::number(index);
    sDefaultFileName = InputTextWidget::getText(tr("请输入文件名"), sDefaultFileName, this);

    if(sDefaultFileName.trimmed().isEmpty()) {
//            pSender->setIcon(QIcon(":/Icon/word1"));
        return;
    }

    sDirPath = sDirPath + "/";
    QString sFilePathName = sDirPath + sDefaultFileName + ".mp4";
    QFileInfo fiInfo(sFilePathName);
    if(fiInfo.exists()) {
        QFile::remove(sFilePathName);
    }

    QStringList args;
    args.append(sDirPath);
    args.append(sDefaultFileName);
#ifdef TRUNK_VERSION
    QString sRecordExePath = QApplication::applicationDirPath() + "/../lukebaiban";
#else
    QString sRecordExePath = QApplication::applicationDirPath() + "/lukebaiban";
#endif
    QString sRecodFile = sRecordExePath + "/" + "xidianvideo.exe";
    QProcess::startDetached(sRecodFile, args, sRecordExePath);

    //必须处理成上传可识别的格式.
    QString sDateFileName = dateString + "/" + sDefaultFileName + ".mp4";
    emit lessonResourceAddComplete(pEditLesson, sDateFileName, sFilePathName);
}
