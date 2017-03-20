#include "coursetreewidget.h"
#include "ui_coursetreewidget.h"
#include <qbitmap.h>

CourseTreeWidget::CourseTreeWidget(int nRelatedRow, QString fileName , QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseTreeWidget),
    m_nRelatedRow(nRelatedRow),
    m_sFileName(fileName)
{
    ui->setupUi(this);

    QFontMetrics fontM = fontMetrics();
    QString sElideText = fontM.elidedText(m_sFileName, Qt::ElideRight, ui->pTextLb->width());
    ui->pTextLb->setText(sElideText);
    ui->pTextLb->setStyleSheet("QLabel{background-color:transparent} QToolTip {background-color:white}");

    if(m_sFileName.size() > sElideText.size()) {
        ui->pTextLb->setToolTip(m_sFileName);
    }


////    QPixmap icon;
////    icon.load(":/image/001.jpg");
////    ui->downloadButton->setFixedSize( icon.width(), icon.height() );
////    ui->downloadButton->setMask(icon.createHeuristicMask(true));
////    ui->downloadButton->setIcon(icon);
////    ui->downloadButton->setIconSize(QSize(icon.width(), icon.height()));
////    ui->downloadButton->setToolTip("上传");
////    ui->downloadButton->setStyleSheet("background-color:transparent");
//    ui->downloadButton->installEventFilter(this);

////    icon.load(":/image/002.jpg");
////    ui->deleteButton->setFixedSize(icon.width(), icon.height());
////    ui->deleteButton->setMask(icon.createHeuristicMask());
////    ui->deleteButton->setIcon(icon);
////    ui->deleteButton->setIconSize(QSize(icon.width(), icon.height()));
////    ui->deleteButton->setToolTip("删除");

    QPixmap icon;
    icon.load(":/image/001.png");
    ui->downloadButton->setFixedSize( icon.width(), icon.height() );
    ui->downloadButton->setMask(icon.createHeuristicMask(true));
    ui->downloadButton->setIcon(icon);
    ui->downloadButton->setIconSize(QSize(icon.width(), icon.height()));
    ui->downloadButton->setToolTip(tr("下载"));
    ui->downloadButton->setStyleSheet("QPushButton{background-color:transparent} QToolTip {background-color:white}");
    ui->downloadButton->installEventFilter(this);

    icon.load(":/image/002.png");
    ui->deleteButton->setFixedSize(icon.width(), icon.height());
    ui->deleteButton->setMask(icon.createHeuristicMask());
    ui->deleteButton->setIcon(icon);
    ui->deleteButton->setIconSize(QSize(icon.width(), icon.height()));
    ui->deleteButton->setToolTip(tr("删除"));

    ui->deleteButton->setStyleSheet("QPushButton{background-color:transparent} QToolTip {background-color:white}");
    ui->deleteButton->installEventFilter(this);

    icon.load(":/image/upload.png");
    ui->pUploadBtn->setFixedSize(icon.width(), icon.height());
    ui->pUploadBtn->setMask(icon.createHeuristicMask());
    ui->pUploadBtn->setIcon(icon);
    ui->pUploadBtn->setIconSize(QSize(icon.width(), icon.height()));
    ui->pUploadBtn->setToolTip(tr("上传"));
    ui->pUploadBtn->setStyleSheet("QPushButton{background-color:transparent} QToolTip {background-color:white}");
    ui->pUploadBtn->installEventFilter(this);
}

CourseTreeWidget::~CourseTreeWidget()
{
    delete ui;
}

void CourseTreeWidget::setRelatedRow(int nRow)
{
    m_nRelatedRow = nRow;
}

void CourseTreeWidget::on_downloadButton_clicked()
{
    emit downloadOK(m_nRelatedRow);
}

void CourseTreeWidget::on_deleteButton_clicked()
{
    emit deleteOK(m_nRelatedRow);
}


//利用事件过滤功能完成按钮hover时改变样式的功能   12/23 by 高峰
bool CourseTreeWidget::eventFilter(QObject *watched, QEvent *e) {
    // 鼠标进入或离开按钮时设置为不同状态的图标
    if (watched == ui->downloadButton) {
        if (e->type() == QEvent::Enter) {
            QPixmap upload_icon;
            upload_icon.load(":/image/download_1.png");
            ui->downloadButton->setIcon(upload_icon);
        } else if (e->type() == QEvent::Leave) {
            QPixmap upload_icon;
            upload_icon.load(":/image/001.png");
            ui->downloadButton->setIcon(upload_icon);
        }
    }
   else if (watched == ui->deleteButton) {
        if (e->type() == QEvent::Enter) {
            QPixmap delete_icon;
            delete_icon.load(":/image/02.png");
            ui->deleteButton->setIcon(delete_icon);
        } else if (e->type() == QEvent::Leave) {
            QPixmap delete_icon;
            delete_icon.load(":/image/002.png");
            ui->deleteButton->setIcon(delete_icon);
        }
    }
    else if(watched == ui->pUploadBtn) {
        if (e->type() == QEvent::Enter) {
            QPixmap icon;
            icon.load(":/image/upload_pressed.png");
            ui->pUploadBtn->setIcon(icon);
        } else if (e->type() == QEvent::Leave) {
            QPixmap icon;
            icon.load(":/image/upload.png");
            ui->pUploadBtn->setIcon(icon);
        }
    }

    return QWidget::eventFilter(watched, e);
}

void CourseTreeWidget::resizeEvent(QResizeEvent *pEv)
{
    if(!m_sFileName.isEmpty()) {
        QFontMetrics fontM = fontMetrics();
        QString sElideText = fontM.elidedText(m_sFileName, Qt::ElideRight, ui->pTextLb->width());
        ui->pTextLb->setText(sElideText);

        if(m_sFileName.size() > sElideText.size()) {
            ui->pTextLb->setToolTip(m_sFileName);
        }
    }
}

void CourseTreeWidget::on_pUploadBtn_clicked()
{
    emit uploadFile(m_nRelatedRow);
}
