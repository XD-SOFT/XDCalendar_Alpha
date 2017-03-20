#ifndef MASKLABEL_H
#define MASKLABEL_H

#include <QLabel>

class MaskLabel : public QLabel
{
    Q_OBJECT
public:
    MaskLabel(QWidget *parent=0);

    void setEnableShaodw(bool bEnabled);

signals:
    void clicked ();
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent (QMouseEvent*);

private:
    bool m_bEnabledShadow;
};

#endif // MASKLABEL_H
