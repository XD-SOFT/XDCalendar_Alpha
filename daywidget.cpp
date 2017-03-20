#include "daywidget.h"
#include <cstdlib>
#include <QApplication>
#include "date.h"
#include "database.h"
#include <QFont>
#include "MainScreen.h"
#include "config.h"
DayWidget::DayWidget(QWidget *parent) : QFrame(parent)
{
    MainScreen::mainScreen->installEventFilter(this);
    //初始化
    setMinimumWidth(115);
    setObjectName("DayWidget"+QString::number((rand()<<15) ^ rand(),16));
    setAcceptDrops(true);

    //构造下部横条窗口
    DownWidget = new QWidget(this);
    DownWidget->setMinimumHeight(Const::MyButtonSize);
    DownHLayout = new QHBoxLayout(DownWidget);
    DownHLayout->setContentsMargins(0,0,0,0);
    DownHLayout->setSpacing(0);
    dateLabel = new QLabel(this);
    SetColorButton = new TransparentButton(QIcon(":/Icon/1178331.png"),"",DownWidget);
    SetColorButton->setObjectName(QStringLiteral("SetColorButton"));
    SetColorButton->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
    SetColorButton->setToolTip("更改背景");
    connect(SetColorButton,SIGNAL(clicked(bool)),this,SLOT(setColor()));
    FileButton = new TransparentButton(QIcon(":/Icon/1197083.png"),"",DownWidget);
    FileButton->setObjectName(QStringLiteral("FileButton"));
    FileButton->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
    FileButton->setToolTip("文件");
    PlusButton = new TransparentButton(QIcon(":/Icon/1163593.png"),"",DownWidget);
    PlusButton->setObjectName(QStringLiteral("PlusButton"));
    PlusButton->setFixedSize(QSize(Const::MyButtonSize, Const::MyButtonSize));
    PlusButton->setToolTip("添加事件");
    FileMenu = new DragMenu(0);
    //connect(FileButton,SIGNAL(clicked(bool)),FileMenu,SLOT(display()));
    FileButton->setMenu(FileMenu);
    FileButton->setStyleSheet("QPushButton::menu-indicator{image:None;}");

    DownHLayout->addWidget(FileButton);
    DownHLayout->addWidget(SetColorButton);
    DownHLayout->addWidget(PlusButton);
    DownHLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    DownHLayout->addWidget(dateLabel);

    //构造中部事件窗口
    EventWidget = new TextShowWindow(this);
    EventWidget->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Expanding);
    connect(PlusButton,SIGNAL(clicked(bool)),EventWidget,SLOT(NewEvent()));

    MainGridLayout = new QGridLayout(this);
    MainGridLayout->setSpacing(0);
    MainGridLayout->setContentsMargins(0, 0, 0, 0);
    MainGridLayout->addWidget(EventWidget,0,0);
    MainGridLayout->addWidget(DownWidget,1,0);
}

void DayWidget::resetStyleSheet()
{
    QString thisname=QString(this->metaObject()->className())+"#"+this->objectName();
    QString str;

    //静态样式
    QColor c=data->Background_Color;
    //休假日偏红
    if(Arg::WeekDayIsHoliday[date.dayOfWeek()-1])
    {
        int r,g,b;
        r=c.red()-30;if(r>255)r=255;if(r<0)r=0;
        g=c.green()+0;if(g>255)g=255;if(g<0)g=0;
        b=c.blue()+50;if(b>255)b=255;if(b<0)b=0;
        c.setRed(r);
        c.setGreen(g);
        c.setBlue(b);
    }
    double a=data->Background_Alpha;
    //当天日期高亮
    if(Arg::todayLight && date == QDateTime::currentDateTime().date())
    {
        a=0.80;
        int r,g,b;
        r=c.red()+180;if(r>255)r=255;if(r<0)r=0;
        g=c.green()+180;if(g>255)g=255;if(g<0)g=0;
        b=c.blue()-80;if(b>255)b=255;if(b<0)b=0;
        c.setRed(r);
        c.setGreen(g);
        c.setBlue(b);
    }
    str+=thisname+" { background-color: rgba("+QString::number(c.red(),10)+","+QString::number(c.green(),10)+","+QString::number(c.blue(),10)+","+QString::number(a)+") ; border: 1px solid black}";
    //动态样式

    {
        double offset=0.1;
        if(a>0.9)a-=offset;
        else a+=offset;
    }
    {
        int offset=30;
        c.setRed(c.red()+offset>255?c.red()-offset:c.red()+offset);
        offset=40;
        c.setBlue(c.blue()+offset>255?c.blue()-offset:c.blue()+offset);
        offset=40;
        c.setGreen(c.green()+offset>255?c.green()-offset:c.green()+offset);
    }
    str+=thisname+":hover { background-color: rgba("+QString::number(c.red(),10)+","+QString::number(c.green(),10)+","+QString::number(c.blue(),10)+","+QString::number(a)+")}";

    setStyleSheet(str);
}

void DayWidget::setDate(const QDate &d)
{
    date=d;
    data = DataBase::getDate(d);
    QString text;
    text+=QString::number(date.day(),10);
    if(Arg::showChineseCalendar)text+=" "+Date::getLunarDate(date.year(),date.month(),date.day());
    dateLabel->setText(text);

//    QFont font = dateLabel->font();
//    font.setPixelSize(Const::MyTextSize);
//    dateLabel->setFont(font);
    reset();
}

void DayWidget::setBackgroundColor(const QColor &c)
{
    data->setBackgroundColor(c);
    resetStyleSheet();
}

void DayWidget::setBackgroundAlpha(double a)
{
    data->setBackgroundAlpha(a);
    resetStyleSheet();
}

void DayWidget::enterEvent(QEvent *event)
{
    if(Arg::GetFocusMode==1)setFocus();
    SetColorButton->setVisible(true);
    PlusButton->setVisible(true);
    QFrame::enterEvent(event);
}
void DayWidget::leaveEvent(QEvent *event)
{
    SetColorButton->setVisible(false);
    PlusButton->setVisible(false);
    QFrame::enterEvent(event);
}
void DayWidget::mousePressEvent(QMouseEvent *event)
{
    if(Arg::GetFocusMode==0)setFocus();
    QFrame::mousePressEvent(event);
}

bool DayWidget::MainWidgetHideOrShow(QKeyEvent *event,bool pd)//hide返回true
{
    bool re=false;
    if(Qt::KeyboardModifier::ControlModifier & event->modifiers())
    {
        if(this->EventWidget->MainWidget->parent()==MainScreen::mainScreen)
        {
            qDebug()<<"keyReleaseEvent Contral";
            this->EventWidget->MainWidget->setParent(this->EventWidget);
            this->EventWidget->MainWidget->move(0,0);
            re=true;
        }
        else if(pd==true && this->hasFocus())
        {
            qDebug()<<"keyPressEvent Contral";
            this->EventWidget->MainWidget->setParent(MainScreen::mainScreen);
            this->EventWidget->MainWidget->move(this->EventWidget->mapToGlobal(QPoint(0,0))-MainScreen::mainScreen->mapToGlobal(QPoint(0,0)));
        }
        this->EventWidget->MainWidget->setVisible(true);
        this->EventWidget->MainWidget->raise();
    }
    return re;
}

void DayWidget::keyPressEvent(QKeyEvent *event)
{
    //MainWidgetHideOrShow(event,true);
    QFrame::keyPressEvent(event);
}

bool DayWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if(MainWidgetHideOrShow(keyEvent,true))return true;
    }
    return QFrame::eventFilter(obj, event);
}

void DayWidget::setColor()
{
    QColor c = QColorDialog::getColor(data->Background_Color,this);
    if(c.isValid())
    {
        setBackgroundColor(c);
    }
}

void DayWidget::reset()
{
    //隐藏不必要的控件
    SetColorButton->setVisible(false);
    FileButton->setVisible(data->urls.size()>0);
    PlusButton->setVisible(false);
    resetStyleSheet();
    //子部件reset
    EventWidget->reset(data);
    FileMenu->reset(data);
}

void DayWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (Arg::lockDrag == false &&event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void DayWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;

    foreach(QUrl url, urls) {
        QString dateString = QString::number(date.year())+QString("-")+QString::number(date.month())+QString("-")+QString::number(date.day());
        QString DirPath = QCoreApplication::applicationDirPath()+"/SaveFile";
        QDir dir;
        if(!dir.exists(DirPath))
            qDebug()<<dir.mkdir(DirPath);
        DirPath+="/"+dateString;
        if(!dir.exists(DirPath))
            qDebug()<<dir.mkdir(DirPath);
        QString NewPath = DirPath+"/"+url.fileName();
        qDebug()<< url.toLocalFile();
        qDebug()<<url.fileName();
        qDebug()<<NewPath;
        if(!QFile(NewPath).exists())
        {
            QFile::copy(url.toLocalFile(),NewPath);
            data->urls.append(QUrl::fromLocalFile(NewPath));
        }
    }
    FileButton->setVisible(data->urls.size()>0);
    FileMenu->reset(data);
}

TextShowWindow::TextShowWindow(QWidget *parent):QFrame(parent)
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


void TextShowWindow::reset(DailyData *d)
{
    clear();
    data=d;
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
    for(int i=(int)(data->text.size()-1);i>=0;i--)
        addEvent((data->text[i]));
}

void TextShowWindow::resizeEvent(QResizeEvent *event)
{
    MainWidget->resize(event->size().width(),MainWidget->height());
}

EventTextButton* TextShowWindow::addEvent(EventData *d)
{
    EventTextButton *e = new EventTextButton(d,MainWidget);
    e->setMinimumHeight(Const::MyButtonSize);
    MainLayout->insertWidget(0,e);
    e->resize(this->width(),e->minimumHeight());
    MainWidget->resize(this->width(),data->text.size()*Const::MyButtonSize);
    e->setText(d->text);
    e->raise();
    e->setVisible(true);
    connect(e,SIGNAL(changed()),SLOT(resetSize()));
    return e;
}

void TextShowWindow::NewEvent()
{
    //构造EventData
    data->text.insert(0,new EventData);
    EventData *d = data->text[0];
    //设置初值
    EventTextButton *e = addEvent(d);
    e->SetText();
    if(e->text()==QString(""))
    {
        data->text.removeAt(0);
        MainLayout->removeWidget(static_cast<QWidget*>(e));
        e->setVisible(false);
    }
    //reset(data);
}
