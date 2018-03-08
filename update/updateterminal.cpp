#include "updateterminal.h"
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "t_cabinet_cell.h"

CUpdateTerminal::CUpdateTerminal(QObject *parent) :
    QThread(parent)
{
}

void CUpdateTerminal::updateTerminalInfo()
{
    syncTerminalDataRet respone;
    SCServerComm::instance()->syncTerminalData(&respone);

    QString date = respone.systemTime();
    if(!date.isEmpty())
    {
        date = "\"" + date + "\"";
        QString temp = QString("date -s %1").arg(date);
        QByteArray clockData = temp.toLatin1();
        system(clockData.data());

        system("hwclock -u -w");
    }

    if(respone.resultCode() == RET_TRUE)
    {
        startUpdateTerminalInfo(respone);
    }

}

void CUpdateTerminal::startUpdateTerminalInfo(syncTerminalDataRet &respone)
{
    QList<QTerminalInfo *>terminalInfoList = respone.getTerminalInfo();
    QList<QNoteTemplateInfo*> noteList = respone.getNoteTemplateList();
    QList<QCabinetInfo*> cabinetList = respone.getCabinetList();

    QTerminalInfo *terminalInfo = terminalInfoList.at(0);

    if(!terminalInfo->encryptCode().isEmpty())
    {
        SCTerminal::instance()->terminalInfo.encryptCode  =  terminalInfo->encryptCode();
    }
    SCTerminal::instance()->terminalInfo.terminalName     =  terminalInfo->terminalName();
    SCTerminal::instance()->terminalInfo.networkCard      =  terminalInfo->networkCard();
    SCTerminal::instance()->terminalInfo.serviceName      =  terminalInfo->linkManName();
    SCTerminal::instance()->terminalInfo.servicePhone     =  terminalInfo->linkMobilePhone();
    SCTerminal::instance()->terminalInfo.smsSendWay       =  terminalInfo->smsSendWay().toInt();
    SCTerminal::instance()->terminalInfo.noteLimit        =  terminalInfo->perSmslimit().toInt();
    SCTerminal::instance()->terminalInfo.tariffType       =  terminalInfo->tariffType().toInt();
    SCTerminal::instance()->terminalInfo.userBasicTariff  =  terminalInfo->userBasicTariff().toDouble();
    SCTerminal::instance()->terminalInfo.bigTariff        =  terminalInfo->expBigTariff().toDouble();
    SCTerminal::instance()->terminalInfo.middleTariff     =  terminalInfo->expMiddleTariff().toDouble();
    SCTerminal::instance()->terminalInfo.smallTariff      =  terminalInfo->expSmallTariff().toDouble();
    SCTerminal::instance()->terminalInfo.expireHours      =  terminalInfo->expireHours().toInt();
    SCTerminal::instance()->terminalInfo.expireStatus     =  terminalInfo->expireStatus().toInt();
    SCTerminal::instance()->terminalInfo.expireStartTariff=  terminalInfo->expireStartTariff().toDouble();
    SCTerminal::instance()->terminalInfo.expireStep       =  terminalInfo->expireStep().toInt();
    SCTerminal::instance()->terminalInfo.expireStepMoney  =  terminalInfo->expireStepMoney().toDouble();
    SCTerminal::instance()->terminalInfo.vipPercent       =  terminalInfo->vipPercent().toDouble();
    SCTerminal::instance()->terminalInfo.goodsCheck       =  terminalInfo->goodsCheck().toInt();
    SCTerminal::instance()->terminalInfo.forceExpTakeHours=  terminalInfo->forceExpTakeHours().toInt();

    //寄存
    SCTerminal::instance()->terminalInfo.storageHours     =  terminalInfo->storageHours().toInt();
    SCTerminal::instance()->terminalInfo.storageStartTariff= terminalInfo->storageStartTariff().toDouble();
    SCTerminal::instance()->terminalInfo.storageStep      =  terminalInfo->storageStep().toInt();
    SCTerminal::instance()->terminalInfo.storageStepMoney =  terminalInfo->storageStepMoney().toDouble();

    //寄件
    SCTerminal::instance()->terminalInfo.sendBaseTariff   =  terminalInfo->sendBaseTariff().toDouble();
    SCTerminal::instance()->terminalInfo.expGetTariff     =  terminalInfo->expGetTariff().toDouble();
    SCTerminal::instance()->terminalInfo.acceptExpCompanyName = terminalInfo->acceptExpCompanyName();
    SCTerminal::instance()->terminalInfo.lanJianName      =  terminalInfo->lanJianName();
    SCTerminal::instance()->terminalInfo.lanJianPhone     =  terminalInfo->lanJianPhone();

    //干洗
    SCTerminal::instance()->terminalInfo.washSwitch       =  terminalInfo->washSwitch().toInt();
    SCTerminal::instance()->terminalInfo.washSenderTariff =  terminalInfo->washSenderTariff().toDouble();
    SCTerminal::instance()->terminalInfo.washGeterTariff  =  terminalInfo->washGeterTariff().toDouble();
    SCTerminal::instance()->terminalInfo.washerName       =  terminalInfo->washerName();
    SCTerminal::instance()->terminalInfo.washerPhone      =  terminalInfo->washerPhone();

    if(!terminalInfo->serverIp().isEmpty())
    {
        SCTerminal::instance()->terminalInfo.serverIp     =  terminalInfo->serverIp();
    }
    if(!terminalInfo->serverPort().isEmpty())
    {
        SCTerminal::instance()->terminalInfo.serverPort   =  terminalInfo->serverPort();
    }
    SCTerminal::instance()->terminalInfo.syncModifyTime   =  QDateTime::fromString(terminalInfo->syncModifyTime(),Qt::ISODate);

    SCTerminal::instance()->setTerminalInfo();

    for(int i = 0; i < noteList.size(); i++)
    {
        for(int j = 0; j < SCNoteTemplate::instance()->noteTemplateInfo.size(); j++)
        {
            if(noteList.at(i)->templateType().toInt() == SCNoteTemplate::instance()->noteTemplateInfo.at(j).templateType)
            {
                NoteTemplateDesc_t oneNoteInfo;
                oneNoteInfo.templateType = noteList.at(i)->templateType().toInt();
                oneNoteInfo.canShutDown = noteList.at(i)->canShutDown().toInt();
                oneNoteInfo.status = noteList.at(i)->status().toInt();
                oneNoteInfo.templateContent = noteList.at(i)->templateContent();
                SCNoteTemplate::instance()->noteTemplateInfo.replace(j,oneNoteInfo);
                break;
            }
        }
    }

    SCNoteTemplate::instance()->setNoteTemplateInfo();

    for(int k = 0; k < cabinetList.size(); k++)
    {
        QString cabinetID = cabinetList.at(k)->cabinetId();
        qint8 cabinetType = cabinetList.at(k)->cabinetType().toInt();
        ST_cabinet_cell::instance()->insertCabinet(cabinetID,cabinetType);
        ST_cabinet_cell::instance()->initCell(cabinetID,cabinetType);
    }
}
