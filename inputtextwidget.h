/****
*InputTextWidget, 具有MoonCalendar风格的文本输入部件.
* used:
* 1.InputTextWidget *pInputTextWgt = new InputTextWidget;
* pInputTextWgt->show();
* pInputTextWgt->getText();
* 这个类使用必须自己管理释放.因为阴影不能设置父窗口对象.
*
* 2. QString sText = InputTextWidget::getText(....);
* @version 2017.02.09.
* @author BiXiaoxia.
*/
#ifndef XDKJ_INPUTTEXTWIDGET_H
#define XDKJ_INPUTTEXTWIDGET_H

#include "framelessmodalmovableshadowwidget.h"

namespace Ui {
class InputTextWidget;
}

class QEventLoop;

class InputTextWidget : public FramelessModalMovableShadowWidget
{
    Q_OBJECT

public:
    ~InputTextWidget();

    /*!
     * \brief getText 获取用户输入的文本.
     * \param sTitle  文本输入部件题目.
     * \param sDefaultText  默认的文本.
     * \param parent   文本输入部件父窗口.
     * \return 输入的文本,如果返回空，无效输入或取消.
     * \example QString sText = InputTextWidget::getText("Please input file name", "Default", this);
     */
    static QString getText(const QString &sTitle, const QString &sDefaultText, QWidget *parent = Q_NULLPTR);

    QString getText() const;

private slots:
    void on_pOKBtn_clicked();

    void on_pCancelBtn_clicked();

private:
    explicit InputTextWidget(QWidget *parent = Q_NULLPTR);
    InputTextWidget(const QString &sTitle, const QString &sDefaultText, QWidget *parent = Q_NULLPTR);

private:
    Ui::InputTextWidget *ui;

    static QEventLoop *m_pEvLoop;
};

#endif // XDKJ_INPUTTEXTWIDGET_H
