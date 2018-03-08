#include "managepage.h"
#include "ui_managepage.h"
#include <QMessageBox>
#include "mainctrl.h"
#include "notopenexception.h"
#include "t_cabinet_cell.h"
managePage::managePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::managePage)
{
    ui->setupUi(this);
    initPage();
}
managePage::~managePage()
{
    delete ui;
}

void managePage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/manage/images/manage/background.png")));
    this->setPalette(palette);

    ui->label->resize(QPixmap(":/manage/images/manage/cabinet.png").size());
    ui->label->setPixmap(QPixmap(":/manage/images/manage/cabinet.png"));

    ui->label_2->resize(QPixmap(":/manage/images/manage/cell.png").size());
    ui->label_2->setPixmap(QPixmap(":/manage/images/manage/cell.png"));

    ui->openBtn->setFlat(true);
    ui->openBtn->setFocusPolicy(Qt::NoFocus);
    ui->openBtn->setIcon(QIcon(":/manage/images/manage/open.png"));
    ui->openBtn->setMinimumSize(QPixmap(":/manage/images/manage/open.png").size());
    ui->openBtn->setIconSize(ui->openBtn->size());
    ui->openBtn->setStyleSheet("background-color:transparent");

    ui->notOpenBtn->setFlat(true);
    ui->notOpenBtn->setFocusPolicy(Qt::NoFocus);
    ui->notOpenBtn->setIcon(QIcon(":/manage/images/manage/notOpen.png"));
    ui->notOpenBtn->setMinimumSize(QPixmap(":/manage/images/manage/notOpen.png").size());
    ui->notOpenBtn->setIconSize(ui->notOpenBtn->size());
    ui->notOpenBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");



    notopenpage = new notopenexception();
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));
    connect(ui->notOpenBtn,SIGNAL(clicked()),this,SLOT(notOpenBtn_slots()));

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
    connect(ui->openBtn,SIGNAL(clicked()),this,SLOT(openCellBtnClicked()));
}

void managePage::openCellBtnClicked()
{
    emit resetTime();
    ui->quitBtn->setEnabled(false);
    ui->openBtn->setEnabled(false);
    qint8 cabinetId =  ui->cabinetComboBox->currentText().toInt();
    qint8 cellId = ui->cellComboBox->currentText().toInt();

    if(lockoperation.openLock(cabinetId,cellId) != 1)
    {
        myMsgBox.showMsgBox("箱门打不开！");
    }

    ui->quitBtn->setEnabled(true);
    ui->openBtn->setEnabled(true);
}

void managePage::cancelBtnClicked()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}

void managePage::notOpenBtn_slots()
{
    emit resetTime();
    notopenpage->readNotOpenInfo();
}
