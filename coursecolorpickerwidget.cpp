#include "coursecolorpickerwidget.h"
#include "ui_coursecolorpickerwidget.h"
#include "titlebar.h"

CourseColorPickerWidget::CourseColorPickerWidget(QWidget *parent) :
    ui(new Ui::CourseColorPickerWidget),
    FramelessModalMovableShadowWidget(parent)
{
    ui->setupUi(this);

    //添加titlebar.
    TitleBar *pTitleBar = new TitleBar(tr("选择颜色"), this);
    ui->pMainLayout->insertWidget(0, pTitleBar);
    connect (pTitleBar, &TitleBar::tbClose, [this] ()
    {
        close ();
    });

    setFixedSize(500, 400);
}

CourseColorPickerWidget::~CourseColorPickerWidget()
{
    delete ui;
}

void CourseColorPickerWidget::on_pConfirmBtn_clicked()
{
    QColor color;
    ui->pColorPickerWgt->getCurrentColor(color);

    emit colorSelectComplete(color);
}

void CourseColorPickerWidget::on_pCancelBtn_clicked()
{
    emit cancelSelectedColor();
}
