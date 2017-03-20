#include "framelessmodalmovableshadowwidget.h"
#include <QMouseEvent>

FramelessModalMovableShadowWidget::FramelessModalMovableShadowWidget(QWidget *parent):
    BaseShadowWidget(parent)
{
    Qt::WindowFlags winFlags = windowFlags();
    setWindowFlags (winFlags | Qt::Window);
}

void FramelessModalMovableShadowWidget::mousePressEvent(QMouseEvent *pEv)
{
    m_lastMouseGlobalPos = pEv->globalPos();
    m_lastMainWindowPos = pos();

    pEv->accept();
}

void FramelessModalMovableShadowWidget::mouseMoveEvent(QMouseEvent *pEv)
{
    if(!m_lastMouseGlobalPos.isNull()) {
        QPoint curPt = pEv->globalPos() - m_lastMouseGlobalPos + m_lastMainWindowPos;
        move(curPt);
    }

    pEv->accept();
}

void FramelessModalMovableShadowWidget::mouseReleaseEvent(QMouseEvent *pEv)
{
    m_lastMouseGlobalPos = QPoint();

    pEv->accept();
}
