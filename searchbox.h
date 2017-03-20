#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QLabel>
class SearchBox : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent = 0);
    ~SearchBox ();
signals:
    void search (const QString&);
public slots:
    void setIcon (const QPixmap& icon);
protected:
    void paintEvent (QPaintEvent* ev);
private:
    QPixmap icon;
    QLabel* iconLabel;
    QLineEdit* textEdit;
    qint16 mShadowMargin = 10;
};

#endif // SEARCHBOX_H
