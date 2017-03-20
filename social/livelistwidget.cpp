#include "livelistwidget.h"
#include "livemessage.h"
#include "livecardwidget.h"
#include "searchbox.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QDebug>

LiveListWidget::LiveListWidget(QVector<LiveMessage *> &messages, QWidget *parent) : QWidget(parent)
{
    //a scroll area for friend list
    auto wrapper = new QVBoxLayout(this);
    wrapper->setMargin(0);
    wrapper->setSpacing(0);

    auto area = new QScrollArea();
    area->setWidgetResizable(true);
    wrapper->addWidget(area);

    auto widget = new QWidget();
    area->setWidget(widget);

    auto layout = new QVBoxLayout (widget);
    layout->setSpacing(0);
    layout->setMargin(0);

    //add components
    layout->addWidget(new SearchBox());

    for(auto iter = messages.begin(); iter != messages.end(); iter++){
        //message
        LiveMessage *&msg = (*iter);
        Friend *&&fr = msg->getFriend();
        layout->addWidget(new LiveCardWidget(fr, new QLabel(msg->getMessage()), msg->getReleaseDateTime()));

        //seperator
        if(iter == messages.end()-1)
            break;
        QFrame* line = new QFrame();
        line->setFrameShape(QFrame::HLine);
        layout->addWidget(line);
    }

    layout->addStretch(5);
}
