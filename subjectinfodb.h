#ifndef SUBJECTINFODB_H
#define SUBJECTINFODB_H

#include <QObject>
#include <QJsonObject>

class SubjectInfoDB : public QObject //本校学科表，客户端主要用于查询
{
    Q_OBJECT
public:
    SubjectInfoDB(QObject *parent = Q_NULLPTR);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setSubjectId(int subjectId) { this->subjectId = subjectId; }
    int getSubjectId() { return subjectId; }
    void setName(QString name) { this->name = name; }
    QString getName() { return name; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }

    //void setBaseUrl(QString url) { this->baseUrl = url; }
    //QString getBaseUrl() { return baseUrl; }

    void query();

private slots:
    void replyArrived(const QJsonObject &jsonObj);

private:
    int id;
    int subjectId;
    QString name;
    int state;

//    QString baseUrl;


signals:
    void updateUI(QJsonObject); //发射信号更新前端界面


//public slots:
//    void visitState(QJsonObject); //handler获取数据后返回给该槽函数
};

#endif // SUBJECTINFODB_H
