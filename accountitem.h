#ifndef ACCOUNTITEM_H
#define ACCOUNTITEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>


namespace Ui {
class AccountItem;
}

class AccountItem : public QWidget
{
    Q_OBJECT

public:
    explicit AccountItem(QWidget *parent = 0);
    ~AccountItem();

private:
    QLabel *account_number;
    QPushButton *delede_button;
    bool mouse_press;

public:
    void setAccountNumber(QString account_text);
    QString getAccountNumber();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::AccountItem *ui;

signals:
    void showAccount(QString);
    void removeAccount(QString);
public slots:
    void removeAccount();
};

#endif // ACCOUNTITEM_H
