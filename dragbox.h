#ifndef DRAGBOX_H
#define DRAGBOX_H
//响应主界面大小与位置更改的黄色虚线边框widget
#include <QDialog>
#include <QLayout>
#include "config.h"
#include <QMessageBox>
#include <QMouseEvent>
#include "mybutton.h"

class DraggableBox : public QDialog
{
    Q_OBJECT
public:
    DraggableBox(QWidget *parent,QWidget *screen):QDialog(parent),Screen(screen)
    {
        setObjectName(QString("MainDragBox"));
        FrameSize=4;FrameOffset=10;
        PosrW=PosrH=SizerW=SizerH=0;
        QVBoxLayout *DialogVLayout = new QVBoxLayout(this);
        DialogVLayout->setSpacing(0);
        DialogVLayout->setMargin(0);
        centralWidget = new QWidget(this);
        DialogVLayout->addWidget(centralWidget);
        centralWidget->setObjectName(QString("DragBoxcentralWidget"));
        setStyleSheet("QWidget#DragBoxcentralWidget {background-color : rgba(0,0,0,0.4);border: 5px dashed rgba(205,228,108,0.7)}");
        this->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);//隐藏边框/菜单图标
        this->setAttribute(Qt::WA_TranslucentBackground);//隐藏背景

        OkButton = new TransparentButton(QIcon(":/Icon/1190933.png"),"",this);
        OkButton->setFixedSize(QSize(Const::MyButtonSize*2, Const::MyButtonSize*2));
        connect(OkButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
        CancelButton = new TransparentButton(QIcon(":/Icon/1190738.png"),"",this);
        CancelButton->setFixedSize(QSize(Const::MyButtonSize*2, Const::MyButtonSize*2));
        connect(CancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
        this->setMouseTracking(true);
        centralWidget->setMouseTracking(true);

        TopHBox = new QHBoxLayout;
        TopHBox->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        TopHBox->addWidget(OkButton);
        TopHBox->addWidget(CancelButton);
        TopHBox->setSpacing(0);
        TopHBox->setMargin(0);

        MainVBox = new QVBoxLayout(centralWidget);
        MainVBox->addLayout(TopHBox);
        MainVBox->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        MainVBox->setSpacing(0);
        MainVBox->setMargin(FrameSize);

        resize(Screen->width()+FrameSize*2,Screen->height()+FrameSize*2);
        move(Screen->pos()-QPoint(FrameSize,FrameSize));
        startPos=Screen->pos();
        startSize=Screen->size();
    }
    ~DraggableBox(){}
public slots:
    void reject()
    {
        if((startPos == Screen->pos() && startSize == Screen->size())
           || QMessageBox::warning(this,tr("教师客户端"),tr("确认要放弃当前调整并恢复到原有位置吗？"),QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            move(startPos);
            resize(startSize);
            Screen->move(startPos);
            Screen->resize(startSize);
            QDialog::reject();
        }
        else return;
    }
protected:
    void mouseMoveEvent(QMouseEvent *event)
    {
        if(PosrW || PosrH || SizerW || SizerH)
        {
            int offsetx=startPoint.x()-event->globalX();
            int offsety=startPoint.y()-event->globalY();
            Screen->move(savePos-QPoint(PosrW*offsetx,PosrH*offsety));
            Screen->resize(saveSize-QSize(SizerW*offsetx,SizerH*offsety));
            setCursor(NowCursorShape);
            resize(Screen->width()+FrameSize*2,Screen->height()+FrameSize*2);
            move(Screen->pos()-QPoint(FrameSize,FrameSize));
        }
        else
        {
            if(event->x()<=FrameOffset && event->y()<=FrameOffset)
                setCursor(Qt::SizeFDiagCursor);
            else if(event->x()>=width()-FrameOffset && event->y()>=height()-FrameOffset)
                setCursor(Qt::SizeFDiagCursor);
            else if(event->x()>=width()-FrameOffset && event->y()<=FrameOffset)
                setCursor(Qt::SizeBDiagCursor);
            else if(event->x()<=FrameOffset && event->y()>=height()-FrameOffset)
                setCursor(Qt::SizeBDiagCursor);
            else if(event->x()<=FrameOffset || event->x()>=width()-FrameOffset)
                setCursor(Qt::SizeHorCursor);
            else if(event->y()<=FrameOffset || event->y()>=height()-FrameOffset)
                setCursor(Qt::SizeVerCursor);
            else setCursor(Qt::SizeAllCursor);
        }
        QDialog::mouseMoveEvent(event);
    }
    void mousePressEvent(QMouseEvent *event)
    {
        savePos=Screen->pos();
        saveSize=Screen->size();
        startPoint = event->globalPos();
        if(event->x()<=FrameOffset)PosrW=1,SizerW=-1;
        else if(event->x()>=width()-FrameOffset)SizerW=1;
        if(event->y()<=FrameOffset)PosrH=1,SizerH=-1;
        else if(event->y()>=height()-FrameOffset)SizerH=1;
        if(!(PosrW || PosrH || SizerW || SizerH))PosrH=PosrW=1;
        QDialog::mousePressEvent(event);
        NowCursorShape = cursor().shape();
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        PosrW=PosrH=SizerW=SizerH=0;
        QDialog::mouseReleaseEvent(event);
    }
private:
    int FrameSize;
    QWidget *centralWidget,*Screen;
    QVBoxLayout *MainVBox;
    QHBoxLayout *TopHBox;
    TransparentButton *OkButton,*CancelButton;

    //跟踪鼠标移动信息
    QPoint startPoint,savePos,startPos;//按下鼠标左键时的坐标/窗体坐标/Dialog打开时的窗体坐标
    Qt::CursorShape NowCursorShape;
    QSize saveSize,startSize;
    int FrameOffset;//响应拖拽事件的边框偏移量
    int PosrW,PosrH,SizerW,SizerH;//坐标/大小在w,h方向是否响应鼠标移动
public slots:
};
#endif // DRAGBOX_H
