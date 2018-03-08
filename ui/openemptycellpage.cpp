#include "openemptycellpage.h"
#include "ui_openemptycellpage.h"
#include "mainctrl.h"
#include "t_cabinet_cell.h"
#include <QMessageBox>
openEmptyCellPage::openEmptyCellPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::openEmptyCellPage)
{
    ui->setupUi(this);
    initPage();
}

openEmptyCellPage::~openEmptyCellPage()
{
    delete ui;
}

void openEmptyCellPage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/openEmptyCell/images/openEmptyCell/background.png")));
    this->setPalette(palette);

    ui->label->resize(QPixmap(":/openEmptyCell/images/openEmptyCell/cabinetNum.png").size());
    ui->label->setPixmap(QPixmap(":/openEmptyCell/images/openEmptyCell/cabinetNum.png"));

    ui->label_2->resize(QPixmap(":/openEmptyCell/images/openEmptyCell/cellNum.png").size());
    ui->label_2->setPixmap(QPixmap(":/openEmptyCell/images/openEmptyCell/cellNum.png"));

    ui->openBtn->setFlat(true);
    ui->openBtn->setFocusPolicy(Qt::NoFocus);
    ui->openBtn->setIcon(QIcon(":/openEmptyCell/images/openEmptyCell/open.png"));
    ui->openBtn->setMinimumSize(QPixmap(":/openEmptyCell/images/openEmptyCell/open.png").size());
    ui->openBtn->setIconSize(ui->openBtn->size());
    ui->openBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    QStringList cabinetList;
    for(int i = 0; i <= 8; i++)
    {
        cabinetList.append(QString::number(i).sprintf("%02d",i));
    }
    ui->cabinetComboBox->addItems(cabinetList);

    QStringList cellList;
    for(int j = 1;j < 21;j++)
    {
        cellList.append(QString::number(j).sprintf("%02d",j));
    }
    ui->cellComboBox->addItems(cellList);

    connect(ui->openBtn,SIGNAL(clicked()),this,SLOT(openBtnClicked()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));
}


void openEmptyCellPage::openBtnClicked()
{
    emit resetTime();
    ui->quitBtn->setEnabled(false);
    ui->openBtn->setEnabled(false);
    QString openCell =  ui->cabinetComboBox->currentText() + ui->cellComboBox->currentText();

    if(filterCell(openCell))
    {
        myMsgBox.showMsgBox("此箱门中已有物品或者箱门不存在，您无法打开！");
    }
    else
    {
        qint8 cabinetId =  ui->cabinetComboBox->currentText().toInt();
        qint8 cellId = ui->cellComboBox->currentText().toInt();
        if(lockoperation.openLock(cabinetId,cellId) != 1)
        {
            myMsgBox.showMsgBox("箱门打不开！");
        }
    }
    ui->quitBtn->setEnabled(true);
    ui->openBtn->setEnabled(true);
}
bool openEmptyCellPage::filterCell(QString openCell)
{
    if(ST_cabinet_cell::instance()->checkCellIsExit(openCell))
    {
        basicCell tempvalue;
        ST_cabinet_cell::instance()->getCellInfo(openCell,tempvalue);
        if(tempvalue.hasGoods == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return true;
}
void openEmptyCellPage::cancelBtnClicked()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
