#include "checkcabinrtwidget.h"
#include "ui_checkcabinrtwidget.h"
#include <QDebug>
#include <QMessageBox>
checkCabinrtWidget::checkCabinrtWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::checkCabinrtWidget)
{
    ui->setupUi(this);
    init();
}

checkCabinrtWidget::~checkCabinrtWidget()
{
    delete ui;
}

void checkCabinrtWidget::readInfo(QString tempvalue)
{
    cabinetIdStr = tempvalue;
    QList <QStringList>    cellValueListStr;
    cellValueListStr.clear();
    m_cellValueList.clear();
    ST_cabinet_cell::instance()->getSaveCellInfo(tempvalue,m_cellValueList);
    for(int i = 0; i < m_cellValueList.size(); i++)
    {
        QStringList tempList;
        tempList.append(m_cellValueList.at(i).cellId);
        tempList.append(m_cellValueList.at(i).cabinetId);
        //类型
        if(m_cellValueList.at(i).cellType == 1)
        {
            tempList.append("大");
        }
        else if(m_cellValueList.at(i).cellType == 2)
        {
            tempList.append("中");
        }
        else
        {
            tempList.append("小");
        }
        //状态
        if(m_cellValueList.at(i).status == 1)
        {
            tempList.append("正常");
        }
        else if(m_cellValueList.at(i).status == 2)
        {
            tempList.append("打不开");
        }
        else if(m_cellValueList.at(i).status == 3)
        {
            tempList.append("未关闭");
        }
        else
        {
            tempList.append("不可用");
        }
        //是否有物品
        if(m_cellValueList.at(i).hasGoods == 1)
        {
            tempList.append("有");
        }
        else
        {
            tempList.append("无");
        }
        //是否有物品检测条
        if(m_cellValueList.at(i).goodsCheck == 1)
        {
            tempList.append("有");
        }
        else
        {
            tempList.append("无");
        }
        //物品检测条状态
        if(m_cellValueList.at(i).goodsCheckStatus == 1)
        {
            tempList.append("正常");
        }
        else
        {
            tempList.append("损坏");
        }

        tempList.append("编辑");
        tempList.append("删除");
        cellValueListStr.append(tempList);
    }
    m_model->setModelData(cellValueListStr);

    m_model->refrushModel();
    ui->tableView->setItemDelegateForColumn(7, m_buttonDelegate);
    ui->tableView->setItemDelegateForColumn(8, m_buttonDelegate);
    ui->tableView->setItemDelegateForColumn(9, m_buttonDelegate);
    ui->tableView->setModel(m_model);
}

void checkCabinrtWidget::init()
{

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/cell/images/cell/cellBackGround.png")));
    this->setPalette(palette);


    ui->addBtn->setFlat(true);
    ui->addBtn->setFocusPolicy(Qt::NoFocus);
    ui->addBtn->setIcon(QIcon(":/cell/images/cell/addCell.png"));
    ui->addBtn->setMinimumSize(QPixmap(":/cell/images/cell/addCell.png").size());
    ui->addBtn->setIconSize(ui->addBtn->size());
    ui->addBtn->setStyleSheet("background-color:transparent");


    ui->pushButton->setFlat(true);
    ui->pushButton->setFocusPolicy(Qt::NoFocus);
    ui->pushButton->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->pushButton->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->pushButton->setIconSize(ui->pushButton->size());
    ui->pushButton->setStyleSheet("background-color:transparent");



    m_model = new takeViewModel(this);
    QStringList headers;
    headers <<"格子号" << "柜子号" << "类型"  << "状态" << "是否有物品" << "是否有物品检测条" \
                   << " 物品检测条状态" << "操作" << "操作";
    m_model->setHorizontalHeaderList(headers);

    m_buttonDelegate = new BtnDelegate(this);



    myaddCell = new addCellWidget();

    myMsgbox = new MsgBox();


    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setFocusPolicy(Qt::NoFocus);

    editCabinet = new editCabinetWidget();
    editCabinet->setAttribute(Qt::WA_DeleteOnClose);
    editCabinet->hide();

    connect(m_buttonDelegate,SIGNAL(deleteSignal(QModelIndex)),this,SLOT(deleteCabinet(QModelIndex)));
    connect(m_buttonDelegate,SIGNAL(editSignal(QModelIndex)),this,SLOT(myeditCabinet(QModelIndex)));
    connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
    connect(ui->addBtn,SIGNAL(clicked()),this,SLOT(addCell_slot()));
    connect(myaddCell,SIGNAL(addCellOK()),this,SLOT(updataUI()));
    connect(editCabinet,SIGNAL(editOKSignal()),this,SLOT(updataUI()));
    connect(myMsgbox,SIGNAL(msgBox_Ok()),this,SLOT(msgboxOK_slots()));
}

void checkCabinrtWidget::myeditCabinet(QModelIndex index)
{
    QVariant value = m_model->mydata(index);
    QString tempCell = value.toStringList().at(0);
    basicCell tempvalue;
    ST_cabinet_cell::instance()->getCellInfo(tempCell,tempvalue);
    editCabinet->readInfo(tempvalue);
    editCabinet->show();
}

void checkCabinrtWidget::cancelClicked()
{
    hide();
}

void checkCabinrtWidget::addCell_slot()
{
    myaddCell->readInfo(cabinetIdStr);
    myaddCell->show();
}

void checkCabinrtWidget::updataUI()
{
    readInfo(cabinetIdStr);
    emit deleteSignal();
}

void checkCabinrtWidget::msgboxOK_slots()
{
    QVariant value = m_model->mydata(myindex);
    QString tempCell = value.toStringList().at(0);
    QString tempCabinet = value.toStringList().at(1);

    ST_cabinet_cell::instance()->delectCell(tempCell);
    int number = 0;
    int type = 0;
    ST_cabinet_cell::instance()->getCabinetNumber(tempCabinet,type,number);
    number = number - 1;
    ST_cabinet_cell::instance()->changeCabinet(tempCabinet,type,number);

    readInfo(tempCabinet);
    emit deleteSignal();
}

void checkCabinrtWidget::deleteCabinet(QModelIndex index)
{
    myindex = index;
    myMsgbox->showMsgBox("确定要删除此格子吗？",true,2);
}


