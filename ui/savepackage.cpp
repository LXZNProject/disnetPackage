#include "savepackage.h"
#include "ui_savepackage.h"
#include "t_basic_terminal.h"
#include "t_cabinet_cell.h"
#include "t_basic_sync_forbidden.h"
#include <QUuid>
#include "mainctrl.h"
#include "mylineedit.h"
#include "driver.h"
//#include "syszuxim.h"
#include <string.h>
#include <QBitmap>
#include "portcomm.h"
#include "scancomm.h"

savepackage::savepackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::savepackage)
{
    ui->setupUi(this);
    initSavePage();
}

savepackage::~savepackage()
{
    delete ui;
}


void savepackage::initSavePage()
{
    keypage = new keyboard(ui->widget);
    ui->widget->setStyleSheet("background-image: url(:/key/images/keyboard/key_bg.png)");


    this->setFixedSize(800,600);
    QPixmap pixmap(":/save/images/saveStart/save_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->idLabel->setFixedSize(185,20);
    ui->idLabel->setPixmap(QPixmap(":/save/images/saveStart/word2.png"));
    ui->phoneLabel->setFixedSize(185,20);
    ui->phoneLabel->setPixmap(QPixmap(":/save/images/saveStart/word3.png"));
    ui->againPhoneLabel->setFixedSize(185,20);
    ui->againPhoneLabel->setPixmap(QPixmap(":/save/images/saveStart/word4.png"));

    ui->barCodeLineEdit->installEventFilter(this);
    ui->barCodeLineEdit->setPlaceholderText(QString::fromUtf8("请扫描"));
    ui->barCodeLineEdit->setMaxLength(25);
    ui->barCodeLineEdit->setFixedSize(200,30);
    ui->barCodeLineEdit->bindKeyPage(keypage);

    ui->phoneLineEdit->installEventFilter(this);
    ui->phoneLineEdit->setFixedSize(200,30);
    ui->phoneLineEdit->bindKeyPage(keypage);

    ui->againPhoneLineEdit->installEventFilter(this);
    ui->againPhoneLineEdit->setFixedSize(200,30);
    ui->againPhoneLineEdit->bindKeyPage(keypage);

    //小号
    ui->smallBoxBtn->setFixedSize(150,80);
    ui->smallBoxBtn->setStyleSheet("background-color:transparent");
    ui->smallBoxBtn->setFocusPolicy(Qt::NoFocus);

    smallBoxLab = new QLabel(ui->smallBoxBtn); //显示箱子剩余量
    smallBoxLab->resize(30,30);
    smallBoxLab->move(0,55);
    smallBoxLab->setAlignment(Qt::AlignCenter);
    smallBoxLab->setStyleSheet("font-family:wenquanyi; font-size:20px; color:rgb(255, 170, 0)");

    smallTextLab = new QLabel(ui->smallBoxBtn);   //小号箱的图片
    smallTextLab->resize(QPixmap(":/save/images/saveStart/small.png").size());
    smallTextLab->move(30,0);
    smallTextLab->setPixmap(QPixmap(":/save/images/saveStart/small.png"));

    smallCheckedLab = new QLabel(ui->smallBoxBtn);  //是否选中图片
    smallCheckedLab->resize(QPixmap(":/save/images/saveStart/no.png").size());
    smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    smallCheckedLab->move(100,60);

    //中号
    ui->middleBoxBtn->setFixedSize(150,80);
    ui->middleBoxBtn->setStyleSheet("background-color:transparent");
    ui->middleBoxBtn->setFocusPolicy(Qt::NoFocus);

    middleBoxLab = new QLabel(ui->middleBoxBtn);
    middleBoxLab->resize(30,30);
    middleBoxLab->move(0,55);
    middleBoxLab->setAlignment(Qt::AlignCenter);
    middleBoxLab->setStyleSheet("font-family:wenquanyi; font-size:20px; color:rgb(255, 170, 0)");

    middleTextLab = new QLabel(ui->middleBoxBtn);
    middleTextLab->resize(QPixmap(":/save/images/saveStart/middle.png").size());
    middleTextLab->move(30,0);
    middleTextLab->setPixmap(QPixmap(":/save/images/saveStart/middle.png"));

    middleCheckedLab = new QLabel(ui->middleBoxBtn);
    middleCheckedLab->resize(QPixmap(":/save/images/saveStart/no.png").size());
    middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    middleCheckedLab->move(100,60);

    //大号
    ui->bigBoxBtn->setFixedSize(150,80);
    ui->bigBoxBtn->setStyleSheet("background-color:transparent");
    ui->bigBoxBtn->setFocusPolicy(Qt::NoFocus);

    bigBoxLab = new QLabel(ui->bigBoxBtn);
    bigBoxLab->resize(30,30);
    bigBoxLab->move(0,55);
    bigBoxLab->setAlignment(Qt::AlignCenter);
    bigBoxLab->setStyleSheet("font-family:wenquanyi; font-size:20px; color:rgb(255, 170, 0)");


    bigTextLab = new QLabel(ui->bigBoxBtn);
    bigTextLab->resize(QPixmap(":/save/images/saveStart/big.png").size());
    bigTextLab->move(30,0);
    bigTextLab->setPixmap(QPixmap(":/save/images/saveStart/big.png"));

    bigCheckedLab = new QLabel(ui->bigBoxBtn);
    bigCheckedLab->resize(QPixmap(":/save/images/saveStart/no.png").size());
    bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    bigCheckedLab->move(100,60);

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

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(ui->bigBoxBtn,0);
    buttonGroup->addButton(ui->middleBoxBtn,1);
    buttonGroup->addButton(ui->smallBoxBtn,2);
    buttonGroup->setExclusive(true);  //互斥


    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(buttonGroup,SIGNAL(buttonClicked(int)),this,SLOT(buttonclick_slots(int)));

    connect(SCScanComm::instance(),SIGNAL(scanSignal(QString)),this,SLOT(startLoadScan(QString)));
    connect(ui->barCodeLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
    connect(ui->barCodeLineEdit,SIGNAL(returnPressed()),this,SLOT(codeSCan_slots()));
    connect(ui->barCodeLineEdit,SIGNAL(editingFinished()),SCScanComm::instance(),SLOT(closeReadCom()));
    connect(ui->phoneLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
    connect(ui->againPhoneLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));

}

void savepackage::buttonclick_slots(int buttonid)
{
    emit resetTime();
    if(0 == buttonid && bigBoxLab->text() != "0")
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));

        this->Cur_Status = BIG_ST;
    }
    if(1 == buttonid && middleBoxLab->text() != "0" )
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        this->Cur_Status = MID_ST;
    }
    if(2 == buttonid && smallBoxLab->text() != "0")
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
        this->Cur_Status = SMA_ST;
    }

}

void savepackage::codeSCan_slots()
{
    if(!ui->barCodeLineEdit->text().trimmed().isEmpty())
    {
        this->focusNextChild();
    }
}

bool savepackage::eventFilter(QObject *obj, QEvent *event)
{
   // SyszuxIM::hideInputMethod();
    if(obj == ui->barCodeLineEdit)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            loadcodeScan();
        }
    }
    if(obj == ui->phoneLineEdit || obj == ui->againPhoneLineEdit)
    {
        QRegExp rxcellphone("^1\\d{10}");
        QLineEdit *myobj = static_cast<QLineEdit *>(obj);
        if(event->type() == QEvent::FocusOut)
        {
            myobj->setEchoMode(QLineEdit::PasswordEchoOnEdit);
            if(obj == ui->againPhoneLineEdit && (!ui->phoneLineEdit->hasFocus()))
            {
                this->setFocus();
            }

            if(ui->phoneLineEdit->hasFocus())
            {
                if(rxcellphone.exactMatch(ui->againPhoneLineEdit->text().trimmed()))
                {
                    qDebug() << "focus";
                    checkIsUserOrder(ui->againPhoneLineEdit->text().trimmed());
                }
            }
            else if(ui->againPhoneLineEdit->hasFocus())
            {
                if(rxcellphone.exactMatch(ui->phoneLineEdit->text().trimmed()))
                {
                    qDebug() << "focus";
                    checkIsUserOrder(ui->phoneLineEdit->text().trimmed());
                }
            }


        }
        else if(event->type() == QEvent::FocusIn)
        {
            myobj->setEchoMode(QLineEdit::Normal);
        }

    }

    return QWidget::eventFilter(obj,event);
}


void savepackage::updateBox()
{

    bigBoxLab->setText(QString("%1").arg(tempBigCellList.size()));
    middleBoxLab->setText(QString("%1").arg(tempMiddleCellList.size()));
    smallBoxLab->setText(QString("%1").arg(tempSmallCellList.size()));


    QString bigbox = bigBoxLab->text();
    QString middlebox = middleBoxLab->text();
    QString smallbox = smallBoxLab->text();

    if("0" == bigbox && this->Cur_Status == BIG_ST)
    {
        //不能选中,告知大箱为false
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    if("0" == middlebox && this->Cur_Status == MID_ST)
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    if("0" == smallbox && this->Cur_Status == SMA_ST)
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }


}

void savepackage::checkBoxStatus(BTN_STATUS status)
{
    this->Cur_Status = status;

    if(status == BIG_ST)
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    else if(status == MID_ST)
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    else if(status == SMA_ST)
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/yes.png"));
    }
    else
    {
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
}

void savepackage::readShowInfo(QByteArray param)
{
    checkBoxStatus(NO_ST);
    setEableEdit(false);

    initList(param);

    setEableEdit(true);
}

void savepackage::setEableEdit(bool status)
{
    if(status)
    {
        ui->barCodeLineEdit->setReadOnly(false);
        ui->phoneLineEdit->setReadOnly(false);
        ui->againPhoneLineEdit->setReadOnly(false);
        ui->smallBoxBtn->setEnabled(true);
        ui->middleBoxBtn->setEnabled(true);
        ui->bigBoxBtn->setEnabled(true);

        ui->okBtn->setEnabled(true);
        ui->quitBtn->setEnabled(true);
    }
    else
    {
        ui->barCodeLineEdit->setReadOnly(true);
        ui->phoneLineEdit->setReadOnly(true);
        ui->againPhoneLineEdit->setReadOnly(true);
        ui->smallBoxBtn->setEnabled(false);
        ui->middleBoxBtn->setEnabled(false);
        ui->bigBoxBtn->setEnabled(false);

        ui->okBtn->setEnabled(false);
        ui->quitBtn->setEnabled(false);
    }
}

bool savepackage::loadcodeScan()
{
    //播放音乐
    QString mp3file = "./music/scan.mp3";
    QString vol = "15";
    SCDriver::instance()->playVoice(mp3file,vol);
    SCScanComm::instance()->openMyCom();

    ui->barCodeLineEdit->setFocus();


    return false;
}

void savepackage::startLoadScan(QString param)
{
    if(ui->barCodeLineEdit == this->focusWidget())
    {
        ui->barCodeLineEdit->setText(param);
        this->focusNextChild();
    }
}

bool savepackage::inputCheck()
{
    QRegExp rxcellphone("^1\\d{10}");
    bool ret = true;
    QString info;
    QStringList readCardList;
 //   double tempBalance = mBalance;

    //下面判断的（后期用到）
    if(ui->barCodeLineEdit->text() == "")
    {
        info = "请扫入快递单号。";
        ret = false;
    }
    else if(!rxcellphone.exactMatch(ui->phoneLineEdit->text().trimmed()))
    {
        ret = false;
        info = "手机号码输入有误！请输入正确号码。";
    }
    else if (ui->phoneLineEdit->text().trimmed() != ui->againPhoneLineEdit->text().trimmed())
    {
        ret = false;
        info = "手机号码输入不一致，请重新确认。";
    }
    else if(Cur_Status == NO_ST)  //选中箱号
    {
        ret = false;
        info = "选择箱子类型。";
    }
    else if(SBalckList::instance()->balckUserInfo.contains(ui->phoneLineEdit->text().trimmed()))
    {
        ret = false;
        info = "此收件人要求包裹不放入自助包裹站，请快递员直接派送！";
    }
 /* else if(SCPortComm::instance()->queryCardInfo(readCardList) == -1)
    {
        ret = false;
        info = "请插入您的会员卡。";
    }
    */
    else if(SCPortComm::instance()->queryCardInfo(readCardList)>0 && !readCardList.contains(cardInfoList.at(7)))
    {
        ret = false;
        info = "卡信息不一致，请重新登陆。";
    }
/*    else if(2 == SCTerminal::instance()->terminalInfo.tariffType || 4 == SCTerminal::instance()->terminalInfo.tariffType)
    {
        if(Cur_Status == BIG_ST)
        {
            if(expOrderBigCellList.isEmpty())
            {
                tempBalance = mBalance - SCTerminal::instance()->terminalInfo.bigTariff;
            }
        }
        else if(Cur_Status == MID_ST)
        {
            if(expOrderMiddleCellList.isEmpty())
            {
                tempBalance = mBalance - SCTerminal::instance()->terminalInfo.middleTariff;
            }
        }
        else if(Cur_Status == SMA_ST)
        {
            if(expOrderSmallCellList.isEmpty())
            {
                tempBalance = mBalance - SCTerminal::instance()->terminalInfo.smallTariff;
            }
        }

        if(tempBalance < 0)
        {
            ret = false;
            info = "余额不足，请先充值。";
        }
        else
        {
            mBalance = tempBalance;
        }
    }
    */
    if(!ret)
    {
         myMsgBox.showMsgBox(info);
    }

    return ret;
}

bool savepackage::openSelectLocks(QStringList *lockList,QStringList *startList)
{
    bool ok = false;
    bool ret = false;
    if(!lockList->size())
        return false;


    while(lockList->size())
    {
        QString curLockString = lockList->last();
        int boxLength = curLockString.length();
        qint8 cabinetId = curLockString.mid(boxLength - 4,2).toInt(&ok,10);
        qint8 cellId = curLockString.mid(boxLength - 2,2).toInt(&ok,10);

        if(1 == SCTerminal::instance()->terminalInfo.goodsCheck) //有物品检测条
        {
            if(lockoperation.queryGoods(cabinetId,cellId) == 0)
            {
                //emit resetTime();
                QString notCheckCell = lockList->last();
                notCheckBoxList.append(notCheckCell);
                lockList->removeLast();
                startList->removeOne(notCheckCell);
                updateBox();
                continue;
            }
        }

        if(lockoperation.openLock(cabinetId,cellId) == 1)
        {
            ret = true;
            break;
        }
        else
        {
            //emit resetTime();
            QString notOpenCell = lockList->last();
            notOpenBoxList.append(notOpenCell);
            lockList->removeLast();
            startList->removeOne(notOpenCell);
            updateBox();

        }
    }

    return ret;
}

void savepackage::packageData(QByteArray &data)
{
    QByteArray tempData;
    tempData.resize(sizeof(DelivertDesc_t));
    memcpy(tempData.data(),&savePackInfo,sizeof(DelivertDesc_t));

    QDataStream out(&data,QIODevice::WriteOnly);
    out << tempData << cardInfoList << notOpenBoxList << notCheckBoxList;
}

void savepackage::packageConfirmInfo()
{
    SCDelivery::instance()->clearDeliveryInfo(savePackInfo);
    //保存发送给后台的信息
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    savePackInfo.deliveryId = strUuId;
    savePackInfo.packageId = ui->barCodeLineEdit->text().trimmed();
    savePackInfo.mobilephone = ui->phoneLineEdit->text().trimmed();
    savePackInfo.expName = cardInfoList.at(2);
    savePackInfo.expMobilePhone = cardInfoList.at(3);
    if(cardInfoList.at(4) == QString::fromUtf8("其他"))
    {
        savePackInfo.expCompanyName = QString::fromUtf8("快递");
    }
    else
    {
        savePackInfo.expCompanyName = cardInfoList.at(4);
    }
    savePackInfo.m1card = cardInfoList.at(6);

    //保存显示列表的信息
   // stockinfo.Id = QString("%1").arg(stockList.length()+1);
  //  stockInfo.expressId = this->lineEdit->text();
  //  stockInfo.mobilePhone = this->phoneLineEdit->text();

    //减去占用的箱子并记录进发给后台数据中
    if(this->Cur_Status == BIG_ST)
    {
        savePackInfo.cellType = 1;
        savePackInfo.cabinetId = tempBigCellList.last().left(2);
        savePackInfo.cellId   = tempBigCellList.last();
  //      qDebug() << "大箱";
        this->tempBigCellList.removeLast();
        bigCellList.removeAll(savePackInfo.cellId);
    }
    else if(this->Cur_Status == MID_ST)
    {
        savePackInfo.cellType = 2;
        savePackInfo.cabinetId   =  tempMiddleCellList.last().left(2);
        savePackInfo.cellId   =  tempMiddleCellList.last();
        this->tempMiddleCellList.removeLast();
        middleCellList.removeAll(savePackInfo.cellId);
    }
    else if(this->Cur_Status == SMA_ST)
    {
        savePackInfo.cellType = 3;
        savePackInfo.cabinetId   = tempSmallCellList.last().left(2);
        savePackInfo.cellId   = tempSmallCellList.last();
        this->tempSmallCellList.removeLast();
        smallCellList.removeAll(savePackInfo.cellId);
    }

    savePackInfo.createtime = QDateTime::currentDateTime();

    savePackInfo.tariffType = SCTerminal::instance()->terminalInfo.tariffType; //收费类型

    if(2 == savePackInfo.tariffType || 4 == savePackInfo.tariffType)
    {
        if(this->Cur_Status == BIG_ST)
        {
            if(!expOrderBigCellList.contains(savePackInfo.cellId))
            {
                savePackInfo.expressPay = SCTerminal::instance()->terminalInfo.bigTariff;
            }
        }
        else if(this->Cur_Status == MID_ST)
        {
            if(!expOrderMiddleCellList.contains(savePackInfo.cellId))
            {
                savePackInfo.expressPay = SCTerminal::instance()->terminalInfo.middleTariff;
            }
        }
        else if(this->Cur_Status == SMA_ST)
        {
            if(!expOrderSmallCellList.contains(savePackInfo.cellId))
            {
                savePackInfo.expressPay = SCTerminal::instance()->terminalInfo.smallTariff;
            }
        }
    }

    if(3 == savePackInfo.tariffType || 4 == savePackInfo.tariffType)
    {
        savePackInfo.ownerBasePay = SCTerminal::instance()->terminalInfo.userBasicTariff;
    }

    savePackInfo.hasPayMoney = savePackInfo.expressPay;        //该派件已经支付了多少钱
    savePackInfo.status = 1;                                   // 默认状态是存入
    savePackInfo.payway = 1;                                   //支付方式
    savePackInfo.expireStatus = SCTerminal::instance()->terminalInfo.expireStatus; //用户是否收取滞留费
    savePackInfo.goodsCheckStatus = 3;                         // 1有物品、2无物品 3无物品检测条(默认）

    if(userOrderBigList.contains(savePackInfo.cellId) || userOrderMiddleList.contains(savePackInfo.cellId) || userOrderSmallList.contains(savePackInfo.cellId))
    {
        savePackInfo.switchCharge = 2;                             //派件收费开关
    }
    else
    {
        savePackInfo.switchCharge = 1;
    }
    savePackInfo.senderPhotoPath = "";
    savePackInfo.getterPhotoPath = "";

    savePackInfo.expireHours = SCTerminal::instance()->terminalInfo.expireHours;
    savePackInfo.expireStartTariff = SCTerminal::instance()->terminalInfo.expireStartTariff;
    savePackInfo.expireStep = SCTerminal::instance()->terminalInfo.expireStep;
    savePackInfo.expireStepMoney = SCTerminal::instance()->terminalInfo.expireStepMoney;

}

void savepackage::checkIsUserOrder(QString phone)
{

    QStringList userOrderCellList;

    userOrderBigList.clear();
    userOrderMiddleList.clear();
    userOrderSmallList.clear();
    tempBigCellList.clear();
    tempMiddleCellList.clear();
    tempSmallCellList.clear();

    for(int i = 0; i < cellInfos.size(); i++)
    {
        if(cellInfos.at(i).phone.contains(phone))
        {
            if(1 == cellInfos.at(i).type)
            {
                if(!userOrderBigList.contains(cellInfos.at(i).cellId))
                {
                    userOrderBigList.append(cellInfos.at(i).cellId);
                }
            }
            else if(2 == cellInfos.at(i).type)
            {
                if(!userOrderMiddleList.contains(cellInfos.at(i).cellId))
                {
                    userOrderMiddleList.append(cellInfos.at(i).cellId);
                }
            }
            else if(3 == cellInfos.at(i).type)
            {
                if(!userOrderSmallList.contains(cellInfos.at(i).cellId))
                {
                    userOrderSmallList.append(cellInfos.at(i).cellId);
                }
            }
        }
        if(!userOrderCellList.contains(cellInfos.at(i).cellId))
        {
            userOrderCellList.append(cellInfos.at(i).cellId);
        }
    }

    tempBigCellList << bigCellList;
    tempMiddleCellList << middleCellList;
    tempSmallCellList << smallCellList;

    if(!userOrderCellList.isEmpty())
    {
        tempBigCellList << userOrderBigList;
        tempMiddleCellList << userOrderMiddleList;
        tempSmallCellList << userOrderSmallList;

        for(int i = 0; i < userOrderCellList.size(); i++)
        {
            tempBigCellList.removeOne(userOrderCellList.at(i));
            tempMiddleCellList.removeOne(userOrderCellList.at(i));
            tempSmallCellList.removeOne(userOrderCellList.at(i));
        }

    }

    if(tempBigCellList.isEmpty() && (this->Cur_Status == BIG_ST))
    {
        checkBoxStatus(NO_ST);
    }
    else if(tempMiddleCellList.isEmpty() && (this->Cur_Status == MID_ST))
    {
        checkBoxStatus(NO_ST);
    }
    else if(tempSmallCellList.isEmpty() && (this->Cur_Status == SMA_ST))
    {
        checkBoxStatus(NO_ST);
    }

    updateBox();

    qDebug() << "tempSmallCellList:" << tempSmallCellList;
}

void savepackage::okBtn_slots()
{
    qDebug() << "hello";
    emit resetTime();
    QByteArray data;
    QStringList* list;
    QStringList* startlist;
    QString info;

    if (inputCheck())
    {
        setEableEdit(false);
        if (Cur_Status == BIG_ST)
        {
            list = &tempBigCellList;
            startlist = &bigCellList;
            info = "大号箱门打不开，重新选择箱门大小!";
        }
        else if(Cur_Status == MID_ST)
        {
            list = &tempMiddleCellList;
            startlist = &middleCellList;
            info = "中号箱门打不开，重新选择箱门大小!";
        }
        else if (Cur_Status == SMA_ST)
        {
            list = &tempSmallCellList;
            startlist = &smallCellList;
            info = "小号箱门打不开，重新选择箱门大小!";
        }
        else
        {
            qDebug() << "Cur_Status is error";
            return ;
        }

        if(openSelectLocks(list,startlist))   //打开箱子
        {
            packageConfirmInfo();    //记录当前要显示信息与要发后台的存件信息
            packageData(data);

            SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_WAIT, data); //存件等待确认

        }
        else
        {
            checkBoxStatus(NO_ST);
            myMsgBox.showMsgBox(info);
        }
        setEableEdit(true);
    }
}

void savepackage::quitBtn_slots()
{
    checkBoxStatus(NO_ST);

    setEableEdit(false);

    //closeEventFilter();

    QByteArray data;
    QString isShowList = "NO_PACK"; //不需添加
    QString notCloseBox;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << isShowList << cardInfoList << notOpenBoxList << notCheckBoxList << notCloseBox;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST, data);
    setEableEdit(true);
}


void savepackage::initList(QByteArray param)
{
    QString isLong;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> isLong;
    if("HOME" == isLong)
    {
        QStringList orderCellInfos;                 //所有承包箱体信息

        ui->barCodeLineEdit->clear();
        ui->phoneLineEdit->clear();
        ui->againPhoneLineEdit->clear();

        expOrderBigCellList.clear();
        expOrderMiddleCellList.clear();
        expOrderSmallCellList.clear();

        bigCellList.clear();
        middleCellList.clear();
        smallCellList.clear();
        cellInfos.clear();

        in >> cardInfoList;
        ST_cabinet_cell::instance()->getSaveCellInfo(bigCellList,middleCellList,smallCellList);
        STCellOrder::instance()->getExpOrderInfo(cardInfoList.at(6),expOrderBigCellList,expOrderMiddleCellList,expOrderSmallCellList,orderCellInfos);
        STCellOrder::instance()->getUserOrderInfo(cellInfos);

        if(!orderCellInfos.isEmpty())
        {
            bigCellList << expOrderBigCellList;
            middleCellList << expOrderMiddleCellList;
            smallCellList << expOrderSmallCellList;

            for(int i = 0; i < orderCellInfos.size(); i++)
            {
                bigCellList.removeOne(orderCellInfos.at(i));
                middleCellList.removeOne(orderCellInfos.at(i));
                smallCellList.removeOne(orderCellInfos.at(i));
            }

        }
//        mBalance = cardInfoList.at(5).toDouble();
        notOpenBoxList.clear();
        notCheckBoxList.clear();

        bigBoxLab->setText(QString("%1").arg(bigCellList.size()));
        middleBoxLab->setText(QString("%1").arg(middleCellList.size()));
        smallBoxLab->setText(QString("%1").arg(smallCellList.size()));
    }
    else if("WAIT" == isLong)
    {
        if(1 == savePackInfo.cellType)
        {
            bigCellList.append(savePackInfo.cellId);
            tempBigCellList.append(savePackInfo.cellId);
        }
        else if(2 == savePackInfo.cellType)
        {
            middleCellList.append(savePackInfo.cellId);
            tempMiddleCellList.append(savePackInfo.cellId);
        }
        if(3 == savePackInfo.cellType)
        {
            smallCellList.append(savePackInfo.cellId);
            tempSmallCellList.append(savePackInfo.cellId);
        }
//        mBalance = mBalance + savePackInfo.expressPay;
        this->setFocus();

        bigBoxLab->setText(QString("%1").arg(tempBigCellList.size()));
        middleBoxLab->setText(QString("%1").arg(tempMiddleCellList.size()));
        smallBoxLab->setText(QString("%1").arg(tempSmallCellList.size()));
    }
    else if( "LIST" == isLong)
    {
        ui->barCodeLineEdit->clear();
        ui->phoneLineEdit->clear();
        ui->againPhoneLineEdit->clear();

        bigBoxLab->setText(QString("%1").arg(bigCellList.size()));
        middleBoxLab->setText(QString("%1").arg(middleCellList.size()));
        smallBoxLab->setText(QString("%1").arg(smallCellList.size()));
    }

    SCDelivery::instance()->clearDeliveryInfo(savePackInfo);
}


