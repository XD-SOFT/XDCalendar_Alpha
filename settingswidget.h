#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class SettingsWidget;
}

class QNetworkAccessManager;
class QNetworkReply;
//class UserDB;

class SettingsWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT
public:
    static SettingsWidget *getInstance();
    static void destroyInstance();

    void load();

protected slots:
    void testReplyArrived(QNetworkReply *reply);


private: ///这些以后都要使用模块组织单例类.
    explicit SettingsWidget();
    SettingsWidget(const SettingsWidget &other);
    SettingsWidget &operator =(const SettingsWidget &ohter);
    ~SettingsWidget();

private slots:
    void on_pOkBtn_clicked();

    void on_pTestNetWorkBtn_clicked();

    void on_pChangeSavePathBtn_clicked();

private:
    Ui::SettingsWidget *ui;
    static SettingsWidget *m_pInstance;
    QNetworkAccessManager *m_pTestNetWorkMgr;
//    UserDB* m_pUserdb; //暂时这么用，DB相关的重构要全放进单例类管理.
    bool m_bServerTestPassed; //新的服务器是否测通过.
};

#endif // SETTINGSWIDGET_H
