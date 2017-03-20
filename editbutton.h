#ifndef EDITBUTTON_H
#define EDITBUTTON_H

#include <QWidget>
#include <QPushButton>


class EditButton : public QPushButton
{
    //Q_OBJECT
public:
    EditButton(QWidget *parent = 0, bool del = false);

private:
    bool isDelete;

protected:
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent *e);

signals:

public slots:
};


#endif // EDITBUTTON_H
