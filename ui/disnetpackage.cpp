#include "disnetpackage.h"
#include "ui_disnetpackage.h"

#include "mainctrl.h"

#include "portcomm.h"

#include <QTextEdit>
#include <QString>
#include "t_basic_terminal.h"
#include "t_basic_delivery.h"
#include "t_temp_consume_record.h"
#include <QBitmap>
#include <QTabBar>

#include "t_basic_terminal.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include "t_basic_sync_forbidden.h"
#include "t_basic_cell_order.h"

#include "servercomm.h"
#include "db.h"
#include "networkthread.h"


#define HOME_KEEP_TIME 10*1000

disnetPackage::disnetPackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disnetPackage)
{
    ui->setupUi(this);

    initHomePage();
}

disnetPackage::~disnetPackage()
{
    delete ui;
}

void disnetPackage::initHomePage()
{
    myStatus = ST_NONE;

    this->setWindowFlags(Qt::FramelessWindowHint);
    QPixmap pixmap("./images/homepage.png");
    this->setMask(pixmap.mask());
    this->resize(pixmap.size() );
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(pixmap) );
    this->setPalette(palette);

    myMsgBox.setCheckTimer(5);
    cardMsgBox.setCheckTimer(5);
    homeTimer.setInterval(HOME_KEEP_TIME);
    homeTimer.setSingleShot(true);

    ui->tabWidget->setCurrentIndex(3);
    ui->tabWidget->setFocusPolicy(Qt::NoFocus);

   // ui->tabWidget->tabBar()->setStyleSheet("border-image: url(:/page/images/main/tab-1.png);min-width:112px;min-height:96px}");
    QSize size = QPixmap(":/page/images/main/tab-1.png").size();
    ui->tabWidget->setTabIcon(3,QIcon(":/page/images/main/tab-1-1.png"));
    ui->tabWidget->setTabIcon(2,QIcon(":/page/images/main/tab-2.png"));
    ui->tabWidget->setTabIcon(1,QIcon(":/page/images/main/tab-3.png"));
    ui->tabWidget->setTabIcon(0,QIcon(":/page/images/main/tab-4.png"));
    ui->tabWidget->setIconSize(QSize(size.width()-6,size.height()+8));
//QTabWidget::pane{	border:none;}
    QString style = "QTabBar::tab{border: 0px;border-top-left-radius: 0px;border-top-right-radius: 0px;min-width: 0ex;padding: 0px;min-width:112px;min-height:88px} \
            QTabWidget::pane{border-image: url(:/page/images/main/tab_bg.png)}";
/*
    QString style = "QTabBar::tab:last:!selected {border-image: url(:/page/images/main/tab-1.png);min-width:112px;min-height:96px } \
                     QTabBar::tab:last:selected {border-image: url(:/page/images/main/tab-1-1.png);min-width:112px;min-height:96px }\
        QTabBar::tab:middle:first:!selected {border-image: url(:/page/images/main/tab-3.png);min-width:112px;min-height:96px }\
        QTabBar::tab:middle:first:selected {border-image: url(:/page/images/main/tab-3-1.png);min-width:112px;min-height:96px }\
        QTabBar::tab:first:!selected {border-image: url(:/page/images/main/tab-4.png);min-width:112px;min-height:96px }\
                     QTabBar::tab:first:selected {border-image: url(:/page/images/main/tab-4-1.png);min-width:112px;min-height:96px }\
                     QTabBar::tab:middle:last:!selected {border-image: url(:/page/images/main/tab-2.png);min-width:112px;min-height:96px }\
                     QTabBar::tab:middle:last:selected {border-image: url(:/page/images/main/tab-2-1.png);min-width:112px;min-height:96px }";
*/
    ui->tabWidget->setStyleSheet(style);

    ui->saveBtn->setFocusPolicy(Qt::NoFocus);
    ui->saveBtn->setIcon(QIcon(":/page/images/main/saveBtn_bg.png"));
    ui->saveBtn->setMinimumSize(QPixmap(":/page/images/main/saveBtn_bg.png").size());
    ui->saveBtn->setIconSize(ui->saveBtn->size());
    ui->saveBtn->setStyleSheet("background-color:transparent");

    ui->takeBtn->setFocusPolicy(Qt::NoFocus);
    ui->takeBtn->setIcon(QIcon(":/page/images/main/takeBtn_bg.png"));
    ui->takeBtn->setMinimumSize(QPixmap(":/page/images/main/takeBtn_bg.png").size());
    ui->takeBtn->setIconSize(ui->takeBtn->size());
    ui->takeBtn->setStyleSheet("background-color:transparent");

    ui->registerBtn->setFlat(true);
    ui->registerBtn->setFocusPolicy(Qt::NoFocus);
    ui->registerBtn->setIcon(QIcon(":/page/images/main/registerBtn_bg.png"));
    ui->registerBtn->setMinimumSize(QPixmap(":/page/images/main/registerBtn_bg.png").size());
    ui->registerBtn->setIconSize(ui->registerBtn->size());
    ui->registerBtn->setStyleSheet("background-color:transparent");

    ui->senderBtn->setFlat(true);
    ui->senderBtn->setFocusPolicy(Qt::NoFocus);
    ui->senderBtn->setIcon(QIcon(":/page/images/main/senderBtn_bg.png"));
    ui->senderBtn->setMinimumSize(QPixmap(":/page/images/main/senderBtn_bg.png").size());
    ui->senderBtn->setIconSize(ui->registerBtn->size());
    ui->senderBtn->setStyleSheet("background-color:transparent");

    ui->tempStoragePushButton->setFlat(true);
    ui->tempStoragePushButton->setFocusPolicy(Qt::NoFocus);
    ui->tempStoragePushButton->setIcon(QIcon(":/page/images/main/storageBtn_bg.png"));
    ui->tempStoragePushButton->setMinimumSize(QPixmap(":/page/images/main/storageBtn_bg.png").size());
    ui->tempStoragePushButton->setIconSize(ui->tempStoragePushButton->size());
    ui->tempStoragePushButton->setStyleSheet("background-color:transparent");

    ui->houseBtn->setFlat(true);
    ui->houseBtn->setFocusPolicy(Qt::NoFocus);
    ui->houseBtn->setIcon(QIcon(":/page/images/main/houseBtn_bg.png"));
    ui->houseBtn->setMinimumSize(QPixmap(":/page/images/main/houseBtn_bg.png").size());
    ui->houseBtn->setIconSize(ui->houseBtn->size());
    ui->houseBtn->setStyleSheet("background-color:transparent");

    ui->rechargeBtn->setFlat(true);
    ui->rechargeBtn->setFocusPolicy(Qt::NoFocus);
    ui->rechargeBtn->setIcon(QIcon(":/page/images/main/rechargeBtn_bg.png"));
    ui->rechargeBtn->setMinimumSize(QPixmap(":/page/images/main/rechargeBtn_bg.png").size());
    ui->rechargeBtn->setIconSize(ui->rechargeBtn->size());
    ui->rechargeBtn->setStyleSheet("background-color:transparent");

    ui->balanceBtn->setFlat(true);
    ui->balanceBtn->setFocusPolicy(Qt::NoFocus);
    ui->balanceBtn->setIcon(QIcon(":/page/images/main/balanceBtn_bg.png"));
    ui->balanceBtn->setMinimumSize(QPixmap(":/page/images/main/balanceBtn_bg.png").size());
    ui->balanceBtn->setIconSize(ui->balanceBtn->size());
    ui->balanceBtn->setStyleSheet("background-color:transparent");

    ui->tariffBtn->setFlat(true);
    ui->tariffBtn->setFocusPolicy(Qt::NoFocus);
    ui->tariffBtn->setIcon(QIcon(":/page/images/main/tariff.png"));
    ui->tariffBtn->setMinimumSize(QPixmap(":/page/images/main/tariff.png").size());
    ui->tariffBtn->setIconSize(ui->tariffBtn->size());
    ui->tariffBtn->setStyleSheet("background-color:transparent");


    ui->rechargeHistoryBtn->setFlat(true);
    ui->rechargeHistoryBtn->setFocusPolicy(Qt::NoFocus);
    ui->rechargeHistoryBtn->setIcon(QIcon(":/page/images/main/rechargeHistoryBtn_bg.png"));
    ui->rechargeHistoryBtn->setMinimumSize(QPixmap(":/page/images/main/rechargeHistoryBtn_bg.png").size());
    ui->rechargeHistoryBtn->setIconSize(ui->rechargeHistoryBtn->size());
    ui->rechargeHistoryBtn->setStyleSheet("background-color:transparent");

    ui->modifyINfoBtn->setFlat(true);
    ui->modifyINfoBtn->setFocusPolicy(Qt::NoFocus);
    ui->modifyINfoBtn->setIcon(QIcon(":/page/images/main/modifyinfo.png"));
    ui->modifyINfoBtn->setMinimumSize(QPixmap(":/page/images/main/modifyinfo.png").size());
    ui->modifyINfoBtn->setIconSize(ui->modifyINfoBtn->size());
    ui->modifyINfoBtn->setStyleSheet("background-color:transparent");

    ui->emptyCellBtn->setFlat(true);
    ui->emptyCellBtn->setFocusPolicy(Qt::NoFocus);
    ui->emptyCellBtn->setIcon(QIcon(":/page/images/main/emptyCellBtn_bg.png"));
    ui->emptyCellBtn->setMinimumSize(QPixmap(":/page/images/main/emptyCellBtn_bg.png").size());
    ui->emptyCellBtn->setIconSize(ui->emptyCellBtn->size());
    ui->emptyCellBtn->setStyleSheet("background-color:transparent");

    ui->exptakeBtn->setFlat(true);
    ui->exptakeBtn->setFocusPolicy(Qt::NoFocus);
    ui->exptakeBtn->setIcon(QIcon(":/page/images/main/expTakeBtn_bg.png"));
    ui->exptakeBtn->setMinimumSize(QPixmap(":/page/images/main/expTakeBtn_bg.png").size());
    ui->exptakeBtn->setIconSize(ui->exptakeBtn->size());
    ui->exptakeBtn->setStyleSheet("background-color:transparent");

    ui->manageBtn->setFlat(true);
    ui->manageBtn->setFocusPolicy(Qt::NoFocus);
    ui->manageBtn->setIcon(QIcon(":/page/images/main/manageBtn_bg.png"));
    ui->manageBtn->setMinimumSize(QPixmap(":/page/images/main/manageBtn_bg.png").size());
    ui->manageBtn->setIconSize(ui->manageBtn->size());
    ui->manageBtn->setStyleSheet("background-color:transparent");

    ui->assistBtn->setFlat(true);
    ui->assistBtn->setFocusPolicy(Qt::NoFocus);
    ui->assistBtn->setIcon(QIcon(":/page/images/main/assistBtn_bg.png"));
    ui->assistBtn->setMinimumSize(QPixmap(":/page/images/main/assistBtn_bg.png").size());
    ui->assistBtn->setIconSize(ui->assistBtn->size());
    ui->assistBtn->setStyleSheet("background-color:transparent");

    ui->basicSetBtn->setFlat(true);
    ui->basicSetBtn->setFocusPolicy(Qt::NoFocus);
    ui->basicSetBtn->setIcon(QIcon(":/page/images/main/basicSetBtn_bg.png"));
    ui->basicSetBtn->setMinimumSize(QPixmap(":/page/images/main/basicSetBtn_bg.png").size());
    ui->basicSetBtn->setIconSize(ui->basicSetBtn->size());
    ui->basicSetBtn->setStyleSheet("background-color:transparent");

    ui->noteSetBtn->setFlat(true);
    ui->noteSetBtn->setFocusPolicy(Qt::NoFocus);
    ui->noteSetBtn->setIcon(QIcon(":/page/images/main/noteSetBtn_bg.png"));
    ui->noteSetBtn->setMinimumSize(QPixmap(":/page/images/main/noteSetBtn_bg.png").size());
    ui->noteSetBtn->setIconSize(ui->noteSetBtn->size());
    ui->noteSetBtn->setStyleSheet("background-color:transparent");

    ui->cellSetBtn->setFlat(true);
    ui->cellSetBtn->setFocusPolicy(Qt::NoFocus);
    ui->cellSetBtn->setIcon(QIcon(":/page/images/main/cellSetBtn_bg.png"));
    ui->cellSetBtn->setMinimumSize(QPixmap(":/page/images/main/cellSetBtn_bg.png").size());
    ui->cellSetBtn->setIconSize(ui->cellSetBtn->size());
    ui->cellSetBtn->setStyleSheet("background-color:transparent");


    ui->netwrokConfigBtn->setFlat(true);
    ui->netwrokConfigBtn->setFocusPolicy(Qt::NoFocus);
    ui->netwrokConfigBtn->setIcon(QIcon(":/page/images/main/networkBtn_bg.png"));
    ui->netwrokConfigBtn->setMinimumSize(QPixmap(":/page/images/main/cellSetBtn_bg.png").size());
    ui->netwrokConfigBtn->setIconSize(ui->netwrokConfigBtn->size());
    ui->netwrokConfigBtn->setStyleSheet("background-color:transparent");

    ui->checkDeviceBtn->setFlat(true);
    ui->checkDeviceBtn->setFocusPolicy(Qt::NoFocus);
    ui->checkDeviceBtn->setIcon(QIcon(":/page/images/main/checkDeviceBtn_bg.png"));
    ui->checkDeviceBtn->setMinimumSize(QPixmap(":/page/images/main/cellSetBtn_bg.png").size());
    ui->checkDeviceBtn->setIconSize(ui->checkDeviceBtn->size());
    ui->checkDeviceBtn->setStyleSheet("background-color:transparent");

    ui->label->setText(QString::fromUtf8("版本号：") + SCTerminal::instance()->mVersion_Name);
    ui->label->setStyleSheet("font-family:wenquanyi; font-size:16px; color: rgb(255, 255, 255)");

    connect(ui->saveBtn,SIGNAL(clicked()),this,SLOT(saveBtn_slots()));
    connect(ui->takeBtn,SIGNAL(clicked()),this,SLOT(takeBtn_slots()));
    connect(ui->senderBtn,SIGNAL(clicked()),this,SLOT(senderBtn_slots()));
    connect(ui->tempStoragePushButton,SIGNAL(clicked()),this,SLOT(tempStorageBtn_slots()));

    connect(ui->registerBtn,SIGNAL(clicked()),this,SLOT(register_slots()));
    connect(ui->rechargeBtn,SIGNAL(clicked()),this,SLOT(rechargeBtn_slots()));
    connect(ui->balanceBtn,SIGNAL(clicked()),this,SLOT(balanceBtn_slots()));
    connect(ui->rechargeHistoryBtn,SIGNAL(clicked()),this,SLOT(balanceHistoryBtn_slots()));
    connect(ui->manageBtn,SIGNAL(clicked()),this,SLOT(manageBtn_slots()));
    connect(ui->emptyCellBtn,SIGNAL(clicked()),this,SLOT(emptyCellBtn_slots()));
    connect(ui->cellSetBtn,SIGNAL(clicked()),this,SLOT(cellSetBtn_slots()));
    connect(ui->noteSetBtn,SIGNAL(clicked()),this,SLOT(noteSetBtn_slots()));
    connect(ui->basicSetBtn,SIGNAL(clicked()),this,SLOT(basicSetBtn_slots()));
    connect(ui->houseBtn,SIGNAL(clicked()),this,SLOT(houseBtn_slots()));
    connect(ui->assistBtn,SIGNAL(clicked()),this,SLOT(assistBtn_slots()));
    connect(ui->netwrokConfigBtn,SIGNAL(clicked()),this,SLOT(networkConfigBtn_slots()));
    connect(ui->checkDeviceBtn,SIGNAL(clicked()),this,SLOT(checkDeviceBtn_slots()));
    connect(ui->tariffBtn,SIGNAL(clicked()),this,SLOT(tariffBtn_slots()));
    connect(ui->modifyINfoBtn,SIGNAL(clicked()),this,SLOT(modifyInfoBtn_slots()));
    connect(ui->exptakeBtn,SIGNAL(clicked()),this,SLOT(exptakeBtn_slots()));

    connect(&detainMsgBox,SIGNAL(msgBox_Ok()),this,SLOT(detainPage_slots()));
 //   connect(&detainMsgBox,SIGNAL(msgBox_Cancel()),this,SLOT(detainMsg_cancel_slots()));
    connect(&homeTimer,SIGNAL(timeout()),this,SLOT(retHomePage_slots()));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabPageChange_slots(int)));
}

void disnetPackage::startRetHomePage(bool value)
{
    if(value)
    {
        if(3 != ui->tabWidget->currentIndex())
            homeTimer.start();
    }
    else
    {
        homeTimer.stop();
    }
}


void disnetPackage::retHomePage_slots()
{
    homeTimer.stop();
    ui->tabWidget->setCurrentIndex(3);
}

void disnetPackage::tabPageChange_slots(int id)
{
    if(3 != id)
    {
        homeTimer.stop();
        homeTimer.start();
    }
    if(3 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/page/images/main/tab-1-1.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/page/images/main/tab-2.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/page/images/main/tab-3.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/page/images/main/tab-4.png"));
    }
    else if(2 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/page/images/main/tab-1.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/page/images/main/tab-2-1.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/page/images/main/tab-3.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/page/images/main/tab-4.png"));
    }
    else if(1 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/page/images/main/tab-1.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/page/images/main/tab-2.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/page/images/main/tab-3-1.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/page/images/main/tab-4.png"));
    }
    else if(0 == id)
    {
        ui->tabWidget->setTabIcon(3,QIcon(":/page/images/main/tab-1.png"));
        ui->tabWidget->setTabIcon(2,QIcon(":/page/images/main/tab-2.png"));
        ui->tabWidget->setTabIcon(1,QIcon(":/page/images/main/tab-3.png"));
        ui->tabWidget->setTabIcon(0,QIcon(":/page/images/main/tab-4-1.png"));
    }
}
void disnetPackage::getReadCardInfo(QStringList dataList)
{


    if(dataList.size() < 8)
    {
        myMsgBox.showMsgBox("卡信息不正确。");
        return;
    }

    if(SBalckList::instance()->lossUserInfo.contains(dataList.at(7)))
    {
        myMsgBox.showMsgBox("会员卡被禁用，详情请咨询客服。");
        return;
    }

    //转换
    QByteArray byteArray;
    byteArray.clear();
    byteArray.append(dataList.at(4));
    QByteArray mytesp = QByteArray::fromHex(byteArray);
    QString nameTempStr = QString::fromUtf8(mytesp.data());
    dataList.replace(4,nameTempStr);


    btnEnable(false);
    QString info;

    QString empType = dataList.at(0);
    cardMsgBox.close();
    myMsgBox.close();

    if(empType == "1")//超级管理员
    {
        if(myStatus != ST_NONE)
        {
            if(myStatus != ST_SAVE_DELIVERY_MENU && myStatus != ST_COURIER_PACK )
            {
                QByteArray     outData;
                QDataStream out(&outData,QIODevice::WriteOnly);
                out << dataList;
                SCMainCtrl::instance()->enterStatus(myStatus,outData);
                myStatus = ST_NONE;
            }
            else
            {
                info =  "无此权限。";
                myMsgBox.showMsgBox(info);
            }
        }
    }
    else if(empType == "2")//公司管理员
    {
        if(myStatus != ST_NONE)
        {
            if(myStatus != ST_SAVE_DELIVERY_MENU && myStatus != ST_COURIER_PACK)
            {
                if(checkCardInfo(dataList.at(1)))//检查是不是该设备的公司管理员卡
                {
                    QByteArray     outData;
                    QDataStream out(&outData,QIODevice::WriteOnly);
                    out << dataList;
                    SCMainCtrl::instance()->enterStatus(myStatus,outData);
                    myStatus = ST_NONE;
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
        }
    }
    else if((empType == "3"))//快递员
    {
        if(checkCardInfo(dataList.at(1)))//检查是不是该公司快递员
        {
            if((myStatus== ST_OPENEMPTYDOOR)|| (myStatus== ST_MODIFY_INFO))
            {
                SCMainCtrl::instance()->enterStatus(myStatus,"");
                myStatus = ST_NONE;
            }
            else if(myStatus == ST_COURIER_PACK)
            {
                QList<DelivertDesc_t> deliveryInfo;
                if(SCDelivery::instance()->selectExpTakeDeliveryInfo(dataList.at(6),deliveryInfo))
                {
                    QByteArray     outData;
                    QDataStream out(&outData,QIODevice::WriteOnly);

                    out << dataList;
                    SCMainCtrl::instance()->enterStatus(myStatus,outData);
                    myStatus = ST_NONE;
                }
                else
                {
                    info =  "该终端没有您存放的派件信息！";
                    myMsgBox.showMsgBox(info);
                }
            }
            else if((myStatus == ST_SAVE_DELIVERY_MENU))
            {

                if(checkTempRecordInfo(dataList))
                {
                    QList<DelivertDesc_t> deliveryInfo;
                    qint8 retValue = SCDelivery::instance()->selectSaveDeliveryInfo(dataList.at(6),deliveryInfo);

                    if(1 == retValue  || 3 == retValue)
                    {
                        QByteArray     outData;
                        QDataStream out(&outData,QIODevice::WriteOnly);

                        out << dataList;
                        SCMainCtrl::instance()->enterStatus(ST_PAKEAGE_CONFIRM,outData);
                        myStatus = ST_NONE;
                    }
                    else if(2 == retValue)
                    {
                        mCardInfoList = dataList;
                        info = "您存在滞留派件，是否取出？";
                        detainMsgBox.showMsgBox(info,true,2);
                    }
                    else
                    {
                        qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
                        if((tariffType == 2 || tariffType == 4) && ((dataList.at(5).toDouble() < 0) || (dataList.at(5).toDouble() == 0 && !STCellOrder::instance()->isExpOrderInfo(dataList.at(6)))))
                        {
                            info =  "余额不足，请先充值！";
                            myMsgBox.showMsgBox(info);
                        }
                        else
                        {
                            QByteArray     outData;
                            QDataStream out(&outData,QIODevice::WriteOnly);

                            out << QString("HOME") << dataList;
                            SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);
                            myStatus = ST_NONE;
                        }
                    }
                }
                else
                {
                    info =  "读卡失败。";
                    myMsgBox.showMsgBox(info);
                }
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
    }
    else if(empType == "4")//用户
    {
        if((myStatus == ST_OPENEMPTYDOOR) || (myStatus== ST_MODIFY_INFO))
        {
            SCMainCtrl::instance()->enterStatus(myStatus,"");
            myStatus = ST_NONE;
        }
        else
        {
            info =  "无此权限。";
            myMsgBox.showMsgBox(info);
        }
    }
    else if(empType == "5")//维修卡
    {
        if(checkCardInfo(dataList.at(1)))//检查是不是该设备的公司卡
        {
             if((myStatus == ST_MANAGE) || (myStatus== ST_OPENEMPTYDOOR) || (myStatus== ST_CHECK_DEVICE)|| (myStatus== ST_MODIFY_INFO)|| (myStatus== ST_NETWORK_SET))
             {
                SCMainCtrl::instance()->enterStatus(myStatus,"");
                myStatus = ST_NONE;
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
    }
    else if(empType == "6")
    {
        if(checkCardInfo(dataList.at(1)))//检查是不是该公司卡
        {
            if((myStatus == ST_RECHARGE)|| (myStatus== ST_OPENEMPTYDOOR)|| (myStatus== ST_MODIFY_INFO))
            {
                QByteArray     outData;
                QDataStream out(&outData,QIODevice::WriteOnly);

                out << dataList;
                SCMainCtrl::instance()->enterStatus(myStatus,outData);
                myStatus = ST_NONE;
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
    }
    else if(empType == "7")
    {
        if(checkCardInfo( dataList.at(1)))//检查是不是该公司管理员卡
        {
            if((myStatus == ST_REGISTER)|| (myStatus== ST_OPENEMPTYDOOR)|| (myStatus== ST_MODIFY_INFO))
             {
                 QByteArray     outData;
                 QDataStream out(&outData,QIODevice::WriteOnly);

                 out << dataList;
                 SCMainCtrl::instance()->enterStatus(myStatus,outData);
                 myStatus = ST_NONE;
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
    }
    else if(empType == "8")
    {
        if((myStatus == ST_MANAGE) || (myStatus== ST_OPENEMPTYDOOR) || (myStatus== ST_CHECK_DEVICE)|| (myStatus== ST_MODIFY_INFO))
        {
           SCMainCtrl::instance()->enterStatus(myStatus,"");
           myStatus = ST_NONE;
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


void disnetPackage::btnEnable(bool type)
{
    ui->saveBtn->setEnabled(type);
    ui->takeBtn->setEnabled(type);
    ui->senderBtn->setEnabled(type);

    ui->tempStoragePushButton->setEnabled(type);
    ui->registerBtn->setEnabled(type);
    ui->rechargeBtn->setEnabled(type);
    ui->balanceBtn->setEnabled(type);
    ui->rechargeHistoryBtn->setEnabled(type);
    ui->manageBtn->setEnabled(type);
    ui->emptyCellBtn->setEnabled(type);

    ui->assistBtn->setEnabled(type);
    ui->basicSetBtn->setEnabled(type);
    ui->noteSetBtn->setEnabled(type);
    ui->cellSetBtn->setEnabled(type);
    ui->houseBtn->setEnabled(type);

    ui->netwrokConfigBtn->setEnabled(type);
    ui->checkDeviceBtn->setEnabled(type);
    ui->modifyINfoBtn->setEnabled(type);
    ui->tariffBtn->setEnabled(type);
    ui->exptakeBtn->setEnabled(type);

}

bool disnetPackage::checkCardInfo(QString companyNumber)
{
    return SCTerminal::instance()->terminalInfo.companyId.startsWith(companyNumber);
}

bool disnetPackage::checkTempRecordInfo(QStringList &dataList)
{
    bool ret = true;
    double cash = dataList.at(5).toDouble();
    double curcash = 0;
    QString cmmUuid;
    if(STempRecord::instance()->queryTempRecord(dataList.at(7),curcash,cmmUuid))
    {
        if(curcash > 0)
        {
            dataList.replace(5,QString::number(cash - curcash,'f',2));
            QString writeCardInfo;
            for(int i = 0; i < dataList.size(); i++)
            {
                if(4 == i)
                {
                    QByteArray byteAry = dataList.at(i).toUtf8();
                    byteAry = byteAry.toHex();
                    writeCardInfo = writeCardInfo + byteAry + "|";
                }
                else
                {
                    writeCardInfo = writeCardInfo + dataList.at(i) + "|";
                }
            }

            if(!SCPortComm::instance()->writeCardInfo(writeCardInfo))
            {
                ret = false;
            }
            else
            {
                STempRecord::instance()->clearTempRecord(dataList.at(7));
                NetworkDesc_t debtNetWorkInfo = SCServerComm::instance()->repairDebtRecord(cmmUuid);
                SCNetworkThread::instance()->sendNetWorkEvent(debtNetWorkInfo);
            }
        }
    }

    return ret;

}

bool disnetPackage::clickReadCard()
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

bool disnetPackage::readCard_slots()
{
    QStringList myReadDataList;
    myReadDataList.clear();

    if(myStatus != ST_NONE && cardMsgBox.isVisible())
    {
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
    return false;
}

void disnetPackage::detainPage_slots()
{
    QByteArray     outData;
    QDataStream out(&outData,QIODevice::WriteOnly);

    out << mCardInfoList;
    SCMainCtrl::instance()->enterStatus(ST_DETAIN_PACK,outData);
}

void disnetPackage::detainMsg_cancel_slots()
{
    qint8 tariffType = SCTerminal::instance()->terminalInfo.tariffType;
    if((tariffType == 2 || tariffType == 4) && (mCardInfoList.at(5).toDouble() <= 0))
    {
        QString info =  "余额不足，请先充值！";
        myMsgBox.showMsgBox(info);
    }
    else
    {
        QByteArray     outData;
        QDataStream out(&outData,QIODevice::WriteOnly);

        out << QString("HOME") << mCardInfoList;
        SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,outData);
        myStatus = ST_NONE;
    }
}


void disnetPackage::tempStorageBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    if(SCTerminal::instance()->terminalInfo.runStatus == 2)
    {
        QString info =  "系统处于只取不存状态，详情请咨询客服！";
        myMsgBox.showMsgBox(info);
    }
    else
    {
        SCMainCtrl::instance()->enterStatus(ST_TEMP_STORAGE,"");
    }
}

void disnetPackage::saveBtn_slots()
{
    myStatus = ST_SAVE_DELIVERY_MENU;

    if(SCTerminal::instance()->terminalInfo.runStatus == 2)
    {
        QString info =  "系统处于只取不存状态，详情请咨询客服！";
        myMsgBox.showMsgBox(info);
    }
    else if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

//    BasicTempStorage dd;
//    dd.cellId = "0121";
//    dd.cabinetId = "01";
//    QByteArray tempData;
//    tempData.resize(sizeof(BasicTempStorage));
//    memcpy(tempData.data(),&dd,sizeof(BasicTempStorage));

//    QStringList aa;
//    aa << "1" << "2" << "3" << "4" << "5" << "6" << "7";
//    QByteArray outData;
//    QDataStream out(&outData,QIODevice::WriteOnly);
//    out << aa;
   // SCMainCtrl::instance()->enterStatus(ST_PAKEAGE_CONFIRM,outData);
}

void disnetPackage::senderBtn_slots()
{
    if(SCTerminal::instance()->terminalInfo.lanJianPhone.isEmpty())
    {
        myMsgBox.showMsgBox("此功能暂未开启...");
    }
    else if(SCTerminal::instance()->terminalInfo.runStatus == 2)
    {
        QString info =  "系统处于只取不存状态，详情请咨询客服！";
        myMsgBox.showMsgBox(info);
    }
    else
    {
        SCMainCtrl::instance()->enterStatus(ST_SENDER_PACK_MENU,"");
    }
}

void disnetPackage::register_slots()
{
    myStatus = ST_REGISTER;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }


//    QString aa;
//    QByteArray outData;
//    QDataStream out(&outData,QIODevice::WriteOnly);
//    out << aa;
//    SCMainCtrl::instance()->enterStatus(ST_REGISTER,outData);
}

void disnetPackage::takeBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_SAVE_DELIVERY_MENU,"");
   // SCMainCtrl::instance()->enterStatus(ST_GET_DELIVERY,"");
}

void disnetPackage::rechargeBtn_slots()
{
//    mRechargeDialog.exec();
//    int temp  = mRechargeDialog.getTypeValue();

//    if(temp == 1)//本地
//    {
//        myStatus = ST_RECHARGE;
//        if(!readCard())
//        {
//            myMsgBox.showMsgBox("请在插卡区插卡操作...");
//        }
//    }
//    else if(temp == 2)//充值卡
//    {
//        SCMainCtrl::instance()->enterStatus(ST_RECHARGE,"");
//    }
    SCMainCtrl::instance()->enterStatus(ST_RECHARGE_DIAG,"");
}

void disnetPackage::balanceBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    SCMainCtrl::instance()->enterStatus(ST_QUERY_BALANCE,"");
}

void disnetPackage::balanceHistoryBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    SCMainCtrl::instance()->enterStatus(ST_RECHARGE_QUERY,"");
}

void disnetPackage::manageBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_MANAGE;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

  // SCMainCtrl::instance()->enterStatus(ST_MANAGE,"");
}

void disnetPackage::emptyCellBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_OPENEMPTYDOOR;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

    //SCMainCtrl::instance()->enterStatus(ST_OPENEMPTYDOOR,"");
}

void disnetPackage::cellSetBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_CABINET_CELL_SET;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }


 //   SCMainCtrl::instance()->enterStatus(ST_CABINET_CELL_SET,"");
}

void disnetPackage::noteSetBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_NOTE_SET;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

 //  SCMainCtrl::instance()->enterStatus(ST_NOTE_SET,"");
}

void disnetPackage::basicSetBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_BASIC_SET;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }
  //  SCMainCtrl::instance()->enterStatus(ST_BASIC_SET,"");
}

void disnetPackage::houseBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOUSE_SERVICE,"");
}

void disnetPackage::assistBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);

    SCMainCtrl::instance()->enterStatus(ST_ASSIST,"");
    //myMsgBox.showMsgBox("正在建设中...");
}

void disnetPackage::networkConfigBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_NETWORK_SET;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

  //  SCMainCtrl::instance()->enterStatus(ST_NETWORK_SET,"");
}

void disnetPackage::checkDeviceBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_CHECK_DEVICE;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }

  //  SCMainCtrl::instance()->enterStatus(ST_CHECK_DEVICE,"");
}

void disnetPackage::tariffBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    SCMainCtrl::instance()->enterStatus(ST_TARIFF,"");
}

void disnetPackage::modifyInfoBtn_slots()
{
//    homeTimer.setInterval(HOME_KEEP_TIME);
//    myStatus = ST_MODIFY_INFO;

//    if(!clickReadCard())
//    {
//        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
//    }
    SCMainCtrl::instance()->enterStatus(ST_MODIFY_INFO,"");
}

void disnetPackage::exptakeBtn_slots()
{
    homeTimer.setInterval(HOME_KEEP_TIME);
    myStatus = ST_COURIER_PACK;

    if(!clickReadCard())
    {
        cardMsgBox.showMsgBox("请在插卡区插卡操作...");
    }
}



