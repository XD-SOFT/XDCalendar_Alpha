#ifndef FOLDER_H
#define FOLDER_H
#include "config.h"
#include <QObject>
#include <QList>
#include <QSet>
#include <QUrl>
#include <QFileInfo>
/*
 * A folder contains some sub-folders and some files
 */
class QJsonObject;
class File: public QObject
{
    Q_OBJECT
public:
    void setFileID(int nFileID);

    int getFileID() const;

public:
    explicit File (QObject *parent = 0);
    //Construct a File via a QJsonObject. Coresponding signals will be emitted.
    explicit File (const QJsonObject& json, QObject* parent = nullptr);
    //Construct a File via a path relative to Arg::configDir. Coresponding signals will be emitted.
    File (const QString& relativePath, QObject *parent = nullptr);
    File (const File&) = delete;
    File& operator= (const File&) = delete;
    ~File ();

    // Open this file via local operate system
    void open ();
    //Get the real file represented by File
    QFileInfo fileInfo () const {return mFile;}
    //Set the real file via a path relative to Arg::configDir. Coresponding signals will be emitted.
    void setFile (const QString& relativePath);
    //Get the real file's name
    QString name () const {return mFile.fileName ();}
    //Load a File from QJsonObject
    void load (const QJsonObject& json);
    //Save the File to a QJsonObject
    QJsonObject save () const;
    //Clear file info. No signal is Emitted.
    void clear ();
    //Emit all signals
    void notify ()
    {
        emit fileChanged (mFile);
    }

signals:
    void fileChanged (QFileInfo file);

private:
    QFileInfo mFile;

    //加入数据库返回的文件ID.
    int m_nFileID;
};

class Folder : public QObject
{
    Q_OBJECT
public:
    //Construct the Folder with a name. Coresponding signals will be emitted.
    explicit Folder(const QString& name, QObject *parent = 0);
    //Construct the Folder with a QJsonObject. Coresponding signals will be emitted.
    explicit Folder (const QJsonObject& json, QObject* parent = nullptr);
    Folder (const Folder& folder) = delete;
    Folder& operator = (const Folder&) = delete;
    ~Folder ();
    //Get the number of files, considering all files only if recurision is true.
    std::size_t size (bool recurision = false) const;
    //Get the Folder's name
    QString name () const {return mName;}
    //Clear the Folder. No signal is emitted
    void clear ();
    //Add Folder or Files. You should set it's parent to be this manually.. Coresponding signals will be emitted.
    void add (Folder *folder);
    void add (File *file);
    void remove (Folder *folder);

    ///@\brief 移除file，不需要再单独删除file.
    void remove (File *file);
    void remove (const QString& name);
    //Whether the name is used
    bool repeated (const QString& name) const{
        return mUsedNames.contains (name);
    }
    QSet<Folder*> subfolders () const {return mFolders;}
    QSet<File*> files () const {return mFiles;}

    //Load a QJsonObject.. Coresponding signals will be emitted.
    void load (const QJsonObject& json);
    //Save to be a QJsonObject
    QJsonObject save () const;
    //Emit all signals.
    void notify ()
    {
        qDebug() << "emit folder changed signal.";
        emit changed ();
    }

    bool checkName (const QString& name)
    {
        return mUsedNames.contains (name);

           // throw std::invalid_argument ("Repeated name");
    }

protected:
    void generateUsedNameSet ()
    {
        mUsedNames.clear ();
        for (const auto& f: mFolders)
        {
            if (mUsedNames.contains (f->name ()))
                throw std::invalid_argument ("Repeated Folder name");
            else
                mUsedNames.insert (f->name ());
        }
        for (const auto& f: mFiles)
        {
            if (mUsedNames.contains (f->name ()))
                throw std::invalid_argument ("Repeated File name");
            else
                mUsedNames.insert (f->name ());
        }
    }

signals:
    void changed ();

    void addFileComplete(File *pFile);

    void fileRemovedComplete(File *pFile);

public slots:
private:
    QString mName;
    QSet<Folder*> mFolders;
    QSet<File*> mFiles;
    QSet<QString> mUsedNames;
};

#endif // FOLDER_H
