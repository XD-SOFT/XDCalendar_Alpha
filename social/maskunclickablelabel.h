#ifndef MASKUNCLICKABLELABEL_H
#define MASKUNCLICKABLELABEL_H

#include <QLabel>

class MaskUnclickableLabel : public QLabel
{
    Q_OBJECT
public:
    MaskUnclickableLabel(double ratio = 0.8, QWidget *parent=0) : QLabel(parent), mRatio(ratio){}
protected:
    void paintEvent(QPaintEvent *);
private:
    double mRatio;
};

#endif // MASKLABELUNCLICKABLE_H
