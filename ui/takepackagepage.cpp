#include "takepackagepage.h"
#include "ui_takepackagepage.h"

#include "t_cabinet_cell.h"
#include "t_basic_terminal.h"

#include "portcomm.h"
#include "driver.h"
#include "t_basic_sync_forbidden.h"
#include "t_basic_terminal.h"
#include "t_log_smslog.h"
#include "t_log_template.h"
#include "t_log_smslimit.h"
#include "t_log_network.h"
#include "networkthread.h"
#include "notethread.h"
#include "mainctrl.h"
#include "servercomm.h"
#include <QBitmap>
#include <QUuid>


#define EXPAND  100.00

takepackagepage::takepackagepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::takepackagepage)
{
    ui->setupUi(this);

    initTakePackPage();

}

takepackagepage::~takepackagepage()
{
    delete ui;
}
void takepackagepage::initTakePackPage()
{
    this->setFixedSize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    keypage = new keyboard(ui->widget);

    ui->widget->setStyleSheet("background-image: url(:/key/images/keyboard/key_bg.png)");

    QPixmap pixmap(":/take/images/take/take_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);


//    QFont font;
//    font.setFamily(QString::fromUtf8("wenquanyi"));
//    font.setPointSize(16);

//    QPalette fontcolor;
//    fontcolor.setColor(QPalette::ButtonText,Qt::white);

    ui->pageLabel->setFixedSize(QPixmap(":/take/images/take/word1.png").size());
    ui->pageLabel->setPixmap(QPixmap(":/take/images/take/word1.png"));

    ui->label->setFixedSize(QPixmap(":/take/images/take/word2.png").size());
    ui->label->setPixmap(QPixmap(":/take/images/take/word2.png"));

    ui->page2Label->setFixedSize(QPixmap(":/take/images/take/word3.png").size());
    ui->page2Label->setPixmap(QPixmap(":/take/images/take/word3.png"));


    ui->getCode_lineEdit->setMaxLength(8);
    ui->getCode_lineEdit->setFixedSize(150,30);
    ui->getCode_lineEdit->bindKeyPage(keypage);

    ui->phone_lineEdit->setMaxLength(11);
    ui->phone_lineEdit->setFixedSize(200,30);
    ui->phone_lineEdit->bindKeyPage(keypage);


    ui->againBtn->setFlat(true);
    ui->againBtn->setFocusPolicy(Qt::NoFocus);
    ui->againBtn->setIcon(QIcon(":/take/images/take/againBtn_bg.png"));
    ui->againBtn->setMinimumSize(QPixmap(":/take/images/take/againBtn_bg.png").size());
    ui->againBtn->setIconSize(ui->againBtn->size());
    ui->againBtn->setStyleSheet("background-color:transparent");

    ui->okBtn_takePack->setFlat(true);
    ui->okBtn_takePack->setFocusPolicy(Qt::NoFocus);
    ui->okBtn_takePack->setIcon(QIcon(":/take/images/take/okBtn_takePack_bg.png"));
    ui->okBtn_takePack->setMinimumSize(QPixmap(":/take/images/take/okBtn_takePack_bg.png").size());
    ui->okBtn_takePack->setIconSize(ui->okBtn_takePack->size());
    ui->okBtn_takePack->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/take/images/take/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/take/images/take/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    ui->okBtn_getCode->setFlat(true);
    ui->okBtn_getCode->setFocusPolicy(Qt::NoFocus);
    ui->okBtn_getCode->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn_getCode->setFixedSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn_getCode->setIconSize(ui->okBtn_getCode->size());
    ui->okBtn_getCode->setStyleSheet("background-color:transparent");

    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/public/images/public/cancelBtn_bg.png"));
    ui->cancelBtn->setFixedSize(QPixmap(":/public/images/public/cancelBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->cancelBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");


    connect(ui->againBtn,SIGNAL(clicked()),this,SLOT(againBtn_slots()));
    connect(ui->okBtn_takePack,SIGNAL(clicked()),this,SLOT(okBtn_takePack_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(ui->okBtn_getCode,SIGNAL(clicked()),this,SLOT(okBtn_getCode_slots()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));

    connect(&debtInfoPage,SIGNAL(savecoin_signal()),this,SLOT(showAdvertPage()));
    connect(this,SIGNAL(savecard_signal()),this,SLOT(showAdvertPage()));
    connect(&debtInfoPage,SIGNAL(sendercoin_signal()),this,SLOT(showAdvertPage()));
    connect(this,SIGNAL(sendercard_signal()),this,SLOT(showAdvertPage()));
    connect(&advertInfoPage,SIGNAL(advPageClose()),this,SLOT(readyOpenBoxLock()));
    connect(&debtInfoPage,SIGNAL(storagecoin_signal()),this,SLOT(showAdvertPage()));
    connect(&openCellPage,SIGNAL(disOpenCell()),this,SLOT(skipCurPage()));
    connect(&senderOpenPage,SIGNAL(disOpenCell()),this,SLOT(skipCurPage()));

    connect(ui->getCode_lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
    connect(ui->getCode_lineEdit,SIGNAL(okBtnPress()),this,SLOT(okBtn_takePack_slots()));
    connect(ui->phone_lineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
    connect(ui->phone_lineEdit,SIGNAL(okBtnPress()),this,SLOT(okBtn_getCode_slots()));

}


void takepackagepage::readTakePackInfo()
{
    this->Cur_Status = NOT_ST;
    mCardWay = false;
    isWorking = false;
    cardInfoList.clear();
    mDebtPay = 0;

    ui->stackedWidget->setCurrentIndex(0);
    SCDelivery::instance()->clearDeliveryInfo(deliveryInfo);
    SCSender::instance()->clearDeliveryInfo(senderInfo);

    SBasicTempStorage::instance()->clearDeliveryInfo(storageInfo);


    clearInfo();
}

void takepackagepage::setBtnEnable(bool status)
{
    ui->againBtn->setEnabled(status);
    ui->okBtn_takePack->setEnabled(status);
    ui->quitBtn->setEnabled(status);
    ui->okBtn_getCode->setEnabled(status);
    ui->cancelBtn->setEnabled(status);
}


/****************************************************************
**
*	函数名称：	startCoin
*	函数功能：	投币付费触发的函数
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
bool takepackagepage::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *myEvent = static_cast<QKeyEvent *>(event);
        if(myEvent->key() == Qt::Key_Insert)
        {
            emit resetTime();
            if(debtInfoPage.isVisible())
            {
                if(this->Cur_Status == SAVE_ST)
                {
                    debtInfoPage.startPayCoin(deliveryInfo);
                }
                else if(this->Cur_Status == SENDER_ST)
                {
                    debtInfoPage.startPayCoin(senderInfo);
                }
                else if(this->Cur_Status == STORE_ST)
                {
                    debtInfoPage.startPayCoin(storageInfo);
                }
            }
        }
    }
    return  QWidget::event(event);
}

void takepackagepage::queryCardInfo_slots()
{
    if(debtInfoPage.isVisible())
    {
        QStringList cardListInfo;
        if(SCPortComm::instance()->queryCardInfo(cardListInfo) >= 0)
        {
            if(cardListInfo.size() < 6)
            {
                return;
            }
            if(SBalckList::instance()->lossUserInfo.contains(cardListInfo.at(7)))
            {
                myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
                return ;
            }
            if(!SCTerminal::instance()->terminalInfo.companyId.startsWith(cardListInfo.at(1)))
            {
                myMsgBox.showMsgBox("不是该公司的会员卡。");
                return;
            }
            emit resetTime();
            if(this->Cur_Status == SAVE_ST)
            {
                if(debtInfoPage.startPayCard(deliveryInfo,cardListInfo))
                {
                    mCardWay = true;
                    cardInfoList = cardListInfo;
                    emit savecard_signal();
                }
            }
            else if(this->Cur_Status == SENDER_ST)
            {
                if(debtInfoPage.startPayCard(senderInfo,cardListInfo))
                {
                    mCardWay = true;
                    cardInfoList = cardListInfo;
                    emit sendercard_signal();
                }
            }
        }
    }
}

void takepackagepage::showAdvertPage()
{
    advertInfoPage.advPageShow();
}

void takepackagepage::againBtn_slots()
{
//    double own = 2;
//    double pay = 0;
//    double debt = 0;
//    debtInfoPage.readDebtInfo(Cur_Status,own,pay,debt);
//    showAdvertPage();
//    senderInfo.deliveryType = 2;
//    senderInfo.mobilephone = "12345673456";
//    senderInfo.cellId = "0120";
//    senderInfo.companyName = QString::fromUtf8("顺丰");
//    senderOpenPage.showOpneCell(senderInfo);
//    QString cell = "0306";
//     advertInfoPage.advPageShow();
//    openCellPage.showOpneCell(cell);
    emit resetTime();
    ui->stackedWidget->setCurrentIndex(1);
}

void takepackagepage::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}


/****************************************************************
**
*	函数名称：	okBtn_getCode_slots
*	函数功能：	再次获取取件码的槽函数
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::okBtn_getCode_slots()
{
    if(!isWorking)
    {
        isWorking = true;
        emit resetTime();
        setBtnEnable(false);
        int noteNum = 1;
        QString code;

        if(checkPhoneStyle(code))
        {
            bool ret = SCNoteLimit::instance()->checkNoteSendInfo(ui->phone_lineEdit->text().trimmed(),SCTerminal::instance()->terminalInfo.noteLimit,noteNum);
            if(ret)
            {
                //发送短信
                sendNote(code,noteNum);
                myMsgBox.showMsgBox("短信已发送，请注意查收。");
            }
            else
            {
                myMsgBox.showMsgBox("您取短信的次数已超上限，详情请咨询客服。");
            }
        }

        clearInfo();
        setBtnEnable(true);
        isWorking = false;
    }
}


void takepackagepage::cancelBtn_slots()
{
    emit resetTime();
    clearInfo();
    ui->stackedWidget->setCurrentIndex(0);
}


/****************************************************************
**
*	函数名称：	okBtn_takePack_slots
*	函数功能：	开始取件槽函数
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
void takepackagepage::okBtn_takePack_slots()
{
    if(!isWorking)
    {
        isWorking = true;
        emit resetTime();
        setBtnEnable(false);

        SCDelivery::instance()->clearDeliveryInfo(deliveryInfo);
        SCSender::instance()->clearDeliveryInfo(senderInfo);

        SBasicTempStorage::instance()->clearDeliveryInfo(storageInfo);


        checkGodeCode();

        clearInfo();
        setBtnEnable(true);
        isWorking = false;
    }
}


/****************************************************************
**
*	函数名称：	checkGodeCode
*	函数功能：	检验取件码是否正确并判断取件类型
* 	函数参数：	无
* 	返回值  :       无
*
****************************************************************/
bool takepackagepage::checkGodeCode()
{
    QRegExp rxpasswd("[0-9]\\d{5}");
    bool ret = true;
    QString info;
    this->Cur_Status = NOT_ST;
    mCardWay = false;
    cardInfoList.clear();
    QString code = ui->getCode_lineEdit->text().trimmed();
    //下面判断的（后期用到）
    if(rxpasswd.exactMatch(code))
    {
        basicCell tempValue;
        qint8 value  = code.left(1).toInt() + code.mid(1,1).toInt();
        if(9 == value)
        {
            if(SCDelivery::instance()->selectTakeDeliveryInfo(code,deliveryInfo))
            {
                //有这派件
                if(ST_cabinet_cell::instance()->getCellInfo(deliveryInfo.cellId,tempValue) && (tempValue.status == 1))
                {
                    this->Cur_Status = SAVE_ST;
                    getSavePack();
                }
                else
                {
                    info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
                    ret = false;
                }
            }
            else
            {
                info = "此终端没有您的派件";
                ret = false;
            }
        }
        else if(8 == value)
        {
            if(SBasicTempStorage::instance()->selectTakeDeliveryInfo(code,storageInfo))
            {
                //有这派件
                if(ST_cabinet_cell::instance()->getCellInfo(storageInfo.cellId,tempValue) && (tempValue.status == 1))
                {
                    this->Cur_Status = STORE_ST;
                    getStoragePack();
                }
                else
                {
                    info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
                    ret = false;
                }
            }
            else
            {
                info = "此终端没有您的派件";
                ret = false;
            }
        }
        else if(7 == value)
        {
            if(SCSender::instance()->selectTakeDeliveryInfo(code,senderInfo))
            {
                //有这派件
                if(ST_cabinet_cell::instance()->getCellInfo(senderInfo.cellId,tempValue) && (tempValue.status == 1))
                {
                    this->Cur_Status = SENDER_ST;
                    getSenderPack();
                }
                else
                {
                    info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
                    ret = false;
                }
            }
            else
            {
                info = "此终端没有您的派件";
                ret = false;
            }
        }
        else
        {
            info = "取件码输入有误";
            ret = false;
        }
    }
    else
    {
        info = "取件码输入有误";
        ret = false;
    }



    if (!ret)
    {
        myMsgBox.showMsgBox(info);

    }

    return ret;
}

double takepackagepage::expandDouble(double value)
{
    return value * EXPAND;
}


/****************************************************************
**
*	函数名称：	getSavePack
*	函数功能：	用户取件查询是否需付费的函数
* 	函数参数：       无
* 	返回值  :       true 有
*
****************************************************************/
bool takepackagepage::getSavePack()
{
    if((deliveryInfo.switchCharge == 2) || ((deliveryInfo.expireStatus ==1) && ((deliveryInfo.tariffType == 1) || (deliveryInfo.tariffType == 2))))
    {
        //不收费开箱
        deliveryInfo.payway = 2;
        //openBoxLock();
        showAdvertPage();
    }
    else //收取滞留费
    {
        if(deliveryInfo.expireStatus == 1)
        {
            mDebtPay = 0;
        }
        else if(deliveryInfo.expireStatus == 2)
        {
            int timeValue = deliveryInfo.messageSentTime.secsTo(QDateTime::currentDateTime());
//            qDebug() << "timeValue/3600:" << timeValue << deliveryInfo.expireHours * 3600;
            if(timeValue <= deliveryInfo.expireHours * 3600)
            {
                timeValue = 0;
            }
            else
            {
                timeValue = timeValue/3600;
            }
//            qDebug() << "tempValue:" << timeValue;
            int tempDebt = timeValue - deliveryInfo.expireHours;
            if( tempDebt < 0)
            {
                mDebtPay = 0;
            }
            else
            {
                int expireStep = deliveryInfo.expireStep;
                double expireStepmoney = deliveryInfo.expireStepMoney;

                //滞留费用

                mDebtPay = (expandDouble(deliveryInfo.expireStartTariff) + expandDouble(tempDebt/expireStep * expireStepmoney))/EXPAND;

            }
        }

        double paidCash = (expandDouble(deliveryInfo.hasPayMoney) - expandDouble(deliveryInfo.expressPay))/EXPAND; //已付费用

        if(deliveryInfo.ownerBasePay + mDebtPay > paidCash)
        {
            this->Cur_Status = SAVE_ST;
            debtInfoPage.readDebtInfo(Cur_Status,deliveryInfo.ownerBasePay,paidCash,mDebtPay);
        }
        else
        {
           // openBoxLock();
            showAdvertPage();
        }
    }

    return true;
}


/****************************************************************
**
*	函数名称：	getSenderPack
*	函数功能：	用户取寄件包裹查询是否需付费的函数
* 	函数参数：       无
* 	返回值  :       true 有
*
****************************************************************/
bool takepackagepage::getSenderPack()
{
    if((senderInfo.switchCharge == 2) || (senderInfo.expressPay <= 0))
    {
        //不收费开箱
        senderInfo.payway = 2;
        //openSenderBoxLock();
        showAdvertPage();
    }
    else if(senderInfo.expressPay > 0) //收取滞留费
    {
        double paidCash = (expandDouble(senderInfo.hasPayMoney) - expandDouble(senderInfo.ownerBasePay))/EXPAND; //已付费用
        if(paidCash < senderInfo.expressPay)
        {
            this->Cur_Status = SENDER_ST;
            debtInfoPage.readDebtInfo(Cur_Status,senderInfo.expressPay,paidCash);
        }
        else
        {
           // openSenderBoxLock();
            showAdvertPage();
        }
    }
    return true;
}
/****************************************************************
**
*	函数名称：	getTempStorage
*	函数功能：	用户取临时存储裹查询是否需付费的函数
* 	函数参数：       无
* 	返回值  :       true 有
*
****************************************************************/
bool takepackagepage::getStoragePack()
{
    int timeValue = storageInfo.messageSentTime.secsTo(QDateTime::currentDateTime());
    if(timeValue <= SCTerminal::instance()->terminalInfo.storageHours *3600)
    {
        timeValue = 0;
    }
    else
    {
        timeValue = timeValue/3600;
    }
    int tempDebt = timeValue - SCTerminal::instance()->terminalInfo.storageHours;
    if( tempDebt < 0)
    {
        mDebtPay = 0;
    }
    else
    {
        int storageStep = SCTerminal::instance()->terminalInfo.storageStep;
        double storageStepmoney = SCTerminal::instance()->terminalInfo.storageStepMoney;
        //滞留费用      
        mDebtPay = (tempDebt/storageStep + 1) * storageStepmoney;

    }
    if((storageInfo.switchCharge == 2) || (mDebtPay <= 0))
    {
        //不收费开箱
        //openStorageBoxLock();
        showAdvertPage();
    }
    else
    {
        double paidCash = (expandDouble(storageInfo.hasPayMoney) - expandDouble(SCTerminal::instance()->terminalInfo.storageStartTariff))/EXPAND; //已付费用
        if(paidCash < mDebtPay)
        {
            this->Cur_Status = STORE_ST;
            double base = 0;
            debtInfoPage.readDebtInfo(Cur_Status,base,paidCash,mDebtPay);
        }
        else
        {
           // openStorageBoxLock();
            showAdvertPage();
        }
    }
    return true;
}

/****************************************************************
**
*	函数名称：	checkPhoneStyle
*	函数功能：	再次获取取件码检验手机号信息的函数
* 	函数参数：       无
* 	返回值  :       true 有
*
****************************************************************/
bool takepackagepage::checkPhoneStyle(QString &code)
{
    QRegExp rxcellphone("^1\\d{10}");
    QString info;
    bool ret = true;

    QString phoneMobile = ui->phone_lineEdit->text().trimmed();
    if(!rxcellphone.exactMatch(phoneMobile))
    {
        info = "手机号码输入不正确";
        ret = false;
    }
    else
    {
        SCDelivery::instance()->selectTakeCodeForPhone(phoneMobile,code);
        SCSender::instance()->selectTakeCodeForPhone(phoneMobile,code);
        SBasicTempStorage::instance()->selectTakeCodeForPhone(phoneMobile,code);

        if(code.isEmpty())
        {
            info = "此终端没有您的派件";  //揽件人
            ret = false;
        }
    }

    if(!ret)
    {
        myMsgBox.showMsgBox(info);
    }
    return ret;
}

void takepackagepage::clearInfo()
{
    ui->getCode_lineEdit->clear();
    ui->phone_lineEdit->clear();
}


void takepackagepage::readyOpenBoxLock()
{
    if(this->Cur_Status == SAVE_ST)
    {
        openBoxLock();
    }
    else if(this->Cur_Status == SENDER_ST)
    {
        openSenderBoxLock();
    }
    else if(this->Cur_Status == STORE_ST)
    {
        openStorageBoxLock();
    }
    advertInfoPage.close();
}

/****************************************************************
**
*	函数名称：	openBoxLock
*	函数功能：	用户取件打开箱门，并保存信息
* 	函数参数：       无
* 	返回值  :        无
*
****************************************************************/
void takepackagepage::openBoxLock()
{
    bool ret = true;
    qint8 cabinetId = deliveryInfo.cellId.left(2).toInt();
    qint8 cellId = deliveryInfo.cellId.right(2).toInt();

    QList<NetCellExcept> exceInfoList;
    cellError boxError;

    if(1 == deliveryInfo.payway)
    {
        if(deliveryInfo.cashMoney >= deliveryInfo.ownerBasePay)
        {
            deliveryInfo.debtPay = (expandDouble(deliveryInfo.hasPayMoney) - expandDouble(deliveryInfo.expressPay) - expandDouble(deliveryInfo.ownerBasePay))/EXPAND;
        }
        else
        {
            double vip = SCTerminal::instance()->terminalInfo.vipPercent;
            double cardMoney = QString::number((deliveryInfo.ownerBasePay - deliveryInfo.cashMoney)*vip,'f',2).toDouble();
            deliveryInfo.ownerBasePay = deliveryInfo.cashMoney + cardMoney;
            deliveryInfo.debtPay = (expandDouble(deliveryInfo.hasPayMoney) - expandDouble(deliveryInfo.expressPay) - expandDouble(deliveryInfo.ownerBasePay))/EXPAND;
            //qDebug() << "vip:" << vip << "deliveryInfo.expressPay:" << deliveryInfo.expressPay << "cardMoney:"<< cardMoney << "deliveryInfo.cashMoney:" << deliveryInfo.cashMoney << "deliveryInfo.ownerBasePay:" << deliveryInfo.ownerBasePay << "deliveryInfo.hasPayMoney:" << deliveryInfo.hasPayMoney << "deliveryInfo.debtPay:" << deliveryInfo.debtPay;
        }

        if((deliveryInfo.cashMoney > 0) && mCardWay)
        {
            deliveryInfo.payway = 5; //混合
        }
        else if(deliveryInfo.cashMoney > 0)
        {
            deliveryInfo.payway = 4; //投币
        }
        else if(mCardWay)
        {
            deliveryInfo.payway = 3; //刷卡
        }
        else
        {
            deliveryInfo.payway = 2; //无需付款
        }
    }

    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
       // qint8 status = 3; //未关闭状态
        //qint8 hasGoods = 2; //无物品
        QString mp3file = "./music/take.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);

        deliveryInfo.status = 2;

        boxError.cabinetId = deliveryInfo.cabinetId;
        boxError.cellId = deliveryInfo.cellId;
        boxError.errorType = 3;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;
        //派件表取出
        deliveryInfo.takeTime = boxError.reporttime;

       //柜子和格子表设置为未关闭无物品
        ST_cabinet_cell::instance()->takeChangeCellInfo(deliveryInfo.cellId);
   //     ST_cabinet_cell::instance()->changeCellHasGoods(deliveryInfo.cellId,hasGoods);
   //     ST_cabinet_cell::instance()->changeCellStatus(deliveryInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);
        sendNote(deliveryInfo);

        openCellPage.showOpneCell(deliveryInfo.cellId);

        NetworkDesc_t netWorkInfo = SCServerComm::instance()->takeDelivery(deliveryInfo,2,cardInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);
    }
    else
    {
        ret = false;
        //上报异常
        NetCellExcept exceInfo;
        exceInfo.cellId = deliveryInfo.cellId;
        exceInfo.flowType = 1; //存件
        exceInfo.errorType = 2;//打不开
        exceInfo.Relkey = deliveryInfo.deliveryId;
        exceInfoList.append(exceInfo);

        qint8 status = 2; //用户取件打不开状态
        boxError.cabinetId = deliveryInfo.cabinetId;
        boxError.cellId = deliveryInfo.cellId;
        boxError.errorType = 2;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;


       //柜子和格子表设置为打不开
        ST_cabinet_cell::instance()->changeCellStatus(deliveryInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        sendExceptionNote(deliveryInfo);

        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    SCDelivery::instance()->updateDeliveryInfo(deliveryInfo);

    if(!ret)
    {
        QString info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
        myMsgBox.showMsgBox(info);
    }
}


/****************************************************************
**
*	函数名称：	openSenderBoxLock
*	函数功能：	用户取寄件包裹打开箱门，并保存信息
* 	函数参数：       无
* 	返回值  :        无
*
****************************************************************/
void takepackagepage::openSenderBoxLock()
{
    bool ret = true;
    qint8 cabinetId = senderInfo.cellId.left(2).toInt();
    qint8 cellId = senderInfo.cellId.right(2).toInt();

    QList<NetCellExcept> exceInfoList;
    cellError boxError;

    if(1 == senderInfo.payway)
    {
        senderInfo.expressPay = (expandDouble(senderInfo.hasPayMoney) - expandDouble(senderInfo.ownerBasePay))/EXPAND;

        if((senderInfo.expressPay > 0) && mCardWay)
        {
            senderInfo.payway = 5;
        }
        else if(senderInfo.expressPay > 0)
        {
            senderInfo.payway = 4;
        }
        else if(mCardWay)
        {
            senderInfo.payway = 3;
        }
        else
        {
            senderInfo.payway = 2;
        }

    }
    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
      //  qint8 status = 3; //未关闭状态
      //  qint8 hasGoods = 2; //无物品
        QString mp3file = "./music/take.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);

        senderInfo.status = 2;

        boxError.cabinetId = senderInfo.cabinetId;
        boxError.cellId = senderInfo.cellId;
        boxError.errorType = 3;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;
        //派件表取出
        senderInfo.takeTime = boxError.reporttime;

       //柜子和格子表设置为未关闭无物品

        ST_cabinet_cell::instance()->takeChangeCellInfo(senderInfo.cellId);
      //ST_cabinet_cell::instance()->changeCellHasGoods(senderInfo.cellId,hasGoods);
      //ST_cabinet_cell::instance()->changeCellStatus(senderInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);
        sendNote(senderInfo);

        //openCellPage.showOpneCell(senderInfo.cellId);
        senderOpenPage.showOpneCell(senderInfo);

        NetworkDesc_t netWorkInfo = SCServerComm::instance()->takeSendDelivery(senderInfo,cardInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);
    }
    else
    {
        ret = false;
        //上报异常
        NetCellExcept exceInfo;
        exceInfo.cellId = senderInfo.cellId;
        if(1 == senderInfo.deliveryType)
        {
            exceInfo.flowType = 2; //寄件
        }
        else if(2 == senderInfo.deliveryType)
        {
            exceInfo.flowType = 4; //干洗
        }
        exceInfo.errorType = 6;//打不开
        exceInfo.Relkey = senderInfo.lanJianId;
        exceInfoList.append(exceInfo);

        qint8 status = 2; //快递员揽件取件打不开状态
        boxError.cabinetId = senderInfo.cabinetId;
        boxError.cellId = senderInfo.cellId;
        boxError.errorType = 6;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;

       //柜子和格子表设置为打不开
        ST_cabinet_cell::instance()->changeCellStatus(senderInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        sendExceptionNote(senderInfo);

        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    SCSender::instance()->updateSenderInfo(senderInfo);

    if(!ret)
    {
        QString info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
        myMsgBox.showMsgBox(info);
    }
}

void takepackagepage::openStorageBoxLock()
{
    bool ret = true;
    qint8 cabinetId = storageInfo.cellId.left(2).toInt();
    qint8 cellId = storageInfo.cellId.right(2).toInt();

    QList<NetCellExcept> exceInfoList;
    cellError boxError;

    storageInfo.debtPay = (expandDouble(storageInfo.hasPayMoney) - expandDouble(storageInfo.saveBasePay))/EXPAND;
    if(storageInfo.debtPay > 0)
    {
        storageInfo.payway = 4;      
    }
    else
    {
        storageInfo.payway = 2;
    }

    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
      //  qint8 status = 3; //未关闭状态
      //  qint8 hasGoods = 2; //无物品
        QString mp3file = "./music/take.mp3";
        QString vol = "15";
        SCDriver::instance()->playVoice(mp3file,vol);

        storageInfo.status = 2;

        boxError.cabinetId = storageInfo.cabinetId;
        boxError.cellId = storageInfo.cellId;
        boxError.errorType = 3;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;
        //派件表取出
        storageInfo.takeTime = boxError.reporttime;

       //柜子和格子表设置为未关闭无物品

        ST_cabinet_cell::instance()->takeChangeCellInfo(storageInfo.cellId);
      //ST_cabinet_cell::instance()->changeCellHasGoods(senderInfo.cellId,hasGoods);
      //ST_cabinet_cell::instance()->changeCellStatus(senderInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        openCellPage.showOpneCell(storageInfo.cellId);

        NetworkDesc_t netWorkInfo = SCServerComm::instance()->takeTempDelivery(storageInfo,cardInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);

    }
    else
    {
        ret = false;
        //上报异常
        NetCellExcept exceInfo;
        exceInfo.cellId = storageInfo.cellId;
        exceInfo.flowType = 3; //临时存放
        exceInfo.errorType = 5;//打不开
        exceInfo.Relkey = storageInfo.storageId;
        exceInfoList.append(exceInfo);

        qint8 status = 2; //用户取件打不开状态
        boxError.cabinetId = storageInfo.cabinetId;
        boxError.cellId = storageInfo.cellId;
        boxError.errorType = 5;
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;


       //柜子和格子表设置为打不开
        ST_cabinet_cell::instance()->changeCellStatus(storageInfo.cellId,status);

        ST_cabinet_cell::instance()->insertCellError(boxError);

        sendExceptionNote(storageInfo);

        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    SBasicTempStorage::instance()->updateStorageInfo(storageInfo);

    if(!ret)
    {
        QString info = "箱门打不开,请等待客服处理，处理完毕将会短信通知";
        myMsgBox.showMsgBox(info);
    }

}

void takepackagepage::skipCurPage()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}


void takepackagepage::sendNote(DelivertDesc_t savePackInfo)
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

    noteInfo.mSmsType = 2;
    noteInfo.mRelPriKey = savePackInfo.deliveryId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(savePackInfo.expCompanyName).arg(savePackInfo.packageId);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}

void takepackagepage::sendNote(SenderDesc_t savePackInfo)
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

    noteInfo.mRelPriKey = savePackInfo.lanJianId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;

    if(savePackInfo.deliveryType == 1)
    {
        noteInfo.mSmsType = 13;
    }
    else if(savePackInfo.deliveryType == 2)
    {
        noteInfo.mSmsType = 33;
    }

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.lanJianPhone);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}

void takepackagepage::sendNote(QString code,int noteNum)
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

    noteInfo.mSmsType = 91;
    noteInfo.mMobilePhone = ui->phone_lineEdit->text().trimmed();
    noteInfo.mSendCount = noteNum;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName + ":" + code.left(code.length() - 1));

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}

void takepackagepage::sendExceptionNote(DelivertDesc_t savePackInfo)
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

    noteInfo.mRelPriKey = savePackInfo.deliveryId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 92;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent;

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }

    QUuid uuId1 = QUuid::createUuid();
    QString strUuId1 = uuId1.toString();
    strUuId1 = strUuId1.remove(QRegExp("[{}-]"));
    strUuId1 = strUuId1.toUpper();
    noteInfo.mSmsLogId = strUuId1;

    noteInfo.mMobilePhone = SCTerminal::instance()->terminalInfo.servicePhone;
    noteInfo.mSmsType = 93;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.cellId);

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}

void takepackagepage::sendExceptionNote(SenderDesc_t savePackInfo)
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

    noteInfo.mRelPriKey = savePackInfo.lanJianId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 92;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent;
        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }

    QUuid uuId1 = QUuid::createUuid();
    QString strUuId1 = uuId1.toString();
    strUuId1 = strUuId1.remove(QRegExp("[{}-]"));
    strUuId1 = strUuId1.toUpper();
    noteInfo.mSmsLogId = strUuId1;

    noteInfo.mMobilePhone = SCTerminal::instance()->terminalInfo.servicePhone;
    noteInfo.mSmsType = 93;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.cellId);
        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}

void takepackagepage::sendExceptionNote(BasicTempStorage savePackInfo)
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

    noteInfo.mRelPriKey = savePackInfo.storageId;
    noteInfo.mMobilePhone = savePackInfo.mobilephone;
    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 92;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent;
        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }

    QUuid uuId1 = QUuid::createUuid();
    QString strUuId1 = uuId1.toString();
    strUuId1 = strUuId1.remove(QRegExp("[{}-]"));
    strUuId1 = strUuId1.toUpper();
    noteInfo.mSmsLogId = strUuId1;
    noteInfo.mMobilePhone = SCTerminal::instance()->terminalInfo.servicePhone;
    noteInfo.mSmsType = 93;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(savePackInfo.cellId);
        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            SCSmslog::instance()->insertNoteInfo(noteInfo);
            SCNoteThread::instance()->sendNoteEvent(noteInfo);
        }
        else
        {
            NetworkDesc_t netInfo;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mRelPriKey = noteInfo.mRelPriKey;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}
