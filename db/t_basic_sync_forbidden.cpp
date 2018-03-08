#include "t_basic_sync_forbidden.h"
#include <QVariant>
#include <QSqlQuery>
#include <QStringList>
#include <QDebug>

t_balckList::t_balckList()
{
}

void t_balckList::init()
{
    QSqlQuery query;
    query.exec("create table [t_basic_sync_forbidden](syncType  integer,syncValue varchar(32),lastSyncTime varchar(32))");

    lossUserInfo.clear();
    balckUserInfo.clear();
    forbidUserInfo.clear();

    getBalckUserInfo();
    getLossUserInfo();
    getForbidUserInfo();
}

void t_balckList::clearInfo(int type)
{
    if(type == 1)
    {
        lossUserInfo.clear();
    }
    else if(type == 2)
    {
        balckUserInfo.clear();
    }
    else if(type == 3)
    {
        forbidUserInfo.clear();
    }

    QSqlQuery query;
    query.prepare("DELETE from t_basic_sync_forbidden where syncType = ?");
    query.bindValue(0,type);
    query.exec();
}

void t_balckList::AnalysisInfo(QString timeStr,QString userInfo,int type)
{
    if(userInfo.isEmpty())
    {
        return;
    }
    QStringList   phoneList = userInfo.split('|');
    QList<syncIllData> deliveryInfo;
    for(int i =0;i < phoneList.size();i++)
    {
        syncIllData tempValue;
        tempValue.syncValue = phoneList.at(i);
        tempValue.lastSyncTime = timeStr;
        tempValue.syncType = type;

        deliveryInfo.append(tempValue);
    }
    clearInfo(type);
    if(type == 1)
    {
        insertLossUserInfo(deliveryInfo);
    }
    else if(type == 2)
    {
        insertBalckUserInfo(deliveryInfo);
    }
    else if(type == 3)
    {
        insertForbidUserInfo(deliveryInfo);
    }
}

void t_balckList::insertBalckUserInfo(QList<syncIllData> deliveryInfo)
{
    if(deliveryInfo.size() < 1)
    {
        return;
    }

    clearInfo(2);
    balckUserTimeStr = deliveryInfo.at(0).lastSyncTime;

    for(int i = 0;i < deliveryInfo.size();i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO t_basic_sync_forbidden VALUES(?,?,?)");
        query.addBindValue(deliveryInfo.at(i).syncType);
        query.addBindValue(deliveryInfo.at(i).syncValue);
        query.addBindValue(deliveryInfo.at(i).lastSyncTime);
        query.exec();

        balckUserInfo.append(deliveryInfo.at(i).syncValue);
    }
}

void t_balckList::insertLossUserInfo(QList<syncIllData> deliveryInfo)
{
    if(deliveryInfo.size() < 1)
    {
        return;
    }

    clearInfo(1);
    lossUserTimeStr = deliveryInfo.at(0).lastSyncTime;

    for(int i = 0;i < deliveryInfo.size();i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO t_basic_sync_forbidden VALUES(?,?,?)");
        query.addBindValue(deliveryInfo.at(i).syncType);
        query.addBindValue(deliveryInfo.at(i).syncValue);
        query.addBindValue(deliveryInfo.at(i).lastSyncTime);
        query.exec();

        lossUserInfo.append(deliveryInfo.at(i).syncValue);
    }
}

void t_balckList::insertForbidUserInfo(QList<syncIllData> deliveryInfo)
{
    if(deliveryInfo.size() < 1)
    {
        return;
    }

    clearInfo(3);
    forbidUserTimeStr = deliveryInfo.at(0).lastSyncTime;

    for(int i = 0;i < deliveryInfo.size();i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO t_basic_sync_forbidden VALUES(?,?,?)");
        query.addBindValue(deliveryInfo.at(i).syncType);
        query.addBindValue(deliveryInfo.at(i).syncValue);
        query.addBindValue(deliveryInfo.at(i).lastSyncTime);
        query.exec();

        forbidUserInfo.append(deliveryInfo.at(i).syncValue);
    }
}

void t_balckList::getBalckUserInfo()
{
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_sync_forbidden where syncType = 2");
    query.exec();

    while(query.next())
    {
        syncIllData tempvalue;
        tempvalue.syncValue = query.value(1).toString();

        balckUserTimeStr = query.value(2).toString();
        balckUserInfo.append(tempvalue.syncValue);
    }
}

void t_balckList::getLossUserInfo()
{
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_sync_forbidden where syncType = 1");
    query.exec();

    while(query.next())
    {
        syncIllData tempvalue;
        tempvalue.syncValue = query.value(1).toString();

        lossUserTimeStr = query.value(2).toString();
        lossUserInfo.append(tempvalue.syncValue);
    }
}

void t_balckList::getForbidUserInfo()
{
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_sync_forbidden where syncType = 3");
    query.exec();

    while(query.next())
    {
        syncIllData tempvalue;
        tempvalue.syncValue = query.value(1).toString();

        forbidUserTimeStr = query.value(2).toString();
        forbidUserInfo.append(tempvalue.syncValue);
    }
}

QString t_balckList::getBalckUserTime()
{
    if(balckUserInfo.size() < 1)
    {
        return "";
    }
    return balckUserTimeStr;
}

QString t_balckList::getLossUserTime()
{
    if(lossUserInfo.size() < 1)
    {
        return "";
    }
    return lossUserTimeStr;
}

QString t_balckList::getForbidUserTime()
{
    if(forbidUserInfo.size() < 1)
    {
        return "";
    }
    return forbidUserTimeStr;
}
