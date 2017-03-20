/**
 * XDCalendar Project 2017-02-13 version
  * 工程中所有只有一个实例的类的管理类，该类本身是一个单例类，为其它类的创建者.
  *
  * @author BiXiaoxia
  * @version 2017.02.13.
*/
#ifndef SINGLEOBJECTMANAGER_H
#define SINGLEOBJECTMANAGER_H

class SingleObjectManagerPrivate;
class LessonDetailDB;

class SingleObjectManager
{
public:
    static SingleObjectManager *getInstance();

    static void destroyInstance();

    /*!
     * \brief getLessonDetailDB
     * \return
     */
    LessonDetailDB *getLessonDetailDB();


private:
    void createLessonDB();

private:
    SingleObjectManager();
    ~SingleObjectManager();
    SingleObjectManager(const SingleObjectManager &other);
    SingleObjectManager &operator=(const SingleObjectManager &other);

private:
    SingleObjectManagerPrivate *m_pPrivate;
    static SingleObjectManager *m_pInstance;
};

#endif // SINGLEOBJECTMANAGER_H
