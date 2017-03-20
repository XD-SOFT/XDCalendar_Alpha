#include "treedisplaydelegate.h"

#include <QPixmap>
#include <QPainter>
#include <QFontMetrics>
#include <QApplication>
#include <QModelIndex>
#include <QStyleOptionViewItem>
#include <QDebug>

TreeDisplayDelegate::TreeDisplayDelegate(QObject *parent):
    QItemDelegate(parent),
    m_normalBgColor(QColor("#f9f9f9")),
    m_selectedBgColor(QColor("#f1f1f1")),
    m_hoverBgColor(QColor("#f1f1f1")),
    m_bDrawBranches(false)
{

}

void TreeDisplayDelegate::setHoverdBackgroundColor(const QColor &color)
{
    m_hoverBgColor = color;
}

void TreeDisplayDelegate::setSelectedBackgroundColor(const QColor &color)
{
    m_selectedBgColor = color;
}

void TreeDisplayDelegate::setNormalBackgroundColor(const QColor &color)
{
    m_normalBgColor = color;
}

void TreeDisplayDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(!index.isValid())
    {
        painter->fillRect(option.rect, Qt::red);
        qDebug() << "ResourceItemDelegate::paint invalid index";
        return;
    }

    painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform);

    drawBranches(painter, option, index);

    //background.
    drawBackground(painter, option, index);

    drawIcon(painter, option, index);

    drawText(painter, option, index);

//    QRect checkRect;
//    QRect decorationRect;
//    QRect displayRect;
//    doLayout(option, &checkRect, &decorationRect, &displayRect, false);

//    // draw the item
//    Qt::CheckState checkState = Qt::Unchecked;
//    QVariant value = index.data(Qt::CheckStateRole);
//    if (value.isValid()) {
//        checkState = static_cast<Qt::CheckState>(value.toInt());
//        checkRect = doCheck(option, option.rect, value);
//    }

//    drawCheck(painter, option, checkRect, checkState);

//    drawFocus(painter, option, displayRect);
}

void TreeDisplayDelegate::drawBranches(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(m_bDrawBranches) {
        int nStartX = option.rect.topLeft().x();
        int nStartY = option.rect.topLeft().y();
        int nHeight = option.rect.height();
        int nWidth = option.rect.width();

        //如果是分支，则绘制上下两条线.
        if((index.parent() == QModelIndex()) || (option.state & QStyle::State_Children)) {
            painter->save();
            QPen pen(Qt::darkGray);
            pen.setWidthF(0.2);
            painter->setPen(pen);

            painter->drawLine(QPointF(nStartX, nStartY), QPointF(nStartX + nWidth, nStartY));
            painter->drawLine(QPointF(nStartX, nStartY + nHeight - 2), QPointF(nStartX + nWidth, nStartY + nHeight - 2));

            //再次绘制，体现凹凸效果.
            pen.setColor(Qt::white);
            painter->drawLine(QPointF(nStartX, nStartY + 0.2), QPointF(nStartX + nWidth, nStartY + 0.2));
            painter->drawLine(QPointF(nStartX, nStartY + nHeight - 1.8), QPointF(nStartX + nWidth, nStartY + nHeight - 1.8));

            pen.setColor(Qt::darkGray);
            painter->drawLine(QPointF(nStartX, nStartY + 0.4), QPointF(nStartX + nWidth, nStartY + 0.4));
            painter->drawLine(QPointF(nStartX, nStartY + nHeight - 1.6), QPointF(nStartX + nWidth, nStartY + nHeight - 1.6));


            //        //闭合状态绘制竖直两条.
            //        if(option.state & QStyle::State_Open) {
            //            painter->drawLine(nStartX, nStartY, nWidth, nStartY);
            //            painter->drawLine(nStartX, nStartY + nHeight - 2, nWidth, nStartY + nHeight - 2);

            //            //再次绘制，体现凹凸效果.
            //            pen.setColor(Qt::white);
            //            painter->drawLine(nStartX, nStartY, nWidth, nStartY);
            //            painter->drawLine(nStartX, nStartY + nHeight - 2, nWidth, nStartY + nHeight -2);
            //        }
            //        else {
            //            pen.setWidthF(0.2);
            //            painter->setPen(pen);
            //            painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
            //            pen.setColor(Qt::white);
            //            painter->drawRect(option.rect.adjusted(1, 1, -1, -1));
            //        }

            painter->restore();
        }
    }
}

void TreeDisplayDelegate::drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(m_bDrawBranches) {
        //分支部要背景.
        if((index.parent() == QModelIndex()) || (option.state & QStyle::State_Children)) {
            return;
        }
    }

    painter->save();
    QBrush brush(m_normalBgColor);
    bool bExistBackGroundColor = false;
    QVariant backRoleVar = index.model()->data(index, Qt::BackgroundRole);

    if(backRoleVar.canConvert(QMetaType::QColor)) {
        QColor backGroundColor = backRoleVar.value<QColor>();
        brush.setColor(backGroundColor);
        bExistBackGroundColor = true;
    }

    if(!bExistBackGroundColor) {
        if(option.state & QStyle::State_Selected) {
             brush.setColor(m_selectedBgColor);
        }
        else if(option.state & QStyle::State_MouseOver) {
            brush.setColor(m_hoverBgColor);
        }
        else if(option.state & QStyle::State_Active) {

        }
    }

    QRect backgroundRect = option.rect;
    if((index.parent() == QModelIndex()) || (option.state & QStyle::State_Children)) {
        if(option.state & QStyle::State_Open) {
//            backgroundRect = option.rect.adjusted(2, 2, -2, -2);
        }
    }
    else {
        backgroundRect = option.rect.adjusted(0, 0, -5, 0);
    }

    painter->fillRect(backgroundRect, brush);
    painter->restore();
}

void TreeDisplayDelegate::drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int nStartX = option.rect.topLeft().x();
    int nStartY = option.rect.topLeft().y();
    int nHeight = option.rect.height();
    int nWidth = option.rect.width();
    int nMinValue = nWidth > nHeight ? nHeight : nWidth;

    //绘制图片.
    QVariant imgVar = index.model()->data(index, Qt::DecorationRole);

    if(imgVar.isValid()) {
        if(imgVar.canConvert(QMetaType::QIcon)) {
            QIcon pix = imgVar.value<QIcon>();
            if(!pix.isNull())
            {
                QStyleOptionViewItem::Position imgPosition = option.decorationPosition;

                switch(imgPosition) {
                case QStyleOptionViewItem::Left:{

                }
                    break;

                case QStyleOptionViewItem::Right: {

                }
                    break;

                case QStyleOptionViewItem::Top: {

                }
                    break;

                case QStyleOptionViewItem::Bottom: {

                }
                    break;
                }

                int nIconWidth = nMinValue - 16;
                painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
                painter->drawPixmap(nStartX + 8, nStartY + 8, nIconWidth, nIconWidth, pix.pixmap(nIconWidth, nIconWidth));
            }
        }
    }
}

void TreeDisplayDelegate::drawText(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int nHeight = option.rect.height();
    int nWidth = option.rect.width();
    int nMinValue = nWidth > nHeight ? nHeight : nWidth;
    QString sText = index.model()->data(index, Qt::DisplayRole).toString();

    if(!sText.isEmpty()) {
        QFontMetrics fontMetrics = QApplication::fontMetrics();
        quint32 nTextPixelWidth = fontMetrics.width(sText);
        quint32 nPaintWidth = option.rect.width();
        if(nTextPixelWidth >= nPaintWidth)
        {
            quint32 nDisPixelNumber = nPaintWidth - 5;
            quint32 nRmovePixelNumber = nTextPixelWidth - nDisPixelNumber;
            sText.remove(nDisPixelNumber, nRmovePixelNumber);
            sText.append("...");
        }

        QColor textColor("#535353");
        QVariant var = index.model()->data(index, Qt::ForegroundRole);
        if(var.canConvert(QMetaType::QColor)) {
            textColor = var.value<QColor>();
        }

        Qt::Alignment align = Qt::AlignVCenter | Qt::AlignLeft;
        var = index.model()->data(index, Qt::TextAlignmentRole);

        if(var.isValid()) {
            align = static_cast<Qt::Alignment>(var.toUInt());
        }

        painter->save();
        painter->setPen(textColor);
        painter->drawText(QRectF(option.rect.topLeft().x() + nMinValue,
                                 option.rect.topLeft().y(),
                                 option.rect.width() - nMinValue,
                                 option.rect.height()),
                          sText,
                          QTextOption(align));

        painter->restore();
    }
}
