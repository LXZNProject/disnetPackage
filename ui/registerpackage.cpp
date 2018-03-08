#include "registerpackage.h"
#include "ui_registerpackage.h"
#include "mainctrl.h"
#include "portcomm.h"
#include "t_basic_terminal.h"
#include <string.h>
#include <QByteArray>
#include <QMessageBox>
#include <QUuid>

#include "expcompany.h"
#include "servercomm.h"
#include "t_log_network.h"
#include "networkthread.h"
#include "shaEncy.h"

extern  QStringList myCompanyNameList;

registerPackage::registerPackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registerPackage)
{
    ui->setupUi(this);
    initPage();

}

registerPackage::~registerPackage()
{
    delete ui;
}

void registerPackage::initPage()
{
    tc =  QTextCodec::codecForName("UTF-8");
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/register/images/register/background.png")));
    this->setPalette(palette);

    keypage = new keyboard(ui->widget);
    ui->widget->setStyleSheet("background-image: url(:/key/images/keyboard/key_bg.png)");

    ui->label->resize(QPixmap(":/register/images/register/lable.png").size());
    ui->label->setPixmap(QPixmap(":/register/images/register/lable.png"));


    ui->typeLabel->resize(QPixmap(":/register/images/register/type.png").size());
    ui->typeLabel->setPixmap(QPixmap(":/register/images/register/type.png"));

    ui->phoneLabel->resize(QPixmap(":/register/images/register/phone.png").size());
    ui->phoneLabel->setPixmap(QPixmap(":/register/images/register/phone.png"));
    ui->phoneLineEdit->bindKeyPage(keypage);

    ui->companyNameLabel->resize(QPixmap(":/register/images/register/companyName.png").size());
    ui->companyNameLabel->setPixmap(QPixmap(":/register/images/register/companyName.png"));

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

    QStringList tempComanyList;
    tempComanyList = myCompanyNameList;
    tempComanyList << tc->toUnicode("唯品会") << tc->toUnicode("快捷") << tc->toUnicode("增益")\
                      << tc->toUnicode("优速快递");

    ui->companyNameComboBox->addItems(tempComanyList);

    connect(ui->typeComboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(typeComboBoxChanged(QString)));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));

    myMsgBox = new MsgBox();
    waringMsgBox =new MsgBox();
    connect(myMsgBox,SIGNAL(msgBox_Ok()),this,SLOT(myok()));
}

void registerPackage::readInfo(QByteArray param)
{
    cardInfoList.clear();
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> cardInfoList;

    typeControl = cardInfoList.at(0);
    QStringList mylist;

    if((typeControl == "2") || (typeControl == "1"))
    {
        mylist << tc->toUnicode("公司管理员") << tc->toUnicode("公司维修人员") << tc->toUnicode("公司充值人员") \
               << tc->toUnicode("公司物业人员") << tc->toUnicode("快递员") << tc->toUnicode("普通用户");
    }
    else if(typeControl == "7")
    {
        mylist << tc->toUnicode("快递员") << tc->toUnicode("普通用户");
    }
    //mylist << tc->toUnicode("超级管理员") << tc->toUnicode("超级维修员") << tc->toUnicode("公司管理员") << tc->toUnicode("公司维修人员") \
    //           << tc->toUnicode("公司充值人员") << tc->toUnicode("公司物业人员") << tc->toUnicode("快递员") << tc->toUnicode("普通用户");
    ui->typeComboBox->addItems(mylist);
    ui->phoneLineEdit->setFocus();
}

void registerPackage::okBtn_slots()
{
    emit resetTime();
    if(checkInfo())
    {
        QStringList tempList;
        if(SCPortComm::instance()->queryCardInfo(tempList) == -1)
        {
            waringMsgBox->showMsgBox("读卡失败，请插入要注册的卡重试。");
            return;
        }

        if(tempList.size() > 6)
        {
            readCardInfoList = tempList;
            myMsgBox->showMsgBox("卡中有用户数据，确定要覆盖此卡中信息吗？",true,2);
        }
        else
        {
            myok();
        }
    }
}

void registerPackage::quitBtn_slots()
{
    clearInfo();
    ui->typeComboBox->clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void registerPackage::typeComboBoxChanged(QString temp)
{
    if(temp == QString::fromUtf8("快递员"))
    {
        ui->companyNameLabel->show();
        ui->companyNameComboBox->show();
    }
    else
    {
        ui->companyNameLabel->hide();
        ui->companyNameComboBox->hide();
    }
}

bool registerPackage::checkInfo()
{
    QRegExp rxcellphone("^1\\d{10}");

    QString phoneNum = ui->phoneLineEdit->text().trimmed();

    if(!rxcellphone.exactMatch(phoneNum))
    {
        waringMsgBox->showMsgBox("手机号码输入错误，检查后注册。");
        return false;
    }
    return true;
}

void registerPackage::clearInfo()
{
    ui->phoneLineEdit->clear();
    myCardNumStr.clear();
    typeControl.clear();
    readCardInfoList.clear();
}

int registerPackage::getIndexFromString()
{
    //mylist << "公司管理员" << "公司维修人员"<<
    //"公司充值人员" << "公司物业人员" << "快递员" << "普通用户";
    QString tempStr = ui->typeComboBox->currentText();
    if(tempStr == QString::fromUtf8("公司管理员"))
    {
        return 2;
    }
    else if(tempStr == QString::fromUtf8("公司维修人员"))
    {
         return 5;
    }
    else if(tempStr == QString::fromUtf8("公司充值人员"))
    {
         return 6;
    }
    else if(tempStr == QString::fromUtf8("公司物业人员"))
    {
         return 7;
    }
    else if(tempStr == QString::fromUtf8("快递员"))
    {
         return 3;
    }
    else if(tempStr == QString::fromUtf8("普通用户"))
    {
         return 4;
    }
    else if(tempStr == QString::fromUtf8("超级管理员"))
    {
         return 1;
    }
    else if(tempStr == QString::fromUtf8("超级维修员"))
    {
         return 8;
    }
    return 4;
}

bool registerPackage::readCard()
{
    QString cardNumStr;
    if(SCPortComm::instance()->readCardNumber(cardNumStr))
    {
        if(cardNumStr.size() > 9)
        {
            myCardNumStr = cardNumStr;
            return true;
        }
    }
    return false;
}

void registerPackage::myok()
{
    emit resetTime();
    if(!readCard())
    {
        waringMsgBox->showMsgBox("读取卡号失败，请重试。");
        return;
    }

    QString phoneNum = ui->phoneLineEdit->text().trimmed();
    QString companyName = ui->companyNameComboBox->currentText().trimmed();
    QString amountStr = "0.00";


   QString  companyNumStr  = SCTerminal::instance()->terminalInfo.companyId;

    if(ui->typeComboBox->currentText().trimmed() != QString::fromUtf8("快递员"))
    {
        companyName = "null";
    }

    QByteArray companyNameByteArray = companyName.toUtf8();
    companyNameByteArray = companyNameByteArray.toHex();


    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();
  //写入卡中
    QString info = QString::number(getIndexFromString())  + QString("|") \
            +  companyNumStr + QString("|") +  QString("null") + QString("|") +  phoneNum + QString("|")   \
            + companyNameByteArray + QString("|") +  amountStr +  QString("|") + myCardNumStr  +  QString("|")  \
            + strUuId + QString("|");

    QString tempAdd = QString("null") + QString("|");

    for(int i = 0; i < 7;i++)
    {
        info += tempAdd;
    }

    bool myResult = SCPortComm::instance()->writeCardInfo(info);
    if(myResult)
    {
        //发送后台
        NetworkDesc_t noteInfo;

        QString tempUUID;
        if(readCardInfoList.size() < 8)
        {
            tempUUID = "";
        }
        else
        {
            tempUUID = readCardInfoList.at(7);
        }
        QStringList sendInfo;
        sendInfo << QString::number(getIndexFromString()) << phoneNum << companyName \
                        << myCardNumStr << strUuId << cardInfoList.at(7) << tempUUID;

        SCServerComm::instance()->userRegist(noteInfo,sendInfo);
        SCNetworkThread::instance()->sendNetWorkEvent(noteInfo);

        waringMsgBox->showMsgBox("注册成功。");
        clearInfo();
    }
    else
    {
        waringMsgBox->showMsgBox("注册失败，请重试。");
    }
}
