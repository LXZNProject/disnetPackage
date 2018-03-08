#include "t_log_network.h"

#include <QSqlQuery>
#include <QVariant>
CNetwork::CNetwork()
{
}

void CNetwork::init()
{
    QSqlQuery query;
    query.exec("create table t_log_network(networkId varchar(32) primary key, userMobilephone varchar(18),url varchar(128), \
               content varchar(512),smsType integer,relPriKey varchar(32),status integer,msgSentTime datetime)");
}

bool CNetwork::insertNetworkInfo(NetworkDesc_t networkInfo)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_log_network WHERE networkId = ?");
    query.bindValue(0,networkInfo.mNetworkId);
    query.exec();

    if(query.first())
    {
        if(networkInfo.mStatus == 1)
        {
            query.prepare("DELETE FROM t_log_network where networkId = ?");
            query.bindValue(0,networkInfo.mNetworkId);
        }
        else
        {
            query.prepare("update t_log_network set status = ?");
            query.addBindValue(networkInfo.mStatus);
        }
    }
    else
    {
        if(networkInfo.mStatus == 1)
        {
            return true;
        }
        else
        {
            query.prepare("INSERT INTO t_log_network VALUES(?,?,?,?,?,?,?,?)");
            query.addBindValue(networkInfo.mNetworkId);
            query.addBindValue(networkInfo.mPhone);
            query.addBindValue(networkInfo.mUrl);
            query.addBindValue(networkInfo.mContent);
            query.addBindValue(networkInfo.mSmsType);
            query.addBindValue(networkInfo.mRelPriKey);
            query.addBindValue(networkInfo.mStatus);
            query.addBindValue(networkInfo.mSendTime);
        }
    }

    return query.exec();
}

bool CNetwork::queryNetworkInfo(QList<NetworkDesc_t> &networkList,qint8 status)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM t_log_network WHERE status = ? order by msgSentTime asc limit 100");
    query.bindValue(0,status);
    query.exec();

    while(query.next())
    {
        NetworkDesc_t network;
        network.mNetworkId = query.value(0).toString();
        network.mPhone = query.value(1).toString();
        network.mUrl = query.value(2).toString();
        network.mContent = query.value(3).toString();
        network.mSmsType = query.value(4).toInt();
        network.mRelPriKey = query.value(5).toString();
        network.mStatus = query.value(6).toInt();
        network.mSendTime = query.value(7).toDateTime();

        networkList.append(network);
    }

    return true;
}

void CNetwork::deleteNetWorkInfo()
{
    QDateTime time = QDateTime::fromString("2015-12-29 00:00:00",Qt::ISODate);
    QSqlQuery query;
    query.prepare("DELETE  FROM t_log_network WHERE msgSentTime < ?");
    query.bindValue(0,time);
    query.exec();
}
