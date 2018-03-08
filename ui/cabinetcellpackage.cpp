#include "cabinetcellpackage.h"
#include "ui_cabinetcellpackage.h"
#include "t_cabinet_cell.h"
#include "mainctrl.h"
#include <QDebug>
#include <QLayout>
#include <QMessageBox>
cabinetCellPackage::cabinetCellPackage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cabinetCellPackage)
{
    ui->setupUi(this);
    init();
}

cabinetCellPackage::~cabinetCellPackage()
{
    delete ui;
}
void cabinetCellPackage::init()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/cell/images/cell/cabinetBackGround.png")));
    this->setPalette(palette);

    ui->addBtn->setFlat(true);
    ui->addBtn->setFocusPolicy(Qt::NoFocus);
    ui->addBtn->setIcon(QIcon(":/cell/images/cell/addCabinet.png"));
    ui->addBtn->setMinimumSize(QPixmap(":/cell/images/cell/addCabinet.png").size());
    ui->addBtn->setIconSize(ui->addBtn->size());
    ui->addBtn->setStyleSheet("background-color:transparent");


    ui->cancelPushButton->setFlat(true);
    ui->cancelPushButton->setFocusPolicy(Qt::NoFocus);
    ui->cancelPushButton->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->cancelPushButton->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->cancelPushButton->setIconSize(ui->cancelPushButton->size());
    ui->cancelPushButton->setStyleSheet("background-color:transparent");


    m_model = new takeViewModel(this);
    QStringList headers;
    headers << "柜子号" << "格子个数"  << "柜子类型"<< "操作" << "操作";
    m_model->setHorizontalHeaderList(headers);

    m_buttonDelegate = new BtnDelegate(this);


    ui->tableView->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableView->setFocusPolicy(Qt::NoFocus);
    ui->tableView->setItemDelegateForColumn(3, m_buttonDelegate);
    ui->tableView->setItemDelegateForColumn(4, m_buttonDelegate);
    ui->tableView->setMinimumWidth(500);


    addCabinet = new addCabinetWidget();


    checkCabinrt = new checkCabinrtWidget();

    myMsgbox = new MsgBox();


    connect(ui->cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));
    connect(ui->addBtn,SIGNAL(clicked()),this,SLOT(addCabinetClicked()));
    connect(m_buttonDelegate,SIGNAL(checkSignal(QModelIndex)),this,SLOT(checkCabinet(QModelIndex)));
    connect(m_buttonDelegate,SIGNAL(deleteSignal(QModelIndex)),this,SLOT(deleteCabinet(QModelIndex)));
    connect(addCabinet,SIGNAL(addSignal()),this,SLOT(updataInfo()));
    connect(checkCabinrt,SIGNAL(deleteSignal()),this,SLOT(updataInfo()));

    connect(myMsgbox,SIGNAL(msgBox_Ok()),this,SLOT(msgboxOK_slot()));
}

void cabinetCellPackage::readInfo()
{
    cabinetID.clear();
    cabinetNum.clear();
    cabinetType.clear();
    ST_cabinet_cell::instance()->getSaveCabinetInfo(cabinetID,cabinetType,cabinetNum);

    QList<QStringList> data;
    for(int i = 0;i < cabinetID.size();i++)
    {
        QString tempType;
        if(cabinetType.at(i) == "1")
        {
            tempType ="副柜";
        }
        else
        {
            tempType ="主柜";
        }
        data.append(QStringList() << cabinetID.at(i) << cabinetNum.at(i)  << tempType << "管理" << "删除");
    }
    m_model->setModelData(data);
    m_model->refrushModel();
    ui->tableView->setModel(m_model);
    ui->tableView->resizeColumnsToContents();
}

void cabinetCellPackage::cancelBtnClicked()
{
    cabinetID.clear();
    cabinetNum.clear();
    cabinetType.clear();
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void cabinetCellPackage::addCabinetClicked()
{
    addCabinet->show();
}

void cabinetCellPackage::checkCabinet(QModelIndex index)
{
    QVariant value = m_model->mydata(index);
    QString tempCabinet = value.toStringList().at(0);

    checkCabinrt->readInfo(tempCabinet);
    checkCabinrt->show();
}

void cabinetCellPackage::deleteCabinet(QModelIndex index)
{
    myindex = index;
    myMsgbox->showMsgBox("确定要删除此柜子吗？",true,2);
}

void cabinetCellPackage::updataInfo()
{
    readInfo();
}

void cabinetCellPackage::msgboxOK_slot()
{
    QVariant value = m_model->mydata(myindex);
    QString tempCabinet = value.toStringList().at(0);
    ST_cabinet_cell::instance()->delectCabinet(tempCabinet);
    readInfo();
}
