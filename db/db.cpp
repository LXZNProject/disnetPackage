
#include "db.h"
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "t_log_smslimit.h"
#include "t_cabinet_cell.h"
#include "t_basic_delivery.h"
#include "t_basic_stored_record.h"
#include "t_basic_temp_storage.h"
#include "t_income_detail.h"
#include "t_income_total.h"
#include "t_log_smslog.h"
#include "t_basic_lan_jian.h"
#include "t_temp_consume_record.h"
#include "t_log_network.h"
#include "t_basic_sync_forbidden.h"
#include "t_image_time.h"
#include "t_basic_cell_order.h"

#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QStringList>

#include "servercomm.h"

#include "notethread.h"
#include "networkthread.h"
#include <QUuid>

void CDatBase::init()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("database.sqlite");
    db.setUserName("lxzn");
    db.setPassword("lxzn123456");


    if(db.open())
    {
        SCTerminal::instance()->init();
        SCNoteTemplate::instance()->init();
        SCNoteLimit::instance()->init();
        ST_cabinet_cell::instance()->init();
        SCDelivery::instance()->init();
        SBasicStoredRecord::instance()->init();
        SBasicTempStorage::instance()->init();
        STIncomeDetail::instance()->init();
        STIncomeTotal::instance()->init();
        SCSmslog::instance()->init();
        SCSender::instance()->init();
        STempRecord::instance()->init();
        SCNetwork::instance()->init();
        SBalckList::instance()->init();
        SCImageTime::instance()->init();
        STCellOrder::instance()->init();
    }

     SCDelivery::instance()->deleteOverTimeDeliveryInfo();
     ST_cabinet_cell::instance()->deleteOverTimeExcpInfo();
     SCNetwork::instance()->deleteNetWorkInfo();
     SCNoteLimit::instance()->deleteOverTimeNoteInfo();
     STempRecord::instance()->deleteTempRecordInfo();

//    QString aa = "12345678901";
//    QDate date;
//    int number = 2;
//    QDate date(2015,2,9);
//    SCNoteLimit::instance()->insertNote(aa,date);
//    SCNoteLimit::instance()->insertNote(aa,date);

//   bool status = SCNoteLimit::instance()->checkNoteSendInfo(aa,number);
    //    qDebug() << status << number;

}

QList<CmdDesc_t> CDatBase::assistTerminalInfo(QList<QRemoteCommandList *> cmd)
{
    QList<CmdDesc_t> cmdList;

    for(int i = 0; i < cmd.size(); i++)
    {
        CmdDesc_t cmdInfo;
        cmdInfo.mId = cmd.at(i)->id();
        cmdInfo.mCmdType = cmd.at(i)->commandType();
        cmdInfo.mContent = cmd.at(i)->commandContent();

        QString unknowInfo = cmd.at(i)->commandContent();
        QString tempCell;

        if(1 == cmdInfo.mCmdType.toInt())
        {
            if(unknowInfo.size() == 4)
            {
                qint8 cabinetId =  unknowInfo.left(2).toInt();
                qint8 cellId = unknowInfo.right(2).toInt();

                if(lockoperation.openLock(cabinetId,cellId) == 1)
                {
                    cmdInfo.mCmdStatus = "2"; //成功
                }
                else
                {
                    cmdInfo.mCmdStatus = "3"; //失败
                    cmdInfo.mCmdRet = QString::fromUtf8("打不开");
                }
            }
            else
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("命令格式不正确");
            }
        }
        else if(2 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SCDelivery::instance()->clearTariffInfo(unknowInfo);
            if(ret == 1)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
            }
            else if(ret == 0)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已取出");
            }
        }
        else if(3 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SBasicTempStorage::instance()->clearTariffInfo(unknowInfo);
            if(ret == 1)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
            }
            else if(ret == 0)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已取出");
            }
        }
        else if(4 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SCSender::instance()->clearTariffInfo(unknowInfo);
            if(ret == 1)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
            }
            else if(ret == 0)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已取出");
            }
        }
        else if(5 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SCDelivery::instance()->closeDeliveryInfo(unknowInfo,tempCell);
            if(1 == ret)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
                 cmdInfo.mCellId = tempCell;
                 ST_cabinet_cell::instance()->closeDeliveryInfo(tempCell);
            }
            else if(0 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已关闭");
            }
        }
        else if(6 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SBasicTempStorage::instance()->closeDeliveryInfo(unknowInfo,tempCell);
            if(1 == ret)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
                 cmdInfo.mCellId = tempCell;
                 ST_cabinet_cell::instance()->closeDeliveryInfo(tempCell);
            }
            else if(0 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已关闭");
            }
        }
        else if(7 == cmdInfo.mCmdType.toInt())
        {
            qint8 ret = SCSender::instance()->closeDeliveryInfo(unknowInfo,tempCell);
            if(1 == ret)
            {
                 cmdInfo.mCmdStatus = "2"; //成功
                 cmdInfo.mCellId = tempCell;
                 ST_cabinet_cell::instance()->closeDeliveryInfo(tempCell);
            }
            else if(0 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
            else if(2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已关闭");
            }
        }
        else if(8 == cmdInfo.mCmdType.toInt())
        {
            DelivertDesc_t deliveryInfo;
            qint8 ret = SCDelivery::instance()->confirmDeliveryInfo(unknowInfo,deliveryInfo);
            if(3 == ret)
            {
                cmdInfo.mCmdStatus = "2"; //成功
                deliveryInfo.status = 1;

                if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
                {
                    sendNote(deliveryInfo);
                }
                else
                {
                    sendNetNote(deliveryInfo);
                }
                SCDelivery::instance()->updateDeliveryInfo(deliveryInfo);
            }
            else if( 2 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已确认");
            }
            else if(1 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已取消");
            }
            else if(0 == ret)
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("派件已取出");
            }
            else
            {
                cmdInfo.mCmdStatus = "3"; //失败
                cmdInfo.mCmdRet = QString::fromUtf8("无此派件");
            }
        }
        else
        {
            cmdInfo.mCmdStatus = "3"; //失败
            cmdInfo.mCmdRet = QString::fromUtf8("暂不支持此命令");
        }

        cmdList.append(cmdInfo);
    }

    return cmdList;
}

void CDatBase::sendNote(DelivertDesc_t &deliveryInfo)
{
    NoteDesc_t noteInfo;
    QString smsContent;
    QString tollContent = "";
    deliveryInfo.messageSentTime = QDateTime::currentDateTime();

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
    noteInfo.mSmsLogId = strUuId;
    noteInfo.mStatus = 3;
    noteInfo.mSendTime = QDateTime::currentDateTime();

    noteInfo.mSmsType = 1;
    noteInfo.mRelPriKey = deliveryInfo.deliveryId;
    noteInfo.mMobilePhone = deliveryInfo.mobilephone;
    noteInfo.mSendCount = 0;

    SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);

    qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
    qint8 tariffStatus = SCTerminal::instance()->terminalInfo.expireStatus;
    if( tariffType == 3 || tariffType == 4 )
    {
        if(1 == tariffStatus)
        {
            tollContent = QString::fromUtf8("，每件%1元").arg(SCTerminal::instance()->terminalInfo.userBasicTariff);
        }
        else if (2 == tariffStatus)
        {
            tollContent = QString::fromUtf8("，每件%1元，逾期加收").arg(SCTerminal::instance()->terminalInfo.userBasicTariff);
        }
        noteInfo.mSmsContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
                deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(tollContent);
    }
    else
    {
        if(2 == tariffStatus)
        {
            tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
        }
        noteInfo.mSmsContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
                    deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(tollContent);
    }

    SCSmslog::instance()->insertNoteInfo(noteInfo);
    SCNoteThread::instance()->sendNoteEvent(noteInfo);

}

void CDatBase::sendNetNote(DelivertDesc_t &deliveryInfo)
{
    //调用短信发送接口
   NetworkDesc_t noteInfo;
   QString smsContent;
   QString tollContent = "";
   noteInfo.mRelPriKey = deliveryInfo.deliveryId;


   noteInfo.mSmsType = 1;
   deliveryInfo.messageSentTime = QDateTime::currentDateTime();
   noteInfo.mPhone = deliveryInfo.mobilephone;


   SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);

   qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
   qint8 tariffStatus = SCTerminal::instance()->terminalInfo.expireStatus;
   if( tariffType == 3 || tariffType == 4 )
   {
       if(1 == tariffStatus)
       {
           tollContent = QString::fromUtf8("，每件%1元").arg(SCTerminal::instance()->terminalInfo.userBasicTariff);
       }
       else if (2 == tariffStatus)
       {
           tollContent = QString::fromUtf8("，每件%1元，逾期加收").arg(SCTerminal::instance()->terminalInfo.userBasicTariff);
       }
       noteInfo.mContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
                   deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(tollContent);
   }
   else
   {
       if(2 == tariffStatus)
       {
           tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
       }
       noteInfo.mContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
                   deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(tollContent);
   }

   SCServerComm::instance()->sendSms(noteInfo);
   SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);
}


void CDatBase::setWarn(QString status)
{
    bool ret = false;
    QSqlQuery query(db);
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET sound = '%1'").arg(status));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }

}


QString CDatBase::getWarn()
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    QString status = query.value(0).toString();

    return status;

}


void CDatBase::setServerInfo(QString ip, QString port)
{
    bool ret = false;
    QSqlQuery query(db);
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET ip = '%1' ,port = '%2'").arg(ip).arg(port));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }

}


void CDatBase::getServerInfo(QString &ip, QString &port)
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    ip = query.value(1).toString();
    port = query.value(2).toString();

}


void CDatBase::setRfidAddr(quint8 addr)
{
    bool ret = false;
    QSqlQuery query;
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET address = '%1'").arg(addr));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }
}

quint8 CDatBase::getRfidAddr()
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    quint8 addr = query.value(3).toUInt();

    return addr;
}

void CDatBase::setComPort(QString com)
{
    bool ret = false;
    QSqlQuery query;
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET com = '%1'").arg(com));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }
}


QString CDatBase::getComPort()
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    QString com = query.value(4).toString();

    return com;
}

void CDatBase::setWarnAddr(quint8 addr)
{
    bool ret = false;
    QSqlQuery query;
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET WarnAddress = '%1'").arg(addr));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }
}

quint8 CDatBase::getWarnAddr()
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    quint8 addr = query.value(5).toUInt();

    return addr;
}

void CDatBase::setWarnComPort(QString com)
{
    bool ret = false;
    QSqlQuery query;
    db.transaction();
    ret = query.exec(QString("UPDATE configInfo SET WarnCom = '%1'").arg(com));

    if(ret)
    {
        db.commit();
    }
    else
    {
        db.rollback();
    }
}

QString CDatBase::getWarnComPort()
{
    QSqlQuery query;
    query.exec("select * from configInfo");
    query.first();

    QString com = query.value(6).toString();

    //qDebug() << "getWarnComPort :" << com;

    return com;
}
