#ifndef T_LOG_NETWORK_H
#define T_LOG_NETWORK_H

#include "singleton.h"
#include <QDateTime>
typedef struct{
    QString mNetworkId;
    QString mPhone;
    QString mUrl;
    QString mContent;
    qint8 mSmsType;
    QString mRelPriKey;
    qint8 mStatus; // 1:成功 2：失败
    QDateTime mSendTime;
}NetworkDesc_t;

typedef struct{
    QString cellId;
    qint8 flowType;
    qint8 errorType;
    QString Relkey;
}NetCellExcept;


class CNetwork
{
public:
    CNetwork();

    void init();
    bool insertNetworkInfo(NetworkDesc_t networkInfo);

    bool queryNetworkInfo(QList<NetworkDesc_t>& networkList,qint8 status);

    void deleteNetWorkInfo();

    DECLARE_SINGLETON_CLASS(CNetwork)
};
typedef Singleton<CNetwork> SCNetwork;

#endif // T_LOG_NETWORK_H
