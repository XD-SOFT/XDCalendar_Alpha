#ifndef MINIMODEFLOATINGWIDGET_H
#define MINIMODEFLOATINGWIDGET_H

#include "baseshadowwidget.h"

class QPushButton;
class QComboBox;
class Lesson;
class TransparentButton;
class QMenu;
class QGraphicsOpacityEffect;

namespace MiniMode {
class InfoView;
class MiniDateWidget;
class FilelistView;

class MainScreen : public BaseShadowWidget
{
    Q_OBJECT
public:
    template <typename... T>
    static MainScreen* makeInstance (T&&... args)
    {
        clearInstance ();
        return instance = new MainScreen (std::forward<T>(args)...);
    }

    static MainScreen* getInstance () {return instance;}
    static MainScreen* clearInstance ()
    {
        if (instance != nullptr)
            delete instance;
        return instance = nullptr;
    }

    void link (Lesson* lesson, int nLeftSeconds);

    void unlink () {link (Q_NULLPTR, 0);}

    /*!
      @ \brief 设置背景透明度.
      @ \param fOpacity,透明度值.
    */
    void setBackgroundOpacity(qreal fOpacity);

    void setForegroundOpacity(qreal fOpacity);

public slots:
    void reload ();
    void showMainMenu ();
protected:
    void setupSubWidgets ();
    void enterEvent (QEvent*);
    void leaveEvent (QEvent*);
//    void paintEvent (QPaintEvent*);
    void mousePressEvent (QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent*);
    void mouseMoveEvent (QMouseEvent*);

    void drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev);

    bool eventFilter(QObject *watched, QEvent *event);

private:
    static MainScreen* instance;
    MainScreen(QWidget *parent = 0);
    ~MainScreen();

    QWidget* topLine = Q_NULLPTR;

    InfoView* mUserInfoWidget = nullptr;

    MiniDateWidget* mDateWidget = nullptr;

    QPushButton* mRecordBtn = nullptr;
    QPushButton* mDrawingPadBtn = nullptr;
    FilelistView* mFileListWidget = nullptr;
    Lesson* mLinkedLesson = nullptr;

    TransparentButton *moreBtn, *ExitBtn;
    QPoint mOldWindowPosition, mOldCursorPosition;
    QMenu *m_pModeCtlMenu;

    ///< \背景图片.
    QPixmap mBackgroundImage;

    ///< \brief 背景透明度.
    qreal m_fBackgroundOpacity = 1.0f;

    ///< \brief 前景透明度.
    qreal m_fPregroundOpacity = 1.0f;

    QGraphicsOpacityEffect *m_pOpacityEffect = Q_NULLPTR;

    bool mPressed = false;
};
}

#endif // MINIMODEFLOATINGWIDGET_H
