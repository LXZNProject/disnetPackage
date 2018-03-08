#include "tempstoragewait.h"
#include "ui_tempstoragewait.h"
#include "mainctrl.h"
#include "t_cabinet_cell.h"

#include <QBitmap>
#include <QtCore>
#include <QDebug>
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "t_log_smslog.h"
#include "notethread.h"
#include "driver.h"
#include "servercomm.h"
#include "t_log_network.h"
#include "networkthread.h"

tempStorageWait::tempStorageWait(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tempStorageWait)
{
    ui->setupUi(this);
    initStorageWaitPage();
}

tempStorageWait::~tempStorageWait()
{
    delete ui;
}
void tempStorageWait::initStorageWaitPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->resize(800,600);
    waitSaveTime = 120;

    this->setFixedSize(800,600);
    QPixmap pixmap(":/storage/images/storage/storage_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
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
void tempStorageWait::readWaitInfo(QByteArray param)
{
    waitSaveTime = 120;
    QByteArray data;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> data >> notOpenBoxList >> notCheckBoxList;

    savePackInfo = *(BasicTempStorage *)data.data();

    ui->timeLabel->setText(QString("%1").arg(waitSaveTime));
    ui->doorCabinetId->setText(savePackInfo.cabinetId);
    ui->doorCellId->setText(savePackInfo.cellId.right(2));

    isOverTime.start(1000);
    queryLock.start(3000);

}

void tempStorageWait::overTime()
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

void tempStorageWait::queryLockStatus()
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

void tempStorageWait::commitExceptionInfo(QString notClose)
{
    qint8 notOpenStatus = 2;
    qint8 goodsCheckStatus = 2;
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
        exceInfo.flowType = 3; //临时存放
        exceInfo.errorType = 1;//打不开

        boxError.cabinetId = notOpenBoxList.at(i).left(2);
        boxError.cellId = notOpenBoxList.at(i);
        boxError.errorType = 1;

        ST_cabinet_cell::instance()->changeCellStatus(notOpenBoxList.at(i),notOpenStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        sendExceptionNote(notOpenBoxList.at(i));

        exceInfoList.append(exceInfo);
    }

    for(int j = 0; j < notCheckBoxList.size(); j++)
    {
        boxError.cabinetId = notCheckBoxList.at(j).left(2);
        boxError.cellId = notCheckBoxList.at(j);
        boxError.errorType = 9;

        ST_cabinet_cell::instance()->changeCellGoodsCheckStatus(notCheckBoxList.at(j),goodsCheckStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);
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
void tempStorageWait::sendStorageNote(BasicTempStorage savePackInfo)
{
    //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;
    savePackInfo.messageSentTime = QDateTime::currentDateTime();

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
    noteInfo.mSmsLogId = strUuId;
    noteInfo.mStatus = 3;
    noteInfo.mSendTime = QDateTime::currentDateTime();

    noteInfo.mSmsType = 21;
    noteInfo.mRelPriKey = savePackInfo.storageId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;

    SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);


    noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.getCode).arg(SCTerminal::instance()->terminalInfo.storageHours).arg(\
                SCTerminal::instance()->terminalInfo.storageStep).arg(SCTerminal::instance()->terminalInfo.storageStepMoney);

    if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
    {
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

    SBasicTempStorage::instance()->updateStorageInfo(savePackInfo);
}
void tempStorageWait::commitSavePackageInfo()
{
    qint8 hasGoods = 1;
    savePackInfo.getCode = SBasicTempStorage::instance()->createRandomCode();

    qDebug() << "storage getcode:" << savePackInfo.getCode;
    SBasicTempStorage::instance()->insertStorage(savePackInfo);
    ST_cabinet_cell::instance()->changeCellHasGoods(savePackInfo.cellId,hasGoods);

    sendStorageNote(savePackInfo);

    QList<BasicTempStorage> infos;
    infos.append(savePackInfo);
    NetworkDesc_t netWorkInfo = SCServerComm::instance()->saveTempDelivery(infos);
    SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

    QString mp3file = "./music/storage.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);
}

void tempStorageWait::sendExceptionNote(QString cell)
{
    //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
    noteInfo.mSmsLogId = strUuId;
    noteInfo.mStatus = 3;
    noteInfo.mSendTime = QDateTime::currentDateTime();

    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 93;

    noteInfo.mMobilePhone = SCTerminal::instance()->terminalInfo.servicePhone;


    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
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

