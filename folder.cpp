#include "folder.h"
#include <QDesktopServices>
#include <stdexcept>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <algorithm>
#include <QDebug>
void File::setFileID(int nFileID)
{
    m_nFileID = nFileID;
}

int File::getFileID() const
{
    return m_nFileID;
}

File::File (QObject* parent): QObject (parent)
{}

File::File (const QString& relativePath, QObject* parent):QObject (parent)
{
    setFile (relativePath);
}

File::File (const QJsonObject& json, QObject* parent): QObject (parent)
{
    load (json);
}

File::~File () = default;

void File::open ()
{
    QDesktopServices::openUrl (QUrl (mFile.absoluteFilePath ()));
}

void File::setFile(const QString &relativePath)
{
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);

    mFile = QFileInfo (/*Arg::configDir*/dir, relativePath);
    emit fileChanged (mFile);
}
void File::load (const QJsonObject& json)
{
    clear ();
    QJsonValue url = json["file"];
    if (url.type () != QJsonValue::String)
        throw std::invalid_argument ("Unable to load a File from this QJsonObject : file's type is not String");

    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);
    mFile = QFileInfo (/*Arg::configDir*/dir, url.toString ());
    notify ();
}

QJsonObject File::save () const
{
    Arg *pArg = Arg::getInstance();
    QDir dir;
    pArg->getSaveDir(dir);

    return QJsonObject {{QStringLiteral ("file"), /*Arg::configDir*/dir.relativeFilePath (mFile.absoluteFilePath ())}};
}

void File::clear ()
{
    mFile = QFileInfo ();
}

//=============================================================
Folder::Folder (const QString& name, QObject* parent): QObject (parent), mName (name)
{
    //notify ();
}

Folder::Folder (const QJsonObject& json, QObject* parent): QObject (parent)
{
    load (json);
}

Folder::~Folder () = default;

std::size_t Folder::size (bool recurision) const
{
    std::size_t rslt = static_cast<std::size_t>(mFiles.size ());
    if (recurision)
    {
        for (auto f: mFolders)
            rslt += f->size (recurision);
    }
    return rslt;
}

void Folder::clear ()
{
    mName.clear ();
    mFolders.clear ();
    mFiles.clear ();
    mUsedNames.clear ();
}

void Folder::add (Folder* folder)
{
    checkName (folder->name ());
    if (mFolders.contains (folder))
        throw std::invalid_argument ("Attempt to add repeated Folder into a Folder");
    mUsedNames.insert (folder->name ());
    mFolders.insert (folder);
    notify ();
}

void Folder::add (File* file)
{
    QString name {file->name ()};
    checkName (name);
    if (mFiles.contains (file))
    {
        qDebug()<<"contain: "<<file->name()<<endl;
        throw std::invalid_argument ("Attempt to add repeated File into a Folder");
    }
    mUsedNames.insert (name);

    qDebug()<<"add file info: "<<file->name()<<endl;
    mFiles.insert (file);

    emit addFileComplete(file);
//    notify ();
}

void Folder::remove (Folder* folder)
{
    if (mFolders.contains (folder))
    {
        mFolders.remove (folder);

        notify ();
    }
//    notify ();
}

void Folder::remove (File* file)
{
    qDebug()<<"remove file info: "<<file->name()<<endl;
    if (mFiles.contains (file))
    {
//        qDebug()<<" remove contains"<<endl;
        mFiles.remove (file);
        mUsedNames.remove(file->name());

        emit fileRemovedComplete(file);

        delete file;
        file = Q_NULLPTR;

//        notify ();
    }

//    notify ();
}

void Folder::remove (const QString& name)
{
    if (repeated (name))
    {
        auto equalPredicate = [&] (auto&& p) -> bool
        {
            return p->name () == name;
        };
        auto folderIt = std::find_if (std::begin (mFolders), std::end (mFolders), equalPredicate);
        if (folderIt != std::end (mFolders))
        {
            mFolders.erase (folderIt);
            return;
        }
        auto fileIt = std::find_if (std::begin (mFiles), std::end (mFiles), equalPredicate);
        if (fileIt != std::end (mFiles))
        {
            mFiles.erase (fileIt);
            return;
        }
        throw std::logic_error ((QStringLiteral ("Cannot find a Folder or File named ") + name + QStringLiteral (" while it's found in mUsedNames")).toStdString ());
    }
    notify ();
}


static QString loadFolderFromJsonErrMsgHdr = "Unable to laod a Folder from this QJsonObject : ";
void Folder::load (const QJsonObject& json)
{
    QJsonValue name {json["name"]};
    if (name.type () != QJsonValue::String)
        throw std::invalid_argument (QString(loadFolderFromJsonErrMsgHdr + "name is not a String.").toStdString ());
    QJsonValue folderV {json["folder"]};
    if (folderV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString (loadFolderFromJsonErrMsgHdr + "folder is not a array").toStdString ());
    QJsonArray folder = folderV.toArray ();
    if (std::end (folder) != std::find_if (std::begin (folder), std::end (folder), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
        throw std::invalid_argument(QString(loadFolderFromJsonErrMsgHdr + "folder is not all Objects").toStdString ());
    QJsonValue fileV {json["file"]};
    if (fileV.type () != QJsonValue::Array)
        throw std::invalid_argument (QString (loadFolderFromJsonErrMsgHdr + "file is not a array").toStdString ());
    QJsonArray file = fileV.toArray ();
    if (std::end (file) != std::find_if (std::begin (file), std::end (file), [] (const QJsonValue& v) {return v.type () != QJsonValue::Object;}))
        throw std::invalid_argument(QString(loadFolderFromJsonErrMsgHdr + "file is not all Objects").toStdString ());
    clear ();
    mName = name.toString ();
    for (auto f: folder)
    {
        mFolders.insert (new Folder (f.toObject (), this));
    }
    for (auto f: file)
    {
        mFiles.insert (new File (f.toObject (), this));
    }

    generateUsedNameSet ();
    notify ();
}


QJsonObject Folder::save () const
{
    QJsonArray folders, files;
    for (const auto& f: mFolders)
        folders.push_back (f->save ());
    for (const auto& f: mFiles)
        files.push_back (f->save ());
    return QJsonObject {{"name", mName}, {"folder", folders}, {"file", files}};
}
