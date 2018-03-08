#include "terminal.h"
#include "ui_terminal.h"
#include "t_basic_terminal.h"
#include "mainctrl.h"
#include <QDebug>
#include <QBitmap>
#include <QTabBar>

terminal::terminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::terminal)
{
    ui->setupUi(this);

    initPage();
}

terminal::~terminal()
{
    delete ui;
}

void terminal::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->smsSendWayComboBox->setEnabled(false);
    ui->smsSendWayComboBox->setFixedSize(200,40);
    ui->smsSendWayComboBox->setStyleSheet("font-family:wenquanyi; font-size:24px");
    ui->tariffTypeComboBox->setFixedSize(200,40);
    ui->tariffTypeComboBox->setStyleSheet("font-family:wenquanyi; font-size:24px;");
    ui->expireStatusComboBox->setFixedSize(200,40);
    ui->expireStatusComboBox->setStyleSheet("font-family:wenquanyi; font-size:24px");
    ui->encryptCodeLineEdit->setEchoMode(QLineEdit::Password);

    QPixmap pixmap(":/terminal/images/terminal/background.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size());
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);


    tc =  QTextCodec::codecForName("UTF-8");

    ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig.png"));
    ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig.png"));
    ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore.png"));
    ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig.png"));
    ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig_1.png"));
    QSize size = QPixmap(":/terminal/images/terminal/communityConfig.png").size();
    ui->tabWidget->setIconSize(QSize(size.width(),size.height()));

    ui->tabWidget->setFocusPolicy(Qt::NoFocus);
    QString style = "QTabBar::tab{border: 0px;border-top-left-radius: 0px;border-top-right-radius: 0px;min-width: 0ex;padding: 0px;min-width:100px;min-height:96px}\
            QTabWidget::pane{border-image: url(:/page/images/main/tab_bg.png)}";

    ui->tabWidget->setStyleSheet(style);


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


    exppage = new expcompany();
    ui->tabWidget->setCurrentIndex(0);

    //终端配置信息
    ui->companyIdLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/companyIdLabel.png").size());
    ui->companyIdLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/companyIdLabel.png"));
    ui->networkCardLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/networkCardLabel.png").size());
    ui->networkCardLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/networkCardLabel.png"));
    ui->terminalIdLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/terminalIdLabel.png").size());
    ui->terminalIdLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/terminalIdLabel.png"));
    ui->linkManNameLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/linkManNameLabel.png").size());
    ui->linkManNameLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/linkManNameLabel.png"));
    ui->encryptCodeLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/encryptCodeLabel.png").size());
    ui->encryptCodeLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/encryptCodeLabel.png"));
    ui->linkMobilePhoneLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/linkMobilePhoneLabel.png").size());
    ui->linkMobilePhoneLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/linkMobilePhoneLabel.png"));
    ui->terminalNameLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/terminalNameLabel.png").size());
    ui->terminalNameLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/terminalNameLabel.png"));
    ui->forceExpTakeHoursLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/forceExpTakeHoursLabel.png").size());
    ui->forceExpTakeHoursLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/forceExpTakeHoursLabel.png"));
    ui->goodsCheckLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/goodsCheckLabel.png").size());
    ui->goodsCheckLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/goodsCheckLabel.png"));
    ui->smsSendWayLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/smsSendWayLabel.png").size());
    ui->smsSendWayLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/smsSendWayLabel.png"));
    ui->perSmslimitLabel->resize(QPixmap(":/terminal/images/terminal/terminalConfig/perSmslimitLabel.png").size());
    ui->perSmslimitLabel->setPixmap(QPixmap(":/terminal/images/terminal/terminalConfig/perSmslimitLabel.png"));


    ui->companyIdLineEdit->setText(SCTerminal::instance()->terminalInfo.companyId);
    ui->terminalIdLineEdit->setText(SCTerminal::instance()->terminalInfo.terminalId);
    ui->encryptCodeLineEdit->setText(SCTerminal::instance()->terminalInfo.encryptCode);
    ui->terminalNameLineEdit->setText(SCTerminal::instance()->terminalInfo.terminalName);
    ui->networkCardLineEdit->setText(SCTerminal::instance()->terminalInfo.networkCard);
    ui->linkManNameLineEdit->setText(SCTerminal::instance()->terminalInfo.serviceName);
    ui->linkMobilePhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.servicePhone);
    ui->perSmslimitLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.noteLimit));
    ui->forceExpTakeHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.forceExpTakeHours));
    ui->smsSendWayComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.smsSendWay - 1);

    ui->goodsCheckBtn->setFlat(true);
    ui->goodsCheckBtn->setFocusPolicy(Qt::NoFocus);
    ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    ui->goodsCheckBtn->setMinimumSize(QPixmap(":/terminal/images/terminal/start.png").size());
    ui->goodsCheckBtn->setIconSize(ui->goodsCheckBtn->size());
    ui->goodsCheckBtn->setStyleSheet("background-color:transparent");

    if(SCTerminal::instance()->terminalInfo.goodsCheck == QString("1").toInt())
    {
        ui->goodsCheckBtn->setWindowIconText("已开启");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
    else if(SCTerminal::instance()->terminalInfo.goodsCheck == QString("2").toInt())
    {
        ui->goodsCheckBtn->setWindowIconText("已关闭");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }

    //第二页
    ui->tariffTypeLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/tariffTypeLabel.png").size());
    ui->tariffTypeLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/tariffTypeLabel.png"));
    ui->userBasicTariffLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/userBasicTariffLabel.png").size());
    ui->userBasicTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/userBasicTariffLabel.png"));
    ui->expBigTariffLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expBigTariffLabel.png").size());
    ui->expBigTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expBigTariffLabel.png"));
    ui->expMiddleTariffLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expMiddleTariffLabel.png").size());
    ui->expMiddleTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expMiddleTariffLabel.png"));
    ui->expSmallTariffLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expSmallTariffLabel.png").size());
    ui->expSmallTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expSmallTariffLabel.png"));
    ui->expireHoursLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expireHoursLabel.png").size());
    ui->expireHoursLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expireHoursLabel.png"));
    ui->expireStartTariffLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expireStartTariffLabel.png").size());
    ui->expireStartTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expireStartTariffLabel.png"));
    ui->expireStepLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expireStepLabel.png").size());
    ui->expireStepLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expireStepLabel.png"));
    ui->expireStatusLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expireStatusLabel.png").size());
    ui->expireStatusLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expireStatusLabel.png"));
    ui->expireStepmoneyLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/expireStepmoneyLabel.png").size());
    ui->expireStepmoneyLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/expireStepmoneyLabel.png"));
    ui->vipPercentLabel->resize(QPixmap(":/terminal/images/terminal/chargeConfig/vipPercentLabel.png").size());
    ui->vipPercentLabel->setPixmap(QPixmap(":/terminal/images/terminal/chargeConfig/vipPercentLabel.png"));

    ui->tariffTypeComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.tariffType - 1);
    ui->expireStatusComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.expireStatus - 1);
    ui->userBasicTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.userBasicTariff,'f',2));
    ui->expBigTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.bigTariff,'f',2));
    ui->expMiddleTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.middleTariff,'f',2));
    ui->expSmallTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.smallTariff,'f',2));
    ui->expireHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireHours));
    ui->expireStartTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStartTariff,'f',2));
    ui->expireStepLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStep));
    ui->expireStepmoneyLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStepMoney,'f',2));
    ui->vipPercentLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.vipPercent,'f',2));

    //第三页
    ui->storageHoursLabel->resize(QPixmap(":/terminal/images/terminal/tempStore/storageHoursLabel.png").size());
    ui->storageHoursLabel->setPixmap(QPixmap(":/terminal/images/terminal/tempStore/storageHoursLabel.png"));
    ui->storageStartTariffLabel->resize(QPixmap(":/terminal/images/terminal/tempStore/storageStartTariffLabel.png").size());
    ui->storageStartTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/tempStore/storageStartTariffLabel.png"));
    ui->storageStepLabel->resize(QPixmap(":/terminal/images/terminal/tempStore/storageStepLabel.png").size());
    ui->storageStepLabel->setPixmap(QPixmap(":/terminal/images/terminal/tempStore/storageStepLabel.png"));
    ui->storageStepMoneyLabel->resize(QPixmap(":/terminal/images/terminal/tempStore/storageStepMoneyLabel.png").size());
    ui->storageStepMoneyLabel->setPixmap(QPixmap(":/terminal/images/terminal/tempStore/storageStepMoneyLabel.png"));

    ui->storageHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageHours));
    ui->storageStartTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStartTariff,'f',2));
    ui->storageStepLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStep));
    ui->storageStepMoneyLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStepMoney,'f',2));

    //第四页
    ui->sendBaseTariffLabel->resize(QPixmap(":/terminal/images/terminal/userConfig/sendBaseTariffLabel.png").size());
    ui->sendBaseTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/userConfig/sendBaseTariffLabel.png"));
    ui->expGetTariffLabel->resize(QPixmap(":/terminal/images/terminal/userConfig/expGetTariffLabel.png").size());
    ui->expGetTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/userConfig/expGetTariffLabel.png"));
    ui->acceptExpCompanyNameLabel->resize(QPixmap(":/terminal/images/terminal/userConfig/acceptExpCompanyNameLabel.png").size());
    ui->acceptExpCompanyNameLabel->setPixmap(QPixmap(":/terminal/images/terminal/userConfig/acceptExpCompanyNameLabel.png"));
    ui->lanJianNameLabel->resize(QPixmap(":/terminal/images/terminal/userConfig/lanJianNameLabel.png").size());
    ui->lanJianNameLabel->setPixmap(QPixmap(":/terminal/images/terminal/userConfig/lanJianNameLabel.png"));
    ui->lanJianPhoneLabel->resize(QPixmap(":/terminal/images/terminal/userConfig/lanJianPhoneLabel.png").size());
    ui->lanJianPhoneLabel->setPixmap(QPixmap(":/terminal/images/terminal/userConfig/lanJianPhoneLabel.png"));


    ui->sendBaseTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.sendBaseTariff,'f',2));
    ui->expGetTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expGetTariff,'f',2));
    ui->acceptExpCompanyNameBtn->setText(SCTerminal::instance()->terminalInfo.acceptExpCompanyName);
    ui->lanJianNameLineEdit->setText(SCTerminal::instance()->terminalInfo.lanJianName);
    ui->lanJianPhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.lanJianPhone);

    //第五页
    //社区模块
    ui->washSwitchLabel->resize(QPixmap(":/terminal/images/terminal/communityConfig/washSwitchLabel.png").size());
    ui->washSwitchLabel->setPixmap(QPixmap(":/terminal/images/terminal/communityConfig/washSwitchLabel.png"));
    ui->washSenderTariffLabel->resize(QPixmap(":/terminal/images/terminal/communityConfig/washSenderTariffLabel.png").size());
    ui->washSenderTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/communityConfig/washSenderTariffLabel.png"));
    ui->washGeterTariffLabel->resize(QPixmap(":/terminal/images/terminal/communityConfig/washGeterTariffLabel.png").size());
    ui->washGeterTariffLabel->setPixmap(QPixmap(":/terminal/images/terminal/communityConfig/washGeterTariffLabel.png"));
    ui->washerNameLabel->resize(QPixmap(":/terminal/images/terminal/communityConfig/washerNameLabel.png").size());
    ui->washerNameLabel->setPixmap(QPixmap(":/terminal/images/terminal/communityConfig/washerNameLabel.png"));
    ui->washerPhoneLabel->resize(QPixmap(":/terminal/images/terminal/communityConfig/washerPhoneLabel.png").size());
    ui->washerPhoneLabel->setPixmap(QPixmap(":/terminal/images/terminal/communityConfig/washerPhoneLabel.png"));

    ui->washSenderTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.washSenderTariff,'f',2));
    ui->washGeterTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.washGeterTariff,'f',2));
    ui->washerNameLineEdit->setText(SCTerminal::instance()->terminalInfo.washerName);
    ui->washerPhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.washerPhone);

    ui->washSwitchBtn->setFlat(true);
    ui->washSwitchBtn->setFocusPolicy(Qt::NoFocus);
    ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    ui->washSwitchBtn->setMinimumSize(QPixmap(":/terminal/images/terminal/start.png").size());
    ui->washSwitchBtn->setIconSize(ui->goodsCheckBtn->size());
    ui->washSwitchBtn->setStyleSheet("background-color:transparent");

    if(SCTerminal::instance()->terminalInfo.washSwitch == QString("1").toInt())
    {
        ui->washSwitchBtn->setWindowIconText("已开启");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
    else if(SCTerminal::instance()->terminalInfo.washSwitch == QString("2").toInt())
    {
        ui->washSwitchBtn->setWindowIconText("已关闭");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->goodsCheckBtn,SIGNAL(clicked()),this,SLOT(goodsCheckBtn_slots()));
    connect(ui->washSwitchBtn,SIGNAL(clicked()),this,SLOT(washSwitchBtn_slots()));
    connect(ui->acceptExpCompanyNameBtn,SIGNAL(clicked()),exppage,SLOT(readExpInfo()));
    connect(exppage,SIGNAL(expInfo_signal(QStringList)),this,SLOT(acceptExpCompanyNameBtn_slots(QStringList)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabPageChange_slots(int)));

}

void terminal::readTerminalInfo(QByteArray param)
{
    QStringList cardInfoList;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> cardInfoList;
    if(!cardInfoList.isEmpty() && cardInfoList.at(0) == "1")
    {
        ui->companyIdLineEdit->setEnabled(true);
    }
    else
    {
        ui->companyIdLineEdit->setEnabled(false);
    }


    ui->tabWidget->setCurrentIndex(0);

    ui->smsSendWayComboBox->setFocus();

    ui->companyIdLineEdit->setText(SCTerminal::instance()->terminalInfo.companyId);
    ui->terminalIdLineEdit->setText(SCTerminal::instance()->terminalInfo.terminalId);
    ui->encryptCodeLineEdit->setText(SCTerminal::instance()->terminalInfo.encryptCode);
    ui->terminalNameLineEdit->setText(tc->toUnicode(SCTerminal::instance()->terminalInfo.terminalName.toUtf8()));
    ui->networkCardLineEdit->setText(SCTerminal::instance()->terminalInfo.networkCard);
    ui->linkManNameLineEdit->setText(tc->toUnicode(SCTerminal::instance()->terminalInfo.serviceName.toUtf8()));
    ui->linkMobilePhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.servicePhone);
    ui->perSmslimitLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.noteLimit));
    ui->forceExpTakeHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.forceExpTakeHours,10));
    ui->smsSendWayComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.smsSendWay - 1);


    if(SCTerminal::instance()->terminalInfo.goodsCheck == QString("1").toInt())
    {
        ui->goodsCheckBtn->setWindowIconText("已开启");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
    else if(SCTerminal::instance()->terminalInfo.goodsCheck == QString("2").toInt())
    {
        ui->goodsCheckBtn->setWindowIconText("已关闭");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }

    //第二页
    ui->tariffTypeComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.tariffType - 1);
    ui->expireStatusComboBox->setCurrentIndex(SCTerminal::instance()->terminalInfo.expireStatus - 1);
    ui->userBasicTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.userBasicTariff,'f',2));
    ui->expBigTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.bigTariff,'f',2));
    ui->expMiddleTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.middleTariff,'f',2));
    ui->expSmallTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.smallTariff,'f',2));
    ui->expireHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireHours));
    ui->expireStartTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStartTariff,'f',2));
    ui->expireStepLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStep));
    ui->expireStepmoneyLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expireStepMoney,'f',2));
    ui->vipPercentLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.vipPercent,'f',2));

    //第三页
    ui->storageHoursLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageHours));
    ui->storageStartTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStartTariff,'f',2));
    ui->storageStepLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStep));
    ui->storageStepMoneyLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.storageStepMoney,'f',2));

    //第四页
    ui->sendBaseTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.sendBaseTariff,'f',2));
    ui->expGetTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.expGetTariff,'f',2));
    ui->acceptExpCompanyNameBtn->setText(SCTerminal::instance()->terminalInfo.acceptExpCompanyName);
    ui->lanJianNameLineEdit->setText(tc->toUnicode(SCTerminal::instance()->terminalInfo.lanJianName.toUtf8()));
    ui->lanJianPhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.lanJianPhone);

    //第五页
    ui->washSenderTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.washSenderTariff,'f',2));
    ui->washGeterTariffLineEdit->setText(QString::number(SCTerminal::instance()->terminalInfo.washGeterTariff,'f',2));
    ui->washerNameLineEdit->setText(tc->toUnicode(SCTerminal::instance()->terminalInfo.washerName.toUtf8()));
    ui->washerPhoneLineEdit->setText(SCTerminal::instance()->terminalInfo.washerPhone);

    if(SCTerminal::instance()->terminalInfo.washSwitch == QString("1").toInt())
    {
        ui->washSwitchBtn->setWindowIconText("已开启");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
    else if(SCTerminal::instance()->terminalInfo.washSwitch == QString("2").toInt())
    {
        ui->washSwitchBtn->setWindowIconText("已关闭");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }

    this->show();
}

void terminal::setBtnEnable(bool status)
{
    ui->okBtn->setEnabled(status);
    ui->quitBtn->setEnabled(status);
}

void terminal::rewrite()
{
    SCTerminal::instance()->terminalInfo.companyId = ui->companyIdLineEdit->text();
    SCTerminal::instance()->terminalInfo.terminalId = ui->terminalIdLineEdit->text();
    SCTerminal::instance()->terminalInfo.encryptCode = ui->encryptCodeLineEdit->text();
    SCTerminal::instance()->terminalInfo.terminalName = ui->terminalNameLineEdit->text();
    SCTerminal::instance()->terminalInfo.networkCard = ui->networkCardLineEdit->text();
    SCTerminal::instance()->terminalInfo.serviceName = ui->lanJianNameLineEdit->text();
    SCTerminal::instance()->terminalInfo.servicePhone = ui->linkMobilePhoneLineEdit->text();
    SCTerminal::instance()->terminalInfo.noteLimit = ui->perSmslimitLineEdit->text().toInt();
    SCTerminal::instance()->terminalInfo.forceExpTakeHours = ui->forceExpTakeHoursLineEdit->text().toDouble();

    SCTerminal::instance()->terminalInfo.smsSendWay = ui->smsSendWayComboBox->currentIndex() + 1;

    if(ui->goodsCheckBtn->windowIconText() == "已开启")
    {
        SCTerminal::instance()->terminalInfo.goodsCheck = 1;
    }
    else if(ui->goodsCheckBtn->windowIconText() == "已关闭")
    {
        SCTerminal::instance()->terminalInfo.goodsCheck = 2;
    }

    //第二页
    SCTerminal::instance()->terminalInfo.tariffType = ui->tariffTypeComboBox->currentIndex() + 1;
    SCTerminal::instance()->terminalInfo.expireStatus = ui->expireStatusComboBox->currentIndex() + 1;
    SCTerminal::instance()->terminalInfo.userBasicTariff = ui->userBasicTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.bigTariff = ui->expBigTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.middleTariff = ui->expMiddleTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.smallTariff = ui->expSmallTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.expireHours = ui->expireHoursLineEdit->text().toInt();
    SCTerminal::instance()->terminalInfo.expireStartTariff = ui->expireStartTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.expireStep = ui->expireStepLineEdit->text().toInt();
    SCTerminal::instance()->terminalInfo.expireStepMoney = ui->expireStepmoneyLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.vipPercent = ui->vipPercentLineEdit->text().toDouble();

    //第三页
    SCTerminal::instance()->terminalInfo.storageHours = ui->storageHoursLineEdit->text().toInt();
    SCTerminal::instance()->terminalInfo.storageStartTariff = ui->storageStartTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.storageStep = ui->storageStepLineEdit->text().toInt();
    SCTerminal::instance()->terminalInfo.storageStepMoney = ui->storageStepMoneyLineEdit->text().toDouble();

    //第四页
    SCTerminal::instance()->terminalInfo.sendBaseTariff = ui->sendBaseTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.expGetTariff = ui->expGetTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.acceptExpCompanyName = ui->acceptExpCompanyNameBtn->text();
    SCTerminal::instance()->terminalInfo.lanJianName = ui->lanJianNameLineEdit->text();
    SCTerminal::instance()->terminalInfo.lanJianPhone = ui->lanJianPhoneLineEdit->text();

    //第五页
    SCTerminal::instance()->terminalInfo.washSenderTariff = ui->washSenderTariffLineEdit->text().toDouble();
    SCTerminal::instance()->terminalInfo.washGeterTariff = ui->washGeterTariffLineEdit->text().toDouble();

    SCTerminal::instance()->terminalInfo.washerName = ui->washerNameLineEdit->text();
    SCTerminal::instance()->terminalInfo.washerPhone = ui->washerPhoneLineEdit->text().trimmed();

    if(ui->washSwitchBtn->windowIconText() == "已开启")
    {
        SCTerminal::instance()->terminalInfo.washSwitch = 1;
    }
    else if(ui->washSwitchBtn->windowIconText() == "已关闭")
    {
        SCTerminal::instance()->terminalInfo.washSwitch = 2;
    }


}

void terminal::okBtn_slots()
{
    setBtnEnable(false);
    rewrite();
    if(SCTerminal::instance()->setTerminalInfo())
    {
        myMsgBox.showMsgBox("信息配置成功");
    }
    else
    {
        myMsgBox.showMsgBox("信息配置失败");
    }
    setBtnEnable(true);
}

void terminal::quitBtn_slots()
{
    //this->close();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void terminal::goodsCheckBtn_slots()
{
    if(ui->goodsCheckBtn->windowIconText() == "已开启")
    {
        ui->goodsCheckBtn->setWindowIconText("已关闭");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }
    else if(ui->goodsCheckBtn->windowIconText() == "已关闭")
    {
        ui->goodsCheckBtn->setWindowIconText("已开启");
        ui->goodsCheckBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
}

void terminal::washSwitchBtn_slots()
{
    if(ui->washSwitchBtn->windowIconText() == "已开启")
    {
        ui->washSwitchBtn->setWindowIconText("已关闭");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/close.png"));
    }
    else if(ui->washSwitchBtn->windowIconText() == "已关闭")
    {
        ui->washSwitchBtn->setWindowIconText("已开启");
        ui->washSwitchBtn->setIcon(QIcon(":/terminal/images/terminal/start.png"));
    }
}

void terminal::acceptExpCompanyNameBtn_slots(QStringList expList)
{


    if(expList.isEmpty())
    {
        ui->acceptExpCompanyNameBtn->setText("");
    }
    else
    {
        QString exp;
        for(int i = 0; i < expList.size() - 1; i++)
        {
            exp = exp + expList.at(i) + ",";
        }
        exp = exp + expList.last();

        ui->acceptExpCompanyNameBtn->setText(exp);
    }
}

void terminal::tabPageChange_slots(int id)
{
    ui->tabWidget->currentWidget()->setFocus();
    if(4 == id)
    {
        ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig_1.png"));
        ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig.png"));
    }
    else if(3 == id)
    {
        ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig.png"));
        ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig_1.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig.png"));
    }
    else if(2 == id)
    {
        ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig.png"));
        ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore_1.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig.png"));
    }
    else if(1 == id)
    {
        ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig.png"));
        ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig_1.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig.png"));
    }
    else if(0 == id)
    {
        ui->tabWidget->setTabIcon(4,QIcon(":/terminal/images/terminal/communityConfig.png"));
        ui->tabWidget->setTabIcon(3,QIcon(":/terminal/images/terminal/userConfig.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/terminal/images/terminal/tempStore.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/terminal/images/terminal/chargeConfig.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/terminal/images/terminal/terminalConfig_1.png"));
    }
}
