#ifndef SCHEDETDB_H
#define SCHEDETDB_H

#include <QObject>
#include <QJsonObject>

class ScheDetDB : public QObject
{
    Q_OBJECT
public:
    explicit ScheDetDB(QObject *parent);

    int getId() { return id; }
    int getScheId() { return scheId; }
    QString getName() { return name; }
    QString getStartTime() { return starttime; }
    QString getEndTime() { return endtime; }
    int getType() { return type; }
    int getSecId() { return secId; }
    int getState() { return state; }

    void setId(int id) { this->id = id; }
    void setScheId(int scheId) { this->scheId = scheId; }
    void setName(QString name) { this->name = name; }
    void setStartTime(QString starttime) { this->starttime = starttime; }
    void setEndTime(QString endtime) { this->endtime = endtime; }
    void setType(int type) { this->type = type; }
    void setSecId(int secId) { this->secId = secId; }
    void setState(int state) { this->state = state; }

    void add(); //新增一条记录
    void delUserSchedule(); //删除一条用户的作息时间详细记录（state = 0）
    void findBySche(); //根据作息时间主表Id查找一条记录
    void updateUserScheDet(); //更新一个用户的作息时间明细

private:
    enum RequestCode{Add, Find, Update, Delete};
    RequestCode m_curRequestCode;
    int id;
    int scheId;
    QString name;
    QString starttime;
    QString endtime;
    int type;
    int secId;
    int state;

//    QString baseUrl;

private slots:
    void replyArrived(const QJsonObject &jsonObj);

signals:
    void addFinish(QJsonObject); //发射信号更新前端界面
    void findFinish(QJsonObject);
    void updateFinish(QJsonObject);
    void delFinish(QJsonObject);

//public slots:

//    void addState(QJsonObject); //handler获取数据后返回给该槽函数
//    void findState(QJsonObject);
//    void updateState(QJsonObject);
//    void delState(QJsonObject);
};

#endif // SCHEDETDB_H
