#include "infoandsettingsstackwidget.h"
#include "sidebar.h"
#include "editrightwidget.h"
#include <QPainter>
#include <QStyleOption>
#include <QLayout>

class InfoAndSettingsStackWidgetPrivate {
    friend class InfoAndSettingsStackWidget;
    QWidget *pStandardModeInfoWgt;
    QWidget *pEditModeSettingsWgt;
};

InfoAndSettingsStackWidget::InfoAndSettingsStackWidget(QWidget *parent) :
    QStackedWidget(parent),
    m_pPrivate(new InfoAndSettingsStackWidgetPrivate)
{
    setAttribute(Qt::WA_TranslucentBackground, false);

    m_pPrivate->pStandardModeInfoWgt = new Sidebar(this);
    addWidget(m_pPrivate->pStandardModeInfoWgt);

    m_pPrivate->pEditModeSettingsWgt = new EditRightWidget(this);
    addWidget(m_pPrivate->pEditModeSettingsWgt);

    QLayout *pLayout = layout();
    if(pLayout != Q_NULLPTR) {
        pLayout->setContentsMargins(0, 0, 0, 0);
    }

    setStyleSheet(" border: 0px; \
                    margin: 0px; \
                    padding:  0px;");
}

InfoAndSettingsStackWidget::~InfoAndSettingsStackWidget()
{
    delete m_pPrivate;
    m_pPrivate = Q_NULLPTR;
}

void InfoAndSettingsStackWidget::switchInfoAndSettingsWidget(InfoAndSettingsStackWidget::InfoSettingsType type)
{
    switch(type) {
    case StandardMode:
        setCurrentWidget(m_pPrivate->pStandardModeInfoWgt);
        break;

    case EditMode:
        setCurrentWidget(m_pPrivate->pEditModeSettingsWgt);
        break;

    default:
        setCurrentWidget(m_pPrivate->pStandardModeInfoWgt);
        break;
    }
}

void InfoAndSettingsStackWidget::paintEvent(QPaintEvent *pEvent)
{
    QPainter p (this);
    QStyleOption opt;
    opt.init(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    p.fillRect(rect(), QColor(255, 255, 255, 255));
}
