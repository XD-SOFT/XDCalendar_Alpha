#ifndef FRIEND_H
#define FRIEND_H
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QDate>
#include <memory>
class Friend : public QObject
{
Q_OBJECT
public:
    Friend(const QJsonObject& json, QObject* parent = 0);
    Friend(int uid, QString name, QString instituteName, QString iconPath, QDate regDate) : mUid(uid), mName(name), mInstituteName(instituteName), mIconPath(iconPath), mRegDate(regDate){}

    int getUid(){return mUid;}
    QString getName(){return mName;}
    int getGender(){return mGender;}
    QString getNickName(){return mNickName;}
    QString getDisplayName(){return mNickName.isEmpty() ? mName : mNickName;}
    QString getInstituteName(){return mInstituteName;}
    int getIconStatus(){return mIconStatus;}
    QString getIconPath(){return mIconPath;}
    QDate getRegDate(){return mRegDate;}
    QString getLocation(){return mLocation;}
    int getLocationId(){return mLocationId;}
    QDate getBirthday(){return mBirthday;}
    QString getSubject(){return mSubject;}
    QString getEmail(){return mEmail;}
    QString getPhoneNum(){return mPhoneNum;}

    int getStatus(){return mStatus;}
    int getLiveStatus(){return mLiveStatus;}

    QString getThirdPersonPronoun();
    QString getLiveStatusToggleString();
    QString getGenderString(){return getGender() == 2 ? tr("女") : tr("男");}
    QString getStatusString();

    void setUid(const int uid){mUid = uid;}
    void setName(const QString &name){mName = name;}
    void setGender(const int gender){mGender = gender;}
    void setNickName(const QString &nickName){mNickName = nickName;}
    void setInstituteName(const QString &instituteName){mInstituteName = instituteName;}
    void setIconStatus(const int status){mIconStatus = status;}
    void setIconPath(const QString &path){mIconPath = path;}
    void setRegDate(const QDate &regDate){mRegDate = regDate;}
    void setLocation(const QString &location, int id){mLocation = location; mLocationId = id;}
    void setBirthday(const QDate &date){mBirthday = date;}
    void setSubject(const QString &subject){mSubject = subject;}
    void setEmail(const QString &email){mEmail = email;}
    void setPhoneNum(const QString &num){mPhoneNum = num;}

    void setStatus(int status){mStatus = status;}
    void setLiveStatus(int status){mLiveStatus = status;}

public slots:
    void changeNickName(QString);
    void changeLiveStatusAlter();
signals:
    void dataChanged(Friend *); //all data, except icon
    void liveStatusChanged();

private:
    int mUid;
    QString mName;
    int mGender = 0;
    QString mNickName;
    QString mInstituteName;
    int mIconStatus;
    QString mIconPath;
    QDate mRegDate;

    QString mLocation;
    int mLocationId;
    QDate mBirthday;
    QString mSubject;
    QString mEmail;
    QString mPhoneNum;

    int mStatus;
    int mLiveStatus = 1;
};

#endif // FRIEND_H
