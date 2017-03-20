/**
 * XDCalendar Project 2016-12-21 version
  * 带有阴影边框的widget类基类，该类去掉了系统原来边框，自己重新绘制边框，禁止了paintEvent函数在派生类中重写，如果需要绘制阴影外部分，则重写.
  * void drawExcludeShadowPart(QPainter *&painter, QPaintEvent* ev)，重写该函数进行绘制的时候，注意阴影尺寸，阴影目前只有左侧.右侧.
  * 底部有，并且宽度一样，可以通过getShadowWidth()获取.也可以禁止阴影，默认除阴影外的部分是白色.如果用到ui文件，那么ui设计上主布局的margin要大于等于阴影宽度.
  *
  *
  * @author BiXiaoxia
  * @version 2016.12.21.
*/
#ifndef XDKJSOFT_BASESHADOWWIDGET_H
#define XDKJSOFT_BASESHADOWWIDGET_H

#include <QWidget>

class BaseShadowWidgetPrivate;

class BaseShadowWidget : public QWidget
{
    Q_OBJECT
public:
    Q_ENUMS(ShadowSide)
    enum ShadowSide{Left, Top, Right, Bottom};

    explicit BaseShadowWidget(QWidget *parent = 0);
    ~BaseShadowWidget();

    /*!
      @ \brief 设置阴影的宽度.
      @ \param nShadowWidth, 阴影大小值.
    */
    void setShadowWidth(int nShadowWidth);

    /*!
      @ \brief 获取阴影宽度.
      @ \return 阴影大小值.
    */
    int getShadowMargin() const;

    void setShadowWidthBySide(ShadowSide side, int nShadowWidth);

    int getShadowWidthBySide(ShadowSide)const;

    /*!
      @ \brief 设置阴影启用与否,设置了这个则全部启用或禁止.
      @ \param bEnabled, true 启用，false，禁用.
    */
    void setShadowEnabled(bool bEnabled);

    /*!
      @ \brief 获取阴影是否启用，这个代表四个边全部启用或者禁止.
      @ \return true，启用，false,禁用.
    */
    bool isShaodwEnabled() const;

    /*!
        @ \brief 单独启用某个边的阴影.

    */
    void setShadowEnabled(ShadowSide side, bool bEnabled);

    bool isShaodwEnabled(ShadowSide side) const;

//    ///Mark,由于分边启用阴影实现逻辑比较复杂，暂时没时间做这个，下面四个接口暂时都是无用的,以后实现.
//    void setTopShadowEnabled(bool bEnabled = false);

//    void setBottomShadowEnabled(bool bEnabled = true);

//    void setLeftShadowEnabled(bool bEnabled = true);

//    void setRightShadowEnabled(bool bEnabled = true);

    void setShadowPixmapBySide(ShadowSide side, const QString &sPixPathName);

    void setOpacityDisabled(bool bDisabled);

protected:
    /*!
      @ \brief 绘制除阴影外的其它部分，这个类的派生类统一在这里实现其它绘制,paintEvent已经被限制为不能再重写的，并且必须要绘制，否则背景会是透明的,绘制的时候注意阴影已占有尺寸.
      @ \param .
      @ \param .
    */
    virtual void drawExcludeShadowPart(QPainter *painter, QPaintEvent* ev);

    void paintEvent(QPaintEvent *event) Q_DECL_FINAL;

private:
    BaseShadowWidgetPrivate *m_pPrivate;
};

#endif // XDKJSOFT_BASESHADOWWIDGET_H
