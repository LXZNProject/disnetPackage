#include "t_log_smslimit.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
CNoteLimit::CNoteLimit()
{
}

void CNoteLimit::init()
{
    QSqlQuery query;
    query.exec("create table [t_log_smslimit](logId integer primary key,mobilePhone  varchar(18),sendDate date, messNum integer)");

}

bool CNoteLimit::insertNote(QString phone, QDate date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_log_smslimit VALUES(null,?,?,1)");
    query.addBindValue(phone);
    query.addBindValue(date);

    return query.exec();
}

bool CNoteLimit::getNoteInfo(QString phone, QDate &date, int &number)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("select sendDate,messNum from t_log_smslimit where mobilePhone = ? ");
    query.addBindValue(phone);
    query.exec();
    while(query.next())
    {
        date = query.value(0).toDate();
        number = query.value(1).toInt();
        ret = true;
    }

    return ret;
}

void CNoteLimit::deleteOverTimeNoteInfo()
{
    QDate time = QDate::currentDate().addDays(-7);
    QSqlQuery query;
    query.prepare("DELETE  FROM t_log_smslimit WHERE sendDate < ?");
    query.bindValue(0,time);
    query.exec();
}

bool CNoteLimit::checkNoteSendInfo(QString phone, int limitNum,int &curNum)
{
    bool ret = true;
    QDate curDate = QDate::currentDate();
    int number = 1;
    QSqlQuery query;
    query.prepare("select messNum from t_log_smslimit where mobilePhone = ? and sendDate = ?");
    query.addBindValue(phone);
    query.addBindValue(curDate);
    query.exec();
    while(query.next())
    {
        number = query.value(0).toInt();
        ret = false;
    }

    curNum = number;

    if(ret)
    {
        query.prepare("INSERT INTO t_log_smslimit VALUES(null,?,?,1)");
        query.addBindValue(phone);
        query.addBindValue(curDate);
        query.exec();
    }
    else
    {
        if(number < limitNum)
        {
            query.prepare("update t_log_smslimit set messNum = ? where mobilePhone = ? and sendDate = ?");
            query.addBindValue(++number);
            query.addBindValue(phone);
            query.addBindValue(curDate);
            query.exec();

            ret = true;
        }
    }

    return ret;
}


