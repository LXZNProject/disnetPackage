#include "t_basic_cell_order.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QStringList>

t_basic_cell_order::t_basic_cell_order()
{
}

void t_basic_cell_order::init()
{
    QSqlQuery query;
    query.exec("create table [t_basic_cell_order](orderType integer,orderValue  varchar(32),cellId varchar(4), cellType integer,\
               createTime datetime,expireTime datatime)");

}

void t_basic_cell_order::insertOrderInfo(QString time, QList<expOrdersCellList *> expList, QList<userOrdersCellList *> userList)
{

    orderCellTime = time;

    QSqlQuery query;
    query.exec("DELETE FROM t_basic_cell_order");
    for(int i = 0;i < expList.size();i++)
    {
        query.prepare("INSERT INTO t_basic_cell_order VALUES(1,?,?,?,?,?)");
        query.addBindValue(expList.at(i)->m1CardId());
        query.addBindValue(expList.at(i)->cellId());
        query.addBindValue(expList.at(i)->cellType());
        query.addBindValue(QDateTime::fromString(orderCellTime,Qt::ISODate));
        query.addBindValue(QDateTime::fromString(expList.at(i)->expireTime(),Qt::ISODate));
        query.exec();
    }

    for(int i = 0;i < userList.size();i++)
    {
        query.prepare("INSERT INTO t_basic_cell_order VALUES(2,?,?,?,?,?)");
        query.addBindValue(userList.at(i)->mobilePhone());
        query.addBindValue(userList.at(i)->cellId());
        query.addBindValue(userList.at(i)->cellType());
        query.addBindValue(QDateTime::fromString(orderCellTime,Qt::ISODate));
        query.addBindValue(QDateTime::fromString(userList.at(i)->expireTime(),Qt::ISODate));
        query.exec();
    }
}



bool t_basic_cell_order::isExpOrderInfo(QString m1Card)
{
    QDateTime time = QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_cell_order where orderValue = ? and orderType = 1 and expireTime > ? ");
    query.bindValue(0,m1Card);
    query.bindValue(1,time);
    query.exec();

    if(query.first())
    {
        return true;
    }
    return false;
}

void t_basic_cell_order::getExpOrderInfo(QString m1Card,QStringList &big, QStringList &middle, QStringList &small, QStringList &order)
{
    QDateTime time = QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("SELECT o.cellId,c.cellType from t_basic_cell_order o left join t_basic_cell c on o.cellId = c.cellId where o.orderValue = ? and o.orderType = 1 and o.expireTime > ? ");
    query.bindValue(0,m1Card);
    query.bindValue(1,time);
    query.exec();

    while(query.next())
    {
        int type = query.value(1).toInt();
        QString cellId = query.value(0).toString();

        if(1 == type)
        {
            if(!big.contains(cellId))
            {
                big.append(cellId);
            }
        }
        else if(2 == type)
        {
            if(!middle.contains(cellId))
            {
                middle.append(cellId);
            }
        }
        else if(3 == type)
        {
            if(!small.contains(cellId))
            {
                small.append(cellId);
            }
        }
    }


    query.prepare("SELECT o.cellId from t_basic_cell_order o left join t_basic_cell c on o.cellId = c.cellId where o.orderType = 1 and o.expireTime > ? ");
    query.bindValue(0,time);
    query.exec();

    while(query.next())
    {
        QString cellId = query.value(0).toString();

        if(!order.contains(cellId))
        {
            order.append(cellId);
        }
    }

    big.sort();
    middle.sort();
    small.sort();
    order.sort();

}

void t_basic_cell_order::getUserOrderInfo(QList<userOrder> &cellInfos)
{
    QDateTime time = QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("SELECT o.orderValue,o.cellId,c.cellType from t_basic_cell_order o left join t_basic_cell c on o.cellId = c.cellId where o.orderType = 2 and o.expireTime > ? order by o.cellId asc ");
    query.bindValue(0,time);
    query.exec();

    while(query.next())
    {
        userOrder cellInfo;
        cellInfo.phone = query.value(0).toString();
        cellInfo.cellId = query.value(1).toString();
        cellInfo.type = query.value(2).toInt();

        cellInfos.append(cellInfo);
    }

}
