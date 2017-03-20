#include "loginstatuswidget.h"
#include <QPainter>
#include <QTimerEvent>
#include <QApplication>
#include <QIcon>

class LoginStatusWidgetPrivate {
    friend class LoginStatusWidget;
private:
    QPixmap backImgPix;
    QFont textFont;
    QString sStatusText;
    int nTimerId = -1;
    qreal fOpacity = 1.0f;
    bool bAutoChangeStatusEnabled = false;
    bool m_bTextNeedChanged = false;
    bool bEnableBackImg = false;
    bool bStatusTextEnabled = true;
};

LoginStatusWidget::LoginStatusWidget(QWidget *parent):
    BaseShadowWidget(parent),
    m_pPrivate(new LoginStatusWidgetPrivate)
{
    Qt::WindowFlags winFlags = windowFlags();
    setWindowFlags (winFlags | Qt::Window);
    setWindowModality(Qt::ApplicationModal);
    setWindowTitle(tr("教师桌面"));
//    setWindowIcon(QIcon(":/Icon/tray.png"));

    m_pPrivate->backImgPix.load(":/image/start-up.jpg");

    m_pPrivate->textFont.setFamily("Microsoft YaHei");
    m_pPrivate->textFont.setPointSize(14);

    m_pPrivate->sStatusText.append(tr("加载中......"));
}

LoginStatusWidget::~LoginStatusWidget()
{
    delete m_pPrivate;
    m_pPrivate = Q_NULLPTR;
}

void LoginStatusWidget::setStartImageEnabled(bool bEnabled)
{
    m_pPrivate->bEnableBackImg = bEnabled;
}

void LoginStatusWidget::setStartImage(const QString &sPixPathName)
{
    Q_ASSERT_X(!sPixPathName.isEmpty(), "LoginStatusWidget::setStartImage", "image path name is null.");

    m_pPrivate->backImgPix.load(sPixPathName);
}

void LoginStatusWidget::setOpacity(qreal fOpacity)
{
    m_pPrivate->fOpacity = fOpacity;
    //如果全透明了，则阴影也要禁用.
    setShadowEnabled(false);

    repaint();
}

void LoginStatusWidget::setFont(const QFont &font)
{
    m_pPrivate->textFont = font;
}

void LoginStatusWidget::setStatusTextEnabled(bool bEnabled)
{
    m_pPrivate->bStatusTextEnabled = bEnabled;
}

void LoginStatusWidget::setStatusText(const QString &sText)
{
    m_pPrivate->sStatusText.clear();
    m_pPrivate->sStatusText.append(sText);
}

void LoginStatusWidget::setAutoChangeStatusText(bool bEnabled)
{
    m_pPrivate->bAutoChangeStatusEnabled = bEnabled;
}

void LoginStatusWidget::drawExcludeShadowPart(QPainter *painter, QPaintEvent *ev)
{
    Q_UNUSED(ev);

    if(m_pPrivate->bEnableBackImg) {
        painter->save();
        painter->setOpacity(m_pPrivate->fOpacity);
        painter->drawPixmap(rect(), m_pPrivate->backImgPix);
        painter->restore();
    }

    if(m_pPrivate->bStatusTextEnabled) {
        painter->save();
        painter->setFont(m_pPrivate->textFont);

        if(m_pPrivate->bEnableBackImg) {
            painter->setCompositionMode(QPainter::CompositionMode_DestinationAtop);
        }

        Qt::Alignment align = Qt::AlignVCenter | Qt::AlignHCenter;
        QString sText = m_pPrivate->sStatusText;

        //status text need auto changes.
        if(m_pPrivate->bAutoChangeStatusEnabled) {
            sText.clear();

            if(!m_pPrivate->m_bTextNeedChanged) {
                sText.append(tr("加载中......"));
            }
            else {
                sText.append(tr("加载中......"));
            }
        }

        painter->drawText(rect(), sText, QTextOption(align));
        painter->restore();
    }
}

void LoginStatusWidget::showEvent(QShowEvent *pEv)
{
    setCursor(Qt::WaitCursor);
//    m_pPrivate->nTimerId = startTimer(1000);
}

void LoginStatusWidget::closeEvent(QCloseEvent *pEv)
{
    if(m_pPrivate->nTimerId != -1) {
        killTimer(m_pPrivate->nTimerId);
    }

    unsetCursor();
}

void LoginStatusWidget::timerEvent(QTimerEvent *pEv)
{
    if(m_pPrivate->nTimerId == pEv->timerId()) {
        m_pPrivate->m_bTextNeedChanged = !m_pPrivate->m_bTextNeedChanged;

        repaint();
        qApp->processEvents();
    }
}
