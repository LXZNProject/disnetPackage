#include "waitconfirmpage.h"
#include "ui_waitconfirmpage.h"
#include "mainctrl.h"

#include "t_cabinet_cell.h"
#include "t_basic_terminal.h"
#include "t_log_smslog.h"
#include "t_log_template.h"
#include "notethread.h"
#include "servercomm.h"
#include "t_log_network.h"
#include "networkthread.h"
#include "t_basic_cell_order.h"
#include <QDataStream>
#include <QUuid>
#include <QDebug>

waitconfirmpage::waitconfirmpage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::waitconfirmpage)
{
    ui->setupUi(this);

    initPage();

}

waitconfirmpage::~waitconfirmpage()
{
    delete ui;
}

void waitconfirmpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/waitconfirm/images/waitConfirm/waitconfirm_bg.png")));
    this->setPalette(palette);

    ui->label->setFixedSize(QPixmap(":/waitconfirm/images/waitConfirm/word.png").size());
    ui->label->setPixmap(QPixmap(":/waitconfirm/images/waitConfirm/word.png"));

    ui->packageIdLabel->setFixedSize(QPixmap(":/waitconfirm/images/waitConfirm/word2.png").size());
    ui->packageIdLabel->setPixmap(QPixmap(":/waitconfirm/images/waitConfirm/word1.png"));

    ui->phoneLabel->setFixedSize(QPixmap(":/waitconfirm/images/waitConfirm/word2.png").size());
    ui->phoneLabel->setPixmap(QPixmap(":/waitconfirm/images/waitConfirm/word2.png"));

    ui->cellLabel->setFixedSize(QPixmap(":/waitconfirm/images/waitConfirm/word2.png").size());
    ui->cellLabel->setPixmap(QPixmap(":/waitconfirm/images/waitConfirm/word3.png"));

    ui->packageIdLineEdit->setReadOnly(true);
    ui->packageIdLineEdit->setFixedSize(200,30);
    ui->phoneLineEdit->setReadOnly(true);
    ui->phoneLineEdit->setFixedSize(200,30);
    ui->cellIdLineEdit->setReadOnly(true);
    ui->cellIdLineEdit->setFixedSize(200,30);

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/waitconfirm/images/waitConfirm/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/waitconfirm/images/waitConfirm/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/waitconfirm/images/waitConfirm/cancelBtn_bg.png"));
    ui->cancelBtn->setMinimumSize(QPixmap(":/waitconfirm/images/waitConfirm/cancelBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->okBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slot()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slot()));
    connect(&confirmMsgBox,SIGNAL(msgBox_Ok()),this,SLOT(confirmPack_slot()));
}

void waitconfirmpage::readwaitconfirminfo(QByteArray data)
{
    QList<DelivertDesc_t> deliveryInfo;


    QDataStream in(&data,QIODevice::ReadOnly);
    in >> cardInfoList;

    SCDelivery::instance()->selectSaveDeliveryInfo(cardInfoList.at(6),deliveryInfo);

    delivery = deliveryInfo.at(0);


    ui->packageIdLineEdit->setText(delivery.packageId);
    ui->phoneLineEdit->setText(delivery.mobilephone);
    ui->cellIdLineEdit->setText(delivery.cellId);
}

void waitconfirmpage::sendNote(DelivertDesc_t &deliveryInfo)
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
       // noteInfo.mSmsContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
       //         deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(deliveryInfo.cellId).arg(tollContent);
    }
    else
    {
        if(2 == tariffStatus)
        {
            tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
        }
       // noteInfo.mSmsContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
       //             deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(deliveryInfo.cellId).arg(tollContent);
    }

    noteInfo.mSmsContent = SCNoteTemplate::instance()->getSaveNoteInfo(smsContent,deliveryInfo.expCompanyName,SCTerminal::instance()->terminalInfo.terminalName,\
                                     deliveryInfo.getCode,QString::number(SCTerminal::instance()->terminalInfo.expireHours),deliveryInfo.cellId,tollContent);

    SCSmslog::instance()->insertNoteInfo(noteInfo);
    SCNoteThread::instance()->sendNoteEvent(noteInfo);

}

void waitconfirmpage::sendNetNote(DelivertDesc_t &deliveryInfo)
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
      // noteInfo.mContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
      //             deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(deliveryInfo.cellId).arg(tollContent);
   }
   else
   {
       if(2 == tariffStatus)
       {
           tollContent = QString::fromUtf8("，逾期%1元递增").arg(SCTerminal::instance()->terminalInfo.expireStepMoney);
       }
     //  noteInfo.mContent = smsContent.arg(deliveryInfo.expCompanyName).arg(SCTerminal::instance()->terminalInfo.terminalName).arg(\
     //              deliveryInfo.getCode).arg(SCTerminal::instance()->terminalInfo.expireHours).arg(deliveryInfo.cellId).arg(tollContent);
   }

   noteInfo.mSmsContent = SCNoteTemplate::instance()->getSaveNoteInfo(smsContent,deliveryInfo.expCompanyName,SCTerminal::instance()->terminalInfo.terminalName,\
                                    deliveryInfo.getCode,QString::number(SCTerminal::instance()->terminalInfo.expireHours),deliveryInfo.cellId,tollContent);

   SCServerComm::instance()->sendSms(noteInfo);
   SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);
}

void waitconfirmpage::cancelBtn_slot()
{
    emit resetTime();

    confirmMsgBox.showMsgBox("您确定此箱门中没有存放派件？",true,2);

}

void waitconfirmpage::confirmPack_slot()
{
    emit resetTime();
    ui->okBtn->setEnabled(false);
    ui->cancelBtn->setEnabled(false);

    qint8 hasGoods = 2;

    delivery.status = 4;

    SCDelivery::instance()->updateDeliveryInfo(delivery);
    ST_cabinet_cell::instance()->changeCellHasGoods(delivery.cellId,hasGoods);

    NetworkDesc_t netWorkInfo = SCServerComm::instance()->confirmDelivery(delivery,2);
    SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

    qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
    if((tariffType == 2 || tariffType == 4) && ((cardInfoList.at(5).toDouble() < 0) || (cardInfoList.at(5).toDouble() == 0 && !STCellOrder::instance()->isExpOrderInfo(cardInfoList.at(6)))))
    {
        QString info =  "余额不足，请先充值！";
        myMsgBox.showMsgBox(info);
    }
    else
    {
        QByteArray     outData;
        QDataStream out(&outData,QIODevice::WriteOnly);

        out << QString("HOME") << cardInfoList;
        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);
    }
    ui->okBtn->setEnabled(true);
    ui->cancelBtn->setEnabled(true);
}




void waitconfirmpage::okBtn_slot()
{
    emit resetTime();
    ui->okBtn->setEnabled(false);
    ui->cancelBtn->setEnabled(false);

    delivery.status = 1;

    if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
    {
        sendNote(delivery);
    }
    else
    {
        sendNetNote(delivery);
    }
    SCDelivery::instance()->updateDeliveryInfo(delivery);

    NetworkDesc_t netWorkInfo = SCServerComm::instance()->confirmDelivery(delivery,1);
    SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

    qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
    if((tariffType == 2 || tariffType == 4) && ((cardInfoList.at(5).toDouble() < 0) || (cardInfoList.at(5).toDouble() == 0 && !STCellOrder::instance()->isExpOrderInfo(cardInfoList.at(6)))))
    {
        QString info =  "余额不足，请先充值！";
        myMsgBox.showMsgBox(info);
    }
    else
    {
        QByteArray     outData;
        QDataStream out(&outData,QIODevice::WriteOnly);

        out << QString("HOME") << cardInfoList;
        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);
    }

    ui->okBtn->setEnabled(true);
    ui->cancelBtn->setEnabled(true);

}

void waitconfirmpage::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
