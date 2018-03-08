#include "t_temp_consume_record.h"


#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>

t_temp_consume_record::t_temp_consume_record()
{
}

void t_temp_consume_record::init()
{
    QSqlQuery query;
    query.exec("create table [t_temp_consume_record](recordId integer primary key,m1Card varchar(12),companyId varchar(16), createTime datetime,curCash decimal(4,2),usrUuid varchar(32),cmmUuid varchar(32))");

    QSqlQuery q("select * from  t_temp_consume_record");
    QSqlRecord rec = q.record();
    if(!rec.contains("usrUuid"))
    {
        query.exec("alter table t_temp_consume_record add column usrUuid varchar(32)");
        query.exec("alter table t_temp_consume_record add column cmmUuid varchar(32)");
    }
}

void t_temp_consume_record::insertTempRecord(TempRecordDesc_t tempRecordInfo)
{
    QSqlQuery query;
    query.exec("SELECT curCash FROM t_temp_consume_record where usrUuid = ?");
    query.bindValue(0,tempRecordInfo.usrUuid);
    query.exec();

    if(query.first())
    {
        query.exec("update t_temp_consume_record set companyId = ?,createTime = ?,curCash = ?,m1Card = ?,cmmUuid = ? where usrUuid = ? ");
        query.bindValue(0,tempRecordInfo.companyId);
        query.bindValue(1,tempRecordInfo.createTime);
        query.bindValue(2,tempRecordInfo.curCash);
        query.bindValue(3,tempRecordInfo.m1Card);
        query.bindValue(4,tempRecordInfo.cmmUuid);
        query.bindValue(5,tempRecordInfo.usrUuid);
        query.exec();
    }
    else
    {
        query.exec("INSERT INTO t_temp_consume_record VALUES(null,?,?,?,?,?,?)");
        query.addBindValue(tempRecordInfo.m1Card);
        query.addBindValue(tempRecordInfo.companyId);
        query.addBindValue(tempRecordInfo.createTime);
        query.addBindValue(tempRecordInfo.curCash);
        query.addBindValue(tempRecordInfo.usrUuid);
        query.addBindValue(tempRecordInfo.cmmUuid);
        query.exec();
    }

}

bool t_temp_consume_record::queryTempRecord(QString usrUuid,double &cash,QString &cmmUuid)
{
    bool ret = false;
    QSqlQuery query;
    query.exec("SELECT curCash,cmmUuid FROM t_temp_consume_record where usrUuid = ?");
    query.bindValue(0,usrUuid);
    query.exec();

    if(query.first())
    {
        ret = true;
        cash = query.value(0).toDouble();
        cmmUuid = query.value(1).toString();
    }

    return ret;
}

void t_temp_consume_record::clearTempRecord(QString usrUuid)
{
    QSqlQuery query;
    query.exec("update t_temp_consume_record set curCash = 0 where usrUuid = ?");
    query.bindValue(0,usrUuid);
    query.exec();
}

void t_temp_consume_record::deleteTempRecordInfo()
{
    QDateTime time = QDateTime::fromString("2015-08-19 00:00:00",Qt::ISODate);
    QSqlQuery query;
    query.prepare("DELETE  FROM t_temp_consume_record WHERE createTime < ?");
    query.bindValue(0,time);
    query.exec();
}
