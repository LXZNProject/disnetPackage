
#include "exptakepage.h"
#include "ui_exptakepage.h"
#include "t_basic_delivery.h"
#include "servercomm.h"
#include "t_cabinet_cell.h"
#include "networkthread.h"
#include "notethread.h"
#include "mainctrl.h"
#include "t_log_smslog.h"
#include "t_log_template.h"
#include "t_log_smslimit.h"
#include "t_basic_terminal.h"
#include <QUuid>

exptakepage::exptakepage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::exptakepage)
{
    ui->setupUi(this);
    initPage();
}

exptakepage::~exptakepage()
{
    delete ui;
}

void exptakepage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/expTake/images/expTake/expTake_bg.png")));
    this->setPalette(palette);


    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    QStringList list;
    list<< "序号" << "快递单号" << "手机号码" << "存入时间" << "箱门号" << "取件";

    model = new takeViewModel();
    model->setHorizontalHeaderList(list);

    //水平模型头充满水平区域
    QHeaderView *header;
    header = ui->tableView->horizontalHeader();
    header->setStretchLastSection(true);


    ui->tableView->verticalHeader()->hide();

    btnDelegate = new BtnDelegate(this);
    ui->tableView->setItemDelegateForColumn(5, btnDelegate);


    connect(btnDelegate,SIGNAL(exptakeSignal(QModelIndex)),this,SLOT(readlyOpen_slots(QModelIndex)));
    connect(&confirmMsgBox,SIGNAL(msgBox_Ok()),this,SLOT(openCell_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
}

void exptakepage::readExpTakePageInfo(QByteArray param)
{
    QList<DelivertDesc_t> deliveryInfo;
    QStringList dataList;
    QDataStream in(&param,QIODevice::ReadOnly);
    in >> dataList;
    modelData.clear();
    sendInfo.clear();
    exceInfoList.clear();
    SCDelivery::instance()->selectExpTakeDeliveryInfo(dataList.at(6),deliveryInfo);

    SCDelivery::instance()->queryExpTakeDeliveryInfo(deliveryInfo,modelData);

    model->setModelData(modelData);
    model->refrushModel();
    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(3,180);
}

void exptakepage::quitBtn_slots()
{
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
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void exptakepage::readlyOpen_slots(QModelIndex index)
{
    emit resetTime();
    mIndex = index;
    QVariant value = model->mydata(index);
    QString cellId = value.toStringList().at(4);
    confirmMsgBox.showMsgBox(QString("您确定要打开%1箱门？").arg(cellId),true,2);
}

void exptakepage::openCell_slots()
{
    confirmMsgBox.close();
    emit resetTime();
    ui->quitBtn->setEnabled(false);

    int row = mIndex.row();
    QStringList list = modelData.at(row);

    QString cellStr = list.at(4);
    qint8 cabinetId = cellStr.left(2).toInt();     //柜号
    qint8 cellId    = cellStr.right(2).toInt();     //箱号



    if(lockoperation.openLock(cabinetId,cellId) == 1)
    {
        list.replace(5,QString("已取出"));
        modelData.replace(row,list);

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
        modelData.replace(row,list);

        exceInfoList.append(exceInfo);

        cellError boxError;
        boxError.cabinetId = cellStr.left(2);
        boxError.cellId = cellStr;
        boxError.errorType = 4;//快递员取件打不开
        boxError.reporttime = QDateTime::currentDateTime();
        boxError.status = 1;

        //柜子和格子表设置为打不开
         ST_cabinet_cell::instance()->changeCellStatus(cellStr,2);

         ST_cabinet_cell::instance()->insertCellError(boxError);

         QString info = "箱门打不开,请等待客服处理";
         myMsgBox.showMsgBox(info);

    }

    model->setModelData(modelData);
    model->refrushModel();
    ui->tableView->setModel(model);


    ui->quitBtn->setEnabled(true);

}

void exptakepage::sendNote(QStringList list)
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
