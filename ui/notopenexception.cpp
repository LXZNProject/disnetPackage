#include "notopenexception.h"
#include "ui_notopenexception.h"

#include "mainctrl.h"
#include "t_cabinet_cell.h"
#include "t_log_smslog.h"
#include "t_log_template.h"
#include "t_basic_terminal.h"
#include "notethread.h"
#include "networkthread.h"
#include "servercomm.h"

#include "t_basic_delivery.h"
#include "t_basic_lan_jian.h"
#include "t_basic_temp_storage.h"
#include "t_log_network.h"
#include <QUuid>

#include <QDebug>

notopenexception::notopenexception(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::notopenexception)
{
    ui->setupUi(this);

    initNotOpenExceptionPage();
}

notopenexception::~notopenexception()
{
    delete ui;
}

void notopenexception::initNotOpenExceptionPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/notopenexception/images/notopenexception/background.png")));
    this->setPalette(palette);


    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    QStringList list;
    list<< "序号" << "箱门号" << "异常类型" << "操作";

    model = new takeViewModel();
    model->setHorizontalHeaderList(list);

    //水平模型头充满水平区域
    QHeaderView *header;
    header = ui->tableView->horizontalHeader();
    header->setResizeMode(QHeaderView::Stretch);

    ui->tableView->verticalHeader()->hide();

    btnDelegate = new BtnDelegate(this);
    ui->tableView->setItemDelegateForColumn(3, btnDelegate);
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
    connect(btnDelegate,SIGNAL(repairSignal(QModelIndex)),this,SLOT(confirmRepair_slots(QModelIndex)));
    connect(&confirmMsgBox,SIGNAL(msgBox_Ok()),this,SLOT(repairCell_slots()));
}

void notopenexception::readNotOpenInfo()
{
    modelData.clear();
    ST_cabinet_cell::instance()->queryExceptionCellError(modelData);

    if(!modelData.isEmpty())
    {
        model->setModelData(modelData);
        model->refrushModel();
        ui->tableView->setModel(model);

        this->show();
    }
    else
    {
        myMsgBox.showMsgBox("没有打不开异常");
    }

}

void notopenexception::confirmRepair_slots(QModelIndex index)
{
    mIndex = index;
    QVariant value = model->mydata(index);
    QString cellId = value.toStringList().at(1);
    confirmMsgBox.showMsgBox(QString("您确定要修复%1箱门异常？").arg(cellId),true,2);
}

void notopenexception::repairCell_slots()
{
    cellError cellException;
    qint8 cellStatus = 1;

    QVariant value = model->mydata(mIndex);
    qint8 number = value.toStringList().at(0).toInt();
    QString cellId = value.toStringList().at(1);
    qint8 excepType = value.toStringList().at(4).toInt();
    cellException.cellId = cellId;
    cellException.repairtime = QDateTime::currentDateTime();

    QStringList list = modelData.at(number-1);
    list.replace(3,QString("已修复"));

    modelData.replace(number-1,list);
    model->setModelData(modelData);
    model->refrushModel();
    ui->tableView->setModel(model);

    sendRepairNote(excepType,cellId);
    ST_cabinet_cell::instance()->updateExceptionCellError(cellException);
    ST_cabinet_cell::instance()->changeCellStatus(cellId,cellStatus);

    NetworkDesc_t exceNetWorkInfo = SCServerComm::instance()->repairCellError(cellId,excepType);
    SCNetworkThread::instance()->sendNetWorkEvent(exceNetWorkInfo);
}

void notopenexception::quitBtn_slots()
{
    this->close();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void notopenexception::sendRepairNote(qint8 type, QString cell)
{
    //调用短信发送接口
    bool ret = false;
    NoteDesc_t noteInfo;
    QString smsContent;

    noteInfo.mSendCount = 0;
    noteInfo.mSmsType = 94;

    if(!SCNoteTemplate::instance()->getCurrentNoteInfo(noteInfo.mSmsType,smsContent))
    {
        return ;
    }

    if(2 == type)
    {
        DelivertDesc_t deliveryInfo;
        ret = SCDelivery::instance()->selectExceptionCodeForCell(cell,deliveryInfo);
        noteInfo.mMobilePhone = deliveryInfo.mobilephone;
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell).arg(deliveryInfo.getCode);
    }
    else if(5 == type)
    {
        BasicTempStorage deliveryInfo;
        ret = SBasicTempStorage::instance()->selectExceptionCodeForCell(cell,deliveryInfo);
        noteInfo.mMobilePhone = deliveryInfo.mobilephone;
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell).arg(deliveryInfo.getCode);
    }
    else if(6 == type)
    {
        SenderDesc_t deliveryInfo;
        ret = SCSender::instance()->selectExceptionCodeForCell(cell,deliveryInfo);
        noteInfo.mMobilePhone = deliveryInfo.mobilephone;
        noteInfo.mSmsContent = smsContent.arg(SCTerminal::instance()->terminalInfo.terminalName).arg(cell).arg(deliveryInfo.getCode);
    }


    if(ret)
    {
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
        else if(SCTerminal::instance()->terminalInfo.smsSendWay == 2)
        {
            NetworkDesc_t netInfo;
            netInfo.mSmsType = noteInfo.mSmsType;
            netInfo.mPhone = noteInfo.mMobilePhone;
            netInfo.mContent = noteInfo.mSmsContent;

            SCServerComm::instance()->sendSms(netInfo);
            SCNetworkThread::instance()->sendNetWorkEvent(netInfo);
        }
    }
}
