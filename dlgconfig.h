#ifndef DLGCONFIG_H
#define DLGCONFIG_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class dlgConfig;
}

class dlgConfig : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    explicit dlgConfig(QWidget *parent = 0);
    ~dlgConfig();
    void addTag(const QString& tagname);
    void initSettings();
public slots:
    void dbgActAddTag(const QString &);
    void refreshConfig();
    void uploadImage();
    void saveData();
private:
    void shiftImage();
    Ui::dlgConfig *ui;
signals:
    void settingChanged(QString category, QString item);

private slots:
    void on_pModifiedPasswordBtn_clicked();
    void on_pSaveDataBtn_clicked();
};

#endif // DLGCONFIG_H
