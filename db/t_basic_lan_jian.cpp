#include "t_basic_lan_jian.h"

#include <QVariant>
#include <QSqlQuery>

CSender::CSender()
{
}

void CSender::init()
{
    QSqlQuery query;
    query.exec("create table [t_basic_lan_jian](\
               lanJianId varchar(32) primary key,deliveryType integer,cellId varchar(4),cellType integer, cabinetId varchar(2),userMobilephone varchar(18), \
               userSelectExpCompanyName varchar(64),lanJianName varchar(32),lanJianPhone varchar(18),createtime datetime,\
               status integer,messageStatus integer,messageSentTime datetime,getCode varchar(16),takeTime datetime,hasPayMoney decimal(5,2),\
               ownerBasePay decimal(4,2),expressPay decimal(4,2),payway integer,cashMoney decimal(4,2),\
               goodsCheckStatus  integer,switchCharge integer,senderPhotoPath varchar(64),getterPhotoPath varchar(64))");
}

bool CSender::insertSenderInfo(SenderDesc_t senderInfo)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_lan_jian VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(senderInfo.lanJianId);
    query.addBindValue(senderInfo.deliveryType);
    query.addBindValue(senderInfo.cellId);
    query.addBindValue(senderInfo.cellType);
    query.addBindValue(senderInfo.cabinetId);
    query.addBindValue(senderInfo.mobilephone);
    query.addBindValue(senderInfo.companyName);
    query.addBindValue(senderInfo.lanJianName);
    query.addBindValue(senderInfo.lanJianPhone);
    query.addBindValue(senderInfo.createtime);
    query.addBindValue(senderInfo.status);
    query.addBindValue(senderInfo.messageStatus);
    query.addBindValue(senderInfo.messageSentTime);
    query.addBindValue(senderInfo.getCode);
    query.addBindValue(senderInfo.takeTime);
    query.addBindValue(senderInfo.hasPayMoney);
    query.addBindValue(senderInfo.ownerBasePay);
    query.addBindValue(senderInfo.expressPay);
    query.addBindValue(senderInfo.payway);
    query.addBindValue(senderInfo.cashMoney);
    query.addBindValue(senderInfo.goodsCheckStatus);
    query.addBindValue(senderInfo.switchCharge);
    query.addBindValue(senderInfo.senderPhotoPath);
    query.addBindValue(senderInfo.getterPhotoPath);


    return query.exec();
}

bool CSender::updateSenderInfo(SenderDesc_t senderInfo)
{
    QSqlQuery query;
    query.prepare("update t_basic_lan_jian set deliveryType = ?,cellId = ?,cellType = ?,cabinetId = ?,userMobilephone = ?,userSelectExpCompanyName = ?,\
                  lanJianName = ?,lanJianPhone = ? ,createtime = ?,status =?,messageStatus = ?,\
                  messageSentTime = ?,getCode = ?,takeTime = ?,hasPayMoney = ?,ownerBasePay = ?,expressPay = ?,\
                  payway = ?,cashMoney =?,goodsCheckStatus  = ?,switchCharge = ?,\
                  senderPhotoPath = ?,getterPhotoPath = ? where lanJianId = ?");

    query.addBindValue(senderInfo.deliveryType);
    query.addBindValue(senderInfo.cellId);
    query.addBindValue(senderInfo.cellType);
    query.addBindValue(senderInfo.cabinetId);
    query.addBindValue(senderInfo.mobilephone);
    query.addBindValue(senderInfo.companyName);
    query.addBindValue(senderInfo.lanJianName);
    query.addBindValue(senderInfo.lanJianPhone);
    query.addBindValue(senderInfo.createtime);
    query.addBindValue(senderInfo.status);
    query.addBindValue(senderInfo.messageStatus);
    query.addBindValue(senderInfo.messageSentTime);
    query.addBindValue(senderInfo.getCode);
    query.addBindValue(senderInfo.takeTime);
    query.addBindValue(senderInfo.hasPayMoney);
    query.addBindValue(senderInfo.ownerBasePay);
    query.addBindValue(senderInfo.expressPay);
    query.addBindValue(senderInfo.payway);
    query.addBindValue(senderInfo.cashMoney);
    query.addBindValue(senderInfo.goodsCheckStatus);
    query.addBindValue(senderInfo.switchCharge);
    query.addBindValue(senderInfo.senderPhotoPath);
    query.addBindValue(senderInfo.getterPhotoPath);

    query.addBindValue(senderInfo.lanJianId);

    return query.exec();

}

void CSender::clearDeliveryInfo(SenderDesc_t &senderInfo)
{
    senderInfo.lanJianId.clear();
    senderInfo.deliveryType = 0;
    senderInfo.cellId.clear();
    senderInfo.cellType = 0;
    senderInfo.cabinetId.clear();
    senderInfo.mobilephone.clear();
    senderInfo.companyName.clear();
    senderInfo.lanJianName.clear();
    senderInfo.lanJianPhone.clear();
   // deliveryInfo.createtime.;
    senderInfo.status = 0;
    senderInfo.messageStatus = 1;
  //  deliveryInfo.messageSentTime = query.value(12).toDateTime();
    senderInfo.getCode.clear();
   // deliveryInfo.takeTime = query.value(14).toDateTime();
    senderInfo.hasPayMoney = 0.00;

    senderInfo.ownerBasePay = 0.00;
    senderInfo.expressPay = 0.00;
    senderInfo.payway = 1;
    senderInfo.cashMoney = 0.00;
    senderInfo.goodsCheckStatus = 3;
    senderInfo.switchCharge = 1;
    senderInfo.senderPhotoPath.clear();
    senderInfo.getterPhotoPath.clear();
}

bool CSender::selectTakeCodeForPhone(QString phone, QString &code)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT getCode from t_basic_lan_jian where lanJianPhone = ? and status = 1");
    query.bindValue(0,phone);
    query.exec();

    while(query.next())
    {
        code = code + query.value(0).toString() + ",";

        ret = true;
    }

    return ret;
}


QString CSender::createRandomCode()
{
    //寄件产生的随机数相加10
    QSqlQuery query;
    QString tempCode;
    qsrand(time(0));

    while(1)
    {
        int firstValue = qrand()%8;
        int twoValue = 7 - firstValue;
        int threeValue = qrand()%10;
        int fourValue = qrand()%10;
        int fiveValue = qrand()%10;
        int sixValue = qrand()%10;

        tempCode = QString::number(firstValue) + QString::number(twoValue) + QString::number(threeValue);
        tempCode = tempCode + QString::number(fourValue) + QString::number(fiveValue) + QString::number(sixValue);

        query.prepare("SELECT getCode from t_basic_lan_jian where getCode = ? and status = 1");
        query.bindValue(0,tempCode);
        query.exec();
        if(!query.first())
        {
           break;
        }
    }
    return tempCode;
}

void CSender::updateSenderNoteSendTimeInfo(QString uuid, QDateTime time)
{
    QSqlQuery query;
    query.prepare("update t_basic_lan_jian set messageSentTime = ? where lanJianId = ?");
    query.bindValue(0,time);
    query.bindValue(1,uuid);
    query.exec();
}


/****************************************************************
**
*	函数名称：	selectTakeDeliveryInfo
*	函数功能：	查找是否有这个取件码
* 	函数参数：	code 手机号 DelivertDesc_t 派件信息列表
* 	返回值  :       true 有
*
****************************************************************/
bool CSender::selectTakeDeliveryInfo(QString code, SenderDesc_t &senderInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_lan_jian where getCode = ? and status = 1");
    query.bindValue(0,code);
    query.exec();

    if(query.first())
    {
        senderInfo.lanJianId = query.value(0).toString();
        senderInfo.deliveryType = query.value(1).toInt();
        senderInfo.cellId = query.value(2).toString();
        senderInfo.cellType = query.value(3).toInt();
        senderInfo.cabinetId = query.value(4).toString();
        senderInfo.mobilephone = query.value(5).toString();
        senderInfo.companyName = query.value(6).toString();
        senderInfo.lanJianName = query.value(7).toString();
        senderInfo.lanJianPhone = query.value(8).toString();
        senderInfo.createtime = query.value(9).toDateTime();
        senderInfo.status = query.value(10).toInt();
        senderInfo.messageStatus = query.value(11).toInt();
        senderInfo.messageSentTime = query.value(12).toDateTime();
        senderInfo.getCode = query.value(13).toString();
        senderInfo.takeTime = query.value(14).toDateTime();
        senderInfo.hasPayMoney = query.value(15).toDouble();
        senderInfo.ownerBasePay = query.value(16).toDouble();
        senderInfo.expressPay = query.value(17).toDouble();
        senderInfo.payway = query.value(18).toInt();
        senderInfo.cashMoney = query.value(19).toDouble();
        senderInfo.goodsCheckStatus = query.value(20).toInt();
        senderInfo.switchCharge = query.value(21).toInt();
        senderInfo.senderPhotoPath = query.value(22).toString();
        senderInfo.getterPhotoPath = query.value(23).toString();

        ret = true;
    }

    return ret;

}

bool CSender::selectExceptionCodeForCell(QString cell, SenderDesc_t &senderInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_lan_jian where cellId = ? and status = 1");
    query.bindValue(0,cell);
    query.exec();

    if(query.first())
    {
        senderInfo.lanJianId = query.value(0).toString();
        senderInfo.deliveryType = query.value(1).toInt();
        senderInfo.cellId = query.value(2).toString();
        senderInfo.cellType = query.value(3).toInt();
        senderInfo.cabinetId = query.value(4).toString();
        senderInfo.mobilephone = query.value(5).toString();
        senderInfo.companyName = query.value(6).toString();
        senderInfo.lanJianName = query.value(7).toString();
        senderInfo.lanJianPhone = query.value(8).toString();
        senderInfo.createtime = query.value(9).toDateTime();
        senderInfo.status = query.value(10).toInt();
        senderInfo.messageStatus = query.value(11).toInt();
        senderInfo.messageSentTime = query.value(12).toDateTime();
        senderInfo.getCode = query.value(13).toString();
        senderInfo.takeTime = query.value(14).toDateTime();
        senderInfo.hasPayMoney = query.value(15).toDouble();
        senderInfo.ownerBasePay = query.value(16).toDouble();
        senderInfo.expressPay = query.value(17).toDouble();
        senderInfo.payway = query.value(18).toInt();
        senderInfo.cashMoney = query.value(19).toDouble();
        senderInfo.goodsCheckStatus = query.value(20).toInt();
        senderInfo.switchCharge = query.value(21).toInt();
        senderInfo.senderPhotoPath = query.value(22).toString();
        senderInfo.getterPhotoPath = query.value(23).toString();

        ret = true;
    }

    return ret;
}


qint8 CSender::clearTariffInfo(QString uuid)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status from t_basic_lan_jian where lanJianId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {
        ret = 2;
        qint8 status = query.value(0).toInt();
        if(1 == status)
        {
            ret = 1;
            query.prepare("update t_basic_lan_jian set switchCharge = 2 ,expressPay = 0.00 where lanJianId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }
    }

    return ret;
}

qint8 CSender::closeDeliveryInfo(QString uuid, QString &cell)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status,cellId from t_basic_lan_jian where lanJianId = ?");
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

            query.prepare("update t_basic_lan_jian set status = 2 where lanJianId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }
    }

    return ret;
}
