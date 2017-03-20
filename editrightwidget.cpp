#include "editrightwidget.h"
#include "ui_editrightwidget.h"
#include "semesterdialog.h"
#include "scheduledialog.h"

EditRightWidget::EditRightWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditRightWidget),
    m_pSemeWgt(Q_NULLPTR),
    m_pScheduleDlg(Q_NULLPTR)
{
    ui->setupUi(this);

//    setAttribute(Qt::WA_TranslucentBackground, false);
//    setAutoFillBackground(true);
////    setStyleSheet("QWidget#EditRightWidget{background:white}");

    //设置学期按钮样样式
    QPalette semester_pal = ui->semester_button->palette();
    semester_pal.setColor(QPalette::ButtonText, qRgb(102,102,102)); //设置按钮字体颜色
    ui->semester_button->setPalette(semester_pal);

    ui->semester_button->setStyleSheet("QPushButton{background-color:white; \
                                               border:1px solid #bbbbbb; \
                                               color: #666666} \
                                         QPushButton:pressed{background-color:#e4e4e4; \
                                        border:1px solid #cbcbcb; \
                                        color: #bdbdbd}");//设置按钮背景、边框颜色

    ui->semester_button->setFixedSize(100,40);//设置按钮大小

    //设置作息时间按钮样式
    QPalette schedule_pal = ui->schedule_button->palette();
    schedule_pal.setColor(QPalette::ButtonText, qRgb(102,102,102));
    ui->schedule_button->setPalette(schedule_pal);

    ui->schedule_button->setStyleSheet("QPushButton{background-color:white; \
                                       border:1px solid #bbbbbb; \
                                       color: #666666} \
                                 QPushButton:pressed{background-color:#e4e4e4; \
                                border:1px solid #cbcbcb; \
                                color: #bdbdbd}");//设置按钮背景、边框颜色
    ui->schedule_button->setFixedSize(100,40);//设置按钮大小

    //完成按钮样式
    QPalette ok_pal = ui->edit_confirm_button->palette();
    ok_pal.setColor(QPalette::ButtonText, qRgb(255,255,255));
    ui->edit_confirm_button->setPalette(ok_pal);

    ui->edit_confirm_button->setStyleSheet("QPushButton{background:#1250b5}"
                                           "QPushButton{border:1px solid #1250b5;}");//设置按钮背景、边框颜色
    ui->edit_confirm_button->setFixedSize(103,33);//设置按钮大小

    //显示设置学期显示结果label样式
    ui->semester_label->setFrameShape (QFrame::Box);
    ui->semester_label->setStyleSheet("border: 1px solid gray");
    ui->semester_label->setFixedSize(200,100);
    //显示设置作息时间显示结果label样式
    ui->schedule_label->setFrameShape (QFrame::Box);
    ui->schedule_label->setStyleSheet("border: 1px solid gray");
    ui->schedule_label->setFixedSize(200,100);
}

EditRightWidget::~EditRightWidget()
{
    delete ui;
}

//点击学期设置按钮对应的响应事件
void EditRightWidget::on_semester_button_clicked()
{
    if(m_pSemeWgt == Q_NULLPTR) {
        m_pSemeWgt = new SemesterDialog (this);
    }

    QWidget *pMainScreen = Q_NULLPTR;
    QWidgetList widgetLst = qApp->topLevelWidgets();
    for (QWidgetList::ConstIterator it = widgetLst.constBegin(), cend = widgetLst.constEnd(); it != cend; ++it) {
        QWidget *w = *it;
        const QMetaObject *metaObj = w->metaObject();
        QString sClassName = metaObj->className(); //必须经过这一步，否则判断不到.
        if(sClassName == "MainScreen") {
            pMainScreen = w;

            break;
        }
    }

    if(pMainScreen != Q_NULLPTR) {
        QPoint pos = pMainScreen->pos();
        QPoint centerPt = QPoint(pMainScreen->width() / 2 + pos.x(), pMainScreen->height() / 2 + pos.y());
        m_pSemeWgt->move(centerPt.x() - m_pSemeWgt->width() / 2, centerPt.y() - m_pSemeWgt->height() / 2);
    }
    else {

    }

    m_pSemeWgt->show ();
}

//点击作息时间按钮对应的响应事件
void EditRightWidget::on_schedule_button_clicked()
{
    if(m_pScheduleDlg == Q_NULLPTR){
        m_pScheduleDlg = new ScheduleDialog (this);
    }

    QWidget *pMainScreen = Q_NULLPTR;
    QWidgetList widgetLst = qApp->topLevelWidgets();
    for (QWidgetList::ConstIterator it = widgetLst.constBegin(), cend = widgetLst.constEnd(); it != cend; ++it) {
        QWidget *w = *it;
        const QMetaObject *metaObj = w->metaObject();
        QString sClassName = metaObj->className(); //必须经过这一步，否则判断不到.
        if(sClassName == "MainScreen") {
            pMainScreen = w;

            break;
        }
    }

    if(pMainScreen != Q_NULLPTR) {
        QPoint pos = pMainScreen->pos();
        QPoint centerPt = QPoint(pMainScreen->width() / 2 + pos.x(), pMainScreen->height() / 2 + pos.y());
        m_pScheduleDlg->move(centerPt.x() - m_pScheduleDlg->width() / 2, centerPt.y() - m_pScheduleDlg->height() / 2);
    }
    else {

    }

    m_pScheduleDlg->show ();
}

//点击完成之后需要接的操作
void EditRightWidget::on_edit_confirm_button_clicked()
{
    if(m_pSemeWgt == Q_NULLPTR) {
        m_pSemeWgt = new SemesterDialog (this);
    }

    m_pSemeWgt->show ();
}
