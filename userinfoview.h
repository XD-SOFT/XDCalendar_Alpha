#ifndef USERINFOVIEW_H
#define USERINFOVIEW_H

#include <QWidget>
//#include <memory>
//#include "masklabel.h"
/*
 * view the user's basic information at the top left of the main screen
 */
class QPixmap;
class QLabel;
class QProgressBar;
class Teacher;
class MaskLabel;

class UserInfoView : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoView(QWidget *parent = 0);
    ~UserInfoView ();

signals:

public slots:
    void reload ();
    void link (const Teacher* user = nullptr);
    void unlink () {link (nullptr);}

protected:
    void setup ();
    void paintEvent (QPaintEvent*);

    void resizeEvent(QResizeEvent *event);

private:
    QProgressBar* mExpBar;
    MaskLabel* mIconLabel;
    //QLabel* mIconLabel;
    QLabel* mNameLabel;
    const Teacher* mLinkedUser = nullptr;
};

#endif // USERINFOVIEW_H
