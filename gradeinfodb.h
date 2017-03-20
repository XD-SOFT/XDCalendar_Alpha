#ifndef GRADEINFODB_H
#define GRADEINFODB_H

#include <QObject>
#include <QJsonObject>

class GradeInfoDB : public QObject //学校年级表
{
    Q_OBJECT
public:
    explicit GradeInfoDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setName(QString name) { this->name = name; }
    QString getName() { return name; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void query();

private:
    int id;
    int state;
    QString name;

//    QString baseUrl;

signals:
    void updateUI(QJsonObject); //发射信号更新前端界面

private slots:
    void requestReply(const QJsonObject &jsonObj);


//public slots:
//    void visitState(QJsonObject); //handler获取数据后返回给该槽函数

};

#endif // GRADEINFODB_H
