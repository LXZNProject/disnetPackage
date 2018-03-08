#ifndef T_LOG_SMSLOG_H
#define T_LOG_SMSLOG_H


#include "singleton.h"
#include <QDateTime>

typedef struct{
    QString   mSmsLogId;
    QString mMobilePhone;
    QString mSmsContent;
    QDateTime mSendTime;
    qint8 mStatus;
    int mSmsType;
    QString mRelPriKey;
    int mSendCount;
}NoteDesc_t;

/*
class NoteDesc_t
{
public:
    NoteDesc_t(int smsLogId, QString mobilePhone, QString smsContent, QDateTime sendTime,\
               qint8 status,int smsType,int relPriKey,int sendCount = 1):
        mSmsLogId(smsLogId),
        mMobilePhone(mobilePhone),
        mSmsContent(smsContent),
        mSendTime(sendTime),
        mStatus(status),
        mSmsType(smsType),
        mRelPriKey(relPriKey),
        mSendCount(sendCount)
    {
    }

public:
    int   mSmsLogId;
    QString mMobilePhone;
    QString mSmsContent;
    QDateTime mSendTime;
    qint8 mStatus;
    int mSmsType;
    int mRelPriKey;
    int mSendCount;
};
*/
class CSmslog
{
public:
    CSmslog();

    void init();

    bool insertNoteInfo(NoteDesc_t noteInfo);
    bool updateNoteInfo(NoteDesc_t noteInfo);

    bool queryLoseNoteInfo(QList<NoteDesc_t> &noteInfo,qint8 status,qint8 loseNum);

    DECLARE_SINGLETON_CLASS(CSmslog)
};

typedef Singleton<CSmslog> SCSmslog;

#endif // T_LOG_SMSLOG_H
