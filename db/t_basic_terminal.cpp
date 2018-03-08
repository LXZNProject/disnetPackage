#include "t_basic_terminal.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <QDateTime>
#include <QSettings>
#include <QDebug>

#define Version_Num     "1.116"
#define Version_Name    "V1.1.16"

CTerminal::CTerminal()
{
}

void CTerminal::init()
{
    bool ret = false;
    QSqlQuery query;
    ret = query.exec("create table [t_basic_terminal](\
                     companyId varchar(16),terminalId varchar(16),encryptCode varchar(16), terminalName varchar(64), networkCard varchar(32) ,linkManName varchar(32),linkMobilePhone varchar(16),smsSendWay integer,perSmslimit integer,tariffType integer, \
               userBasicTariff decimal(4,2),expBigTariff decimal(4,2),expMiddleTariff decimal(4,2),expSmallTariff decimal(4,2),expireHours integer,expireStatus integer,expireStartTariff decimal(4,2),expireStep integer, \
               expireStepmoney decimal(4,2),vipPercent decimal(3,2),goodsCheck integer,forceExpTakeHours integer,storageHours integer,storageStartTariff decimal(4,2),storageStep integer,storageStepMoney decimal(4,2), \
               sendBaseTariff decimal(4,2),expGetTariff decimal(4,2),acceptExpCompanyName varchar(128),lanJianName varchar(128),lanJianPhone varchar(128),washSwitch integer,washSenderTariff decimal(4,2),\
               washGeterTariff decimal(4,2),washerName varchar(128),washerPhone varchar(18),serverIp varchar(20),serverPort varchar(5),syncModifyTime datetime),runStatus integer");
    if(ret)
    {
        query.exec("INSERT INTO t_basic_terminal(\
           encryptCode,smsSendWay,perSmslimit,tariffType,userBasicTariff,expBigTariff,expMiddleTariff,expSmallTariff,expireHours,expireStatus,expireStartTariff,expireStep,expireStepmoney,vipPercent,goodsCheck,forceExpTakeHours,\
           storageHours,storageStartTariff,storageStep,storageStepMoney,sendBaseTariff,expGetTariff,washSwitch,washSenderTariff,\
                   washGeterTariff,syncModifyTime,runStatus) VALUES ('lxyz03V100',2,5,1,1.00,0.3,0.3,0.3,48,1,1.00,24,1.00,1.00,2,168,24,1.00,24,1.00,0.00,0.00,2,0.00,0.00,datetime('2015-01-01 00:00:00'),1)");
    }

    QSqlQuery q("select * from  t_basic_terminal");
    QSqlRecord rec = q.record();
    if(!rec.contains("runStatus"))
    {
        query.exec("alter table t_basic_terminal add column runStatus integer");
        query.prepare("update t_basic_terminal set runStatus = 1");
        query.exec();
    }

    getTerminalInfo();

    initConfigure();
    readSetting();
}

void CTerminal::getTerminalInfo()
{
    QSqlQuery query;
    query.exec("select * from t_basic_terminal");

    query.first();

    terminalInfo.companyId        =  query.value(0).toString();
    terminalInfo.terminalId       =  query.value(1).toString();
    terminalInfo.encryptCode      =  query.value(2).toString();
    terminalInfo.terminalName     =  query.value(3).toString();
    terminalInfo.networkCard      =  query.value(4).toString();
    terminalInfo.serviceName      =  query.value(5).toString();
    terminalInfo.servicePhone     =  query.value(6).toString();
    terminalInfo.smsSendWay       =  query.value(7).toInt();
    terminalInfo.noteLimit        =  query.value(8).toInt();
    terminalInfo.tariffType       =  query.value(9).toInt();
    terminalInfo.userBasicTariff  =  query.value(10).toDouble();
    terminalInfo.bigTariff        =  query.value(11).toDouble();
    terminalInfo.middleTariff     =  query.value(12).toDouble();
    terminalInfo.smallTariff      =  query.value(13).toDouble();
    terminalInfo.expireHours      =  query.value(14).toInt();
    terminalInfo.expireStatus     =  query.value(15).toInt();
    terminalInfo.expireStartTariff=  query.value(16).toDouble();
    terminalInfo.expireStep       =  query.value(17).toInt();
    terminalInfo.expireStepMoney  =  query.value(18).toDouble();
    terminalInfo.vipPercent       =  query.value(19).toDouble();
    terminalInfo.goodsCheck       =  query.value(20).toInt();
    terminalInfo.forceExpTakeHours=  query.value(21).toInt();

    //寄存
    terminalInfo.storageHours     =  query.value(22).toInt();
    terminalInfo.storageStartTariff= query.value(23).toDouble();
    terminalInfo.storageStep      =  query.value(24).toInt();
    terminalInfo.storageStepMoney =  query.value(25).toDouble();

    //寄件
    terminalInfo.sendBaseTariff   =  query.value(26).toDouble();
    terminalInfo.expGetTariff     =  query.value(27).toDouble();
    terminalInfo.acceptExpCompanyName = query.value(28).toString();
    terminalInfo.lanJianName      =  query.value(29).toString();
    terminalInfo.lanJianPhone     =  query.value(30).toString();

    //干洗
    terminalInfo.washSwitch       =  query.value(31).toInt();
    terminalInfo.washSenderTariff =  query.value(32).toDouble();
    terminalInfo.washGeterTariff  =  query.value(33).toDouble();
    terminalInfo.washerName       =  query.value(34).toString();
    terminalInfo.washerPhone      =  query.value(35).toString();

    terminalInfo.serverIp = query.value(36).toString();
    terminalInfo.serverPort = query.value(37).toString();
    terminalInfo.syncModifyTime = query.value(38).toDateTime();

    terminalInfo.runStatus = query.value(39).toInt();

    mUrl = "http://" + SCTerminal::instance()->terminalInfo.serverIp +":" + SCTerminal::instance()->terminalInfo.serverPort;

}

bool CTerminal::setTerminalInfo()
{
    bool ret = false;
    QSqlQuery query;
    query.prepare("update t_basic_terminal set companyId = ?,terminalId = ?,encryptCode = ?,terminalName = ?,networkCard = ?,linkManName = ?,linkMobilePhone = ?,smsSendWay = ?,perSmslimit = ?,\
                  tariffType = ?,userBasicTariff = ?,expBigTariff = ?,expMiddleTariff = ?,expSmallTariff =?,expireHours = ?,\
                  expireStatus = ?,expireStartTariff = ?,expireStep = ?,expireStepmoney = ?,vipPercent = ?,goodsCheck = ?,\
                  forceExpTakeHours = ?,storageHours = ?,storageStartTariff = ?,storageStep= ?,storageStepMoney =?,\
                  sendBaseTariff = ?,expGetTariff = ?,acceptExpCompanyName = ?,lanJianName = ?,lanJianPhone = ?,\
                  washSwitch = ?,washSenderTariff = ?,washGeterTariff = ?,washerName = ?,washerPhone =?,serverIp = ?,serverPort = ?,syncModifyTime =?,runStatus =?");

    query.addBindValue(terminalInfo.companyId);
    query.addBindValue(terminalInfo.terminalId);
    query.addBindValue(terminalInfo.encryptCode);
    query.addBindValue(terminalInfo.terminalName);
    query.addBindValue(terminalInfo.networkCard);
    query.addBindValue(terminalInfo.serviceName);
    query.addBindValue(terminalInfo.servicePhone);
    query.addBindValue(terminalInfo.smsSendWay);
    query.addBindValue(terminalInfo.noteLimit);
    query.addBindValue(terminalInfo.tariffType);
    query.addBindValue(terminalInfo.userBasicTariff);
    query.addBindValue(terminalInfo.bigTariff);
    query.addBindValue(terminalInfo.middleTariff);
    query.addBindValue(terminalInfo.smallTariff);
    query.addBindValue(terminalInfo.expireHours);
    query.addBindValue(terminalInfo.expireStatus);
    query.addBindValue(terminalInfo.expireStartTariff);
    query.addBindValue(terminalInfo.expireStep);
    query.addBindValue(terminalInfo.expireStepMoney);
    query.addBindValue(terminalInfo.vipPercent);
    query.addBindValue(terminalInfo.goodsCheck);
    query.addBindValue(terminalInfo.forceExpTakeHours);

    query.addBindValue(terminalInfo.storageHours);
    query.addBindValue(terminalInfo.storageStartTariff);
    query.addBindValue(terminalInfo.storageStep);
    query.addBindValue(terminalInfo.storageStepMoney);

    query.addBindValue(terminalInfo.sendBaseTariff);
    query.addBindValue(terminalInfo.expGetTariff);
    query.addBindValue(terminalInfo.acceptExpCompanyName);
    query.addBindValue(terminalInfo.lanJianName);
    query.addBindValue(terminalInfo.lanJianPhone);

    query.addBindValue(terminalInfo.washSwitch);
    query.addBindValue(terminalInfo.washSenderTariff);
    query.addBindValue(terminalInfo.washGeterTariff);
    query.addBindValue(terminalInfo.washerName);
    query.addBindValue(terminalInfo.washerPhone);

    query.addBindValue(terminalInfo.serverIp);
    query.addBindValue(terminalInfo.serverPort);
    query.addBindValue(terminalInfo.syncModifyTime);

    query.addBindValue(terminalInfo.runStatus);

    ret = query.exec();

    if(ret)
    {
        mUrl = "http://" + SCTerminal::instance()->terminalInfo.serverIp +":" + SCTerminal::instance()->terminalInfo.serverPort;
    }
    return ret;
}

void CTerminal::initConfigure()
{
    QSettings settings("CONFIG.INI",QSettings::IniFormat);
    settings.beginGroup("VERSION");
    settings.setValue("NUMBER",Version_Num);
    settings.setValue("NAME",Version_Name);
    settings.endGroup();
}

void CTerminal::readSetting()
{
    QSettings settings("CONFIG.INI",QSettings::IniFormat);

    mVersion_Num = settings.value("VERSION/NUMBER").toString();
    mVersion_Name = settings.value("VERSION/NAME").toString();
}

bool CTerminal::setCompanyId(QString companyId)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set companyId = ?");
    query.addBindValue(companyId);

    return query.exec();
}

bool CTerminal::setTerminalName(QString terminalName)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set terminalName = ?");
    query.addBindValue(terminalName);

    return query.exec();
}

bool CTerminal::setNetworkCard(QString card)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set networkCard = ?");
    query.addBindValue(card);

    return query.exec();
}

bool CTerminal::setPhone(QString phone)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set linkMobilePhone = ?");
    query.addBindValue(phone);

    return query.exec();
}

bool CTerminal::setPerSmslimit(QString noteNumber)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set perSmslimit = ?");
    query.addBindValue(noteNumber.toInt());

    return query.exec();
}

bool CTerminal::setTariffType(QString tartype)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set tariffType = ?");
    query.addBindValue(tartype.toInt());

    return query.exec();
}

bool CTerminal::setUserBasicTariff(QString userBasic)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set userBasicTariff = ?");
    query.addBindValue(userBasic.toDouble());

    return query.exec();
}

bool CTerminal::setExpTariff(QString big, QString middle, QString small)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expBigTariff = ?,expMiddleTariff = ?,expSmallTariff =?");
    query.addBindValue(big.toDouble());
    query.addBindValue(middle.toDouble());
    query.addBindValue(small.toDouble());

    return query.exec();
}

bool CTerminal::setExpireHours(QString expireHours)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expireHours = ?");
    query.addBindValue(expireHours.toInt());

    return query.exec();
}

bool CTerminal::setExpireStatus(QString expireStatus)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expireStatus = ?");
    query.addBindValue(expireStatus.toInt());

    return query.exec();
}

bool CTerminal::setExpireStartTariff(QString expireStartTariff)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expireStartTariff = ?");
    query.addBindValue(expireStartTariff.toDouble());

    return query.exec();
}

bool CTerminal::setExpireStep(QString expireStep)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expireStep = ?");
    query.addBindValue(expireStep.toInt());

    return query.exec();
}

bool CTerminal::setExpireStepmoney(QString expireStepmoney)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set expireStepmoney = ?");
    query.addBindValue(expireStepmoney.toDouble());

    return query.exec();
}

bool CTerminal::setVipPercent(QString vipPercent)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set vipPercent = ?");
    query.addBindValue(vipPercent.toDouble());

    return query.exec();
}

bool CTerminal::setGoodsCheck(QString goodsCheck)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set goodsCheck = ?");
    query.addBindValue(goodsCheck.toInt());

    return query.exec();
}

bool CTerminal::setForceExpTakeHours(QString forceExpTakeHours)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set forceExpTakeHours = ?");
    query.addBindValue(forceExpTakeHours.toInt());

    return query.exec();
}

bool CTerminal::setTempStorage(QString hours, QString startTariff, QString step, QString stepMoney)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set storageHours = ?,storageStartTariff = ?,storageStep= ?,storageStepMoney =?");
    query.addBindValue(hours.toInt());
    query.addBindValue(startTariff.toDouble());
    query.addBindValue(step.toInt());
    query.addBindValue(stepMoney.toDouble());

    return query.exec();
}

bool CTerminal::setSender(QString BaseTariff, QString expGetTariff, QString ExpCompanyName, QString name, QString phone)
{
    QSqlQuery query;
    query.prepare("update t_basic_terminal set sendBaseTariff = ?,expGetTariff = ?,acceptExpCompanyName = ?,lanJianName = ?,lanJianPhone = ?");
    query.addBindValue(BaseTariff.toDouble());
    query.addBindValue(expGetTariff.toDouble());
    query.addBindValue(ExpCompanyName);
    query.addBindValue(name);
    query.addBindValue(phone);

    return query.exec();
}


