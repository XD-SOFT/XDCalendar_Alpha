#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QObject>
#include <QDate>
#include <QVector>
#include <QDebug>

class Schedule : public QObject
{
    Q_OBJECT
public:
    //explicit Schedule(QObject *parent = 0);

    //explicit Schedule(QDate startTime, QDate endTime, QString title, int yearStart = 2016, int ownership = 0, QDate createTime = QDate(2016,1,1), QDate updateTime = QDate(2016,1,1)) : mTitle(title), mCreateTime(createTime), mUpdateTime(updateTime), mYearStart(yearStart),  mOwnership(ownership), mStartTime(startTime), mEndTime(endTime) {}
    //Schedule(const Schedule &se) :QObject (), mTitle(se.mTitle), mCreateTime(se.mCreateTime), mUpdateTime(se.mUpdateTime), mYearStart(se.mYearStart), mOwnership(se.mOwnership),mStartTime(se.mStartTime), mEndTime(se.mEndTime){}
    Schedule(){}
    Schedule(const QJsonObject& json, QObject* parent = 0);
    QString gradesStr(/*QVector<int> gradesInt*/)
    {
        QString str;
        for (int i = 0; i < mGrades.size(); i++)
        {
            if(i != mGrades.size() -1)
            {
                str += QString::number(mGrades[i]) + ',';
            }
            else
            {
                str += QString::number(mGrades[i]);
            }
        }
        qDebug()<<"gradesStr: "<<str<<endl;
        return str;
    }

    QVector<int> gradesInt(QString gradesStr)
    {
        QVector<int> grades;
        QStringList gradesList = gradesStr.split(',');
        for (int i = 0; i < gradesList.size(); i++)
        {
            QString str = gradesList[i];
            grades.append(str.toInt());
        }
        return grades;
    }

    void setGrades(QString grade_str)
    {
        this->mGrades = gradesInt(grade_str);
    }

    QString getName(){ return name; }
    void setName(QString name) { this->name = name; }
    QString getTerm(){ return term; }
    void setTerm(QString term) { this->term = term; }
    int getId() { return id; }
    void setId(int id) { this->id = id; }
    int getTermId() { return fk_termid; }
    void setTermId(int termId) { this->fk_termid = termId; }
    QDate getCreateTime() { return mCreateTime; }
    void setCreateTime(QDate date) {this->mCreateTime = date; }
    QDate getUpdateTime() { return mUpdateTime; }
    void setUpdateTime(QDate date) {this->mUpdateTime = date; }
    void setSection(int section) { this->section = section; }
    int getSection() { return section; }
    QDate getStartTime() { return mStartDate; }
    void setStartTime(QDate date) {this->mStartDate = date; }
    QDate getEndTime() { return mEndDate; }
    void setEndTime(QDate date) {this->mEndDate = date; }

    void update(){emit scheduleChanged(this);}
    QJsonObject save();
    void load(const QJsonObject &json);

signals:
    void scheduleChanged(Schedule *);

private:
    int id;
    int fk_termid;
    QString name;
    QString term;
    QDate mCreateTime;
    QDate mUpdateTime;
    int section;
    QDate mStartDate;
    QDate mEndDate;
    QVector<int> mGrades;


};

#endif // SCHEDULE_H
