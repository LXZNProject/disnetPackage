#include "t_log_template.h"
#include <QSqlQuery>
#include <QVariant>
#include <QTextCodec>

CNoteTemplate::CNoteTemplate()
{
    tc =  QTextCodec::codecForName("UTF-8");
}

void CNoteTemplate::init()
{
    bool ret = false;
    QSqlQuery query;
    ret = query.exec("create table [t_log_template](\
               templateType integer, canShutDown integer, status integer ,templateContent varchar(500))");

    if(ret)
    {
        QString content = "您的%1包裹已存入%2%5格，取件码%3，请%4小时内取件%9。4006002128【易邮】";
        //1 收件人取件模板
        query.prepare("INSERT INTO t_log_template(\
           templateType,canShutDown,status,templateContent)\
                   VALUES (1,1,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();


        //2 用户取出派件模板
        content = "%1%2包裹已被取出。为了您的隐 私，建议将收件地址写为本小区自助包裹站XX收。4006002128【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (2,2,2,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();


        //3 快递员取出派件
        content = "您的取件码为%1的%2包裹已被快递员取出，快递员电话%3。4006002128【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (3,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //11 寄件通知寄件人模板
        content = "您的包裹已放入%1，揽件人将很快取件，揽件电话%2【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (11,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //12 寄件通知揽件人
        content = "寄件人%1选择%2，已将包裹存入%3%4格，取件码%5，请及时揽件【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (12,1,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //13寄件被取走，再次通知寄件人模板。
        content = "您放入%1的包裹已被取出，揽件人电话%2【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (13,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //21 临时寄存存入模板。
        content = "您的包裹已临时存放到%1，取件码%2,请于%3小时内取回。逾期将产生每%4小时%5元的滞留费【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (21,1,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //91 忘记取件码
        content = "您的取件码为:%1【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (91,1,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //92 格子异常，通知取件人模板
        content = "由于机械故障导致箱门无法打开，我们将紧急处理。修复后，会以短信方式通知您取件。给您带来不便，深表歉意。【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (92,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //93 格子异常，通知维修模板
        content = "%1%2格发生打不开异常，请及时修复。4006002128【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (93,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();


        //94 格子异常，修复通知取件人模板
        content = "%1%2格打不开异常已修复，请您及时取件，取件码%3。给您带来不便，深表歉意。【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (94,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //31 干洗通知寄件人模板
        content = "您的衣物已存入%1，揽件人将很快取件，揽件电话%2【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (31,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //32  干洗通知收件人
        content = "业主%1已将衣物放入%2%3格，取件码%4，请及时取件干洗【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (32,1,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //33  干洗被取走，再次通知寄件人
        content = "您放入%1的衣物已被取出，揽件电话%2【易邮】";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (33,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();

        //4  待确认
        content = "由于没有正常退出系统，您在%1包裹站，存入的派件没有确认,请重新登录。";
        query.prepare("INSERT INTO t_log_template(\
               templateType,canShutDown,status,templateContent)\
                       VALUES (4,2,1,?)");
        query.addBindValue(tc->toUnicode(content.toAscii()));
        query.exec();
    }

    getNoteTemplateInfo();
}

void CNoteTemplate::getNoteTemplateInfo()
{
    QSqlQuery query;
    query.exec("select * from t_log_template order by templateType asc");

    noteTemplateInfo.clear();
    while(query.next())
    {
        NoteTemplateDesc_t tempNote;

        tempNote.templateType = query.value(0).toInt();
        tempNote.canShutDown = query.value(1).toInt();
        tempNote.status = query.value(2).toInt();
        tempNote.templateContent = query.value(3).toString();

        noteTemplateInfo.append(tempNote);
    }

}

bool CNoteTemplate::setNoteTemplateInfo()
{
    for(int i = 0; i < noteTemplateInfo.size(); i++)
    {
        QSqlQuery query;
        query.prepare("update t_log_template set canShutDown = ?,status = ?,templateContent = ? where templateType = ?");
        query.bindValue(0,noteTemplateInfo.at(i).canShutDown);
        query.bindValue(1,noteTemplateInfo.at(i).status);
        query.bindValue(2,noteTemplateInfo.at(i).templateContent);
        query.bindValue(3,noteTemplateInfo.at(i).templateType);
        query.exec();
    }

    return true;
}

/*
  uoqu获取短信模版
  */
bool CNoteTemplate::getCurrentNoteInfo(int noteType,QString &content)
{
    bool ret = false;

    for(int i = 0; i < noteTemplateInfo.size(); i++)
    {
        if(noteTemplateInfo.at(i).templateType == noteType)
        {
            if(noteTemplateInfo.at(i).status == 1)
            {
                ret = true;
                content = noteTemplateInfo.at(i).templateContent;
                break;
            }
        }
    }

    return ret;
}

QString CNoteTemplate::getSaveNoteInfo(QString info, QString exp, QString terminal, QString code, QString time, QString cellId, QString inform)
{
    QString noteInfo = info;
    if(info.contains("%1"))
    {
        noteInfo.replace("%1",exp);
    }
    if(info.contains("%2"))
    {
        noteInfo.replace("%2",terminal);
    }
    if(info.contains("%3"))
    {
        noteInfo.replace("%3",code);
    }
    if(info.contains("%4"))
    {
        noteInfo.replace("%4",time);
    }
    if(info.contains("%5"))
    {
        noteInfo.replace("%5",cellId);
    }
    if(info.contains("%9"))
    {
        noteInfo.replace("%9",inform);
    }

    return noteInfo;
}



bool CNoteTemplate::setTemplateId(QString templateId)
{
    QSqlQuery query;
    query.prepare("update t_log_template set templateId = ?");
    query.addBindValue(templateId);

    return query.exec();
}

bool CNoteTemplate::setTemplateName(QString templateName)
{
    QSqlQuery query;
    query.prepare("update t_log_template set templateName = ?");
    query.addBindValue(templateName);

    return query.exec();
}

bool CNoteTemplate::setAddresseeContent(QString addresseeContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set addresseeContent = ?");
    query.addBindValue(addresseeContent);

    return query.exec();
}

bool CNoteTemplate::setGetPasswordContent(QString getPasswordContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set getPasswordContent = ?");
    query.addBindValue(getPasswordContent);

    return query.exec();
}

bool CNoteTemplate::setCellErrorContent(QString cellErrorContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set cellErrorContent = ?");
    query.addBindValue(cellErrorContent);

    return query.exec();
}

bool CNoteTemplate::setrepairContent(QString repairContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set repairContent = ?");
    query.addBindValue(repairContent);

    return query.exec();
}

bool CNoteTemplate::setNotifyReairContenxt(QString notifyReairContenxt)
{
    QSqlQuery query;
    query.prepare("update t_log_template set notifyReairContenxt = ?");
    query.addBindValue(notifyReairContenxt);

    return query.exec();
}

bool CNoteTemplate::setExpGetDeliveryContent(QString expGetDeliveryContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set expGetDeliveryContent = ?");
    query.addBindValue(expGetDeliveryContent);

    return query.exec();
}

bool CNoteTemplate::setSenderSaveContent(QString senderSaveContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set senderSaveContent = ?");
    query.addBindValue(senderSaveContent);

    return query.exec();
}

bool CNoteTemplate::setLanJianContent(QString lanJianContent)
{
    QSqlQuery query;
    query.prepare("update t_log_template set lanJianContent = ?");
    query.addBindValue(lanJianContent);

    return query.exec();
}
