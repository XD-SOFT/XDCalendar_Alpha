/**
 * XDCalendar Project 2017-01-22 version
  * 课表、课程等数据管理类对象管理类，负责这些对象的创建使用,这个类暂时用来处理这些数据类不停的new 连接信号问题，避免过多的使用单例.
  *
  * @author BiXiaoxia
  * @version 2017.01.22.
*/
#ifndef XDKJSOFT_DATACLASSINSTANCEMANAGE_H
#define XDKJSOFT_DATACLASSINSTANCEMANAGE_H

class DataClassInstanceManagePrivate;
class TermDB;
class LessonRemarkDB;
class LessonDetailDB;
class ResFilesDB;
class UserDB;

class DataClassInstanceManage
{
public:
    static DataClassInstanceManage *getInstance();
    static void destroyInstance();

    ///@note 下面这些控制类，都为QObject派生类，重构时候考虑信号控制在一个信号控制类里，这些访问控制类全作为普通类设计.
    /// 不再从QObject派生.

    ///@brief 获取学期网络数据库访问控制对象指针.
    TermDB *getTemDBPtr();

    ///@brief 获取课程备注数据库访问控制对象指针.
    LessonRemarkDB *getLessonRemarkDBPtr();

    ///@brief 获取课程详细信息网络数据库访问对象指针.
    LessonDetailDB *getLessonDetailDBPtr();

    ///@brief 获取文件资源网络数据库访问对象指针.
    ResFilesDB *getResFilesDBPtr();

    ///@brief 获取用户信息数据库对象指针.
    UserDB *getUserDBPtr();

private:
    DataClassInstanceManage();
    ~DataClassInstanceManage();
    DataClassInstanceManage(const DataClassInstanceManage &other);
    DataClassInstanceManage& operator=(const DataClassInstanceManage &other);

    static DataClassInstanceManage *m_pInstance;

    DataClassInstanceManagePrivate *m_pPrivate;
};

#endif // XDKJSOFT_DATACLASSINSTANCEMANAGE_H
