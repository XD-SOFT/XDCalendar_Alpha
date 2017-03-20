/**
 * XDCalendar Project 2016-12-24 version
  * treewidget显示代理类.
  *
  * @author BiXiaoxia
  * @version 2016.12.24.
*/
#ifndef XDKJSOFT_TREEDISPLAYDELEGATE_H
#define XDKJSOFT_TREEDISPLAYDELEGATE_H

#include <QItemDelegate>

class TreeDisplayDelegate : public QItemDelegate
{
public:
    TreeDisplayDelegate(QObject *parent = Q_NULLPTR);

    /*!
    * \brief 设置鼠标悬停时候背景颜色.
    * \param color.
    */
    void setHoverdBackgroundColor(const QColor &color);

    /*!
    * \brief 设置Item选中时候背景颜色.
    * \param color.
    */
    void setSelectedBackgroundColor(const QColor &color);

    /*!
    * \brief 设置普通状态背景颜色.
    * \param color.
    */
    void setNormalBackgroundColor(const QColor &color);

    /*!
    * \brief paint 项显示绘制函数.
    * \param painter, QPainter绘图设备对象指针.
    * \param option, item绘制样式选项.
    * \param index, 父对象.
    */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void drawBranches(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void drawBackground(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void drawIcon(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void drawText(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    /// < \variable 常态下背景色.
    QColor m_normalBgColor;

    /// < \variable item选中状态背景颜色.
    QColor m_selectedBgColor;

    /// < \variable item 鼠标悬停时背景色.
    QColor m_hoverBgColor;

    bool m_bDrawBranches;
};

#endif // XDKJSOFT_TREEDISPLAYDELEGATE_H
