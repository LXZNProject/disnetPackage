#include "assistpage.h"
#include "ui_assistpage.h"
#include "mainctrl.h"
#include "servercomm.h"
#include "db.h"
#include "networkthread.h"
#include <QBitmap>

assistpage::assistpage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::assistpage)
{
    ui->setupUi(this);

    initPage();
}

assistpage::~assistpage()
{
    delete ui;
}

void assistpage::initPage()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/assist/images/assist/assist_bg.png")));
    this->setPalette(palette);

    ui->label->setFixedSize(QPixmap(":/assist/images/assist/help.png").size());
    ui->label->setPixmap(QPixmap(":/assist/images/assist/help.png"));

    ui->startBtn->setFlat(true);
    ui->startBtn->setFocusPolicy(Qt::NoFocus);
    ui->startBtn->setIcon(QIcon(":/assist/images/assist/startBtn_bg.png"));
    ui->startBtn->setMinimumSize(QPixmap(":/assist/images/assist/startBtn_bg.png").size());
    ui->startBtn->setIconSize(ui->startBtn->size());
    ui->startBtn->setStyleSheet("background-color:transparent");

    ui->quitBtn->setFlat(true);
    ui->quitBtn->setFocusPolicy(Qt::NoFocus);
    ui->quitBtn->setIcon(QIcon(":/public/images/public/quitBtn_bg.png"));
    ui->quitBtn->setMinimumSize(QPixmap(":/public/images/public/quitBtn_bg.png").size());
    ui->quitBtn->setIconSize(ui->quitBtn->size());
    ui->quitBtn->setStyleSheet("background-color:transparent");


    connect(ui->startBtn,SIGNAL(clicked()),this,SLOT(startBtn_slots()));
    connect(ui->quitBtn,SIGNAL(clicked()),this,SLOT(quitBtn_slots()));
}

void assistpage::setBtnEnable(bool status)
{
    ui->startBtn->setEnabled(status);
    ui->quitBtn->setEnabled(status);
}

void assistpage::startBtn_slots()
{
    emit resetTime();
    setBtnEnable(false);

    progress.setMinimum(0);
    progress.setMaximum(0);
    progress.setValue(0);
    progress.setCancelButton(0);
    progress.setWindowFlags(Qt::FramelessWindowHint);
    progress.setLabelText(QString::fromUtf8("连接中，请稍后..."));
    progress.setStyleSheet("background-color: rgb(93, 186, 0)");
    progress.show();


    syncRemoteCommandRet ret;
    QList<QRemoteCommandList*> cmdList;
    SCServerComm::instance()->syncRemoteCommand(&ret);
    progress.hide();

    if(ret.resultCode() == RET_TRUE)
    {
        cmdList = ret.getCmdInfo();

        if(cmdList.size() > 0)
        {
            QList<CmdDesc_t> retCmdList = SCDatBase::instance()->assistTerminalInfo(cmdList);
            NetworkDesc_t cmdNetWorkInfo = SCServerComm::instance()->remoteCommandCallBack(retCmdList);
            SCNetworkThread::instance()->sendNetWorkEvent(cmdNetWorkInfo);

            myMsgBox.showMsgBox("远程协助完成。");
        }
        else
        {
            myMsgBox.showMsgBox(ret.resultMessage(),false);
        }
    }
    else if(ret.resultCode().isEmpty())
    {
        myMsgBox.showMsgBox("通讯失败。");
    }
    else
    {
        myMsgBox.showMsgBox(ret.resultMessage(),false);
    }

    setBtnEnable(true);
   // progress.hide();
}

void assistpage::quitBtn_slots()
{
    SCMainCtrl::instance()->enterStatus(ST_HOME,"");
}
