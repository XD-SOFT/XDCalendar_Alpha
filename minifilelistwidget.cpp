#include "minifilelistwidget.h"
#include "myfileiconprovider.h"
#include "minimode.h"
#include "minimode.h"
#include "mybutton.h"
#include "folder.h"
#include <QFileInfo>
#include <QListWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDebug>
#include <QMenu>
#include <QLabel>
#include <QSignalMapper>
#include <algorithm>
#include <stdexcept>
#include <QUrl>
#include <QPainter>
#include <QDesktopServices>
using namespace MiniMode;
const QString FilelistView::defaultFileLabelFiller {"Personal Files"};
FilelistView::FilelistView(QWidget *parent) : QWidget(parent) {
//    setup();

    m_pFileIconLb = new QLabel;
    m_pFileIconLb->setFixedWidth(20);

    listLabel = new QLabel (defaultFileLabelFiller, this);

    listIconButton = new TransparentButton(this);
    listIconButton->setIcon (QIcon(":/Icon/Mini/01_47.png"));
    listIconButton->setFixedWidth(20);
    listIconButton->setSizePolicy (QSizePolicy::Fixed, QSizePolicy::Expanding);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(m_pFileIconLb);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(listLabel);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(listIconButton);
    mainLayout->setSpacing (0);
    mainLayout->setMargin (0);

    connect (listIconButton, SIGNAL(clicked (bool)), this, SLOT(openFileMenu ()));

    setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Preferred);
}

FilelistView::~FilelistView()
{
    if(m_pIconProvider != Q_NULLPTR) {
        delete m_pIconProvider;
        m_pIconProvider = Q_NULLPTR;
    }
}

void FilelistView::setup()
{

    listLabel = new QLabel (defaultFileLabelFiller, this);

    listIconButton = new TransparentButton(this);
    listIconButton->setIcon (QIcon(":/Icon/Mini/01_47.png"));
    listIconButton->setSizePolicy (QSizePolicy::Preferred, QSizePolicy::Expanding);
}

void FilelistView::openFileInList (int id)
{
    if (id < 0 || id >= mFiles.size ())
    {
        throw std::out_of_range ("file id out of range");
    }
    try {
        QUrl file = QUrl::fromLocalFile (mFiles[id].absoluteFilePath ());
        QDesktopServices::openUrl (file);

        QFontMetrics fontMetric = fontMetrics();
        QString sElideText = fontMetric.elidedText(mFiles[id].baseName(), Qt::ElideRight, listLabel->width());
        listLabel->setText(sElideText);

        if(m_pIconProvider == Q_NULLPTR) {
            m_pIconProvider = new MyFileIconProvider;
        }

        m_pFileIconLb->setPixmap(m_pIconProvider->icon(mFiles[id]).pixmap(m_pFileIconLb->width(), m_pFileIconLb->height()));
    }
    catch (std::exception& e)
    {
        throw std::invalid_argument (std::string("open file failed:") + e.what ());
    }
}

void FilelistView::openFileMenu ()
{
    if(m_pIconProvider == Q_NULLPTR) {
        m_pIconProvider = new MyFileIconProvider;
    }

    if(m_pFileMenu == Q_NULLPTR) {
        m_pFileMenu = new QMenu(this);
    }

    listIconButton->setIcon (QIcon(":/Icon/Mini/01_48.png"));
//    update ();

    ///Replace，重复创建与连接信号.
    ///Mark，这个因为使用它的Minimainscreen每次都销毁重新创建，所以暂时这么处理，以后跟Minimainscreen都得修改使用方式.
    if(m_pSignalMapper == Q_NULLPTR) {
         m_pSignalMapper  = new QSignalMapper(this);

         for (int i = 0; i < mFiles.size (); ++i)
         {
             QAction* ac = new QAction (mFiles[i].fileName (), this);
             ac->setIcon (m_pIconProvider->icon (mFiles[i]));
             connect (ac, SIGNAL(triggered ()), m_pSignalMapper, SLOT(map()));
             m_pSignalMapper->setMapping (ac, i);
             m_pFileMenu->addAction (ac);
         }

         connect (m_pSignalMapper, SIGNAL(mapped (int)), this, SLOT(openFileInList (int)));
    }

    m_pFileMenu->exec (listLabel->mapToGlobal (listLabel->geometry ().bottomLeft ()));
    listIconButton->setIcon (QIcon(":/Icon/Mini/01_47.png"));
//    update ();
}

void FilelistView::link (Folder* folder)
{
    if (mLinkedFolder != nullptr)
        disconnect (mLinkedFolder, SIGNAL (changed ()), this, SLOT (reload ()));
    mLinkedFolder = folder;
    if (mLinkedFolder != nullptr)
        connect (mLinkedFolder, SIGNAL (changed ()), this, SLOT (reload ()));
    reload ();
}

void FilelistView::reload ()
{
    if (mLinkedFolder == nullptr)
        return;
    addFilesIntoList (mLinkedFolder);

    if (mFiles.size () > 0) {
        listLabel->setText (mFiles.value(0).fileName ());

        if(m_pIconProvider == Q_NULLPTR) {
            m_pIconProvider = new MyFileIconProvider;
        }

        m_pFileIconLb->setPixmap(m_pIconProvider->icon(mFiles.value(0)).pixmap(m_pFileIconLb->width(), m_pFileIconLb->height()));
    }
    else
        listLabel->setText (defaultFileLabelFiller);
}

void FilelistView::addFilesIntoList (const Folder* folder)
{
    for (const auto& f: folder->files ())
    {
        mFiles.append (f->fileInfo ());
    }
    for (const auto& f: folder->subfolders ())
    {
        addFilesIntoList (f);
    }
}

void FilelistView::paintEvent (QPaintEvent*)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
