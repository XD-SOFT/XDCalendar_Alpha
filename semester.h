#ifndef SEMESTER_H
#define SEMESTER_H
#include <QObject>
#include <QDate>

class QJsonObject;

class Semester : public QObject
{
    Q_OBJECT
public:
    explicit Semester(QDate startTime, QDate endTime, QString title, int yearStart = 2016, int ownership = 0, QDate createTime = QDate(2016,1,1), QDate updateTime = QDate(2016,1,1)) : mTitle(title), mCreateTime(createTime), mUpdateTime(updateTime), mYearStart(yearStart),  mOwnership(ownership), mStartTime(startTime), mEndTime(endTime) {}
    Semester(const Semester &se) :QObject (), mTitle(se.mTitle), mCreateTime(se.mCreateTime), mUpdateTime(se.mUpdateTime), mYearStart(se.mYearStart), mOwnership(se.mOwnership),mStartTime(se.mStartTime), mEndTime(se.mEndTime){}
    Semester(const QJsonObject& json, QObject* parent = 0);
    Semester(){}

    int getId() { return id; }
    QString getTitle() const {return mTitle;}
    QString getTerm() const {return mTerm;}
    QDate getCreateTime() const {return mCreateTime;}
    QDate getUpdateTime() const {return mUpdateTime;}
    int getYearStart() const {return mYearStart;}
    int getOwnership() const {return mOwnership;}
    QDate getStartTime() const {return mStartTime;}
    QDate getEndTime() const {return mEndTime;}


    void setId(int id) { this->id = id; }
    void setTitle(QString title) {mTitle = title;}
    void setTerm(QString term) {mTerm = term;}
    void setCreateTime(QDate createTime) {mCreateTime = createTime;}
    void setUpdateTime(QDate updateTime) {mUpdateTime = updateTime;}
    void setYearStart(int yearStart) {mYearStart = yearStart;}
    void setOwnership(int ownership) {mOwnership = ownership;}
    void setStartTime(QDate startTime) {mStartTime = startTime;}
    void setEndTime(QDate endTime) {mEndTime = endTime;}

    ~Semester () = default;

    void update(){emit semesterChanged(this);}
    QJsonObject save() const;
    void load(const QJsonObject &json);

signals:
    void semesterChanged(Semester *);

private:
    int id;

    QString mTitle;
    QString mTerm;
    QDate mCreateTime;
    QDate mUpdateTime;
    int mYearStart;
    int mOwnership;
    QDate mStartTime;
    QDate mEndTime;
};

#endif // SEMESTER_H
