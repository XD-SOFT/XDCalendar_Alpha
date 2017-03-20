#ifndef CLASSINFOWIDGET_H
#define CLASSINFOWIDGET_H

#include <QWidget>

namespace Ui {
class ClassInfoWidget;
}

class ClassInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClassInfoWidget(QWidget *parent = 0);
    ~ClassInfoWidget();

private:
    Ui::ClassInfoWidget *ui;
};

#endif // CLASSINFOWIDGET_H
