#include "t_basic_delivery.h"
#include "t_basic_terminal.h"

#include <QVariant>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QDebug>
CDelivery::CDelivery()
{
}

void CDelivery::init()
{
    QSqlQuery query;
    query.exec("create table [t_basic_delivery](\
               deliveryId varchar(32) primary key,cellId varchar(4),cellType integer, cabinetId varchar(2),m1Card varchar(12),mobilephone varchar(18), \
               packageId varchar(32),expName varchar(32),expMobilePhone varchar(18),expCompanyName varchar(32),createtime datetime,\
               status integer,messageStatus integer,messageSentTime datetime,getCode varchar(16),takeTime datetime,hasPayMoney decimal(5,2),\
               expressPay decimal(4,2),ownerBasePay decimal(4,2),tariffType integer,expireStatus integer,payway integer,debtPay decimal(4,2),\
               cashMoney decimal(4,2),goodsCheckStatus  integer,switchCharge integer,senderPhotoPath varchar(64),getterPhotoPath varchar(64),\
               expireHours integer,expireStartTariff decimal(4,2),expireStep integer,expireStepMoney decimal(4,2))");

    QSqlQuery q("select * from  t_basic_delivery");
    QSqlRecord rec = q.record();
    if(!rec.contains("expireHours"))
    {
        query.exec("alter table t_basic_delivery add column expireHours integer");
        query.exec("alter table t_basic_delivery add column expireStartTariff decimal(4,2)");
        query.exec("alter table t_basic_delivery add column expireStep integer");
        query.exec("alter table t_basic_delivery add column expireStepMoney decimal(4,2)");

        query.prepare("update t_basic_delivery set expireHours = ?,expireStartTariff = ?,expireStep = ?,expireStepMoney = ?");
        query.bindValue(0,SCTerminal::instance()->terminalInfo.expireHours);
        query.bindValue(1,SCTerminal::instance()->terminalInfo.expireStartTariff);
        query.bindValue(2,SCTerminal::instance()->terminalInfo.expireStep);
        query.bindValue(3,SCTerminal::instance()->terminalInfo.expireStepMoney);
        query.exec();
    }
}

bool CDelivery::insertDeliveryInfo(DelivertDesc_t deliveryInfo)
{
    QSqlQuery query;
    query.prepare("INSERT INTO t_basic_delivery VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    query.addBindValue(deliveryInfo.deliveryId);
    query.addBindValue(deliveryInfo.cellId);
    query.addBindValue(deliveryInfo.cellType);
    query.addBindValue(deliveryInfo.cabinetId);
    query.addBindValue(deliveryInfo.m1card);
    query.addBindValue(deliveryInfo.mobilephone);
    query.addBindValue(deliveryInfo.packageId);
    query.addBindValue(deliveryInfo.expName);
    query.addBindValue(deliveryInfo.expMobilePhone);
    query.addBindValue(deliveryInfo.expCompanyName);
    query.addBindValue(deliveryInfo.createtime);
    query.addBindValue(deliveryInfo.status);
    query.addBindValue(deliveryInfo.messageStatus);
    query.addBindValue(deliveryInfo.messageSentTime);
    query.addBindValue(deliveryInfo.getCode);
    query.addBindValue(deliveryInfo.takeTime);
    query.addBindValue(deliveryInfo.hasPayMoney);
    query.addBindValue(deliveryInfo.expressPay);
    query.addBindValue(deliveryInfo.ownerBasePay);
    query.addBindValue(deliveryInfo.tariffType);
    query.addBindValue(deliveryInfo.expireStatus);
    query.addBindValue(deliveryInfo.payway);
    query.addBindValue(deliveryInfo.debtPay);
    query.addBindValue(deliveryInfo.cashMoney);
    query.addBindValue(deliveryInfo.goodsCheckStatus);
    query.addBindValue(deliveryInfo.switchCharge);
    query.addBindValue(deliveryInfo.senderPhotoPath);
    query.addBindValue(deliveryInfo.getterPhotoPath);
    query.addBindValue(deliveryInfo.expireHours);
    query.addBindValue(deliveryInfo.expireStartTariff);
    query.addBindValue(deliveryInfo.expireStep);
    query.addBindValue(deliveryInfo.expireStepMoney);


    return query.exec();
}

bool CDelivery::updateDeliveryInfo(DelivertDesc_t deliveryInfo)
{
    QSqlQuery query;
    query.prepare("update t_basic_delivery set cellId = ?,cellType = ?,cabinetId = ?,m1Card = ?,mobilephone = ?,packageId = ?,\
                  expName = ?,expMobilePhone = ?,expCompanyName = ?,createtime = ?,status =?,messageStatus = ?,\
                  messageSentTime = ?,getCode = ?,takeTime = ?,hasPayMoney = ?,expressPay = ?,ownerBasePay = ?,\
                  tariffType = ?,expireStatus = ?,payway = ?,debtPay = ?,cashMoney =?,\
                  goodsCheckStatus  = ?,switchCharge = ?,senderPhotoPath = ?,getterPhotoPath = ?,expireHours = ?,\
                  expireStartTariff = ?,expireStep = ?,expireStepMoney = ? where deliveryId = ?");

    query.addBindValue(deliveryInfo.cellId);
    query.addBindValue(deliveryInfo.cellType);
    query.addBindValue(deliveryInfo.cabinetId);
    query.addBindValue(deliveryInfo.m1card);
    query.addBindValue(deliveryInfo.mobilephone);
    query.addBindValue(deliveryInfo.packageId);
    query.addBindValue(deliveryInfo.expName);
    query.addBindValue(deliveryInfo.expMobilePhone);
    query.addBindValue(deliveryInfo.expCompanyName);
    query.addBindValue(deliveryInfo.createtime);
    query.addBindValue(deliveryInfo.status);
    query.addBindValue(deliveryInfo.messageStatus);
    query.addBindValue(deliveryInfo.messageSentTime);
    query.addBindValue(deliveryInfo.getCode);
    query.addBindValue(deliveryInfo.takeTime);
    query.addBindValue(deliveryInfo.hasPayMoney);
    query.addBindValue(deliveryInfo.expressPay);
    query.addBindValue(deliveryInfo.ownerBasePay);
    query.addBindValue(deliveryInfo.tariffType);
    query.addBindValue(deliveryInfo.expireStatus);
    query.addBindValue(deliveryInfo.payway);
    query.addBindValue(deliveryInfo.debtPay);
    query.addBindValue(deliveryInfo.cashMoney);
    query.addBindValue(deliveryInfo.goodsCheckStatus);
    query.addBindValue(deliveryInfo.switchCharge);
    query.addBindValue(deliveryInfo.senderPhotoPath);
    query.addBindValue(deliveryInfo.getterPhotoPath);
    query.addBindValue(deliveryInfo.expireHours);
    query.addBindValue(deliveryInfo.expireStartTariff);
    query.addBindValue(deliveryInfo.expireStep);
    query.addBindValue(deliveryInfo.expireStepMoney);

    query.addBindValue(deliveryInfo.deliveryId);

    return query.exec();

}

void CDelivery::deleteOverTimeDeliveryInfo()
{
    QDateTime time = QDateTime::currentDateTime().addMonths(-1);
    QSqlQuery query;
    query.prepare("DELETE  FROM t_basic_delivery WHERE takeTime < ?");
    query.bindValue(0,time);
    query.exec();
}



/****************************************************************
**
*	函数名称：	selectSaveDeliveryInfo
*	函数功能：	查找快递员存件的信息是否有滞留或待确认 第一个是待确认
* 	函数参数：	phone 手机号 QList<DelivertDesc_t> 派件信息列表
* 	返回值  :       0 无待确认和滞留 1 待确认 2 滞留 3 混合
*
****************************************************************/
qint8 CDelivery::selectSaveDeliveryInfo(QString m1Card, QList<DelivertDesc_t> &deliveryInfo)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_delivery where m1Card = ? and status = 3");
    query.bindValue(0,m1Card);
    query.exec();

    if(query.next())
    {
        ret = 1;
        DelivertDesc_t delivery;

        delivery.deliveryId = query.value(0).toString();
        delivery.cellId = query.value(1).toString();
        delivery.cellType = query.value(2).toInt();
        delivery.cabinetId = query.value(3).toString();
        delivery.m1card = query.value(4).toString();
        delivery.mobilephone = query.value(5).toString();
        delivery.packageId = query.value(6).toString();
        delivery.expName = query.value(7).toString();
        delivery.expMobilePhone = query.value(8).toString();
        delivery.expCompanyName = query.value(9).toString();
        delivery.createtime = query.value(10).toDateTime();
        delivery.status = query.value(11).toInt();
        delivery.messageStatus = query.value(12).toInt();
        delivery.messageSentTime = query.value(13).toDateTime();
        delivery.getCode = query.value(14).toString();
        delivery.takeTime = query.value(15).toDateTime();
        delivery.hasPayMoney = query.value(16).toDouble();
        delivery.expressPay = query.value(17).toDouble();
        delivery.ownerBasePay = query.value(18).toDouble();
        delivery.tariffType = query.value(19).toInt();
        delivery.expireStatus = query.value(20).toInt();
        delivery.payway = query.value(21).toInt();
        delivery.debtPay = query.value(22).toDouble();
        delivery.cashMoney = query.value(23).toDouble();
        delivery.goodsCheckStatus = query.value(24).toInt();
        delivery.switchCharge = query.value(25).toInt();
        delivery.senderPhotoPath = query.value(26).toString();
        delivery.getterPhotoPath = query.value(27).toString();
        delivery.expireHours = query.value(28).toInt();
        delivery.expireStartTariff = query.value(29).toDouble();
        delivery.expireStep = query.value(30).toInt();
        delivery.expireStepMoney = query.value(31).toDouble();


        deliveryInfo.append(delivery);
    }

    QDateTime time = QDateTime::currentDateTime().addSecs(SCTerminal::instance()->terminalInfo.forceExpTakeHours * 3600 *(-1));

    query.prepare("SELECT d.deliveryId,d.cellId,d.cellType,d.cabinetId,d.m1Card,d.mobilephone,d.packageId,d.expName,d.expMobilePhone,d.expCompanyName,d.createtime,d.status,d.messageStatus,d.messageSentTime,d.getCode,d.takeTime,\
                  d.hasPayMoney,d.expressPay,d.ownerBasePay,d.tariffType,d.expireStatus,d.payway,d.debtPay,d.cashMoney,d.goodsCheckStatus,d.switchCharge,d.senderPhotoPath,d.getterPhotoPath,d.expireHours,d.expireStartTariff,d.expireStep,d.expireStepMoney\
                  from t_basic_delivery d left join t_basic_cell c on d.cellId = c.cellId where d.m1Card = ? and d.status = 1 and d.createtime < ? and c.status = 1");
    query.bindValue(0,m1Card);
    query.bindValue(1,time);
    query.exec();

    while(query.next())
    {
        if(ret == 0)
        {
            ret = 2;
        }
        else if(ret == 1)
        {
            ret = 3;
        }
        DelivertDesc_t delivery;

        delivery.deliveryId = query.value(0).toString();
        delivery.cellId = query.value(1).toString();
        delivery.cellType = query.value(2).toInt();
        delivery.cabinetId = query.value(3).toString();
        delivery.m1card = query.value(4).toString();
        delivery.mobilephone = query.value(5).toString();
        delivery.packageId = query.value(6).toString();
        delivery.expName = query.value(7).toString();
        delivery.expMobilePhone = query.value(8).toString();
        delivery.expCompanyName = query.value(9).toString();
        delivery.createtime = query.value(10).toDateTime();
        delivery.status = query.value(11).toInt();
        delivery.messageStatus = query.value(12).toInt();
        delivery.messageSentTime = query.value(13).toDateTime();
        delivery.getCode = query.value(14).toString();
        delivery.takeTime = query.value(15).toDateTime();
        delivery.hasPayMoney = query.value(16).toDouble();
        delivery.expressPay = query.value(17).toDouble();
        delivery.ownerBasePay = query.value(18).toDouble();
        delivery.tariffType = query.value(19).toInt();
        delivery.expireStatus = query.value(20).toInt();
        delivery.payway = query.value(21).toInt();
        delivery.debtPay = query.value(22).toDouble();
        delivery.cashMoney = query.value(23).toDouble();
        delivery.goodsCheckStatus = query.value(24).toInt();
        delivery.switchCharge = query.value(25).toInt();
        delivery.senderPhotoPath = query.value(26).toString();
        delivery.getterPhotoPath = query.value(27).toString();
        delivery.expireHours = query.value(28).toInt();
        delivery.expireStartTariff = query.value(29).toDouble();
        delivery.expireStep = query.value(30).toInt();
        delivery.expireStepMoney = query.value(31).toDouble();

        deliveryInfo.append(delivery);
    }

    return ret;
}

bool CDelivery::selectExpTakeDeliveryInfo(QString m1Card, QList<DelivertDesc_t> &deliveryInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT d.deliveryId,d.cellId,d.cellType,d.cabinetId,d.m1Card,d.mobilephone,d.packageId,d.expName,d.expMobilePhone,d.expCompanyName,d.createtime,d.status,d.messageStatus,d.messageSentTime,d.getCode,d.takeTime,\
                  d.hasPayMoney,d.expressPay,d.ownerBasePay,d.tariffType,d.expireStatus,d.payway,d.debtPay,d.cashMoney,d.goodsCheckStatus,d.switchCharge,d.senderPhotoPath,d.getterPhotoPath,d.expireHours,d.expireStartTariff,d.expireStep,d.expireStepMoney\
                  from t_basic_delivery d left join t_basic_cell c on d.cellId = c.cellId where d.m1Card = ? and d.status = 1 and c.status = 1");
    query.bindValue(0,m1Card);
    query.exec();

    while(query.next())
    {
        ret = true;
        DelivertDesc_t delivery;

        delivery.deliveryId = query.value(0).toString();
        delivery.cellId = query.value(1).toString();
        delivery.cellType = query.value(2).toInt();
        delivery.cabinetId = query.value(3).toString();
        delivery.m1card = query.value(4).toString();
        delivery.mobilephone = query.value(5).toString();
        delivery.packageId = query.value(6).toString();
        delivery.expName = query.value(7).toString();
        delivery.expMobilePhone = query.value(8).toString();
        delivery.expCompanyName = query.value(9).toString();
        delivery.createtime = query.value(10).toDateTime();
        delivery.status = query.value(11).toInt();
        delivery.messageStatus = query.value(12).toInt();
        delivery.messageSentTime = query.value(13).toDateTime();
        delivery.getCode = query.value(14).toString();
        delivery.takeTime = query.value(15).toDateTime();
        delivery.hasPayMoney = query.value(16).toDouble();
        delivery.expressPay = query.value(17).toDouble();
        delivery.ownerBasePay = query.value(18).toDouble();
        delivery.tariffType = query.value(19).toInt();
        delivery.expireStatus = query.value(20).toInt();
        delivery.payway = query.value(21).toInt();
        delivery.debtPay = query.value(22).toDouble();
        delivery.cashMoney = query.value(23).toDouble();
        delivery.goodsCheckStatus = query.value(24).toInt();
        delivery.switchCharge = query.value(25).toInt();
        delivery.senderPhotoPath = query.value(26).toString();
        delivery.getterPhotoPath = query.value(27).toString();
        delivery.expireHours = query.value(28).toInt();
        delivery.expireStartTariff = query.value(29).toDouble();
        delivery.expireStep = query.value(30).toInt();
        delivery.expireStepMoney = query.value(31).toDouble();

        deliveryInfo.append(delivery);
    }

    return ret;
}


/****************************************************************
**
*	函数名称：	selectTakeDeliveryInfo
*	函数功能：	查找是否有这个取件码
* 	函数参数：	code取件码  DelivertDesc_t 派件信息列表
* 	返回值  :       true 有
*
****************************************************************/
bool CDelivery::selectTakeDeliveryInfo(QString code, DelivertDesc_t &deliveryInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_delivery where getCode = ? and (status = 1 or status = 3) ");
    query.bindValue(0,code);
    query.exec();

    if(query.first())
    {
        deliveryInfo.deliveryId = query.value(0).toString();
        deliveryInfo.cellId = query.value(1).toString();
        deliveryInfo.cellType = query.value(2).toInt();
        deliveryInfo.cabinetId = query.value(3).toString();
        deliveryInfo.m1card = query.value(4).toString();
        deliveryInfo.mobilephone = query.value(5).toString();
        deliveryInfo.packageId = query.value(6).toString();
        deliveryInfo.expName = query.value(7).toString();
        deliveryInfo.expMobilePhone = query.value(8).toString();
        deliveryInfo.expCompanyName = query.value(9).toString();
        deliveryInfo.createtime = query.value(10).toDateTime();
        deliveryInfo.status = query.value(11).toInt();
        deliveryInfo.messageStatus = query.value(12).toInt();
        deliveryInfo.messageSentTime = query.value(13).toDateTime();
        deliveryInfo.getCode = query.value(14).toString();
        deliveryInfo.takeTime = query.value(15).toDateTime();
        deliveryInfo.hasPayMoney = query.value(16).toDouble();
        deliveryInfo.expressPay = query.value(17).toDouble();
        deliveryInfo.ownerBasePay = query.value(18).toDouble();
        deliveryInfo.tariffType = query.value(19).toInt();
        deliveryInfo.expireStatus = query.value(20).toInt();
        deliveryInfo.payway = query.value(21).toInt();
        deliveryInfo.debtPay = query.value(22).toDouble();
        deliveryInfo.cashMoney = query.value(23).toDouble();
        deliveryInfo.goodsCheckStatus = query.value(24).toInt();
        deliveryInfo.switchCharge = query.value(25).toInt();
        deliveryInfo.senderPhotoPath = query.value(26).toString();
        deliveryInfo.getterPhotoPath = query.value(27).toString();
        deliveryInfo.expireHours = query.value(28).toInt();
        deliveryInfo.expireStartTariff = query.value(29).toDouble();
        deliveryInfo.expireStep = query.value(30).toInt();
        deliveryInfo.expireStepMoney = query.value(31).toDouble();

        ret = true;
    }

    return ret;

}

/****************************************************************
**
*	函数名称：	selectTakeCodeForPhone
*	函数功能：	通过手机号查询取件码
* 	函数参数：	phone 手机号 code 派件信息列表
* 	返回值  :       true 有 code 格式 206761、377461、
*
****************************************************************/
bool CDelivery::selectTakeCodeForPhone(QString phone, QString &code)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT getCode from t_basic_delivery where mobilephone = ? and status = 1");
    query.bindValue(0,phone);
    query.exec();

    while(query.next())
    {
        code = code + query.value(0).toString() + ",";

        ret = true;
    }

    return ret;
}


/****************************************************************
**
*	函数名称：	createRandomCode
*	函数功能：	生成存件取件码
* 	函数参数：	 无
* 	返回值  :       QString
*
****************************************************************/
QString CDelivery::createRandomCode()
{
    QSqlQuery query;
    QString tempCode;
    qsrand(time(0));

    while(1)
    {
        int firstValue = qrand()%10;
        int twoValue = 9 - firstValue;
        int threeValue = qrand()%10;
        int fourValue = qrand()%10;
        int fiveValue = qrand()%10;
        int sixValue = qrand()%10;

        tempCode = QString::number(firstValue) + QString::number(twoValue) + QString::number(threeValue);
        tempCode = tempCode + QString::number(fourValue) + QString::number(fiveValue) + QString::number(sixValue);

        query.prepare("SELECT * from t_basic_delivery where getCode = ? and (status = 1 or status = 3)");
        query.bindValue(0,tempCode);
        query.exec();
        if(!query.first())
        {
           break;
        }
    }
    return tempCode;
}

bool CDelivery::selectExceptionCodeForCell(QString cell, DelivertDesc_t &deliveryInfo)
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_delivery where cellId = ? and status = 1");
    query.bindValue(0,cell);
    query.exec();

    if(query.first())
    {
        deliveryInfo.deliveryId = query.value(0).toString();
        deliveryInfo.cellId = query.value(1).toString();
        deliveryInfo.cellType = query.value(2).toInt();
        deliveryInfo.cabinetId = query.value(3).toString();
        deliveryInfo.m1card = query.value(4).toString();
        deliveryInfo.mobilephone = query.value(5).toString();
        deliveryInfo.packageId = query.value(6).toString();
        deliveryInfo.expName = query.value(7).toString();
        deliveryInfo.expMobilePhone = query.value(8).toString();
        deliveryInfo.expCompanyName = query.value(9).toString();
        deliveryInfo.createtime = query.value(10).toDateTime();
        deliveryInfo.status = query.value(11).toInt();
        deliveryInfo.messageStatus = query.value(12).toInt();
        deliveryInfo.messageSentTime = query.value(13).toDateTime();
        deliveryInfo.getCode = query.value(14).toString();
        deliveryInfo.takeTime = query.value(15).toDateTime();
        deliveryInfo.hasPayMoney = query.value(16).toDouble();
        deliveryInfo.expressPay = query.value(17).toDouble();
        deliveryInfo.ownerBasePay = query.value(18).toDouble();
        deliveryInfo.tariffType = query.value(19).toInt();
        deliveryInfo.expireStatus = query.value(20).toInt();
        deliveryInfo.payway = query.value(21).toInt();
        deliveryInfo.debtPay = query.value(22).toDouble();
        deliveryInfo.cashMoney = query.value(23).toDouble();
        deliveryInfo.goodsCheckStatus = query.value(24).toInt();
        deliveryInfo.switchCharge = query.value(25).toInt();
        deliveryInfo.senderPhotoPath = query.value(26).toString();
        deliveryInfo.getterPhotoPath = query.value(27).toString();
        deliveryInfo.expireHours = query.value(28).toInt();
        deliveryInfo.expireStartTariff = query.value(29).toDouble();
        deliveryInfo.expireStep = query.value(30).toInt();
        deliveryInfo.expireStepMoney = query.value(31).toDouble();

        ret = true;
    }

    return ret;
}

void CDelivery::clearDeliveryInfo(DelivertDesc_t &deliveryInfo)
{
    deliveryInfo.deliveryId.clear();
    deliveryInfo.cellId.clear();
    deliveryInfo.cellType = 0;
    deliveryInfo.cabinetId.clear();
    deliveryInfo.m1card.clear();
    deliveryInfo.mobilephone.clear();
    deliveryInfo.packageId.clear();
    deliveryInfo.expName.clear();
    deliveryInfo.expMobilePhone.clear();
    deliveryInfo.expCompanyName.clear();
   // deliveryInfo.createtime.;
    deliveryInfo.status = 0;
    deliveryInfo.messageStatus = 1;
  //  deliveryInfo.messageSentTime = query.value(12).toDateTime();
    deliveryInfo.getCode.clear();
   // deliveryInfo.takeTime = query.value(14).toDateTime();
    deliveryInfo.hasPayMoney = 0.00;
    deliveryInfo.expressPay = 0.00;
    deliveryInfo.ownerBasePay = 0.00;
    deliveryInfo.tariffType = 1;
    deliveryInfo.expireStatus = 1;
    deliveryInfo.payway = 1;
    deliveryInfo.debtPay = 0.00;
    deliveryInfo.cashMoney = 0.00;
    deliveryInfo.goodsCheckStatus = 3;
    deliveryInfo.switchCharge = 1;
    deliveryInfo.senderPhotoPath.clear();
    deliveryInfo.getterPhotoPath.clear();
    deliveryInfo.expireHours = 0;
    deliveryInfo.expireStartTariff = 0.00;
    deliveryInfo.expireStep = 0;
    deliveryInfo.expireStepMoney = 0.00;
}

void CDelivery::updateDeliveryNoteSendTimeInfo(QString uuid, QDateTime time)
{
    QSqlQuery query;
    query.prepare("update t_basic_delivery set messageSentTime = ? where deliveryId = ?");
    query.bindValue(0,time);
    query.bindValue(1,uuid);
    query.exec();
}

void CDelivery::updateDeliveryExpTakeInfo(QString uuid)
{
    QDateTime time = QDateTime::currentDateTime();
    QSqlQuery query;
    query.prepare("update t_basic_delivery set status = 5,takeTime = ? where deliveryId = ?");
    query.bindValue(0,time);
    query.bindValue(1,uuid);
    query.exec();
}

qint8 CDelivery::clearTariffInfo(QString uuid)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status from t_basic_delivery where deliveryId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {
        ret = 2;
        qint8 status = query.value(0).toInt();
        if(status == 4)
        {
            ret = 0;
        }
        if((status == 1 ) || (status == 3))
        {
            ret = 1;
            query.prepare("update t_basic_delivery set switchCharge = 2,ownerBasePay =0.00 where deliveryId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }
    }

    return ret;
}

qint8 CDelivery::closeDeliveryInfo(QString uuid,QString &cell)
{
    qint8 ret = 0;
    QSqlQuery query;
    query.prepare("SELECT status,cellId from t_basic_delivery where deliveryId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {
        ret = 2;
        qint8 status = query.value(0).toInt();
        if(status == 4)
        {
            ret = 0;
        }
        if((status == 1) || (status == 3))
        {
            ret = 1;
            cell = query.value(1).toString();

            query.prepare("update t_basic_delivery set status = 2 where deliveryId = ?");
            query.bindValue(0,uuid);
            query.exec();
        }

    }

    return ret;
}

qint8 CDelivery::confirmDeliveryInfo(QString uuid, DelivertDesc_t &deliveryInfo)
{
    qint8 ret = -1;
    QSqlQuery query;
    query.prepare("SELECT * from t_basic_delivery where deliveryId = ?");
    query.bindValue(0,uuid);
    query.exec();

    if(query.first())
    {
        ret = 0;
        deliveryInfo.deliveryId = query.value(0).toString();
        deliveryInfo.cellId = query.value(1).toString();
        deliveryInfo.cellType = query.value(2).toInt();
        deliveryInfo.cabinetId = query.value(3).toString();
        deliveryInfo.m1card = query.value(4).toString();
        deliveryInfo.mobilephone = query.value(5).toString();
        deliveryInfo.packageId = query.value(6).toString();
        deliveryInfo.expName = query.value(7).toString();
        deliveryInfo.expMobilePhone = query.value(8).toString();
        deliveryInfo.expCompanyName = query.value(9).toString();
        deliveryInfo.createtime = query.value(10).toDateTime();
        deliveryInfo.status = query.value(11).toInt();
        deliveryInfo.messageStatus = query.value(12).toInt();
        deliveryInfo.messageSentTime = query.value(13).toDateTime();
        deliveryInfo.getCode = query.value(14).toString();
        deliveryInfo.takeTime = query.value(15).toDateTime();
        deliveryInfo.hasPayMoney = query.value(16).toDouble();
        deliveryInfo.expressPay = query.value(17).toDouble();
        deliveryInfo.ownerBasePay = query.value(18).toDouble();
        deliveryInfo.tariffType = query.value(19).toInt();
        deliveryInfo.expireStatus = query.value(20).toInt();
        deliveryInfo.payway = query.value(21).toInt();
        deliveryInfo.debtPay = query.value(22).toDouble();
        deliveryInfo.cashMoney = query.value(23).toDouble();
        deliveryInfo.goodsCheckStatus = query.value(24).toInt();
        deliveryInfo.switchCharge = query.value(25).toInt();
        deliveryInfo.senderPhotoPath = query.value(26).toString();
        deliveryInfo.getterPhotoPath = query.value(27).toString();
        deliveryInfo.expireHours = query.value(28).toInt();
        deliveryInfo.expireStartTariff = query.value(29).toDouble();
        deliveryInfo.expireStep = query.value(30).toInt();
        deliveryInfo.expireStepMoney = query.value(31).toDouble();

        if(4 == deliveryInfo.status)
        {
            ret = 1; //已取消派件
        }
        else if(3 == deliveryInfo.status)
        {
            ret = 3; //待确认派件
        }
        else if(1 == deliveryInfo.status)
        {
            ret = 2; //已存入
        }
    }

    return ret;

}

bool CDelivery::queryExpDeliveryInfo(QList<DelivertDesc_t> deliveryInfo,QList<QStringList> &modelData)
{
    qint8 tab = 0;
    for(int i = 0; i < deliveryInfo.size(); i++)
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << deliveryInfo.at(i).packageId << deliveryInfo.at(i).mobilephone \
                         << deliveryInfo.at(i).createtime.toString("yyyy-MM-dd hh:mm:ss") << deliveryInfo.at(i).cellId << "存入" << deliveryInfo.at(i).deliveryId\
                         << deliveryInfo.at(i).getCode << deliveryInfo.at(i).expCompanyName << deliveryInfo.at(i).expMobilePhone);
    }

    return true;
}

bool CDelivery::queryExpTakeDeliveryInfo(QList<DelivertDesc_t> deliveryInfo, QList<QStringList> &modelData)
{
    qint8 tab = 0;
    for(int i = 0; i < deliveryInfo.size(); i++)
    {
        tab++;
        modelData.append(QStringList() << QString::number(tab) << deliveryInfo.at(i).packageId << deliveryInfo.at(i).mobilephone \
                         << deliveryInfo.at(i).createtime.toString("yyyy-MM-dd hh:mm:ss") << deliveryInfo.at(i).cellId << "打开" << deliveryInfo.at(i).deliveryId\
                         << deliveryInfo.at(i).getCode << deliveryInfo.at(i).expCompanyName << deliveryInfo.at(i).expMobilePhone);
    }

    return true;
}
