#include "t_basic_temp_storage.h"
#include <QSqlQuery>
#include <QVariant>
t_basic_temp_storage::t_basic_temp_storage()
{
}

void t_basic_temp_storage::init()
{
    QSqlQuery query;
    query.exec("create table t_basic_temp_storage(storageId varchar(32) primary key, cellId varchar(4), \
               cellType integer, cabinetId   varchar(2),mobilephone varchar(18),createtime datetime,\
               status integer,messageStatus integer,messageSentTime datetime,getCode varchar(16),\
               takeTime  datetime,hasPayMoney decimal(5,2),payway integer,saveBasePay decimal(4,2),debtPay decimal(4,2),\
               cashMoney decimal(4,2),goodsCheckStatus integer,switchCharge integer,senderPhotoPath varchar(64),getterPhotoPath varchar(64) )");
}

bool t_basic_temp_storage::insertStorage(BasicTempStorage date)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_temp_storage VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(date.storageId);
    query.addBindValue(date.cellId);
    query.addBindValue(date.cellType);
    query.addBindValue(date.cabinetId);
    query.addBindValue(date.mobilephone);
    query.addBindValue(date.createtime);
    query.addBindValue(date.status);
    query.addBindValue(date.messageStatus);
    query.addBindValue(date.messageSentTime);
    query.addBindValue(date.getCode);
    query.addBindValue(date.takeTime);
    query.addBindValue(date.hasPayMoney);
    query.addBindValue(date.payway);
    query.addBindValue(date.saveBasePay);
    query.addBindValue(date.debtPay);
    query.addBindValue(date.cashMoney);
    query.addBindValue(date.goodsCheckStatus);
    query.addBindValue(date.switchCharge);
    query.addBindValue(date.senderPhotoPath);
    query.addBindValue(date.getterPhotoPath);
    return query.exec();
}

bool t_basic_temp_storage::updateStorageInfo(BasicTempStorage storageInfo)
{
    QSqlQuery query;
    query.prepare("update t_basic_temp_storage set cellId = ?,cellType = ?,cabinetId = ?,mobilephone = ?,createtime = ?,\
                  status = ?,messageStatus = ? ,messageSentTime = ?,getCode = ?,takeTime = ?,hasPayMoney = ?,\
                  payway = ?,saveBasePay = ?,debtPay = ?,cashMoney =?,goodsCheckStatus  = ?,switchCharge = ?,\
                  senderPhotoPath = ?,getterPhotoPath = ? where storageId = ?");

    query.addBindValue(storageInfo.cellId);
    query.addBindValue(storageInfo.cellType);
    query.addBindValue(storageInfo.cabinetId);
    query.addBindValue(storageInfo.mobilephone);
    query.addBindValue(storageInfo.createtime);
    query.addBindValue(storageInfo.status);
    query.addBindValue(storageInfo.messageStatus);
    query.addBindValue(storageInfo.messageSentTime);
    query.addBindValue(storageInfo.getCode);
    query.addBindValue(storageInfo.takeTime);
    query.addBindValue(storageInfo.hasPayMoney);
    query.addBindValue(storageInfo.payway);
    query.addBindValue(storageInfo.saveBasePay);
    query.addBindValue(storageInfo.debtPay);
    query.addBindValue(storageInfo.cashMoney);
    query.addBindValue(storageInfo.goodsCheckStatus);
    query.addBindValue(storageInfo.switchCharge);
    query.addBindValue(storageInfo.senderPhotoPath);
    query.addBindValue(storageInfo.getterPhotoPath);

    query.addBindValue(storageInfo.storageId);

    return query.exec();
}


void t_basic_temp_storage::clearDeliveryInfo(BasicTempStorage &deliveryInfo)
{
    deliveryInfo.storageId.clear();
    deliveryInfo.cellId.clear();
    deliveryInfo.cellType = 0;
    deliveryInfo.cabinetId.clear();
    deliveryInfo.mobilephone.clear();
   // deliveryInfo.createtime;
    deliveryInfo.status = 0;
    deliveryInfo.messageStatus = 1;
  //  deliveryInfo.messageSentTime;
    deliveryInfo.getCode.clear();
   // deliveryInfo.takeTime;
    deliveryInfo.hasPayMoney = 0.00;
    deliveryInfo.payway = 1;
    deliveryInfo.saveBasePay = 0.00;
    deliveryInfo.debtPay = 0.00;
    deliveryInfo.cashMoney = 0.00;
    deliveryInfo.goodsCheckStatus = 3;
    deliveryInfo.switchCharge = 1;
    deliveryInfo.senderPhotoPath.clear();
    deliveryInfo.getterPhotoPath.clear();
}

bool t_basic_temp_storage::selectTakeCodeForPhone(QString phone, QString &code)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT getCode from t_basic_temp_storage where mobilephone = ? and status = 1");
    query.bindValue(0,phone);
    query.exec();

    while(query.next())
    {
        code = code + query.value(0).toString() + ",";

        ret = true;
    }

    return ret;
}



QString t_basic_temp_storage::createRandomCode()
{
    //寄件产生的随机数相加8
    QSqlQuery query;
    QString tempCode;
    qsrand(time(0));

    while(1)
    {
        int firstValue = qrand()%9;
        int twoValue = 8 - firstValue;
        int threeValue = qrand()%10;
        int fourValue = qrand()%10;
        int fiveValue = qrand()%10;
        int sixValue = qrand()%10;

        tempCode = QString::number(firstValue) + QString::number(twoValue) + QString::number(threeValue);
        tempCode = tempCode + QString::number(fourValue) + QString::number(fiveValue) + QString::number(sixValue);

        query.prepare("SELECT getCode from t_basic_temp_storage where getCode =? and status = 1");
        query.bindValue(0,tempCode);
        query.exec();
        if(!query.first())
        {
           break;
        }
    }
    return tempCode;
}

void t_basic_temp_storage::updateStorageNoteSendTimeInfo(QString uuid, QDateTime time)
{
    QSqlQuery query;
    query.prepare("update t_basic_temp_storage set messageSentTime = ? where storageId = ?");
    query.bindValue(0,time);
    query.bindValue(1,uuid);
    query.exec();
}


/****************************************************************
**
*	函数名称：	selectTakeDeliveryInfo
*	函数功能：	查找是否有这个取件码
* 	函数参数：	code 手机号 BasicTempStorage 派件信息列表
* 	返回值  :       true 有
*
****************************************************************/
bool t_basic_temp_storage::selectTakeDeliveryInfo(QString code, BasicTempStorage &storageInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_temp_storage where getCode = ? and status = 1");
    query.bindValue(0,code);
    query.exec();

    if(query.first())
    {
        storageInfo.storageId = query.value(0).toString();
        storageInfo.cellId = query.value(1).toString();
        storageInfo.cellType = query.value(2).toInt();
        storageInfo.cabinetId = query.value(3).toString();
        storageInfo.mobilephone = query.value(4).toString();
        storageInfo.createtime = query.value(5).toDateTime();
        storageInfo.status = query.value(6).toInt();
        storageInfo.messageStatus = query.value(7).toInt();
        storageInfo.messageSentTime = query.value(8).toDateTime();
        storageInfo.getCode = query.value(9).toString();
        storageInfo.takeTime = query.value(10).toDateTime();
        storageInfo.hasPayMoney = query.value(11).toDouble();
        storageInfo.payway = query.value(12).toInt();
        storageInfo.saveBasePay = query.value(13).toDouble();
        storageInfo.debtPay = query.value(14).toDouble();
        storageInfo.cashMoney = query.value(15).toDouble();
        storageInfo.goodsCheckStatus = query.value(16).toInt();
        storageInfo.switchCharge = query.value(17).toInt();
        storageInfo.senderPhotoPath = query.value(18).toString();
        storageInfo.getterPhotoPath = query.value(19).toString();

        ret = true;
    }

    return ret;

}


bool t_basic_temp_storage::selectExceptionCodeForCell(QString cell, BasicTempStorage &storageInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_temp_storage where cellId = ? and status = 1");
    query.bindValue(0,cell);
    query.exec();

    if(query.first())
    {
        storageInfo.storageId = query.value(0).toString();
        storageInfo.cellId = query.value(1).toString();
        storageInfo.cellType = query.value(2).toInt();
        storageInfo.cabinetId = query.value(3).toString();
        storageInfo.mobilephone = query.value(4).toString();
        storageInfo.createtime = query.value(5).toDateTime();
        storageInfo.status = query.value(6).toInt();
        storageInfo.messageStatus = query.value(7).toInt();
        storageInfo.messageSentTime = query.value(8).toDateTime();
        storageInfo.getCode = query.value(9).toString();
        storageInfo.takeTime = query.value(10).toDateTime();
        storageInfo.hasPayMoney = query.value(11).toDouble();
        storageInfo.payway = query.value(12).toInt();
        storageInfo.saveBasePay = query.value(13).toDouble();
        storageInfo.debtPay = query.value(14).toDouble();
        storageInfo.cashMoney = query.value(15).toDouble();
        storageInfo.goodsCheckStatus = query.value(16).toInt();
        storageInfo.switchCharge = query.value(17).toInt();
        storageInfo.senderPhotoPath = query.value(18).toString();
        storageInfo.getterPhotoPath = query.value(19).toString();

        ret = true;
    }

    return ret;
}


qint8 t_basic_temp_storage::clearTariffInfo(QString uuid)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status from t_basic_temp_storage where storageId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {
        ret = 2;
        qint8 status = query.value(0).toInt();
        if(status == 1)
        {
            ret = 1;
            query.prepare("update t_basic_temp_storage set switchCharge = 2 where storageId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }
    }

    return ret;
}

qint8 t_basic_temp_storage::closeDeliveryInfo(QString uuid, QString &cell)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status,cellId from t_basic_temp_storage where storageId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {       
        ret = 2;
        qint8 status = query.value(0).toInt();
        if(1 == status)
        {
            ret = 1;
            cell = query.value(1).toString();

            query.prepare("update t_basic_temp_storage set status = 2 where storageId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }
    }

    return ret;
}
