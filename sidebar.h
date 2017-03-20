#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QStackedWidget>
#include <QTabBar>
class Sidebar : public QWidget
{
    Q_OBJECT
public:
    Sidebar(QWidget* parent = nullptr);
//    void setOpacity (double o) {mOpacity = o;}
    void setBackground (const QImage& image);
    void addTab (QWidget*, const QString&);
protected:
    void paintEvent (QPaintEvent*);
    void resizeEvent (QResizeEvent*);

private:
    void initialize();

private:
//    double mOpacity = 1.0;
    QImage background;
    QTabBar* mTabBar;
    QStackedWidget* mPages;
};

#endif // SIDEBAR_H
