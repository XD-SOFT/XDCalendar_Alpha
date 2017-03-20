#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QJsonObject>
#include <QHash>
#include <QVector>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

///Mark2017.02.14，这个Handler重构考虑分成多个，一个接口，多个类别的handler.
 ///Mark,这个类会多次创建多次连接信号....修改到源码中去.局部对类设计进行调整,暂时肩负信息处理中心用.
 /// Mark 2017.01.22,因为这个类担负着多个种类的请求，所以重构时候考虑采取简单工厂模式创建多个实例，每一个实例担负着请求处理功能.
class Handler: public QObject
{
    Q_OBJECT
public:
    /*!
      * \enum RequestType,请求种类枚举类.
      * \var Login, 登录.
      * \var Term, 学期.
      * \var Schedule, 课表.
      * \var ScheduleDetailDB, 作息时间表.
      * \var LessonDB,教师课程表主表.
      * \var LessonDetailDB,这个是什么？.
      * \var LessonRemarkDB, 课程备注数据.
      * \var ResFileDB,资源文件数据.
      * \var SubjectInfoDB, 本校学科数据?!!!.
      * \var GradeInfoDB,年纪信息数据.
      * \var ClientConfigDB,客户端配置数据.
      * \var KeywordDB, 标签记录数据？？？!.(带有？的概念暂时不太清晰,原设计者想表达什么,需进一步沟通.)
      * \var FileKeywordDB, 文件记录数据.
      * \var DataNW, 这个想表达什么数据???.
      * \var ScheDetDB,这个是什么？？.
      * \var UserInfomation,
      * \var ModifiedPassword,这个跟UserInfomation是UserDB里面用到的，现在暂时放进这里，重构再考虑.
    */
    Q_ENUMS(RequestType)
    enum RequestType {Login, TermDB, Schedule, ScheduleDetailDB, LessonDB, LessonDetailDB, LessonRemarkDB, ResFileDB, SubjectInfoDB,
                     GradeInfoDB, ClientConfigDB, KeywordDB, FileKeywordDB, DataNW, ScheDetDB, UserInfomation, ModifiedPassword, Default = 10000};
    /*!
     * \brief getInstance, 获取request handler单一实例对象.
     * \return Handler*;
     */
    static Handler *getInstance();

    /*!
     * \brief setRequestUrl,设置请求url.
     * \param arguments, 请求url字符串.
     */
    void requestByUrlAndType(const QString &arguments, RequestType type);


    /*!
     * \brief destroyInstance,销毁Handler单一实例对象.
     */
    static void destroyInstance();

    void reset();

signals:
    void updateUi(QJsonObject);

    /*!
     * \brief loginStateChanged,登录状态改变.Mark这个暂时实现在这里，以后重构再仔细考虑.
     * \param jsonObj
     */
    void loginStateChanged(const QJsonObject &jsonObj);

    /*!
     * \brief termReplyArrived, 续期请求应答到达信号.
     * \param jsonObj
     */
    void termDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief scheduleReplyArrived,课表请求应答到达信号.
     * \param jsonObj
     */
    void scheduleReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief scheduleDetailDBReplyArrived, 作息时间应答到达信号.
     * \param jsonObj
     */
    void scheduleDetailDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief lessonDBReplyArrived,课表数据应答到达信号.
     * \param jsonObj
     */
    void lessonDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief lessonDetailDBReplyArrived,?.
     * \param jsonObj
     */
    void lessonDetailDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief lessonRemarkDBReplyArrived,课程备注数据到达信号.
     * \param jsonObj
     */
    void lessonRemarkDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief resFileDBReplayArrived,资源文件数据应答到达信号.
     * \param jsonObj
     */
    void resFileDBReplayArrived(const QJsonObject &jsonObj);

    /*!
     * \brief subjectInfoDBReplyArrived,本校学科数据应答到达信号.
     * \param jsonObj
     * \note 2017.01.22.这个概念不是很清晰，需要进一步跟最初开发人员交流....
     */
    void subjectInfoDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief gradeInfoDBReplyArrived,年级信息数据到达信号.
     * \param jsonObj
     */
    void gradeInfoDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief clientConfigDBReplyArrived,客户端配置数据应答到达信号.
     * \param jsonObj
     */
    void clientConfigDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief keywordDBReplyArrived,keyword数据应答到达信号.
     * \param jsonObj
     */
    void keywordDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief fileKeyWordDBReplyArrived, 文件记录数据请求到达信号.
     * \param jsonObj
     */
    void fileKeyWordDBReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief dataNWReplyArrived,什么？？数据应答到达信号.
     * \param jsonObj
     */
    void dataNWReplyArrived(const QJsonObject &jsonObj);

    /*!
     * \brief scheDetDBReplyArrived,这个什么数据？？应答到达信号.
     * \param jsonObj
     */
    void scheDetDBReplyArrived(const QJsonObject &jsonObj);

    ///@\brief 获取用户信息.
    ///
    void getUserInfomationFinished(const QJsonObject &jsonObj);

    ///@\brief 修改密码完成.
    void modifiedPasswordFinished(const QJsonObject &jsonObj);

    ///@\brief 登录错误信息信号.
    void loginRequestError(const QString &sError);

public slots:
    void Finished(QNetworkReply *reply);
//    {
//        //qDebug()<<"handler Finished"<<endl;
//        if (reply->error() == QNetworkReply::NoError)
//             {
//                 QByteArray bytes = reply->readAll();
//                 //qDebug()<<bytes;
//                 //QString res = QString::fromUtf8(bytes);
//                 //qDebug()<<"report: "<<res<<endl;
//                 QJsonObject jo = QJsonDocument::fromJson(bytes).object();
//                 //qDebug()<<jo<<endl;
//                 //qDebug() << "jo size: " <<jo.length()<<endl;
//                 //qDebug() << "jo: " << jo["summary\\"].toArray()<<endl;


//                 if(jo["status"] == "tokenFalseRedirect")
//                 {
//                     qDebug()<<"false token"<<endl;
//                     exit(-1);
//                     //return;
//                     //QMessageBox::information(this, "提示", "用户名或密码错误");
//                 }
//                 else
//                 {
//                     emit updateUi(jo);
//                 }

//             }
//         else
//             {
//                 reconnect++;
//                 qDebug()<<"handle errors here";
//                 QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
//                 //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
//                 qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
//                 qDebug(qPrintable(reply->errorString()));
//                 /*
//                 if(reconnect<=10)
//                 {
//                     run();
//                 }
//                 */
//                 //QMessageBox::information(this, "提示", "服务器连接错误");
//             }
//             reply->deleteLater();

//    }

private:
    Handler(QObject *parent = Q_NULLPTR);
//    Handler(const QString &arguments, QObject *parent = Q_NULLPTR);

    ~Handler();

    void run();

    Handler(const Handler &other);

    Handler& operator=(const Handler &other);

private:
    static Handler *m_pInstance;
    int reconnect = 0;
    QString m_reqArguments;
    QNetworkAccessManager* m_pManager;
    QNetworkRequest *m_pRequest;

    /*!
     * \brief requestUrlTypeHash, 请求url与类型对应hash表，因为应答与请求不是同步，所以需要对返回的reply的request进行判断，避免应答.
     * 与请求不一致.
     */
    QHash<QString, RequestType> requestUrlTypeHash;

    ///< \brief 请求类型数组, 它与requestUrlTypeHash保证请求返回处理不错乱，返回一个reply时候先获取url，在requestUrlTypeHash查找type.
    /// 在用这个type查询m_requestTypeVec里存在不，存在则对应处理，并移除m_requestTypeVec里的类型，当m_requestTypeQueue出队列为空.
    /// 要对应清除requestUrlTypeHash，否则不能清空requestUrlTypeHash.
    QVector<RequestType> m_requestTypeVec;

    //请求reply链表.
    QList<QNetworkReply*> m_replyList;
};

#endif // HANDLER_H
