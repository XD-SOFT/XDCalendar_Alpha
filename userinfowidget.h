#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class IconSelectWidget;
}

class QButtonGroup;

class IconSelectWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    IconSelectWidget(QWidget *parent = 0);
    ~IconSelectWidget();

private slots:
    void iconBtnClicked(int index);

    void upLoadIcon();

    void on_pUploadBtn_clicked();

    void selectUploadImg();

signals:
    void iconSelectComplete(const QPixmap &pixmap);

private:
    Ui::IconSelectWidget *ui;
    QButtonGroup *m_pBtnGrp;
};

namespace Ui {
class UserInfoWidget;
}

class Teacher;
///Mark，这里为什么用了要给QLabel，一个QPushButton,因为用label无论怎么调名字都不能跟头像紧挨，所以暂时这么处理，什么原因，再查.

class UserInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoWidget(QWidget *parent = 0);
    ~UserInfoWidget();

protected:
    void paintEvent(QPaintEvent*);

    void resizeEvent(QResizeEvent *event);

private slots:
    void reload();

    void link(const Teacher* user = Q_NULLPTR);

    void unlink();

    void showHeaderIconSelectWidget();

    void setHeaderIcon(const QPixmap &pix);

private:
    Ui::UserInfoWidget *ui;
    const Teacher* mLinkedUser = Q_NULLPTR;
    IconSelectWidget *m_pIconSelectWgt = Q_NULLPTR;
};

#endif // USERINFOWIDGET_H
