#include "savepackagelistpage.h"
#include "ui_savepackagelistpage.h"

#include "mainctrl.h"
#include "driver.h"
#include "t_cabinet_cell.h"
#include "t_temp_consume_record.h"
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "t_log_smslog.h"
#include "t_log_network.h"
#include "portcomm.h"
#include "notethread.h"
#include "networkthread.h"
#include "servercomm.h"

#include <QHeaderView>
#include <QBitmap>
#include <QUuid>

//快递员存件的取件码前两位相加 9


savepackagelistpage::savepackagelistpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::savepackagelistpage)
{
    ui->setupUi(this);
    initListPage();
}

savepackagelistpage::~savepackagelistpage()
{
    delete ui;
}

void savepackagelistpage::initListPage()
{
    this->resize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/save/images/saveList/list_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    QStringList list;
    list<< "序号" << "快递单号" << "用户手机号" << "柜号" << "箱号";

    showModel = new takeViewModel();
    showModel->setHorizontalHeaderList(list);

    //水平模型头充满水平区域
    QHeaderView *header;
    header = ui->savetableView->horizontalHeader();
    header->setResizeMode(QHeaderView::Stretch);

    //ui->savetableView->setStyleSheet();

    ui->savetableView->verticalHeader()->hide();
    ui->savetableView->sizeHint();

    ui->nextBtn->setFlat(true);
    ui->nextBtn->setFocusPolicy(Qt::NoFocus);
    ui->nextBtn->setIcon(QIcon(":/save/images/saveList/nextBtn.png"));
    ui->nextBtn->setMinimumSize(QPixmap(":/save/images/saveList/nextBtn.png").size());
    ui->nextBtn->setIconSize(ui->nextBtn->size());
    ui->nextBtn->setStyleSheet("background-color:transparent");

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/save/images/saveList/okBtn.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/save/images/saveList/okBtn.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->nextBtn,SIGNAL(clicked()),this,SLOT(nextBtn_slots()));
}

void savepackagelistpage::sendNote(DelivertDesc_t savePackInfo)
{
     //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;
    QString tollContent = "";
    noteInfo.mRelPriKey = savePackInfo.deliveryId;
    savePackInfo.messageSentTime = QDateTime::currentDateTime();

    if(savePackInfo.status == 1)
    {
        noteInfo.mSmsType = 1;
        noteInfo.mMobilePhone = savePackInfo.mobilephone;
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
          //  noteInfo.mSmsContent = smsContent.arg(savePackInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
          //              savePackInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(savePackInfo.cellId).arg(tollContent);
        }
        else
        {        
            if(2 == tariffStatus)
            {
                tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
            }
          //  noteInfo.mSmsContent = smsContent.arg(savePackInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
           //             savePackInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(savePackInfo.cellId).arg(tollContent);
        }
        noteInfo.mSmsContent = SCNoteTemplate::instance()->getSaveNoteInfo(smsContent,savePackInfo.expCompanyName,SCTerminal::instance()->terminalInfo.terminalName,\
                                         savePackInfo.getCode,QString::number(SCTerminal::instance()->terminalInfo.expireHours),savePackInfo.cellId,tollContent);
    }
    else if(savePackInfo.status == 3)
    {
        noteInfo.mSmsType = 4;
        noteInfo.mMobilePhone = savePackInfo.expMobilePhone;
        noteInfo.mSendCount = 0;

        SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);

        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName);
    }

    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
    noteInfo.mSmsLogId = strUuId;
    noteInfo.mStatus = 3;
    noteInfo.mSendTime = QDateTime::currentDateTime();

    SCSmslog::instance()->insertNoteInfo(noteInfo);
    SCNoteThread::instance()->sendNoteEvent(noteInfo);
    SCDelivery::instance()->updateDeliveryInfo(savePackInfo);
}

void savepackagelistpage::sendNetNote(DelivertDesc_t savePackInfo)
{
    //调用短信发送接口
   NetworkDesc_t noteInfo;
   QString smsContent;
   QString tollContent = "";
   noteInfo.mRelPriKey = savePackInfo.deliveryId;
   savePackInfo.messageSentTime = QDateTime::currentDateTime();

   if(savePackInfo.status == 1)
   {
       noteInfo.mSmsType = 1;
       noteInfo.mPhone = savePackInfo.mobilephone;


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
          // noteInfo.mContent = smsContent.arg(savePackInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
          //             savePackInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(savePackInfo.cellId).arg(tollContent);
       }
       else
       {
           if(2 == tariffStatus)
           {
               tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
           }
          // noteInfo.mContent = smsContent.arg(savePackInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
          //            savePackInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(savePackInfo.cellId).arg(tollContent);
       }
       noteInfo.mContent = SCNoteTemplate::instance()->getSaveNoteInfo(smsContent,savePackInfo.expCompanyName,SCTerminal::instance()->terminalInfo.terminalName,\
                                 savePackInfo.getCode,QString::number(SCTerminal::instance()->terminalInfo.expireHours),savePackInfo.cellId,tollContent);
   }
   else if(savePackInfo.status == 3)
   {
       noteInfo.mSmsType = 4;
       noteInfo.mPhone = savePackInfo.expMobilePhone;

       SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent);

       noteInfo.mContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName);
   }

   SCServerComm::instance()->sendSms(noteInfo);
   SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);
   SCDelivery::instance()->updateDeliveryInfo(savePackInfo);
}


void savepackagelistpage::readListInfo(QByteArray param)
{
    setBtnEnable(true);
    QString isShowList;
    QByteArray data;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> isShowList >> cardInfoList >> notOpenBoxList >> notCheckBoxList >> notCloseBox >> data;

    DelivertDesc_t savePackInfo = *(DelivertDesc_t *)data.data();

    qDebug() << "readListInfo" << savePackInfo.cellId;
    savePackInfoList.append(savePackInfo);
    QStringList tempList;
    tempList.append(QString("%1").arg(savePackInfoList.size()));
    tempList.append(savePackInfo.packageId);
    tempList.append(savePackInfo.mobilephone);
    tempList.append(savePackInfo.cabinetId);
    tempList.append(savePackInfo.cellId.right(2));
    modelData.append(tempList);

    showModel->setModelData(modelData);
    showModel->refrushModel();
    ui->savetableView->setModel(showModel);

    commitSavePackageInfo();

    if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
    {
        sendNote(savePackInfoList.last());
    }
    else
    {
        sendNetNote(savePackInfoList.last());
    }
}

void savepackagelistpage::setExceptionBoxInfo(QStringList card,QStringList open, QStringList check, QString close)
{
    cardInfoList = card;
    notOpenBoxList = open;
    notCheckBoxList = check;
    notCloseBox  = close;
}

void savepackagelistpage::okBtn_slots()
{
    setBtnEnable(false);
    emit timeStop();

    commitExceptionInfo();

    SCMainCtrl::instance()->enterStatus(ST_HOME,"");   
}

void savepackagelistpage::nextBtn_slots()
{
    setBtnEnable(false);
    emit timeStop();
    QString page = "LIST";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << page;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,data);
}

void savepackagelistpage::setBtnEnable(bool status)
{
    ui->nextBtn->setEnabled(status);
    ui->okBtn->setEnabled(status);
}

void savepackagelistpage::commitSavePackageInfo()
{
    qint8 hasGoods = 1;
    savePackInfoList.last().getCode = SCDelivery::instance()->createRandomCode();

    qDebug() << "save getcode:" << savePackInfoList.last().getCode;
    SCDelivery::instance()->insertDeliveryInfo(savePackInfoList.last());

    ST_cabinet_cell::instance()->changeCellHasGoods(savePackInfoList.last().cellId,hasGoods);
}

void savepackagelistpage::commitExceptionInfo()
{
    qint8 notOpenStatus = 2;
    qint8 goodsCheckStatus = 2;
    qint8 notCloseStatus = 3;
    double payExp = 0.00; //需扣的钱

    cellError boxError;
    boxError.reporttime = QDateTime::currentDateTime();
    boxError.status = 1;

    QList<NetCellExcept> exceInfoList;

    QString mp3file = "./music/save.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);

    for(int i = 0; i < notOpenBoxList.size(); i++)
    {
        //上报异常
        NetCellExcept exceInfo;
        exceInfo.cellId = notOpenBoxList.at(i);
        exceInfo.flowType = 1; //存件
        exceInfo.errorType = 1;//打不开

        boxError.cabinetId = notOpenBoxList.at(i).left(2);
        boxError.cellId = notOpenBoxList.at(i);
        boxError.errorType = 1;

        ST_cabinet_cell::instance()->changeCellStatus(notOpenBoxList.at(i),notOpenStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            sendExceptionNote(notOpenBoxList.at(i));
        }
        else
        {
            sendNetExceptionNote(notOpenBoxList.at(i));
        }

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

    if(!notCloseBox.isEmpty())
    {
        boxError.cabinetId = notCloseBox.left(2);
        boxError.cellId = notCloseBox;
        boxError.errorType = 3;

        ST_cabinet_cell::instance()->changeCellStatus(notCloseBox,notCloseStatus);

        ST_cabinet_cell::instance()->insertCellError(boxError);
    }

    for(int k = 0; k < savePackInfoList.size(); k++)
    {
        payExp += savePackInfoList.at(k).expressPay;
    }

    if(payExp > 0)
    {
        QString writeCardInfo;
        QStringList tempCardList = cardInfoList;
        double oweExp = cardInfoList.at(5).toDouble() - payExp;
        tempCardList.replace(5,QString::number(oweExp,'f',2));

        for(int i = 0; i < tempCardList.size(); i++)
        {
            if(4 == i)
            {
                QByteArray byteAry = tempCardList.at(i).toUtf8();
                byteAry = byteAry.toHex();
                writeCardInfo = writeCardInfo + byteAry + "|";
            }
            else
            {
                writeCardInfo = writeCardInfo + tempCardList.at(i) + "|";
            }
        }

        QStringList tempRead;
        //保存发送给后台的信息
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();

        if(SCPortComm::instance()->queryCardInfo(tempRead) > 0 && tempRead.contains(cardInfoList.at(7)))
        {
            if(!SCPortComm::instance()->writeCardInfo(writeCardInfo))
            {
                //临时扣费
                TempRecordDesc_t tempRecordInfo;
                tempRecordInfo.m1Card = cardInfoList.at(6);  //M1卡号
                tempRecordInfo.companyId = cardInfoList.at(1);
                tempRecordInfo.createTime = QDateTime::currentDateTime();
                tempRecordInfo.curCash = payExp;   //欠款钱数
                tempRecordInfo.usrUuid = cardInfoList.at(7);
                tempRecordInfo.cmmUuid = strUuId;
                STempRecord::instance()->insertTempRecord(tempRecordInfo);

                NetworkDesc_t debtNetWorkInfo = SCServerComm::instance()->reportDebtRecord(tempRecordInfo,cardInfoList);
                SCNetworkThread::instance()->sendNetWorkEvent(debtNetWorkInfo);
            }
        }
        else
        {
            //临时扣费
            TempRecordDesc_t tempRecordInfo;
            tempRecordInfo.m1Card = cardInfoList.at(6);  //M1卡号
            tempRecordInfo.companyId = cardInfoList.at(1);
            tempRecordInfo.createTime = QDateTime::currentDateTime();
            tempRecordInfo.curCash = payExp;   //欠款钱数
            tempRecordInfo.usrUuid = cardInfoList.at(7);
            tempRecordInfo.cmmUuid = strUuId;
            STempRecord::instance()->insertTempRecord(tempRecordInfo);

            NetworkDesc_t debtNetWorkInfo = SCServerComm::instance()->reportDebtRecord(tempRecordInfo,cardInfoList);
            SCNetworkThread::instance()->sendNetWorkEvent(debtNetWorkInfo);
        }
    }

    if(!savePackInfoList.isEmpty())
    {
        NetworkDesc_t netWorkInfo = SCServerComm::instance()->saveDelivery(savePackInfoList,cardInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);
    }

    if(!exceInfoList.isEmpty())
    {
        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    modelData.clear();
    savePackInfoList.clear();
}


void savepackagelistpage::sendExceptionNote(QString cell)
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

        SCSmslog::instance()->insertNoteInfo(noteInfo);
        SCNoteThread::instance()->sendNoteEvent(noteInfo);
    }
}

void savepackagelistpage::sendNetExceptionNote(QString cell)
{
    //调用短信发送接口
    NetworkDesc_t noteInfo;
    QString smsContent;


    noteInfo.mSmsType = 93;

    noteInfo.mPhone = SCTerminal::instance()->terminalInfo.servicePhone;


    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell);
        SCServerComm::instance()->sendSms(noteInfo);
        SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);
    }
}
