#include "balancepage.h"
#include "ui_balancepage.h"
#include "portcomm.h"
#include "mainctrl.h"
#include <QTimer>
#include <QTextCodec>

#include "t_basic_terminal.h"
#include "t_basic_sync_forbidden.h"
balancePage::balancePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::balancePage)
{
    ui->setupUi(this);
    initPage();
}

balancePage::~balancePage()
{
    delete ui;
}

void balancePage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/balance/images/balance/background.png")));
    this->setPalette(palette);

    ui->companylabel->resize(QPixmap(":/balance/images/balance/company.png").size());
    ui->companylabel->setPixmap(QPixmap(":/balance/images/balance/company.png"));

    ui->label->resize(QPixmap(":/balance/images/balance/lable.png").size());
    ui->label->setPixmap(QPixmap(":/balance/images/balance/lable.png"));

    ui->typeLabel->resize(QPixmap(":/balance/images/balance/type.png").size());
    ui->typeLabel->setPixmap(QPixmap(":/balance/images/balance/type.png"));

    ui->phoneLabel->resize(QPixmap(":/balance/images/balance/phoneNum.png").size());
    ui->phoneLabel->setPixmap(QPixmap(":/balance/images/balance/phoneNum.png"));

    ui->balanceLabel->resize(QPixmap(":/balance/images/balance/balance.png").size());
    ui->balanceLabel->setPixmap(QPixmap(":/balance/images/balance/balance.png"));

    ui->queryBtn->setFlat(true);
    ui->queryBtn->setFocusPolicy(Qt::NoFocus);
    ui->queryBtn->setIcon(QIcon(":/balance/images/balance/query.png"));
    ui->queryBtn->setMinimumSize(QPixmap(":/balance/images/balance/query.png").size());
    ui->queryBtn->setIconSize(ui->queryBtn->size());
    ui->queryBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    clearInfo();
    openQueryTimer = new QTimer(this);
    connect(openQueryTimer, SIGNAL(timeout()),this, SLOT(readCard_slots()));
    connect(ui->queryBtn,SIGNAL(clicked()),this,SLOT(queryBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));

    tc =  QTextCodec::codecForName("UTF-8");
    hideLable(true);
}

void balancePage::readInfo()
{
    hideLable(true);
}

void balancePage::timeStart()
{
    openQueryTimer->start(3000);
}

void balancePage::timeStop()
{
    openQueryTimer->stop();
}

void balancePage::queryBtn_slots()
{
    clearInfo();
    readCard_slots();
}

void balancePage::quitBtn_slots()
{
    clearInfo();
    openQueryTimer->stop();
    hideLable(true);
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void balancePage::readCard_slots()
{
    setBtnStatus(false);
    QStringList myReadDataList;
    myReadDataList.clear();

    if(SCPortComm::instance()->queryCardInfo(myReadDataList) >= 0)
    {
        dealReadCardInfo(myReadDataList);
    }
    setBtnStatus(true);
}

void balancePage::clearInfo()
{
    ui->typelineEdit->clear();
    ui->phonelineEdit->clear();
    ui->balancelineEdit->clear();
    ui->companylineEdit->clear();
}

void balancePage::dealReadCardInfo(QStringList myReadDataList)
{
    if(myReadDataList.size() < 8)
    {
        myMsgBox.showMsgBox("卡信息不正确。");
        return;
    }
    if(!SCTerminal::instance()->terminalInfo.companyId.startsWith(myReadDataList.at(1)))
    {
        myMsgBox.showMsgBox("不是该公司的会员卡，不能查询。");
        return;
    }

    if(SBalckList::instance()->lossUserInfo.contains(myReadDataList.at(7)))
    {
        myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
        return ;
    }

    hideLable(false);
    //超级管理员：1|002500000001|管理员|1377655XXXX|null|0.00
    if(myReadDataList.at(0) == "1")
    {
        ui->typelineEdit->setText(QString::fromUtf8("超级管理员"));
    }
    else if(myReadDataList.at(0) == "2")
    {
        ui->typelineEdit->setText(QString::fromUtf8("公司管理员"));
    }
    else if(myReadDataList.at(0) == "3")
    {
        ui->typelineEdit->setText(QString::fromUtf8("快递员"));
    }
    else if(myReadDataList.at(0) == "4")
    {
        ui->typelineEdit->setText(QString::fromUtf8("普通用户"));
    }
    else if(myReadDataList.at(0) == "5")
    {
        ui->typelineEdit->setText(QString::fromUtf8("公司维修人员"));
    }
    else if(myReadDataList.at(0) == "6")
    {
        ui->typelineEdit->setText(QString::fromUtf8("公司充值人员"));
    }
    else if(myReadDataList.at(0) == "7")
    {
        ui->typelineEdit->setText(QString::fromUtf8("公司物业人员"));
    }
    else if(myReadDataList.at(0) == "8")
    {
        ui->typelineEdit->setText(QString::fromUtf8("超级维修员"));
    }
    else
    {
        ui->typelineEdit->setText(QString::fromUtf8("未知"));
    }
    //转换
    QByteArray byteArray;
    byteArray.clear();
    byteArray.append(myReadDataList.at(4));
    QByteArray mytesp = QByteArray::fromHex(byteArray);
    QString nameTempStr = QString::fromUtf8(mytesp.data());
    //myReadDataList.replace(4,nameTempStr);

    ui->companylineEdit->setText(nameTempStr);
    ui->phonelineEdit->setText(myReadDataList.at(3));
    ui->balancelineEdit->setText(myReadDataList.at(5));
    timeStop();
}

void balancePage::setBtnStatus(bool type)
{
    ui->queryBtn->setEnabled(type);
    ui->quitBtn->setEnabled(type);
}

void balancePage::hideLable(bool value)
{
    if(value)
    {
        ui->typeLabel->hide();
        ui->phoneLabel->hide();
        ui->balanceLabel->hide();
        ui->companylabel->hide();

        ui->typelineEdit->hide();
        ui->phonelineEdit->hide();
        ui->balancelineEdit->hide();
        ui->companylineEdit->hide();
    }
    else
    {
        ui->typeLabel->show();
        ui->phoneLabel->show();
        ui->balanceLabel->show();
        ui->companylabel->show();

        ui->typelineEdit->show();
        ui->phonelineEdit->show();
        ui->balancelineEdit->show();
        ui->companylineEdit->show();
    }
}
