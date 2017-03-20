#ifndef FRAMELESSMODALMOVABLESHADOWWIDGET_H
#define FRAMELESSMODALMOVABLESHADOWWIDGET_H

#include "baseshadowwidget.h"

class FramelessModalMovableShadowWidget : public BaseShadowWidget
{
public:
    FramelessModalMovableShadowWidget(QWidget *parent = Q_NULLPTR);


protected:
    void mousePressEvent(QMouseEvent *pEv) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QMouseEvent *pEv) Q_DECL_OVERRIDE;

    void mouseReleaseEvent(QMouseEvent *pEv) Q_DECL_OVERRIDE;

private:
    QPoint m_lastMouseGlobalPos;
    QPoint m_lastMainWindowPos;
};

#endif // FRAMELESSMODALMOVABLESHADOWWIDGET_H
