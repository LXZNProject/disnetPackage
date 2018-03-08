#include "rechargedialog.h"
#include "ui_rechargedialog.h"
#include "portcomm.h"
#include "t_basic_terminal.h"
#include "t_basic_sync_forbidden.h"
#include "mainctrl.h"
#include <QDesktopWidget>
rechargeDialog::rechargeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rechargeDialog)
{
    ui->setupUi(this);
    initPage();
}

rechargeDialog::~rechargeDialog()
{
    delete ui;
}

void rechargeDialog::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/recharge/images/recharge/background.png")));
    this->setPalette(palette);
    //this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);


    ui->label->resize(QPixmap(":/recharge/images/recharge/choiceType.png").size());
    ui->label->setPixmap(QPixmap(":/recharge/images/recharge/choiceType.png"));

    ui->localRechargeBtn->setFlat(true);
    ui->localRechargeBtn->setFocusPolicy(Qt::NoFocus);
    ui->localRechargeBtn->setIcon(QIcon(":/recharge/images/recharge/local.png"));
    ui->localRechargeBtn->setMinimumSize(QPixmap(":/recharge/images/recharge/local.png").size());
    ui->localRechargeBtn->setIconSize(ui->localRechargeBtn->size());
    ui->localRechargeBtn->setStyleSheet("background-color:transparent");

    ui->cardRechargeBtn->setFlat(true);
    ui->cardRechargeBtn->setFocusPolicy(Qt::NoFocus);
    ui->cardRechargeBtn->setIcon(QIcon(":/recharge/images/recharge/card.png"));
    ui->cardRechargeBtn->setMinimumSize(QPixmap(":/recharge/images/recharge/card.png").size());
    ui->cardRechargeBtn->setIconSize(ui->cardRechargeBtn->size());
    ui->cardRechargeBtn->setStyleSheet("background-color:transparent");


    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");


    connect(ui->localRechargeBtn,SIGNAL(clicked()),this,SLOT(localBtn_slots()));
    connect(ui->cardRechargeBtn,SIGNAL(clicked()),this,SLOT(cardBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
}

void rechargeDialog::localBtn_slots()
{
    if(!readCard())
    {
        myMsgBox.showMsgBox("请在插卡区插卡操作...");
    }
}

void rechargeDialog::cardBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_RECHARGE,"");
}

void rechargeDialog::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

bool rechargeDialog::readCard()
{
    QStringList myReadDataList;
    myReadDataList.clear();

    if(SCPortComm::instance()->queryCardInfo(myReadDataList) >= 0)
    {
        getReadCardInfo(myReadDataList);
        return true;
    }
    else
    {
        return false;
    }
}

void rechargeDialog::getReadCardInfo(QStringList dataList)
{
    btnEnable(false);
    if(dataList.size() < 8)
    {
        myMsgBox.showMsgBox("卡信息不正确。");
        return;
    }
    if(SBalckList::instance()->lossUserInfo.contains(dataList.at(7)))
    {
        myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
        return ;
    }

    QString info;

    QString empType = dataList.at(0);
    myMsgBox.close();

    QByteArray     outData;
    QDataStream out(&outData,QIODevice::WriteOnly);
    out << dataList;

    if(empType == "1")//超级管理员
    {
        SCMainCtrl::instance()->enterStatus(ST_RECHARGE,outData);
    }
    else if(empType == "2")//公司管理员
    {
        if(checkCardInfo(dataList.at(1)))//检查是不是该公司卡
        {
            SCMainCtrl::instance()->enterStatus(ST_RECHARGE,outData);
        }
        else
        {
            info =  "无此权限。";
            myMsgBox.showMsgBox(info);
        }
    }
    else if(empType == "6")
    {
        if(checkCardInfo(dataList.at(1)))//检查是不是该公司卡
        {
            SCMainCtrl::instance()->enterStatus(ST_RECHARGE,outData);
        }
        else
        {
            info =  "无此权限。";
            myMsgBox.showMsgBox(info);
        }
    }
    else
    {
        info =  "无此权限。";
        myMsgBox.showMsgBox(info);
    }
    btnEnable(true);
}

bool rechargeDialog::checkCardInfo(QString companyNumber)
{
    return SCTerminal::instance()->terminalInfo.companyId.startsWith(companyNumber);
}

void rechargeDialog::btnEnable(bool value)
{
    ui->localRechargeBtn->setEnabled(value);
    ui->quitBtn->setEnabled(value);
    ui->cardRechargeBtn->setEnabled(value);
}
