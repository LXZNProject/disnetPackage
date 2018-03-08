#include "addcellwidget.h"
#include "ui_addcellwidget.h"
#include "t_cabinet_cell.h"
#include <QMessageBox>
#include <QBitmap>
#include <QDesktopWidget>
addCellWidget::addCellWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addCellWidget)
{
    ui->setupUi(this);
    initPage();
}

addCellWidget::~addCellWidget()
{
    delete ui;
}

void addCellWidget::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowModality(Qt::ApplicationModal);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/cell/images/cell/background.png")));
    this->setPalette(palette);

    ui->label->resize(QPixmap(":/cell/images/cell/cellNum.png").size());
    ui->label->setPixmap(QPixmap(":/cell/images/cell/cellNum.png"));

    ui->label_2->resize(QPixmap(":/cell/images/cell/cellType.png").size());
    ui->label_2->setPixmap(QPixmap(":/cell/images/cell/cellType.png"));

    ui->label_3->resize(QPixmap(":/cell/images/cell/cellStatus.png").size());
    ui->label_3->setPixmap(QPixmap(":/cell/images/cell/cellStatus.png"));

    ui->label_4->resize(QPixmap(":/cell/images/cell/hasGood.png").size());
    ui->label_4->setPixmap(QPixmap(":/cell/images/cell/hasGood.png"));

    ui->label_5->resize(QPixmap(":/cell/images/cell/hasGoodCheck.png").size());
    ui->label_5->setPixmap(QPixmap(":/cell/images/cell/hasGoodCheck.png"));

    ui->label_6->resize(QPixmap(":/cell/images/cell/goodCheckStatus.png").size());
    ui->label_6->setPixmap(QPixmap(":/cell/images/cell/goodCheckStatus.png"));

    ui->okBtn->setFlat(true);
    ui->okBtn->setFocusPolicy(Qt::NoFocus);
    ui->okBtn->setIcon(QIcon(":/public/images/public/okBtn_bg.png"));
    ui->okBtn->setMinimumSize(QPixmap(":/public/images/public/okBtn_bg.png").size());
    ui->okBtn->setIconSize(ui->okBtn->size());
    ui->okBtn->setStyleSheet("background-color:transparent");

    ui->cancelBtn->setFlat(true);
    ui->cancelBtn->setFocusPolicy(Qt::NoFocus);
    ui->cancelBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->cancelBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->cancelBtn->setIconSize(ui->cancelBtn->size());
    ui->cancelBtn->setStyleSheet("background-color:transparent");

    //connect(ui->cellNumLineEdit,SIGNAL(editingFinished()),this,SLOT(cellNumLineEditChange()));
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtnClick()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClick()));

     this->move((QApplication::desktop()->width() - this->width())/2,(QApplication::desktop()->height() - this->height())/2);

}

void addCellWidget::readInfo(QString cabinetStr)
{
    mycabinetStr = cabinetStr;
    checkResult = false;
}

bool addCellWidget::cellNumLineEditChange()
{
    QString tempCell = ui->cellNumLineEdit->text().trimmed();

    QRegExp rrx("\\d{2}");
    if(!rrx.exactMatch(tempCell) )
    {
        myMsgBox.showMsgBox("格子号输入错误，请检查后重新输入。");
        checkResult = false;
        return checkResult;
    }
   tempCell = mycabinetStr + tempCell;
    if(ST_cabinet_cell::instance()->checkCellIsExit(tempCell))
    {
        myMsgBox.showMsgBox("格子号存在，请检查后重新输入。");
        checkResult = false;
        return checkResult;
    }
    mycellId = tempCell;
    checkResult = true;
    return  checkResult;
}

void addCellWidget::okBtnClick()
{
    if(cellNumLineEditChange())
    {
        basicCell tempvalue;
        tempvalue.cellId = mycellId;
        tempvalue.cabinetId = mycabinetStr;
        tempvalue.cellType = ui->cellTypeComboBox->currentIndex() + 1;
        tempvalue.status = ui->statusComboBox->currentIndex() + 1;
        tempvalue.hasGoods = ui->hasGoodsComboBox->currentIndex() + 1;
        tempvalue.goodsCheck = ui->goodsCheckComboBox->currentIndex() + 1;
        tempvalue.goodsCheckStatus = ui->goodsCheckStatusComboBox->currentIndex() + 1;

        ST_cabinet_cell::instance()->insertCell(tempvalue);
        int type;
        int number;
        ST_cabinet_cell::instance()->getCabinetNumber(mycabinetStr, type,number);//获取柜子个数
        number = number + 1;
        ST_cabinet_cell::instance()->changeCabinet(mycabinetStr, type,number);//修改柜子

        myMsgBox.showMsgBox("增加格子号成功。");
        ui->cellNumLineEdit->clear();
        emit addCellOK();
    }
}

void addCellWidget::cancelBtnClick()
{
    ui->cellNumLineEdit->clear();
    checkResult = false;
    hide();
}
