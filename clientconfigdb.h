#ifndef CLIENTCONFIGDB_H
#define CLIENTCONFIGDB_H

#include <QObject>
#include <QDate>
#include <QJsonObject>
#include "handler.h"

class ClientConfigDB : public QObject
{
    Q_OBJECT
public:
    explicit ClientConfigDB(QObject *parent = 0);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setMac(QString mac) { this->mac = mac; }
    QString getMac() { return mac; }
    void setFilename(QString filename) { this->filename = filename; }
    QString getFilename() { return filename; }
    void setUpdatetime(QDate updatetime) { this->updateDate = updatetime; }
    QDate getUpdatetime() { return updateDate; }

    void setBaseUrl(QString url) { this->baseUrl = url; }
    QString getBaseUrl() { return baseUrl; }

    void add(); //新增一条记录
    void update(); //更新一条记录
    void find(); //查找一条记录

private slots:
    /*!
     * \brief dealReply, 请求应答到达处理函数.
     * \param jsonObj
     */
    void handleReply(const QJsonObject &jsonObj);

private:
    /*!
     * \brief The RequestType enum, 请求代码.
     * \var Add, 添加纪录.
     * \var Update, 更新记录.
     * \var Find,查找记录.
     */
    enum RequestType {Add, Update, Find};

    RequestType m_curRequestType;

    int id;
    int userId;
    QString url; //配置文件本地存储地址
    QString mac; //客户端mac地址
    QDate updateDate; //更新日期
    QString filename; //

    QString baseUrl;

    //说明：
    //    1.该表是记录客户端同步上传的配置文件。
    //    2.配置文件存储地址是存储在userconfig下，并且以userid新建用户文件夹。
    //    3.文件名称根据用户上传时间和用户的ID生成年+月+日+时+秒+毫秒+用户四位ID
    //    4.记录用户上传的MAC地址。

signals:
    void addClientConfigFinished(const QJsonObject &jsonObj);

    void findClientConfigFinished(const QJsonObject &jsonObj);

    void updateClientConfigFinished(const QJsonObject &jsonObj);
//    void addFinish(QJsonObject); //发射信号更新前端界面
//    void findFinish(QJsonObject);
//    void updateFinish(QJsonObject);

//public slots:
//    void addState(QJsonObject);
//    void findState(QJsonObject);
//    void updateState(QJsonObject);
};

#endif // CLIENTCONFIGDB_H
