#include "t_income_detail.h"
#include <QSqlQuery>
#include <QVariant>
t_income_detail::t_income_detail()
{
}

void t_income_detail::init()
{
   QSqlQuery query;
    query.exec("create table t_income_detail(incomeId integer primary key,incomeTime datetime, incomeType integer, incomeWay integer,incomeObject integer,incomePrice  decimal(5,2))");
}
bool t_income_detail::insertNote(TIncomeDetail date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_income_detail VALUES(null,?,?,?,?,?)");
    query.addBindValue(date.incomeTime);
    query.addBindValue(date.incomeType);
    query.addBindValue(date.incomeWay);
    query.addBindValue(date.incomeObject);
    query.addBindValue(date.incomePrice);

    return query.exec();
}
