#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QWidget>
#include <QFileInfoList>
#include <memory>
//#include <QListWidget>
class QPushButton;
class QLabel;
class Folder;
class MyFileIconProvider;
class QSignalMapper;
class QMenu;

namespace MiniMode {
class FilelistView : public QWidget {
    Q_OBJECT
public:
    explicit FilelistView(QWidget *parent = 0);
    ~FilelistView();
    void link (Folder* = nullptr);
    void unlink () {link (nullptr);}
public slots:
    void reload ();

signals:

protected slots:
    void openFileMenu ();
    void openFileInList (int id);

protected:
    static const QString defaultFileLabelFiller;
    void setup();
    void paintEvent (QPaintEvent*);


private:
    QFileInfoList mFiles;
    QLabel* listLabel = nullptr;
    QPushButton* listIconButton = nullptr;
    Folder* mLinkedFolder = nullptr;
    MyFileIconProvider *m_pIconProvider = Q_NULLPTR;
    QSignalMapper* m_pSignalMapper = Q_NULLPTR;
    QMenu *m_pFileMenu = Q_NULLPTR;
    QLabel *m_pFileIconLb = Q_NULLPTR;

    void addFilesIntoList (const Folder* folder);
};
}

#endif // FILELISTWIDGET_H
