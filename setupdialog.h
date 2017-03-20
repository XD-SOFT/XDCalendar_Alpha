#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H
#include <QDialog>
#include <QVector>
#include <QPixmap>
#include <QFileInfo>
#include <tuple>
class QTabWidget;
class QWidget;
class QGridLayout;
class QGroupBox;
class QButtonGroup;
class TitleBar;
class QListWidget;
class QStackedWidget;
class MaskLabel;
class QLabel;
class QFileInfo;
class QPixmap;
class QSignalMapper;
class SetupDialog : public QDialog
{
    Q_OBJECT
public:
    using AvatarTuple = std::tuple<QLabel*, QFileInfo, QPixmap, bool>;
    enum AvatarTupleElement:char {Label = 0, File = 1, Pixmap = 2, Activated = 3};
    explicit SetupDialog(QWidget *parent = 0);
    ~SetupDialog() = default;
protected:
    void setup ();
    void setupUserSettings ();
    void setupGeneralSettings ();
    void setupCalendarSettings ();
protected slots:
    void updateIconGroupBox ();
    void addIcon (const QFileInfo&, const QPixmap& pixmap, bool = false);
    void selectAvatarLabel (int index);
private:
    QStackedWidget* mSettingWidgets = nullptr;
    QListWidget* mTabbar = nullptr;
    QWidget* mUserSettings = nullptr;
    QWidget* mGeneralSettings = nullptr;
    QWidget* mCalendarSettings = nullptr;
    TitleBar* mTitleBar;
    QGridLayout* mAvatarsLayout = nullptr;
    QVector<AvatarTuple> mAvatars;
    QSignalMapper* mAvatarLabelSelectMapper = nullptr;
};

#endif // SETUPDIALOG_H
