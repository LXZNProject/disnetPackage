#include "t_basic_stored_record.h"
#include <QSqlQuery>
#include <QVariant>
t_basic_stored_record::t_basic_stored_record()
{
}

void t_basic_stored_record::init()
{
    QSqlQuery query;
    query.exec("create table t_basic_stored_record( \
               recordId int primary key, price varchar(5), recordtime datetime, storeCardNo  varchar(18),\
               relM1CardId  varchar(18),relPhone  varchar(18),optM1CardId  varchar(18),optPhone  varchar(18))");
}

bool t_basic_stored_record::insertNote(BasicStoredRecord date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_stored_record VALUES(null,?,?,?,?,?,?,?)");
    query.addBindValue(date.price);
    query.addBindValue(date.recordtime);
    query.addBindValue(date.storeCardNo);
    query.addBindValue(date.relM1CardId);
    query.addBindValue(date.relPhone);
    query.addBindValue(date.optM1CardId);
    query.addBindValue(date.optPhone);
    return query.exec();
}

void t_basic_stored_record::getRecordHistory(QString phoneNum, QList<BasicStoredRecord> &historyList)
{
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_stored_record where relPhone = ?");
    query.bindValue(0,phoneNum);
    query.exec();

    while(query.next())
    {
        BasicStoredRecord tempvalue;
        tempvalue.price = query.value(1).toString();
        tempvalue.recordtime = query.value(2).toDateTime();
        tempvalue.storeCardNo = query.value(3).toString();
        tempvalue.relM1CardId = query.value(4).toString();
        tempvalue.relPhone = query.value(5).toString();
        tempvalue.optM1CardId = query.value(6).toString();
        tempvalue.optPhone = query.value(7).toString();
        historyList.append(tempvalue);
    }
}


