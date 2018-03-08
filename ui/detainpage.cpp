#include "detainpage.h"
#include "ui_detainpage.h"
#include "t_basic_delivery.h"
#include "servercomm.h"
#include "t_cabinet_cell.h"
#include "t_log_network.h"
#include "networkthread.h"
#include "notethread.h"
#include "mainctrl.h"
#include "t_log_smslog.h"
#include "t_log_template.h"
#include "t_log_smslimit.h"
#include "t_basic_terminal.h"
#include <QUuid>

detainpage::detainpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::detainpage)
{
    ui->setupUi(this);
    initPage();
}

detainpage::~detainpage()
{
    delete ui;
}

void detainpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/expTake/images/expTake/detain_bg.png")));
    this->setPalette(palette);

    ui->openBtn->setFlat(true);
    ui->openBtn->setFocusPolicy(Qt::NoFocus);
    ui->openBtn->setIcon(QIcon(":/expTake/images/expTake/openBtn_bg.png"));
    ui->openBtn->setMinimumSize(QPixmap(":/expTake/images/expTake/openBtn_bg.png").size());
    ui->openBtn->setIconSize(ui->openBtn->size());
    ui->openBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    QStringList list;
    list<< "序号" << "快递单号" << "手机号码" << "存入时间" << "箱门号" << "状态";

    model = new takeViewModel();
    model->setHorizontalHeaderList(list);

    //水平模型头充满水平区域
    QHeaderView *header;
    header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);

    ui->tableView->verticalHeader()->hide();

    connect(ui->openBtn,SIGNAL(clicked()),this,SLOT(openBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
}

void detainpage::readDetainInfo(QByteArray param)
{
    QList<DelivertDesc_t> deliveryInfo;
    QStringList dataList;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> dataList;
    modelData.clear();
    SCDelivery::instance()->selectSaveDeliveryInfo(dataList.at(6),deliveryInfo);

    SCDelivery::instance()->queryExpDeliveryInfo(deliveryInfo,modelData);

    model->setModelData(modelData);
    model->refrushModel();
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(3,180);
}

void detainpage::openBtn_slots()
{
    ui->openBtn->setEnabled(false);
    ui->quitBtn->setEnabled(false);
    sendInfo.clear();

    emit resetTime();
    int length = modelData.size();
    QStringList list;
    QList<NetCellExcept> exceInfoList;

    while(length)
    {
        list = modelData.at(length-1);
        QString cellStr = list.at(4);
        qint8 cabinetId = cellStr.left(2).toInt();     //柜号
        qint8 cellId    = cellStr.right(2).toInt();     //箱号

        QString cellStatus = list.at(5); //箱门状态

        if(cellStatus == QString("存入"))
        {
            if(lockoperation.openLock(cabinetId,cellId) == 1)
            {
                list.replace(5,QString("已取出"));
                modelData.replace(length-1,list);

                sendNote(list);

                sendInfo.insert(list.at(6),list.at(3));

                cellError boxError;
                boxError.cabinetId = cellStr.left(2);
                boxError.cellId = cellStr;
                boxError.errorType = 3;
                boxError.reporttime = QDateTime::currentDateTime();
                boxError.status = 1;

                ST_cabinet_cell::instance()->takeChangeCellInfo(cellStr);

                ST_cabinet_cell::instance()->insertCellError(boxError);
                SCDelivery::instance()->updateDeliveryExpTakeInfo(list.at(6));
            }
            else
            {
                NetCellExcept exceInfo;
                exceInfo.cellId = cellStr;
                exceInfo.flowType = 1; //派件
                exceInfo.errorType = 4;//快递员取件
                exceInfo.Relkey = list.at(6);
                list.replace(5,QString("等待维修"));
                modelData.replace(length-1,list);

                exceInfoList.append(exceInfo);

                cellError boxError;
                boxError.cabinetId = cellStr.left(2);
                boxError.cellId = cellStr;
                boxError.errorType = 4; //快递员取件打不开
                boxError.reporttime = QDateTime::currentDateTime();
                boxError.status = 1;

                //柜子和格子表设置为打不开
                 ST_cabinet_cell::instance()->changeCellStatus(cellStr,2);

                 ST_cabinet_cell::instance()->insertCellError(boxError);

            }
        }
        length = length - 1;
    }

    model->setModelData(modelData);
    model->refrushModel();
    ui->tableView->setModel(model);

    if(!sendInfo.isEmpty())
    {
        NetworkDesc_t netWorkInfo = SCServerComm::instance()->expTakeDelivery(sendInfo);
        SCNetworkThread::instance()->sendNetWorkEvent(netWorkInfo);
    }

    if(!exceInfoList.isEmpty())
    {
        NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->reportCellError(exceInfoList);
        SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
    }

    ui->openBtn->setEnabled(true);
    ui->quitBtn->setEnabled(true);
}

void detainpage::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void detainpage::sendNote(QStringList list)
{
    //调用短信发送接口
    NoteDesc_t noteInfo;
    QString smsContent;

    noteInfo.mSmsType = 3;
    noteInfo.mRelPriKey = list.at(6);
    noteInfo.mMobilePhone = list.at(2);
    noteInfo.mSendCount = 0;

    if(SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        noteInfo.mSmsContent = smsContent.arg(list.at(7)).arg(list.at(8)).arg(list.at(9));

        if(SCTerminal::instance()->terminalInfo.smsSendWay == 1)
        {
            QUuid uuId = QUuid::createUuid();
            QString strUuId = uuId.toString();
            strUuId = strUuId.remove(QRegExp("[{}-]"));
            strUuId = strUuId.toUpper();
            noteInfo.mSmsLogId = strUuId;
            noteInfo.mStatus = 3;
            noteInfo.mSendTime = QDateTime::currentDateTime();

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
