#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton
{
public:
    ColorButton(QWidget* parent = 0);
    void setColor(QColor c);
    QSize sizeHint() const {return QSize(3, 30);}

protected:
    void paintEvent(QPaintEvent* e);


private:
    QColor myColor;

};

#endif // COLORBUTTON_H
