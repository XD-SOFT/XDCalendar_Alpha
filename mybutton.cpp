#include "mybutton.h"
#include "database.h"
#include "config.h"
#include <QColorDialog>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QResizeEvent>
#include <QMimeData>
#include <QDrag>
#include <QLabel>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>
#include <QLineEdit>
#include <QEventLoop>
#include <QTimer>
#include <QTime>
#include <QSignalMapper>
#include <QMenu>
#include <QFileIconProvider>
#include <QVBoxLayout>
#include <cstdint>
#include <termwidget.h>
#include "mybutton.h"
#include "MainScreen.h"
#include "teacher.h"
#include <QPainter>
#include <QStyleOption>


void MyButton::init()
{
    if(interval == Q_NULLPTR) {
        interval->start();
    }

    connect(this,SIGNAL(singleclicked()),this,SLOT(singleclickedOver()));

}

void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(timer != Q_NULLPTR) {
        int offset=200;
        if (interval->restart() < offset )
        {
            disconnect(timer, SIGNAL( timeout() ),this,SIGNAL(singleclicked()));  //终止发送单击事件
            timer->stop();       //停止timer
            emit doubleclicked();
        }
        else
        {
            timer->start( offset );
            connect(timer, SIGNAL( timeout() ), this, SIGNAL(singleclicked()));
        }
    }

    QPushButton::mouseReleaseEvent(e);
}

void MyButton::singleclickedOver()
{
    if(timer != Q_NULLPTR) {
        disconnect(timer, SIGNAL( timeout() ),this,SIGNAL(singleclicked()));  //终止发送单击事件
        timer->stop();       //停止timer
        //执行单击事件
    }
}

EventTextButton::~EventTextButton(){}

void EventTextButton::resetStyleSheet()
{
    QString thisname=QString(this->metaObject()->className())+"#"+this->objectName();
    QString str;

//    QFont font = this->font();
//    font.setPixelSize(Const::MyTextSize);
//    this->setFont(font);

    //静态样式
    QColor c=data->Background_Color;
    c.setRed(c.red()*0.8);
    c.setGreen(c.green()*0.8);
    c.setBlue(c.blue()*0.8);
    str+=thisname+" {text-align: left;color: white;background-color: rgba("+QString::number(c.red(),10)+","+QString::number(c.green(),10)+","+QString::number(c.blue(),10)+","+QString::number(data->Background_Alpha)+") ; border: 0px solid black}";
    //动态样式
    double a=data->Background_Alpha;
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

void EventTextButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::RightButton)
    {
        QMenu *menu = new QMenu(0);

        QAction *ChangeColor_action = menu->addAction(tr("更改颜色"));
        ChangeColor_action->setIcon(QIcon(":/Icon/1178331.png"));
        connect(ChangeColor_action, SIGNAL(triggered(bool)), SLOT(chooseColor()));

        if(data->EventListID>=0)
        {
            QAction *deleteList_action = menu->addAction(tr("批量删除"));
            deleteList_action->setIcon(QIcon(":/Icon/1190332.png"));
            connect(deleteList_action, SIGNAL(triggered(bool)), SLOT(removeEventList()));
        }
        //menu->addSeparator();

        menu->exec(QCursor::pos());
    }
    MyButton::mousePressEvent(e);
}

void EventTextButton::chooseColor()
{
    QColor c = QColorDialog::getColor(EventData::DefaultEventData.Background_Color);
    if(c.isValid())
    {
        EventData::setNowColor(c);
        data->setBackgroundColor(c);
        resetStyleSheet();
    }
}

void EventTextButton::removeEventList()
{
    DataBase::getDataBase()->remove(data->EventListID);
    MainScreen::mainScreen->gridView->reset();
}

void EventTextButton::SetText(QString start)
{
    if(start == "")
    {
        QEventLoop *loop = new QEventLoop;
        QLineEdit *LE = new QLineEdit(data->text,this);
        LE->selectAll();
        LE->setVisible(true);
        LE->raise();
        LE->move(QPoint(0,0));
        LE->resize(this->width(),this->height());
        connect(LE,SIGNAL(editingFinished()),loop,SLOT(quit()));
        LE->setFocus();
        loop->exec();
        setText(LE->text());
        data->text=LE->text();
        delete LE;
        delete loop;
    }
    else
    {
        setText(start);
        data->text=start;
    }
    setToolTip(text());
}


void EventTextButton::enterEvent(QEvent *event)
{
    deleteButton->setVisible(true);
    deleteButton->raise();
    deleteButton->resize(Const::MyButtonSize,Const::MyButtonSize);
    deleteButton->move(this->width()-Const::MyButtonSize,(this->height()-Const::MyButtonSize)/2);
    MyButton::enterEvent(event);
}
void EventTextButton::remove()
{
    DataBase::getDataBase()->remove(data);
    this->setVisible(false);
    emit changed();
}
void EventTextButton::leaveEvent(QEvent *event)
{\
    if(deleteButton != nullptr)deleteButton->setVisible(false);
    MyButton::leaveEvent(event);
}

void DragMenu::reset(QList<QUrl> urls)
{
    ActionList.clear();
    if(Mapper != nullptr)delete Mapper;
    if(openFileMapper != nullptr)delete openFileMapper;
    this->clear();
    //重新载入urls
    //data=d;
    Mapper = new QSignalMapper(this);
    openFileMapper = new QSignalMapper(this);
    connect(Mapper,SIGNAL(mapped(int)),SLOT(setNowAction(int)));
    connect(openFileMapper,SIGNAL(mapped(QString)),SLOT(openFile(QString)));
    //foreach(QUrl url, data->urls)
    foreach(QUrl url, urls)
    {
        //获取文件图标
        QFileInfo file_info(url.toLocalFile());\
        QFileIconProvider icon_provider;
        QIcon *icon = new QIcon;
        (*icon) = icon_provider.icon(file_info);
        QAction *a = new QAction(*icon,url.fileName());
        a->setToolTip(url.toLocalFile());
        this->addAction(a);
        ActionList.append(a);
        connect(a,SIGNAL(hovered()),Mapper,SLOT(map()));
        connect(a,SIGNAL(triggered(bool)),openFileMapper,SLOT(map()));
        Mapper->setMapping(a,(long)a);
        openFileMapper->setMapping(a,QString(url.toLocalFile()));
    }
}


void DragMenu::reset(DailyData *d)
{
    ActionList.clear();
    if(Mapper != nullptr)delete Mapper;
    if(openFileMapper != nullptr)delete openFileMapper;
    this->clear();
    //重新载入urls
    data=d;
    Mapper = new QSignalMapper(this);
    openFileMapper = new QSignalMapper(this);
    connect(Mapper,SIGNAL(mapped(int)),SLOT(setNowAction(int)));
    connect(openFileMapper,SIGNAL(mapped(QString)),SLOT(openFile(QString)));
    foreach(QUrl url, data->urls)
    {
        //获取文件图标
        QFileInfo file_info(url.toLocalFile());\
        QFileIconProvider icon_provider;
        QIcon *icon = new QIcon;
        (*icon) = icon_provider.icon(file_info);
        QAction *a = new QAction(*icon,url.fileName());
        a->setToolTip(url.toLocalFile());
        this->addAction(a);
        ActionList.append(a);
        connect(a,SIGNAL(hovered()),Mapper,SLOT(map()));
        connect(a,SIGNAL(triggered(bool)),openFileMapper,SLOT(map()));
        Mapper->setMapping(a,(long)a);
        openFileMapper->setMapping(a,QString(url.toLocalFile()));
    }
}

void DragMenu::openFile(QString s)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(s));
}


void DragMenu::leaveEvent(QEvent *event)
{
    NowAction = nullptr;
    QMenu::leaveEvent(event);
}
void DragMenu::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        beginDrag=false;
        start = event->globalPos();
    }
    else if(event->button() == Qt::RightButton)
    {
        if(NowAction!=nullptr)
        {
            //删除NowAction对应的url
            int i;
            for(i=0;i<ActionList.size();i++)
                if(ActionList[i]==NowAction)
                {
                    ActionList[i]->setVisible(false);
                    ActionList.removeAt(i);
                    QFile::remove(data->urls[i].toLocalFile());
                    data->urls.removeAt(i);
                    if(ActionList.size()==0)MainScreen::mainScreen->gridView->reset();
                    break;
                }
            NowAction = nullptr;
            qDebug()<<"delete "<<NowAction;
        }
    }
    QMenu::mousePressEvent(event);
}
void DragMenu::mouseMoveEvent(QMouseEvent *event)
{
    if(Arg::lockDrag == false &&beginDrag==false &&
        (abs((event->globalPos() - start).x())>1 || abs((event->globalPos() - start).y())>1))
    {
        beginDrag=true;
        QString file_name;
        int i;
        for(i=0;i<ActionList.size();i++)
            if(ActionList[i]==NowAction)
            {
                file_name = data->urls[i].toLocalFile();
                break;
            }
        if(i<ActionList.size())
        {
            QMimeData *QD = new QMimeData;
            QList<QUrl> urls;
            urls.append(QUrl::fromLocalFile(file_name));
            QD->setUrls(urls);
            QDrag *drag = new QDrag(this);
            drag->setMimeData(QD);
            drag->start();
            NowAction = nullptr;
        }
    }
    QMenu::mouseMoveEvent(event);
}
//============================================================================
IconChooserBtn::IconChooserBtn (const QPixmap& pixmap, const QFileInfo& mf, QWidget* parent): QPushButton (parent), mIconFile (mf), mPixmap (pixmap)
{
    QVBoxLayout* layout = new QVBoxLayout (this);
    mIconLabel = new QLabel (this);
    mInfoLabel = new QLabel ("Choosen", this);
    mInfoLabel->setFixedHeight (Const::SetupDialog::avatarInfoHeight);
    layout->addWidget (mIconLabel);
    layout->addWidget (mInfoLabel);
    setCheckable (true);
    connect (this, SIGNAL (clicked (bool)), this, SLOT (onClicked (bool)));
    onClicked (this->isChecked ());
    resize (this->size ());
    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void IconChooserBtn::onClicked (bool checked)
{
    if (checked)
    {
        mInfoLabel->setText ("Choosen");
        Arg::user->setIcon (mIconFile);
    }
    else
        mInfoLabel->setText ("");
}

void IconChooserBtn::resizeEvent (QResizeEvent* event)
{
    QPushButton::resizeEvent (event);
    mIconLabel->setPixmap (mPixmap.scaled (mIconLabel->size ()));
}

//=========================================================================================
TransparentButton::TransparentButton (QWidget* parent):
    QPushButton (parent)
{
//    setAttribute (Qt::WA_TranslucentBackground);
    m_hoverdColor = QColor("#2260c3");
    m_selectedColor = QColor(Qt::green);
    m_bActived = false;
    m_bSelected = false;
    m_bEnbaledOpacity = false;
    m_fOpacity = 1.0f;

    m_bTextChangedColor = false;
}

TransparentButton::TransparentButton(const QString &text, QWidget *parent):
    QPushButton (text, parent)
{
//    setAttribute (Qt::WA_TranslucentBackground);
    m_hoverdColor = QColor("#2260c3");
    m_selectedColor = QColor(Qt::green);
    m_bActived = false;
    m_bSelected = false;
    m_bEnbaledOpacity = false;
    m_fOpacity = 1.0f;

    m_bTextChangedColor = false;
}

TransparentButton::TransparentButton(const QIcon &icon, const QString &text, QWidget *parent):
    QPushButton(icon,text,parent)
{
    m_hoverdColor = QColor("#2260c3");
    m_selectedColor = QColor(Qt::green);
    m_bActived = false;
    m_bSelected = false;
    m_bEnbaledOpacity = false;
    m_fOpacity = 1.0f;

    m_bTextChangedColor = false;
}

void TransparentButton::setHoverdColor(const QColor &color)
{
    m_hoverdColor = color;
}

void TransparentButton::setSelectedColor(const QColor &color)
{
    m_selectedColor = color;
}

void TransparentButton::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
}

void TransparentButton::setSelected(bool bSelected)
{
    m_bSelected = bSelected;
 }

void TransparentButton::setOpacity(qreal fOpacity)
{
    m_fOpacity = fOpacity;

    update();
}

void TransparentButton::setOpacityEnabled(bool bOpacityEnabled)
{
    m_bEnbaledOpacity = bOpacityEnabled;
}

void TransparentButton::setTextColorChangedEnabled(bool bEnabled)
{
    m_bTextChangedColor = bEnabled;

//    if(bEnabled) {
//        setStyleSheet("color: gray");
//    }
}

void TransparentButton::enterEvent(QEvent *event)
{
//    setCursor(Qt::PointingHandCursor);

    m_bActived = true;
    repaint();

//    QPushButton::enterEvent(event);
}

void TransparentButton::leaveEvent(QEvent *event)
{
    m_bActived = false;
    repaint();

    //    QPushButton::leaveEvent(event);
}

//void TransparentButton::showEvent(QShowEvent *pShowEv)
//{
//    if(m_bFirstShow) {
//        m_bFirstShow = false;
//    }
//}

//void TransparentButton::setOpacity (double opacity)
//{
//    mOpacity = opacity;
//    update ();
//}

void TransparentButton::paintEvent (QPaintEvent* ev)
{
    if(m_bTextChangedColor){
        QString sText = text();
        QStringList sTextList = sText.split("\n");

        if(sTextList.size() == 2) {
            QPainter painter(this);
            QStyleOption opt;
            opt.init(this);
            style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

            QPalette pal = opt.palette;
            QBrush brush = pal.buttonText();
            QColor color = brush.color();
            painter.setPen(color);
            QTextOption option(Qt::AlignHCenter | Qt::AlignBottom);
            painter.drawText(QRect(0, 0, width(), height() / 2), sTextList.at(0), option);

            painter.setPen(Qt::darkGray);
            option.setAlignment(Qt::AlignHCenter | Qt::AlignTop);
            painter.drawText(QRect(0, height() / 2, width(), height() / 2), sTextList.at(1), option);

            return;
        }
    }

    QPushButton::paintEvent(ev);

//    QString sDateText = text();
//    QStringList dateList = sDateText.split("/n");

//    if(dateList.size() == 2) {
//        painter.setColor()
//        painter.drawText();
//    }

#if 0
    //draw background color.
    if(m_backgroundColor.isValid()) {
        p.setCompositionMode(QPainter::CompositionMode_Plus);
        if(m_bEnbaledOpacity) {
            m_backgroundColor.setAlpha(m_fOpacity);
        }

        p.fillRect(rect(), m_backgroundColor);
    }

      if(m_bActived) {
        p.setCompositionMode (QPainter::CompositionMode_DestinationAtop);

        if(m_bEnbaledOpacity) {
            m_hoverdColor.setAlpha(m_fOpacity);
        }

        p.fillRect(QRect(0, 0, width (), height ()), m_hoverdColor);
    }
    else if(m_bSelected) {
        if(m_bEnbaledOpacity) {
            m_selectedColor.setAlpha(m_fOpacity);
        }

        p.setCompositionMode(QPainter::CompositionMode_Overlay);
        p.fillRect(QRect(0, 0, width(), height()), m_selectedColor);
    }

#endif
}
