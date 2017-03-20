#ifndef FILEKEYWORD_H
#define FILEKEYWORD_H

#include <QObject>
#include <QJsonObject>
//#include "handler.h"

class FileKeyword : public QObject
{
    Q_OBJECT
public:
    explicit FileKeyword(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setState(int state) { this->state = state; }
    void setFileid( int fileid) { this->fileid = fileid; }
    int getFileid() { return fileid; }
    void setKeyid(int keyid) { this->keyid = keyid; }
    int getKeyid() { return keyid; }
    void setKeyname(QString keyname) { this->keyname = keyname; }
    QString getKeyname() { return keyname; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void add(); //增加一条记录
    void del(); //删除一条记录
    void find(); //搜索一个文件的所有标签

private slots:
    void handleReply(const QJsonObject &jsonObj);

private:
    /*!
     * \brief The RequestCode enum
     */
    enum RequestCode{Add, Delete, Find};
    RequestCode m_curRequestCode;

    int id;
    int fileid;
    int keyid;
    int state;
    QString keyname;

//    QString baseUrl;

signals:
    void addFileKeywordFinished(const QJsonObject &jsonObj); //发射信号更新前端界面
    void deleteFileKeywordFinished(const QJsonObject &jsonObj);
    void findKeywordFinished(const QJsonObject &jsonObj);
//    void addFinish(QJsonObject); //发射信号更新前端界面
//    void delFinish(QJsonObject);
//    void findFinish(QJsonObject);

//public slots:
//    void addState(QJsonObject); //handler获取数据后返回给该槽函数
//    void delState(QJsonObject);
//    void findState(QJsonObject);


};

#endif // FILEKEYWORD_H
