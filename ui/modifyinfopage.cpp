#include "modifyinfopage.h"
#include "ui_modifyinfopage.h"
#include "mainctrl.h"
#include "portcomm.h"
#include "t_basic_terminal.h"
#include "t_basic_sync_forbidden.h"
#include "t_log_network.h"
#include "networkthread.h"
#include "servercomm.h"

#include "expcompany.h"

extern  QStringList myCompanyNameList;

modifyINfoPage::modifyINfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modifyINfoPage)
{
    ui->setupUi(this);
    initPage();
}

modifyINfoPage::~modifyINfoPage()
{
    delete ui;
}

void modifyINfoPage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/modifyInfo/images/modifyInfo/background.png")));
    this->setPalette(palette);

    ui->label->resize(QPixmap(":/modifyInfo/images/modifyInfo/text.png").size());
    ui->label->setPixmap(QPixmap(":/modifyInfo/images/modifyInfo/text.png"));

    ui->typeLabel->resize(QPixmap(":/modifyInfo/images/modifyInfo/type.png").size());
    ui->typeLabel->setPixmap(QPixmap(":/modifyInfo/images/modifyInfo/type.png"));

    ui->companyLabel->resize(QPixmap(":/modifyInfo/images/modifyInfo/companyName.png").size());
    ui->companyLabel->setPixmap(QPixmap(":/modifyInfo/images/modifyInfo/companyName.png"));

    ui->phoneLabel->resize(QPixmap(":/modifyInfo/images/modifyInfo/label.png").size());
    ui->phoneLabel->setPixmap(QPixmap(":/modifyInfo/images/modifyInfo/label.png"));

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


    openQueryTimer = new QTimer(this);
    connect(openQueryTimer, SIGNAL(timeout()),this, SLOT(readCard_slots()));

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));


    tc =  QTextCodec::codecForName("UTF-8");

    clearInfo();
    ui->typeLineEdit->setEnabled(false);
}

void modifyINfoPage::timeStart()
{
    openQueryTimer->start(3000);
}

void modifyINfoPage::dealReadCardInfo(QStringList valueList)
{
    if(valueList.size() < 8)
    {
        myMsgBox.showMsgBox("卡信息不正确。");
        return;
    }
    if(SBalckList::instance()->lossUserInfo.contains(valueList.at(7)))
    {
        myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
    }
    else if(!SCTerminal::instance()->terminalInfo.companyId.startsWith(valueList.at(1)))
    {
        myMsgBox.showMsgBox("不是该公司的会员卡，不能修改。");
        return;
    }
    else
    {
        openQueryTimer->stop();
        ui->phoneLineEdit->setText(valueList.at(3));
        ui->typeLineEdit->setText(getStringFromIndex(valueList.at(0)));

        QByteArray byteArray;
        byteArray.clear();
        byteArray.append(valueList.at(4));
        QByteArray mytesp = QByteArray::fromHex(byteArray);
        QString nameTempStr = QString::fromUtf8(mytesp.data());

        ui->companyComboBox->setCurrentIndex(myCompanyNameList.indexOf(nameTempStr));
        mReadDataList = valueList;
    }
}

QString modifyINfoPage::getStringFromIndex(QString indexStr)
{
    userType = 1;
    QString tempStr;
    if(indexStr == QString::fromUtf8("1"))
    {
        tempStr =  QString::fromUtf8("超级管理员");
    }
    else if(indexStr == QString::fromUtf8("2"))
    {
        tempStr =  QString::fromUtf8("公司管理人员");
    }
    else if(indexStr == QString::fromUtf8("3"))
    {
        ui->companyComboBox->addItems(myCompanyNameList);
        ui->companyComboBox->show();
        ui->companyLabel->show();
        tempStr =  QString::fromUtf8("快递员");
        userType = 2;
    }
    else if(indexStr == QString::fromUtf8("4"))
    {
        tempStr =  QString::fromUtf8("普通用户");
    }
    else if(indexStr == QString::fromUtf8("5"))
    {
        tempStr =  QString::fromUtf8("公司维修人员");
    }
    else if(indexStr == QString::fromUtf8("6"))
    {
        tempStr =  QString::fromUtf8("公司充值人员");
    }
    else if(indexStr == QString::fromUtf8("7"))
    {
        tempStr =  QString::fromUtf8("公司物业人员");
    }
    else if(indexStr == QString::fromUtf8("8"))
    {
        tempStr =  QString::fromUtf8("超级维修员");
    }
    else
    {
        tempStr =  QString::fromUtf8("未知");
    }
    ui->typeLabel->show();
    ui->typeLineEdit->show();
    ui->phoneLineEdit->show();
    ui->phoneLabel->show();
    return tempStr;
}


void modifyINfoPage::clearInfo()
{
    openQueryTimer->stop();
    ui->phoneLineEdit->clear();
    ui->typeLineEdit->clear();
    ui->companyComboBox->clear();
    mReadDataList.clear();

    ui->companyComboBox->hide();
    ui->companyLabel->hide();
    ui->typeLabel->hide();
    ui->typeLineEdit->hide();
    ui->phoneLineEdit->hide();
    ui->phoneLabel->hide();
}
void modifyINfoPage::setBtnEnable(bool status)
{
    ui->okBtn->setEnabled(status);
    ui->quitBtn->setEnabled(status);
}

void modifyINfoPage::okBtn_slots()
{
    emit resetTime();
    if(checkInfo())
    {
        QString tempNum = ui->phoneLineEdit->text() ;
        QString nameTempStr = "null";
        QByteArray companyNameByteArray;
        //写入卡中
        QString info;
        for(int i = 0; i < mReadDataList.size();i++)
        {
            if(i == 3)
            {
                info += (tempNum + QString("|"));
            }
            else if((i == 4)&&(userType == 2))
            {
                nameTempStr = ui->companyComboBox->currentText();
                companyNameByteArray = nameTempStr.toUtf8();
                companyNameByteArray = companyNameByteArray.toHex();

                info += (companyNameByteArray + QString("|"));
            }
            else
            {
                info += (mReadDataList.at(i) + QString("|"));
            }
        }

        bool myResult = SCPortComm::instance()->writeCardInfo(info);
        if(myResult)
        {
            //发送后台

            //转换
            //            QByteArray byteArray;
            //            byteArray.clear();
            //            byteArray.append(mReadDataList.at(4));
            //            QByteArray mytesp = QByteArray::fromHex(byteArray);
            //            QString nameTempStr = QString::fromUtf8(mytesp.data());


            //            QByteArray byteAry = mReadDataList.at(4).toUtf8();
            //            byteAry = byteAry.toHex();

            QStringList sendInfo;
            sendInfo << mReadDataList.at(7) << tempNum << nameTempStr;

            NetworkDesc_t netWorkInfo;
            SCServerComm::instance()->updateUserInfo(netWorkInfo,sendInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

            clearInfo();
            myMsgBox.showMsgBox("修改成功。");
            SCMainCtrl::instance()->enterStatus(ST_HOME,"");
        }
        else
        {
            myMsgBox.showMsgBox("修改失败，请重试。");
        }

    }
}

void modifyINfoPage::quitBtn_slots()
{
    clearInfo();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void modifyINfoPage::readCard_slots()
{
    QStringList myReadDataList;

    if(SCPortComm::instance()->queryCardInfo(myReadDataList) >= 0)
    {
        dealReadCardInfo(myReadDataList);
    }
}
bool modifyINfoPage::checkInfo()
{
    QRegExp rxcellphone("^1\\d{10}");

    QString phoneNum = ui->phoneLineEdit->text().trimmed();

    if(!rxcellphone.exactMatch(phoneNum))
    {
        myMsgBox.showMsgBox("手机号码输入错误。");
        return false;
    }

    QStringList myReadDataList;
    SCPortComm::instance()->queryCardInfo(myReadDataList);
    if(!myReadDataList.contains(mReadDataList.at(7)))
    {
        myMsgBox.showMsgBox("不是同一张卡，不能做修改。");
        return false;
    }
    return true;
}
