#ifndef KEYWORDDB_H
#define KEYWORDDB_H

#include <QObject>
#include <QJsonObject>
#include "handler.h"

class KeywordDB : public QObject
{
    Q_OBJECT
public:
    explicit KeywordDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setState(int state) { this->state = state; }
    void setUserId( int userid) { this->userid = userid; }
    int getUserId() { return userid; }
    void setHotkey(int hotkey) { this->hotkey = hotkey; }
    int getHotkey() { return hotkey; }
    void setMostkey(int mostkey) { this->mostkey = mostkey; }
    int getMostkey() { return mostkey; }
    void setKeyname(QString keyname) { this->keyname = keyname; }
    QString getKeyname() { return keyname; }


    void setBaseUrl(QString url) { this->baseUrl = url; }
    QString getBaseUrl() { return baseUrl; }

    void add(); //增加一个标签记录
    void find(); //根据标签id查找一条记录

private:
    int id;
    int state;
    int userid;
    int hotkey;
    int mostkey;
    QString keyname;

    QString baseUrl;

signals:
    void findFinish(QJsonObject);

private slots:
    void requestReply(const QJsonObject &jsonObj);

//public slots:
//    void findState(QJsonObject);
};

#endif // KEYWORDDB_H
