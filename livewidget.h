#ifndef LIVEWIDGET_H
#define LIVEWIDGET_H

#include <QWidget>
#include <QPushButton>

class LiveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LiveWidget(QWidget* parent = 0);

private:
    QPushButton *mBackButton;

signals:

public slots:
};

#endif // LIVEWIDGET_H
