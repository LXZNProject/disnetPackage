#include "t_income_total.h"
#include <QSqlQuery>
#include <QVariant>
t_income_total::t_income_total()
{
}

void t_income_total::init()
{
    QSqlQuery query;
     query.exec("create table t_income_detail(allCoin decimal(10,2),allIncome decimal(10,2), activeTime datetime)");
}

bool t_income_total::updataNote(TIncomeTotal date)
{
    QSqlQuery query;
    query.prepare("update t_income_detail set allCoin = ?,allIncome = ?,activeTime = ?");
    query.addBindValue(date.allCoin);
    query.addBindValue(date.allIncome);
    query.addBindValue(date.activeTime);
    return query.exec();
}

bool t_income_total::getdataNote(TIncomeTotal date)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("select * from t_income_detail");
    query.exec();
    while(query.next())
    {
        date.allCoin = query.value(0).toDouble();
        date.allIncome = query.value(1).toDouble();
        date.activeTime = query.value(2).toDateTime();
        ret = true;
    }
    return ret;
}
