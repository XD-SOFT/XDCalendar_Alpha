#include "baseshadowwidget.h"

#include <QPainter>
#include <QApplication>
#include <QPushButton>

class BaseShadowWidgetPrivate {
    friend class BaseShadowWidget;

   BaseShadowWidgetPrivate(BaseShadowWidget *painterDevice):
       m_pPtDevice(painterDevice)
   {
       resetShadowWidth(true);
   }

   void caculateAndDrawTopLeftCorner(int &nTopLeftCornerWidth, int &nTopLeftCornerHeight, QPainter *painter)
   {
       if(m_bLeftEnabled && m_bTopEnabled) {
           //左侧顶部阴影都启用情况.
           nTopLeftCornerWidth = m_nLeftShadowWidth;
           nTopLeftCornerHeight = m_nTopShadowWidth;
       }
       else if(m_bLeftEnabled) {
           //只有左侧启用.
           nTopLeftCornerWidth = m_nLeftShadowWidth;;
           nTopLeftCornerHeight = m_nLeftShadowWidth;
       }
       else if(m_bTopEnabled) {
           nTopLeftCornerWidth = m_nTopShadowWidth;
           nTopLeftCornerHeight = m_nTopShadowWidth;
       }

       if((nTopLeftCornerHeight != 0) && (nTopLeftCornerHeight != 0)) {
           //top left corner.
           if(m_topLeftPix.isNull()) {
               m_topLeftPix.load(":/Icon/shadow_topleftcorner.png");
           }

           if(!m_topLeftPix.isNull()) {
               painter->drawPixmap(m_nShadowMargin - nTopLeftCornerWidth, m_nShadowMargin - nTopLeftCornerHeight, nTopLeftCornerWidth, nTopLeftCornerHeight, m_topLeftPix);
           }
       }
   }


   void caculateAndDrawTopRightCorner(int &nTopRightCornerWidth, int &nTopRightCornerHeight, QPainter *painter)
   {
       if(m_bRightEnabled && m_bTopEnabled) {
           //左侧顶部阴影都启用情况.
           nTopRightCornerWidth = m_nRightShadowWidth;
           nTopRightCornerHeight = m_nTopShadowWidth;
       }
       else if(m_bRightEnabled) {
           //只有左侧启用.
           nTopRightCornerWidth = m_nRightShadowWidth;
           nTopRightCornerHeight = m_nRightShadowWidth;
       }
       else if(m_bTopEnabled) {
           nTopRightCornerWidth = m_nTopShadowWidth;
           nTopRightCornerHeight = m_nTopShadowWidth;
       }

       if((nTopRightCornerWidth != 0) && (nTopRightCornerHeight != 0)) {
           //top left corner.
           if(m_topRightPix.isNull()) {
               m_topRightPix.load(":/Icon/shadow_toprightcorner.png");
           }

           if(!m_topRightPix.isNull()) {
               painter->drawPixmap(m_pPtDevice->width() - m_nShadowMargin, m_nShadowMargin - nTopRightCornerHeight, nTopRightCornerWidth, nTopRightCornerHeight, m_topRightPix);
           }
       }
   }

   void caculateAndDrawBottomLeftCorner(int &nBottomLeftCornerWidth, int &nBottomLeftCornerHeight, QPainter *painter)
   {
       if(m_bLeftEnabled && m_bBottomEnabled) {
           //左侧顶部阴影都启用情况.
           nBottomLeftCornerWidth = m_nLeftShadowWidth;
           nBottomLeftCornerHeight = m_nBottomShadowWidth;
       }
       else if(m_bLeftEnabled) {
           //只有左侧启用.
           nBottomLeftCornerWidth = m_nLeftShadowWidth;
           nBottomLeftCornerHeight = m_nLeftShadowWidth;
       }
       else if(m_bBottomEnabled) {
           nBottomLeftCornerWidth = m_nBottomShadowWidth;
           nBottomLeftCornerHeight = m_nBottomShadowWidth;
        }

       if((nBottomLeftCornerWidth != 0) && (nBottomLeftCornerHeight != 0)) {
           //top left corner.
           if(m_bottomLeftPix.isNull()) {
               m_bottomLeftPix.load(":/Icon/shadow_bottomleftcorner.png");
           }

           if(!m_bottomLeftPix.isNull()) {
               painter->drawPixmap(m_nShadowMargin - nBottomLeftCornerWidth, m_pPtDevice->height() - m_nShadowMargin, nBottomLeftCornerWidth, nBottomLeftCornerHeight, m_bottomLeftPix);
           }
       }
   }

   void caculateAndDrawBottomRightCorner(int &nBottomRightCornerWidth, int &nBottomRightCornerHeight, QPainter *painter)
   {
       if(m_bRightEnabled && m_bBottomEnabled) {
           //左侧顶部阴影都启用情况.
           nBottomRightCornerWidth = m_nRightShadowWidth;
           nBottomRightCornerHeight = m_nBottomShadowWidth;
       }
       else if(m_bRightEnabled) {
           //只有左侧启用.
           nBottomRightCornerWidth = m_nRightShadowWidth;
           nBottomRightCornerHeight = m_nRightShadowWidth;
       }
       else if(m_bBottomEnabled) {
           nBottomRightCornerWidth = m_nBottomShadowWidth;
           nBottomRightCornerHeight = m_nBottomShadowWidth;
       }

       if((nBottomRightCornerWidth != 0) && (nBottomRightCornerHeight != 0)) {
           //top left corner.
           if(m_bottomRightPix.isNull()) {
               m_bottomRightPix.load(":/Icon/shadow_bottomrightcorner.png");
           }

           if(!m_bottomRightPix.isNull()) {
               painter->drawPixmap(m_pPtDevice->width() - m_nShadowMargin, m_pPtDevice->height() - m_nShadowMargin, nBottomRightCornerWidth, nBottomRightCornerHeight, m_bottomRightPix);
           }
       }
   }

   QPixmap m_leftPix;
   QPixmap m_topLeftPix;
   QPixmap m_bottomLeftPix;
   QPixmap m_bottomPix;
   QPixmap m_topPix;
   QPixmap m_topRightPix;
   QPixmap m_rightPix;
   QPixmap m_bottomRightPix;

   QWidget *m_pPtDevice;

    int m_nShadowMargin = 30; //四周留出绘制阴影的margin.
    int m_nLeftShadowWidth; //阴影实际大小.
    int m_nTopShadowWidth;
    int m_nRightShadowWidth;
    int m_nBottomShadowWidth;

    bool m_bShadowEnabled = true;
    bool m_bLeftEnabled = true;
    bool m_bRightEnabled = true;
    bool m_bTopEnabled = true;
    bool m_bBottomEnabled = true;
    bool m_bOpacityDisabled = false; //透明是否禁用，这个如果使用阴影必须为false,因为使用阴影四周必须透明.

    void resetShadowWidth(bool bEnabled) {
        if(bEnabled) {
            m_nShadowMargin = 30;
            m_nLeftShadowWidth = 30;
            m_nTopShadowWidth = 30;
            m_nRightShadowWidth = 30;
            m_nBottomShadowWidth = 30;
        }
        else {
            m_nShadowMargin = 0;
            m_nLeftShadowWidth = 0;
            m_nTopShadowWidth = 0;
            m_nRightShadowWidth = 0;
            m_nBottomShadowWidth = 0;
        }
    }
};

BaseShadowWidget::BaseShadowWidget(QWidget *parent) :
    QWidget(parent),
    m_pPrivate(new BaseShadowWidgetPrivate(this))
{
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
}

BaseShadowWidget::~BaseShadowWidget()
{
    delete m_pPrivate;
    m_pPrivate = Q_NULLPTR;
}

void BaseShadowWidget::setShadowWidth(int nShadowWidth)
{
    m_pPrivate->m_nShadowMargin = nShadowWidth;
//    m_pPrivate->m_nLeftShadowWidth = nShadowWidth;
//    m_pPrivate->m_nTopShadowWidth = nShadowWidth; //暂时这个不做处理
//    m_pPrivate->m_nRightShadowWidth = nShadowWidth;
//    m_pPrivate->m_nBottomShadowWidth = nShadowWidth;
}

int BaseShadowWidget::getShadowMargin() const
{
    //如果没启用阴影，则阴影大小值设置为0.
//    int nValidShadowWidth = m_pPrivate->m_bShadowEnabled ? m_pPrivate->m_nLeftShadowWidth : 0;

    return m_pPrivate->m_nShadowMargin;
}

void BaseShadowWidget::setShadowWidthBySide(BaseShadowWidget::ShadowSide side, int nShadowWidth)
{
    switch(side) {
    case Left: {
        m_pPrivate->m_nLeftShadowWidth = nShadowWidth;
    }
        break;

    case Top: {
        m_pPrivate->m_nTopShadowWidth = nShadowWidth;
    }
        break;

    case Right: {
        m_pPrivate->m_nRightShadowWidth = nShadowWidth;
    }
        break;

    case Bottom: {
        m_pPrivate->m_nBottomShadowWidth = nShadowWidth;
    }
        break;
    }
}

int BaseShadowWidget::getShadowWidthBySide(BaseShadowWidget::ShadowSide side) const
{
    switch(side) {
    case Left: {
        return m_pPrivate->m_nLeftShadowWidth;
    }
        break;

    case Top: {
        return m_pPrivate->m_nTopShadowWidth;
    }
        break;

    case Right: {
        return m_pPrivate->m_nRightShadowWidth;
    }
        break;

    case Bottom: {
        return m_pPrivate->m_nBottomShadowWidth;
    }
        break;
    }
}

void BaseShadowWidget::setShadowEnabled(bool bEnabled)
{
    m_pPrivate->m_bShadowEnabled = bEnabled;
    m_pPrivate->resetShadowWidth(bEnabled);
}

bool BaseShadowWidget::isShaodwEnabled() const
{
    return m_pPrivate->m_bShadowEnabled;
//    return (m_pPrivate->m_bLeftEnabled && m_pPrivate->m_bTopEnabled && m_pPrivate->m_bRightEnabled && m_pPrivate->m_bBottomEnabled);
}

void BaseShadowWidget::setShadowEnabled(ShadowSide side, bool bEnabled)
{
    switch(side) {
    case Left: {
         m_pPrivate->m_bLeftEnabled = bEnabled;
         m_pPrivate->m_nLeftShadowWidth = bEnabled ? 30 : 0;
    }
        break;

    case Top: {
         m_pPrivate->m_bTopEnabled = bEnabled;
         m_pPrivate->m_nTopShadowWidth = bEnabled ? 30 : 0;
    }
        break;

    case Right: {
         m_pPrivate->m_bRightEnabled = bEnabled;
         m_pPrivate->m_nRightShadowWidth = bEnabled ? 30 : 0;
    }
        break;

    case Bottom: {
        m_pPrivate->m_bBottomEnabled = bEnabled;
        m_pPrivate->m_nBottomShadowWidth = bEnabled ? 30 : 0;
    }
        break;
    }

    m_pPrivate->m_bShadowEnabled = bEnabled ? true : m_pPrivate->m_bShadowEnabled;
}

bool BaseShadowWidget::isShaodwEnabled(ShadowSide side) const
{
    switch(side) {
    case Left: {
        return m_pPrivate->m_bLeftEnabled;
    }

    case Top: {
        return m_pPrivate->m_bTopEnabled;
    }

    case Right: {
        return m_pPrivate->m_bRightEnabled;
    }

    case Bottom: {
        return m_pPrivate->m_bBottomEnabled;
    }
    }
}

void BaseShadowWidget::setShadowPixmapBySide(BaseShadowWidget::ShadowSide side, const QString &sPixPathName)
{
    switch(side) {
    case Left: {
        m_pPrivate->m_leftPix.load(sPixPathName);
        repaint(0, 0, width(), m_pPrivate->m_nShadowMargin);
    }

    case Top: {
        m_pPrivate->m_topPix.load(sPixPathName);
        repaint(0, 0, width(), m_pPrivate->m_nShadowMargin);
    }

    case Right: {
        m_pPrivate->m_rightPix.load(sPixPathName);
        repaint(width() - m_pPrivate->m_nShadowMargin, 0, m_pPrivate->m_nShadowMargin, height());
    }

    case Bottom: {
        m_pPrivate->m_bottomPix.load(sPixPathName);
        repaint(0, height()- m_pPrivate->m_nShadowMargin, width(), m_pPrivate->m_nShadowMargin);
    }
    }

    if(side == Right) {

    }
}

void BaseShadowWidget::setOpacityDisabled(bool bDisabled)
{
    m_pPrivate->m_bOpacityDisabled = bDisabled;
}

//void BaseShadowWidget::setTopShadowEnabled(bool bEnabled)
//{
//    Q_UNUSED(bEnabled)
////    m_pPrivate->m_bTopEnabled = bEnabled;

////    repaint(0, 0, width(), m_pPrivate->m_nShadowMargin);
//}

//void BaseShadowWidget::setBottomShadowEnabled(bool bEnabled)
//{
//    Q_UNUSED(bEnabled)
////    m_pPrivate->m_bBottomEnabled = bEnabled;

////    repaint(0, height() - m_pPrivate->m_nShadowMargin, width(), m_pPrivate->m_nShadowMargin);
//}

//void BaseShadowWidget::setLeftShadowEnabled(bool bEnabled)
//{
//     Q_UNUSED(bEnabled)
////    m_pPrivate->m_bLeftEnabled = bEnabled;

////    repaint(0, 0, m_pPrivate->m_nShadowMargin, height());
//}

//void BaseShadowWidget::setRightShadowEnabled(bool bEnabled)
//{
//     Q_UNUSED(bEnabled)
//    //    m_pPrivate->m_bRightEnabled = bEnabled;
//}

void BaseShadowWidget::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev)

//    painter->fillRect(m_pPrivate->m_nLeftShadowWidth, m_pPrivate->m_nTopShadowWidth, width() - m_pPrivate->m_nLeftShadowWidth - m_pPrivate->m_nRightShadowWidth,
//                      height() - m_pPrivate->m_nTopShadowWidth - m_pPrivate->m_nBottomShadowWidth, Qt::white);
    if(!m_pPrivate->m_bOpacityDisabled) {
        painter->fillRect(m_pPrivate->m_nShadowMargin, m_pPrivate->m_nShadowMargin, width() - 2 * m_pPrivate->m_nShadowMargin,
                          height() - 2 * m_pPrivate->m_nShadowMargin, Qt::white);
    }
    else {
        painter->fillRect(rect(), Qt::white);
    }
}

void BaseShadowWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    drawExcludeShadowPart(&painter, event);
    //阴影绘制原理.
    //
    //topLeftCorner___|___top________|_topRightCorner
    //                |              |
    //   left         |              |right
    //                |              |
    //            ____|______________|_
    //bottomLeftCorner|    bottom    | bottomRightCorner
    //
    if(m_pPrivate->m_bShadowEnabled) {
        if(!m_pPrivate->m_bOpacityDisabled) {
            painter.setCompositionMode(QPainter::CompositionMode_Source);
        }
        else {
            painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        }

        painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
//        int nBottomCornerStartY = height() - m_pPrivate->m_nBottomShadowWidth; //计算出底部阴影绘制y起点，减掉底部阴影宽度.
//        int nRightStartX = width() -  m_pPrivate->m_nRightShadowWidth; //计算出右侧阴影绘制x起点，减掉右侧阴影宽度.

        //定义左上角阴影宽高.
        int nTopLeftCornerWidth = 0;
        int nTopLeftCornerHeight = 0;
        m_pPrivate->caculateAndDrawTopLeftCorner(nTopLeftCornerWidth, nTopLeftCornerHeight, &painter);

        //右上角阴影宽高.
        int nTopRightCornerWidth = 0;
        int nTopRightCornerHeight = 0;
        m_pPrivate->caculateAndDrawTopRightCorner(nTopRightCornerWidth, nTopRightCornerHeight, &painter);

        //左下角阴影宽高.
        int nBottomLeftCornerWidth = 0;
        int nBottomLeftCornerHeight = 0;
        m_pPrivate->caculateAndDrawBottomLeftCorner(nBottomLeftCornerWidth, nBottomLeftCornerHeight, &painter);

        //右下角阴影宽高.
        int nBottomRightCornerWidth = 0;
        int nBottomRightCornerHeight = 0;
        m_pPrivate->caculateAndDrawBottomRightCorner(nBottomRightCornerWidth, nBottomRightCornerHeight, &painter);

//        int nVShadowHeight = height() - m_pPrivate->m_nTopShadowWidth - m_pPrivate->m_nBottomShadowWidth;
//        QPixmap pixmap;

        if(m_pPrivate->m_bLeftEnabled) {
            int nVLeftShadowHeight = height() - 2 * m_pPrivate->m_nShadowMargin;

            if(m_pPrivate->m_leftPix.isNull()) {
                //left side.
                m_pPrivate->m_leftPix.load(":/Icon/shadow_left.png");
            }

            if(!m_pPrivate->m_leftPix.isNull()) {
                painter.drawPixmap(m_pPrivate->m_nShadowMargin - m_pPrivate->m_nLeftShadowWidth, m_pPrivate->m_nShadowMargin, m_pPrivate->m_nLeftShadowWidth, nVLeftShadowHeight, m_pPrivate->m_leftPix);
            }
        }

        //bottom side.
        if(m_pPrivate->m_bBottomEnabled) {
            if(m_pPrivate->m_bottomPix.isNull()) {
                m_pPrivate->m_bottomPix.load(":/Icon/shadow_bottom.png");
            }

            int nBottomCornerStartY = height() - m_pPrivate->m_nShadowMargin; //计算出底部阴影绘制y起点，减掉底部阴影宽度.
            int nBottomHShadowWidth = width() - 2 * m_pPrivate->m_nShadowMargin;

            if(!m_pPrivate->m_bottomPix.isNull()) {
                painter.drawPixmap(m_pPrivate->m_nShadowMargin, nBottomCornerStartY, nBottomHShadowWidth, m_pPrivate->m_nBottomShadowWidth, m_pPrivate->m_bottomPix);
            }
        }

        if(m_pPrivate->m_bRightEnabled) {
            //right side.
            if(m_pPrivate->m_rightPix.isNull()) {
                m_pPrivate->m_rightPix.load(":/Icon/shadow_right.png");
            }

            int nRightStartX = width() - m_pPrivate->m_nRightShadowWidth;
            int nRightVShadowHeight = height() - 2 * m_pPrivate->m_nShadowMargin;
            if(!m_pPrivate->m_rightPix.isNull()) {
                painter.drawPixmap(nRightStartX, nTopRightCornerHeight, m_pPrivate->m_nRightShadowWidth, nRightVShadowHeight, m_pPrivate->m_rightPix);
            }
        }

        if(m_pPrivate->m_bTopEnabled) {
            //right left corner.
            if(m_pPrivate->m_topPix.isNull()) {
                m_pPrivate->m_topPix.load(":/Icon/shadow_top.png");
            }

            int nHTopShadowWidth = width() - 2 * m_pPrivate->m_nShadowMargin;
            if(!m_pPrivate->m_topPix.isNull()) {
                painter.drawPixmap(m_pPrivate->m_nShadowMargin, m_pPrivate->m_nShadowMargin - m_pPrivate->m_nTopShadowWidth, nHTopShadowWidth, m_pPrivate->m_nTopShadowWidth, m_pPrivate->m_topPix);
            }
        }

        painter.end();
    }
}
