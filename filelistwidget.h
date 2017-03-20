#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H
#include <QWidget>
#include <QMultiHash>
#include <QMap>
//#include <qtextedit.h>
//#include <QPushButton>
//#include <QLabel>
//#include "downloadfile.h"
//#include <QVBoxLayout>
#include <QJsonObject>

class SearchBox;
class QTreeWidget;
class QTreeWidgetItem;
class QTextEdit;
class QLabel;
class QProgressBar;
class Folder;
class File;
class Lesson;
class downloadFile;
class MyFileIconProvider;
class UploadFiles2Ftp;

class FileListWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileListWidget(QWidget *parent = 0);
    ~FileListWidget ();
    void updatecontentTextEdit(const QMap<QString, QString> &lessonMap);
    void updateContent(Folder* root);
    void setFileMap(const QMap<QString, QString> &fileMap){ this->fileMap = fileMap; }

    void setLesson(Lesson* les)
    {
        this->mLinkedLesson = les;
    }

    Lesson* getLesson()
    {
        return mLinkedLesson;
    }

    void handleUporDownloadFile(Lesson *pLesson);

    void clearResourceDisplay();

    void showUpdateProgress(bool bVisible);

protected:
    void setup ();
signals:


public slots:
    void search (const QString& keyword);
    void update ();
    void link (/*const*/ Folder* folder = nullptr);

    void unlink () {link (nullptr);}

    void openFile(QTreeWidgetItem* ,int);
    void spreadList(QTreeWidgetItem* ,int); //单击文件夹展开其列表
    //void a( QTreeWidgetItem * current, QTreeWidgetItem * previous );
    void GF_downloadFile(int nRow);
    void deleteFile(int nRow);
    void nwState(const QJsonObject &);

    void addFileComplete(File *file);

protected:
    void getItemFromFolder(const Folder* folder);
    void paintEvent (QPaintEvent*);

    bool eventFilter(QObject *watched, QEvent *event);

private slots:
    void downLoadComplete(const QString &sError = QString());

    void uploadFile(int nRow);

    void uploadFinished();

private:
    SearchBox* searchBox = nullptr;
    QTreeWidget* fileTreeWidget = nullptr;
    QTextEdit *contentTextEdit = nullptr;
    //QLabel *contentTextEdit = nullptr;

    //const Folder* mLinkedFolder = nullptr;
    Folder* mLinkedFolder = Q_NULLPTR;
    Lesson* mLinkedLesson = nullptr;
    QMultiHash<QString, QTreeWidgetItem*> name2Item;

    QMap<QString, QString> fileMap;
    QMap<QString, QString> lessonMap;
//    QMap<QTreeWidgetItem*, QString> nodeMap;

    QString lastSearchTarget;
    //QVBoxLayout* mainLayout;


    downloadFile* dtTool = Q_NULLPTR;

    UploadFiles2Ftp *upLoad = Q_NULLPTR;
//    MyPushButton *upload_button;
//    MyPushButton *delete_button;
//    ///Mark由于disconnet总崩溃，暂时这么处理，不牵涉disconnect,重构考虑改变folder与fileListWidget的方式.
//    QList<Folder*> m_allRootFolderList;

    QString m_curResPath; //当前资源路径.

    QLabel *m_pResIconLb;

    QLabel *m_pResTextLb;

    QLabel *m_pUpCountLb;

    QLabel *m_pDownCountLb;

    MyFileIconProvider *m_provider = Q_NULLPTR;

    QProgressBar *m_pProgressBar = Q_NULLPTR;

    ///Mark，由于所有的file改变都会发出changed信号，但是有的不需要重重置,所以做这个限制.
    bool m_bUsedReset = true;
};

#endif // FILELISTWIDGET_H
