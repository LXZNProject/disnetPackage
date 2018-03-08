#include "checkdevicestatus.h"

CCheckDeviceStatus::CCheckDeviceStatus(STATUS status):
    CStatus(status)
{
    myPage = new checkDevice();
    connect(myPage,SIGNAL(resetTime()),this,SLOT(resetStart()));
    connect(this,SIGNAL(jumpPage_signal()),this,SLOT(jumpPage()));
}

void CCheckDeviceStatus::enter(QByteArray param)
{
    openTimer();
    myPage->readInfo();
    myPage->show();
}

void CCheckDeviceStatus::leave(QByteArray param)
{
    stopTimer();
    myPage->hide();
}


void CCheckDeviceStatus::resetStart()
{
    clearTimer();
}

void CCheckDeviceStatus::jumpPage()
{
    myPage->quitBtn_slots();
}

