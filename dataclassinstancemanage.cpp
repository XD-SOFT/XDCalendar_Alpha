#include "dataclassinstancemanage.h"
#include "termdb.h"
#include "lessonremarkdb.h"
#include "lessondetaildb.h"
#include "resfilesdb.h"
#include "userdb.h"
#include <QMutex>

class DataClassInstanceManagePrivate {
    friend class DataClassInstanceManage;
    TermDB *pTermDB = Q_NULLPTR;
    LessonRemarkDB *pLessonRemarkDB = Q_NULLPTR;
    LessonDetailDB *pLessonDetailDB = Q_NULLPTR;
    ResFilesDB *pResFilesDB = Q_NULLPTR;
    UserDB *pUserDB = Q_NULLPTR;

    QString sNetHostUrl;

    ~DataClassInstanceManagePrivate() {
        if(pTermDB != Q_NULLPTR) {
            delete pTermDB;
            pTermDB = Q_NULLPTR;
        }

        if(pLessonRemarkDB != Q_NULLPTR){
            delete pLessonRemarkDB;
            pLessonRemarkDB = Q_NULLPTR;
        }

        if(pLessonDetailDB != Q_NULLPTR){
            delete pLessonDetailDB;
            pLessonDetailDB = Q_NULLPTR;
        }

        if(pResFilesDB != Q_NULLPTR) {
            delete pResFilesDB;
            pResFilesDB = Q_NULLPTR;
        }

        if(pUserDB != Q_NULLPTR) {
            delete pUserDB;
            pUserDB = Q_NULLPTR;
        }
    }
};

DataClassInstanceManage *DataClassInstanceManage::m_pInstance = Q_NULLPTR;

DataClassInstanceManage *DataClassInstanceManage::getInstance()
{
    if(m_pInstance == Q_NULLPTR) {
        static QMutex mutex;
        mutex.lock();

        DataClassInstanceManage *pInstance = m_pInstance;

        if(m_pInstance == Q_NULLPTR) {
            pInstance = new DataClassInstanceManage;

            if(m_pInstance == Q_NULLPTR) {
                m_pInstance = pInstance;
            }
            else {
                delete pInstance;
                pInstance = Q_NULLPTR;
            }
        }

        mutex.unlock();
    }

    return m_pInstance;
}

void DataClassInstanceManage::destroyInstance()
{
    if(m_pInstance != Q_NULLPTR) {
        static QMutex mutex;
        mutex.lock();

        delete m_pInstance;
        m_pInstance = Q_NULLPTR;

        mutex.unlock();
    }
}

TermDB *DataClassInstanceManage::getTemDBPtr()
{
    if(m_pPrivate->pTermDB == Q_NULLPTR) {
        m_pPrivate->pTermDB = new TermDB;
    }

    return m_pPrivate->pTermDB;
}

LessonRemarkDB *DataClassInstanceManage::getLessonRemarkDBPtr()
{
    if(m_pPrivate->pLessonRemarkDB == Q_NULLPTR) {
        m_pPrivate->pLessonRemarkDB = new LessonRemarkDB();
    }

    return m_pPrivate->pLessonRemarkDB;
}

LessonDetailDB *DataClassInstanceManage::getLessonDetailDBPtr()
{
    if(m_pPrivate->pLessonDetailDB == Q_NULLPTR) {
        m_pPrivate->pLessonDetailDB = new LessonDetailDB();
    }

    return m_pPrivate->pLessonDetailDB;
}

ResFilesDB *DataClassInstanceManage::getResFilesDBPtr()
{
    if(m_pPrivate->pResFilesDB == Q_NULLPTR) {
        m_pPrivate->pResFilesDB = new ResFilesDB;
    }

    return m_pPrivate->pResFilesDB;
}

UserDB *DataClassInstanceManage::getUserDBPtr()
{
   if(m_pPrivate->pUserDB == Q_NULLPTR) {
       m_pPrivate->pUserDB = new UserDB;
   }

   return m_pPrivate->pUserDB;
}

DataClassInstanceManage::DataClassInstanceManage():
    m_pPrivate(new DataClassInstanceManagePrivate)
{

}

DataClassInstanceManage::~DataClassInstanceManage()
{
    delete m_pPrivate;
    m_pPrivate = Q_NULLPTR;   
}
