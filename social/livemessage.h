#ifndef LIVEMESSAGE_H
#define LIVEMESSAGE_H

#include "friend.h"
#include <QDateTime>

class LiveMessage
{
public:
    LiveMessage(Friend *fr, const QString &message, const QDateTime &releaseDateTime) : mFriend(fr), mMessage(message), mReleaseDateTime(releaseDateTime) {};

    Friend *getFriend(){return mFriend;}
    const QString &getMessage(){return mMessage;}
    const QDateTime &getReleaseDateTime(){return mReleaseDateTime;}

private:
    Friend *mFriend;
    QString mMessage;
    QDateTime mReleaseDateTime;
};

#endif // LIVEMESSAGE_H
