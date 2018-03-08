#include "recharge.h"
#include "ui_recharge.h"
#include "mainctrl.h"
#include "portcomm.h"
#include <string.h>
#include <QByteArray>
#include "t_basic_stored_record.h"
#include <QDateTime>
#include <QTextCodec>
#include <QUuid>
#include "t_log_network.h"
#include "networkthread.h"
#include "servercomm.h"

#include "t_basic_terminal.h"
#include "t_basic_sync_forbidden.h"

recharge::recharge(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recharge)
{
    ui->setupUi(this);
    initPage();
}

recharge::~recharge()
{
    delete ui;
}

void recharge::initPage()
{
    tc =  QTextCodec::codecForName("UTF-8");

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/recharge/images/recharge/background.png")));
    this->setPalette(palette);

    keypage = new keyboard(ui->widget);
    ui->widget->setStyleSheet("background-image: url(:/key/images/keyboard/key_bg.png)");


    openQueryTimer = new QTimer(this);
    connect(openQueryTimer, SIGNAL(timeout()),this, SLOT(readCard_slots()));

    ui->typeLabel->resize(QPixmap(":/recharge/images/recharge/type.png").size());
    ui->typeLabel->setPixmap(QPixmap(":/recharge/images/recharge/type.png"));

    ui->phoneLabel->resize(QPixmap(":/recharge/images/recharge/phoneNum.png").size());
    ui->phoneLabel->setPixmap(QPixmap(":/recharge/images/recharge/phoneNum.png"));

    ui->haveMoneyLabel->resize(QPixmap(":/recharge/images/recharge/hasMoney.png").size());
    ui->haveMoneyLabel->setPixmap(QPixmap(":/recharge/images/recharge/hasMoney.png"));

    ui->rechargeLabel->resize(QPixmap(":/recharge/images/recharge/rechargeMoney.png").size());
    ui->rechargeLabel->setPixmap(QPixmap(":/recharge/images/recharge/rechargeMoney.png"));

    ui->label->resize(QPixmap(":/recharge/images/recharge/lable.png").size());
    ui->label->setPixmap(QPixmap(":/recharge/images/recharge/lable.png"));

    ui->rechargeLineEdit->bindKeyPage(keypage);

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    //    QListView* view = (QListView*)ui->comboBox->view();
    //view->setSpacing(3);
    ui->typeComboBox->setStyleSheet("background-color:transparent");

    QStringList mylist;
    mylist << tc->toUnicode(" ") << tc->toUnicode("超级管理员") << tc->toUnicode("公司管理员") \
           << tc->toUnicode("快递员") << tc->toUnicode("普通用户") << tc->toUnicode("公司维修人员") \
           << tc->toUnicode("公司充值人员") << tc->toUnicode("公司物业人员")  << tc->toUnicode("超级维修员");

    ui->typeComboBox->addItems(mylist);

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));

    ui->typeComboBox->setEnabled(false);

    isSureRecharge = new MsgBox();
    connect(isSureRecharge,SIGNAL(msgBox_Ok()),this,SLOT(myok()));
    InvalidCardInfor.clear();
}

void recharge::readInfor(QByteArray param)
{
    cardInfoList.clear();
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> cardInfoList;
    if(cardInfoList.size() < 6)
    {
        rechargeType = 2;
        ui->rechargeLabel->resize(QPixmap(":/recharge/images/recharge/cardmima.png").size());
        ui->rechargeLabel->setPixmap(QPixmap(":/recharge/images/recharge/cardmima.png"));
    }
    else
    {
        rechargeType = 1;
        ui->rechargeLabel->resize(QPixmap(":/recharge/images/recharge/rechargeMoney.png").size());
        ui->rechargeLabel->setPixmap(QPixmap(":/recharge/images/recharge/rechargeMoney.png"));
    }
    ui->typeComboBox->setCurrentIndex(0);
}

void recharge::timeStart()
{
    openQueryTimer->start(3000);
}

void recharge::timeStop()
{
    openQueryTimer->stop();
}

void recharge::okBtn_slots()
{
    emit resetTime();
    btnEnable(false);
    if(checkInfo())
    {
        // 7|05190001|王五|1377655XXXX|null|100.00|xxxx卡号|uuid
        QString amountStr = ui->rechargeLineEdit->text().trimmed();

        if(amountStr.toDouble() > 10000)
        {
            myMsgBox.showMsgBox("充值金额过大，请重新输入。");
            btnEnable(true);
            return;
        }
        double totalMoney = m_readDataList.at(5).toDouble() + amountStr.toDouble();

        QString writeAmountStr = QString::number(totalMoney,'f',2);

        //写入卡中
        QString infoStr = m_readDataList.at(0)  + "|" +  m_readDataList.at(1) + "|" \
                + m_readDataList.at(2)+ "|" + m_readDataList.at(3) + "|" + m_readDataList.at(4) + "|"  \
                + writeAmountStr + "|"+ m_readDataList.at(6) + "|" + m_readDataList.at(7)  + "|";

        QString tempAdd = QString("null") + QString("|");
        for(int i = 0; i < 7;i++)
        {
            infoStr += tempAdd;
        }


        bool myResult = SCPortComm::instance()->writeCardInfo(infoStr);
        if(myResult)
        {
            BasicStoredRecord tempValue;
            tempValue.price = amountStr;
            tempValue.recordtime = QDateTime::currentDateTime();
            tempValue.storeCardNo = "null";
            tempValue.relM1CardId = m_readDataList.at(6);
            tempValue.relPhone = m_readDataList.at(3);
            tempValue.optM1CardId = cardInfoList.at(6);
            tempValue.optPhone = cardInfoList.at(3);
            //插入数据库
            SBasicStoredRecord::instance()->insertNote(tempValue);
            //发送后台
            NetworkDesc_t noteInfo;
            QString temp =  QString::null;

            QUuid uuId = QUuid::createUuid();
            QString strUuId = uuId.toString();
            strUuId = strUuId.remove(QRegExp("[{}-]"));
            strUuId = strUuId.toUpper();

            QStringList sendInfo;
            sendInfo << m_readDataList.at(3) << m_readDataList.at(6) << m_readDataList.at(7) \
                            <<  cardInfoList.at(7) << QString::number(1) << amountStr << writeAmountStr \
                            <<  temp  <<  temp  << strUuId;


            SCServerComm::instance()->localRecharge(noteInfo,sendInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);

            clearInfo();
            myMsgBox.showMsgBox("充值成功");

            SCMainCtrl::instance()->enterStatus(ST_HOME,"");
        }
        else
        {
            myMsgBox.showMsgBox("充值失败，请重试。");
        }
    }
    btnEnable(true);
}

void recharge::quitBtn_slots()
{
    clearInfo();
    timeStop();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void recharge::readCard_slots()
{
    QStringList myReadDataList;

    if(SCPortComm::instance()->queryCardInfo(myReadDataList) >= 0)
    {
        dealReadCardInfo(myReadDataList);
    }
}

void recharge::myok()
{
    emit resetTime();
    QString cardInfor = ui->rechargeLineEdit->text().trimmed();

    QString amountStr = rechargeCardInfoList.at(3);
    double totalMoney = m_readDataList.at(5).toDouble() + amountStr.toDouble();
    QString writeAmountStr = QString::number(totalMoney,'f',2);

    //写入卡中
    QString infoStr = m_readDataList.at(0)  + "|" +  m_readDataList.at(1) + "|" \
            + m_readDataList.at(2)+ "|" + m_readDataList.at(3) + "|" + m_readDataList.at(4) + "|"  \
            + writeAmountStr + "|"+ m_readDataList.at(6) + "|" + m_readDataList.at(7)  + "|";

    QString tempAdd = QString("null") + QString("|");
    for(int i = 0; i < 7;i++)
    {
        infoStr += tempAdd;
    }


    bool myResult = SCPortComm::instance()->writeCardInfo(infoStr);
    if(myResult)
    {
        //记录当前的卡信息
        InvalidCardInfor.append(cardInfor);


        BasicStoredRecord tempValue;
        tempValue.price = amountStr;
        tempValue.recordtime = QDateTime::currentDateTime();
        tempValue.storeCardNo = rechargeCardInfoList.at(0);
        tempValue.relM1CardId = m_readDataList.at(6);
        tempValue.relPhone = m_readDataList.at(3);
        tempValue.optM1CardId = "";
        tempValue.optPhone = "";
        //插入数据库
        SBasicStoredRecord::instance()->insertNote(tempValue);

        //发送后台充值确认信息
        QUuid uuId = QUuid::createUuid();
        QString strUuId = uuId.toString();
        strUuId = strUuId.remove(QRegExp("[{}-]"));
        strUuId = strUuId.toUpper();

        QString tempStr = "";

        QStringList info;
        info << rechargeCardInfoList.at(0) << rechargeCardInfoList.at(1) << m_readDataList.at(7) << m_readDataList.at(6) \
                << m_readDataList.at(3) << rechargeCardInfoList.at(2) << rechargeCardInfoList.at(3) << writeAmountStr \
                << tempStr << tempStr << strUuId;

        passwordRechargeRet respone;
        SCServerComm::instance()->passwordRechargeSynchronous(info,&respone);
        if(respone.resultCode() != "0000")
        {
            NetworkDesc_t noteInfo;
            SCServerComm::instance()->passwordRechargeAsynchronous(noteInfo,info);
            SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);
        }

        clearInfo();
        myMsgBox.showMsgBox("充值成功");

        SCMainCtrl::instance()->enterStatus(ST_HOME,"");
    }
    else
    {
        myMsgBox.showMsgBox("充值失败，请重试。");
    }
    btnEnable(true);
}

bool recharge::checkInfo()
{
    QRegExp rxcellphone("^1\\d{10}");

    QString info;
    bool myResult = true;
    if((!rxcellphone.exactMatch(ui->phoneLineEdit->text().trimmed())))
    {
        myResult =false;
        info = "用户手机号码不正确。";
        myMsgBox.showMsgBox(info);
    }
    else if(ui->rechargeLineEdit->text().trimmed().size() < 1)
    {
        myResult =false;
        info = "充值金额不对或者卡号错误，请检查后重新充值。";
        myMsgBox.showMsgBox(info);
    }

    //充值卡校验
    if(rechargeType == 2)
    {
        QString cardNumStr = ui->rechargeLineEdit->text().trimmed();
        for(int i = 0; i < InvalidCardInfor.size();i++)
        {
            if(cardNumStr == InvalidCardInfor.at(i))
            {
                myMsgBox.showMsgBox("此卡被充值过，不能再次使用。");
                return false;
            }
        }

        queryStoreCardRet respone;
        SCServerComm::instance()->queryStoreCard(cardNumStr,&respone);

        if(respone.resultCode() != "0000")
        {
            if(respone.resultMessage().isEmpty())
            {
                myMsgBox.showMsgBox("网络错误，请检查网络后重试。");
            }
            else
            {
                myMsgBox.showMsgBox(respone.resultMessage(),false);
            }
            myResult =false;
        }
        else
        {
            rechargeCardInfoList.clear();
            rechargeCardInfoList << respone.cardNo() << respone.cardPasswd() << respone.cardType() <<respone.cardValue();
            info = QString("充值卡金额为%1元,确定要为用户卡充值吗？").arg(respone.cardValue());
            isSureRecharge->showMsgBox(info,true,2);
            myResult = false;
        }
    }
    return myResult;
}

void recharge::clearInfo()
{
    m_readDataList.clear();
    ui->haveMoneyLineEdit->clear();
    ui->rechargeLineEdit->clear();
    ui->phoneLineEdit->clear();
    rechargeCardInfoList.clear();
}

void recharge::dealReadCardInfo(QStringList myReadDataList)
{
    if(myReadDataList.size() < 8)
    {
        myMsgBox.showMsgBox("卡信息不正确。");
        return;
    }
    if(SBalckList::instance()->lossUserInfo.contains(myReadDataList.at(7)))
    {
        myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
        return ;
    }
    if(!SCTerminal::instance()->terminalInfo.companyId.startsWith(myReadDataList.at(1)))
    {
        myMsgBox.showMsgBox("不是该公司的会员卡，不能充值。");
        return;
    }

    if(rechargeType == 1)//本地充值
    {
        if(myReadDataList.at(6) != cardInfoList.at(6))
        {
            timeStop();
            m_readDataList = myReadDataList;

            int temp = myReadDataList.at(0).toInt();
            ui->typeComboBox->setCurrentIndex(temp);
            ui->phoneLineEdit->setText(myReadDataList.at(3));
            ui->haveMoneyLineEdit->setText(myReadDataList.at(5));
        }
    }
    else//密保卡充值
    {
        timeStop();
        m_readDataList = myReadDataList;

        int temp = myReadDataList.at(0).toInt();
        ui->typeComboBox->setCurrentIndex(temp);
        ui->phoneLineEdit->setText(myReadDataList.at(3));
        ui->haveMoneyLineEdit->setText(myReadDataList.at(5));
    }
}

void recharge::btnEnable(bool value)
{
    ui->okBtn->setEnabled(value);
    ui->quitBtn->setEnabled(value);
}
