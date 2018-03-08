#include "rechargestatus.h"
#include <QObject>
CRechargeStatus::CRechargeStatus(STATUS status):
    CStatus(status)
{
    rechargePage = new recharge();
    connect(rechargePage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CRechargeStatus::enter(QByteArray param)
{
    openTimer();
    rechargePage->readInfor(param);
    rechargePage->show();
    rechargePage->timeStart();
}

void CRechargeStatus::leave(QByteArray param)
{
    stopTimer();
    rechargePage->hide();
}

void CRechargeStatus::resetStart()
{
    clearTimer();
}

void CRechargeStatus::jumpPage()
{
    rechargePage->quitBtn_slots();
}


