#include "networkconfigure.h"
#include "ui_networkconfigure.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "networkinit.h"
#include "t_basic_terminal.h"
#include "t_log_template.h"
#include "t_cabinet_cell.h"
#include "networkinit.h"
#include "t_basic_terminal.h"
#include "updatesystem.h"

#include <QFile>
#include <QDebug>
#include <QBitmap>
networkConfigure::networkConfigure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::networkConfigure)
{
    ui->setupUi(this);
    initPage();
}

networkConfigure::~networkConfigure()
{
    delete ui;
}

void networkConfigure::readInfo()
{
    QStringList listValue;
    listValue = SCNetworkInit::instance()->getNetwork();
    myindex  =  listValue.at(1).toInt();
    comboBoxChanged_Slots(myindex);
    ui->comboBox->setCurrentIndex(myindex);

    ui->ipAddrLineEdit->setText(listValue.at(2));
    ui->netmaskLineEdit->setText(listValue.at(3));
    ui->defaultGwSetLineEdit->setText(listValue.at(4));
    ui->dns1LineEdit->setText(listValue.at(5));
    ui->dns2LineEdit->setText(listValue.at(6));


    ui->serviceipAddrLineEdit->setText(SCTerminal::instance()->terminalInfo.serverIp);
    ui->servicePortLineEdit->setText(SCTerminal::instance()->terminalInfo.serverPort);
}

void networkConfigure::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    QPixmap pixmap(":/networkConfig/images/networkConfig/background.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    ui->servicePortLabel->resize(QPixmap(":/networkConfig/images/networkConfig/servicePortLabel.png").size());
    ui->servicePortLabel->setPixmap(QPixmap(":/networkConfig/images/networkConfig/servicePortLabel.png"));
    ui->serviceipAddrLabel->resize(QPixmap(":/networkConfig/images/networkConfig/serviceipAddrLabel.png").size());
    ui->serviceipAddrLabel->setPixmap(QPixmap(":/networkConfig/images/networkConfig/serviceipAddrLabel.png"));
    ui->typeLabel->resize(QPixmap(":/networkConfig/images/networkConfig/typeLabel.png").size());
    ui->typeLabel->setPixmap(QPixmap(":/networkConfig/images/networkConfig/typeLabel.png"));
    ui->ipAddrLabel->resize(QPixmap(":/networkConfig/images/networkConfig/ipAddrLabel.png").size());
    ui->ipAddrLabel->setPixmap(QPixmap(":/networkConfig/images/networkConfig/ipAddrLabel.png"));
    ui->defaultGwSetLable->resize(QPixmap(":/networkConfig/images/networkConfig/defaultGwSetLable.png").size());
    ui->defaultGwSetLable->setPixmap(QPixmap(":/networkConfig/images/networkConfig/defaultGwSetLable.png"));
    ui->netmaskLable->resize(QPixmap(":/networkConfig/images/networkConfig/netmaskLable.png").size());
    ui->netmaskLable->setPixmap(QPixmap(":/networkConfig/images/networkConfig/netmaskLable.png"));
    ui->dns1label->resize(QPixmap(":/networkConfig/images/networkConfig/dns1.png").size());
    ui->dns1label->setPixmap(QPixmap(":/networkConfig/images/networkConfig/dns1.png"));
    ui->dns2label->resize(QPixmap(":/networkConfig/images/networkConfig/dns2.png").size());
    ui->dns2label->setPixmap(QPixmap(":/networkConfig/images/networkConfig/dns2.png"));

    ui->updateBtn->setFlat(true);
    ui->updateBtn->setFocusPolicy(Qt::NoFocus);
    ui->updateBtn->setIcon(QIcon(":/networkConfig/images/networkConfig/updateBtn.png"));
    ui->updateBtn->setMinimumSize(QPixmap(":/networkConfig/images/networkConfig/updateBtn.png").size());
    ui->updateBtn->setIconSize(ui->updateBtn->size());
    ui->updateBtn->setStyleSheet("background-color:transparent");

    ui->checkBtn->setFlat(true);
    ui->checkBtn->setFocusPolicy(Qt::NoFocus);
    ui->checkBtn->setIcon(QIcon(":/networkConfig/images/networkConfig/checkBtn.png"));
    ui->checkBtn->setMinimumSize(QPixmap(":/networkConfig/images/networkConfig/checkBtn.png").size());
    ui->checkBtn->setIconSize(ui->checkBtn->size());
    ui->checkBtn->setStyleSheet("background-color:transparent");

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


    QString styleSheet = "QLineEdit{font-family:wenquanyi; font-size:20px;padding: 1px;border-style: solid;border: 2px solid gray;border-radius: 8px;}";

    ui->ipAddrLineEdit->setStyleSheet(styleSheet);
    ui->netmaskLineEdit->setStyleSheet(styleSheet);
    ui->defaultGwSetLineEdit->setStyleSheet(styleSheet);
    ui->dns1LineEdit->setStyleSheet(styleSheet);
    ui->dns2LineEdit->setStyleSheet(styleSheet);
    ui->serviceipAddrLineEdit->setStyleSheet(styleSheet);
    ui->servicePortLineEdit->setStyleSheet(styleSheet);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxChanged_Slots(int)));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->updateBtn,SIGNAL(clicked()),this,SLOT(updateTerminalInfo()));
    connect(ui->checkBtn,SIGNAL(clicked()),this,SLOT(checkVersionInfo()));
}

void networkConfigure::hidden(bool type)
{
    if(type)
    {
        ui->ipAddrLabel->hide();
        ui->ipAddrLineEdit->hide();

        ui->defaultGwSetLable->hide();
        ui->defaultGwSetLineEdit->hide();

        ui->netmaskLable->hide();
        ui->netmaskLineEdit->hide();

        ui->dns1label->hide();
        ui->dns1LineEdit->hide();

        ui->dns2label->hide();
        ui->dns2LineEdit->hide();
    }
    else
    {
        ui->ipAddrLabel->show();
        ui->ipAddrLineEdit->show();

        ui->defaultGwSetLable->show();
        ui->defaultGwSetLineEdit->show();

        ui->netmaskLable->show();
        ui->netmaskLineEdit->show();

        ui->dns1label->show();
        ui->dns1LineEdit->show();

        ui->dns2label->show();
        ui->dns2LineEdit->show();
    }
}

void networkConfigure::writeResolvFile()
{
    QString dns1 = "nameserver " + ui->dns1LineEdit->text();
    QString dns2 = "nameserver " + ui->dns2LineEdit->text();
    QFile file("/etc/resolv.conf");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qDebug() << "/etc/resolv.conf open WriteOnly fail";
        myMsgBox.showMsgBox("写入配置DNS服务失败。");
        return;
    }
    file.resize(0);
    QTextStream out(&file);
    out.setCodec("UTF-8");

    out << dns1 << endl;
    out << dns2 << endl;

    file.close();
}

bool networkConfigure::checkInfo()
{
    QRegExp rrx("\\d+\\.\\d+\\.\\d+\\.\\d+");
    QRegExp portrx("[1-9]\\d{3,4}");

    QString ipSetStr = ui->ipAddrLineEdit->text();
    QString ipNetmaskStr = ui->netmaskLineEdit->text();
    QString ipDefaultGwStr = ui->defaultGwSetLineEdit->text();
    QString dns1Str = ui->dns1LineEdit->text();
    QString dns2Str = ui->dns2LineEdit->text();

    QString portStr = ui->servicePortLineEdit->text();
    QString serviceIPStr = ui->serviceipAddrLineEdit->text();

    //
    int networkLevel = ui->comboBox->currentIndex();

    bool netbool = true;

    if(networkLevel == 3)
    {
        if( !rrx.exactMatch(ipSetStr) ||  !rrx.exactMatch(ipNetmaskStr) || !rrx.exactMatch(ipDefaultGwStr)|| !rrx.exactMatch(dns1Str)|| !rrx.exactMatch(dns2Str))
        {
            netbool = false;
            myMsgBox.showMsgBox("请输入正确的IP地址、网关、子网掩码。");
            return netbool;
        }
    }

    if(!portrx.exactMatch(portStr) ||  !rrx.exactMatch(serviceIPStr))
    {
        netbool = false;
        myMsgBox.showMsgBox("请输入正确的IP地址、端口。");
        return netbool;
    }
    return netbool;
}

void networkConfigure::setBtnEnable(bool status)
{
    ui->comboBox->setEnabled(status);
    ui->updateBtn->setEnabled(status);
    ui->quitBtn->setEnabled(status);
    ui->okBtn->setEnabled(status);
    ui->checkBtn->setEnabled(status);
}

void networkConfigure::comboBoxChanged_Slots(int index)
{
    if(index == 3)
    {
        hidden(false);
    }
    else
    {
        hidden(true);
    }
}

void networkConfigure::okBtn_slots()
{
    setBtnEnable(false);
    if(!checkInfo())
    {
        setBtnEnable(true);
        return;
    }

    SCTerminal::instance()->terminalInfo.serverIp = ui->serviceipAddrLineEdit->text();
    SCTerminal::instance()->terminalInfo.serverPort = ui->servicePortLineEdit->text();
    SCTerminal::instance()->setTerminalInfo();

    QFile file("mynetwork.sh");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate |QIODevice::Text))
    {
        qDebug() << "mynetwork.sh open WriteOnly fail";
        myMsgBox.showMsgBox("打开脚本文件失败。");
        setBtnEnable(true);
        return;
    }
    file.resize(0);
    QTextStream out(&file);
    out.setCodec("UTF-8");

    int temp = ui->comboBox->currentIndex();
    switch(temp)
    {
    case 0:
        out << "killall -KILL pppd" << endl;
        out << "killall udhcpc" << endl;
        out << "ifconfig eth0 down" << endl;
        break;
    case 1:
        out << "killall -q udhcpc" << endl;
        out << "ifconfig eth0 down" << endl;
        out << "ps w | grep pppd | grep -v grep" << endl;
        out << "if [ $? -ne 0 ];then" << endl;
        out << "(pppd call cdma &) >> debuglog.txt" << endl;
        out <<"fi" << endl;
        out <<"sleep 60s" << endl;
        out <<"cp /etc/ppp/resolv.conf /etc/" << endl;
        break;
    case 2:
        out << "killall -KILL pppd" << endl;
        out << "ifconfig eth0 up" << endl;
        out << "ps w | grep udhcpc | grep -v grep" << endl;
        out << "if [ $? -ne 0 ];then" << endl;
        out << "udhcpc" << endl;
        out <<"fi" << endl;
        break;
    case 3:
        QString ipSetStr = ui->ipAddrLineEdit->text();
        QString ipNetmaskStr = ui->netmaskLineEdit->text();
        QString ipDefaultGwStr = ui->defaultGwSetLineEdit->text();

        QString tempIPStr = "ifconfig eth0 " + ipSetStr + " netmask " + ipNetmaskStr;
        QString tempGWStr = "route add default gw " + ipDefaultGwStr;
        out << "killall -KILL pppd" << endl;
        out << "killall udhcpc" << endl;
        out << "ifconfig eth0 up" << endl;
        out << tempIPStr << endl;
        out << tempGWStr << endl;
        writeResolvFile();
        break;
    }
    file.close();

    QString ipSetStr = ui->ipAddrLineEdit->text();
    QString ipNetmaskStr = ui->netmaskLineEdit->text();
    QString ipDefaultGwStr = ui->defaultGwSetLineEdit->text();
    QString dns1Str = ui->dns1LineEdit->text();
    QString dns2Str = ui->dns2LineEdit->text();
    QStringList listValue;
    listValue = SCNetworkInit::instance()->getNetwork();
    QString pingIp =  listValue.at(0);
    listValue.clear();
    listValue << pingIp << QString::number(ui->comboBox->currentIndex()) << ipSetStr \
                   << ipNetmaskStr << ipDefaultGwStr << dns1Str << dns2Str;

    qDebug() << "listValue " << listValue;

    SCNetworkInit::instance()->setNetwork(listValue);


    system("(sh mynetwork.sh &) >> debuglog.txt");
    myMsgBox.showMsgBox("设置成功。");
    setBtnEnable(true);
}

void networkConfigure::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

//同步更新
void networkConfigure::updateTerminalInfo()
{
    setBtnEnable(false);
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
    else if(respone.resultCode().isEmpty())
    {
        myMsgBox.showMsgBox("通讯失败。");
    }
    else
    {
        myMsgBox.showMsgBox(respone.resultMessage(),false);
    }
    setBtnEnable(true);
}

void networkConfigure::checkVersionInfo()
{
    setBtnEnable(false);

    bool ret = SCUpdateSystemThrd::instance()->updateHomePng();
    qint8 appRet = SCUpdateSystemThrd::instance()->updateAppSoftWare();


    if(appRet == 0)
    {
        if(ret)
        {
            system("reboot");
            return;
        }
        myMsgBox.showMsgBox("版本已是最新版，不需升级。");
        setBtnEnable(true);

    }
    else if(appRet == -1)
    {
        if(ret)
        {
            system("reboot");
            return;
        }
        myMsgBox.showMsgBox("通讯失败。");
        setBtnEnable(true);
    }

}

void networkConfigure::startUpdateTerminalInfo(syncTerminalDataRet& respone)
{
    QList<QTerminalInfo *>terminalInfoList = respone.getTerminalInfo();
    QList<QNoteTemplateInfo*> noteList = respone.getNoteTemplateList();
    QList<QCabinetInfo*> cabinetList = respone.getCabinetList();


    QTerminalInfo *terminalInfo = terminalInfoList.at(0);

    if(!terminalInfo->encryptCode().isEmpty())
    {
        SCTerminal::instance()->terminalInfo.encryptCode      =  terminalInfo->encryptCode();
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
        SCTerminal::instance()->terminalInfo.serverIp         =  terminalInfo->serverIp();
    }
    if(!terminalInfo->serverPort().isEmpty())
    {
        SCTerminal::instance()->terminalInfo.serverPort       =  terminalInfo->serverPort();
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

    myMsgBox.showMsgBox("同步更新成功。");
}
