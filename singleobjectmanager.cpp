#include "singleobjectmanager.h"
#include "lessondetaildb.h"
#include <QMutex>
#include <QDebug>

class SingleObjectManagerPrivate {
    LessonDetailDB *pLessonDB = Q_NULLPTR;

    ~SingleObjectManagerPrivate() {
        if(pLessonDB != Q_NULLPTR) {
            delete pLessonDB;
            pLessonDB = Q_NULLPTR;
        }
    }

    friend class SingleObjectManager;
};

SingleObjectManager *SingleObjectManager::m_pInstance = Q_NULLPTR;

SingleObjectManager *SingleObjectManager::getInstance()
{
    if (m_pInstance == Q_NULLPTR)
    {
        QMutex mutex;
        mutex.lock();

        SingleObjectManager *tempInstance = m_pInstance;

        if(m_pInstance == NULL)
        {
            tempInstance = new SingleObjectManager;
            m_pInstance = tempInstance;
        }

        mutex.unlock();
    }

    return m_pInstance;
}

void SingleObjectManager::destroyInstance()
{
    qDebug() << "deleting~~~~~~" << m_pInstance;
    if(m_pInstance != NULL)
    {
        static QMutex mutex;
        mutex.lock();

        if(m_pInstance != NULL)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }

        mutex.unlock();
    }
}

LessonDetailDB *SingleObjectManager::getLessonDetailDB()
{
    if(m_pPrivate->pLessonDB == Q_NULLPTR) {
        m_pPrivate->pLessonDB = new LessonDetailDB;
    }

    return m_pPrivate->pLessonDB;
}

SingleObjectManager::SingleObjectManager()
{

}

SingleObjectManager::~SingleObjectManager()
{
    delete m_pPrivate;
    m_pPrivate = Q_NULLPTR;
}
