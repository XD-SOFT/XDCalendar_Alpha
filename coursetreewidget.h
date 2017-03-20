#ifndef COURSETREEWIDGET_H
#define COURSETREEWIDGET_H

#include <QWidget>

namespace Ui {
class CourseTreeWidget;
}

class CourseTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CourseTreeWidget(int nRelatedRow , QString fileName , QWidget *parent = 0);
    ~CourseTreeWidget();

    void setRelatedRow(int nRow);
signals:
    void downloadOK(int nRow);

    void deleteOK(int nRow);

    void uploadFile(int nRow);

private slots:
    void on_downloadButton_clicked();

    void on_deleteButton_clicked();

    void on_pUploadBtn_clicked();

protected:
     bool eventFilter(QObject *,QEvent *);

     void resizeEvent(QResizeEvent *pEv);

private:
    Ui::CourseTreeWidget *ui;
    int m_nRelatedRow;
    QString m_sFileName;
};

#endif // COURSETREEWIDGET_H
