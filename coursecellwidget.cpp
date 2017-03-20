#include "coursecellwidget.h"
//#include "ccu.h"
#include "notewidget.h"
#include "date.h"
#include "database.h"
#include "MainScreen.h"
#include "folder.h"
#include "copyhelper.h"
#include "logindialog.h"
#include "filenumwidget.h"
#include "lesson.h"
#include "teacher.h"
#include "term.h"
#include "colorbutton.h"
#include "editbutton.h"
#include "editwindow.h"
#include "uploadfiles2ftp.h"
#include "downloadfile.h"
#include "dialog.h"
#include "coursecolorpickerwidget.h"
#include "lessondetaildb.h"
#include "dataclassinstancemanage.h"
#include <QFont>
#include <QVBoxLayout>
#include <QLabel>
#include <cstdlib>
#include <QApplication>
#include <QStyleOption>
#include <QXmlStreamAttributes>
#include <QGraphicsEffect>
#include <QMimeData>
#include <QPainter>
#include <QMouseEvent>
#include <QProcess>
#include <algorithm>

#include <QMessageBox>
#include <QSettings>
#include <QProgressBar>

CourseCellWidget::CourseCellWidget(QWidget *parent) :
    QWidget(parent),
    editWindow(Q_NULLPTR)
{
    MainScreen::mainScreen->installEventFilter(this);
    setAcceptDrops(true);
    setup();

    QHBoxLayout* mainLayout = new QHBoxLayout(this);
//    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->setSpacing (0);
    mainLayout->setMargin(0);

    QHBoxLayout* editLayout = new QHBoxLayout;
    editLayout->addWidget(courseLabel);
    editLayout->addWidget(editButton);

    QVBoxLayout* courseLayout = new QVBoxLayout;
    courseLayout->setSpacing (0);
    courseLayout->setMargin (0);
    courseLayout->addLayout (editLayout); //调整.
    courseLayout->addWidget (locationLabel);
    courseLayout->addWidget (classLabel);

    mainLayout->addWidget (colorButton);
    mainLayout->addLayout(courseLayout);
    mainLayout->setSpacing(0);
//    mainLayout->addLayout(editLayout, 5);
    mainLayout->setMargin (0);

    setMouseTracking (true);

    ///Mark，为了好控制日期，一到上一层去CourseGrid类里.
//    connect(CCU::ccu, SIGNAL(changeEdit()), this, SLOT(setEdit()));
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
//    adjustSize ();
//    setFixedSize(sizeHint());
//    setFixedSize(sizeHint());
    resize(sizeHint());
    setLayout(mainLayout);

    ///Mark，这个应该提取出来一个就行.
    upload = new UploadFiles2Ftp();
    connect(upload, SIGNAL(uploadFinished()), this, SLOT(upLoadFinished()));
    connect(upload, SIGNAL(fileUploadFinished(const QString&)), this, SLOT(fileUploadFinished(const QString&)));
    connect(upload, SIGNAL(uploadError(const QString&, const QString&)), this, SLOT(uploadFileError(const QString&, const QString&)));

    m_pShadowEffect = Q_NULLPTR;
    m_pColorPicker = Q_NULLPTR;
    m_linkedLessonCurColor = Qt::black;
    m_bNeedRecordColor = true;
    m_bKeepSameRadius = true;

    LessonDetailDB* lesDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();
    connect(lesDB, &LessonDetailDB::delFinish, this, &CourseCellWidget::deleteLessonFinished);

    createWeekDay();
}

CourseCellWidget::~CourseCellWidget()
{
    NoteWidget *pNoteWgt = NoteWidget::getInstance();

    if(pNoteWgt != Q_NULLPTR) {
         pNoteWgt->cleanInstance();
    }
}

bool CourseCellWidget::isEditStatus() const
{
    return m_bEditEnabled;
}

//void CourseCellWidget::getDBState(const QJsonObject &json)
//{
//    qDebug()<<"DB result: "<<json<<endl;
//}

void CourseCellWidget::showEditWindow()
{

//    qDebug()<<"showEditWindow"<<endl;

    ///Mark,暂时这么处理，其实以后这个没必要每个coursecell都给创建一个，考虑弄一个总体的.
    if(editWindow == Q_NULLPTR) {
        editWindow = new EditWindow(this);
    }

    editWindow->reset();
    editWindow->show();

    /*
    lessonDB->setLessonId(lesson->getLessonId());
    connect(lessonDB, SIGNAL(delFinish(QJsonObject)), this, SLOT(getDBState(QJsonObject)));
    lessonDB->del();
    */

/*
    loginDialog* login = new loginDialog(MainScreen::mainScreen);
    login->show();
*/


/*
    Dialog* download = new Dialog(this);
    download->show();
*/


}

void CourseCellWidget::delCourse()
{
//    qDebug()<<"delete the course"<<endl;
    if(mLinkedLesson == nullptr)
    {
        qDebug()<<"Delete a nullptr course!!!!"<<endl;
        return;
    }
    else
    {
        LessonDetailDB* lesDB = DataClassInstanceManage::getInstance()->getLessonDetailDBPtr();/*new LessonDetailDB();*/
//        LessonDetailDB *lesDB = SingleObjectManager::getInstance()->getLessonDetailDB();
        lesDB->setId(mLinkedLesson->getLessonDetailId());
        QDate delDate = mLinkedLesson->getDate();
        lesDB->setEndDate(delDate.addDays(-7));
        lesDB->del();
//        mLinkedLesson = nullptr;
        //delButton->setVisible(false);
//        reset();
    }

}

void CourseCellWidget::setup ()
{
    delAction = new QAction(tr("删除"), this);
    addAction(delAction);
    connect(delAction, SIGNAL(triggered(bool)), SLOT(delCourse()));


    editButton = new EditButton (this, false);
    connect(editButton, SIGNAL(clicked(bool)), this, SLOT(showEditWindow()));
    editButton->move(parentWidget()->geometry().right(), parentWidget()->geometry().top());
    editButton->hide();

//    QGraphicsOpacityEffect *pOpacityEffect = new QGraphicsOpacityEffect;
//    pOpacityEffect->setOpacity(0.0f);
//    editButton->setGraphicsEffect(pOpacityEffect);
//    editButton->setEnabled(false);

    /*
    delButton = new EditButton (this, true);
    connect(delButton, SIGNAL(clicked(bool)), this, SLOT(delCourse()));
    delButton->move(parentWidget()->geometry().right() - 70, parentWidget()->geometry().top() + 5);
    delButton->hide();
    */


    colorButton = new ColorButton (this);
    colorButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);
    connect(colorButton, SIGNAL(clicked(bool)), this, SLOT(setColor()));

    fileNumWidget = new FileNumWidget(this, 0);
    fileNumWidget->setFixedWidth(16);
    fileNumWidget->setFixedHeight(16);

    //mLinkedLesson->lesFileNumWidget = fileNumWidget;


    courseLabel = new QLabel (this);
    locationLabel = new QLabel (this);
    classLabel = new QLabel (this);
    //TEMPORAY WAITING FOR SETTINGS SGINAL TO BE FINISHED
//    locationLabel->hide ();

    //qDebug()<<"editButton geometry: "<<editButton->geometry()<<endl;
}

void CourseCellWidget::resizeEvent (QResizeEvent* event)
{
    QWidget::resizeEvent (event);
    colorButton->setMaximumWidth (this->width () >> 4);
    fileNumWidget->move(width ()- fileNumWidget->width (), 0);
}

void CourseCellWidget::createWeekDay()
{
    m_weekDayMap.insert(1, tr("星期一"));
    m_weekDayMap.insert(2, tr("星期二"));
    m_weekDayMap.insert(3, tr("星期三"));
    m_weekDayMap.insert(4, tr("星期四"));
    m_weekDayMap.insert(5, tr("星期五"));
    m_weekDayMap.insert(6, tr("星期六"));
    m_weekDayMap.insert(7, tr("星期日"));
}

void CourseCellWidget::paintEvent(QPaintEvent *)
{
    //绘制边框
    QPainter painter(this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
    painter.setRenderHint (QPainter::Antialiasing);
    painter.setRenderHint (QPainter::HighQualityAntialiasing);
    painter.setPen(QColor(139, 139, 139));
    //绘制网格线.
    painter.drawLine(0, 0, this->width()/* - 1*/, 0);
    painter.drawLine(0, 0, 0, this->height()/* - 1*/);
//    painter.drawLine(this->width() - 1, 0, this->width() - 1, this->height() - 1);
//    painter.drawLine(0, this->height() - 1, this->width() - 1, this->height() - 1);

    //存在课程绘制课程展现.
    if (mLinkedLesson != nullptr) {
        painter.save();

        QBrush brush;

        if (mActivated)
        {
            painter.setPen(QColor(Qt::white));
            brush = QBrush(Qt::white);
        }
        else {
            double opa = std::min(mOpacity * 2, 1.0);
//            painter.save ();
            painter.setPen(QColor(255, 255, 255, static_cast<int>(opa * 255)));
            brush = QBrush(QColor(255, 255, 255, static_cast<int>(opa * 255)));
        }

        painter.setBrush(brush);

        //是否四个角弧度一致绘制.
        if(m_bKeepSameRadius) { //要保持四个角一致的弧度，则要求跟colorButton一个绘制边角方式.
            //colorButton之外的中间部分.
            painter.fillRect(colorButton->geometry ().right (), 1, width() - 2 * colorButton->width() + 1, height(), brush);

            //右上角弧度.

//            painter.fillRect(, Qt::white);

            //最右侧跟colorbutton保持一致绘制.
            int radius = std::min(colorButton->width () << 1, colorButton->height () << 1);
            int rectHeight = std::max(colorButton->height () - radius, 0);
            int nRightPaintStartX = width() - colorButton->width();

            //左右侧中间矩形.
            painter.fillRect(nRightPaintStartX, radius >> 1 , colorButton->width(), rectHeight, brush);

            painter.save();
            painter.setPen(QColor(255, 255, 255, 0));
            painter.drawPie(nRightPaintStartX - colorButton->width(), 0, radius, radius, 0, 1440);
            painter.drawPie(nRightPaintStartX - colorButton->width(), height () - radius, radius, radius, 4320, 1440);
            painter.restore();
        }
        else {
            QRect roundRect(this->colorButton->geometry ().right (), 1, width() - this->colorButton->width(), height());
            painter.drawRoundedRect(roundRect, 5, 5);
        }

        painter.restore();
    }

    painter.end();
}



//----------------------Code from dayWidget-------------------//
void CourseCellWidget::resetStyleSheet()
{

}

void CourseCellWidget::setBackgroundAlpha(double a)
{
    mBackgroundAlpha = a;
    resetStyleSheet();
}
//void CourseCellWidget::mouseMoveEvent (QMouseEvent* ev)
//{
//    QWidget::mouseMoveEvent (ev);
////    static bool in = false;
////    if (mLinkedLesson == nullptr)
////    {
////        ev->ignore ();
////        return;
////    }
////    bool intmp = false;
////    if (this->geometry ().contains (mapToParent (ev->pos ())))
////        intmp = true;
////    qDebug() << "instance:" << NoteWidget::getInstance ();
////    qDebug() << "in" << in << "intmp" << intmp;
////    if (intmp && !in)
////        NoteWidget::makeInstance (mLinkedLesson, this->mapToGlobal (QPoint(this->width () >> 1, this->height ())), this);
////    else if (!intmp && in)
////        NoteWidget::cleanInstance ();
////    in = intmp;
//}

void CourseCellWidget::enterEvent (QEvent* ev)
{
    QWidget::enterEvent (ev);

    if (mLinkedLesson == nullptr)
    {
        ev->ignore ();
        return;
    }

    if(Arg::viewMode != ViewMode::Edit) {
        //    if (mLinkedLesson != nullptr)
        //    {
        int nStartGlobalX = mapToGlobal(rect().topLeft()).x();
        //    int nCenterGlobalX = nStartGlobalX /*+ rect().width() / 2*/; //要求跟coursecellwidget左侧对齐显示.
        int nGlobalY = mapToGlobal(rect().bottomLeft()).y();

        NoteWidget *pNoteWidget = NoteWidget::makeInstance (mLinkedLesson, QRect (mapToGlobal (QPoint(0, 0)), mapToGlobal (QPoint(width (), height ()))),
                                                            QPoint(nStartGlobalX, nGlobalY));

        ///根据NoteWidget修改，暂时这么处理，先解绑定再绑定是避免多次绑定.
        disconnect(pNoteWidget, &NoteWidget::noteWidgetClosed, this, &CourseCellWidget::lostActiveted);
        connect(pNoteWidget, &NoteWidget::noteWidgetClosed, this, &CourseCellWidget::lostActiveted);

//        disconnect(pNoteWidget, &NoteWidget::lessonResourceAddComplete, this, &CourseCellWidget::addLessonResource, );
        connect(pNoteWidget, &NoteWidget::lessonResourceAddComplete, this, &CourseCellWidget::addLessonResource, Qt::UniqueConnection);
    }

    mActivated = true;
    update ();
    //    }

    if(m_pShadowEffect == Q_NULLPTR) {
        m_pShadowEffect = new QGraphicsDropShadowEffect();
        m_pShadowEffect->setColor (QColor(0, 0, 0, 80));

        setGraphicsEffect (m_pShadowEffect);
    }

    m_pShadowEffect->setBlurRadius(10);
    m_pShadowEffect->setXOffset(2.0);
    m_pShadowEffect->setYOffset(2.0);
}

void CourseCellWidget::leaveEvent (QEvent* ev)
{
    QWidget::leaveEvent (ev);

    if (mLinkedLesson == nullptr)
    {
        ev->ignore ();
        return;
    }

//    mActivated = false;
//    QPoint bias (10, 10);
    //ensure the note widget is not closed when the cursor is move onto it.
//    if (/*mLinkedLesson == nullptr || (*/NoteWidget::getInstance () && NoteWidget::getInstance ()->geometry ().intersected (QRect(QCursor::pos () - bias, QCursor::pos () + bias)).isValid ()/*)*/)
//    {
//        ev->ignore ();
//        return;
//    }
//    else if (mLinkedLesson != nullptr) {
    ///Mark,根据NoteWidget修改.
    NoteWidget *pNoteWgt = NoteWidget::getInstance();

    if((pNoteWgt != Q_NULLPTR) && (pNoteWgt->isVisible())) {
//        QPoint bias(1, 1);
//        QPoint cellWgtPt = mapFromGlobal(QCursor::pos()); ///NoteWidget是无父窗口使用，所以这里直接是针对global计算.
//        QRect adjustRect(QCursor::pos(), QCursor::pos() + bias);
        ///Mark,从notewidget的非显示端离开，notewidget关闭.
        QPoint globalCursorPos = mapFromGlobal(QCursor::pos());
        int nCursorX = globalCursorPos.x();
        int nCursorY = globalCursorPos.y();

        if((nCursorX < 0) || (nCursorX > width()) || nCursorY < 0) {
            qDebug() << "the cursor pos is:" << nCursorX << nCursorY;
            pNoteWgt->handleLessonRemarkDB();
            pNoteWgt->hide();
        }
        else {
            ///Mark,notewidget使用无父窗口，所以使用全局pos.
            qDebug() << "the cursor pos is" << QCursor::pos() << pNoteWgt->geometry();
            int nShadowWidth = pNoteWgt->getShadowMargin();
//            QPoint adjustPt = QCursor::pos() + QPoint(0, 2);
            //这里这么算原理，一是去掉看不到的阴影，第二，垂直方向不减掉yin阴影，为了保证notewidget跟课程部件有效的逻辑接触，能有效的进入notewidget.
            QRect noteWgtRect(pNoteWgt->pos() + QPoint(nShadowWidth, nShadowWidth), QSize(pNoteWgt->width() - 2 * nShadowWidth,
                                                                                          pNoteWgt->height() - 2 * nShadowWidth));

            if(noteWgtRect.contains(QCursor::pos())/*.isValid()*/) { //鼠标在noteWidget范围则默认NoteWidgety拥有鼠标.
                ev->ignore ();
//                pNoteWgt->show();
                pNoteWgt->setFocus();

                return;
            }
        }
    }
//    }

    if(m_pShadowEffect != Q_NULLPTR) {
        m_pShadowEffect->setBlurRadius(0);
        m_pShadowEffect->setXOffset(0);
        m_pShadowEffect->setYOffset(0);
    }

    mActivated = false;

    update ();
//    setGraphicsEffect (nullptr);


}

//void CourseCellWidget::mouseMoveEvent (QMouseEvent* ev)
//{
//    if (!this->geometry ().contains (this->mapToParent (ev->pos ())))
//    {
//        NoteWidget::cleanInstance ();
//    }
//}

void CourseCellWidget::mousePressEvent(QMouseEvent *event)
{
//    QToolTip::showText(event->pos(), tr("我在测试"), this, rect(), 2000);

    qDebug()<< "press: "<<event->pos()<<endl;
    //qDebug()<<"editButton: "<<editButton->geometry()<<endl;
    if(Arg::GetFocusMode==0)setFocus();
    if(mLinkedLesson != nullptr && mLinkedLesson->rootFolder() !=nullptr)
    {
        qDebug()<<"prese course: "<<mLinkedLesson->unit()<<endl;
        ///Delete Mark 2017.02.24,猜测性去掉.
//        Arg::fileListWidget->updateContent(mLinkedLesson->rootFolder());
        ///Delete Mark End.
        //qDebug()<<"hhhhhh"<<endl;
        //修改右侧显示课程信息的文本框内容 2016/12/14.
        ///Modified 2017.01.04，添加星期几信息.
        QDate date = mLinkedLesson->getDate();
        int nWeekDay = date.dayOfWeek();
        QString sDate = date.toString("yyyy-MM-dd");
        sDate.append(" ");
        sDate.append(m_weekDayMap.value(nWeekDay));
        lessonMap.insert("date", sDate);
        lessonMap.insert("unit", mLinkedLesson->unit());
        lessonMap.insert("subject", mLinkedLesson->subject());
        lessonMap.insert("location", mLinkedLesson->location());

        Arg::fileListWidget->updatecontentTextEdit(lessonMap);

        Arg::fileListWidget->setFileMap(fileMap);

        ///Mark,可能mlinkedLesson会在某个地方被置为空值.
        Lesson *pCurLesson = mLinkedLesson;
        Arg *pArg = Arg::getInstance();
        pArg->setCurrentLesson(pCurLesson);

        Arg::fileListWidget->setLesson(pCurLesson);

        Arg::fileListWidget->updateContent(mLinkedLesson->rootFolder());

        if(fileNumWidget != nullptr)
        {
            mLinkedLesson->lesFileNumWidget = fileNumWidget;

            if(fileNumWidget->getFileNum() != 0) {
                emit requestShowLessons();

//                Arg::fileListWidget->updateContent(mLinkedLesson->rootFolder());
            }
            else {
                Arg::fileListWidget->clearResourceDisplay();
            }
        }
    }


    QWidget::mousePressEvent(event);
}
void CourseCellWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
}
bool CourseCellWidget::eventFilter(QObject *obj, QEvent *event)
{
    /*
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(MainWidgetHideOrShow(keyEvent,true))return true;
    }
    */
    //return QFrame::eventFilter(obj, event);
    return QWidget::eventFilter(obj, event);
}
void CourseCellWidget::setColor()
{
    if (mLinkedLesson == nullptr)
        return;

    m_bNeedRecordColor = true;
    //    QColor c = QColorDialog::getColor(mLinkedLesson->getColor (), this);
//    QColor c = CourseColorPickerWidget
    if(m_pColorPicker == Q_NULLPTR) {
        m_pColorPicker = new CourseColorPickerWidget(this);
        connect(m_pColorPicker, &CourseColorPickerWidget::colorSelectComplete, this, &CourseCellWidget::setLinkedLessonColor);
//        connect(m_pColorPicker, &CourseColorPickerWidget::cancelSelectedColor, this, &CourseCellWidget::cancelSetLinkedLessonColor);
    }

    m_pColorPicker->show();
//    if(c.isValid())
//    {
//        mLinkedLesson->setColor (c);
//    }


}

void CourseCellWidget::setLinkedLessonColor(const QColor &color)
{
    if (mLinkedLesson != Q_NULLPTR) {
        if(m_bNeedRecordColor) {
            m_linkedLessonCurColor = mLinkedLesson->getColor();
            m_bNeedRecordColor = false;
        }

        mLinkedLesson->setColor(color);

        ///Mark，由于原来设计限制，这些配置没做管理，重构需要设计管理配置类.
        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        QDate date = mLinkedLesson->getDate();
        QString sDate = date.toString("yyyy-MM-dd");
        QString  sKey = Arg::username + QString("%1%2").arg(sDate).arg(mLinkedLesson->getLessonDetailId());
        configSettings.setValue(sKey, color);

        m_pColorPicker->close();
    }
}

void CourseCellWidget::cancelSetLinkedLessonColor()
{
    if (mLinkedLesson != Q_NULLPTR) {
        mLinkedLesson->setColor(m_linkedLessonCurColor);
    }
}

void CourseCellWidget::addLessonResource(Lesson *pLinkedLesson, const QString &sFileName, const QString &sFileDir)
{
    //不是一节课.
    if(pLinkedLesson != mLinkedLesson) {
        return;
    }

    //    ///why?因为enter后会出现noteWidget，所以会存在创建资源操作.
        QDate date = pLinkedLesson->getDate();
        int nWeekDay = date.dayOfWeek();
        QString sDate = date.toString("yyyy-MM-dd");
        sDate.append(" ");
        sDate.append(m_weekDayMap.value(nWeekDay));
        lessonMap.insert("date", sDate);
        lessonMap.insert("unit", mLinkedLesson->unit());
        lessonMap.insert("subject", mLinkedLesson->subject());
        lessonMap.insert("location", mLinkedLesson->location());
        Arg::fileListWidget->updatecontentTextEdit(lessonMap);

        Arg::fileListWidget->setFileMap(fileMap);

        ///Mark,可能mlinkedLesson会在某个地方被置为空值.
        Lesson *pCurLesson = mLinkedLesson;
        ///Mark,不建议下面这样用，替换为后面两句代码，why，因为有时候会失效，无效操作，崩溃.
    //    Arg::setCurrentLesson(pCurLesson);
        Arg *pArg = Arg::getInstance();
        pArg->setCurrentLesson(pCurLesson);

        Arg::fileListWidget->setLesson(pCurLesson);

        if(fileNumWidget != nullptr)
        {
            mLinkedLesson->lesFileNumWidget = fileNumWidget;
        }
        ///End.
        ///

    fileMap.insert(sFileName, sFileDir);

    ///Mark,只能按原来设计实现路径保存，但是这个有些混乱，重构要改变.

    //-------------------------ftp upload-----------------------------//
    sourcefileMap.insert("fileName", sFileName);
    sourcefileMap.insert("localFilePath", sFileDir);
    sourcefileMap.insert("detailID", QString::number(mLinkedLesson->getLessonDetailId()));
    sourcefileMap.insert("date", mLinkedLesson->getDate().toString("yyyy-M-d"));
    //-------------------------ftp upload-----------------------------//

    my_urls.append(QUrl::fromLocalFile(sFileDir));
    QString fileName = sFileName.split("/").at(1);
    QString fpName = "./SaveFile/" + fileName;
    File* file = new File(fpName, mLinkedLesson->rootFolder());
    qDebug() << "add file: " << file << " name: " << file->name() << endl;
    mLinkedLesson->rootFolder()->add(file);
//    qDebug()<<"ddddfile"<<endl;
    fileNumWidget->addFile();
    fileNumWidget->update();

//    qDebug() << "add dddd: "<<endl;
    mLinkedLesson->name2File.insert(file->name(), file);
//    qDebug() << "add map: "<<endl;

    if(mLinkedLesson != nullptr && fileNumWidget != nullptr)
    {
        mLinkedLesson->lesFileNumWidget = fileNumWidget;
    }

    reset();

    /// Mark,这里没上传完，怎么打开?.
    m_bCreateNewFile = true;

    upload->up2ftp(sourcefileMap);
    Arg::fileListWidget->showUpdateProgress(true);

    Arg::fileListWidget->updateContent(mLinkedLesson->rootFolder());

    if(fileNumWidget->getFileNum() != 0) {
        emit requestShowLessons();
    }

//    QString sFilePathName = sFileDir;
//    sFilePathName.replace("/","\\");

//    QProcess::startDetached("explorer " + sFilePathName);
}

void CourseCellWidget::setEdit(bool bEditEnabled)
{
    QGraphicsOpacityEffect *pOpacityEffect = dynamic_cast<QGraphicsOpacityEffect*>(editButton->graphicsEffect());

    m_bEditEnabled = bEditEnabled;
    //qDebug()<<"come in setEdit: "<<static_cast<int>(Arg::viewMode)<<endl;
    if(m_bEditEnabled)
    {
        //---------------sec 4 and 7 are breaks-----------------//
        ///Delete 2017.02.21,这个break不使用CourseCellWidget,课程不能考虑到其它非课程功能.
//        if(secIndex != 4 && secIndex != 7)
//        {
        if(pOpacityEffect != Q_NULLPTR) {
            pOpacityEffect->setOpacity(1.0f);
            editButton->setEnabled(true);
            editButton->raise();
        }
        else {
            editButton->show();
        }
//        }

        if(mLinkedLesson != nullptr/* && secIndex != 4 && secIndex != 7*/)
        {
            setContextMenuPolicy(Qt::ActionsContextMenu);
            //delButton->show();
        }
    }
    else
    {
        setContextMenuPolicy(Qt::DefaultContextMenu);

        if(pOpacityEffect != Q_NULLPTR) {
            pOpacityEffect->setOpacity(0.0f);
            editButton->setEnabled(false);
        }
        else {
            editButton->hide();
        }
        //delButton->hide();
    }
}

void CourseCellWidget::deleteLessonFinished(int nDetailID, const QJsonObject &jsonObj)
{
//    qDebug()<<"delete course result: "<<json<<endl;
    if(jsonObj["status"] == "false") return;

    if(mLinkedLesson != Q_NULLPTR) {
        int nLinkedDetailID = mLinkedLesson->getLessonDetailId();

        //不是当前操作的.
        if(nLinkedDetailID != nDetailID) {
            return;
        }

        int nWeekDay = mLinkedLesson->getWeekId();
        int nSection = mLinkedLesson->getSection();

        Arg *pArg = Arg::getInstance();

        //删除该节课对应资源.
        ///Mark todo,暂时不从服务器跟数据库删除对应数据，只处理上传下载文件.
        if(mLinkedLesson == pArg->currentLesson()) {
            pArg->fileListWidget->handleUporDownloadFile(mLinkedLesson);
            pArg->fileListWidget->unlink();
            pArg->fileListWidget->clearResourceDisplay();
        }

        QDate delDate = mLinkedLesson->getDate();

        pArg->deleteLessonFinished(nWeekDay, nSection, nDetailID, delDate.addDays(-7));

        //删除本周创建出的课程对象.
        unlink();
        reset();
    }
}

void CourseCellWidget::upLoadFinished()
{
    Arg::fileListWidget->showUpdateProgress(false);
}

void CourseCellWidget::lostActiveted()
{
    mActivated = false;

    if(m_pShadowEffect != Q_NULLPTR) {
        m_pShadowEffect->setBlurRadius(0);
        m_pShadowEffect->setXOffset(0);
        m_pShadowEffect->setYOffset(0);
    }

    repaint();
}

void CourseCellWidget::uploadFileError(const QString &sFilePathName, const QString &sError)
{
    if(m_bCreateNewFile) {
//        QFile::remove(sFilePathName);
        m_bCreateNewFile = false;
    }

    QMessageBox::information(this, tr("上传失败"), tr("上传文件失败:%1，请手动再次尝试上传！").arg(sError));
}

void CourseCellWidget::fileUploadFinished(const QString &fileName)
{
    if(m_bCreateNewFile) {
        QString sFilePathName = fileName;
        sFilePathName.replace("/","\\");
        QProcess::startDetached("explorer " + sFilePathName);

        qDebug() << "the opend file is:" << fileName;

        m_bCreateNewFile = false;
    }
}

void CourseCellWidget::reset()
{
    QColor c;
    if (mLinkedLesson == nullptr)
    {
        fileNumWidget->setDisplayFileNumEnabled(false);
        colorButton->setVisible (false);
        locationLabel->setText ("");
        courseLabel->setText ("");
        classLabel->setText ("");
        return;
    }

    colorButton->setVisible (true);
    c = mLinkedLesson->getColor ();

    QString rs = QString::number(c.red(), 10);
    QString gs = QString::number(c.green(), 10);
    QString bs = QString::number(c.blue(), 10);
    QString cs = "rgb(" + rs + ", " + gs + ", " + bs + ");";

    colorButton->setColor(c);

    QString course_style = "color: " + cs;
    courseLabel->setStyleSheet(course_style);

    //set file num Widget

    fileNumWidget->setDisplayFileNumEnabled(true);

    if(mLinkedLesson->rootFolder() == nullptr)
    {
        fileNumWidget->setFileNum(0);
        fileNumWidget->setDisplayFileNumEnabled(false);
    }
    else
    {
        if(mLinkedLesson->rootFolder()->size(true) == 0)
        {
            fileNumWidget->setFileNum(0);
            fileNumWidget->setDisplayFileNumEnabled(false);
        }
        else
        {
            if(mLinkedLesson->rootFolder()->size(true) > 100)
            {
                fileNumWidget->setFileNum(100);
            }
            else
            {
                fileNumWidget->setFileNum(mLinkedLesson->rootFolder()->size(true));
            }
        }

    }

    fileNumWidget->update();

    //set infos
//    locationLabel->setText (mLinkedLesson->location ());
    courseLabel->setText (mLinkedLesson->subject ());
    classLabel->setText (mLinkedLesson->unit ());
}

void CourseCellWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(Arg::viewMode == ViewMode::Edit) {
        if(mLinkedLesson != Q_NULLPTR) {
            QMessageBox::information(this, tr("教师客户端"), tr("编辑模式不能拖入资源,请切换模式!"));
        }

        return;
    }

    if (Arg::lockDrag == false &&event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}
QFileInfoList CourseCellWidget::getFileList(QString path)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(QDir::Files /*| QDir::Hidden*/ | QDir::NoSymLinks);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    for(int i = 0; i != folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = getFileList(name);
         file_list.append(child_file_list);
    }

    return file_list;
}

void CourseCellWidget::fileCopy(QString &from_path, QString &target_path, QString &fileName)
{
    if(!QFile(target_path).exists() && mLinkedLesson)
    {
        qDebug() << "new path exist && lesson not null" << endl;

        qDebug()<< from_path;
        qDebug()<< fileName;
        qDebug()<< target_path;

        QFile::copy(from_path, target_path);

        fileMap.insert(fileName, target_path);
        sourcefileMap.insert(fileName, from_path);

        my_urls.append(QUrl::fromLocalFile(target_path));
        QString fp = "./SaveFile/" + fileName;
        File* file = new File(fp, mLinkedLesson->rootFolder());
        qDebug() << "add file: " << fp << endl;
        mLinkedLesson->rootFolder()->add(file);
        fileNumWidget->addFile();
        fileNumWidget->update();
        reset();
     }

}


bool CourseCellWidget::copyFileToPath(QString sourceDir ,QString toDir, QString fileName, bool coverFileIfExist)
{

    //toDir.replace("\\","/");
    if (sourceDir == toDir){
        return true;
    }
    if (!QFile::exists(sourceDir)){
        return false;
    }

    QDir createfile/* = new QDir*/;
    bool exist = createfile.exists(toDir);
    if (exist){
        if(coverFileIfExist){
            createfile.remove(toDir);
        }
    }//end if

    QString store = CopyHelper::store(sourceDir, toDir);
    qDebug()<<store<<endl;

    fileMap.insert(fileName, toDir);

    //-------------------------ftp upload-----------------------------//
    sourcefileMap.insert("fileName", fileName);
    sourcefileMap.insert("localFilePath", sourceDir);
    sourcefileMap.insert("detailID", QString::number(mLinkedLesson->getLessonDetailId()));
    sourcefileMap.insert("date", mLinkedLesson->getDate().toString("yyyy-M-d"));
    //-------------------------ftp upload-----------------------------//

    my_urls.append(QUrl::fromLocalFile(toDir));
    QString fp = "./SaveFile/" + fileName;
    File* file = new File(fp, mLinkedLesson->rootFolder());
    qDebug() << "add file: " << file << " name: " << file->name() << endl;
    mLinkedLesson->rootFolder()->add(file);
    qDebug()<<"ddddfile"<<endl;
    fileNumWidget->addFile();
    fileNumWidget->update();

    qDebug() << "add dddd: "<<endl;
    mLinkedLesson->name2File.insert(file->name(), file);
    qDebug() << "add map: "<<endl;

    if(mLinkedLesson != nullptr && fileNumWidget != nullptr)
    {
        mLinkedLesson->lesFileNumWidget = fileNumWidget;
    }

    reset();

    return true;
}


bool CourseCellWidget::copyDirectoryFiles(QString dateString, Folder* folder, QString fromDir, QString toDir, bool coverFileIfExist)
{
    QDir sourceDir(fromDir);
    QDir targetDir(toDir);
    if(!targetDir.exists()){    /*如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList){
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    /*当为目录时，递归的进行copy */

            QString subFp = folder->name() + "/" + fileInfo.fileName();
            Folder* subFolder = new Folder(subFp, folder);

            if(!copyDirectoryFiles(dateString, subFolder, fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()),
                coverFileIfExist))
                return false;

            else //add sub folder to root folder for UI use
            {

                folder->add(subFolder);
            }
        }
        else{            /*当允许覆盖操作时，将旧文件进行删除操作 */
            if(coverFileIfExist && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                targetDir.filePath(fileInfo.fileName()))){
                    return false;
            }
            //QString store = CopyHelper::store(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()));
            //qDebug()<<store<<endl;

            else //add file to folder for UI use
            {
                QString fp = "./SaveFile/" + folder->name() + "/" + fileInfo.fileName();
                qDebug()<<"add file to folder: "<<fp<<endl;
                File* file = new File(fp, folder);
                folder->add(file);
            }
        }
    }
    return true;
}

Lesson *CourseCellWidget::getLesson()
{
    return mLinkedLesson;
}

//drop folder needs to be finished
void CourseCellWidget::dropEvent(QDropEvent *event)
{
    if(Arg::viewMode == ViewMode::Edit) {
        return;
    }

    if (mLinkedLesson == nullptr)
    {
        event->ignore ();
        return;
    }
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls) {
       QFileInfo fileInfo(url.toLocalFile());
       qDebug()<<"path type: "<<fileInfo.isFile()<<endl;
       //QDir fdir(url.toLocalFile());
       //qDebug()<<"filesize: "<<fileInfo.size()<<endl;
       QFileInfoList fileInfoList = getFileList(url.toLocalFile());
       qDebug()<<"file info list: "<<fileInfoList.size()<<endl;


       QString dateString = QString(mLinkedLesson->getDate().toString("yyyy-M-d")) + QString("-") + QString::number(mLinkedLesson->section ());
       //QString currentApp = QCoreApplication::applicationDirPath();
       Arg *pArg = Arg::getInstance();
       QDir saveDir;
       pArg->getSaveDir(saveDir);
       QString currentApp = /*Arg::configDir*/saveDir.absolutePath();
       qDebug()<<"currentApp: "<<currentApp<<endl;

       //currentApp.replace("/","\\");
       //qDebug()<<"after replace: "<<currentApp<<endl;

       QString DirPath = currentApp + "/SaveFile";
       QDir dir;

       if(!dir.exists(DirPath) && mLinkedLesson)
           qDebug()<<dir.mkdir(DirPath);
       DirPath+="/"+dateString;
       if(!dir.exists(DirPath) && mLinkedLesson)
           qDebug()<<dir.mkdir(DirPath);

       QString NewPath = DirPath+"/"+url.fileName();

       QString fileName = dateString+"/"+url.fileName();


       qDebug()<<"New path is: "<<NewPath<<endl;

       if(QFile(NewPath).exists())
       {
//           QMessageBox::information(this, tr("文件同名"), tr("改路径下存在同名文件，是否删除原文件？"));
           QFile::remove(NewPath);

//           continue;
       }

       if(fileInfo.isFile())
       {
            //fileCopy(url.toLocalFile(), NewPath, url.fileName());
           copyFileToPath(url.toLocalFile(), NewPath, fileName, true);
           //qDebug() << store << endl;
       }
      else
       {
           qDebug()<<url.fileName()<<" is a folder"<<endl;
           qDebug()<<"In version 1.0 folder is forbidden"<<endl;
           return;

           fileMap.insert(url.fileName(), NewPath);

           //------------------------ftp upload---------------------------------//
           sourcefileMap.insert("fileName", url.fileName());
           sourcefileMap.insert("localFilePath", NewPath);
           sourcefileMap.insert("detailID", QString::number(mLinkedLesson->getLessonDetailId()));
           sourcefileMap.insert("date", mLinkedLesson->getDate().toString("yyyy-M-d"));
           //------------------------ftp upload---------------------------------//

           my_urls.append(QUrl::fromLocalFile(NewPath));

           //sFolder* folder = new Folder(url.fileName(), lesson->rootFolder());

           ///Mark，按照上传路径组成，这里先这么处理.
           QString sFileName = fileName.split("/").at(1);
           QString fpName = "./SaveFile/" + sFileName;
           Folder* folder = new Folder(fpName, mLinkedLesson->rootFolder());

           copyDirectoryFiles(dateString, folder, url.toLocalFile(), NewPath, true);

           qDebug() << "add folder: " << NewPath << endl;
           mLinkedLesson->rootFolder()->add(folder);
           //fileNumWidget->addFile();
           //fileNumWidget->update();
           reset();

       }
    }

    if(!sourcefileMap.isEmpty())
    {
//        emit localFinish(sourcefileMap);
//        sourcefileMap.clear();

        qDebug() << "drop file finish" << endl;

        ///why?显示资源时候Arg当前课程需要变为显示资源的了.
        Arg::fileListWidget->updatecontentTextEdit(lessonMap);

        Arg::fileListWidget->setFileMap(fileMap);

        ///Mark,可能mlinkedLesson会在某个地方被置为空值.
        Lesson *pCurLesson = mLinkedLesson;
        Arg *pArg = Arg::getInstance();
        pArg->setCurrentLesson(pCurLesson);

        Arg::fileListWidget->setLesson(pCurLesson);

        if(fileNumWidget != nullptr)
        {
            mLinkedLesson->lesFileNumWidget = fileNumWidget;
        }
        ///End.
        ///

//        emit localFinish(sourcefileMap);
        upload->up2ftp(sourcefileMap);
        sourcefileMap.clear();

        if(fileNumWidget->getFileNum() != 0) {
            emit requestShowLessons();
            Arg::fileListWidget->updateContent(mLinkedLesson->rootFolder());
            Arg::fileListWidget->showUpdateProgress(true);
        }
    }
}

void CourseCellWidget::link (Lesson* lesson)
{
    if (mLinkedLesson != Q_NULLPTR)
    {
        disconnect (mLinkedLesson, SIGNAL (colorChanged (QColor)), this, SLOT (reset ()));
        disconnect (mLinkedLesson, SIGNAL (locationChanged (QString)), this, SLOT (reset ()));
        disconnect (mLinkedLesson, SIGNAL (subjectChanged (QString)), this, SLOT (reset ()));
        disconnect (mLinkedLesson, SIGNAL (unitChanged (QString)), this, SLOT (reset ()));
    }

    mLinkedLesson = lesson;
    upload->setLesson(mLinkedLesson);

    if (mLinkedLesson != nullptr)
    {
        mLinkedLesson->setStartTime(m_sStartTime);
        mLinkedLesson->setEndTime(m_sEndTime);

        connect (mLinkedLesson, SIGNAL (colorChanged (QColor)), this, SLOT (reset ()));
        connect (mLinkedLesson, SIGNAL (locationChanged (QString)), this, SLOT (reset ()));
        connect (mLinkedLesson, SIGNAL (subjectChanged (QString)), this, SLOT (reset ()));
        connect (mLinkedLesson, SIGNAL (unitChanged (QString)), this, SLOT (reset ()));


        qDebug() << "linked lessons" << mLinkedLesson->getLessonDetailId() << mLinkedLesson->getLessonId();
        ///Mark,为了兼容，暂时这么弄，其实数据管理与ui就不能绑定.
        mLinkedLesson->lesFileNumWidget = fileNumWidget;

        ///Mark，由于原来设计限制，这些配置没做管理，重构需要设计管理配置类.
        QSettings configSettings("./UserConfig/config", QSettings::IniFormat);
        QDate date = mLinkedLesson->getDate();
        QString sDate = date.toString("yyyy-MM-dd");
        QString  sKey = Arg::username + QString("%1%2").arg(sDate).arg(mLinkedLesson->getLessonDetailId());
        QVariant var = configSettings.value(sKey, QColor(0, 0, 0));
        if(var.canConvert(QMetaType::QColor)) {
            QColor color = var.value<QColor>();

            if(color == QColor(0, 0, 0)) {
                int red = (date.day() * (mLinkedLesson->getWeekId() * 30 + mLinkedLesson->getLessonDetailId() * 10)) % 255;
                int green = (date.day() * (mLinkedLesson->getWeekId() * 20 + mLinkedLesson->getLessonDetailId() * 20)) % 255;
                int blue = (date.day() * (mLinkedLesson->getWeekId() * 30 + mLinkedLesson->getLessonDetailId() * 10)) % 255;

                if((255 -red < 30) && (255 -blue < 30) && (255 -green < 30)) {
                    red = red - 40;
                    green = green - 40;
                    blue = blue - 40;
                }

                color.setRed(red);
                color.setGreen(green);
                color.setBlue(blue);

                mLinkedLesson->setColor(color);
            }
            else {
                mLinkedLesson->setColor(color);
            }
        }
    }

    reset ();
}

void CourseCellWidget::unlink()
{
    link(nullptr);

    ///Mark，这个的管理不归CourseCellWidget.
//    if(mLinkedLesson != Q_NULLPTR) {
//        delete mLinkedLesson;
//        mLinkedLesson = Q_NULLPTR;
//    }
}

TextShowWindowXD::TextShowWindowXD(QWidget *parent):QFrame(parent)
{
    verticalSlider = new QSlider(this);
    verticalSlider->setObjectName(QStringLiteral("verticalSlider"));
    verticalSlider->setOrientation(Qt::Vertical);
    verticalSlider->setVisible(false);
    MainWidget = new QWidget(this);
    MainLayout = new QVBoxLayout(MainWidget);
    MainLayout->setMargin(0);
    MainLayout->setSpacing(0);
    MainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}


//void TextShowWindowXD::reset(DailyData *d)
void TextShowWindowXD::reset(QList<EventData*> t)
{
    clear();
    //data=d;
    text = t;
    if(MainWidget != nullptr)
    {
        delete MainWidget;
    }
    MainWidget = new QWidget(this);
    MainWidget->move(0, 0);
    MainWidget->setVisible(true);
    MainLayout = new QVBoxLayout(MainWidget);
    MainLayout->setMargin(0);
    MainLayout->setSpacing(0);
    MainLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    MainWidget->resize(this->size().width(),MainWidget->height());

    //for(int i=(int)(data->text.size()-1);i>=0;i--)
    //    addEvent((data->text[i]));

    for(int i=(int)(text.size()-1);i>=0;i--)
        addEvent(text[i]);
}


void TextShowWindowXD::resizeEvent(QResizeEvent *event)
{
    MainWidget->resize(event->size().width(),MainWidget->height());
}

EventTextButton* TextShowWindowXD::addEvent(EventData *d)
{
    EventTextButton *e = new EventTextButton(d,MainWidget);
    e->setMinimumHeight(Const::MyButtonSize);
    MainLayout->insertWidget(0,e);
    e->resize(this->width(),e->minimumHeight());
    //MainWidget->resize(this->width(),data->text.size()*Const::MyButtonSize);
    MainWidget->resize(this->width(),text.size()*Const::MyButtonSize);
    e->setText(d->text);
    e->raise();
    e->setVisible(true);
    connect(e,SIGNAL(changed()),SLOT(resetSize()));
    return e;
}

void TextShowWindowXD::NewEvent()
{
    //构造EventData
    //data->text.insert(0,new EventData);
    text.insert(0,new EventData);
    //EventData *d = data->text[0];
    EventData *d = text[0];
    //设置初值
    EventTextButton *e = addEvent(d);
    e->SetText();
    if(e->text()==QString(""))
    {
        //data->text.removeAt(0);
        text.removeAt(0);
        MainLayout->removeWidget(static_cast<QWidget*>(e));
        e->setVisible(false);
    }
    //reset(data);
}

QSize CourseCellWidget::sizeHint () const
{
    return QSize (90, 60);
}

void CourseCellWidget::setOpacity(double o)
{
    mOpacity = o;
    update();
}

void CourseCellWidget::setStartTime(int nSeconds)
{
    m_nStartTimeSeconds = nSeconds;
}

int CourseCellWidget::getStartTime() const
{
    return m_nStartTimeSeconds;
}

void CourseCellWidget::setEndTime(int nSeconds)
{
    m_nEndTimeSeconds = nSeconds;
}

int CourseCellWidget::getEndTime() const
{
    return m_nEndTimeSeconds;
}

void CourseCellWidget::setStartTime(const QString &sStartTime)
{
    m_sStartTime = sStartTime;
}

void CourseCellWidget::getStartTime(QString &sStartTime)
{
    sStartTime = m_sStartTime;
}

void CourseCellWidget::setEndTime(const QString &sEndTime)
{
    m_sEndTime = sEndTime;
}

void CourseCellWidget::getEndTime(QString &sEndTime)
{
    sEndTime = m_sEndTime;
}



