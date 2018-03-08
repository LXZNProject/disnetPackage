#include "savepackagewaitpage.h"
#include "ui_savepackagewaitpage.h"

#include "t_basic_terminal.h"
#include "mainctrl.h"

#include <string.h>
#include <QBitmap>
#include <QMessageBox>
#include <QDebug>

#define         OKBTN     1
#define     CANCELBTN     2

savepackagewaitpage::savepackagewaitpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::savepackagewaitpage)
{
    ui->setupUi(this);

    initWaitpage();
}

savepackagewaitpage::~savepackagewaitpage()
{
    delete ui;
}

void savepackagewaitpage::initWaitpage()
{
    this->resize(800,600);
    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap(":/save/images/saveStart/save_bg.png");
    this->setMask(pixmap.mask());
    this->resize( pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    isOpenBox = false;
    waitSaveTime = 120;
    ui->label->setFixedSize(QPixmap(":/save/images/saveWait/word1.png").size());
    ui->label->setPixmap(QPixmap(":/save/images/saveWait/word1.png"));


    ui->timeLabel->setStyleSheet("font-family:wenquanyi; font-weight:bold; font-size:36px; color: rgb(255, 0, 0)");
    ui->timeLabel->setText(QString("%1").arg(120));


    ui->door->resize(QPixmap(":/save/images/saveWait/door.png").size());
    ui->door->setPixmap(QPixmap(":/save/images/saveWait/door.png"));

    ui->doorCabinetId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCabinet->resize(QPixmap(":/save/images/saveWait/doorcabinet.png").size());
    ui->doorCabinet->setPixmap(QPixmap(":/save/images/saveWait/doorcabinet.png"));


    ui->doorCellId->setStyleSheet("font-family:wenquanyi; font-size:30px; color: rgb(0, 255, 64)");

    ui->doorCell->resize(QPixmap(":/save/images/saveWait/doorcell.png").size());
    ui->doorCell->setPixmap(QPixmap(":/save/images/saveWait/doorcell.png"));

    ui->okBtn->setFlat(true);
    ui->okBtn->setEnabled(false);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/save/images/saveWait/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/save/images/saveWait/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setEnabled(false);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/save/images/saveWait/cancelBtn_bg.png"));
    ui->cancelBtn->setMinimumSize(QPixmap(":/save/images/saveWait/cancelBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->cancelBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");


    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtn_slots()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slots()));

    connect(&isOverTime,SIGNAL(timeout()),this,SLOT(overTime()));
    connect(&queryLock,SIGNAL(timeout()),this,SLOT(queryLockStatus()));

}

void savepackagewaitpage::readWaitInfo(QByteArray param)
{
    isOpenBox = false;
    waitSaveTime = 120;
    notCloseBox.clear();
    QByteArray data;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> data >> cardInfoList >> notOpenBoxList >> notCheckBoxList;

    savePackInfo = *(DelivertDesc_t *)data.data();

    ui->timeLabel->setText(QString("%1").arg(waitSaveTime));
    ui->doorCabinetId->setText(savePackInfo.cabinetId);
    ui->doorCellId->setText(savePackInfo.cellId.right(2));

    isOverTime.start(1000);
    queryLock.start(3000);

    setBtnEnable(false);
}

void savepackagewaitpage::setBtnEnable(bool status,qint8 btn)
{
    if(status)
    {
        if(1 == btn)
        {
            ui->okBtn->setEnabled(true);
            ui->okBtn->setIcon(QIcon(":/save/images/saveWait/okBtn.png"));

        }
        else if(2 == btn)
        {
            ui->cancelBtn->setEnabled(true);
            ui->cancelBtn->setIcon(QIcon(":/save/images/saveWait/cancelBtn.png"));
        }
        else
        {
            ui->okBtn->setEnabled(true);
            ui->okBtn->setIcon(QIcon(":/save/images/saveWait/okBtn.png"));
            ui->cancelBtn->setEnabled(true);
            ui->cancelBtn->setIcon(QIcon(":/save/images/saveWait/cancelBtn.png"));
        }
    }
    else
    {
        ui->okBtn->setEnabled(false);
        ui->okBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/okBtn_bg.png)");
        ui->cancelBtn->setEnabled(false);
        ui->cancelBtn->setStyleSheet("background-color:transparent;background-image:url(:/page/images/saveWait/cancelBtn_bg.png)");
    }
}


void savepackagewaitpage::okBtn_slots()
{
    isOverTime.stop();
    QString isShowList = "ADD_PACK";
    QByteArray data;
    QByteArray tempData;
    tempData.resize(sizeof(DelivertDesc_t));
    memcpy(tempData.data(),&savePackInfo,sizeof(DelivertDesc_t));

    QDataStream out(&data,QIODevice::WriteOnly);
    out << isShowList << cardInfoList << notOpenBoxList << notCheckBoxList << notCloseBox << tempData;

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST,data);

}

void savepackagewaitpage::cancelBtn_slots()
{
    QString page = "WAIT";
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);
    out << page;

    isOverTime.stop();

    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,data);

}

void savepackagewaitpage::overTime()
{
    waitSaveTime--;

    if(waitSaveTime <= 0)
    {
        QString isShowList;
        QByteArray data;

        QDataStream out(&data,QIODevice::WriteOnly);
        //自动退出、锁箱、发送信息给后台
        if(isOpenBox)
        {
            isOpenBox = false;
            savePackInfo.status = 3;
            isShowList = "ADD_WAIT_PACK"; //待确认派件

            QByteArray tempData;
            tempData.resize(sizeof(DelivertDesc_t));
            memcpy(tempData.data(),&savePackInfo,sizeof(DelivertDesc_t));

            out << isShowList << cardInfoList << notOpenBoxList << notCheckBoxList << notCloseBox << tempData;

        }
        else
        {
            isShowList = "NO_PACK";  //未关闭的箱门

            notCloseBox = savePackInfo.cellId;
            out << isShowList << cardInfoList << notOpenBoxList << notCheckBoxList << notCloseBox;
        }

        setBtnEnable(false);
        isOverTime.stop();

        queryLock.stop();

        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_LIST,data);
        //发送消息给后台

    }
    else
    {
        ui->timeLabel->setText(QString("%1").arg(waitSaveTime));
    }
}

void savepackagewaitpage::queryLockStatus()
{
    qint8 status = -1;

    qint8 cabinetId = savePackInfo.cellId.left(2).toInt();
    qint8 cellId    = savePackInfo.cellId.right(2).toInt();

    status = lockoperation.queryLock(cabinetId,cellId);
    if(status == 0)
    {
        queryLock.stop();
        isOpenBox = true;

        if(1 == SCTerminal::instance()->terminalInfo.goodsCheck)
        {
            if(0 == lockoperation.queryGoods(cabinetId,cellId))
            {
                savePackInfo.goodsCheckStatus = 1;  //有物品
                okBtn_slots();
            }
            else
            {
                savePackInfo.goodsCheckStatus = 2;  //无物品
                qint8 btnNum = 2;
                myMsgBox.showMsgBox(QString("系统未检测到%1箱%2柜包裹，是否打开重新放置？").arg(cabinetId).arg(cellId),true,btnNum);
            }
        }
        else
        {
            setBtnEnable(true);
        }
    }
}

