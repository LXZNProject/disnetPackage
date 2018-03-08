#include "serverpage.h"
#include "ui_serverpage.h"

#include "mainctrl.h"
#include "t_basic_terminal.h"

serverpage::serverpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serverpage)
{
    ui->setupUi(this);
    initServicePage();
}

serverpage::~serverpage()
{
    delete ui;
}

void serverpage::initServicePage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/house/images/house/house_bg.png")));
    this->setPalette(palette);

    ui->dryBtn->setFlat(true);
    ui->dryBtn->setFocusPolicy(Qt::NoFocus);
    ui->dryBtn->setIcon(QIcon(":/house/images/house/dryBtn_bg.png"));
    ui->dryBtn->setMinimumSize(QPixmap(":/house/images/house/dryBtn_bg.png").size());
    ui->dryBtn->setIconSize(ui->dryBtn->size());
    ui->dryBtn->setStyleSheet("background-color:transparent");

    ui->unkownBtn1->setFlat(true);
    ui->unkownBtn1->setFocusPolicy(Qt::NoFocus);
    ui->unkownBtn1->setIcon(QIcon(":/house/images/house/unkownBtn1_bg.png"));
    ui->unkownBtn1->setMinimumSize(QPixmap(":/house/images/house/unkownBtn1_bg.png").size());
    ui->unkownBtn1->setIconSize(ui->unkownBtn1->size());
    ui->unkownBtn1->setStyleSheet("background-color:transparent");

    ui->unkownBtn2->setFlat(true);
    ui->unkownBtn2->setFocusPolicy(Qt::NoFocus);
    ui->unkownBtn2->setIcon(QIcon(":/house/images/house/unkownBtn2_bg.png"));
    ui->unkownBtn2->setMinimumSize(QPixmap(":/house/images/house/unkownBtn2_bg.png").size());
    ui->unkownBtn2->setIconSize(ui->unkownBtn2->size());
    ui->unkownBtn2->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");

    connect(ui->dryBtn,SIGNAL(clicked()),this,SLOT(dryBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
}

void serverpage::dryBtn_slots()
{
    if(SCTerminal::instance()->terminalInfo.washSwitch == 2 || SCTerminal::instance()->terminalInfo.washerPhone.isEmpty())
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
        SCMainCtrl::instance()->enterStatus(ST_HOUSE_PACK_MENU,"");
    }
}

void serverpage::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
