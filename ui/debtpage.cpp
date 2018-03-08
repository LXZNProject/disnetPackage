#include "debtpage.h"
#include "ui_debtpage.h"
#include "t_basic_terminal.h"

#include "portcomm.h"

#include <QBitmap>

debtpage::debtpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::debtpage)
{
    ui->setupUi(this);

    initDebtPage();
}

debtpage::~debtpage()
{
    delete ui;
}

void debtpage::initDebtPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/debt/images/debt/debt_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);


    this->setFocusPolicy(Qt::NoFocus);
    this->setAttribute(Qt::WA_ShowWithoutActivating,true);
    this->resize(480,450);
    this->move(160,140);

    timer.setInterval(120*1000);
    timer.setSingleShot(true);

    ui->vipPercentLab->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:22px; color: rgb(255, 0, 0)");

    ui->baseLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:22px; color: rgb(0, 255, 64)");
    ui->debtLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:22px; color: rgb(0, 255, 64)");
    ui->paidLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:22px; color: rgb(0, 255, 64)");
    ui->oweLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:26px; color: rgb(255, 0, 0)");


    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/take/images/take/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/take/images/take/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(timeOver()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(timeOver()));
}

void debtpage::readDebtInfo(TAKE_STATUS status,double base,double pay,double debt)
{
    mStatus = status;
    isUpdate = false;

    if(1 == SCTerminal::instance()->terminalInfo.vipPercent)
    {
        ui->vipPercentLab->setText(QString::fromUtf8("不打"));
    }
    else
    {
        ui->vipPercentLab->setText(QString("%1").arg(SCTerminal::instance()->terminalInfo.vipPercent*10));
    }
    ui->baseLabel->setText(QString("%1").arg(base));     //基础
    ui->debtLabel->setText(QString("%1").arg(debt));     //滞留
    ui->paidLabel->setText(QString("%1").arg(pay));      //已付

    ui->oweLabel->setText(QString("%1").arg(base + debt - pay)); //欠费

    timer.start();
    this->show();

}

void debtpage::startPayCoin(DelivertDesc_t &deliveryInfo)
{
    isUpdate = true;
    timer.setInterval(120*1000);
    double owe = ui->oweLabel->text().toDouble() - 1;
    double paid = ui->paidLabel->text().toDouble() + 1;

    ui->paidLabel->setText(QString("%1").arg(paid));

    ui->oweLabel->setText(QString("%1").arg(owe));

    deliveryInfo.hasPayMoney += 1;
    deliveryInfo.cashMoney += 1;

    mDeliveryInfo = deliveryInfo;
    if(owe <= 0)
    {
        timeOver();
        emit savecoin_signal();
    }


}

bool debtpage::startPayCard(DelivertDesc_t &deliveryInfo,QStringList cardListInfo)
{
    bool ret = false;
    timer.setInterval(120*1000);
    double owe = ui->oweLabel->text().toDouble();

    double cash = cardListInfo.at(5).toDouble();  //卡里现金

    owe = owe * SCTerminal::instance()->terminalInfo.vipPercent;
    if(owe > cash )
    {
        myMsgBox.showMsgBox("会员卡中余额不足，请先充值");
    }
    else
    {
        QString writeCardInfo;
        QStringList tempCardList = cardListInfo;
        tempCardList.replace(5,QString::number(cash - owe,'f',2));
        for(int i = 0; i < tempCardList.size(); i++)
        {
            writeCardInfo = writeCardInfo + tempCardList.at(i) + "|";
        }

        if(SCPortComm::instance()->writeCardInfo(writeCardInfo))
        {
            ret = true;
            deliveryInfo.hasPayMoney += owe;
            timeOver();          
        }
        else
        {
            myMsgBox.showMsgBox("读卡失败");
        }
    }

    return ret;
}

bool debtpage::startPayCard(SenderDesc_t &senderInfo, QStringList cardListInfo)
{
    bool ret = false;
    timer.setInterval(120*1000);
    double owe = ui->oweLabel->text().toDouble();

    double cash = cardListInfo.at(5).toDouble();  //卡里现金

    owe = owe * SCTerminal::instance()->terminalInfo.vipPercent;
    if(owe > cash )
    {
        myMsgBox.showMsgBox("会员卡中余额不足，请先充值");
    }
    else
    {
        QString writeCardInfo;
        QStringList tempCardList = cardListInfo;
        tempCardList.replace(5,QString::number(cash - owe,'f',2));
        for(int i = 0; i < tempCardList.size(); i++)
        {          
            writeCardInfo = writeCardInfo + tempCardList.at(i) + "|";
        }

        if(SCPortComm::instance()->writeCardInfo(writeCardInfo))
        {
            ret = true;
            senderInfo.hasPayMoney += owe;
            timeOver();                  
        }
        else
        {
            myMsgBox.showMsgBox("读卡失败");
        }
    }

    return ret;
}

void debtpage::startPayCoin(SenderDesc_t &senderInfo)
{
    isUpdate = true;
    timer.setInterval(120*1000);
    double owe = ui->oweLabel->text().toDouble() - 1;
    double paid = ui->paidLabel->text().toDouble() + 1;

    ui->paidLabel->setText(QString("%1").arg(paid));

    ui->oweLabel->setText(QString("%1").arg(owe));

    senderInfo.hasPayMoney += 1;
    senderInfo.cashMoney += 1;

    mSenderInfo = senderInfo;
    if(owe <= 0)
    {
        timeOver();
        emit sendercoin_signal();

    }
}

void debtpage::startPayCoin(BasicTempStorage &storageInfo)
{
    isUpdate = true;
    timer.setInterval(120*1000);
    double owe = ui->oweLabel->text().toDouble() - 1;
    double paid = ui->paidLabel->text().toDouble() + 1;

    ui->paidLabel->setText(QString("%1").arg(paid));

    ui->oweLabel->setText(QString("%1").arg(owe));

    storageInfo.hasPayMoney += 1;
    storageInfo.cashMoney += 1;

    mStorageInfo = storageInfo;
    if(owe <= 0)
    {
        timeOver();
        emit storagecoin_signal();
    }
}

void debtpage::timeOver()
{
    timer.stop();
    if(isUpdate)
    {
        if(mStatus == SAVE_ST)
        {
            SCDelivery::instance()->updateDeliveryInfo(mDeliveryInfo);
        }
        else if(mStatus == SENDER_ST)
        {
            SCSender::instance()->updateSenderInfo(mSenderInfo);
        }
        else if(mStatus == STORE_ST)
        {
            SBasicTempStorage::instance()->updateStorageInfo(mStorageInfo);
        }
    }
    this->close();
}

