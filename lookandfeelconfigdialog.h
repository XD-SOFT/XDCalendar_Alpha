#ifndef LOOKANDFEELCONFIGDIALOG_H
#define LOOKANDFEELCONFIGDIALOG_H
#include <QWidget>
#include <QDialog>
#include <QVector>
#include <QImage>
#include "framelessmodalmovableshadowwidget.h"
class QSlider;
class QFileInfo;
class QDir;
class QVBoxLayout;
class QScrollArea;
class QButtonGroup;

class LookAndFeelConfigDialog : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    static constexpr int candidatesColumn = 5;
    explicit LookAndFeelConfigDialog(double bo, double fo, QWidget *parent = 0);
    using ImageSet = QVector<QImage>;
     ///< Replace loadBackgroundImageCandidates (const QString &sIconPathName) by BiXiaoxia 2016.12.20.
//    void loadBackgroundImageCandidates (const QImage&);
    /// < \brief 加载背景图片.
    /// < \param sIconPathName, 背景图片路径名.
    void loadBackgroundImageCandidates (const QString &sIconPathName);

    void loadBackgroundImageCandidates (const QFileInfo&);
    void loadBackgroundImageCandidates (const QDir&);
    void pickOne ();

signals:
    void backgroundOpacityChanged (double opacity);
    void foregroundOpacityChanged (double opacity);

    /// < Replace by BiXiaoxia 2016.12.20.
//    void backgroundImageChanged (QImage image);
    void backgroundImageChanged(const QString &sIconPathName);

    void restoreSkin();


private slots:
    /// < \brief 代表皮肤图片的按钮选中与否信号.
    /// < \param bChecked, if true, 该图片被选中, false, 取消该图片选择.
    void skinButtonChecked(bool bChecked);

    ///< \brief 恢复默认.
    void restoreDefault();

protected:
    void mousePressEvent (QMouseEvent* ev);
    void mouseMoveEvent (QMouseEvent* ev);
    void mouseReleaseEvent (QMouseEvent* ev);
private:
    QSlider* mBackgroundOpacitySlider = nullptr;
    QSlider* mForegroundOpacitySlider = nullptr;
    ImageSet mBackgroundImageCandidates;
    QLayout* mImageCandidatesLayout = nullptr;
    QButtonGroup* mButtonGroup = nullptr;
    QPoint mPreWindowPos, mPreCursorPos;
    bool mPressed = false;
};

#endif // LOOKANDFEELCONFIGDIALOG_H
