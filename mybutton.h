#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QPushButton>
#include <QMenu>
//#include <QWidget>
//#include <QIcon>
//#include <QString>
//#include <QMouseEvent>
//#include <QDebug>
//#include <QLineEdit>
//#include <QEventLoop>
//#include <QTimer>
//#include <QTime>
//#include <QSignalMapper>
//#include <QMenu>
//#include <QFileIconProvider>
#include <QFileInfo>
//#include <QVBoxLayout>
//#include <cstdint>
class DailyData;
class EventData;
class QTimer;
class QSignalMapper;

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = Q_NULLPTR):QPushButton(parent){init();}
    explicit MyButton(const QString &text, QWidget *parent = Q_NULLPTR):QPushButton(text,parent){init();}
    MyButton(const QIcon& icon, const QString &text, QWidget *parent = Q_NULLPTR):QPushButton(icon,text,parent){init();}
    ~MyButton(){}
signals:
    void doubleclicked();
    void singleclicked();
private:
    QTimer *timer = Q_NULLPTR;
    QTime *interval = Q_NULLPTR;
    void init();

protected:
    void mouseReleaseEvent(QMouseEvent *e);
private slots:
    void singleclickedOver();

};

//所有自定义的透明按钮
class TransparentButton : public QPushButton
{
    Q_OBJECT

public:
    explicit TransparentButton(QWidget *parent = Q_NULLPTR);
    explicit TransparentButton(const QString &text, QWidget *parent = Q_NULLPTR);
    TransparentButton(const QIcon& icon, const QString &text, QWidget *parent = Q_NULLPTR);
    ~TransparentButton(){}

    void setHoverdColor(const QColor &color);

    void setSelectedColor(const QColor &color);

    void setBackgroundColor(const QColor &color);

    void setSelected(bool bSelected);

    void setOpacity(qreal fOpacity);

    void setOpacityEnabled(bool bOpacityEnabled);

    void setTextColorChangedEnabled(bool bEnabled);

protected:
    void paintEvent (QPaintEvent* ev);

    void enterEvent(QEvent *event);

    void leaveEvent(QEvent *event);

private:
    QColor m_hoverdColor;

    QColor m_selectedColor;

    QColor m_backgroundColor;

    qreal m_fOpacity;

    bool m_bActived;

    bool m_bSelected;

    bool m_bEnbaledOpacity;

    bool m_bTextChangedColor;

//    bool m_bFirstShow;
};

//日期事件窗体按钮
class EventTextButton : public MyButton
{
    Q_OBJECT
public:
    explicit EventTextButton(EventData *d,QWidget *parent = Q_NULLPTR):MyButton(parent),data(d)
    {
        connect(this,SIGNAL(doubleclicked()),this,SLOT(SetText()));
        setObjectName("EventTextButton"+QString::number((rand()<<15) ^ rand(),16));
        deleteButton = new TransparentButton(QIcon(":/Icon/1143862_red.png"),"",this);
        connect(deleteButton,SIGNAL(clicked(bool)),this,SLOT(remove()));
        deleteButton->hide();
        resetStyleSheet();
    }
    ~EventTextButton();
private:
    EventData *data;
    TransparentButton *deleteButton;
public slots:
    void SetText(QString start = "");
    void remove();//删除此条事件
protected:
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
private slots:
    void resetStyleSheet();
    void chooseColor();
    void removeEventList();
signals:
    void changed();
};


//支持从中拖出url的Menu
class DragMenu : public QMenu
{
    Q_OBJECT
private:
    bool beginDrag;
    QPoint start;
    QVector<QAction*>ActionList;
    QAction *NowAction;
    QSignalMapper *Mapper;
    DailyData *data;
    QSignalMapper *openFileMapper;
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void leaveEvent(QEvent *);
public:
    DragMenu(QWidget *p = 0):QMenu(p)
    {
        beginDrag=true;
        NowAction=nullptr;
        Mapper=nullptr;
        openFileMapper=nullptr;
        data=nullptr;
    }
    ~DragMenu(){}

    void reset(QList<QUrl> urls);
    void reset(DailyData *d);
private slots:
    void setNowAction(int a){NowAction = reinterpret_cast<QAction*>(static_cast<std::uintptr_t> (a));}
    void openFile(QString);
};


class QLabel;
class IconChooserBtn: public QPushButton
{
    Q_OBJECT
public:
    explicit IconChooserBtn (const QPixmap& pixmap, const QFileInfo& iconFile, QWidget* parent = nullptr);
    ~IconChooserBtn () = default;
    QPixmap pixmap () const {return mPixmap;}
public slots:
    void onClicked (bool checked);
protected:
    void resizeEvent (QResizeEvent* event);
private:
    QFileInfo mIconFile;
    QPixmap mPixmap;
    QLabel* mIconLabel;
    QLabel* mInfoLabel;
};

#endif // MYBUTTON_H
