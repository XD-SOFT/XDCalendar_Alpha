#ifndef RESFILESDB_H
#define RESFILESDB_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QVector>
#include <QJsonObject>
#include "handler.h"

class ResFilesDB : public QObject
{
    Q_OBJECT
public:
    explicit ResFilesDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setUserId(int userId) { this->userId = userId; }
    int getUserId() { return userId; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setLessonDetailId(int lessonDetailId) { this->lessonDetailId = lessonDetailId; }
    int getLessonDetailId() { return lessonDetailId; }
    void setRemark(const QString &remark) { this->remark = remark; }
    QString getRemark() { return remark; }
    void setUpdateTime(const QDate &updatetime) { this->updatetime = updatetime; }
    QDate getUpdateTime() { return updatetime; }
    void setUseDataTime(const QDate &usedatatime) { this->usedatatime = usedatatime; }
    QDate getUsedDataTime() { return usedatatime; }
    void setFileUrl(const QString &url) { this->fileurl = url; }
    QString getFileUrl() { return fileurl; }
    void setFileName(const QString &filename) { this->filename = filename; }
    QString getFileName() { return filename; }
    void setFilePic(const QString &filepic) { this->filepic = filepic; }
    QString getFilePic() { return filepic; }
    void setFileIco(const QString &fileico) { this->fileico = fileico; }
    QString getFileIco() { return fileico; }
    void setKeyword(const QVector<int> &keyword) { this->keyword = keyword; }
    QVector<int> getKeyword() { return keyword; }

    void setBaseUrl(const QString &url) { this->baseUrl = url; }
    QString getBaseUrl() { return baseUrl; }

    void add(); //为某一课程格子增加资源文件
    void del(); //删除某一课程格子的某一资源文件
    void update(); //更新某一课程格子的某一资源文件
    void find(); //获取某一课程格子的所有资源文件

private slots:
    void replyArrived(const QJsonObject &jsonObj);

private:
    /*!
     * \brief The RequestCode enum Mark所有的RequestCode重构时候可以移到Handler里面去，这里为了好改造，暂时都放入各自头文件.
     */
    enum RequestCode{Add, Delete, Update, Find};
    RequestCode m_curRequestCode;

    int id;
    int lessonDetailId;

    QString remark; //资源备注表（上课用，积累素材），目前均为上课用
    QDate updatetime; //上传时间
    QDate usedatatime; //使用时间 哪节课使用
    QString fileurl; //文件地址
    QString filename;
    int userId;
    int state;

    //not sure if needed
    QString filepic; //文件缩略图
    QString fileico; //文件图标
    QVector<int> keyword; //关键字Id集合

    QString baseUrl;

signals:
    void addFinish(const QJsonObject&); //发射信号更新前端界面
    void findFinish(const QJsonObject&);
    void delFinish(const QJsonObject&);
    void updateFinish(QJsonObject);

//public slots:
//    void addState(QJsonObject);
//    void findState(QJsonObject);
//    void delState(QJsonObject);
//    void updateState(QJsonObject);

};

#endif // RESFILESDB_H
