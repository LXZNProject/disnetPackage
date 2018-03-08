#include "housepage.h"
#include "ui_housepage.h"

#include "t_basic_terminal.h"
#include "t_cabinet_cell.h"
#include "mainctrl.h"
#include <QUuid>

housepage::housepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::housepage)
{
    ui->setupUi(this);

    initHousePage();
}

housepage::~housepage()
{
    delete ui;
}

void housepage::initHousePage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    keypage = new keyboard(ui->widget);
    ui->widget->setStyleSheet("background-image: url(:/key/images/keyboard/key_bg.png)");

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/house/images/house/dry_bg.png")));
    this->setPalette(palette);

    ui->phoneLabel->setFixedSize(185,20);
    ui->phoneLabel->setPixmap(QPixmap(":/storage/images/storage/word1.png"));

    ui->againPhoneLabel->setFixedSize(185,20);
    ui->againPhoneLabel->setPixmap(QPixmap(":/storage/images/storage/word2.png"));

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

    connect(ui->phoneLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
    connect(ui->againPhoneLineEdit,SIGNAL(textChanged(QString)),this,SIGNAL(resetTime()));
}

void housepage::readShowInfo(QByteArray param)
{
    mPayCoin = 0;
    checkBoxStatus(NO_ST);
    setEableEdit(false);

    ui->phoneLineEdit->setFocus();
    ui->phoneLineEdit->clear();
    ui->againPhoneLineEdit->clear();
    bigCellList.clear();
    middleCellList.clear();
    smallCellList.clear();
    notOpenBoxList.clear();


    ST_cabinet_cell::instance()->getSaveCellInfo(bigCellList,middleCellList,smallCellList);

    setEableEdit(true);
    bigBoxLab->setText(QString("%1").arg(bigCellList.size()));
    middleBoxLab->setText(QString("%1").arg(middleCellList.size()));
    smallBoxLab->setText(QString("%1").arg(smallCellList.size()));
}

void housepage::setEableEdit(bool status)
{
    if(status)
    {
        ui->phoneLineEdit->setReadOnly(false);
        ui->againPhoneLineEdit->setReadOnly(false);

        ui->okBtn->setEnabled(true);
        ui->quitBtn->setEnabled(true);
    }
    else
    {
        ui->phoneLineEdit->setReadOnly(true);
        ui->againPhoneLineEdit->setReadOnly(true);

        ui->okBtn->setEnabled(false);
        ui->quitBtn->setEnabled(false);
    }
}

void housepage::updateBox()
{
    bigBoxLab->setText(QString("%1").arg(bigCellList.size()));
    middleBoxLab->setText(QString("%1").arg(middleCellList.size()));
    smallBoxLab->setText(QString("%1").arg(smallCellList.size()));


    QString bigbox = bigBoxLab->text();
    QString middlebox = middleBoxLab->text();
    QString smallbox = smallBoxLab->text();


    if("0" == bigbox)
    {
        //不能选中,告知大箱为false
        bigCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    if("0" == middlebox)
    {
        middleCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
    if("0" == smallbox)
    {
        smallCheckedLab->setPixmap(QPixmap(":/save/images/saveStart/no.png"));
    }
}

void housepage::checkBoxStatus(BTN_STATUS status)
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

bool housepage::inputCheck()
{
    QRegExp rxcellphone("^1\\d{10}");
    bool ret = true;
    QString info;

    //下面判断的（后期用到）
    if(!rxcellphone.exactMatch(ui->phoneLineEdit->text().trimmed()))
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
    else if(SCTerminal::instance()->terminalInfo.washSenderTariff > mPayCoin)
    {
        ret = false;
        info = QString("请投币%1元").arg(SCTerminal::instance()->terminalInfo.washSenderTariff - mPayCoin);
    }

    if (!ret)
    {
        qint8 btnNum = 1;
        myMsgBox.showMsgBox(info,true,btnNum,Qt::NonModal);
    }

    return ret;
}

bool housepage::openSelectLocks(QStringList *lockList)
{
    bool ok = false;
    bool ret = false;
    if (!lockList->size())
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
                //QString notCheckCell = lockList->last();
                //notCheckBoxList.append(notCheckCell);
                lockList->removeLast();
                updateBox();
                continue;
            }
        }

        if (lockoperation.openLock(cabinetId,cellId) == 1)
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
            updateBox();

        }
    }

    return ret;
}

void housepage::packageData(QByteArray &data)
{
    QByteArray tempData;
    tempData.resize(sizeof(SenderDesc_t));
    memcpy(tempData.data(),&savePackInfo,sizeof(SenderDesc_t));

    QDataStream out(&data,QIODevice::WriteOnly);
    out << tempData << notOpenBoxList ;
}

void housepage::buttonclick_slots(int buttonid)
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


bool housepage::event(QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *myEvent = static_cast<QKeyEvent *>(event);
        if(myEvent->key() == Qt::Key_Insert)
        {
            emit resetTime();
            mPayCoin++;
            if(mPayCoin >= SCTerminal::instance()->terminalInfo.washSenderTariff)
            {
                myMsgBox.close();
                okBtn_slots();
            }
        }
    }

    return  QWidget::event(event);
}

bool housepage::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->phoneLineEdit || obj == ui->againPhoneLineEdit)
    {
        QLineEdit *myobj = static_cast<QLineEdit *>(obj);
        if(event->type() == QEvent::FocusOut)
        {
            myobj->setEchoMode(QLineEdit::PasswordEchoOnEdit);
            if(obj == ui->againPhoneLineEdit && (!ui->phoneLineEdit->hasFocus()))
            {
                this->setFocus();
            }
        }
        else if(event->type() == QEvent::FocusIn)
        {
            myobj->setEchoMode(QLineEdit::Normal);
        }

    }

    return QWidget::eventFilter(obj,event);
}


void housepage::packageConfirmInfo()
{
    SCSender::instance()->clearDeliveryInfo(savePackInfo);
    //保存发送给后台的信息
    QUuid uuId = QUuid::createUuid();
    QString strUuId = uuId.toString();
    strUuId = strUuId.remove(QRegExp("[{}-]"));
    strUuId = strUuId.toUpper();

    savePackInfo.lanJianId = strUuId;
    savePackInfo.mobilephone = ui->phoneLineEdit->text().trimmed();

    //保存显示列表的信息
   // stockinfo.Id = QString("%1").arg(stockList.length()+1);
  //  stockInfo.expressId = this->lineEdit->text();
  //  stockInfo.mobilePhone = this->phoneLineEdit->text();

    //减去占用的箱子并记录进发给后台数据中
    if(this->Cur_Status == BIG_ST)
    {
        savePackInfo.cellType = 1;
        savePackInfo.cabinetId = bigCellList.last().left(2);
        savePackInfo.cellId   = bigCellList.last();
  //      qDebug() << "大箱";
        this->bigCellList.removeLast();
    }
    else if(this->Cur_Status == MID_ST)
    {
        savePackInfo.cellType = 2;
        savePackInfo.cabinetId   =  middleCellList.last().left(2);
        savePackInfo.cellId   =  middleCellList.last();
        this->middleCellList.removeLast();
    }
    else if(this->Cur_Status == SMA_ST)
    {
        savePackInfo.cellType = 3;
        savePackInfo.cabinetId   = smallCellList.last().left(2);
        savePackInfo.cellId   = smallCellList.last();
        this->smallCellList.removeLast();
    }

    savePackInfo.deliveryType = 2;
    savePackInfo.status = 1;                                   // 默认状态是存入
    savePackInfo.createtime = QDateTime::currentDateTime();
    savePackInfo.lanJianName = SCTerminal::instance()->terminalInfo.washerName;
    savePackInfo.lanJianPhone = SCTerminal::instance()->terminalInfo.washerPhone;
    savePackInfo.ownerBasePay = SCTerminal::instance()->terminalInfo.washSenderTariff;
    savePackInfo.hasPayMoney =  mPayCoin;     //savePackInfo.ownerBasePay;
    savePackInfo.expressPay = SCTerminal::instance()->terminalInfo.washGeterTariff;
    savePackInfo.cashMoney = mPayCoin;
    savePackInfo.goodsCheckStatus = 3;                 // 1有物品、2无物品 3无物品检测条(默认）
    savePackInfo.switchCharge = 1;
    savePackInfo.senderPhotoPath = "";
    savePackInfo.getterPhotoPath = "";
}

void housepage::okBtn_slots()
{
    emit resetTime();
    QByteArray data;
    QStringList* list;
    QString info;

    if (inputCheck())
    {
        setEableEdit(false);
        if (Cur_Status == BIG_ST)
        {
            list = &bigCellList;
            info = "大号箱门打不开，重新选择箱门大小!";
        }
        else if(Cur_Status == MID_ST)
        {
            list = &middleCellList;
            info = "中号箱门打不开，重新选择箱门大小!";
        }
        else if (Cur_Status == SMA_ST)
        {
            list = &smallCellList;
            info = "小号箱门打不开，重新选择箱门大小!";
        }
        else
        {
            qDebug() << "Cur_Status is error";
            return ;
        }

        if(openSelectLocks(list))   //打开箱子
        {
            packageConfirmInfo();    //记录当前要显示信息与要发后台的存件信息
            packageData(data);

            SCMainCtrl::instance()->enterStatus(ST_HOUSE_PACK_WAIT, data); //存件等待确认

        }
        else
        {
            checkBoxStatus(NO_ST);
            myMsgBox.showMsgBox(info);
        }
        setEableEdit(true);
    }
}

void housepage::quitBtn_slots()
{
    ui->phoneLineEdit->clear();
    ui->againPhoneLineEdit->clear();
    myMsgBox.close();
    SCMainCtrl::instance()->enterStatus(ST_HOME, ""); //存件等待确认
}
