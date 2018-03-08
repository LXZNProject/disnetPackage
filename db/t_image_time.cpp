#include "t_image_time.h"
#include <QSqlQuery>
#include <QVariant>

CImageTime::CImageTime()
{
}

void CImageTime::init()
{
   bool ret = false;
   QSqlQuery query;
   ret = query.exec("create table t_image_time(homePageTime varchar(32),advertTime varchar(32))");

   if(ret)
   {
       query.exec("INSERT INTO t_image_time(homePageTime,advertTime) VALUES ('0','0')");
   }

   getImageTimeInfo();

}

void CImageTime::getImageTimeInfo()
{
    QSqlQuery query;
    query.exec("select * from t_image_time");

    query.first();

    mHomeTime    = query.value(0).toString();
    mAdvertTime  = query.value(1).toString();
}

void CImageTime::updateImageTimeInfo()
{
    QSqlQuery query;
    query.prepare("update t_image_time set homePageTime = ?,advertTime = ?");

    query.bindValue(0,mHomeTime);
    query.bindValue(1,mAdvertTime);

    query.exec();
}
