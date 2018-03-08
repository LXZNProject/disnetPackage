#include "housewaitpage.h"
#include "ui_housewaitpage.h"

#include "t_cabinet_cell.h"
#include "mainctrl.h"
#include "t_log_smslog.h"
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "notethread.h"
#include "driver.h"
#include "servercomm.h"
#include "t_log_network.h"
#include "networkthread.h"

#include <QtCore>

housewaitpage::housewaitpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::housewaitpage)
{
    ui->setupUi(this);
    initHouseWaitPage();
}

housewaitpage::~housewaitpage()
{
    delete ui;
}

void housewaitpage::initHouseWaitPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);
    waitSaveTime = 120;

    this->setFixedSize(800,600);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/house/images/house/dry_bg.png")));
    this->setPalette(palette);

    ui->label->setFixedSize(QPixmap(":/sender/images/sender/word3.png").size());
    ui->label->setPixmap(QPixmap(":/sender/images/sender/word3.png"));

    ui->timeLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:36px; color: rgb(255, 0, 0)");
    ui->timeLabel->setText(QString("%1").arg(120));

    ui->door->resize(QPixmap(":/save/images/saveWait/door.png").size());
    ui->door->setPixmap(QPixmap(":/save/images/saveWait/door.png"));

    ui->doorCabinetId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCabinet->resize(QPixmap(":/save/images/saveWait/doorcabinet.png").size());
    ui->doorCabinet->setPixmap(QPixmap(":/save/images/saveWait/doorcabinet.png"));


    ui->doorCellId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCell->resize(QPixmap(":/save/images/saveWait/doorcell.png").size());
    ui->doorCell->setPixmap(QPixmap(":/save/images/saveWait/doorcell.png"));


    connect(&isOverTime,SIGNAL(timeout()),this,SLOT(overTime()));
    connect(&queryLock,SIGNAL(timeout()),this,SLOT(queryLockStatus()));

}

void housewaitpage::sendHouseNote(SenderDesc_t savePackInfo)
{
    //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;
    savePackInfo.messageSentTime = QDateTime::currentDateTime();


    noteInfo.mSmsType = 32;
    noteInfo.mRelPriKey = savePackInfo.lanJianId;
    noteInfo.mMobilePhone = savePackInfo.lanJianPhone;
    noteInfo.mSendCount = 0;

    SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);


    noteInfo.mSmsContent = smsContent.arg(savePackInfo.mobilephone).arg(\
            SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.cellId).arg(savePackInfo.getCode);

    if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
    {
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();
        noteInfo.mSmsLogId = strUuId;
        noteInfo.mStatus = 3;
        noteInfo.mSendTime = QDateTime::currentDateTime();

        SCSmslog::instance()->insertNoteInfo(noteInfo);
        SCNoteThread::instance()->sendNoteEvent(noteInfo);
    }
    else
    {
        NetworkDesc_t netInfo;
        netInfo.mPhone = noteInfo.mMobilePhone;
        netInfo.mSmsType = noteInfo.mSmsType;
        netInfo.mRelPriKey = noteInfo.mRelPriKey;
        netInfo.mContent = noteInfo.mSmsContent;

        SCServerComm::instance()->sendSms(netInfo);
        SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
    }

    //通知寄件人
    NoteDesc_t noteInfo2;
    QString smsContent2;
    noteInfo2.mSmsType = 31;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo2.mSmsType,smsContent2))
    {
        noteInfo2.mMobilePhone = savePackInfo.mobilephone;
        noteInfo2.mSendCount = 0;
        noteInfo2.mRelPriKey = savePackInfo.lanJianId;
        noteInfo2.mSmsContent = smsContent2.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.lanJianPhone);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            QUuid uuId1 = QUuid::createUuid();
            QString strUuId1 = uuId1.toString();
            strUuId1 = strUuId1.remove(QRegExp("[{}-]"));
            strUuId1 = strUuId1.toUpper();
            noteInfo2.mSmsLogId = strUuId1;
            noteInfo2.mStatus = 3;
            noteInfo2.mSendTime = QDateTime::currentDateTime();

            SCSmslog::instance()->insertNoteInfo(noteInfo2);
            SCNoteThread::instance()->sendNoteEvent(noteInfo2);
        }
        else
        {
            NetworkDesc_t netInfo2;
            netInfo2.mPhone = noteInfo2.mMobilePhone;
            netInfo2.mSmsType = noteInfo2.mSmsType;
            netInfo2.mRelPriKey = noteInfo2.mRelPriKey;
            netInfo2.mContent = noteInfo2.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo2);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo2);
        }
    }


    SCSender::instance()->updateSenderInfo(savePackInfo);
}
void housewaitpage::readWaitInfo(QByteArray param)
{
    waitSaveTime = 120;
    QByteArray data;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> data >> notOpenBoxList;

    savePackInfo = *(SenderDesc_t *)data.data();

    ui->timeLabel->setText(QString("%1").arg(waitSaveTime));
    ui->doorCabinetId->setText(savePackInfo.cabinetId);
    ui->doorCellId->setText(savePackInfo.cellId.right(2));

    isOverTime.start(1000);
    queryLock.start(3000);
}

void housewaitpage::overTime()
{
    waitSaveTime--;

    if(waitSaveTime <= 0)
    {
        isOverTime.stop();

        queryLock.stop();

        commitExceptionInfo(savePackInfo.cellId);
        //发送消息给后台

    }
    else
    {
        ui->timeLabel->setText(QString("%1").arg(waitSaveTime));
    }
}

void housewaitpage::queryLockStatus()
{
    qint8 status = -1;

    qint8 cabinetId = savePackInfo.cellId.left(2).toInt();
    qint8 cellId    = savePackInfo.cellId.right(2).toInt();

    status = lockoperation.queryLock(cabinetId,cellId);
    if(status == 0)
    {
        isOverTime.stop();
        queryLock.stop();

        commitSavePackageInfo();
        commitExceptionInfo();
    }
}

void housewaitpage::commitExceptionInfo(QString notClose)
{
    qint8 notOpenStatus = 2;
    qint8 notCloseStatus = 3;

    QList<NetCellExcept> exceInfoList;
    cellError boxError;
    boxError.reporttime = QDateTime::currentDateTime();
    boxError.status = 1;
    for(int i = 0; i < notOpenBoxList.size(); i++)
    {
        //上报异常
        NetCellExcept exceInfo;
        exceInfo.cellId = notOpenBoxList.at(i);
        exceInfo.flowType = 4; //干洗
        exceInfo.errorType = 1;//打不开

        boxError.cabinetId = notOpenBoxList.at(i).left(2);
        boxError.cellId = notOpenBoxList.at(i);
        boxError.errorType = 1;

        ST_cabinet_cell::instance()->changeCellStatus(notOpenBoxList.at(i),notOpenStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        sendExceptionNote(notOpenBoxList.at(i));

        exceInfoList.append(exceInfo);
    }

    if(!notClose.isEmpty())
    {
        boxError.cabinetId = notClose.left(2);
        boxError.cellId = notClose;
        boxError.errorType = 3;

        ST_cabinet_cell::instance()->changeCellStatus(notClose,notCloseStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);
    }

    if(!exceInfoList.isEmpty())
    {
        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void housewaitpage::commitSavePackageInfo()
{
    qint8 hasGoods = 1;
    savePackInfo.getCode = SCSender::instance()->createRandomCode();

    qDebug() << "house getcode:" << savePackInfo.getCode;
    SCSender::instance()->insertSenderInfo(savePackInfo);
    ST_cabinet_cell::instance()->changeCellHasGoods(savePackInfo.cellId,hasGoods);

    sendHouseNote(savePackInfo);

    QList<SenderDesc_t> infos;
    infos.append(savePackInfo);
    NetworkDesc_t netWorkInfo = SCServerComm::instance()->saveSendDelivery(infos,2);
    SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

    QString mp3file = "./music/house.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);
}

void housewaitpage::sendExceptionNote(QString cell)
{
    //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;


    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 93;

    noteInfo.mMobilePhone = SCTerminal::instance()->terminalInfo.servicePhone;


    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            QUuid uuId = QUuid::createUuid();
            QString strUuId = uuId.toString();
            strUuId = strUuId.remove(QRegExp("[{}-]"));
            strUuId = strUuId.toUpper();
            noteInfo.mSmsLogId = strUuId;
            noteInfo.mStatus = 3;
            noteInfo.mSendTime = QDateTime::currentDateTime();

            SCSmslog::instance()->insertNoteInfo(noteInfo);

            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}
