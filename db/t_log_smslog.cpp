#include "t_log_smslog.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

CSmslog::CSmslog()
{
}

void CSmslog::init()
{
    QSqlQuery query;
    query.exec("create table [t_log_smslog](smsLogId varchar(32) primary key,mobilePhone  varchar(18),smsContent varchar(500), sendTime datetime,status integer,smsType interger,relPriKey varchar(32),sendCount interger)");
}

bool CSmslog::insertNoteInfo(NoteDesc_t noteInfo)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_log_smslog WHERE smsLogId = ?");
    query.bindValue(0,noteInfo.mSmsLogId);
    query.exec();

    if(query.first())
    {
        if(noteInfo.mStatus == 1)
        {
            query.prepare("DELETE FROM t_log_smslog where smsLogId = ?");
            query.bindValue(0,noteInfo.mSmsLogId);
        }
        else
        {
            query.prepare("update t_log_smslog set sendTime = ?,status = ?,sendCount = ?");
            query.addBindValue(noteInfo.mSendTime);
            query.addBindValue(noteInfo.mStatus);
            query.addBindValue(noteInfo.mSendCount);
        }
    }
    else
    {
        if(noteInfo.mStatus == 1)
        {
            return true;
        }
        else
        {
            query.prepare("INSERT INTO t_log_smslog VALUES(?,?,?,?,?,?,?,?)");
            query.addBindValue(noteInfo.mSmsLogId);
            query.addBindValue(noteInfo.mMobilePhone);
            query.addBindValue(noteInfo.mSmsContent);
            query.addBindValue(noteInfo.mSendTime);
            query.addBindValue(noteInfo.mStatus);
            query.addBindValue(noteInfo.mSmsType);
            query.addBindValue(noteInfo.mRelPriKey);
            query.addBindValue(noteInfo.mSendCount);
        }
    }

    return query.exec();
}

bool CSmslog::updateNoteInfo(NoteDesc_t noteInfo)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_log_smslog WHERE smsType = ? and relPriKey = ?");
    query.bindValue(0,noteInfo.mSmsType);
    query.bindValue(1,noteInfo.mRelPriKey);
    query.exec();

    if(query.first())
    {
        query.prepare("update t_log_smslog set sendTime = ?,status = ?,sendCount = ?");
        query.addBindValue(noteInfo.mSendTime);
        query.addBindValue(noteInfo.mStatus);
        query.addBindValue(noteInfo.mSendCount);
    }
    else
    {
        query.prepare("INSERT INTO t_log_smslog VALUES(null,?,?,?,?,?,?,?)");
        query.addBindValue(noteInfo.mMobilePhone);
        query.addBindValue(noteInfo.mSmsContent);
        query.addBindValue(noteInfo.mSendTime);
        query.addBindValue(noteInfo.mStatus);
        query.addBindValue(noteInfo.mSmsType);
        query.addBindValue(noteInfo.mRelPriKey);
        query.addBindValue(noteInfo.mSendCount);

    }

    return query.exec();
}

bool CSmslog::queryLoseNoteInfo(QList<NoteDesc_t> &noteInfo,qint8 status, qint8 loseNum)
{
    QSqlQuery query;

    query.prepare("SELECT * FROM t_log_smslog WHERE status = ?");
    query.bindValue(0,status);
    query.exec();

    while(query.next())
    {
        NoteDesc_t note;
        note.mSmsLogId = query.value(0).toString();
        note.mMobilePhone = query.value(1).toString();
        note.mSmsContent = query.value(2).toString();
        note.mSendTime = query.value(3).toDateTime();
        note.mStatus = query.value(4).toInt();
        note.mSmsType = query.value(5).toInt();
        note.mRelPriKey = query.value(6).toString();
        note.mSendCount = query.value(7).toInt();

        if(note.mSendCount < loseNum)
        {
            noteInfo.append(note);
        }
    }

    return true;
}
