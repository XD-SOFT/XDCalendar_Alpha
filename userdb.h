#ifndef USERDB_H
#define USERDB_H

#include <QObject>
#include <QJsonObject>

class UserDB : public QObject //用户登录表 客户端主要进行查询
{
    Q_OBJECT
public:
    explicit UserDB(QObject *parent = Q_NULLPTR);

    void modifiedPasswordByUserName(const QString &sUserName, const QString &sPassword);

    void setId(int id) { this->id = id; }
    int getId() { return id; }
    void setState(int state) { this->state = state; }
    int getState() { return state; }
    void setName(QString name) { this->name = name; }
    QString getName() { return name; }
    void setUsername(QString username) { this->username = username;}
    QString getUsername() { return username; }
    void setPassword(QString password) { this->password = password; }
    QString getPassword() { return password; }
    void setLogintype(int type) { this->logintype = type; }
    int getLogintype() { return logintype; }
    void setUsertype(int type) { this->usertype = type; }
    int getUsertype() { return usertype; }
    void setEmail(QString email) { this->email = email; }
    QString getEmail() { return email; }
    void setMobile(QString mobile) { this->mobile = mobile; }
    QString getMobile() { return mobile; }
    void setSex(QString sex) { this->sex = sex; }
    QString getSex() { return sex; }
    void setAge(int age) { this->age = age; }
    int getAge() { return age; }
    void setAddress(QString address) { this->address = address; }
    QString getAddress() { return address; }
    void setRemark(QString remark) { this->remark = remark; }

//    void setBaseUrl(QString url) { this->baseUrl = url; }
//    QString getBaseUrl() { return baseUrl; }

    void query(); //根据用户名密码查找


private:
    int id;
    QString username;
    QString password;
    int state;
    int logintype; //0-username 1-mobile 2-email
    int usertype; //0-后台用户 1-前台教师
    QString email;
    QString mobile;
    QString name;
    QString sex;
    int age;

    QString address;
    QString remark;

//    QString baseUrl;

signals:
    //void start();
    void updateLoginUI(const QJsonObject&); //发射信号更新前端界面


    void modifiedPasswordByUserNameFinished(const QString &sUserName, const QString &setPassword);

    void loginRequestError(const QString &sError);

public slots:
    void visitState(const QJsonObject &); //handler获取数据后返回给该槽函数

private slots:
    void setUserInfomation(const QJsonObject &jsonObj);

    void modifiedPasswordFinished(const QJsonObject &jsonObj);

private:
    QString m_sPassword;

    QString m_sUserName;
};

#endif // USERDB_H
