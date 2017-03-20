#ifndef TITLEBAR_H
#define TITLEBAR_H
#include <QWidget>
class TransparentButton;
class QLabel;

class TitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = Q_NULLPTR);

    TitleBar(const QString& title, QWidget *parent = 0);

    void setTitle(const QString &sTitle);

protected:
    void paintEvent (QPaintEvent*);
signals:
    void tbClose ();
public slots:
private:
    QLabel* mTitle;
    TransparentButton* mClose;
};

#endif // TITLEBAR_H
